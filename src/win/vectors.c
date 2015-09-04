#include "vectors.h"
CRITICAL_SECTION empty_sec;

int newVectors(vectors* objects)
{
    int times = 0;  //重试次数
    vectors*    localvec = objects;
    combine**   localArr = NULL;

	localArr = (combine**)Malloc(20 * sizeof(combine*));
    if(!localArr)
    {
         return 2;
    }
	 
    localvec->capcity  = 20; // 容量
    localvec->rear = 0;    // 队尾
	localvec->fronts = 0; // 队首

    localvec->nameArray = localArr;

    localvec->PushBack = pushback;
    localvec->PopFront = popfront;
    localvec->Delete   = delVectors;

	InitializeCriticalSection(&empty_sec);
    return 0;
}

void delVectors(vectors* objects)
{
    combine**   localArr = objects->nameArray;
    free(localArr);

	DeleteCriticalSection(&empty_sec);
    return;
}

int pushback(vectors* objects, const char* src, const char* dst)
{
    int times = 0;
    char*    localSrc = NULL, *localDst = NULL;
    vectors* localVec = objects;
	combine* localCom = NULL;
    size_t   len_src  = strlen(src);
	size_t   len_dst  = strlen(dst);
	size_t   rear  = localVec->rear;   /*获取队尾*/
	size_t   front = localVec->fronts; /*获取队首*/

	localSrc = (char*)Malloc(len_src + 1);
	if (!localSrc)
	{
		return -3;
	}

	localDst = (char*)Malloc(len_dst + 1);
	if (!localDst)
	{
		return -4;
	}
    strcpy(localSrc, src);
	strcpy(localDst, dst);

	localCom = (combine*)Malloc(sizeof(combine));
	if (!localCom)
	{
		return -5;
	}
	localCom->dstPath = localDst;
	localCom->srcPath = localSrc;

    localVec->nameArray[rear++] = localCom;
	localVec->rear = (rear%20); //20为容量

	EnterCriticalSection(&empty_sec);
	if (localVec->rear == localVec->fronts)
	{
		localVec->emptys = 0;
	}
	LeaveCriticalSection(&empty_sec);
    return localVec->rear;
}

combine* popfront(vectors* objects)
{
	size_t   localfront = objects->fronts; /*获取当前队首*/
	combine* localCom = objects->nameArray[localfront]; /*获取当前文件名*/
	objects->nameArray[localfront] = NULL; /*出队操作*/
	objects->fronts = ((objects->fronts) + 1) % 20; /*完成出队*/

	EnterCriticalSection(&empty_sec); 
	if (objects->fronts == objects->rear)
		objects->emptys = 1;
	else
		objects->emptys = 0;
	LeaveCriticalSection(&empty_sec);
	return localCom;
}

