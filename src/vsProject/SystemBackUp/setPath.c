#include "setPath.h"

static char PathBuf[SELF_BU_PATH_MAX_SIZE] = SELF_LOAD_DEFAULT_PATH; //用于存放备份的路径

void initPath()
{
	int len = 0;
	char last_path[SELF_BU_PATH_MAX_SIZE] = {0};
	FILE* hist_file = Fopen("LastPath.conf", "r");
	if (!hist_file) /* 打开失败则不初始化 */
		return;
	fgets(last_path, SELF_BU_PATH_MAX_SIZE, hist_file);
	len = strlen(last_path);
	if (len > 1)
	{
		last_path[len - 1] = '\0'; /* 消除一个多余的 ‘\n’ */
		strcpy(PathBuf, last_path);
	}
	return;
}

/** 
* 获取和设置备份路径 
*/
void getEnterPath()
{
    int intJudge = 0;
    char tmpBuf[SELF_BU_PATH_MAX_SIZE]; /**临时缓冲区**/

    while(1)
    {
		printf("Enter The Path You want!\n");
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

/** 
* 供使用的存储时格式为：路径
* 供查看的存储时格式为：路径 时间 
*/
void storePathHistory(const char path[])
{
	time_t ctimes;
	time(&ctimes);
	FILE* input_use = Fopen("LastPath.conf", "w"); /* 每次写入覆盖 */
    FILE* input_show = Fopen("PathHistory.txt", "a");
    if(!input_show || !input_use)
		return;
	fprintf(input_use, "%s\n", path);
    fprintf(input_show, "%s %s", path, ctime(&ctimes));
    fclose(input_show);
	fclose(input_use);
    return;
}

/**
* 显示路径历史
*/
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

/** 
* 返回当前备份存储路径 
*/
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
