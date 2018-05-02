/****************************************************************************
 *   FileName    : TC_BT_SC_CMD.h
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
*  Description : This file contains functions for a Bluetooth Security Control command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/05/31     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_SC_CMD_H__
#define __TC_BT_SC_CMD_H__

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
typedef enum
{
	TC_BT_SC_BOND_REQ = 0,
	TC_BT_SC_DEBOND_REQ,
	TC_BT_SC_PASSKEY_RES,
	TC_BT_SC_AUTHORISE_RES,
	TC_BT_SC_SSP_NOTIFICATION_RES,
	TC_BT_SC_SSP_COMPARE_RES,
	TC_BT_SC_SSP_JUSTWORKS_RES,
	TC_BT_SC_DEBUG_MODE_REQ
} TcBtScCmdEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* SSP, Combined Input/Output Capabilities   */
#define TC_BT_IO_CAP_DISPLAY_ONLY				((TcBtUint8)0x00)
#define TC_BT_IO_CAP_DISPLAY_YES_NO				((TcBtUint8)0x01)
#define TC_BT_IO_CAP_KEYBOARD_ONLY				((TcBtUint8)0x02)
#define TC_BT_IO_CAP_NO_INPUT_NO_OUTPUT			((TcBtUint8)0x03)

/* SSP, Authentication_Requirements          */
#define TC_BT_MITM_NOT_REQUIRED_NO_BONDING        	((TcBtUint8)0x00)
#define TC_BT_MITM_REQUIRED_NO_BONDING            	((TcBtUint8)0x01)
#define TC_BT_MITM_NOT_REQUIRED_DEDICATED_BONDING 	((TcBtUint8)0x02)
#define TC_BT_MITM_REQUIRED_DEDICATED_BONDING     	((TcBtUint8)0x03)
#define TC_BT_MITM_NOT_REQUIRED_GENERAL_BONDING   	((TcBtUint8)0x04)
#define TC_BT_MITM_REQUIRED_GENERAL_BONDING       	((TcBtUint8)0x05)

/* Set the security mode to be used by the device          */
#define TC_BT_SEC_MODE0_OFF                   		((TcBtUint8) 0x00)
#define TC_BT_SEC_MODE1_NON_SECURE            		((TcBtUint8) 0x01)
#define TC_BT_SEC_MODE2_SERVICE               		((TcBtUint8) 0x02)
#define TC_BT_SEC_MODE3_LINK                  		((TcBtUint8) 0x03)
#define TC_BT_SEC_MODE4_SSP                   		((TcBtUint8) 0x04)

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	TcBtCmdType		type;
	TcBtBdAddr		bdAddr;
} TcBtScBondCmd;

typedef struct
{
	TcBtCmdType		type;
	TcBtBdAddr		bdAddr;
	TcBtUint16		bAccept;
} TcBtScAuthoirseCmd;

typedef struct
{
	TcBtCmdType		type;
	TcBtUint16		bAccept;
} TcBtScPairCmd;

typedef struct
{
	TcBtCmdType		type;
	TcBtUint8		PassKey[32];
	TcBtUint16		PassKeyLen;
} TcBtScLegacyPinCmd;

typedef struct
{
	TcBtCmdType		type;
	TcBtUint8		enable;
} TcBtScDebugModeCmd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_SC_Bond(TcBtBdAddr bdAddr);
extern void TC_BT_SC_SetIoCapability(TcBtUint8 ioCapability);
extern TcBtUint8 TC_BT_SC_GetIoCapability(void);
extern TcBtCharString *TC_BT_SC_DB_GetRemoteDeviceName(const TcBtBdAddr *pBdAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_SC_CMD_H__ */
