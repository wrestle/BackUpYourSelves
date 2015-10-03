#ifndef INCLUDE_SHOWFILES_H
#define INCLUDE_SHOWFILES_H
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> /* Windows API */
#include <string.h>  /* �ַ������� */
#include <tchar.h>	 /* _ftprintf */
#include <setjmp.h>  /* setjmp, longjmp */

//static const char prefix[10] = "\\\\?\\";



/* ������Ҫ��������������������� */
/** �����ĵ�ע�ͣ���̫ϲ�������Ӳ�ף�ѡ���Լ�������Ҫ����Ϣ��¼��
 * @version 1.0 2015/09/28
 * @author  wushengxin
 * @param   from_dir_name ԴĿ¼��������ɨ���Ŀ¼
			depth         �ݹ����ȣ���������Ļ�ϸ�ʽ�������ÿ������ 4
 * @function ���������ʾĿ¼�ṹ�ģ����Ըı������
 */
void show_structure(const char * from_dir_name, int depth);

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
void adjust_path(char * src_path, char * src_file);



/**
 * @version 1.0 2015/09/28
 * @author  wushengxin
 * @param   src �ⲿ����ģ����ڵ���
 * @function �����滻·���е� / Ϊ \ ��
 */
void repl_str(char * src);
#endif
#endif