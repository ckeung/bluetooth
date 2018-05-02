/****************************************************************************
 *   FileName    : TC_BT_CMD.h
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
*  Description : This file contains functions for a Bluetooth command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/05/31     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_CMD_H__
#define __TC_BT_CMD_H__

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
typedef TcBtUint16 TcBtCmdType;

typedef enum
{
	TC_BT_CMD_TYPE_ENUM_BASE = 0xCC00,
	TC_BT_A2DP_CMD_TYPE_ENUM,
	TC_BT_AVRCP_CMD_TYPE_ENUM,
	TC_BT_HF_CMD_TYPE_ENUM,
	TC_BT_PAC_CMD_TYPE_ENUM,
	TC_BT_SPP_CMD_TYPE_ENUM,
	TC_BT_SC_CMD_TYPE_ENUM,
	TC_BT_SD_CMD_TYPE_ENUM,
	TC_BT_CM_CMD_TYPE_ENUM,
	TC_BT_MAPC_CMD_TYPE_ENUM,
	TC_BT_HIDD_CMD_TYPE_ENUM,	
	TC_BT_PAN_CMD_TYPE_ENUM
} TcBtCmdTypeEnum;

typedef enum
{
	TC_BT_CMD_RESULT_SUCCESS_ENUM = 0,
	TC_BT_CMD_RESULT_BT_OFF_ENUM,
	TC_BT_CMD_RESULT_INVALID_ARGUMENT_ENUM,
	TC_BT_CMD_RESULT_DISCONNECTED_ENUM,
	TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM,
	TC_BT_CMD_RESULT_INVALID_IDX_ENUM,
	TC_BT_CMD_RESULT_SERVICE_RECORD_TOO_BING_ENUM,

	TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM
} TcBtCmdResultEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define TC_BT_A2DP_CMD (TC_BT_A2DP_CMD_TYPE_ENUM)
#define TC_BT_AVRCP_CMD (TC_BT_AVRCP_CMD_TYPE_ENUM)
#define TC_BT_HF_CMD (TC_BT_HF_CMD_TYPE_ENUM)
#define TC_BT_PAC_CMD (TC_BT_PAC_CMD_TYPE_ENUM)
#define TC_BT_SPP_CMD (TC_BT_SPP_CMD_TYPE_ENUM)
#define TC_BT_SC_CMD (TC_BT_SC_CMD_TYPE_ENUM)
#define TC_BT_SD_CMD (TC_BT_SD_CMD_TYPE_ENUM)
#define TC_BT_CM_CMD (TC_BT_CM_CMD_TYPE_ENUM)
#define TC_BT_MAPC_CMD (TC_BT_MAPC_CMD_TYPE_ENUM)
#define TC_BT_HIDD_CMD (TC_BT_HIDD_CMD_TYPE_ENUM)
#define TC_BT_PAN_CMD (TC_BT_PAN_CMD_TYPE_ENUM)

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    TcBtCmdType                 type;
} TcBtCmd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_SendCommand(TcBtCmdType cmdType, void *pCmdStr);

//#include "TC_BT_CMD.h"
//#include "TC_BT_SYS_CMD.h"
//#include "TC_BT_HF_CMD.h"
//#include "TC_BT_A2DP_CMD.h"
//#include "TC_BT_AVRCP_CMD.h"
//#include "TC_BT_PAC_CMD.h"
//#include "TC_BT_SPP_CMD.h"
//#include "TC_BT_SC_CMD.h"
//#include "TC_BT_SD_CMD.h"
//#include "TC_BT_CM_CMD.h"
//#include "TC_BT_MAPC_CMD.h"
//#include "TC_BT_HIDD_CMD.h"
//#include "TC_BT_PAN_CMD.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_CMD_H__ */
