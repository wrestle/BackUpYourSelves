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
    char** nameArray; //����ģ�� |str1|str2|str3|str4|str5| ...
    int             position;  //����
    size_t          capcity;   //����
    fpPushBack      PushBack;  // ����1 ѹ�����
    fpRemove        Remove;    // ����2 ɾ������
    fpDestructor    Delete;    // ����3 �ͷŲ���
};

/**
**  ������ĩβ�����һ��Ԫ�أ����ҷ������� , -1 Ϊ�ڴ����ʧ��
**  -2 Ϊ����vectors����ʧ��  -3 Ϊ�����������ޣ�������������
**  �ں����з����ڴ������Ԫ��
**/
int pushback(vectors * objects, const char * elements);

/** ɾ�����һ��Ԫ�أ���������ֵ
**  ����ɾ��������ɾ��(���ͷ��ڴ�)�� ����Ԫ���򷵻� NULL
**/
const char* remove_s(vectors* objects);

/** ����
**  ����0��ʾ�ɹ�������1��ʾ  vectors �ڴ����ʧ�ܣ�
**  ����2��ʾ vectors.nameAarray �ڴ����ʧ��
**/
int newVectors(vectors* objects);

/** �ͷ� **/
void delVectors(vectors* objects);

/** �ֱ����� newVectors, newVectors, pushback **/
jmp_buf fstJmp, secJmp, trdJmp;

#endif // VECTORS_H_INCLUDED
