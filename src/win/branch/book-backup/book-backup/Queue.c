#include "Queue.h"

//static CRITICAL_SECTION empty_sec; /* 用于判断队列是否为空时的关键段 */
static CRITICAL_SECTION io_section; /* 提供线程安全 */
SRWLOCK rw_mutex; /* 读写锁 */
const int CAPCITY = CAPCITY_OF_QUEUE;
/**
 * @version  1.0 2015/10/03
 * @author   wushengxin 
 * @param    object 外部传入的对象指针 相当于 this
 * @function 释放整个队列实体的空间
 */
static void del_queue(queue * object)
{
//	DeleteCriticalSection(&empty_sec);
	DeleteCriticalSection(&io_section);
	Free_s(object->path_contain);
	return;
}

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    object 外部传入的对象指针 相当于 this
			 src    源路径
			 dst    目的路径
 * @function 将外部传入的<源路径，目的路径> 存入队列中
 */
static int push_back(queue * __restrict object, const char * __restrict src, const char * __restrict dst)
{
	char*    loc_src = NULL; /* 本地变量，尽量利用寄存器以及缓存 */
	char*    loc_dst = NULL;
	combine* loc_com = NULL;
	queue*   loc_que = object;

	size_t   len_src = strlen(src); /* 获取路径长度 */
	size_t   len_dst = strlen(dst);
	size_t   rear = 0; /* 队列的队尾 */
	size_t   front = 0; /* 队列的队首 */

	loc_src = Malloc_s(len_src + 1); /* 分配空间 */
	loc_dst = Malloc_s(len_dst + 1);
	loc_com = Malloc_s(sizeof(combine));
	if (loc_src == NULL || loc_dst == NULL || loc_com == NULL)
	{
		Free_s(loc_src);
		Free_s(loc_dst);
		Free_s(loc_src);
		return 1;
	}
	strcpy(loc_src, src); /* 构造路径模型 */
	strcpy(loc_dst, dst);
	loc_com->dst_to_path = loc_dst;
	loc_com->src_from_path = loc_src;

	EnterCriticalSection(&io_section);
    rear = loc_que->rear;   /*获取队尾*/
    front = loc_que->front; /*获取队首*/
	loc_que->path_contain[rear++] = loc_com; /* 将本地路径加入实体 */
	loc_que->rear = (rear % CAPCITY);     /* 用数组实现循环队列的步骤 */
//	EnterCriticalSection(&empty_sec);
	AcquireSRWLockExclusive(&rw_mutex);
	if (loc_que->rear == loc_que->front)  
	{
		loc_que->empty = 0;
	}
	ReleaseSRWLockExclusive(&rw_mutex);
//	LeaveCriticalSection(&empty_sec);
	LeaveCriticalSection(&io_section);
	return 0;
}

/**
 * @version  1.0 2015/10/03
 * @author   wushengxin
 * @param    object 外部传入的对象指针
 */
static combine * pop_front(queue* object)
{
	EnterCriticalSection(&io_section);
	size_t   loc_front = object->front;                   /*获取当前队首*/
	combine* loc_com   = object->path_contain[loc_front]; /*获取当前文件名*/
	object->path_contain[loc_front] = NULL;     /*出队操作*/
	object->front = ((object->front) + 1) % CAPCITY; /*完成出队*/

//	EnterCriticalSection(&empty_sec);
	AcquireSRWLockExclusive(&rw_mutex);
	if (object->front == object->rear)
		object->empty = 1;
	else
		object->empty = 0;
	ReleaseSRWLockExclusive(&rw_mutex);
//	LeaveCriticalSection(&empty_sec);
	LeaveCriticalSection(&io_section);
	return loc_com;
}

int newQueue(queue * object)
{
	queue*      loc_que = object;
	combine**   loc_arr = NULL;

	loc_arr = Malloc_s(CAPCITY * sizeof(combine*));
	if (!loc_arr)
		return 1;

	loc_que->capcity = CAPCITY; /* 容量 */
	loc_que->front = 0;        /* 队首 */
	loc_que->rear = 0;        /* 队尾 */

	loc_que->path_contain = loc_arr; /* 将分配好的空间，放进对象中 */
	loc_que->PushBack = push_back;
	loc_que->PopFront = pop_front;
	loc_que->Delete = del_queue;
//	InitializeCriticalSection(&empty_sec);
	InitializeCriticalSection(&io_section);
	InitializeSRWLock(&rw_mutex);
	return 0;
}
