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

#ifndef BTAPP_PLATFORM_H
#define BTAPP_PLATFORM_H

//#include "../inc/BtDataType.h"
#include "inc/BtBase.h"
/*****************************************************************************
*
* miscellaneous
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* data types
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#if 1   /* Telechips Linux HF remark */
typedef enum
{
    BTAPP_TIMERMODE_NOTUSED,
    BTAPP_TIMERMODE_ONCE,
    BTAPP_TIMERMODE_REPEAT,

    BTAPP_TIMERMODE_MAX
} BTAPP_TIMER_MODE;
#endif

/* TODO: Add your specialized code here */
#define TIMER_BT_RING_TIMEOUT_TIME	100 /* tick of 10s */
#define TIMER_BT_CALL_TIMESTAMP_TIME	10 /* tick of 1s */
#define TIMER_BT_CALL_END_TIMEOUT_TIME	30 /* tick of 3s */
#define TIMER_BT_CHLD_TIMEOUT_TIME	30 /* tick of 3s */
#define TIMER_BT_OUTGOING_END_TIMEOUT_TIME	600 /* tick of 60s */
#define TIMER_BT_CLCC_TIMEOUT_TIME	15 /* tick of 1.5s */
/* tick of 5s(includes margin 1s), iphone 3gs, 5s
 * connection during incoming call + pbap overhead minimum 4s */
#define TIMER_BT_PRIVATE_NUMBER_TIMEOUT_TIME	50
#define TIMER_BT_CLCC_CALL_HELD_TIMEOUT_TIME	5	/* tick of 0.5s */

#define TC_BT_TIMER_MASK_SET_DEVNO(mask,devno,begin,end)	(unsigned int)((mask)				\
                                                            +((devno)*(((end)-(begin))+1)))
#define TC_BT_TIMER_MASK_GET_ID(mask,begin,end)				(unsigned int)(((mask)/*-begin*/)	\
                                                            -(((((mask)-(begin))				\
                                                            /(((end)-(begin))+1)))				\
                                                            *(((end)-(begin))+1)))
#define TC_BT_TIMER_MASK_GET_DEVNO(mask,begin,end)			(unsigned int)((((mask)-(begin))	\
                                                            /(((end)-(begin))+1)))

/* HFP */
#define TC_BT_TIMER_SET_HFP(id,devno)						TC_BT_TIMER_MASK_SET_DEVNO(id			\
                                                            ,devno/*TC_BT_HF_COMMON_GetCurrDevNum()*/	\
                                                            ,TIMER_BT_HFx_BEGIN,TIMER_BT_HFx_END)
#define TC_BT_TIMER_GET_HFP_DEVNO(id)						TC_BT_TIMER_MASK_GET_DEVNO(id			\
                                                            ,TIMER_BT_HFx_BEGIN,TIMER_BT_HFx_END)

typedef enum
{
    TIMER_BT_BEGIN,
    /*TIMER_BT_READ_AVAILABLE_SERVICE_ID=TIMER_BT_BEGIN,
    TIMER_BT_CONNECT_RETRY_ID,*/
    TIMER_BT_CONNECT_STREAM_ID = TIMER_BT_BEGIN,
    TIMER_BT_CONNECT_TIMEOUT_ID,
    TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID,
    TIMER_BT_DISCONNECT_STREAM_ID,
    TIMER_BT_DISCONNECT_TIMEOUT_ID,
    TIMER_BT_DEACTIVATE_TIMEOUT_ID,
    TIMER_BT_ACTIVATE_TIMEOUT_ID,
    /*TIMER_BT_DISCONNECT_ID0,
    TIMER_BT_DISCONNECT_ID1,
    TIMER_BT_DISCONNECT_ID2,
    TIMER_BT_DISCONNECT_ID3,
    TIMER_BT_DISCONNECT_ID4,
    TIMER_BT_DISCONNECT_ID5,
    TIMER_BT_DISCONNECT_ID6,
    TIMER_BT_DISCONNECT_ID7,
    TIMER_BT_DISCONNECT_ID8,*/
    TIMER_BT_DISCONNECT_AT_BTOFF,
    /*TIMER_BT_DB_DISCONNECT_ID,
    TIMER_BT_DB_READ_RSSI_ID,*/
    TIMER_BT_AUTOCONNECT_CANCEL_ID,
#ifdef BT_AUDIO_SINK
    TIMER_BT_AV_STREAM_CHECK_ID,
    TIMER_BT_AV_PAS_CHECK_ID,
    TIMER_BT_AV_META_CHECK_ID,
    TIMER_BT_AV_BROWSE_SUPPORT_ID,
    TIMER_BT_AV_STREAM_ANALYZE_ID,
	TIMER_BT_AV_GET_MPLIST_ID,    
    //TIMER_BT_AV_STREAM_SUSPEND_ID,
    TIMER_BT_AV_STREAM_STOP_ID,
    //TIMER_BT_AVRCP_CONNECT_ID,
    //TIMER_BT_AVRCP_DISCONNECT_ID,
    TIMER_BT_AVRCP_CONTROL_BY_PLAY_STATUS_ID,
    TIMER_BT_AVRCP_CONTROL_RETRY_ID,
#endif
/* Telechips Linux HF remark */
#ifdef BT_HANDSFREE_UNIT
    TIMER_BT_HFx_MULTI_BEGIN,
    TIMER_BT_HFx_BEGIN=TIMER_BT_HFx_MULTI_BEGIN,
    TIMER_BT_HFx_CLCC_ID=TIMER_BT_HFx_BEGIN, /* Telechips' 2008.12 QA - 3.6 (a), (b) */
    TIMER_BT_HFx_COPS_ID,	
    TIMER_BT_HFx_RING_ID,
    TIMER_BT_HFx_CALL_TIMESTAMP_ID,
    TIMER_BT_HFx_CALL_END_TIMEOUT_ID,
    TIMER_BT_HFx_OUTGOING_END_TIMEOUT_ID,
    TIMER_BT_HFx_AUTO_ANSWER_ID,
    TIMER_BT_HFx_OUTGOING_RETRY_ID,
    TIMER_BT_HFx_PERIODIC_EVENT_ID,
    TIMER_BT_HFx_AUDIO_TRANSFER_ID,
    TIMER_BT_HFx_PRIVATE_NUMBER_ID,

    /* Enhanced call timeout */
    TIMER_BT_HFx_RELEASE_ALL_HELD_CALL,          /* AT+CHLD=0  */
    TIMER_BT_HFx_RELEASE_ACTIVE_ACCEPT,          /* AT+CHLD=1  */
    TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,         /* AT+CHLD=1x */
    TIMER_BT_HFx_HOLD_ACTIVE_ACCEPT,             /* AT+CHLD=2  */
    TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED, /* AT+CHLD=2x */
    TIMER_BT_HFx_ADD_CALL,                       /* AT+CHLD=3  */
    TIMER_BT_HFx_CONNECT_TWO_CALLS,              /* AT+CHLD=4  */
    TIMER_BT_HFx_CLCC_CALL_HELD_TWO,	         /* CALLHELD 2 */

    TIMER_BT_HSx_RING_ID,
    TIMER_BT_HFx_END=TIMER_BT_HSx_RING_ID,
    TIMER_BT_HFx_MULTI_END=TIMER_BT_HFx_END
                            +(BT_HFP_HF_CONNECTIONS-1)
                            *(TIMER_BT_HFx_END-TIMER_BT_HFx_BEGIN+1),
#endif
    TIMER_BT_PB_DOWNLOADTIME_ID,
    /*TIMER_BT_PB_WAITRESPONSE_ID,*/

    TIMER_BT_END=TIMER_BT_PB_DOWNLOADTIME_ID,

    TIMER_NUM_MAX
} _TIMER_ID;

typedef	enum BT_SIMPLE_BUFFER_STATE
{
	BT_S_BUFFER_EMPTY = 1,
	BT_S_BUFFER_NORMAL,
	BT_S_BUFFER_FULL
}	BT_S_STATE;

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    unsigned char	fUsed;
    unsigned char   ulTmrId;
    short           lTicks;
    short           lRepeatTicks;
    void			(*pvTimerExpire)(unsigned char, unsigned char);
} _TIMER_BLOCK;

typedef struct BT_SIMPLE_RINGBUFFER_FOR_STREAM
{
	unsigned int 	mHead,mTail;
	unsigned int	mMaxBufferSize;
	char *pBuffer;
	BT_S_STATE mState;

} BT_SRBUFFER;


/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int BTAPP_Create_Lock(void);
extern int BTAPP_Destory_Unlock(void);
extern void BTAPP_Lock(void);
extern void BTAPP_Unlock(void);

extern void TCCAPI_Timer_ClearAll(void);
extern void TCCAPI_Timer_CancelTime(unsigned char);
extern int TCCAPI_Timer_SetTime(short ulMsTicks, unsigned char ulTmrId, void (*pvTimerExpiration)(unsigned char, unsigned char));
extern int TCCAPI_Timer_SetRepeatedTime(short ulMsTicks, unsigned char ulTmrId, BTAPP_TIMER_MODE repeat, void (*pvTimerExpiration)(unsigned char, unsigned char));
extern int TCCAPI_Timer_BT_IsActiveTime(unsigned char ulTmrId);
extern void TCCAPI_Timer_CheckTime(void);
//extern void Timer_TCCTimefn(void);
extern int str_len16(void *pString);

extern void TC_Q_InitBuffer(BT_SRBUFFER* pBuffer,unsigned int size,char* pBuff);
extern void TC_Q_DeinitBuffer(BT_SRBUFFER* pBuffer, char *pBuff);
extern int TC_Q_GetData(BT_SRBUFFER* pBuffer, char * pOutputBuff, long RequestSize);
extern int TC_Q_PutData(BT_SRBUFFER* pBuffer, char * pInputBuff, long RequestSize);
extern int TC_Q_GetDataSize(BT_SRBUFFER* pBuffer);
extern int TC_Q_GetAvailableSpace(BT_SRBUFFER* pBuffer);

#ifdef TCM3800
extern int BTAPP_GetBDAddr(unsigned long int *plap, unsigned char *puap, unsigned short int  *pnap);
#endif

//extern int TC_BT_SendEvent(TcBtEvt *pTcBtEvt);
extern int TC_BT_SYS_GetBTStatus(void);
#endif // BTAPP_PLATFORM_H
