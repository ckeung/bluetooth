/****************************************************************************
 *   FileName    : BTAPP_PLATFORM.c
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
/******************************************************************************
*  Description : This file contains functions and variables for a Bluetooth common application.
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    15/01/14     0.1            created                     Kwon Sunhong.
*******************************************************************************/

#include "BT_CFG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "inc/BtPlatform.h"

#define  FILE_LFN_LEN_TC    (255 * 2)


/************************************ FOR TIMER ************************************/
static _TIMER_BLOCK stTccApiTimers[TIMER_NUM_MAX];
pthread_mutex_t     gSyncMutex;

int BTAPP_Create_Lock(void)
{
    /* Create mutex */
    pthread_mutex_init(&gSyncMutex, NULL);

    return 0;
}

int BTAPP_Destory_Unlock(void)
{
    int fr = 0, status = 0;

    /* Destory mutex */
    status = pthread_mutex_destroy(&gSyncMutex);
    if(status != 0)
    {
        fr = -1;
    }

    return fr;
}

void BTAPP_Lock(void)
{
    pthread_mutex_lock(&gSyncMutex);	
}

void BTAPP_Unlock(void)
{
    pthread_mutex_unlock(&gSyncMutex);
}

/**************************************************************************
*  FUNCTION NAME :
*      void TCCAPI_Timer_ClearAll (void);
*
*  DESCRIPTION : clear all timer
*  INPUT:
*			None
*
*  OUTPUT:	void - Return Type
*  REMARK  :
**************************************************************************/
void TCCAPI_Timer_ClearAll (void)
{
    int		iIdx;

    for (iIdx=0; iIdx < TIMER_NUM_MAX; iIdx++)
    {
        stTccApiTimers[iIdx].fUsed   = FALSE;
        stTccApiTimers[iIdx].ulTmrId = TIMER_NUM_MAX;
    }
}

 int TC_BT_SYS_test(void)
 {
     return 1;
 }
/**************************************************************************
*  FUNCTION NAME :
*      void TCCAPI_Timer_CancelTime (unsigned char ulTmrId);
*
*  DESCRIPTION : Timer disable by Timer ID
*  INPUT:
*			ulTmrId	=
*
*  OUTPUT:	void - Return Type
*  REMARK  :
**************************************************************************/

 void TCCAPI_Timer_CancelTime (unsigned char ulTmrId)
 {
     int		iIdx = (int) ulTmrId;

     /* If an entry was found, reset it */
     if (iIdx < TIMER_NUM_MAX)
     {
         stTccApiTimers[iIdx].fUsed   = FALSE;
         stTccApiTimers[iIdx].ulTmrId = TIMER_NUM_MAX;
     }
 }


/**************************************************************************
*  FUNCTION NAME :
*      int TCCAPI_Timer_SetTime (short ulMsTicks, unsigned char ulTmrId, void (*pvTimerExpiration)(unsigned char));
*
*  DESCRIPTION : Timer enable by Timer ID , per 0.1s
*  INPUT:
*			pvTimerExpiration	=
*			ulMsTicks	=
*			ulTmrId	=
*
*  OUTPUT:	int - Return Type
*  			=
*  REMARK  :
**************************************************************************/
int TCCAPI_Timer_SetTime (short ulMsTicks, unsigned char ulTmrId, void (*pvTimerExpiration)(unsigned char, unsigned char))
{
    int		iIdx = (int) ulTmrId;

    if ( (ulMsTicks == 0) || (ulTmrId >= TIMER_NUM_MAX) )
    {
        /* Parameters error */
        return -1;
    }

    stTccApiTimers[ulTmrId].ulTmrId			= ulTmrId;
    stTccApiTimers[ulTmrId].pvTimerExpire	= pvTimerExpiration;
    stTccApiTimers[ulTmrId].lTicks			= ulMsTicks;  //per 0.5 s
    stTccApiTimers[ulTmrId].fUsed 			= TRUE;

    return 0;
}

/**************************************************************************
*  FUNCTION NAME :
*      int TCCAPI_Timer_SetRepeatedTime (short ulMsTicks, unsigned char ulTmrId, void (*pvTimerExpiration)(unsigned char));
*
*  DESCRIPTION : Timer enable by Timer ID , per 0.1s
*  INPUT:
*			pvTimerExpiration	=
*			ulMsTicks	=
*			ulTmrId	=
*
*  OUTPUT:	int - Return Type
*  			=
*  REMARK  :
**************************************************************************/
/* Telechips Linux HF remark */
int TCCAPI_Timer_SetRepeatedTime (short ulMsTicks, unsigned char ulTmrId, BTAPP_TIMER_MODE repeat, void (*pvTimerExpiration)(unsigned char, unsigned char))
{
    int		iIdx = (int) ulTmrId;

    if ( (ulMsTicks == 0) || (ulTmrId >= TIMER_NUM_MAX) )
    {
        /* Parameters error */
        return -1;
    }

    stTccApiTimers[ulTmrId].ulTmrId			= ulTmrId;
    stTccApiTimers[ulTmrId].pvTimerExpire	= pvTimerExpiration;
    stTccApiTimers[ulTmrId].lTicks			= ulMsTicks;  //per 0.5 s
    stTccApiTimers[ulTmrId].lRepeatTicks	= ulMsTicks;  //per 0.5 s
    stTccApiTimers[ulTmrId].fUsed 			= repeat;

    return 0;
}

int TCCAPI_Timer_BT_IsActiveTime (unsigned char ulTmrId)
{
    return stTccApiTimers[ulTmrId].fUsed;
}

/**************************************************************************
*  FUNCTION NAME :
*      unsigned char TCCAPI_Timer_CheckTime (void);
*
*  DESCRIPTION : Check Timer by timer signal(500ms)
*  INPUT:
*			None
*
*  OUTPUT:	None
*
*  REMARK  :
**************************************************************************/
void TCCAPI_Timer_CheckTime (void)
{
    int	iIdx;


    for (iIdx=TIMER_BT_BEGIN; iIdx <= TIMER_BT_END; iIdx++)
    {
        /* Telechips Linux HF remark */
        if (stTccApiTimers[iIdx].fUsed != FALSE)
        {
            //if (--stTccApiTimers[iIdx].lTicks <= 0)	/* QAC : 12.13 */
            stTccApiTimers[iIdx].lTicks--;

            if (stTccApiTimers[iIdx].lTicks <= 0)
            {
                /* Telechips Linux HF remark */
 #ifdef BT_HANDSFREE_UNIT
                if((iIdx >= TIMER_BT_HFx_MULTI_BEGIN) && (iIdx <= TIMER_BT_HFx_MULTI_END))
                {
                   // BtApp_HF_SetCurrDevNum(TC_BT_TIMER_GET_HFP_DEVNO(iIdx));
                }
#endif
                /* Telechips Linux HF remark */
                if (stTccApiTimers[iIdx].fUsed == BTAPP_TIMERMODE_REPEAT)	// Repeat Timer
                {
                    stTccApiTimers[iIdx].lTicks = stTccApiTimers[iIdx].lRepeatTicks;
                }
                else
                {
                    stTccApiTimers[iIdx].fUsed  = FALSE;
                    stTccApiTimers[iIdx].lRepeatTicks = 0;
                }
				
                if (stTccApiTimers[iIdx].pvTimerExpire != NULL)
                {
                    (*stTccApiTimers[iIdx].pvTimerExpire)(stTccApiTimers[iIdx].ulTmrId, 0);
                }
            }
        }
    }
}

/**************************************************************************
*  FUNCTION NAME	:
*  		Timer_TCCTimefn
*
*  DESCRIPTION		: Check Timer by NU_TIMER... period : 1sec
*
*  INPUT			: NONE
*
*  OUTPUT			: void - Return Type
*
*  REMARK			:
*
**************************************************************************/
#if 0
void Timer_TCCTimefn(void)
{
    while(bTimerRun)
    {
        usleep(500000);
        (void)TCCAPI_Timer_CheckTime();
#ifdef BT_AUTO_CONNECTION
		BtApp_GAP_RecoveryLinkLostEvent();
#endif /* BLUETOOTH_AUTO_CONNECTION */

    }
}
#endif
int str_len16(void *pString)
{
    int count;

    if (((unsigned int)pString & 0x01) != 0)
    {
        unsigned char *pS = (unsigned char *)pString;
        unsigned short sData;

        for (count=0; ; count+=2)
        {
            sData = pS[count+1];
            sData <<= 8;
            sData |= pS[count];

            if ((sData == ((unsigned short)0)) || (count > FILE_LFN_LEN_TC))	// end of string
            {
                count /= 2;	// convert to number of the unicode string
                break;
            }
        }
    }
    else
    {
        unsigned short *pS = (unsigned short *)pString;

        for (count=0; ; count++)
        {
            if (((pS[count]) == ((unsigned short)0)) || (count > (FILE_LFN_LEN_TC/2)))
            {
                break;
            }
        }
    }

    return(count);
}

/**************************************************************************
*  FUNCTION NAME :
*      			 TC_Q_InitBuffer()
*
*  DESCRIPTION :
*			intialization of the Q buffer
*  INPUT:
*			BT_SRBUFFER* pBuffer : the pointer of struct	
*			unsigned int size : maxium size of the main buffer
*			char* pBuff :	pointer of main buffer
*	
*
*  OUTPUT:	void - Return Type
*  	
*  REMARK  :
**************************************************************************/
 void TC_Q_InitBuffer(BT_SRBUFFER* pBuffer,unsigned int size,char* pBuff)
{
	BT_SRBUFFER*	pSimpleBuffer;
	pSimpleBuffer = pBuffer;
	pSimpleBuffer->mHead = 0;	// input
	pSimpleBuffer->mTail =0;	// output
	pSimpleBuffer->mMaxBufferSize = size;
	pSimpleBuffer->pBuffer = pBuff;
	pSimpleBuffer->mState = BT_S_BUFFER_EMPTY;
}

/**************************************************************************
*  FUNCTION NAME :
*      			 TC_Q_DeinitBuffer()
*
*  DESCRIPTION :
*			Deinitialization of the Q buffer.
*  INPUT:
*			BT_SRBUFFER* pBuffer : the pointer of struct	
*			char* pBuff :	pointer of main buffer
*	
*
*  OUTPUT:	void - Return Type
*  	
*  REMARK  :
**************************************************************************/
void TC_Q_DeinitBuffer(BT_SRBUFFER* pBuffer, char *pBuff)
{
	BT_SRBUFFER*  pSimpleBuffer; 
	pSimpleBuffer = pBuffer;

	(void)memset(pBuff,0x0,pSimpleBuffer->mMaxBufferSize);
	(void)memset(pSimpleBuffer, 0x0, sizeof(BT_SRBUFFER));
	(void)free(pBuff);
	(void)free(pSimpleBuffer);
	
	

}

/**************************************************************************
*  FUNCTION NAME :
*      		TC_Q_GetDataSize()
*
*  DESCRIPTION :
*			return buffered data size in bytes
*  INPUT:
*			BT_SRBUFFER* pBuffer : the pointer of BT_SRBUFFER.	
*
*  OUTPUT:	int - Return Type (the data size to be output.)
*  	
*  REMARK  :
*  	Data means the buffered data
**************************************************************************/
int TC_Q_GetDataSize(BT_SRBUFFER* pBuffer)
{
	unsigned int lRemainSize;
	BT_SRBUFFER*	pSimpleBuffer;
	lRemainSize = 0;
	pSimpleBuffer = pBuffer;

	if (pSimpleBuffer->mState == BT_S_BUFFER_EMPTY)
	{
		lRemainSize =  0;
	}
	else
	{
		if ( pSimpleBuffer->mTail < pSimpleBuffer->mHead )	//	Tail  < 	Head
		{
			lRemainSize = pSimpleBuffer->mHead - pSimpleBuffer->mTail;
		}
		else
		{
			lRemainSize = (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail) +  pSimpleBuffer->mHead;
		}
	}

	return (int)lRemainSize;
}


/**************************************************************************
*  FUNCTION NAME :
*      	TC_Q_GetAvailableSpace()
*
*  DESCRIPTION :
*			return unbuffered buffer size in bytes
*  INPUT:
*			BT_SRBUFFER* pBuffer : the point of struct
*	
*
*  OUTPUT:	int  - Return Type (size of dat to be written in Q buffer.)
*  	
*  REMARK  :
* 	
**************************************************************************/
int TC_Q_GetAvailableSpace(BT_SRBUFFER* pBuffer)
{
	unsigned int lEmptySize;
	BT_SRBUFFER*	pSimpleBuffer;
	lEmptySize = 0;
	pSimpleBuffer = pBuffer;

	if (pSimpleBuffer->mState == BT_S_BUFFER_FULL)
	{
		lEmptySize = 0;
	}
	else
	{
		if ( pSimpleBuffer->mTail > pSimpleBuffer->mHead )	//	Tail > Head
		{
			lEmptySize =  pSimpleBuffer->mTail - pSimpleBuffer->mHead;
		}
		else
		{
			lEmptySize = (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead) + pSimpleBuffer->mTail;
		}
	}

	return (int)lEmptySize;
}
/**************************************************************************
*  FUNCTION NAME :
*      		 TC_Q_GetData()
*
*  DESCRIPTION :
*			return buffered data at the first (in FIFO structure)
*  INPUT:
*			BT_SRBUFFER* pBuffer : the pointer of struct
*			char * pOutputBuff: the pointer of output buffer
*			long RequestSize : size of data 
*
*  OUTPUT:	int - Return Type (state of buffer)
*  	
*  REMARK  :
**************************************************************************/
int TC_Q_GetData(BT_SRBUFFER* pBuffer, char * pOutputBuff, long RequestSize)
{
	unsigned int lRemainSize ;
	BT_SRBUFFER*	pSimpleBuffer;
	long Size;
	unsigned int TempAddress;
	int returnvalue;
	Size = RequestSize;
	lRemainSize = 0;
	pSimpleBuffer = pBuffer;
	returnvalue = 1;

	if ( (pSimpleBuffer->mState == BT_S_BUFFER_EMPTY) || (RequestSize==0) )
	{
		returnvalue =  0;
	}
	else
	{
		if ( pSimpleBuffer->mTail < pSimpleBuffer->mHead )	/*	Tail  < 	Head */
		{
			lRemainSize = pSimpleBuffer->mHead - pSimpleBuffer->mTail;

			if (lRemainSize >= (unsigned int)Size )
			{
				(void) memcpy(pOutputBuff,&pSimpleBuffer->pBuffer[pSimpleBuffer->mTail],Size);
				pSimpleBuffer->mTail += (unsigned int)Size;
			}
			else
			{
				returnvalue = 0;	/* short available data! */
			}
		}
		else
		{
			lRemainSize = (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail) +  pSimpleBuffer->mHead;

			if (lRemainSize >= (unsigned int)Size )
			{
				if ( (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail) >= (unsigned int)Size)
				{
					(void) memcpy(pOutputBuff,&(pSimpleBuffer->pBuffer[pSimpleBuffer->mTail]),Size);
					pSimpleBuffer->mTail += (unsigned int)Size;
				}
				else
				{
					(void) memcpy(pOutputBuff,&pSimpleBuffer->pBuffer[pSimpleBuffer->mTail],(long)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail));
					Size -= (long)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail);
					TempAddress = (unsigned int)(pOutputBuff);
					TempAddress = TempAddress + (unsigned int)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail);
					pOutputBuff = (char*)TempAddress;
					/*
									(void) mem_cpy(pBuff+(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mTail),
									               &pSimpleBuffer->pBuffer[0],Size);
					*/
					(void) memcpy(pOutputBuff, &pSimpleBuffer->pBuffer[0],Size);
					pSimpleBuffer->mTail = (unsigned int)Size;
				}
			}
			else
			{
				returnvalue =  0;	/* short available data! */
			}
		}

		if (pSimpleBuffer->mTail  == pSimpleBuffer->mHead)
		{
			pSimpleBuffer->mState = BT_S_BUFFER_EMPTY;
		}
		else
		{
			pSimpleBuffer->mState = BT_S_BUFFER_NORMAL;
		}
	}

	return returnvalue;
}
/**************************************************************************
*  FUNCTION NAME :
*      			TC_Q_PutData()
*
*  DESCRIPTION :
*			Put data into the buffer at last(in FIFO structure)
*  INPUT:
*			BT_SRBUFFER* pBuffer : the pointer of struct
*			char * pInputBuff: the pointer of input buffer
*			long RequestSize : size of Requested data 	
*
*  OUTPUT:	int  - Return Type (state of buffer) 
*  	
*  REMARK  :
**************************************************************************/
int TC_Q_PutData(BT_SRBUFFER* pBuffer, char * pInputBuff, long RequestSize)
{
	unsigned  int lEmptySize;
	BT_SRBUFFER*	pSimpleBuffer;
	long Size;
	unsigned long TempAddress;
	int	returnvalue;
	Size = RequestSize;
	pSimpleBuffer = pBuffer;
	returnvalue = 1;

	if ( (pSimpleBuffer->mState != BT_S_BUFFER_FULL) && (RequestSize != 0) )
	{
		if ((unsigned int)Size <= pSimpleBuffer->mMaxBufferSize)
		{
			if ( pSimpleBuffer->mTail >  pSimpleBuffer->mHead )
			{
				lEmptySize =  pSimpleBuffer->mTail - pSimpleBuffer->mHead;

				if ( lEmptySize >= (unsigned int)Size )
				{
					(void) memcpy(&pSimpleBuffer->pBuffer[pSimpleBuffer->mHead],pInputBuff,Size);
					pSimpleBuffer->mHead+=(unsigned int)Size;
				}
				else
				{
					returnvalue = 0;
				}
			}
			else
			{
				lEmptySize = (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead) + pSimpleBuffer->mTail;

				if ( (lEmptySize >= (unsigned int)Size) || ((unsigned int)(pSimpleBuffer->mState == BT_S_BUFFER_EMPTY)))
				{
					if ( (pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead) >= (unsigned int)Size)
					{
						(void) memcpy(&pSimpleBuffer->pBuffer[pSimpleBuffer->mHead],pInputBuff,Size);
						pSimpleBuffer->mHead += (unsigned int)Size;
					}
					else
					{
						//				memcpy(&pSimpleBuffer->pSimpleBuffer[pSimpleBuffer->mTail],pBuff,(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead));
						(void) memcpy(&pSimpleBuffer->pBuffer[pSimpleBuffer->mHead],pInputBuff,(long)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead));
						Size -= (long)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead);
						TempAddress = (unsigned int)(pInputBuff);
						TempAddress = TempAddress + (unsigned int)(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead);
						pInputBuff = (char*)TempAddress;
						/*
										(void) memcpy(&pSimpleBuffer->pBuffer[0],pBuff+(pSimpleBuffer->mMaxBufferSize - pSimpleBuffer->mHead),
										               Size);
						*/
						(void) memcpy(&pSimpleBuffer->pBuffer[0],pInputBuff, Size);
						pSimpleBuffer->mHead = (unsigned int)Size;
					}
				}
				else
				{
					returnvalue = 0;
				}
			}

			if (pSimpleBuffer->mTail == pSimpleBuffer->mHead)
			{
				pSimpleBuffer->mState = BT_S_BUFFER_FULL;
			}
			else
			{
				pSimpleBuffer->mState = BT_S_BUFFER_NORMAL;
			}
		}
		else
		{
			returnvalue =0;
		}
	}
	else
	{
		returnvalue =0;
	}

	return returnvalue;
}

#ifdef TCM3800
#define MAC_PATH "/tmp/tcm3800_mac_address.bin"

int BTAPP_GetMacAddrFromEnv(unsigned char *pAddr)
{
    static  char MAC_ADDR[6] = {0, 0, 0, 0, 0, 0};
     char EmptyAddr[6] = {0, 0, 0, 0, 0, 0};
	FILE *fp = NULL;
	int fr = 0, Cnt = 0;
	unsigned char bFlag = 1;

	/* Check the existing address */
	fr = strncmp(MAC_ADDR, EmptyAddr, 6);
	if(fr != 0)
    {
		memcpy(pAddr, MAC_ADDR, 6);
		fr = 0;
		printf("BTAPP_GetMacAddrFromEnv : Mac address is exist : %02x:%02x:%02x:%02x:%02x:%02x\n", 
			MAC_ADDR[0], MAC_ADDR[1], MAC_ADDR[2], MAC_ADDR[3], MAC_ADDR[4], MAC_ADDR[5]);
		
		goto ErrExit;
	}

	do
    {
		/* Read WIFI Mac Address */	
		fp = fopen(MAC_PATH, "rb");
		if(fp != NULL)
		{
			int readlen = 0;
			printf("BTAPP_GetMacAddrFromEnv : fopen success!!!!\n");
			readlen = fread(MAC_ADDR, sizeof(unsigned char), 6, fp);
			printf("BTAPP_GetMacAddrFromEnv : fread len = %d\n", readlen);
			fclose(fp);
			if(readlen == 6)
			{
				bFlag = 0;
				memcpy(pAddr, MAC_ADDR, 6);				
			}
			else
			{
				printf("BTAPP_GetMacAddrFromEnv : fread len is not enought!!!\n");
			}
		}

		/* Wait 0.5 sec and retry read mac address */
		if(bFlag == 1)
		{
			usleep(500000);
			if(++Cnt > 10)
			{
				bFlag = 0;
				fr = -1;
			}
		}
	} while(bFlag);
ErrExit:
	return fr;
}

int BTAPP_GetBDAddr(TcBtUint24 *plap, TcBtUint8 *puap, TcBtUint16 *pnap)
{
	TcBtBdAddr BdAddrFromWLan = {0,0,0};
	unsigned char Addr[6] = {0, };
	int fr = 0;
	fr = BTAPP_GetMacAddrFromEnv(Addr);
	if(fr != 0)
    {
		goto ErrExit;
	}
	BdAddrFromWLan.lap = ((TcBtUint32)Addr[3]<<16) | ((TcBtUint32)Addr[4]<<8) | ((TcBtUint32)Addr[5]);
	BdAddrFromWLan.lap += 1;
	BdAddrFromWLan.uap = (TcBtUint8)Addr[2];
	BdAddrFromWLan.nap = ((TcBtUint16)Addr[0]<<8) | ((TcBtUint16)Addr[1]);

	*plap = BdAddrFromWLan.lap;
	*puap = BdAddrFromWLan.uap;
	*pnap = BdAddrFromWLan.nap;

ErrExit:
	return fr;
}

#endif
