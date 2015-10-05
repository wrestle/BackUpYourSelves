#ifndef INCLUDE_BACK_UP_H
#define INCLUDE_BACK_UP_H
#include "Queue.h"
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <process.h>
#include <sys/stat.h>

#define TRY_TIMES 3  /* 重新尝试获取的最大次数 */
#define MIN_PATH_NAME _MAX_PATH /* 最小的限制 */
#define LARGEST_PATH_NAME 32767 /* 路径的最大限制 */

#define MAX_ENTER_PATH _MAX_PATH
jmp_buf select_jmp;
jmp_buf alloc_jmp;
jmp_buf get_hd_jmp;

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * function  显示二级界面
 */
void sec_main_windows();

/**
* @version  1.0 2015/10/03
* @author   wushengxin
* @param    path  源路径
bpath 目的路径
* @function 用于遍历查找备份文件，将之压入队列中
*/
void backup(const char * __restrict path, const char * __restrict bpath);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src 外部传进来的，用于向所分配空间中填充的路径
* @function 用于在堆上为存储的路径分配空间。
*/
char * make_path(const char * src);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src 外部传进来的，是由 make_path 所分配的空间，将其释放
* @function 用于释放 make_path 分配的内存空间
*/
void rele_path(char * src);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src_path 用于 FindFirstFile()
src_file 用于添加找到的目录名，形成新的目录路径
* @function 用于调整用户从键盘输入的路径字符串，使他们变得一致，便于处理
*/
void adjust_path(char * __restrict src_path, char * __restrict src_file);


#endif