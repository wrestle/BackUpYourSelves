#ifndef INCLUDE_SAFE_FUNCTION_H
#define INCLUDE_SAFE_FUNCTION_H

#include <stdio.h> /* size_t */
#include <stdlib.h>
#include <setjmp.h>
#define TRY_TIMES 3

typedef struct _input_para{
	char * file; /* ���򿪻򴴽����ļ��� */
	char * mode; /* �򿪵�ģʽ */
}params;

jmp_buf malc_jmp; /*Malloc_s*/
jmp_buf fopn_jmp; /*Fopen*/

 /**
 * @version 1.0 2015/10/01
 * @author  wushengixin
 * @param   ... �ο��ṹ��˵��
			�ɴ�������ĸ����ģ���ʽΪ .file = "xxx", .mode = "x" �Ĳ���
 * function ����ʹ��Ĭ�ϲ����������ú��� Fopen ���д򿪲���
 */
#define Fopen_s(...) Fopen((params){.file = NULL, .mode = "r", __VA_ARGS__})
FILE* Fopen(const params file_open);

/**
 * @version 1.0 2015/10/01
 * @author  wushengxin
 * param    sizes ������Ҫ����Ĵ�С
 * function ��������һЩ�Դ���Ĵ���������malloc�⺯������ռ�
 */
void * Malloc_s(size_t sizes);

/**
 * @version 1.0 2015/10/01
 * @author  wushengxin
 * @param   input �ⲿ����ĵȴ��ͷŵ�ָ��
 * function ��������һЩ�Դ���Ĵ���������free�⺯�������ͷ�ָ��
 */
void Free_s(void * input);
#endif