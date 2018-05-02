/****************************************************************************
 *   FileName    : TC_BT_UTIL.h
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
*  Description : This file contains functions for a Bluetooth utility
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/05/31     0.1            created                     Ji, Seung-Nam
*    10/07/08     0.2            Patch for copy addr         Ji, Seung-Nam
*                                String alloc and copy added
*    10/07/09     0.3            Patch for BdAddr            Ji, Seung-Nam
*    10/09/20     0.4            Patch TC_BT_StringAllocCopy Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_UTIL_H__
#define __TC_BT_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtDataType.h"
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
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct 
{
	TcBtInt32	Head;
	TcBtInt32	Tail;	
	TcBtUint32 	*pQueueBuf;
	TcBtInt32	QueueSize;
} TcBtQueueCtx;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#ifdef TC_BT_CHK_MEM_LEAK
extern void* TC_BT_ChkML_Malloc(unsigned int size, const char *file, unsigned int line);
#define TC_BT_Pmalloc(s) TC_BT_ChkML_Malloc((s),__FILE__, __LINE__)
#else
extern void *TC_BT_Pmalloc(TcBtUint32 size);
#endif

#ifdef TC_BT_CHK_MEM_LEAK
extern void TC_BT_ChkML_Free(void *ptr, const char *file, unsigned int line);
#define TC_BT_Pfree(p) TC_BT_ChkML_Free((p), __FILE__, __LINE__)
#else
extern void TC_BT_Pfree(void *ptr);
#endif

extern void TC_BT_BdAddrCopyIfExist(TcBtBdAddr *pDstBdAddr, const TcBtBdAddr *pSrcBdAddr);
extern TcBtUint8 TC_BT_BdAddrEq(const TcBtBdAddr *pBdAddr1, const TcBtBdAddr *pBdAddr2);
extern TcBtUint8 TC_BT_BdAddrEqZero(const TcBtBdAddr *pBdAddr);
extern void TC_BT_BdAddrZero(TcBtBdAddr *pBdAddr);

extern TcBtString *TC_BT_StringAllocCopy(TcBtString *pSrcStr);

extern void TC_BT_QueueInit(TcBtQueueCtx *pCtx, void *pQueueBuf, TcBtInt32 QuebufSize);	/* Initialize the queue */
extern TcBtBool TC_BT_QueueEnqueue(TcBtQueueCtx *pCtx, void *ptr);							/* Enter an item in the queue */
extern void* TC_BT_QueueDequeue(TcBtQueueCtx *pCtx);										/* Remove an item from the queue */
extern TcBtBool TC_BT_QueueIsEmpty(TcBtQueueCtx *pCtx);										/* Return TRUE if queue is empty */
extern TcBtBool TC_BT_QueueIsFull(TcBtQueueCtx *pCtx);										/* Return TRUE if queue is full */
typedef int (*TcBtEvtCb)(void *pTcBtEvt);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_UTIL_H__ */
