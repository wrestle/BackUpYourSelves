#ifndef BACKUP_H_INCLUDED
#define BACKUP_H_INCLUDED

#include <io.h>
#include <time.h>
#include <process.h>
#include <Windows.h>

#include "setPath.h"
#include "vectors.h"


jmp_buf forJmp;

void showBUSelect();

/**不同的备份模式**/
// 1 以当前路径为根目录，备份所有子文件和子目录
// 2 以当前路径的根路径为根目录，备份所有的子文件和子目录
void backup(int mode, const char* path, const char* bupath);

unsigned int __stdcall callBackup(void * pSelect);
unsigned int __stdcall callCopyFile(void* para);
#endif // BACKUP_H_INCLUDED
