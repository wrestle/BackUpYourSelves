#include "vectors.h"

int newVectors(vectors* objects)
{
    int times = 0;  //重试次数
    vectors* localvec = objects;
    char**   localArr = NULL;
/*
    setjmp(fstJmp);
    localvec = (vectors*)malloc(sizeof(vectors));
    if(!localvec)
    {
        fprintf(stderr, "Allocate Memory For \"vectors\" Fail! \n ");
        if(times++ < 2)
            longjmp(fstJmp, 1);
        else
            return 1;
    }
    times = 0;
*/
    setjmp(secJmp);
    localArr = (char**)malloc(20*sizeof(char*));
    if(!localArr)
    {
        fprintf(stderr, "Allocate Memory For \"vectors->nameArray\" Fail! \n ");
        if(times++ < 2)
        longjmp(secJmp, 1);//  second jump from here
        else
        {
            //free(localvec); 除去分配localvec
            return 2;
        }
    }

    localvec->capcity  = 20; // 容量
    localvec->position = 0;  // 当前位置

    //objects            = localvec;
    objects->nameArray = localArr;

    objects->PushBack = pushback;
    objects->Remove   = remove_s;
    objects->Delete   = delVectors;
    return 0;
}

void delVectors(vectors* objects)
{
    vectors* localVec = objects;
    char**   localArr = localVec->nameArray;
    int      sizes    = localVec->capcity, i;
    for(i = 0;i < sizes;++i)
    {
        free(localArr[i]);
    }
    // free(localArr);
    return;
}

int pushback(vectors* objects, const char* elements)
{
    int times = 0;
    char *   localBuf = NULL;
    vectors* localVec = objects;
    size_t   capacity = localVec->capcity;
    size_t   length   = strlen(elements);
    size_t   position = localVec->position;


    localBuf = (char*)malloc(length+1);
    setjmp(trdJmp);
    if(!localBuf)
    {
        fprintf(stderr, "Allocate Memory For \"localBuf in pushback\" Fail! \n ");
        if(times++ < 2)
            longjmp(trdJmp, 1); /** third jump from there **/
        else
            return -1;
    }
    strcpy(localBuf, elements);

    if(position < capacity)
        localVec->nameArray[position] = localBuf;
    else if(position < 160)
    {
        localVec->capcity *= 2;
        setjmp(fstJmp);
        char** check = realloc(localVec->nameArray, //原地址
                            (localVec->capcity)*sizeof(char*)); //现大小
        if(!check)
        {
            if(times++ < 2)
                longjmp(fstJmp, 1);
            free(localBuf);
            return -2;
        }
        localVec->nameArray[position] = localBuf;
    }
    else
    {
        free(localBuf);
        return -3;
    }

    return ++(localVec->position);
}

const char* remove_s(vectors* objects)
{
    if( (objects->position) > 0 )
    {
        int position = --(objects->position);
        return objects->nameArray[position];
    }
    else
        return NULL;
}
