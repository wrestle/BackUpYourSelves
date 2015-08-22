#include "saftfunc.h"

void* Malloc(size_t sizes)
{
	int times = 0;
	void* localpoint = malloc(sizes);
	
	setjmp(fstJmp);
	if (!localpoint)
	{
		if (times++ < 2)
			longjmp(fstJmp, 1);
		else
		{
			fprintf(stderr, "The malloc allocate %zu bytes Fail!\n", sizes);
			return localpoint;
		}
	}
	return localpoint;
}

void Free(void* pointer)
{
	if (!pointer) 
		return;
	free(pointer);
	return;
}

FILE* Fopen(const char* filename, char* mode)
{
	FILE* localfile;
	int times = 0;
	if (!filename)
	{
		fprintf(stderr, "Open File Fail! The Filename is Empty\n");
		return NULL;
	}
	if (!mode)
	{
		if (_access(filename, 0) != -1)
			mode = "w"; //文件不存在则默认为写入，可以创建一个新文件
		else
			mode = "r"; // 否则，默认为 读入，不会损坏文件
	}

	setjmp(secJmp);
	localfile = fopen(filename, mode);
	if (!localfile)
	{
		if (times++ < 2)
			longjmp(secJmp, 1);
		else
		{
			fprintf(stderr, "Open file %s Fail!\n", filename);
			return localfile;
		}
	}
	return localfile;
}