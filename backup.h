#ifndef BACKUP_H_INCLUDED
#define BACKUP_H_INCLUDED

#include <io.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#include "setPath.h"
#include "vectors.h"
#include "showFile.h"
jmp_buf forJmp;

void showBUSelect();

/**不同的备份模式**/
// 1 以当前路径为根目录，备份所有子文件和子目录
// 2 以当前路径的根路径为根目录，备份所有的子文件和子目录
void backup(int mode, const char* path, const char* bupath);

#endif // BACKUP_H_INCLUDED
