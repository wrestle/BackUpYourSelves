#ifndef BACKUP_H_INCLUDED
#define BACKUP_H_INCLUDED
//#define NOT_DEBUG

#if defined(__cplusplus)
extern "C" {
#endif

#include <time.h>
#include <process.h>
#include <sys/stat.h>

#include "setPath.h"
#include "vectors.h"


	jmp_buf forJmp;

	/**
	* 选项的显示
	*/
	void showBUSelect();

	/**
	* 备份
	*/
	void backup(int mode, const char* path, const char* bupath);

	/** 该备份文件是否做过修改
	* 增量备份的实现
	*/
	static int is_changed(const char * dstfile, const char * srcfile);

	/**
	* 用于子线程调用，将备份文件入队功能的函数
	*/
	static unsigned int __stdcall callBackup(void * pSelect);

	/**
	* 用于子线程调用，将备份文件拷贝到备份路径
	*/
	static unsigned int __stdcall callCopyFile(void* para);

#if defined(__cplusplus)
}
#endif
#endif // BACKUP_H_INCLUDED
