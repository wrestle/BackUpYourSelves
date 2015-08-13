#include "showFile.h"

/**
 ** 显示当前路径下的文件与文件夹
**/
void showAllFileInDir(const char * DirPath, int shownmode)
{
    /** 如果获得句柄失败，再尝试三次 **/
    int times = 0;
    /** 操作时的必要参数 **/
    HANDLE fileHandle;
    WIN32_FIND_DATAA fileData;
    LARGE_INTEGER fileSize;

    size_t length = strlen(DirPath);
    char dirBuf[SELF_MAX_PATH_LENGTH]; //路径
    char dirFileBuf[SELF_MAX_PATH_LENGTH]; //文件

    /** 复制一份路径到本地变量上 **/
    strcpy(dirBuf, DirPath);

    /** 预处理路径，符合Windows函数的要求 **/
    if(length == 1) /** 该情况是为了当路径是盘符时的处理 **/
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, ":/");

        strcat(dirBuf, "://*");
    }
    else if(dirBuf[length-1] == '/') /** 如果是手动输入的路径可能最后会带有 / **/
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, "/");

        strcat(dirBuf, "/*");
    }
    else
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, "/");

        strcat(dirBuf, "//*"); /** 手动输入可能最后不带 / 或者由函数获得的路径 **/
    }

    setjmp(sAFID_jump);
    fileHandle = FindFirstFile(dirBuf, &fileData);
    if( fileHandle == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "The Handle getting Failure! \n");
        if(times++ < 2)
            longjmp(sAFID_jump, SELF_LONGJMP_RET_VALUE);
        else
            return;
    }
    fprintf(stdout, "-------------------------------------\n");
    /** 遍历输出路径下的所有文件名以及文件夹名 **/
    do{
        char tempFileODirBuf[SELF_MAX_FILE_LENGTH];
        strcpy(tempFileODirBuf, dirFileBuf);
        //fprintf(stdout, "handle is %p <<<<<<<<<\n", fileHandle);
        if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            _ftprintf(stdout, TEXT("  %s    \t<DIR>\t\n"), fileData.cFileName);
            if(strcmp(fileData.cFileName, ".") == 0 ||
               strcmp(fileData.cFileName, "..") == 0)
                    continue;
            strcat(tempFileODirBuf, fileData.cFileName);
            //fprintf(stdout, "strcat >>> %s <<<\n", tempFileODirBuf);
            showAllFileInDir(tempFileODirBuf, 1);
        }
        else
        {
            fileSize.LowPart  = fileData.nFileSizeLow;
            fileSize.HighPart = fileData.nFileSizeHigh;
            _ftprintf(stdout, TEXT("   %s    \t%ld bytes\t\n"), fileData.cFileName,
                                                 fileSize.QuadPart);
        }
    }while(FindNextFile(fileHandle, &fileData) != 0);
    fprintf(stdout, "************************************\n");
    FindClose(fileHandle);
}

/** ReadMe **/
void showReadMe()
{
    printf("The SoftWare is about BackUp System Which Create By \n");
    printf("WushengXin that wanna to Practice himself\n");
    printf("It can help you to BackUp your Files without doubt!\n");
    printf("Green, Free, OpenSource it is\n");
    printf("It can BackUp in a certain Directory that you want!\n");
    printf("To Use it: \n");
    printf("No Sale\n");
    printf("AnyThing You Want Because of the Source Code will OPEN\n");
    printf("\n Press Any Key Return To the Main Windows \n");
    system("Pause");
    return;
}

void replSymb(char * source)
{
    size_t len = strlen(source), i;
    for(i = 0;i <= len;++i)
    {
        if(source[i] == '\\')
            source[i] = '/';
    }
    return;
}
