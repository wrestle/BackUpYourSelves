#include "showFile.h"

/**
 ** ��ʾ��ǰ·���µ��ļ����ļ���
**/
void showAllFileInDir(const char * DirPath, int shownmode)
{
    /** �����þ��ʧ�ܣ��ٳ������� **/
    int times = 0;
    /** ����ʱ�ı�Ҫ���� **/
    HANDLE fileHandle;
    WIN32_FIND_DATAA fileData;
    LARGE_INTEGER fileSize;

    size_t length = strlen(DirPath);
    char dirBuf[SELF_MAX_PATH_LENGTH]; //·��
    char dirFileBuf[SELF_MAX_PATH_LENGTH]; //�ļ�

    /** ����һ��·�������ر����� **/
    strcpy(dirBuf, DirPath);

    /** Ԥ����·��������Windows������Ҫ�� **/
    if(length == 1) /** �������Ϊ�˵�·�����̷�ʱ�Ĵ��� **/
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, ":/");

        strcat(dirBuf, "://*");
    }
    else if(dirBuf[length-1] == '/') /** ������ֶ������·������������� / **/
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, "/");

        strcat(dirBuf, "/*");
    }
    else
    {
        strcpy(dirFileBuf, dirBuf);
        strcat(dirFileBuf, "/");

        strcat(dirBuf, "//*"); /** �ֶ����������󲻴� / �����ɺ�����õ�·�� **/
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
    /** �������·���µ������ļ����Լ��ļ����� **/
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
