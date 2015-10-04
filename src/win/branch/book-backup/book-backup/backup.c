#include "backup.h"
#define SELF_THREADS_LIMIT 4

static queue filesVec;            /* 队列主体 */
HANDLE vecEmpty, vecFull; /* 两个 Semaphore */
HANDLE pushThread;       /* 将路径加入队列中的线程 */
HANDLE copyThread[SELF_THREADS_LIMIT]; /* 将路径弹出队列并复制的线程 */
CRITICAL_SECTION inputSec, testSec;

const char * get_backup_topath(); /* setPath.h */
void repl_str(char * src); /* setPath.h */

/* 计算时间 */
static clock_t start, finish; /* 备份开始，结束时间 */
static double Total_time;    /* 计算备份花费时间 */

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    dstfile 目的路径的文件
			 srcfile 源路径的文件
 * @function 判断两个文件的最后修改时间是否相同
 */
static int is_changed(const char * dstfile, const char * srcfile)
{
	struct stat dst_stat, src_stat;
	stat(dstfile, &dst_stat);
	stat(srcfile, &src_stat);
	return dst_stat.st_mtime == src_stat.st_mtime;
}

/**
 * @version 1.0 2015/10/03
 * @author  wushengxin
 * @param   pSelect 传入的参数，可以是NULL
 * function 作为线程开始函数的一个参数，作用是调用 backup 函数
 */
static unsigned int __stdcall callBackup(void * pSelect)
{
	char* tmpPath = (char*)pSelect;
	start = clock();
	backup(tmpPath, get_backup_topath());
	return 0;
}
/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    para 此处的参数没有使用
 * @function 用于将队列中的路径模型弹出，复制，释放
 */
static unsigned int __stdcall callCopyFile(void * para)
{
	DWORD    isExit   = 0;         /* 判断入队线程是否还存在 */
	queue*   address  = &filesVec;
	combine* localCom = NULL;
	int      empty    = 0;
	while (1)
	{
		char * dst_path = NULL;
		char * src_path = NULL;
		EnterCriticalSection(&testSec);
		GetExitCodeThread(pushThread, &isExit); /* 查看入队的线程是否已经结束 */
		empty = address->empty; /* 查看此时队列是否为空 */
		LeaveCriticalSection(&testSec);
		if (isExit != STILL_ACTIVE && empty) /* STILL_ACTIVE 代表还在运行 */
		{
			puts("Push Thread is End!\n");
			break;   
		}

		isExit = WaitForSingleObject(vecFull, 3000); /* 设定一个等待时间，以防死锁 */
		if (isExit == WAIT_TIMEOUT)
		{
			fprintf(stderr, "Copy Thread wait time out!\n");
			continue;
		}

		EnterCriticalSection(&inputSec); /* 这个关键段的添加十分重要，是读取时候的核心 */
		if (!(localCom = filesVec.PopFront(address))) /* 每次弹出时一定要防止资源争夺带来的冲突 */
			continue;
		LeaveCriticalSection(&inputSec);

		dst_path = localCom->dst_to_path;
		src_path = localCom->src_from_path;

		if (CopyFileA(src_path, dst_path, FALSE) == 0) /* 显式使用 CopyFileA 函数，而不是使用 CopyFile 宏 */
		{
			EnterCriticalSection(&inputSec); /* 这一段不添加也是合理的，只是不清楚为何不复制而已 */
			if (ERROR_ACCESS_DENIED == GetLastError())
			{
				fprintf(stderr, "\nThe File has already existed and is HIDDEN or ReadOnly! \n");
				fprintf(stderr, "Copy File from %s Fail!\n", src_path);
			}
			else if (ERROR_ENCRYPTION_FAILED == GetLastError())
			{
				fprintf(stderr, "\nThe File is Encrypted(被加密), And Can't not be copy\n");
				fprintf(stderr, "Copy File from %s Fail!\n", src_path);
			}
			LeaveCriticalSection(&inputSec);
		}
		Free_s(src_path);
		Free_s(dst_path);
		Free_s(localCom);
		ReleaseSemaphore(vecEmpty, 1, NULL); /* 是放一个信号量 */
	}
	return 0;
}

void sec_main_windows()
{
	char tmpBuf[256];
	int selects;
	do{
		InitializeCriticalSection(&inputSec); /* 每次使用前必须初始化的四个变量 */
		InitializeCriticalSection(&testSec);
		vecEmpty = CreateSemaphoreA(NULL, 20, 20, NULL); /* 显式使用 A 版本 */
		vecFull = CreateSemaphoreA(NULL, 0, 20, NULL);

		setjmp(select_jmp);
		system("cls");
		puts("-------------------1. Back Up------------------ ");
		puts(" For This Select, You can choose Two Options: ");
		puts(" 1. Start Back up (The Directory Path That You Enter LATER) ");
		puts(" 2. Back To last level ");
		puts("----------------------------------------------- ");
		fprintf(stdout, "Enter Your Selection: ");
		fgets(tmpBuf, 256, stdin);
		sscanf(tmpBuf, "%d", &selects);
		if (selects != 1 && selects != 2 )
		{
			fprintf(stdout, "\n Your Select \" %s \" is Invalid!\n Try Again \n", tmpBuf);
			longjmp(select_jmp, 1);
		}
		newQueue(&filesVec);  // 初始化文件队列
		switch (selects)
		{
			jmp_buf enter_path_jmp; 
		case 1:
		{
			char tmpBuf[MAX_ENTER_PATH], tmpPath[MAX_ENTER_PATH]; /* 使用栈分配空间，因为只用分配一次 */
			setjmp(enter_path_jmp);         /* enter jump to there */
			puts(" Enter the Full Path You want to BackUp(e.g: C:/Programing/)");
			fprintf(stdout, " Or Enter q to back to select\nYour Enter : ");
			fgets(tmpBuf, MAX_ENTER_PATH, stdin);
			sscanf(tmpBuf, "%s", tmpPath);
			if (_access(tmpPath, 0) != 0)   /*检查路径是否存在，有效*/
			{
				if (tmpPath[0] == 'q' || tmpPath[0] == 'Q') 
					longjmp(select_jmp, 0); /* 回到可以选择返回的界面 */
				fprintf(stderr, "The Path You Enter is Not Exit! \n Try Again : ");
				longjmp(enter_path_jmp, 0); /* enter jump from here */
			}
			/* 这里调用线程创建函数 _beginthreadex,各个参数的意义可以上网查询 */
			pushThread = (HANDLE)_beginthreadex(NULL, 0, callBackup, (void*)tmpPath, 0, NULL);
			for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			{
				copyThread[i] = (HANDLE)_beginthreadex(NULL, 0, callCopyFile, NULL, 0, NULL);
			}
			WaitForSingleObject(pushThread, INFINITE); /* 等待线程结束 */
			WaitForMultipleObjects(SELF_THREADS_LIMIT, copyThread, TRUE, INFINITE);
			printf("All Thread Exit!\n");
		}
			break;
		case 2:
			return;
		default:
			break;
		}
		/* 计算时间 */
		finish = clock();
		Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("Time cost is %f s\n", Total_time);

		/* 销毁使用的 CS */
		DeleteCriticalSection(&inputSec);
		DeleteCriticalSection(&testSec);
		/* 销毁信号量 */
		CloseHandle(vecEmpty);
		CloseHandle(vecFull);
		/* 销毁线程使用过的句柄 */
		CloseHandle(pushThread);
		for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			CloseHandle(copyThread[i]);
		//释放 filesVec
		filesVec.Delete(&filesVec);
		system("Pause");
	} while (1);
	return;
}

char * make_path(const char * src)
{
	int times = 0;
	size_t len_of_src = strlen(src);
	size_t len_of_dst = MIN_PATH_NAME;

	if (len_of_src > MIN_PATH_NAME - 10) /* \\?\ //* 8个字符 */
	{
		len_of_dst = LARGEST_PATH_NAME;
		if (len_of_src > LARGEST_PATH_NAME - 10)
		{
			fprintf(stderr, "The Path Name is larger than 32767, Which is not Support!\n%s", src);
			return NULL;
		}
	}
	setjmp(alloc_jmp); /* alloc_jmp to here */
	char * loc_buf = malloc(len_of_dst + 1);
	if (loc_buf == NULL)
	{
		fprintf(stderr, "ERROR OCCUR When malloc the memory at %s\n Try the %d th times", __LINE__, times + 1);
		if (times++ < TRY_TIMES)
			longjmp(alloc_jmp, 0); /* alloc_jmp from here */
		return NULL;
	}
	//sprintf(loc_buf, "\\\\?\\%s", src); /* 前方已经确保过不会超过范围，故不必使用故意 snprintf */
	strcpy(loc_buf, src);
	return loc_buf;
}

void adjust_path(char * src_path, char * src_file)
{
	size_t length = strlen(src_file); /* 两个参数的长度在此函数调用之前必定一致 */
	if (src_path != NULL)
	{
		if (length == 1) /* 处理情况为，当用户输入的是根目录的情况 例如: C */
		{
			strcat(src_file, ":/");
		}
		else if (src_path[length - 1] != '\\' && src_path[length - 1] != '/')
		{
			strcat(src_file, "/");
		}
		else
		{
			src_path[length - 1] = '/';
		}
		strcpy(src_path, src_file);
		strcat(src_path, "/*");
	}
	else
	{
		if (src_file[length - 1] != '/')
		strcat(src_file, "/");
	}
	return;
}


void rele_path(char * src)
{
	free(src);
	src = NULL;
	return;
}

void backup(const char * path, const char * bpath)
{
	/* 用来配合 setjmp和longjmp重新获取句柄HANDLE的变量 */
	int times = 0;
	/** 操作时获取文件夹，文件信息的的必要变量,为了WIN API区别，这里变量的命名使用驼峰法 **/
	HANDLE fileHandle;
	WIN32_FIND_DATAA fileData;

	char * from_path_buf = make_path(path);
	char * to_path_buf = make_path(bpath);
	char * find_path_buf = make_path(path); /* 用于 Windows API FindFirstFile */

	if (from_path_buf == NULL || to_path_buf == NULL || find_path_buf == NULL)
	{
		rele_path(from_path_buf); /* 释放 */
		rele_path(to_path_buf);
		rele_path(find_path_buf);
		return;
	}
	adjust_path(find_path_buf, from_path_buf); /* 调整路径到标准格式 */
	adjust_path(NULL, to_path_buf);
	repl_str(from_path_buf);
	repl_str(to_path_buf);
	repl_str(find_path_buf);
	//	printf("After adjust \n%s \n%s\n", dir_buf, dir_file_buf);
	/* 开始调用 Windows API 获取路径下的文件和文件夹信息 */
	setjmp(get_hd_jmp);
	fileHandle = FindFirstFileA(find_path_buf, &fileData);
	if (fileHandle == INVALID_HANDLE_VALUE) /* 如果无法获取句柄超过上限次数，就退出 */
	{
		fprintf(stderr, "The Handle getting Failure! \n");
		if (times++ < TRY_TIMES)
			longjmp(get_hd_jmp, 0);
		rele_path(from_path_buf); /* 释放 */
		rele_path(to_path_buf);
		rele_path(find_path_buf);
		return;
	}
	do{
		char * tmp_from_file_buf = make_path(from_path_buf); /* 不完整的源路径 */
		char * tmp_to_file_buf = make_path(to_path_buf); /* 不完整的目的路径 */
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//fprintf(stderr, "%*s%s\t<DIR>\t\n", depth, "", fileData.cFileName);
			if (strcmp(fileData.cFileName, ".") == 0 ||  /* . 和 .. 便是当前文件夹和上一级文件夹, 世界上所有系统都一样 */
				strcmp(fileData.cFileName, "..") == 0)
			{
				rele_path(tmp_from_file_buf);
				rele_path(tmp_to_file_buf);
				continue;
			}
			strcat(tmp_from_file_buf, fileData.cFileName); /* 将文件名连接到当前文件夹路径之后，形成文件路径 */
			strcat(tmp_to_file_buf, fileData.cFileName);
			fprintf(stderr, "Found dir %s \n", tmp_from_file_buf);
			fprintf(stderr, "Confirm dir %s \n", tmp_to_file_buf);
			if (_access(tmp_to_file_buf, 0) == -1)      /* 如果目录不存在 */
			{
				fprintf(stderr, ">>The dir not exist %s \n", tmp_to_file_buf);
				CreateDirectoryA(tmp_to_file_buf, NULL);
			}
			backup(tmp_from_file_buf, tmp_to_file_buf);
		}
		else
		{
			strcat(tmp_from_file_buf, fileData.cFileName);
			strcat(tmp_to_file_buf, fileData.cFileName);
			if (_access(tmp_to_file_buf, 0) == 0) /*如果目标文件存在*/
			{
				if (is_changed(tmp_from_file_buf, tmp_to_file_buf))
				{
					rele_path(tmp_from_file_buf);
					rele_path(tmp_to_file_buf);
					continue;  /*如果目标文件与源文件的修改时间相同，则不需要入队列*/
				}
				fprintf(stderr, "File : %s hast changed!\n", tmp_from_file_buf);
			}
			else
				fprintf(stderr, "Add New File %s \n", tmp_from_file_buf);

			WaitForSingleObject(vecEmpty, INFINITE);

			filesVec.PushBack(&filesVec, tmp_from_file_buf, tmp_to_file_buf);
			ReleaseSemaphore(vecFull, 1, NULL);
		}
		rele_path(tmp_from_file_buf);
		rele_path(tmp_to_file_buf);
	} while (FindNextFileA(fileHandle, &fileData));
	FindClose(fileHandle);
	rele_path(from_path_buf);
	rele_path(to_path_buf);
	rele_path(find_path_buf);
	return;
}