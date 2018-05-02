/****************************************************************************
 *   FileName    : TC_BT_CM_CMD.c
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
*  Description : This file contains functions for a Bluetooth Connection Manager command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/14     0.1            created                     Kwon SunHong
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*
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

#include "inc/TC_BT_CM_CMD.h"
#include "inc/TC_BT_CM_EVT.h"
#ifndef GAP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

/*****************************************************************************
*
* debug
*
*****************************************************************************/

int TC_BT_CM_SetLocalName(TcBtUint8 *pstrLocalName)
{
	TcBtCmSetLocalNameCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
    pCmdStr = (TcBtCmSetLocalNameCmd *)TC_BT_Pmalloc(sizeof(TcBtCmCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SET_LOCAL_NAME_REQ;
	pCmdStr->pLocalName = TC_BT_StringAllocCopy(pstrLocalName);
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadLocalBdAddr(void)
{
	TcBtCmCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmCmd*)TC_BT_Pmalloc(sizeof(TcBtCmCmd));
	if(pCmdStr == NULL)
	{
		return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_LOCAL_BD_ADDR_REQ;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteScanEnable(TcBtUint8 disableInquiryScan, TcBtUint8 disablePageScan)
{
	TcBtCmWriteScanEnableCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmWriteScanEnableCmd *)TC_BT_Pmalloc(sizeof(TcBtCmWriteScanEnableCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_WRITE_SCAN_ENABLE_REQ;
	pCmdStr->disableInquiryScan = disableInquiryScan;
	pCmdStr->disablePageScan = disablePageScan;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadScanEnable(void)
{
	TcBtCmCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmCmd *)TC_BT_Pmalloc(sizeof(TcBtCmCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_SCAN_ENABLE_REQ;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_EnableDUTMode(TcBtBool enable)
{
	TcBtCmEnableDUTCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmEnableDUTCmd *)TC_BT_Pmalloc(sizeof(TcBtCmEnableDUTCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_ENABLE_DUT_MODE_REQ;
	pCmdStr->enable = enable;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WritePageTo(TcBtUint16 thePageTimeout)
{
	TcBtCmWritePageToCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmWritePageToCmd *)TC_BT_Pmalloc(sizeof(TcBtCmWritePageToCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_WRITE_PAGE_TO_REQ;
	pCmdStr->pageTimeout = thePageTimeout;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteCod(TcBtUint32 ServiceCod, TcBtUint32 MajorCod, TcBtUint32 MinorCod)
{
	TcBtCmWriteCodCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmWriteCodCmd *)TC_BT_Pmalloc(sizeof(TcBtCmWriteCodCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_WRITE_COD_REQ;
	pCmdStr->ServiceCod = ServiceCod;
	pCmdStr->MajorCod = MajorCod;
	pCmdStr->MinorCod = MinorCod;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadCod(void)
{
	TcBtCmCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmCmd *)TC_BT_Pmalloc(sizeof(TcBtCmCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_COD_REQ;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadRemoteName(TcBtBdAddr bdAddr)
{
	TcBtCmReadRemoteNameCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmReadRemoteNameCmd *)TC_BT_Pmalloc(sizeof(TcBtCmReadRemoteNameCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_REMOTE_NAME_REQ;
	pCmdStr->bdAddr = bdAddr;

	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_CancelReadRemoteName(TcBtBdAddr bdAddr)
{
	TcBtCmReadRemoteNameCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmReadRemoteNameCmd *)TC_BT_Pmalloc(sizeof(TcBtCmReadRemoteNameCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_CANCEL_READ_REMOTE_NAME_REQ;
	pCmdStr->bdAddr = bdAddr;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SdcSearchReqSend(TcBtBdAddr bdAddr, const TcBtUuid32 Service)
{
	TcBtCmSdcSearchCmd		*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmSdcSearchCmd *)TC_BT_Pmalloc(sizeof(TcBtCmSdcSearchCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SDC_SEARCH_REQ;
	pCmdStr->bdAddr = bdAddr;
	pCmdStr->Service = Service;
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SdcCancelSearchReqSend(TcBtBdAddr bdAddr)
{
	TcBtCmSdcCancelSearchCmd	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmSdcCancelSearchCmd *)TC_BT_Pmalloc(sizeof(TcBtCmSdcCancelSearchCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SDC_CANCEL_SEARCH_REQ;
	pCmdStr->bdAddr = bdAddr;
		
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SdcUuid128SearchReqSend(TcBtBdAddr bdAddr, const TcBtUuid128 Service)
{
	TcBtCmSdcUuid128SearchCmd	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmSdcUuid128SearchCmd *)TC_BT_Pmalloc(sizeof(TcBtCmSdcUuid128SearchCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SDC_UUID128_SEARCH_REQ;
	pCmdStr->bdAddr = bdAddr;
	memcpy(pCmdStr->Service, Service, sizeof(TcBtUuid128));
		
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);



	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SdcCancelUuid128SearchReqSend(TcBtBdAddr bdAddr)
{
	TcBtCmSdcCancelSearchCmd	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmSdcCancelSearchCmd *)TC_BT_Pmalloc(sizeof(TcBtCmSdcCancelSearchCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SDC_CANCEL_UUID128_SEARCH_REQ;
	pCmdStr->bdAddr.lap = bdAddr.lap;
	pCmdStr->bdAddr.uap = bdAddr.uap;
	pCmdStr->bdAddr.nap = bdAddr.nap;
		
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteLinkPolicyReqSend(TcBtBdAddr bdAddr, TcBtUint16 theLinkPolicySetting)
{
	TcBtCmWriteLinkPolicyCmd	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmWriteLinkPolicyCmd *)TC_BT_Pmalloc(sizeof(TcBtCmWriteLinkPolicyCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_WRITE_LINK_POLICY_REQ;
	pCmdStr->bdAddr = bdAddr;
	pCmdStr->theLinkPolicySetting = theLinkPolicySetting;
		
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadLinkPolicyReqSend(TcBtBdAddr bdAddr)
{
	TcBtCmReadLinkPolicyCmd	*pCmdStr = 0;

	/* Check for BT ON ??? */
	
	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmReadLinkPolicyCmd *)TC_BT_Pmalloc(sizeof(TcBtCmReadLinkPolicyCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_LINK_POLICY_REQ;
	pCmdStr->bdAddr = bdAddr;
		
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	
	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteLinkSuperVisionTimeoutReq(TcBtBdAddr bdAddr, TcBtUint16 timeout)
{
	TcBtCmWriteLinkSupervTimeoutCmd 	*pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
	
    pCmdStr = (TcBtCmWriteLinkSupervTimeoutCmd *)TC_BT_Pmalloc(sizeof(TcBtCmWriteLinkSupervTimeoutCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ;
	pCmdStr->bdAddr = bdAddr;
	pCmdStr->timeout = timeout;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_RoleSwitchConfigReq(TcBtUint32 config)
{
	TcBtCmRoleSwitchConfigCmd *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmRoleSwitchConfigCmd *)TC_BT_Pmalloc(sizeof(TcBtCmRoleSwitchConfigCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_ROLE_SWITCH_CONFIG_REQ;
	pCmdStr->config = config;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_GetLinkQualityReq(TcBtBdAddr bdAddr)
{
	TcBtCmGetLinkQualityReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmGetLinkQualityReq *)TC_BT_Pmalloc(sizeof(TcBtCmGetLinkQualityReq));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_GET_LINK_QUALITY_REQ;
	pCmdStr->deviceAddr = bdAddr;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadRssiReq(TcBtBdAddr bdAddr)
{
	TcBtCmReadRssiReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmReadRssiReq *)TC_BT_Pmalloc(sizeof(TcBtCmReadRssiReq));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_READ_RSSI_REQ;
	pCmdStr->deviceAddr = bdAddr;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_AclDetachReq(TcBtBdAddr bdAddr)
{
	TcBtCmAclDetachReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmAclDetachReq *)TC_BT_Pmalloc(sizeof(TcBtCmAclDetachReq));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_ACL_DETACH_REQ;
	pCmdStr->deviceAddr = bdAddr;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SetQOS(TcBtBdAddr deviceAddr, TcBtUint8 flag, TcBtUint8 service_type, TcBtInt32 token_rate, 
						TcBtInt32 peak_bandwidth, TcBtInt32	latency, TcBtInt32 delay_variation)
{
	TcBtCmQOSSetupReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmQOSSetupReq *)TC_BT_Pmalloc(sizeof(TcBtCmQOSSetupReq));
    if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SET_QOS_SETUP_REQ;
	pCmdStr->deviceAddr = deviceAddr;
	pCmdStr->flag = flag;
	pCmdStr->service_type = service_type;
	pCmdStr->token_rate = token_rate;
	pCmdStr->peak_bandwidth = peak_bandwidth;
	pCmdStr->latency = latency;
	pCmdStr->delay_variation = delay_variation;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

/*******************************************************************************
 **
 ** Function         TC_BT_CM_SetAfhChannels
 **
 ** Description      This function is used to avoid an AFH channel range
 **
 ** Parameters
 **                  FirstDisabledCh: first afh channel to avoid
 **                  LastDisabledCh: last afh channel to avoid
 **                  ex) 0, 5 : 2402MHz ~ 2406MHz is disabled.
 **
 ** Returns          0 if success / otherwise is error!
 **
 *******************************************************************************/

int TC_BT_CM_SetAfhChannels(TcBtUint8 FirstDisabledCh, TcBtUint8 LastDisabledCh)
{
	TcBtCmSetAfhChReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
    if ((FirstDisabledCh > LastDisabledCh) || (LastDisabledCh > 78))
    {
        return TC_BT_CMD_RESULT_INVALID_IDX_ENUM;
	}

    pCmdStr = (TcBtCmSetAfhChReq *)TC_BT_Pmalloc(sizeof(TcBtCmSetAfhChReq));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_SET_AFH_CHANNELS_REQ;
	pCmdStr->FirstDisabledCh = FirstDisabledCh;
	pCmdStr->LastDisabledCh = LastDisabledCh;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_Reset(void)
{
	TcBtCmCmd *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */

    pCmdStr = (TcBtCmCmd *)TC_BT_Pmalloc(sizeof(TcBtCmCmd));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_RESET;
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;	
}

int TC_BT_CM_VSC(TcBtUint16 opcode, TcBtUint16 length, TcBtUint8 *pdata)
{
	TcBtCmVSCReq *pCmdStr = 0;

	/* Check for BT ON ??? */

	/* FAIL: No BT ON -1 */
    if (length > TC_BT_VSC_DATA_LEN_MAX)
    {
        return TC_BT_CMD_RESULT_INVALID_IDX_ENUM;
	}

    pCmdStr = (TcBtCmVSCReq *)TC_BT_Pmalloc(sizeof(TcBtCmVSCReq));
	if(pCmdStr == NULL)
	{
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}

	pCmdStr->type = TC_BT_CM_VSC_CMD;
	pCmdStr->opcode = opcode;
	pCmdStr->length = length;
	memcpy(pCmdStr->data, pdata, length);
	
	(void)TC_BT_SendCommand(TC_BT_CM_CMD, pCmdStr);

	{	//Print data
		unsigned short 	i, quotient, remain, idx = 0;
		unsigned char 	*pB = pdata;

		quotient = length/16;
		remain = length%16;

		for(i= 0; i < quotient; i++)
		{
			idx = i*16;
			printf("0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", 
			pB[idx+0],pB[idx+1],pB[idx+2],pB[idx+3],pB[idx+4],pB[idx+5],pB[idx+6],pB[idx+7],pB[idx+8],pB[idx+9],pB[idx+10],pB[idx+11],pB[idx+12],pB[idx+13],pB[idx+14],pB[idx+15]);					
		}

		idx = quotient * 16;
		for(i = 0; i < remain; i++)
		{
			printf("0x%02x, ", pB[idx+i]);
		}

		printf("\n");
	}


	return TC_BT_CMD_RESULT_SUCCESS_ENUM;	
}

/*******************************************************************************
 **
 ** Function        TC_BT_CM_Bluetest_Tx
 **
 ** Description     This function is used to BLUETEST tx. Please refer to BLUE TEST guide document before using this function.
 **
 ** Parameters      
 **                 HoppingMode : 0 - 79 channel, 1 - single channel, 2 - Fixed Pattern.
 **                 Freq: It's valid only if HoppingMode is 1-single channel. 
 **                       frequency selected - 2402 (0 => 2402Mhz, 50 => 2452Mhz)
 **                 ModultionType: 1 - 0x00 8bit pattern, 2- 0xFF 8bit pattern, 3 - 0xAA 8bit pattern, 
 **                                9 - 0xF0 8bit pattern, 4 - PRBS Pattern
 **                 LogicalCh : 0 - ACL EDR, 1 - ACL BASIC, 2 - eSCO EDR, 3 - eSCO Basic, 4 - SCO Basic
 **                 BBPacketType : 0 - NULL, 1 - POLL, 2 - FHS, 3 - DM1, 4 - DH1/2-DH1, 5 - DV1, 6 - HV2/2-EV3
 **                                7 - HV3/EV3/3-EV3, 8 - DV/3-DH1, 9 - AUX1/PS, 10 - DM3/2-DH3, 11 - DH3/3-2H3
 **                                12 - EV4/2-EV5, 13 - EV5/3-EV5, 14 - DM5/2-DH5, 15 - DH5/3-DH5
 **                 BBPacketLen : Please refer to BLUE TEST guide document
 **                 TXPowerLevel : 0 - 0dbm, 1 - -4dbm, 2 - -8dbm, 3 - -12dbm, 4 - -16dbm, 5- -20dbm, 6 - -24dbm, 7 - -28dbm
 **                                8 - Specify Power in dBm, 9 - Specify Power Table index
 **                 TransmitPwoerdBM(-128 to 127 dbm) : It's valid only if TXPowerLevel is 8 - Specify Power in dBm.
 **                 TransmitPowerTableIdx(0 TO 7) : It's valid only if TXPowerLevel is 9 - Specify Power Table index.
 **
 ** Returns         status: 0 if success / otherwise is error!
 **
 *******************************************************************************/
