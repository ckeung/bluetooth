/****************************************************************************
 *   FileName    : TC_BT_SYS_CMD.h
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
*  Description : This file contains functions for a Bluetooth SPP command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    11/10/18     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_SYS_CMD_H__
#define __TC_BT_SYS_CMD_H__

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
typedef enum
{
	TC_BT_SYS_Idle = 0, 
	TC_BT_SYS_Activating, 
	TC_BT_SYS_Activated, 
	TC_BT_SYS_Deactivating
} TC_BT_Status;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#define TC_BT_SOCKET_PATH "/var/run/"

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
extern int BsaAppEntry(const char *p_uipc_path);
extern int BsaAppExit(void);

extern TcBtInt8 gBtLocalDeviceName[TC_BT_MAX_FRIENDLY_NAME_LEN+1];
extern TcBtBdAddr gBtLocalDeviceAddr;

extern void TC_BT_SYS_SetLocalDeviceName(void);
extern void TC_BT_SYS_SetLocalDeviceAddr(void);
extern int TC_BT_SYS_GetBTStatus(void);
extern void TC_BT_SYS_SetBTStatus(int Status);
extern int TC_BT_SYS_Activate(void);
extern int TC_BT_SYS_Deactivate(void);
extern unsigned int TC_BT_SYS_GetTick(void);
extern TcBtString *TC_BT_SYS_GetTime(void);
extern void TC_BT_SYS_PrintLinkKey(TcBtBdAddr *pBdAddr, TcBtUint8 *pRemoteName, TcBtUint8 *k, TcBtUint8 linkkeyType, TcBtBool linkkeyValid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_SYS_CMD_H__ */
