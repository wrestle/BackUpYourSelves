#include "vectors.h"
CRITICAL_SECTION empty_sec;

int newVectors(vectors* objects)
{
    int times = 0;  //重试次数
    vectors*       localvec = objects;
    combine**   localArr = NULL;

    //setjmp(secJmp);
    //localArr = (combine**)malloc(20*sizeof(combine*));
	localArr = (combine**)Malloc(20 * sizeof(combine*));
    if(!localArr)
    {
    //    fprintf(stderr, "Allocate Memory For \"vectors->nameArray\" Fail! \n ");
    //    if(times++ < 2)
	//		longjmp(secJmp, 1);//  second jump from here
    //    else
            return 2;
     }
	 
    localvec->capcity  = 20; // 容量
    localvec->rear = 0;    // 队尾
	localvec->fronts = 0; // 队首

    localvec->nameArray = localArr;

    localvec->PushBack = pushback;
    localvec->PopFront = popfront;
    localvec->Delete      = delVectors;

	InitializeCriticalSection(&empty_sec);
    return 0;
}

void delVectors(vectors* objects)
{
    combine**   localArr   = objects->nameArray;
    free(localArr);

	DeleteCriticalSection(&empty_sec);
    return;
}

int pushback(vectors* objects, const char* src, const char* dst)
{
    int times = 0;
    char *    localSrc = NULL,* localDst = NULL;
    vectors* localVec = objects;
	combine* localCom = NULL;
    size_t   len_src  = strlen(src);
	size_t   len_dst  = strlen(dst);
    size_t   rear       = localVec->rear;
	size_t   front     = localVec->fronts;

	//setjmp(trdJmp);
    //localSrc = (char*)malloc(len_src + 1);
	localSrc = (char*)Malloc(len_src + 1);
	if (!localSrc)
	{
	//	fprintf(stderr, "Allocate Memory For \"localSrc in pushback\" Fail! \n ");
	//	if (times++ < 2)
	//		longjmp(trdJmp, 1); /** third jump from there **/
	//	else
			return -3;
	}

	//times = 0;
	//setjmp(fifJmp);
	//localDst = (char*)malloc(len_dst + 1);
	localDst = (char*)Malloc(len_dst + 1);
	if (!localDst)
	{
	//	fprintf(stderr, "Allocate Memory For \"localDst in pushback\" Fail! \n ");
	//	if (times++ < 2)
	//		longjmp(fifJmp, 1); /** fifth jump from there **/
	//	else
			return -4;
	}
    strcpy(localSrc, src);
	strcpy(localDst, dst);

	//times = 0;
	//setjmp(sixJmp);
	//localCom = (combine*)malloc(sizeof(combine));
	localCom = (combine*)Malloc(sizeof(combine));
	if (!localCom)
	{
	//	fprintf(stderr, "Allocate Memory For \"localCom in pushback\" Fail! \n ");
	//	if (times++ < 2)
	//		longjmp(sixJmp, 1); /** sixth jump from there **/
	//	else
			return -5;
	}
	localCom->dstPath = localDst;
	localCom->srcPath = localSrc;

    localVec->nameArray[rear++] = localCom;
	localVec->rear = (rear%20); //20为容量
	if (localVec->rear == localVec->fronts)
	{
		localVec->emptys = 0;
	}
    return localVec->rear;
}

combine* popfront(vectors* objects)
{
	size_t   localfront = objects->fronts;
	combine* localCom = objects->nameArray[localfront];
	objects->nameArray[localfront] = NULL;
	objects->fronts = ((objects->fronts) + 1) % 20;

	EnterCriticalSection(&empty_sec);
	if (objects->fronts == objects->rear)
		objects->emptys = 1;
	else
		objects->emptys = 0;
	LeaveCriticalSection(&empty_sec);
	return localCom;
}

