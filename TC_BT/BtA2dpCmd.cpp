/****************************************************************************
 *   FileName    : TC_BT_A2DP_CMD.c
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
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*
*******************************************************************************/
#include "BT_CFG.h"

#if defined(BT_AUDIO_SINK)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "TC_BT.h"
#include "TC_BT_A2DP_CMD.h"
#include "TC_BT_CMD.h"
#ifndef A2DP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

TcBtBdAddr   gTCBtA2dpCurrentAddr = {0, 0, 0};
unsigned int gTcBtA2dpFootprint   = 0;
unsigned int gTcBtA2dpStreamState = 0;

static int TC_BT_A2DP_DefaultCommand(TcBtCmdType type);

/*****************************************************************************
*
* debug
*
*****************************************************************************/

static int TC_BT_A2DP_DefaultCommand(TcBtCmdType type)
{
	TcBtA2dpDefaultCmd *pCmdStr;

    pCmdStr = (TcBtA2dpDefaultCmd *)TC_BT_Pmalloc(sizeof(TcBtA2dpDefaultCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = type;

		/* SUCCESS: Send Command */		
		(void)TC_BT_SendCommand(TC_BT_A2DP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_A2DP_Activate(void)
{
	TcBtA2dpActivateCmd *pCmdStr;

    pCmdStr = (TcBtA2dpActivateCmd *)TC_BT_Pmalloc(sizeof(TcBtA2dpActivateCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_A2DP_ACTIVATE_REQ;
		
		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_A2DP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;		
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_A2DP_Deactivate(void)
{

	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_DEACTIVATE_REQ);
}

int TC_BT_A2DP_Connect(const TcBtBdAddr *pBdAddr)
{
    TcBtA2dpConnectCmd *pCmdStr;

	if(TC_BT_BdAddrEqZero(pBdAddr) == TRUE)
	{
		return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	}
	
    pCmdStr = (TcBtA2dpConnectCmd *)TC_BT_Pmalloc(sizeof(TcBtA2dpConnectCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_A2DP_CONNECT_REQ;
		TC_BT_BdAddrCopyIfExist(&(pCmdStr->deviceAddr), pBdAddr);
		
		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_A2DP_CMD, pCmdStr);


		return TC_BT_CMD_RESULT_SUCCESS_ENUM;		
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_A2DP_Disconnect(void)
{

	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_DISCONNECT_REQ);
}

int TC_BT_A2DP_OpenConfig(void)
{
#if 0	/* No longer supported */
	
	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_OPEN_CONFIG_REQ);
#else
	return 0;
#endif	
}

int TC_BT_A2DP_StartStream(void)
{
#if 0	/* No longer supported */
	
	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_START_STREAM_REQ);
#else
	return 0;
#endif	
}

int TC_BT_A2DP_SuspendStream(void)
{
#if 0	/* No longer supported */
	
	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_SUSPEND_STREAM_REQ);
#else
	return 0;
#endif	
}

int TC_BT_A2DP_StopStream(void)
{
#if 0	/* No longer supported */
	
	return TC_BT_A2DP_DefaultCommand(TC_BT_A2DP_STOP_STREAM_REQ);
#else
	return 0;
#endif	
}

int TC_BT_A2DP_CancelConnect(const TcBtBdAddr *pBdAddr)
{
	TcBtA2dpCancelConnectCmd *pCmdStr;

    pCmdStr = (TcBtA2dpCancelConnectCmd *)TC_BT_Pmalloc(sizeof(TcBtA2dpCancelConnectCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_A2DP_CANCEL_CONNECT_REQ;
		TC_BT_BdAddrCopyIfExist(&(pCmdStr->deviceAddr), pBdAddr);
		
		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_A2DP_CMD, pCmdStr);

		if(pBdAddr != NULL)
		{
		}	
		else
		{
		}	

		return TC_BT_CMD_RESULT_SUCCESS_ENUM;		
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_A2DP_SetBypass(unsigned char flag)
{
	TcBtA2dpSetBypassCmd *pCmdStr;

    pCmdStr = (TcBtA2dpSetBypassCmd *)TC_BT_Pmalloc(sizeof(TcBtA2dpSetBypassCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_A2DP_SET_BYPASS_REQ;
		pCmdStr->flag = flag;

		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_A2DP_CMD, pCmdStr);

		return TC_BT_CMD_RESULT_SUCCESS_ENUM;		
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}
#endif	/* defined(BLUETOOTH_INCLUDE) && defined(BT_AUDIO_SINK) */
