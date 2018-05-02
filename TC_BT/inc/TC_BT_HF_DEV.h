/****************************************************************************
 *   FileName    : TC_BT_HF_DEV.h
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
*  Description : This file contains functions for a Bluetooth HF Device
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/08/31     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_HF_DEV_H__
#define __TC_BT_HF_DEV_H__

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
#define TC_BT_HF_DEV_RESULT_SUCCESS (0)
#define TC_BT_HF_DEV_RESULT_FAIL (-1)

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern TcBtBdAddr *TC_BT_HF_DEV_GetBdAddr(TcBtUint32 selectedNum);
extern TcBtUint32 TC_BT_HF_DEV_GetCurrentNum(void);
extern void TC_BT_HF_DEV_SetCurrentNum(TcBtUint32 i);
extern TcBtInt32 TC_BT_HF_DEV_GetNum(const TcBtBdAddr *pBdAddr);
extern int TC_BT_HF_DEV_SetNum(const TcBtBdAddr *pBdAddr);
extern int TC_BT_HF_DEV_Init(TcBtUint8 maxConnections);
extern int TC_BT_HF_DEV_Deinit(void);
extern int TC_BT_HF_DEV_Register(const TcBtBdAddr *pBdAddr);
extern int TC_BT_HF_DEV_Deregister(const TcBtBdAddr *pBdAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_HF_DEV_H__ */
