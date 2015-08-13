#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

typedef struct _vectors vectors;

typedef int         (*fpPushBack)(vectors *, const char *);
typedef const char* (*fpRemove)(vectors* );
typedef void        (*fpDestructor)(vectors* );

struct _vectors{               //         | 0  | 1  | 2  | 3  | 4  | ...
    char** nameArray; //主体模型 |str1|str2|str3|str4|str5| ...
    int             position;  //坐标
    size_t          capcity;   //容量
    fpPushBack      PushBack;  // 操作1 压入操作
    fpRemove        Remove;    // 操作2 删除操作
    fpDestructor    Delete;    // 操作3 释放操作
};

/**
**  向向量末尾中添加一个元素，并且返回坐标 , -1 为内存分配失败
**  -2 为扩大vectors容量失败  -3 为到达扩容上限，不再允许扩容
**  在函数中分配内存来添加元素
**/
int pushback(vectors * objects, const char * elements);

/** 删除最后一个元素，并返回其值
**  其中删除并非真删除(不释放内存)， 若无元素则返回 NULL
**/
const char* remove_s(vectors* objects);

/** 构造
**  返回0表示成功，返回1表示  vectors 内存分配失败，
**  返回2表示 vectors.nameAarray 内存分配失败
**/
int newVectors(vectors* objects);

/** 释放 **/
void delVectors(vectors* objects);

/** 分别用于 newVectors, newVectors, pushback **/
jmp_buf fstJmp, secJmp, trdJmp;

#endif // VECTORS_H_INCLUDED
