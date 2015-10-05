#ifndef INCLUDE_BACK_UP_H
#define INCLUDE_BACK_UP_H
#include "Queue.h"
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <process.h>
#include <sys/stat.h>

#define TRY_TIMES 3  /* ���³��Ի�ȡ�������� */
#define MIN_PATH_NAME _MAX_PATH /* ��С������ */
#define LARGEST_PATH_NAME 32767 /* ·����������� */

#define MAX_ENTER_PATH _MAX_PATH
jmp_buf select_jmp;
jmp_buf alloc_jmp;
jmp_buf get_hd_jmp;

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * function  ��ʾ��������
 */
void sec_main_windows();

/**
* @version  1.0 2015/10/03
* @author   wushengxin
* @param    path  Դ·��
bpath Ŀ��·��
* @function ���ڱ������ұ����ļ�����֮ѹ�������
*/
void backup(const char * __restrict path, const char * __restrict bpath);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src �ⲿ�������ģ�������������ռ�������·��
* @function �����ڶ���Ϊ�洢��·������ռ䡣
*/
char * make_path(const char * src);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src �ⲿ�������ģ����� make_path ������Ŀռ䣬�����ͷ�
* @function �����ͷ� make_path ������ڴ�ռ�
*/
void rele_path(char * src);

/**
* @version 1.0 2015/09/28
* @author  wushengxin
* @param   src_path ���� FindFirstFile()
src_file ��������ҵ���Ŀ¼�����γ��µ�Ŀ¼·��
* @function ���ڵ����û��Ӽ��������·���ַ�����ʹ���Ǳ��һ�£����ڴ���
*/
void adjust_path(char * __restrict src_path, char * __restrict src_file);


#endif