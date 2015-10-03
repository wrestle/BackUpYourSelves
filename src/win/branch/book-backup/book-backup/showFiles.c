#if 0
#include "showFiles.h"
static jmp_buf alloc_jmp;
static jmp_buf get_hd_jmp;
void show_structure(const char * from_dir_name, int depth)
{
	/* 用来配合 setjmp和longjmp重新获取句柄HANDLE的变量 */
	int times = 0;
	/** 操作时获取文件夹，文件信息的的必要变量,为了WIN API区别，这里变量的命名使用驼峰法 **/
	HANDLE fileHandle;    
	WIN32_FIND_DATAA fileData;
	LARGE_INTEGER fileSize;

	size_t length = strlen(from_dir_name);
	char * dir_buf = make_path(from_dir_name);  //路径
	char * dir_file_buf = make_path(from_dir_name); //文件
	if (dir_buf == NULL || dir_file_buf == NULL)
		return;
	adjust_path(dir_buf, dir_file_buf); /* 调整路径和文件格式到标准格式 */
	repl_str(dir_buf);
	repl_str(dir_file_buf);
//	printf("After adjust \n%s \n%s\n", dir_buf, dir_file_buf);
	/* 开始调用 Windows API 获取路径下的文件和文件夹信息 */
	setjmp(get_hd_jmp);
	fileHandle = FindFirstFileA(dir_buf, &fileData);
	if (fileHandle == INVALID_HANDLE_VALUE) /* 如果无法获取句柄超过上限次数，就退出 */
	{
		fprintf(stderr, "The Handle getting Failure! \n");
		if (times++ < TRY_TIMES)
			longjmp(get_hd_jmp, 0);
		return;
	}
	do{
		char * tmp_dir_file_buf = make_path(dir_file_buf);
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			fprintf(stderr, "%*s%s\t<DIR>\t\n", depth, "", fileData.cFileName);
			if (strcmp(fileData.cFileName, ".") == 0 ||  /* . 和 .. 便是当前文件夹和上一级文件夹, 世界上所有系统都一样 */
				strcmp(fileData.cFileName, "..") == 0)
				continue;
			strcat(tmp_dir_file_buf, fileData.cFileName); /* 将文件名连接到当前文件夹路径之后，形成文件路径 */
			show_structure(tmp_dir_file_buf, depth + 4);
		}
		else
		{
			fileSize.LowPart = fileData.nFileSizeLow;   /* 输出大小 */
			fileSize.HighPart = fileData.nFileSizeHigh;
			fprintf(stderr, "%*s%s    \t%ld bytes\t\n", depth, "", fileData.cFileName, 
				                         fileSize.QuadPart);
		}
		rele_path(tmp_dir_file_buf);
	} while (FindNextFileA(fileHandle, &fileData));
	FindClose(fileHandle);
	rele_path(dir_buf);
	rele_path(dir_file_buf);
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
		fprintf(stderr, "ERROR OCCUR When malloc the memory at %s\n Try the %d th times", __LINE__, times+1);
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
	size_t length = strlen(src_path); /* 两个参数的长度在此函数调用之前必定一致 */
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
	return;
}
#if 0
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
#endif
void rele_path(char * src)
{
	free(src);
	src = NULL;
	return;
}
#endif