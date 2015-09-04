#ifndef SETPATH_H_INCLUDED
#define SETPATH_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <time.h>
#include <string.h>
#include "saftfunc.h"
#define SELF_PATH_HISTIRY_MAX_NUM 10
#define SELF_BU_PATH_MAX_SIZE 2048
#define SELF_LOAD_DEFAULT_PATH "C:/User-wu/backup/"

	/** 获取并设置备份位置 **/
	void getEnterPath();

	/** 返回当前的备份路径 **/
	const char * getBackUpPath();

	/** 将使用过的存储路径 写入文件中保存 **/
	void storePathHistory(const char[]);

	/** 获取存储的历史路径
	**  返回形式为 每个路径为一个数组元素， 最后一个元素为 NULL
	**/
	void showPathHistory();

	/** 换掉路径中的 '\', 换为 '/' **/
	void replSymb(char *);

#if defined(__cplusplus)
}
#endif
#endif // SETPATH_H_INCLUDED
