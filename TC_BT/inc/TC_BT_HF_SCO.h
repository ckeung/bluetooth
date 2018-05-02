/****************************************************************************
 *   FileName    : TC_BT_HF_SCO.h
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
*  Description : This file contains functions for a Bluetooth HF SCO
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/31     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_HF_SCO_H__
#define __TC_BT_HF_SCO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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
#define TC_BT_HF_SCO_RESULT_SUCCESS (0)
#define TC_BT_HF_SCO_RESULT_FAIL (-1)

#define TC_BT_HF_NO_SCO_HANDLE (0xFFFF)

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	TcBtBdAddr bdAddr;
	TcBtUint16 scoHandle;
} TcBtHfScoEntry;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern TcBtBdAddr *TC_BT_HF_SCO_GetBdAddr(TcBtUint16 scoHandle);
extern int TC_BT_HF_SCO_Init(TcBtUint8 maxConnections);
extern int TC_BT_HF_SCO_Deinit(void);
extern int TC_BT_HF_SCO_DataCallback(TcBtUint8 *theData);
extern int TC_BT_HF_SCO_RegisterHandle(const TcBtBdAddr *pBdAddr, TcBtUint16 scoHandle);
extern int TC_BT_HF_SCO_DeregisterHandle(const TcBtBdAddr *pBdAddr, TcBtUint16 scoHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_HF_SCO_H__ */
