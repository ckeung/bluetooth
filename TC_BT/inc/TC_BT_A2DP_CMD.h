/****************************************************************************
 *   FileName    : TC_BT_A2DP_CMD.h
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
*  Description : This file contains functions for a Bluetooth A2DP command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/06/02     0.1            created                     Park, Kwang-Jin
*
*******************************************************************************/

#ifndef __TC_BT_A2DP_CMD_H__
#define __TC_BT_A2DP_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtBase.h"
#include "BtUtil.h"
#include "BT_CFG.h"
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
/* AVRCP Cotroller Player Application Setting */

typedef enum
{
	TC_BT_A2DP_ACTIVATE_REQ,                  
	TC_BT_A2DP_DEACTIVATE_REQ,
	TC_BT_A2DP_CONNECT_REQ,
	TC_BT_A2DP_CANCEL_CONNECT_REQ,
	TC_BT_A2DP_DISCONNECT_REQ,
	TC_BT_A2DP_DISCOVER_REQ,
	TC_BT_A2DP_GET_CAPABILITIES_REQ,
	TC_BT_A2DP_SET_CONFIGURATION_REQ,
	TC_BT_A2DP_GET_CONFIGURATION_REQ,
	TC_BT_A2DP_RECONFIGURE_REQ,
	TC_BT_A2DP_OPEN_REQ,
	TC_BT_A2DP_START_REQ,
	TC_BT_A2DP_CLOSE_REQ,
	TC_BT_A2DP_SUSPEND_REQ,
	TC_BT_A2DP_ABORT_REQ,
	TC_BT_A2DP_SECURITY_CONTROL_REQ,
	TC_BT_A2DP_STREAM_DATA_REQ,
	TC_BT_A2DP_REGISTER_STREAM_HANDLE_REQ,
	TC_BT_A2DP_SECURITY_IN_REQ,
	TC_BT_A2DP_SECURITY_OUT_REQ,
	TC_BT_A2DP_SET_QOS_INTERVAL_REQ,
	TC_BT_A2DP_LP_NEG_CONFIG_REQ,
	TC_BT_A2DP_DELAY_REPORT_REQ,

	TC_BT_A2DP_OPEN_CONFIG_REQ,
	TC_BT_A2DP_START_STREAM_REQ,
	TC_BT_A2DP_SUSPEND_STREAM_REQ,
	TC_BT_A2DP_STOP_STREAM_REQ,
	TC_BT_A2DP_SET_BYPASS_REQ,
	
	TC_BT_A2DP_END
} TcBtA2dpCmdEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
/* Codec Type */
#define A2DP_CODEC_NONE				0x00
#define A2DP_CODEC_SBC				0x01 /* Mandatory */
#define A2DP_CODEC_MPEG24_AAC		0x02 /* Optional  */

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	TcBtCmdType 			type;							/* primitive/message identity */
} TcBtA2dpCmd;

typedef TcBtA2dpCmd TcBtA2dpDefaultCmd;
typedef TcBtA2dpCmd TcBtA2dpDeactivateCmd;
typedef TcBtA2dpCmd TcBtA2dpDisconnectCmd;
typedef TcBtA2dpCmd TcBtA2dpOpenConfigCmd;
typedef TcBtA2dpCmd TcBtA2dpStartStreamCmd;
typedef TcBtA2dpCmd TcBtA2dpSuspendStreamCmd;
typedef TcBtA2dpCmd TcBtA2dpStopStreamCmd;

typedef struct
{
	TcBtCmdType 			type;
} TcBtA2dpActivateCmd;

typedef struct
{
	TcBtCmdType 			type;
 	TcBtBdAddr				deviceAddr;
} TcBtA2dpConnectCmd;

typedef struct
{
	TcBtCmdType 			type;
	TcBtBdAddr				deviceAddr;
} TcBtA2dpCancelConnectCmd;

typedef struct
{
	TcBtCmdType 			type;
	TcBtUint32				option;	 
} TcBtA2dpScmsEnableCmd;

typedef struct
{
	TcBtCmdType 			type;
	TcBtUint8				flag;	 
} TcBtA2dpSetBypassCmd;

typedef struct
{
	TcBtCmdType 			type;
	TcBtUint8				optionalCodec;
} TcBtA2dpSetOptionalCodecCmd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_A2DP_Activate(void);
extern int TC_BT_A2DP_Deactivate(void);
extern int TC_BT_A2DP_Connect(const TcBtBdAddr *pBdAddr);
extern int TC_BT_A2DP_Disconnect(void);
extern int TC_BT_A2DP_OpenConfig(void);
extern int TC_BT_A2DP_StartStream(void);
extern int TC_BT_A2DP_SuspendStream(void);
extern int TC_BT_A2DP_StopStream(void);
extern int TC_BT_A2DP_CancelConnect(const TcBtBdAddr *pBdAddr);
extern int TC_BT_A2DP_SetBypass(unsigned char flag);

extern TcBtBdAddr   gTCBtA2dpCurrentAddr;
extern unsigned int gTcBtA2dpFootprint;
extern unsigned int gTcBtA2dpStreamState;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_A2DP_CMD_H__ */
