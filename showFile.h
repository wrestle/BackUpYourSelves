#ifndef SHOWFILE_H_INCLUDED
#define SHOWFILE_H_INCLUDED
#include <windows.h>
#include <string.h>
#include <memory.h>
#include <setjmp.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#define SELF_MAX_PATH_LENGTH 4096
#define SELF_MAX_FILE_LENGTH 256
#define SELF_LONGJMP_RET_VALUE 1

/** ר���� showAllFileInDir ������ longjmp **/
jmp_buf sAFID_jump;
/** mode = 0 ��ʾ��ǰĿ¼�µ��ļ���Ŀ¼
 ** TODO mode = 1 �ݹ���ʾ����
**/
void showAllFileInDir(const char * DirPath, int shownmode);

void showReadMe();

/** ����·���е� '\', ��Ϊ '/' **/
void replSymb(char *);
#endif // SHOWFILE_H_INCLUDED
