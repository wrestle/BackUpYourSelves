#ifndef SETPATH_H_INCLUDED
#define SETPATH_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <string.h>

#define SELF_PATH_HISTIRY_MAX_NUM 10
#define SELF_BU_PATH_MAX_SIZE 4096
#define SELF_LOAD_DEFAULT_PATH "C:/User-wu/backup/"

/** ��ȡ�����ñ���λ�� **/
void getEnterPath();

/** ���ص�ǰ�ı���·�� **/
const char * getBackUpPath();

/** ��ʹ�ù��Ĵ洢·�� д���ļ��б��� **/
void storePathHistory(const char[]);

/** ��ȡ�洢����ʷ·��
**  ������ʽΪ ÿ��·��Ϊһ������Ԫ�أ� ���һ��Ԫ��Ϊ NULL
**/
void showPathHistory();

#endif // SETPATH_H_INCLUDED
