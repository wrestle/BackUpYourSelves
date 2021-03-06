﻿/**
*** 为了实现多线程，故打算实现一个容器，用于存放待拷贝的文件
***
**/
#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <string.h>
#include <Windows.h>
#include "saftfunc.h"

typedef struct _vectors vectors;
typedef struct _combine combine;

typedef int          (*fpPushBack)(vectors *, const char *, const char*);
typedef combine *	 (*fppopfront)(vectors* );
typedef void         (*fpDestructor)(vectors* );

struct _vectors{         //         | 0  | 1 | 2 | 3 | 4  | ... | 19    |
    combine**   nameArray; //主体模型 |co1 |co2|co3|co4|co5 | ... | co20 |
    int			   rear;         // 队尾坐标
    int            fronts;       // 队首坐标
	int            emptys;       //是否为空 
    size_t         capcity;      //容量
    fpPushBack       PushBack;    // 操作1 压入操作
    fppopfront       PopFront;    // 操作2 删除操作
    fpDestructor     Delete;      // 操作3 释放操作
};

struct _combine
{
	char* srcPath;    // 源文件路径
	char* dstPath;   // 目标路径，即备份到何处
};

/** 压入
**  向向量末尾中添加一个元素，并且返回坐标 
**/
static int pushback(vectors * objects, const char * , const char *);

static combine* popfront(vectors* objects);

/** 构造
**  返回0表示成功，返回1表示  vectors 内存分配失败，
**  返回2表示 vectors.nameAarray 内存分配失败
**/
int newVectors(vectors* objects);

/** 释放 **/
void delVectors(vectors* objects);

#if defined(__cplusplus)
	}
#endif
#endif // VECTORS_H_INCLUDED
