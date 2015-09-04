#ifndef BACKUP_H_INCLUDED
#define BACKUP_H_INCLUDED
//#define NOT_DEBUG

#include <time.h>
#include <process.h>
#include <sys/stat.h>

#include "setPath.h"
#include "vectors.h"


jmp_buf forJmp;

void showBUSelect();

/**不同的备份模式**/
// 1 以当前路径为根目录，备份所有子文件和子目录
// 2 以当前路径的根路径为根目录，备份所有的子文件和子目录
void backup(int mode, const char* path, const char* bupath);

/** 该备份文件是否做过修改 **/
/**
* 增量备份的实现
*/
int  is_changed(const char * dstfile, const char * srcfile);

unsigned int __stdcall callBackup(void * pSelect);
unsigned int __stdcall callCopyFile(void* para);
#endif // BACKUP_H_INCLUDED
