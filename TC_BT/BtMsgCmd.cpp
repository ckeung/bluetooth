#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "inc/BtUtil.h"
#include "inc/BtPlatform.h"
#include "TC_BT_EVT.h"
#define TC_BT_MSG_BUFSIZE   128
unsigned long int      gMsgBuf[TC_BT_MSG_BUFSIZE];
static TcBtQueueCtx QCtx;
static pthread_mutex_t     Qmutex;
 int TC_BT_InitEvent(void)
{
    TC_BT_QueueInit(&QCtx, gMsgBuf, TC_BT_MSG_BUFSIZE);
    pthread_mutex_init(&Qmutex, NULL);
}

 int TC_BT_SendEvent(TcBtEvt *pTcBtEvt)
 {
     int fr = 0;
      int status = 0;
     status = pthread_mutex_lock(&Qmutex);
     if(TC_BT_QueueEnqueue(&QCtx, (void *)pTcBtEvt) == FALSE)
     {
         fr = -1;
     }
     status = pthread_mutex_unlock(&Qmutex);
         return fr;
 }

unsigned long * TC_BT_ReceiveEvent(void)
{
    unsigned long *msg; 
    int status = 0;
    status = pthread_mutex_lock(&Qmutex);
    msg = (unsigned long *)TC_BT_QueueDequeue(&QCtx);
    status = pthread_mutex_unlock(&Qmutex);
    if (msg == NULL)
    {
        return NULL;
    }
    return msg;
}

