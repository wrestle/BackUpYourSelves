#ifndef INCLUDE_SHOWFILES_H
#define INCLUDE_SHOWFILES_H
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> /* Windows API */
#include <string.h>  /* 字符串函数 */
#include <tchar.h>	 /* _ftprintf */
#include <setjmp.h>  /* setjmp, longjmp */

//static const char prefix[10] = "\\\\?\\";



/* 我们需要在这里面包含函数的声明 */
/** 加上文档注释，不太喜欢死板的硬套，选择自己觉得重要的信息记录吧
 * @version 1.0 2015/09/28
 * @author  wushengxin
 * @param   from_dir_name 源目录，即用于扫描的目录
			depth         递归的深度，用于在屏幕上格式化输出，每次增加 4
 * @function 用于输出显示目录结构的，可以改变输出流
 */
void show_structure(const char * from_dir_name, int depth);

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
void adjust_path(char * src_path, char * src_file);



/**
 * @version 1.0 2015/09/28
 * @author  wushengxin
 * @param   src 外部传入的，用于调整
 * @function 用于替换路径中的 / 为 \ 的
 */
void repl_str(char * src);
#endif
#endif