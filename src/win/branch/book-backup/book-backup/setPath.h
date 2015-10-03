#ifndef INCLUDE_SET_PATH_H
#define INCLUDE_SET_PATH_H
#include "safeFunc.h"
#include <time.h>
#include <string.h>
#include <io.h>

#define SELF_LOAD_DEFAULT_PATH "C:/"
#define MIN_PATH_NAME _MAX_PATH /* 最小的限制 */
#define LARGEST_PATH_NAME 32767 /* 路径的最大限制 */


enum {NOT_EXIST = 0, EXIST = 1};

/**
 * @versions  1.0 2015/10/02
 * @author    wushengxin
 * @function  用于每次程序启动时初始化目的路径
 */
void init_path();

/**
 * @version  1.0 2015/10/02
 * author    wushengxin
 * function  用于在窗口显示所有的历史路径
 */
void show_hist_path();

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @function 用于读取从键盘输入的路径并将之设置为默认路径，并存储。
 */
void set_enter_path();

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @param    path 用于检查的路径
 * @function 用于检查用户输入的路径是否是有效的
 */
int is_valid_path(const char * path);

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @param    path 需要存储的路径
 * @function 用于存储路径到本地文件 "show_hist" 和 "use_hist" 
 */
void store_hist_path(const char * path);

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @function 用于返回当前使用的目的路径
 */
const char * get_backup_topath();

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src 外部传入的，用于调整
* @function 用于替换路径中的 / 为 \ 的
*/
void repl_str(char * src);

#endif