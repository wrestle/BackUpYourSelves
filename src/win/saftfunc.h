#ifndef MALLOCS_H_INCLUDE
#define MALLOCS_H_INCLUDE
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <io.h>

void* Malloc(size_t sizes); // ��ȫ��malloc
void Free(void* pointer); // ��ȫ��free
FILE* Fopen(const char* filename, char* mode);


jmp_buf fstJmp; // Malloc
jmp_buf secJmp; // Fopen
#endif