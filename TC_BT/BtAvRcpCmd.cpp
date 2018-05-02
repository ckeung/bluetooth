/****************************************************************************
 *   FileName    : TC_BT_AVRCP_CMD.c
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
*  Description : This file contains functions for a Bluetooth AVRCP command
*******************************************************************************
*
*	 yy/mm/dd	  ver			 descriptions				 Author
*	 ---------	  --------		 -----------------			 -----------------
*	 10/06/02	  0.1			 created					 Park, Kwang-Jin
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
#include "BTAPP.h"
#include "TC_BT_AVRCP_CMD.h"
#include "TC_BT_CMD.h"
#include "TC_BT_CM_CMD.h"
#ifndef A2DP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

unsigned long			gTcBtAvrcpNotification	= 0;
unsigned char			gTcBtAvrcpPasAttId		= 0;
unsigned char			gTcBtAvrcpFolderScope	= TC_BT_AVRCP_SCOPE_INVALID;
unsigned int			gTcBtAvrcpFolderStart	= 0;

unsigned int			gTcBtAvrcpUidCounter    = 0;
unsigned char			gTcBtAvrcpUidCurrent[8] = {0, 0, 0, 0, 0, 0, 0, 0};
#define BT_AV_CT_PAS_TEXT_SIZE		50		/* AVRCP PAS Attribute or Value Text Size */
#if defined(BT_AUDIO_SINK_AVRCP_PAS)
unsigned short					gBtAvRemoteVersion;
unsigned char					gBtAvPasValidList[BTAPP_AV_CT_PAS_ATT_END];
unsigned char					gBtAvPasCurrValue[BTAPP_AV_CT_PAS_ATT_END];
unsigned char					gBtAvPasEventAtt;
unsigned char					gBtAvPasEventVal;
unsigned char					gBtAvPasEventText[BT_AV_CT_PAS_TEXT_SIZE];
#endif
static int TC_BT_AVRCP_DefaultCommand(TcBtCmdType type);

/*****************************************************************************
*
* debug
*
*****************************************************************************/

static int TC_BT_AVRCP_DefaultCommand(TcBtCmdType type)
{
	TcBtAvrcpDefaultCmd *pCmdStr;

    pCmdStr = (TcBtAvrcpDefaultCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpDefaultCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = type;

		/* SUCCESS: Send Command */		
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_Activate(unsigned char *service_name, unsigned char *provider_name)
{
	TcBtAvrcpActivateCmd *pCmdStr;

    pCmdStr = (TcBtAvrcpActivateCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpActivateCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_ACTIVATE_REQ;
	#if   defined(BT_AUDIO_SINK_AVRCP_BROWSING)
		pCmdStr->avrcpVersion = TC_BT_AVRCP_CONFIG_SR_VERSION_15;
	#elif defined(BT_AUDIO_SINK_AVRCP_METADATA) 
		pCmdStr->avrcpVersion = TC_BT_AVRCP_CONFIG_SR_VERSION_13;
	#else
		pCmdStr->avrcpVersion = TC_BT_AVRCP_CONFIG_SR_VERSION_10;
	#endif

		if(service_name != NULL)
		{
            pCmdStr->nameOfService = (TcBtString *)TC_BT_Pmalloc(strlen((char *)service_name));
            memcpy((char *)pCmdStr->nameOfService, (char *)service_name, strlen((char *)service_name));
		}
		else
		{
			return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
		}

		if(provider_name != NULL)
		{
            pCmdStr->nameOfProvider = (TcBtString *)TC_BT_Pmalloc(strlen((char *)provider_name));
            memcpy((char *)pCmdStr->nameOfProvider, (char *)provider_name, strlen((char *)provider_name));
		}
		else
		{
			return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
		}

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_Deactivate(void)
{
	return TC_BT_AVRCP_DefaultCommand(TC_BT_AVRCP_DEACTIVATE_REQ);
}

int TC_BT_AVRCP_Connect(const TcBtBdAddr *pBdAddr)
{
	TcBtAvrcpConnectCmd *pCmdStr;

	if(TC_BT_BdAddrEqZero(pBdAddr) == TRUE)
	{
		return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	}

    pCmdStr = (TcBtAvrcpConnectCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpConnectCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_AVRCP_CONNECT_REQ;
		TC_BT_BdAddrCopyIfExist(&(pCmdStr->deviceAddr), pBdAddr);
		
		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);


		return TC_BT_CMD_RESULT_SUCCESS_ENUM;		
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_Disconnect(void)
{
	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	return TC_BT_AVRCP_DefaultCommand(TC_BT_AVRCP_DISCONNECT_REQ);
}

int TC_BT_AVRCP_CancelConnect(const TcBtBdAddr *pBdAddr)
{
#if 0	/* No longer supported */
	TcBtAvrcpCancelConnectCmd *pCmdStr;

	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCancelConnectCmd));
	if(pCmdStr != NULL)
	{	
		pCmdStr->type = TC_BT_AVRCP_CANCEL_CONNECT_REQ;
		TC_BT_BdAddrCopyIfExist(&(pCmdStr->deviceAddr), pBdAddr);
		
		/* SUCCESS: Send Command */			
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);

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
#else
	return 0;
#endif
}	

int TC_BT_AVRCP_CtInformDispCharset(TcBtUint8 charsetCount, const TcBtAvrcpCharSet *charset)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtInformDispCharsetCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtInformDispCharsetCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ;
		pCmdStr->charsetCount = charsetCount;
		pCmdStr->charset = TC_BT_Pmalloc(sizeof(TcBtAvrcpCharSet));
		memcpy(pCmdStr->charset, charset, sizeof(TcBtAvrcpCharSet));	

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
#else
	return 0;
#endif
}

int TC_BT_AVRCP_CtNotiRegister(TcBtAvrcpNotiMask mask, TcBtUint32 playbackInterval, TcBtAvrcpNotiRegConfigMask config)
{
	TcBtAvrcpCtNotiRegisterCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtNotiRegisterCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtNotiRegisterCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_NOTI_REGISTER_REQ;
		pCmdStr->mask = mask;
		pCmdStr->playbackInterval = playbackInterval;
		pCmdStr->config = config;		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_CtPassThrough(TcBtAvrcpPTOpId key, TcBtAvrcpPTState state)
{
	TcBtAvrcpCtPassThroughCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}

    pCmdStr = (TcBtAvrcpCtPassThroughCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPassThroughCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PASS_THROUGH_REQ;
		pCmdStr->key = key;
		pCmdStr->state = state;		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_CtPasAttId(void)
{
	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}

#if defined(BT_AUDIO_SINK_AVRCP_PAS)
	memset(gBtAvPasValidList, 0, sizeof(gBtAvPasValidList));
	if(gBtAvRemoteVersion < TC_BT_AVRCP_CONFIG_SR_VERSION_13)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
#endif
	
	return TC_BT_AVRCP_DefaultCommand(TC_BT_AVRCP_CT_PAS_ATT_ID_REQ);
}

int TC_BT_AVRCP_CtPasValId(TcBtAvrcpPasAttId attId)
{
	TcBtAvrcpCtPasValIdCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtPasValIdCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPasValIdCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PAS_VAL_ID_REQ;
		pCmdStr->attId = attId;
		gTcBtAvrcpPasAttId = attId;

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_CtPasAttTxt(TcBtUint8 attIdCount, const TcBtAvrcpPasAttId *attId)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtPasAttTxtCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPasAttTxtCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PAS_ATT_TXT_REQ;
		pCmdStr->attIdCount = attIdCount;
		pCmdStr->attId = TC_BT_Pmalloc(attIdCount*sizeof(TcBtAvrcpPasAttId));
		memcpy(pCmdStr->attId, attId, attIdCount*sizeof(TcBtAvrcpPasAttId));		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
#else
	return 0;
#endif
}

int TC_BT_AVRCP_CtPasValTxt(TcBtAvrcpPasAttId attId, TcBtUint8 valIdCount, const TcBtAvrcpPasValId *valId)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtPasValTxtCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPasValTxtCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PAS_VAL_TXT_REQ;
		pCmdStr->attId = attId;
		pCmdStr->valIdCount = valIdCount;		
		pCmdStr->valId =  TC_BT_Pmalloc(valIdCount*sizeof(TcBtAvrcpPasValId));
		memcpy(pCmdStr->valId, valId, valIdCount*sizeof(TcBtAvrcpPasValId));		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
#else
	return 0;
#endif
}

int TC_BT_AVRCP_CtPasCurrent(TcBtUint8 attIdCount, const TcBtAvrcpPasAttId *attId)
{
	TcBtAvrcpCtPasCurrentCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}

#if defined(BT_AUDIO_SINK_AVRCP_PAS)
    memset((char *)gBtAvPasCurrValue, 0, sizeof(gBtAvPasCurrValue));
	if(gBtAvRemoteVersion < TC_BT_AVRCP_CONFIG_SR_VERSION_13)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
#endif

    pCmdStr = (TcBtAvrcpCtPasCurrentCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPasCurrentCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PAS_CURRENT_REQ;
		pCmdStr->attIdCount = attIdCount;
        pCmdStr->attId = (TcBtAvrcpPasAttId *)TC_BT_Pmalloc(attIdCount*sizeof(TcBtAvrcpPasAttId));
		memcpy(pCmdStr->attId, attId, attIdCount*sizeof(TcBtAvrcpPasAttId));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_CtPasSet(TcBtUint8 attValPairCount, const TcBtAvrcpPasAttValPair *attValPair)
{
	TcBtAvrcpCtPasSetCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtPasSetCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPasSetCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PAS_SET_REQ;
		pCmdStr->attValPairCount = attValPairCount;		
        pCmdStr->attValPair = (TcBtAvrcpPasAttValPair *)TC_BT_Pmalloc(attValPairCount*sizeof(TcBtAvrcpPasAttValPair));
		memcpy(pCmdStr->attValPair, attValPair, attValPairCount*sizeof(TcBtAvrcpPasAttValPair));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
}

int TC_BT_AVRCP_CtGetPlayStatus(void)
{
	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	return TC_BT_AVRCP_DefaultCommand(TC_BT_AVRCP_CT_GET_PLAY_STATUS_REQ);
}

int TC_BT_AVRCP_CtGetAttributes(TcBtAvrcpScope scope, const TcBtUint8 *uid, TcBtUint16 uidCounter, TcBtAvrcpItemAttMask attributeMask)
{
	TcBtAvrcpCtGetAttributesCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtGetAttributesCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtGetAttributesCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_GET_ATTRIBUTES_REQ;
		pCmdStr->scope = scope;
		memcpy(pCmdStr->uid, uid, sizeof(TcBtAvrcpUid));
		pCmdStr->uidCounter = uidCounter;
		pCmdStr->attributeMask = attributeMask;

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtGetFolderItems(TcBtAvrcpScope scope, TcBtUint32 startItem, TcBtUint32 endItem, TcBtAvrcpItemAttMask attributeMask)
{
	TcBtAvrcpCtGetFolderItemsCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtGetFolderItemsCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtGetFolderItemsCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ;
		pCmdStr->scope = scope;
		pCmdStr->startItem = startItem;
		pCmdStr->endItem = endItem;
		pCmdStr->attributeMask = attributeMask;
		gTcBtAvrcpFolderScope = scope;
		gTcBtAvrcpFolderStart = startItem;

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtPlay(TcBtAvrcpScope scope, TcBtUint16 uidCounter, const TcBtUint8 *uid)
{
	TcBtAvrcpCtPlayCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtPlayCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtPlayCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_PLAY_REQ;
		pCmdStr->scope = scope;
		pCmdStr->uidCounter = uidCounter;
		memcpy(pCmdStr->uid, uid, sizeof(TcBtAvrcpUid));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtSearch(const TcBtString *text)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtSearchCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtSearchCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_SEARCH_REQ;
		pCmdStr->text = TC_BT_Pmalloc(strlen(text) + 1);
		(void)strcpy(pCmdStr->text, text);

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
#else
	return 0;
#endif
}

int TC_BT_AVRCP_CtChangePath(TcBtUint16 uidCounter, TcBtAvrcpFolderDirection folderDir, const TcBtUint8 *folderUid)
{
	TcBtAvrcpCtChangePathCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtChangePathCmd*)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtChangePathCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_CHANGE_PATH_REQ;
		pCmdStr->uidCounter = uidCounter;
		pCmdStr->folderDir = folderDir;
		memcpy(pCmdStr->folderUid, folderUid, sizeof(TcBtAvrcpUid));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtSetVolume(TcBtUint8 volume)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtSetVolumeCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtSetVolumeCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_SET_VOLUME_REQ;
		pCmdStr->volume = volume;

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
#else
	return 0;
#endif
}

int TC_BT_AVRCP_CtSetAddressedPlayer(TcBtUint32 playerId)
{
	TcBtAvrcpCtSetAddressedPlayerCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtSetAddressedPlayerCmd *)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtSetAddressedPlayerCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ;
		pCmdStr->playerId = playerId;
	#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
		gstBtAvBrowsing.AddressedPlayerID = playerId;
	#endif		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtSetBrowsedPlayer(TcBtUint32 playerId)
{
	TcBtAvrcpCtSetBrowsedPlayerCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtSetBrowsedPlayerCmd*)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtSetBrowsedPlayerCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ;
		pCmdStr->playerId = playerId;
	#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
		gstBtAvBrowsing.BrowsedPlayerID	= playerId;
	#endif		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtAddToNowPlaying(TcBtAvrcpScope scope, TcBtUint16 uidCounter, const TcBtUint8 *uid)
{
	TcBtAvrcpCtAddToNowPlayingCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
    pCmdStr = (TcBtAvrcpCtAddToNowPlayingCmd*)TC_BT_Pmalloc(sizeof(TcBtAvrcpCtAddToNowPlayingCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ;
		pCmdStr->scope = scope;
		pCmdStr->uidCounter = uidCounter;
		memcpy(pCmdStr->uid, uid, sizeof(TcBtAvrcpUid));		

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
}

int TC_BT_AVRCP_CtInformBatteryStatus(TcBtAvrcpBatteryStatus batStatus)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtInformBatteryStatusCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtInformBatteryStatusCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ;
		pCmdStr->batStatus = batStatus;

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}	
#else
	return 0;
#endif	
}

int TC_BT_AVRCP_CtUnitInfoCmd(TcBtUint16 pDataLen, const TcBtUint8 *pData)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtUnitInfoCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtUnitInfoCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_UNIT_INFO_CMD_REQ;
		pCmdStr->pDataLen = pDataLen;		
		pCmdStr->pData = TC_BT_Pmalloc(pDataLen*sizeof(TcBtUint8));
		memcpy(pCmdStr->pData, pData, pDataLen*sizeof(TcBtUint8));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
#else
	return 0;
#endif	
}

int TC_BT_AVRCP_CtSubUnitInfoCmd(TcBtUint16 pDataLen, const TcBtUint8 *pData)
{
#if 0	/* No longer supported */
	TcBtAvrcpCtSubUnitInfoCmd *pCmdStr;

	if(TC_BT_CM_GetAVRCPStatus(NULL) != TC_BT_CM_CONNECTED)
	{
		return TC_BT_CMD_RESULT_DISCONNECTED_ENUM;
	}
	
	pCmdStr = TC_BT_Pmalloc(sizeof(TcBtAvrcpCtSubUnitInfoCmd));
	if(pCmdStr != NULL)
	{
		pCmdStr->type = TC_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ;
		pCmdStr->pDataLen = pDataLen;		
		pCmdStr->pData = TC_BT_Pmalloc(pDataLen*sizeof(TcBtUint8));
		memcpy(pCmdStr->pData, pData, pDataLen*sizeof(TcBtUint8));

		/* SUCCESS: Send Command */
		(void)TC_BT_SendCommand(TC_BT_AVRCP_CMD, pCmdStr);
		return TC_BT_CMD_RESULT_SUCCESS_ENUM;
	}
	else
	{
		/* FAIL: No free Memory */
		return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
	}
#else
	return 0;
#endif	
}

#endif /* defined(BT_AUDIO_SINK) */
