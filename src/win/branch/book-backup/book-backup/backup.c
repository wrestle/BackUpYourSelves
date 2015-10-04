#include "backup.h"
#define SELF_THREADS_LIMIT 4

static queue filesVec;            /* �������� */
HANDLE vecEmpty, vecFull; /* ���� Semaphore */
HANDLE pushThread;       /* ��·����������е��߳� */
HANDLE copyThread[SELF_THREADS_LIMIT]; /* ��·���������в����Ƶ��߳� */
CRITICAL_SECTION inputSec, testSec;

const char * get_backup_topath(); /* setPath.h */
void repl_str(char * src); /* setPath.h */

/* ����ʱ�� */
static clock_t start, finish; /* ���ݿ�ʼ������ʱ�� */
static double Total_time;    /* ���㱸�ݻ���ʱ�� */

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    dstfile Ŀ��·�����ļ�
			 srcfile Դ·�����ļ�
 * @function �ж������ļ�������޸�ʱ���Ƿ���ͬ
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
 * @param   pSelect ����Ĳ�����������NULL
 * function ��Ϊ�߳̿�ʼ������һ�������������ǵ��� backup ����
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
 * @param    para �˴��Ĳ���û��ʹ��
 * @function ���ڽ������е�·��ģ�͵��������ƣ��ͷ�
 */
static unsigned int __stdcall callCopyFile(void * para)
{
	DWORD    isExit   = 0;         /* �ж�����߳��Ƿ񻹴��� */
	queue*   address  = &filesVec;
	combine* localCom = NULL;
	int      empty    = 0;
	while (1)
	{
		char * dst_path = NULL;
		char * src_path = NULL;
		EnterCriticalSection(&testSec);
		GetExitCodeThread(pushThread, &isExit); /* �鿴��ӵ��߳��Ƿ��Ѿ����� */
		empty = address->empty; /* �鿴��ʱ�����Ƿ�Ϊ�� */
		LeaveCriticalSection(&testSec);
		if (isExit != STILL_ACTIVE && empty) /* STILL_ACTIVE ���������� */
		{
			puts("Push Thread is End!\n");
			break;   
		}

		isExit = WaitForSingleObject(vecFull, 3000); /* �趨һ���ȴ�ʱ�䣬�Է����� */
		if (isExit == WAIT_TIMEOUT)
		{
			fprintf(stderr, "Copy Thread wait time out!\n");
			continue;
		}

		EnterCriticalSection(&inputSec); /* ����ؼ��ε����ʮ����Ҫ���Ƕ�ȡʱ��ĺ��� */
		if (!(localCom = filesVec.PopFront(address))) /* ÿ�ε���ʱһ��Ҫ��ֹ��Դ��������ĳ�ͻ */
			continue;
		LeaveCriticalSection(&inputSec);

		dst_path = localCom->dst_to_path;
		src_path = localCom->src_from_path;

		if (CopyFileA(src_path, dst_path, FALSE) == 0) /* ��ʽʹ�� CopyFileA ������������ʹ�� CopyFile �� */
		{
			EnterCriticalSection(&inputSec); /* ��һ�β����Ҳ�Ǻ���ģ�ֻ�ǲ����Ϊ�β����ƶ��� */
			if (ERROR_ACCESS_DENIED == GetLastError())
			{
				fprintf(stderr, "\nThe File has already existed and is HIDDEN or ReadOnly! \n");
				fprintf(stderr, "Copy File from %s Fail!\n", src_path);
			}
			else if (ERROR_ENCRYPTION_FAILED == GetLastError())
			{
				fprintf(stderr, "\nThe File is Encrypted(������), And Can't not be copy\n");
				fprintf(stderr, "Copy File from %s Fail!\n", src_path);
			}
			LeaveCriticalSection(&inputSec);
		}
		Free_s(src_path);
		Free_s(dst_path);
		Free_s(localCom);
		ReleaseSemaphore(vecEmpty, 1, NULL); /* �Ƿ�һ���ź��� */
	}
	return 0;
}

void sec_main_windows()
{
	char tmpBuf[256];
	int selects;
	do{
		InitializeCriticalSection(&inputSec); /* ÿ��ʹ��ǰ�����ʼ�����ĸ����� */
		InitializeCriticalSection(&testSec);
		vecEmpty = CreateSemaphoreA(NULL, 20, 20, NULL); /* ��ʽʹ�� A �汾 */
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
		newQueue(&filesVec);  // ��ʼ���ļ�����
		switch (selects)
		{
			jmp_buf enter_path_jmp; 
		case 1:
		{
			char tmpBuf[MAX_ENTER_PATH], tmpPath[MAX_ENTER_PATH]; /* ʹ��ջ����ռ䣬��Ϊֻ�÷���һ�� */
			setjmp(enter_path_jmp);         /* enter jump to there */
			puts(" Enter the Full Path You want to BackUp(e.g: C:/Programing/)");
			fprintf(stdout, " Or Enter q to back to select\nYour Enter : ");
			fgets(tmpBuf, MAX_ENTER_PATH, stdin);
			sscanf(tmpBuf, "%s", tmpPath);
			if (_access(tmpPath, 0) != 0)   /*���·���Ƿ���ڣ���Ч*/
			{
				if (tmpPath[0] == 'q' || tmpPath[0] == 'Q') 
					longjmp(select_jmp, 0); /* �ص�����ѡ�񷵻صĽ��� */
				fprintf(stderr, "The Path You Enter is Not Exit! \n Try Again : ");
				longjmp(enter_path_jmp, 0); /* enter jump from here */
			}
			/* ��������̴߳������� _beginthreadex,�����������������������ѯ */
			pushThread = (HANDLE)_beginthreadex(NULL, 0, callBackup, (void*)tmpPath, 0, NULL);
			for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			{
				copyThread[i] = (HANDLE)_beginthreadex(NULL, 0, callCopyFile, NULL, 0, NULL);
			}
			WaitForSingleObject(pushThread, INFINITE); /* �ȴ��߳̽��� */
			WaitForMultipleObjects(SELF_THREADS_LIMIT, copyThread, TRUE, INFINITE);
			printf("All Thread Exit!\n");
		}
			break;
		case 2:
			return;
		default:
			break;
		}
		/* ����ʱ�� */
		finish = clock();
		Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("Time cost is %f s\n", Total_time);

		/* ����ʹ�õ� CS */
		DeleteCriticalSection(&inputSec);
		DeleteCriticalSection(&testSec);
		/* �����ź��� */
		CloseHandle(vecEmpty);
		CloseHandle(vecFull);
		/* �����߳�ʹ�ù��ľ�� */
		CloseHandle(pushThread);
		for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			CloseHandle(copyThread[i]);
		//�ͷ� filesVec
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

	if (len_of_src > MIN_PATH_NAME - 10) /* \\?\ //* 8���ַ� */
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
	//sprintf(loc_buf, "\\\\?\\%s", src); /* ǰ���Ѿ�ȷ�������ᳬ����Χ���ʲ���ʹ�ù��� snprintf */
	strcpy(loc_buf, src);
	return loc_buf;
}

void adjust_path(char * src_path, char * src_file)
{
	size_t length = strlen(src_file); /* ���������ĳ����ڴ˺�������֮ǰ�ض�һ�� */
	if (src_path != NULL)
	{
		if (length == 1) /* �������Ϊ�����û�������Ǹ�Ŀ¼����� ����: C */
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
	/* ������� setjmp��longjmp���»�ȡ���HANDLE�ı��� */
	int times = 0;
	/** ����ʱ��ȡ�ļ��У��ļ���Ϣ�ĵı�Ҫ����,Ϊ��WIN API�����������������ʹ���շ巨 **/
	HANDLE fileHandle;
	WIN32_FIND_DATAA fileData;

	char * from_path_buf = make_path(path);
	char * to_path_buf = make_path(bpath);
	char * find_path_buf = make_path(path); /* ���� Windows API FindFirstFile */

	if (from_path_buf == NULL || to_path_buf == NULL || find_path_buf == NULL)
	{
		rele_path(from_path_buf); /* �ͷ� */
		rele_path(to_path_buf);
		rele_path(find_path_buf);
		return;
	}
	adjust_path(find_path_buf, from_path_buf); /* ����·������׼��ʽ */
	adjust_path(NULL, to_path_buf);
	repl_str(from_path_buf);
	repl_str(to_path_buf);
	repl_str(find_path_buf);
	//	printf("After adjust \n%s \n%s\n", dir_buf, dir_file_buf);
	/* ��ʼ���� Windows API ��ȡ·���µ��ļ����ļ�����Ϣ */
	setjmp(get_hd_jmp);
	fileHandle = FindFirstFileA(find_path_buf, &fileData);
	if (fileHandle == INVALID_HANDLE_VALUE) /* ����޷���ȡ����������޴��������˳� */
	{
		fprintf(stderr, "The Handle getting Failure! \n");
		if (times++ < TRY_TIMES)
			longjmp(get_hd_jmp, 0);
		rele_path(from_path_buf); /* �ͷ� */
		rele_path(to_path_buf);
		rele_path(find_path_buf);
		return;
	}
	do{
		char * tmp_from_file_buf = make_path(from_path_buf); /* ��������Դ·�� */
		char * tmp_to_file_buf = make_path(to_path_buf); /* ��������Ŀ��·�� */
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//fprintf(stderr, "%*s%s\t<DIR>\t\n", depth, "", fileData.cFileName);
			if (strcmp(fileData.cFileName, ".") == 0 ||  /* . �� .. ���ǵ�ǰ�ļ��к���һ���ļ���, ����������ϵͳ��һ�� */
				strcmp(fileData.cFileName, "..") == 0)
			{
				rele_path(tmp_from_file_buf);
				rele_path(tmp_to_file_buf);
				continue;
			}
			strcat(tmp_from_file_buf, fileData.cFileName); /* ���ļ������ӵ���ǰ�ļ���·��֮���γ��ļ�·�� */
			strcat(tmp_to_file_buf, fileData.cFileName);
			fprintf(stderr, "Found dir %s \n", tmp_from_file_buf);
			fprintf(stderr, "Confirm dir %s \n", tmp_to_file_buf);
			if (_access(tmp_to_file_buf, 0) == -1)      /* ���Ŀ¼������ */
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
			if (_access(tmp_to_file_buf, 0) == 0) /*���Ŀ���ļ�����*/
			{
				if (is_changed(tmp_from_file_buf, tmp_to_file_buf))
				{
					rele_path(tmp_from_file_buf);
					rele_path(tmp_to_file_buf);
					continue;  /*���Ŀ���ļ���Դ�ļ����޸�ʱ����ͬ������Ҫ�����*/
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