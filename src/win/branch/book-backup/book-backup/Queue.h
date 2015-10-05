#ifndef INCLUDE_QUEUE_H
#define INCLUDE_QUEUE_H
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <Windows.h>

/* 本文件使用的常量 */
#define CAPCITY_OF_QUEUE 20


/* 外部函数 */
void * Malloc_s(size_t sizes);
void Free_s(void * input);

typedef struct _vector_queue queue;
typedef struct _combine combine;

typedef int(*fpPushBack)(queue * __restrict, const char * __restrict, const char * __restrict);
typedef combine * (*fpPopFront)(queue *);
typedef void            (*fpDelete)(queue *);

struct _combine{
	char * src_from_path; /* 源路径 */
	char * dst_to_path;   /* 目的路径 */
};

struct _vector_queue{
	combine **      path_contain; /* 存储路径的容器主体 */
	unsigned int    rear;         /* 队尾坐标 */
	unsigned int    front;        /* 队首坐标 */
	int             empty;        /* 是否为空 */
	unsigned int    capcity;      /* 容器的容量 */
	fpPushBack      PushBack;  /* 将元素压入队尾 */
	fpPopFront      PopFront;  /* 将队首出队 */
	fpDelete        Delete;    /* 析构释放整个队列空间 */
};

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @function 初始化队列模型，建立队列实体，分配空间，以及设置属性。
 */
int newQueue(queue* objects);

#endif