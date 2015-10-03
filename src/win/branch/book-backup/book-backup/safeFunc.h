#ifndef INCLUDE_SAFE_FUNCTION_H
#define INCLUDE_SAFE_FUNCTION_H

#include <stdio.h> /* size_t */
#include <stdlib.h>
#include <setjmp.h>
#define TRY_TIMES 3

typedef struct _input_para{
	char * file; /* 待打开或创建的文件名 */
	char * mode; /* 打开的模式 */
}params;

jmp_buf malc_jmp; /*Malloc_s*/
jmp_buf fopn_jmp; /*Fopen*/

 /**
 * @version 1.0 2015/10/01
 * @author  wushengixin
 * @param   ... 参看结构体说明
			可传入任意的个数的，形式为 .file = "xxx", .mode = "x" 的参数
 * function 用于使用默认参数，并调用函数 Fopen 进行打开操作
 */
#define Fopen_s(...) Fopen((params){.file = NULL, .mode = "r", __VA_ARGS__})
FILE* Fopen(const params file_open);

/**
 * @version 1.0 2015/10/01
 * @author  wushengxin
 * param    sizes 输入需要分配的大小
 * function 用于隐藏一些对错误的处理，并调用malloc库函数分配空间
 */
void * Malloc_s(size_t sizes);

/**
 * @version 1.0 2015/10/01
 * @author  wushengxin
 * @param   input 外部传入的等待释放的指针
 * function 用于隐藏一些对错误的处理，并调用free库函数进行释放指针
 */
void Free_s(void * input);
#endif