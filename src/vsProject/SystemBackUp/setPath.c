﻿#include "setPath.h"

static char PathBuf[SELF_BU_PATH_MAX_SIZE] = SELF_LOAD_DEFAULT_PATH; //用于存放备份的路径

/** 获取和设置备份路径 **/
void getEnterPath()
{
    int intJudge = 0;
    char tmpBuf[SELF_BU_PATH_MAX_SIZE]; /**临时缓冲区**/
    printf("Enter The Path You want!\n");
    while(1)
    {
        fgets(tmpBuf, SELF_BU_PATH_MAX_SIZE*sizeof(char), stdin);
        if(strlen(tmpBuf) <= 1)
        {
            fprintf(stderr, "Your Enter is Empty, So Load the Default Path\n");
            fprintf(stderr, "%s \n", SELF_LOAD_DEFAULT_PATH);
            strcpy(tmpBuf, SELF_LOAD_DEFAULT_PATH);
        }
        sscanf(tmpBuf, "%s", PathBuf);

        fprintf(stdout, "Your Enter is \" %s \" ?(1 for yes, 0 for no) \n", PathBuf);

        fgets(tmpBuf, SELF_BU_PATH_MAX_SIZE*sizeof(char), stdin);
        sscanf(tmpBuf, "%d", &intJudge);
        if(intJudge)
        {
            if(PathBuf[strlen(PathBuf)-1] != '/')
                strcat(PathBuf, "/");
            storePathHistory(PathBuf);
            break;
        }
    }
    return;
}

/** 存储时格式为：路径 时间 **/
void storePathHistory(const char path[])
{
    time_t timep;
    time (&timep);

    FILE* input = Fopen("PathHistory.txt", "a");
    if(!input)
		return;

    fprintf(input, "%s %s", path, ctime(&timep));
    fclose(input);
    return;
}

void showPathHistory()
{
    char outBufName[SELF_BU_PATH_MAX_SIZE];

    FILE* reading = Fopen("PathHistory.txt", "r");
	if (!reading)
		return;

	for(int i = 1;i <= 10 && (!feof(reading));++i)
    {
        fgets(outBufName, SELF_BU_PATH_MAX_SIZE*sizeof(char), reading);
        fprintf(stdout, "%2d. %s",i , outBufName);
    }

    fclose(reading);
    system("Pause");
    return;
}

/** 返回当前备份存储路径 **/
const char * getBackUpPath()
{
    return PathBuf;
}

void replSymb(char * source)
{
	size_t len = strlen(source), i;
	for (i = 0; i <= len; ++i)
	{
		if (source[i] == '\\') // 两个饭斜杆是因为C语言中转义符号的需要。
			source[i] = '/';
	}
	return;
}
