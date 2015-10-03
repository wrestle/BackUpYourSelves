#include "safeFunc.h"

/**
* @version 1.0 2015/10/01
* @author  wushengixin
* @param   params 参看结构体说明
* function 用于隐藏一些对错误的处理，并调用库函数 fopen 进行打开操作
*/
FILE* Fopen(const params file_open)
{
	int times = 0;
	FILE* ret_p = NULL;
	if (file_open.file == NULL)
	{
		fputs("The File Name is EMPTY! Comfirm it and Try Again", stderr);
		return ret_p;
	}
	setjmp(fopn_jmp); /* fopn_jmp To there */
	ret_p = fopen(file_open.file, file_open.mode);
	if (ret_p == NULL)
	{
		if (times++ < TRY_TIMES)  
			longjmp(fopn_jmp, 0); /* fopn_jmp From here */
		fprintf(stderr, "The File : %s Open with Mode (%s) Fail!\n", file_open.file, file_open.mode);
	}
	return ret_p;
}

void * Malloc_s(size_t sizes)
{
	int times = 0;
	void * ret_p = NULL;
	if (sizes == 0)
		return NULL;
	setjmp(malc_jmp); /* malc_jmp To There */
	ret_p = malloc(sizes);
	if (ret_p == NULL)
	{
		if (times++ < TRY_TIMES) /* malc_jmp From Here */
			longjmp(malc_jmp, 0);
		fputs("Allocate Memory Fail!", stderr);
	}
	return ret_p;
}

void Free_s(void * input)
{
	if (input == NULL)
	{
#if !defined(NOT_DEBUG_AT_ALL)
		fputs("Sent A NULL pointer to the Free_s Function!", stderr);
#endif
		return;
	}
	free(input);
	input = NULL;
}