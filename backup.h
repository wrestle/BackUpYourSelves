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

/**��ͬ�ı���ģʽ**/
// 1 �Ե�ǰ·��Ϊ��Ŀ¼�������������ļ�����Ŀ¼
// 2 �Ե�ǰ·���ĸ�·��Ϊ��Ŀ¼���������е����ļ�����Ŀ¼
void backup(int mode, const char* path, const char* bupath);

#endif // BACKUP_H_INCLUDED
