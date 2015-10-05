#ifndef INCLUDE_QUEUE_H
#define INCLUDE_QUEUE_H
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <Windows.h>

/* ���ļ�ʹ�õĳ��� */
#define CAPCITY_OF_QUEUE 20


/* �ⲿ���� */
void * Malloc_s(size_t sizes);
void Free_s(void * input);

typedef struct _vector_queue queue;
typedef struct _combine combine;

typedef int(*fpPushBack)(queue * __restrict, const char * __restrict, const char * __restrict);
typedef combine * (*fpPopFront)(queue *);
typedef void            (*fpDelete)(queue *);

struct _combine{
	char * src_from_path; /* Դ·�� */
	char * dst_to_path;   /* Ŀ��·�� */
};

struct _vector_queue{
	combine **      path_contain; /* �洢·������������ */
	unsigned int    rear;         /* ��β���� */
	unsigned int    front;        /* �������� */
	int             empty;        /* �Ƿ�Ϊ�� */
	unsigned int    capcity;      /* ���������� */
	fpPushBack      PushBack;  /* ��Ԫ��ѹ���β */
	fpPopFront      PopFront;  /* �����׳��� */
	fpDelete        Delete;    /* �����ͷ��������пռ� */
};

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @function ��ʼ������ģ�ͣ���������ʵ�壬����ռ䣬�Լ��������ԡ�
 */
int newQueue(queue* objects);

#endif