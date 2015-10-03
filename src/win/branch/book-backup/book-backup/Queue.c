#include "Queue.h"

static CRITICAL_SECTION empty_sec;
const int CAPCITY = CAPCITY_OF_QUEUE;
/**
 * @version  1.0 2015/10/03
 * @author   wushengxin 
 * @param    object �ⲿ����Ķ���ָ�� �൱�� this
 * @function �ͷ���������ʵ��Ŀռ�
 */
static void del_queue(queue * object)
{
	DeleteCriticalSection(&empty_sec);
	Free_s(object->path_contain);
	return;
}

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    object �ⲿ����Ķ���ָ�� �൱�� this
			 src    Դ·��
			 dst    Ŀ��·��
 * @function ���ⲿ�����<Դ·����Ŀ��·��> ���������
 */
static int push_back(queue * object, const char * src, const char * dst)
{
	int times = 0;
	char*    loc_src = NULL; /* ���ر������������üĴ����Լ����� */
	char*    loc_dst = NULL;
	combine* loc_com = NULL;
	queue*   loc_que = object;

	size_t   len_src = strlen(src); /* ��ȡ·������ */
	size_t   len_dst = strlen(dst);
	size_t   rear = loc_que->rear;   /*��ȡ��β*/
	size_t   front = loc_que->front; /*��ȡ����*/

	loc_src = Malloc_s(len_src + 1); /* ����ռ� */
	if (!loc_src)
		return 1;

	loc_dst = Malloc_s(len_dst + 1);
	if (!loc_dst)
		return 2;
	strcpy(loc_src, src);
	strcpy(loc_dst, dst);

	loc_com = Malloc_s(sizeof(combine));
	if (!loc_com)
		return 3;

	loc_com->dst_to_path = loc_dst; 
	loc_com->src_from_path = loc_src;
	loc_que->path_contain[rear++] = loc_com; /* ������·������ʵ�� */
	loc_que->rear = (rear % CAPCITY);     /* ������ʵ��ѭ�����еĲ��� */

	EnterCriticalSection(&empty_sec);
	if (loc_que->rear == loc_que->front)  
	{
		loc_que->empty = 0;
	}
	LeaveCriticalSection(&empty_sec);
	return 0;
}

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    object �ⲿ����Ķ���ָ��
 */
static combine * pop_front(queue* object)
{
	size_t   loc_front = object->front;                   /*��ȡ��ǰ����*/
	combine* loc_com   = object->path_contain[loc_front]; /*��ȡ��ǰ�ļ���*/
	object->path_contain[loc_front] = NULL;     /*���Ӳ���*/
	object->front = ((object->front) + 1) % 20; /*��ɳ���*/

	EnterCriticalSection(&empty_sec);
	if (object->front == object->rear)
		object->empty = 1;
	else
		object->empty = 0;
	LeaveCriticalSection(&empty_sec);
	return loc_com;
}

int newQueue(queue * object)
{
	queue*      loc_que = object;
	combine**   loc_arr = NULL;

	loc_arr = Malloc_s(CAPCITY * sizeof(combine*));
	if (!loc_arr)
		return 1;

	loc_que->capcity = CAPCITY; /* ���� */
	loc_que->front = 0;        /* ���� */
	loc_que->rear = 0;        /* ��β */

	loc_que->path_contain = loc_arr; /* ������õĿռ䣬�Ž������� */
	loc_que->PushBack = push_back;
	loc_que->PopFront = pop_front;
	loc_que->Delete = del_queue;
	InitializeCriticalSection(&empty_sec);
	return 0;
}
