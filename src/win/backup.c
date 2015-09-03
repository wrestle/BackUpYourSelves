#include "backup.h"
#define SELF_THREADS_LIMIT 5

vectors filesVec;
HANDLE vecEmpty, vecFull; //两个 Semaphore
HANDLE pushThread;  // 将路径加入队列中的线程
CRITICAL_SECTION inputSec, testSec;
HANDLE popMutex;

// 计算时间
clock_t start, finish;
double Total_time;

void showBUSelect()
{
    char tmpBuf[256];
    int selects;
	HANDLE copyThread[SELF_THREADS_LIMIT]; //复制文件的线程

	InitializeCriticalSection(&inputSec);
	InitializeCriticalSection(&testSec);
	vecEmpty = CreateSemaphore(NULL, 20, 20, NULL);
	vecFull = CreateSemaphore(NULL, 0, 20, NULL);
	//popMutex = CreateMutex(NULL, FALSE, NULL);

    do{
        system("cls");
        fprintf(stdout, "-------------------1. Back Up------------------ \n");
        fprintf(stdout, " For This Select, You can choose Three Options: \n");
        fprintf(stdout, " 1. Mode 0 (Current Directory >>> Now CANCEL!) \n");
        fprintf(stdout, " 2. Mode 1 (Root of Current Directory >>> Now CANCEL!)\n");
        fprintf(stdout, " 3. Mode 2 (The Directory Path That You Enter) \n");
        fprintf(stdout, " 4. Back To last level \n");
        fprintf(stdout, "----------------------------------------------- \n");
        fprintf(stdout, "Enter Your Selection: ");
        setjmp(forJmp);
        fgets(tmpBuf, 256, stdin);
        sscanf(tmpBuf, "%d", &selects);
        if(selects != 1 && selects != 2 &&
                selects != 3 && selects != 4)
        {
            fprintf(stdout, "\n Your Select \" %s \" is Invalid!\n Try Again: ", tmpBuf);
            longjmp(forJmp, 1);
        }

        memset(tmpBuf, 0, 256*sizeof(char));
		newVectors(&filesVec);  // 初始化文件队列

		
        switch(selects)
        {
			//int case_three;
        case 1: // 取消 case 1  和 case 2 的选项功能，因为实际上没有用处
			/*
			pushThread = (HANDLE)_beginthreadex(NULL, 0, callBackup, &selects, 0, NULL);
			for (int i = 0; i < 10; ++i)
			{
				copyThread[i] = (HANDLE)_beginthreadex(NULL, 0, callCopyFile, NULL, 0, NULL);
			}
			WaitForSingleObject(pushThread, INFINITE);
			WaitForMultipleObjects(20, copyThread, TRUE, INFINITE);
            break;
			*/
        case 2 :
			/*
			pushThread = (HANDLE)_beginthreadex(NULL, 0, callBackup, &selects, 0, NULL);
			for (int i = 0; i < 10; ++i)
			{
				copyThread[i] = (HANDLE)_beginthreadex(NULL, 0, callCopyFile, NULL, 0, NULL);
			}
			WaitForSingleObject(pushThread, INFINITE);
			WaitForMultipleObjects(20, copyThread, TRUE, INFINITE);
            break;
			*/
        case 3 :
			//case_three = selects;
			pushThread = (HANDLE)_beginthreadex(NULL, 0, callBackup, &selects, 0, NULL);
			for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			{
				copyThread[i] = (HANDLE)_beginthreadex(NULL, 0, callCopyFile, NULL, 0, NULL);
			}
			WaitForSingleObject(pushThread, INFINITE);
			WaitForMultipleObjects(SELF_THREADS_LIMIT, copyThread, TRUE, INFINITE);
			printf("All Thread Exit!\n");
            break;
        case 4 :
            return;
        }

		// 计算时间
		finish = clock();
		Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("Time cost is %f s\n", Total_time);
		
		// 销毁线程使用过的句柄
		CloseHandle(pushThread);
		for (int i = 0; i < SELF_THREADS_LIMIT; ++i)
			CloseHandle(copyThread[i]);
		//释放 filesVec
		filesVec.Delete(&filesVec);
		system("Pause");
    }while(1);
    return;
}

void backup(int mode, const char* path, const char* bupath)
{
    //操作文件参数
    HANDLE fileHandle;
    WIN32_FIND_DATAA fileData;
	int len = strlen(path);

    char backupFrom[SELF_BU_PATH_MAX_SIZE];  // 需要备份的位置
    char backupTo[SELF_BU_PATH_MAX_SIZE];      // 存储备份的位置
    char dirBuf[SELF_BU_PATH_MAX_SIZE];           //  临时路径
   
     strcpy(backupFrom, path);
     if(len == 1)
         strcat(backupFrom, ":/"); // C
     else if(backupFrom[len-1] != '/')
         strcat(backupFrom, "/");  // C:/dir/subdir

    strcpy(dirBuf, backupFrom);
    strcat(dirBuf, "/*");
    strcpy(backupTo, bupath);
    if(backupTo[strlen(backupTo)-1] != '/')
        strcat(backupTo, "/");  //在递归的时候，需要向路径末尾添加 /

    fileHandle = FindFirstFile(dirBuf, &fileData);
    if( fileHandle == INVALID_HANDLE_VALUE)
    {
#if !defined(NOT_DEBUG)
     
		fprintf(stderr, "The Handle getting Failure!\n"
                            "  AT %s in %d lines\n", __FILE__, __LINE__);
        system("Pause");
#endif
        return;
    }
 
    do{
        char tempFileDirBuf[SELF_BU_PATH_MAX_SIZE];
        char tempBackUpPath[SELF_BU_PATH_MAX_SIZE];
        strcpy(tempBackUpPath, backupTo);
        strcpy(tempFileDirBuf, backupFrom); //拷贝一份无 '/*' 结尾的路径用于补齐完整路径
        if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(strcmp(fileData.cFileName, ".") == 0 ||
               strcmp(fileData.cFileName, "..") == 0)
                    continue;
            strcat(tempFileDirBuf, fileData.cFileName);
            strcat(tempBackUpPath, fileData.cFileName);

			if (_access(tempFileDirBuf, 0) != -1)
#if !defined(NOT_DEBUG)
				if (!CreateDirectory(tempBackUpPath, NULL))
					fprintf(stderr, "ERROR CODE >> %d \n", GetLastError());
#else
				CreateDirectory(tempBackUpPath, NULL);
#endif
            backup(2, tempFileDirBuf, tempBackUpPath);
        }
        else
        {
            strcat(tempFileDirBuf, fileData.cFileName);
            strcat(tempBackUpPath, fileData.cFileName);
			if (_access(tempBackUpPath, 0) == 0) /*如果目标文件存在*/
			{
				if (is_changed(tempBackUpPath, tempFileDirBuf))
				{
#if !defined(NOT_DEBUG)
					fprintf(stderr, "File : %s hasn't changed!\n", tempFileDirBuf);
#endif
					continue;  /*如果目标文件与源文件的修改时间相同，则不需要入队列*/
				}
			}
			WaitForSingleObject(vecEmpty, INFINITE);

			filesVec.PushBack(&filesVec, tempFileDirBuf, tempBackUpPath);
			ReleaseSemaphore(vecFull, 1, NULL);
        }
    }while(FindNextFile(fileHandle, &fileData) != 0);
    FindClose(fileHandle);
    
    return;
}

int is_changed(const char * dstfile, const char * srcfile)
{
	struct stat dst_stat, src_stat;
	stat(dstfile, &dst_stat);
	stat(srcfile, &src_stat);
	return dst_stat.st_mtime == src_stat.st_mtime;
}

unsigned int __stdcall callBackup(void * pSelect)
{
	int Select = *(int *)pSelect;

	if (Select == 3)
	{
		char tmpBuf[SELF_BU_PATH_MAX_SIZE], tmpPath[SELF_BU_PATH_MAX_SIZE];

		fprintf(stdout, " Enter the Full Path You want to BackUp\n");
		fgets(tmpBuf, 256, stdin);
		sscanf(tmpBuf, "%s", tmpPath);
		start = clock();
		backup(2, tmpPath, getBackUpPath());
	}
	else
	{
		backup(Select, NULL, getBackUpPath());
	}

	return 0;
}

unsigned int __stdcall callCopyFile(void * para)
{
	DWORD isExit;
	vectors*   address = &filesVec;
	combine* localCom;
	int empty;
	while (1)
	{
		char * dst_path = NULL;
		char * src_path = NULL;
		EnterCriticalSection(&testSec);
		GetExitCodeThread(pushThread, &isExit);
		empty = address->emptys;
		LeaveCriticalSection(&testSec);
		if (isExit != STILL_ACTIVE && empty)
		{
			puts("Push Thread is End!\n");
			break;
		}
		
		isExit = WaitForSingleObject(vecFull, 30000);
		if (isExit == WAIT_TIMEOUT)
			break;

		EnterCriticalSection(&inputSec); // 这个关键段的添加十分重要，是读取时候的核心
		//WaitForSingleObject(popMutex, INFINITE);
		if(!(localCom = filesVec.PopFront(address)))
			continue;
		//ReleaseMutex(popMutex);
		LeaveCriticalSection(&inputSec);

		dst_path = localCom->dstPath;
		src_path = localCom->srcPath;

#if !defined(NOT_DEBUG)
		if (CopyFile(src_path, dst_path, FALSE) == 0)
		{
			EnterCriticalSection(&inputSec);
			fprintf(stderr, "<<<<<Error Code>>>>>>%d \n", GetLastError());
			LeaveCriticalSection(&inputSec);
		}
#else
			CopyFile(src_path, dst_path, FALSE);
#endif
		Free(src_path);
		Free(dst_path);
		Free(localCom);
		ReleaseSemaphore(vecEmpty, 1, NULL);
	}
	return 0;
}