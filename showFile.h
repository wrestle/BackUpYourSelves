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

/** 专用于 showAllFileInDir 函数的 longjmp **/
jmp_buf sAFID_jump;
/** mode = 0 显示当前目录下的文件与目录
 ** TODO mode = 1 递归显示所有
**/
void showAllFileInDir(const char * DirPath, int shownmode);

void showReadMe();

/** 换掉路径中的 '\', 换为 '/' **/
void replSymb(char *);
#endif // SHOWFILE_H_INCLUDED
