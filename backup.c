#include "backup.h"

void showBUSelect()
{
    char tmpBuf[256];
    char tmpPath[256];

    int selects;
    do{
        system("cls");
        fprintf(stdout, "-------------------1. Back Up------------------ \n");
        fprintf(stdout, " For This Select, You can choose Three Options: \n");
        fprintf(stdout, " 1. Mode 0 (Current Directory) \n");
        fprintf(stdout, " 2. Mode 1 (Root of Current Directory \n");
        fprintf(stdout, " 3. Mode 2 (The Directory Path That You Enter) \n");
        fprintf(stdout, " 4. Back To last level \n");
        fprintf(stdout, "----------------------------------------------- \n");
        fprintf(stdout, "Enter Your Selection: ");
        setjmp(&forJmp);
        fgets(tmpBuf, 256, stdin);
        sscanf(tmpBuf, "%d", &selects);
        if(selects != 1 && selects != 2 &&
                selects != 3 && selects != 4)
        {
            fprintf(stdout, "\n Your Select \" %s \" is Invalid!\n Try Again: ", tmpBuf);
            longjmp(&forJmp, 1);
        }

        memset(tmpBuf, 0, 256*sizeof(char));
        switch(selects)
        {
        case 1:
            backup(0, NULL, getBackUpPath());
            break;
        case 2 :
            backup(1, NULL, getBackUpPath());
            break;
        case 3 :
            fprintf(stdout, " Enter the Full Path You want to BackUp\n");
            fgets(tmpBuf, 256, stdin);
            sscanf(tmpBuf, "%s", tmpPath);
            LARGE_INTEGER l1, l2;
            LARGE_INTEGER m_liPerfStart;
            int time;
            QueryPerformanceFrequency(&l1);
            QueryPerformanceCounter(&m_liPerfStart);

            backup(2, tmpPath, getBackUpPath());
            QueryPerformanceCounter(&l2);
            time=( ((l2.QuadPart - m_liPerfStart.QuadPart) * 1000)/l1.QuadPart);

            printf("Time cost is %d ms", time);
            printf("Press ANY KEY to Return\n");
            system("Pause");
            break;
        case 4 :
            return;
        }
    }while(1);
    return;
}

void backup(int mode, const char* path, const char* bupath)
{
    //操作文件参数
    HANDLE fileHandle;
    WIN32_FIND_DATAA fileData;

    vectors filesVec;
    vectors dirsVec;
    char backupFrom[SELF_BU_PATH_MAX_SIZE];
    char backupTo[SELF_BU_PATH_MAX_SIZE];
    char dirBuf[SELF_BU_PATH_MAX_SIZE];

    if(!mode)/** 路径预处理 **/
    {
        GetCurrentDirectory(SELF_BU_PATH_MAX_SIZE, backupFrom); // C:/dir
        replSymb(backupFrom); // '\' 替换为 '/'
        strcat(backupFrom, "/");
    }
    else if(mode == 1)
    {
        GetCurrentDirectoryW(SELF_BU_PATH_MAX_SIZE, backupFrom); // C
        strcat(backupFrom, ":/");
    }
    else
    {
        int len = strlen(path);
        strcpy(backupFrom, path);
        if(len == 1)
            strcat(backupFrom, ":/"); // C
        else if(backupFrom[len-1] != '/')
            strcat(backupFrom, "/");  // C:/dir/subdir
    }
    strcpy(dirBuf, backupFrom);
    strcat(dirBuf, "/*");
    strcpy(backupTo, bupath);
    if(backupTo[strlen(backupTo)-1] != '/')
        strcat(backupTo, "/");  //在递归的时候，需要向路径末尾添加 /

    fileHandle = FindFirstFile(dirBuf, &fileData);
    if( fileHandle == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "The Handle getting Failure!\n"
                            "  AT %s in %d lines\n", __FILE__, __LINE__);
        system("Pause");
        return;
    }
    //newVectors(&filesVec);
    //newVectors(&dirsVec);

    do{
        char tempFileDirBuf[SELF_BU_PATH_MAX_SIZE];
        char tempBackUpPath[SELF_BU_PATH_MAX_SIZE];
        strcpy(tempBackUpPath, backupTo);
        strcpy(tempFileDirBuf, backupFrom); //拷贝一份无 '/*' 结尾的路径用于补齐完整路径
        //fprintf(stdout, "handle is %p <<<<<<<<<\n", fileHandle);
        if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(strcmp(fileData.cFileName, ".") == 0 ||
               strcmp(fileData.cFileName, "..") == 0)
                    continue;
            strcat(tempFileDirBuf, fileData.cFileName);
            strcat(tempBackUpPath, fileData.cFileName);
           // fprintf(stdout, "Found Direction >>> %s <<<\n", tempFileDirBuf);
           // fprintf(stdout, "Create Direction >>> %s <<<\n", tempBackUpPath);
           // dirsVec.PushBack(&dirsVec, tempFileDirBuf);
            //showAllFileInDir(tempFileODirBuf, 1);
            if(access(tempFileDirBuf, 0) != -1)
                if(!CreateDirectory(tempBackUpPath, NULL))
                    fprintf(stderr, "ERROR CODE >> %d \n", GetLastError());
            backup(2, tempFileDirBuf, tempBackUpPath);
        }
        else
        {
            strcat(tempFileDirBuf, fileData.cFileName);
            strcat(tempBackUpPath, fileData.cFileName);
            if(!CopyFile(tempFileDirBuf, tempBackUpPath, FALSE))
                fprintf(stderr ,"<<<<<Error Code>>>>>>%d \n", GetLastError());
            //filesVec.PushBack(&filesVec, tempFileDirBuf);

        }
    }while(FindNextFile(fileHandle, &fileData) != 0);
    FindClose(fileHandle);

   // filesVec.Delete(&filesVec);
   // dirsVec.Delete(&dirsVec);
    //system("Pause");
    return;
}
