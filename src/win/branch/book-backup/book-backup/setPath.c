#include "setPath.h"

static char to_path_buf[LARGEST_PATH_NAME] = SELF_LOAD_DEFAULT_PATH;
const char * DEFAULT_TO_PATH = SELF_LOAD_DEFAULT_PATH;
const int LARGEST_PATH = LARGEST_PATH_NAME;

void init_path()
{
	int len = 0;
	char last_path[LARGEST_PATH_NAME] = { '\0' };
	FILE* hist_file = Fopen_s(.file = "LastPath.conf", .mode = "r");
	if (!hist_file) /* 打开失败则不初始化 */
		return;
	fgets(last_path, LARGEST_PATH_NAME, hist_file);
	len = strlen(last_path);
	if (len > 1)
	{
		last_path[len - 1] = '\0'; /* 消除一个多余的 ‘\n’ */
		strcpy(to_path_buf, last_path);
	}
	return;
}

void show_hist_path()
{
	system("cls");
	char outBufName[LARGEST_PATH_NAME] = {'\0'};
	FILE* reading = Fopen_s(.file = "PathHistory.txt", .mode = "r");
	if (!reading)
		return;

	for (int i = 1; i <= 10 && (!feof(reading)); ++i)
	{
		fgets(outBufName, LARGEST_PATH_NAME*sizeof(char), reading);
		fprintf(stdout, "%2d. %s", i, outBufName);
	}
	fclose(reading);
	system("pause");
	return;
}

int is_valid_path(const char * path)
{
	if (_access(path, 0) == 0) /* 是否存在 */
		return EXIST;
	else
		return NOT_EXIST;
}

void set_enter_path()
{
	int intJudge = 0; /* 用来判断是否决定完成输入 */
	char tmpBuf[LARGEST_PATH_NAME]; /** 临时缓冲区 **/
	while (1)
	{
		printf("Enter The Path You want!\n");
		fgets(tmpBuf, LARGEST_PATH_NAME*sizeof(char), stdin); /* 获取输入的路径 */
		sscanf(tmpBuf, "%s", to_path_buf);
		if (is_valid_path(to_path_buf) == NOT_EXIST)
		{
			fprintf(stderr, "Your Enter is Empty, So Load the Default Path\n");
			fprintf(stderr, "%s \n", SELF_LOAD_DEFAULT_PATH);
			strcpy(to_path_buf, SELF_LOAD_DEFAULT_PATH);
		}
		fprintf(stdout, "Your Enter is \" %s \" ?(1 for yes, 0 for no) \n", to_path_buf);

		fgets(tmpBuf, LARGEST_PATH_NAME*sizeof(char), stdin);
		sscanf(tmpBuf, "%d", &intJudge);
		if (intJudge != 0)
		{
			if (to_path_buf[strlen(to_path_buf) - 1] != '/')
				strcat(to_path_buf, "/");
			store_hist_path(to_path_buf);
			break;
		}
	}
	return;
}

void store_hist_path(const char * path)
{
	time_t ctimes;
	time(&ctimes); /* 获取时间 */
	FILE* input_use = Fopen_s(.file = "LastPath.conf", .mode = "w"); /* 每次写入覆盖 */
	FILE* input_show = Fopen_s(.file = "PathHistory.txt", .mode = "a");
	if (!input_show || !input_use)
	{
#if !defined(NOT_DEBUG_AT_ALL)
		fputs("Open/Create the File Fail!", stderr);
#endif
		return;
	}
	fprintf(input_use, "%s\n", path); /* 写入 */
	fprintf(input_show, "\t%s \t%s", path, ctime(&ctimes));
	fclose(input_show);
	fclose(input_use);
	return;
}

const char * get_backup_topath()
{
	return to_path_buf;
}

void repl_str(char * src)
{
	size_t length = strlen(src);
	for (size_t i = 0; i <= length; ++i)
	{
		if (src[i] == '/')
			src[i] = '\\';
	}
	return;
}
	