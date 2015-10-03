#ifndef INCLUDE_SET_PATH_H
#define INCLUDE_SET_PATH_H
#include "safeFunc.h"
#include <time.h>
#include <string.h>
#include <io.h>

#define SELF_LOAD_DEFAULT_PATH "C:/"
#define MIN_PATH_NAME _MAX_PATH /* ��С������ */
#define LARGEST_PATH_NAME 32767 /* ·����������� */


enum {NOT_EXIST = 0, EXIST = 1};

/**
 * @versions  1.0 2015/10/02
 * @author    wushengxin
 * @function  ����ÿ�γ�������ʱ��ʼ��Ŀ��·��
 */
void init_path();

/**
 * @version  1.0 2015/10/02
 * author    wushengxin
 * function  �����ڴ�����ʾ���е���ʷ·��
 */
void show_hist_path();

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @function ���ڶ�ȡ�Ӽ��������·������֮����ΪĬ��·�������洢��
 */
void set_enter_path();

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @param    path ���ڼ���·��
 * @function ���ڼ���û������·���Ƿ�����Ч��
 */
int is_valid_path(const char * path);

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @param    path ��Ҫ�洢��·��
 * @function ���ڴ洢·���������ļ� "show_hist" �� "use_hist" 
 */
void store_hist_path(const char * path);

/**
 * @version  1.0 2015/10/02
 * @author   wushengxin
 * @function ���ڷ��ص�ǰʹ�õ�Ŀ��·��
 */
const char * get_backup_topath();

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src �ⲿ����ģ����ڵ���
* @function �����滻·���е� / Ϊ \ ��
*/
void repl_str(char * src);

#endif