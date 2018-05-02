/****************************************************************************
 *   FileName    : BTAPP_GAP.c
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
*    11/01/06     0.1            created                     Kwon Sunhong
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
#include "TC_BT.h"
#include "BtApp_Gap.h"
#include "BtPhoneBook.h"
#include "BtApp_Av.h"
#include "TcBtEvt.h"
#include"BtSysCmd.h"
#include "TC_BT/inc/TC_BT_A2DP_CMD.h"
#include "TC_BT/inc/BtDmCmd.h"
#include "TC_BT/inc/BtPlatform.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/TC_BT_CMD.h"
#include "TC_BT/inc/TC_BT_PAC_CMD.h"
#include "TC_BT/inc/TC_BT_CM_CMD.h"

#include "TC_BT/inc/BtHfCmd.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "TC_BT/inc/TC_BT_HF_DEV.h"
#ifndef GAP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

#define	TC_BT_RETRY_CONNECT_MAX			1
#define TC_BT_RETRY_RAS_MAX				2
#define TC_BT_CONNECT_TIMEOUT			50*5 /* 25sec */
#define TC_BT_DISCONNECT_TIMEOUT		50*5 /* 25sec */
#define TC_BT_CANCEL_CONNECT_TIMEOUT	50*5 /* 25sec */
#define TC_BT_DEACTIVATE_TIMEOUT		20*5 /* 10sec */
#define TC_BT_ACTIVATE_TIMEOUT			20*5 /* 10sec */

unsigned int gBtAppGapActiveProfile = 0 
#ifdef IPOD_BT_AUTO_ACTIVATE
		| BT_PROFILEMASK_IAP
#endif
#ifdef BT_AUTO_ACTIVATE_SPP
		| BT_PROFILEMASK_SPP
#endif
#ifdef BT_AUTO_ACTIVATE_AUDIOSINK
		| BT_PROFILEMASK_A2DP
#endif
#ifdef BT_AUTO_ACTIVATE_HANDSFREE
		| BT_PROFILEMASK_HF
#endif		
#ifdef BT_AUTO_ACTIVATE_PAC
		| BT_PROFILEMASK_PBAP
#endif
#ifdef BT_AUTO_ACTIVATE_MAPC
		| BT_PROFILEMASK_MAP
#endif
#ifdef BT_AUTO_ACTIVATE_HIDD
		| BT_PROFILEMASK_HIDD
#endif
#ifdef BT_AUTO_ACTIVATE_PAN
        | BT_PROFILEMASK_PAN
#endif
		;

static TcBtBool bBtAppGapAuthoriseFlag = FALSE;

unsigned char gSD_SearchFlag = FALSE;

//**************************static fun*****************************//


int BtApp_PAC_ActivateReq(void)
{
    TcBtPacActivateCmd *pCmdStr;
 printf("============ %s ============ \n",__FUNCTION__);
    pCmdStr = (TcBtPacActivateCmd *)TC_BT_Pmalloc(sizeof(TcBtPacActivateCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_ACTIVATE_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);


        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtAPp_PAC_ConnectReq(const TcBtBdAddr *BdAddr, TcBtUint16 pacMaxObexPacketSize)
{
    TcBtPacCmd *pCmdStr;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

        if(pCmdStr != NULL)
        {
            pCmdStr->type = TC_BT_PAC_CONNECT_REQ;
            pCmdStr->pacMaxObexPacketSize = pacMaxObexPacketSize;
            TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)BdAddr);

            (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

            /* SUCCESS: Send Command */
            return TC_BT_CMD_RESULT_SUCCESS_ENUM;
        }
        else
        {
            /* FAIL: No free Memory */
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
}


int BtApp_PAC_CancelConnectReq(void)
{
    TcBtPacCmd *pCmdStr;

    pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_CANCELCONNECT_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}



int BtApp_PAC_DisconnectReq(void)
{
    TcBtPacCmd *pCmdStr;

    pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_DISCONNECT_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);


        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

/*****************************************************************************
*
* debug
*
*****************************************************************************/

#if 0
void BtApp_GAP_TestOnOff(unsigned char id, unsigned char loc)
{
//	char status;
	static unsigned int uiCallTimer1=0,uiCallTimer2=50;

	loc=0;

#if 1 //def DMP_MESSAGE_INCLUDE
	DMP_SetLocationByDisk((unsigned char)DISK_DEVICE_BT_SYS, (unsigned char)loc);
		//DMP_SetLocationByDisk((unsigned char)DISK_DEVICE_BT_AV, (unsigned char)loc);
		//DMP_SetLocationByDisk((unsigned char)DISK_DEVICE_BT_HF, (unsigned char)loc);
#endif	

	if(BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_TASK_INIT)==TRUE)
	{
	
		BtApp_SYS_Off();
	}
	else
	{
	
		BtApp_SYS_On();
	}
#if 0
	TCCAPI_Timer_SetTime(5*uiCallTimer2, TIMER_BT_TEST_ONOFF_ID,BtApp_GAP_TestOnOff);
#endif

	//if(status)
	{
		if((++uiCallTimer1)%1==0)
		{
			uiCallTimer1=0;
			uiCallTimer2++;
		}

		if(uiCallTimer2>60)
		{
			uiCallTimer2=50;
		}
	}
}
#endif

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
								FOR ACTIVATE
**************************************************************************/
void BtApp_GAP_SetAllProfileActivation(void)
{
	gBtAppGapActiveProfile = 0 
#ifdef IPOD_BT_AUTO_ACTIVATE
		| BT_PROFILEMASK_IAP
#endif
#ifdef BT_AUTO_ACTIVATE_SPP
		| BT_PROFILEMASK_SPP
#endif
#ifdef BT_AUTO_ACTIVATE_AUDIOSINK
		| BT_PROFILEMASK_A2DP
#endif
#ifdef BT_AUTO_ACTIVATE_HANDSFREE
		| BT_PROFILEMASK_HF
#endif		
#ifdef BT_AUTO_ACTIVATE_PAC
		| BT_PROFILEMASK_PBAP
#endif
#ifdef BT_AUTO_ACTIVATE_MAPC
		| BT_PROFILEMASK_MAP
#endif
#ifdef BT_AUTO_ACTIVATE_HIDD
		| BT_PROFILEMASK_HIDD
#endif
#ifdef BT_AUTO_ACTIVATE_PAN
        | BT_PROFILEMASK_PAN
#endif
		;
}

void BtApp_GAP_SetMultiProfileActivation(unsigned int profile)
{
	gBtAppGapActiveProfile |= profile;
}

void BtApp_GAP_ClearMultiProfileActivation(unsigned int profile)
{
	gBtAppGapActiveProfile &= ~profile;
}

unsigned int BtApp_GAP_GetMultiProfileActivation(void)
{
	return gBtAppGapActiveProfile;
}

void BtApp_GAP_ActivateProfilesAutomatically(void)
{
#ifdef BT_AUTO_ACTIVATE_AUDIOSINK
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_A2DP) != 0)
	{
		BtApp_AV_SNK_Activate();
	}
#endif

#ifdef BT_AUTO_ACTIVATE_HANDSFREE
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_HF) != 0)
	{
        (void)G_BtHfCmd->TC_BT_HF_Activate();
	}
#endif	

#ifdef BT_AUTO_ACTIVATE_SPP
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_SPP) != 0)
	{
		(void)TC_BT_SPP_Activate(0);	
	}
#endif

#ifdef IPOD_BT_AUTO_ACTIVATE
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_IAP) != 0)
	{
		(void)TC_BT_SPP_IAP_Activate();
	}
#endif

#ifdef BT_AUTO_ACTIVATE_PAC
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_PBAP) != 0)
	{
        (void)BtApp_PAC_ActivateReq();
	}
#endif
#ifdef BT_AUTO_ACTIVATE_MAPC
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_MAP) != 0)
	{
		(void)TC_BT_MAPC_ActivateReq();
	}
#endif
#ifdef BT_AUTO_ACTIVATE_HIDD
	if((gBtAppGapActiveProfile & BT_PROFILEMASK_HIDD) != 0)
	{
		TC_BT_HIDD_Activate();
	}
#endif
#ifdef BT_AUTO_ACTIVATE_PAN
    if((gBtAppGapActiveProfile & BT_PROFILEMASK_PAN) != 0)
    {
        // TODO PAN ACTIVATE
        TC_BT_PAN_Activate();
    }
#endif

	//TC_BT_SPP_ExActivateTest();	
}

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
								FOR CONNECT
**************************************************************************/
/**********************
 *  PRIVATE FUNCTION  *
 **********************/
/* list function */
static void BtApp_GAP_ListInit(BtAppGapList_t *list, BtAppGapListRemoveFunc_t removeFunc);
static void BtApp_GAP_ListDeinit(BtAppGapList_t *list);
static void BtApp_GAP_ListElementAddFirst(BtAppGapList_t *list, BtAppGapListElm_t *element);
static void BtApp_GAP_ListElementAddLast(BtAppGapList_t *list, BtAppGapListElm_t *element);
static void BtApp_GAP_ListElementRemove(BtAppGapList_t *list, BtAppGapListElm_t *element);
static void BtApp_GAP_ListElementRelease(BtAppGapList_t *list, BtAppGapListElm_t *element);
static void BtApp_GAP_ListElementMoveLast(BtAppGapList_t *list, BtAppGapListElm_t *element);
static BtAppGapListElm_t *BtApp_GAP_ListSearch(const BtAppGapList_t *list, BtAppGapListSearchFunc_t searchFunc, const void *data);
static void BtApp_GAP_ConnDeviceRemove(BtAppGapListElm_t *element);
static TcBtBool BtApp_GAP_DevConnStatusFind(BtAppGapListElm_t *element, void *arg);
static TcBtBool BtApp_GAP_ConnDeviceFind(BtAppGapListElm_t *element, void *arg);
static TcBtBool BtApp_GAP_ConnProfileFind(BtAppGapListElm_t *element, void *arg);
static void BtApp_GAP_DiscDeviceRemove(BtAppGapListElm_t *element);
static TcBtBool BtApp_GAP_DiscDeviceFind(BtAppGapListElm_t *element, void *arg);
static TcBtBool BtApp_GAP_DiscProfileFind(BtAppGapListElm_t *element, void *arg);
static TcBtBool BtApp_GAP_RecoveryProfileFind(BtAppGapListElm_t *element, void *arg);
static BtAppGapMainConnStatusInst_t *BtApp_GAP_GetMainConnStatusInst(void);
static BtAppGapConnInstance_t *BtApp_GAP_GetConnInst(void);
static BtAppGapCcanInstance_t *BtApp_GAP_GetCcanInst(void);
static BtAppGapDiscInstance_t *BtApp_GAP_GetDiscInst(void);
static BtAppGapRecoveryInstance_t *BtApp_GAP_GetRecoveryInst(void);
static BtAppGapHfpConnStatusInfo_t *BtApp_GAP_GetHfpConnStatusInfo(void);
static BtAppGapA2dpConnStatusInfo_t *BtApp_GAP_GetA2dpConnStatusInfo(void);
static BtAppGapAvrcpConnStatusInfo_t *BtApp_GAP_GetAvrcpConnStatusInfo(void);
static BtAppGapPacConnStatusInfo_t *BtApp_GAP_GetPacConnStatusInfo(void);
static BtAppGapMapcConnStatusInfo_t *BtApp_GAP_GetMapcConnStatusInfo(void);
static BtAppGapSppConnStatusInfo_t *BtApp_GAP_GetSppConnStatusInfo(void);
static BtAppGapIapConnStatusInfo_t *BtApp_GAP_GetIapConnStatusInfo(void);
static TcBtBool BtApp_GAP_IsConnectable(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid);
static TcBtBool BtApp_GAP_IsInServiceList(TcBtUint16 profileUuid, const TcBtUint32 *pServiceList, TcBtUint32 serviceCount);
static TcBtUint32 BtApp_GAP_DeactivateCommonFunc(TcBtUint16 profileUuid, TcBtBdAddr *pBdAddr, TcBtUint8 instanceIdx);
static void BtApp_GAP_ConnectForTimer(TcBtUint8 id, TcBtUint8 loc);
static void BtApp_GAP_ConnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc);
static void BtApp_GAP_CancelConnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc);
static TcBtInt32 BtApp_GAP_CancelCurrentOperation(void);
static TcBtUint16 BtApp_GAP_GetConnStatus(void);
static void BtApp_GAP_SetConnStatus(TcBtUint32 connStatus);
static TcBtUint16 BtApp_GAP_GetCcanStatus(void);
static void BtApp_GAP_SetCcanStatus(TcBtUint32 ccanStatus);

static TcBtUint16 BtApp_GAP_GetDiscStatus(void);
static void BtApp_GAP_SetDiscStatus(TcBtUint32 discStatus);
static void BtApp_GAP_DisconnectCommonFunc(void);
static void BtApp_GAP_DisconnectForTimer(TcBtUint8 id, TcBtUint8 loc);
static void BtApp_GAP_DisconnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc);
static void BtApp_GAP_ActivateForRefreshing(void);
static void BtApp_GAP_DeactivateTimeoutHandler(TcBtUint8 id, TcBtUint8 loc);
static void BtApp_GAP_ActivateTimeoutHandler(TcBtUint8 id, TcBtUint8 loc);

/* main instance for connection status */
static BtAppGapMainConnStatusInst_t mainConnStatusInst;
/* main instance for connect process */
static BtAppGapConnInstance_t connInst;
/* main instance for disconnect process */
static BtAppGapDiscInstance_t discInst;
/* main instance for connect_cancel process */
static BtAppGapCcanInstance_t ccanInst;
/* main instance for recovery process */
static BtAppGapRecoveryInstance_t recoveryInst;

static void BtApp_GAP_ListInit(BtAppGapList_t *list, BtAppGapListRemoveFunc_t removeFunc)
{
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	list->removeFunc = removeFunc;
}

static void BtApp_GAP_ListDeinit(BtAppGapList_t *list)
{
	BtAppGapListElm_t *element, *nextElement;
	
	for(element = list->first; element != NULL; element = nextElement)
	{
		nextElement = element->next;
		BtApp_GAP_ListElementRemove(list, element);
	}

	list->first = NULL;
	list->last = NULL;
	list->count = 0;
}

static void BtApp_GAP_ListElementAddFirst(BtAppGapList_t *list, BtAppGapListElm_t *element)
{
	element->next = list->first;
	element->prev = NULL;
	
	if(list->first != NULL)
	{/* An element is already present in the list */
		list->first->prev = element;
	}
	else
	{/* This is the first element in the list */
		list->last = element;
	}
	
	list->first = element;
	list->count++;
}

static void BtApp_GAP_ListElementAddLast(BtAppGapList_t *list, BtAppGapListElm_t *element)
{
	element->next = NULL;
	element->prev = list->last;
	
	if(list->last != NULL)
	{/* An element is already present in the list */
		list->last->next = element;
	}
	else
	{/* This is the first element in the list */
		list->first = element;
	}
	
	list->last = element;
	list->count++;
}

static void BtApp_GAP_ListElementRemove(BtAppGapList_t *list, BtAppGapListElm_t *element)
{
	if(element->prev != NULL)
	{/* This is not the first element in the list */
		element->prev->next = element->next;
	}
	else
	{/* This is the first element in the list */
		list->first = element->next;
	}
	
	if(element->next != NULL)
	{/* This is not the last element in the list */
		element->next->prev = element->prev;
	}
	else
	{/* This is the last element in the list */
		list->last = element->prev;
	}
	
	list->count--;

	if(list->removeFunc != NULL)
	{
		list->removeFunc(element);
	}
	
	TC_BT_Pfree(element);
}

static void BtApp_GAP_ListElementRelease(BtAppGapList_t *list, BtAppGapListElm_t *element)
{
	if(element->prev != NULL)
	{/* This is not the first element in the list */
		element->prev->next = element->next;
	}
	else
	{/* This is the first element in the list */
		list->first = element->next;
	}
	
	if(element->next != NULL)
	{/* This is not the last element in the list */
		element->next->prev = element->prev;
	}
	else
	{/* This is the last element in the list */
		list->last = element->prev;
	}
	
	list->count--;
	
	element->next = NULL;
	element->prev = NULL;
}

static void BtApp_GAP_ListElementMoveLast(BtAppGapList_t *list, BtAppGapListElm_t *element)
{
	if(list->last == element)
	{/* This is already last of the list */
		return;
	}
	
	/* Update old links around src */
	if(element->prev != NULL)
	{
		element->prev->next = element->next;
	}
	else
	{
		list->first = element->next;
	}
	
	if(element->next != NULL)
	{
		element->next->prev = element->prev;
	}
	else /* The entry to move is the last in the list */
	{
		list->last = element->prev;
	}

	/* Update new links around last */
	element->next = NULL;
	element->prev = list->last;
	
	if(list->last != NULL)
	{/* An element is already present in the list */
		list->last->next = element;
	}
	else
	{/* This is the first element in the list */
		list->first = element;
	}
	
	list->last = element;
}

static BtAppGapListElm_t *BtApp_GAP_ListSearch(const BtAppGapList_t *list, BtAppGapListSearchFunc_t searchFunc, const void *data)
{
	BtAppGapListElm_t *currentElem;

	for(currentElem = list->first; currentElem != NULL; currentElem = currentElem->next)
	{
        if(searchFunc(currentElem, (void*)data) == TRUE)
		{
			break;
		}
	}

	return currentElem;
}

/* connProfList remove function */
static void BtApp_GAP_ConnDeviceRemove(BtAppGapListElm_t *element)
{
	BtAppGapConnDevListElm_t *pConnDevice = (BtAppGapConnDevListElm_t *)element;

	/* remove stored service list */
	TC_BT_Pfree(pConnDevice->pServiceList);
	pConnDevice->serviceCount = 0;
	pConnDevice->retryCountForRas = 0;

	/* remove connProfList for a device */
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pConnDevice->connProfList);
}

/* devConnStatusList search function */
static TcBtBool BtApp_GAP_DevConnStatusFind(const BtAppGapListElm_t *element, void *arg)
{
	BtAppGapDevConnStatusListElm_t *pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)element;
	TcBtBdAddr *pBdAddr = (TcBtBdAddr *)arg;

	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&pDevConnStatus->bdAddr, (const TcBtBdAddr *)pBdAddr) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* connDevList search function */
static TcBtBool BtApp_GAP_ConnDeviceFind(const BtAppGapListElm_t *element, void *arg)
{
	BtAppGapConnDevListElm_t *pConnDevice = (BtAppGapConnDevListElm_t *)element;
	TcBtBdAddr *pBdAddr = (TcBtBdAddr *)arg;

	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&pConnDevice->bdAddr, (const TcBtBdAddr *)pBdAddr) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* connProfList search function */
static TcBtBool BtApp_GAP_ConnProfileFind(const BtAppGapListElm_t *element, void *arg)
{
	BtAppGapConnProfListElm_t *pConnProfile = (BtAppGapConnProfListElm_t *)element;
	BtAppGapProfUuidInstIdx *pSearchData = (BtAppGapProfUuidInstIdx *)arg;

	if((pConnProfile->profileUuid == pSearchData->profileUuid) && (pConnProfile->instanceIdx == pSearchData->instanceIdx))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* discProfList remove function */
static void BtApp_GAP_DiscDeviceRemove(BtAppGapListElm_t *element)
{
	BtAppGapDiscDevListElm_t *pDiscDevice = (BtAppGapDiscDevListElm_t *)element;
	
	/* remove connProfList for a device */
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pDiscDevice->readyToDiscProfList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pDiscDevice->waitForRespProfList);
}

/* discDevList search function */
static TcBtBool BtApp_GAP_DiscDeviceFind(const BtAppGapListElm_t *element, void *arg)
{
	BtAppGapDiscDevListElm_t *pConnDevice = (BtAppGapDiscDevListElm_t *)element;
	TcBtBdAddr *pBdAddr = (TcBtBdAddr *)arg;

	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&pConnDevice->bdAddr, (const TcBtBdAddr *)pBdAddr) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* discProfList search function */
static TcBtBool BtApp_GAP_DiscProfileFind(const BtAppGapListElm_t *element, void *arg)
{
	BtAppGapDiscProfListElm_t *pConnProfile = (BtAppGapDiscProfListElm_t *)element;
	BtAppGapProfUuidInstIdx *pSearchData = (BtAppGapProfUuidInstIdx *)arg;

	if((pConnProfile->profileUuid == pSearchData->profileUuid) && (pConnProfile->instanceIdx == pSearchData->instanceIdx))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* recoveryList search function */
static TcBtBool BtApp_GAP_RecoveryProfileFind(const BtAppGapListElm_t *element, void *arg)
{
    BtAppGapRecoveryListElm_t *pRecoveryProfile = (BtAppGapRecoveryListElm_t *)element;
	BtAppGapProfUuidInstIdx *pSearchData = (BtAppGapProfUuidInstIdx *)arg;

	if((pRecoveryProfile->profileUuid == pSearchData->profileUuid) && (pRecoveryProfile->instanceIdx == pSearchData->instanceIdx))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
							FOR CONNECTION STATUS
**************************************************************************/
void BtApp_GAP_InitMainConnStatusInst(void)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	
	memset(pMainConnStatusInst, 0x00, sizeof(BtAppGapMainConnStatusInst_t));
	pMainConnStatusInst->hfp.idleInstCount = BTAPP_GAP_HFP_MAX_CONN_INST;
	pMainConnStatusInst->a2dp.idleInstCount = 1;
	pMainConnStatusInst->avrcp.idleInstCount = 1;
	pMainConnStatusInst->pac.idleInstCount = 1;
	pMainConnStatusInst->mapc.idleInstCount = BTAPP_GAP_MAPC_MAX_CONN_INST;
	pMainConnStatusInst->spp.idleInstCount = BTAPP_GAP_SPP_MAX_CONN_INST;
	pMainConnStatusInst->iap.idleInstCount = 1;
	pMainConnStatusInst->hidd.idleInstCount = 1;
	pMainConnStatusInst->pan.idleInstCount = 1;
}

static BtAppGapMainConnStatusInst_t *BtApp_GAP_GetMainConnStatusInst(void)
{
	return &mainConnStatusInst;
}

static BtAppGapConnInstance_t *BtApp_GAP_GetConnInst(void)
{
	return &connInst;
}

static BtAppGapCcanInstance_t *BtApp_GAP_GetCcanInst(void)
{
	return &ccanInst;
}

static BtAppGapDiscInstance_t *BtApp_GAP_GetDiscInst(void)
{
	return &discInst;
}

static BtAppGapRecoveryInstance_t *BtApp_GAP_GetRecoveryInst(void)
{
	return &recoveryInst;
}

static BtAppGapHfpConnStatusInfo_t *BtApp_GAP_GetHfpConnStatusInfo(void)
{
	return &mainConnStatusInst.hfp;
}

static BtAppGapA2dpConnStatusInfo_t *BtApp_GAP_GetA2dpConnStatusInfo(void)
{
	return &mainConnStatusInst.a2dp;
}

static BtAppGapAvrcpConnStatusInfo_t *BtApp_GAP_GetAvrcpConnStatusInfo(void)
{
	return &mainConnStatusInst.avrcp;
}

static BtAppGapPacConnStatusInfo_t *BtApp_GAP_GetPacConnStatusInfo(void)
{
	return &mainConnStatusInst.pac;
}

static BtAppGapMapcConnStatusInfo_t *BtApp_GAP_GetMapcConnStatusInfo(void)
{
	return &mainConnStatusInst.mapc;
}
 
static BtAppGapSppConnStatusInfo_t *BtApp_GAP_GetSppConnStatusInfo(void)
{
	return &mainConnStatusInst.spp;
}

static BtAppGapIapConnStatusInfo_t *BtApp_GAP_GetIapConnStatusInfo(void)
{
	return &mainConnStatusInst.iap;
}

static BtAppGapHiddConnStatusInfo_t *BtApp_GAP_GetHiddConnStatusInfo(void)
{
	return &mainConnStatusInst.hidd;
}

static BtAppGapPanConnStatusInfo_t *BtApp_GAP_GetPanConnStatusInfo(void)
{
	return &mainConnStatusInst.pan;
}
 
TcBtUint16 BtApp_GAP_CheckCondition(const TcBtBdAddr *pBdAddr, const TcBtUint16 action, const TcBtUint16 side)
{
	TcBtUint16 ret = BTAPP_GAP_COND_UNKNOWN;
	
	switch(action)
	{
		case BTAPP_GAP_ACT_CONNECT:
		{
			if((side & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
			{
				ret = BtApp_GAP_CheckConditionForConnectByLocal(pBdAddr);
			}
			else
			{
				ret = BtApp_GAP_CheckConditionForConnectByRemote(pBdAddr);
			}
			break;
		}
		case BTAPP_GAP_ACT_DISCONNECT:
		{
			if((side & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
			{
				ret = BtApp_GAP_CheckConditionForDisconnectByLocal(pBdAddr);
			}
			else
			{
				ret = BtApp_GAP_CheckConditionForDisconnectByRemote(pBdAddr);
			}
			break;
		}
		case BTAPP_GAP_ACT_CANCEL_CONNECT:
		{
			if((side & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
			{
				ret = BtApp_GAP_CheckConditionForCancelConnectByLocal(pBdAddr);
			}
			else
			{
				ret = BtApp_GAP_CheckConditionForCancelConnectByRemote(pBdAddr);
			}
			break;
		}
		default:
			break;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForConnectByLocal(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	TcBtUint16 ret;
	
	if(pMainConnStatusInst->overallStatus == BTAPP_GAP_STATUS_IDLE)
	{
		ret = BTAPP_GAP_COND_ALLOWED;
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECTING) != 0)
	{
		/* Case 2.1;2.13 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if((pDevConnStatus->connSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.1a */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case 2.13a */
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				/* Case 2.1b;2.13b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.1b;2.13b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
	{
		/* Case 2.3;2.15 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
			{
				if((pDevConnStatus->discSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.3a */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case 2.15a */
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				/* Case 2.3b;2.15b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.3b;2.15b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
	{
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else
	{
		/* unexpected */
		ret = BTAPP_GAP_COND_UNKNOWN;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForConnectByRemote(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	TcBtUint16 ret;

	if(pMainConnStatusInst->overallStatus == BTAPP_GAP_STATUS_IDLE)
	{
		ret = BTAPP_GAP_COND_ALLOWED;
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECTING) != 0)
	{
		/* Case 2.5;2.9 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if((pDevConnStatus->connSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.5a */
					ret = BTAPP_GAP_COND_ALLOWED;
				}
				else
				{
					/* Case 2.9a */
					ret = BTAPP_GAP_COND_ALLOWED;
				}
			}
			else
			{
				/* Case 2.5b;2.9b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.5b;2.9b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
	{
		/* Case 2.7;2.11 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
			{
				if((pDevConnStatus->discSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.7a */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case 2.11a */
					ret = BTAPP_GAP_COND_ALLOWED;
				}
			}
			else
			{
				/* Case 2.7b;2.11b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.7b;2.11b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
	{
		/* Case */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case */
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				/* Case */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else
	{
		/* unexpected */
		ret = BTAPP_GAP_COND_UNKNOWN;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForCancelConnectByLocal(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	TcBtUint16 ret;

	if(pMainConnStatusInst->overallStatus == BTAPP_GAP_STATUS_IDLE)
	{
		ret = BTAPP_GAP_COND_NONE;
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECTING) != 0)
	{
		if(pBdAddr == NULL)
		{
			ret = BTAPP_GAP_COND_ALLOWED;
		}
		else
		{
			/* Case 2.17;2.19 */
			BtAppGapDevConnStatusListElm_t *pDevConnStatus;
			pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
			if(pDevConnStatus != NULL)
			{
				if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
				{
					if((pDevConnStatus->connSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
					{
						/* Case 2.17a */
						ret = BTAPP_GAP_COND_ALLOWED;
					}
					else
					{
						/* Case 2.19a */
						ret = BTAPP_GAP_COND_ALLOWED;
					}
				}
				else
				{
					/* Case 2.17b;2.19b */
					ret = BTAPP_GAP_COND_NONE;
				}
			}
			else
			{
				/* Case 2.17b;2.19b */
				ret = BTAPP_GAP_COND_NONE;
			}
		}
		
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
	{
		/* Case 2.18;2.20 */
		ret = BTAPP_GAP_COND_NONE;
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
	{
		ret = BTAPP_GAP_COND_BUSY;
	}
	else
	{
		/* unexpected */
		ret = BTAPP_GAP_COND_UNKNOWN;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForCancelConnectByRemote(const TcBtBdAddr *pBdAddr)
{
	TcBtUint16 ret;

	ret = BTAPP_GAP_COND_ALLOWED;

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForDisconnectByLocal(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	TcBtUint16 ret;

	if(pMainConnStatusInst->overallStatus == BTAPP_GAP_STATUS_IDLE)
	{
		ret = BTAPP_GAP_COND_ALLOWED;
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECTING) != 0)
	{
		/* Case 2.2;2.14 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if((pDevConnStatus->connSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.2a */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case 2.14a */
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				/* Case 2.2b;2.14b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.2b;2.14b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
	{
		/* Case 2.4;2.16 */
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
			{
				if((pDevConnStatus->discSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					/* Case 2.4a */
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					/* Case 2.16a */
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				/* Case 2.4b;2.16b */
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			/* Case 2.4b;2.16b */
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else if((pMainConnStatusInst->overallStatus & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
	{
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_CONN_SIDE_LOCAL) != 0)
				{
					ret = BTAPP_GAP_COND_BUSY;
				}
				else
				{
					ret = BTAPP_GAP_COND_BUSY;
				}
			}
			else
			{
				ret = BTAPP_GAP_COND_BUSY;
			}
		}
		else
		{
			ret = BTAPP_GAP_COND_BUSY;
		}
	}
	else
	{
		/* unexpected */
		ret = BTAPP_GAP_COND_UNKNOWN;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_CheckConditionForDisconnectByRemote(const TcBtBdAddr *pBdAddr)
{
	TcBtUint16 ret;

	ret = BTAPP_GAP_COND_ALLOWED;

	return ret;
}

void BtApp_GAP_SetMainConnStatus(const TcBtBdAddr *pBdAddr, const TcBtUint16 status, const TcBtUint16 side)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;

	pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
	if(pDevConnStatus != NULL)
	{
		if(status == BTAPP_GAP_STATUS_CONNECTING)
		{
			if((pDevConnStatus->status & status) == 0)
			{
				pDevConnStatus->status |= status;
				pDevConnStatus->connSide = side;
				pMainConnStatusInst->connNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else
			{
                printf("11 BtApp_GAP_SetMainConnStatus : Connecting already\n");
			}
		}
		else if(status == BTAPP_GAP_STATUS_DISCONNECTING)
		{
			if((pDevConnStatus->status & status) == 0)
			{
				pDevConnStatus->status |= status;
				pDevConnStatus->discSide = side;
				pMainConnStatusInst->discNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else
			{
                printf("22 BtApp_GAP_SetMainConnStatus : Connecting already\n");
			}
		}
		else if(status == BTAPP_GAP_STATUS_CONNECT_CANCELLING)
		{
			if((pDevConnStatus->status & status) == 0)
			{
				pDevConnStatus->status |= status;
				pDevConnStatus->ccanSide = side;
				pMainConnStatusInst->ccanNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else
			{
                printf("33 BtApp_GAP_SetMainConnStatus : Connecting already\n");
			}
		}
		else
		{
			/* unexpected */
		}
	}
	else
	{
		TcBtBool add = TRUE;
		
        pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDevConnStatusListElm_t));
		if(pDevConnStatus != NULL)
		{
			TC_BT_BdAddrCopyIfExist(&(pDevConnStatus->bdAddr), pBdAddr);
			if(status == BTAPP_GAP_STATUS_CONNECTING)
			{
				pDevConnStatus->status = status;
				pDevConnStatus->connSide = side;
				pDevConnStatus->ccanSide = 0;
				pDevConnStatus->discSide = 0;
				pMainConnStatusInst->connNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else if(status == BTAPP_GAP_STATUS_DISCONNECTING)
			{
				pDevConnStatus->status = status;
				pDevConnStatus->connSide = 0;
				pDevConnStatus->ccanSide = 0;
				pDevConnStatus->discSide = side;
				pMainConnStatusInst->discNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else if(status == BTAPP_GAP_STATUS_CONNECT_CANCELLING)
			{
				pDevConnStatus->status = status;
				pDevConnStatus->connSide = 0;
				pDevConnStatus->ccanSide = side;
				pDevConnStatus->discSide = 0;
				pMainConnStatusInst->ccanNum++;
				pMainConnStatusInst->overallStatus |= status;
			}
			else
			{
				/* unexpected */
                printf("44 BtApp_GAP_SetMainConnStatus : Connecting already\n");
				add = FALSE;
				TC_BT_Pfree(pDevConnStatus);
			}
			
			if(add == TRUE)
			{
				(void)BtApp_GAP_AddToDevConnStatusList(pDevConnStatus);
			}
		}
		else
		{
			/* malloc failure */
		}
	}
}

void BtApp_GAP_UnsetMainConnStatus(const TcBtBdAddr *pBdAddr, const TcBtUint16 status)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;

	pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
	if(pDevConnStatus != NULL)
	{
		if(status == BTAPP_GAP_STATUS_CONNECTING)
		{
			pDevConnStatus->status &= ~status;
			pDevConnStatus->connSide = 0;
			pMainConnStatusInst->connNum--;
			if(pMainConnStatusInst->connNum == 0)
			{
				pMainConnStatusInst->overallStatus &= ~status;
			}
		}
		else if(status == BTAPP_GAP_STATUS_DISCONNECTING)
		{
			pDevConnStatus->status &= ~status;
			pDevConnStatus->discSide = 0;
			pMainConnStatusInst->discNum--;
			if(pMainConnStatusInst->discNum == 0)
			{
				pMainConnStatusInst->overallStatus &= ~status;
			}
		}
		else if(status == BTAPP_GAP_STATUS_CONNECT_CANCELLING)
		{
			pDevConnStatus->status &= ~status;
			pDevConnStatus->ccanSide = 0;
			pMainConnStatusInst->ccanNum--;
			if(pMainConnStatusInst->ccanNum == 0)
			{
				pMainConnStatusInst->overallStatus &= ~status;
			}
		}
		else
		{
			/* unexpected */
		}

		if(pDevConnStatus->status == BTAPP_GAP_STATUS_IDLE)
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, (BtAppGapListElm_t *)pDevConnStatus);
		}
	}
}

TcBtInt32 BtApp_GAP_AddToDevConnStatusList(const BtAppGapDevConnStatusListElm_t *pGivenDevConnStatus)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pGivenDevConnStatus == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a element that matches bdAddr */
    pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)BtApp_GAP_ListSearch((const BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, BtApp_GAP_DevConnStatusFind, (const void *)&(pGivenDevConnStatus->bdAddr));
	if(pDevConnStatus != NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_ALREADY_EXIST;
	}
	else
	{
		/* add new element */
        pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDevConnStatusListElm_t));
		TC_BT_BdAddrCopyIfExist(&(pDevConnStatus->bdAddr), &(pGivenDevConnStatus->bdAddr));
		pDevConnStatus->status = pGivenDevConnStatus->status;
		pDevConnStatus->connSide = pGivenDevConnStatus->connSide;
		pDevConnStatus->ccanSide = pGivenDevConnStatus->ccanSide;
		pDevConnStatus->discSide = pGivenDevConnStatus->discSide;
		
		(void)BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, (BtAppGapListElm_t *)pDevConnStatus);
	}
	
	return ret;
}

TcBtInt32 BtApp_GAP_RemoveFromDevConnStatusList(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pBdAddr == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a element that matches bdAddr */
    pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)BtApp_GAP_ListSearch((const BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, BtApp_GAP_DevConnStatusFind, (const void *)pBdAddr);
	if(pDevConnStatus != NULL)
	{
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, (BtAppGapListElm_t *)pDevConnStatus);
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_NOT_FOUND;
	}

	return ret;
}

TcBtUint16 BtApp_GAP_GetOverallConnStatus(void)
{
	return mainConnStatusInst.overallStatus;
}

BtAppGapDevConnStatusListElm_t *BtApp_GAP_GetDevConnStatus(const TcBtBdAddr *pBdAddr)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus = NULL;

	/* param check */
	if(pBdAddr == NULL)
	{/* invalid parameter */
		return pDevConnStatus;
	}

	/* find a device that matches bdAddr */
    pDevConnStatus = (BtAppGapDevConnStatusListElm_t *)BtApp_GAP_ListSearch((const BtAppGapList_t *)&pMainConnStatusInst->devConnStatusList, BtApp_GAP_DevConnStatusFind, (const void *)pBdAddr);
	
	return pDevConnStatus;
}

TcBtUint16 BtApp_GAP_GetHfpState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx)
{
	/* return the status for the given bdAddr */
	BtAppGapHfpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHfpConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;
	TcBtUint8 index;

	if(pBdAddr == NULL)
	{
		if(instanceIdx < BTAPP_GAP_HFP_MAX_CONN_INST)
		{
			ret = pConnStatusInfo->connInst[instanceIdx].state;
		}
	}
	else
	{
		for(index = 0; index < BTAPP_GAP_HFP_MAX_CONN_INST; index++)
		{
			if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnStatusInfo->connInst[index].bdAddr), pBdAddr) == TRUE)
			{
				ret = pConnStatusInfo->connInst[index].state;
			}
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetHfpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) search the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapHfpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHfpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	TcBtBool found = FALSE;
	TcBtUint8 index;
	TcBtUint8 emptyIdx = BTAPP_GAP_UNUSED_INST_IDX;

	for(index = 0; index < BTAPP_GAP_HFP_MAX_CONN_INST; index++)
	{
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnStatusInfo->connInst[index].bdAddr), pBdAddr) == TRUE)
		{
			found = TRUE;
			break;
		}
		if(emptyIdx == BTAPP_GAP_UNUSED_INST_IDX)
		{
			if(TC_BT_BdAddrEqZero((const TcBtBdAddr *)&(pConnStatusInfo->connInst[index].bdAddr)) == TRUE)
			{
				emptyIdx = index;
			}
		}
	}

	if(found == TRUE)
	{
		if(pConnStatusInfo->connInst[index].state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst[index].bdAddr));
			}
			pConnStatusInfo->connInst[index].state = state;
		}
	}
	else
	{
		if(emptyIdx != BTAPP_GAP_UNUSED_INST_IDX)
		{
			if(pConnStatusInfo->connInst[emptyIdx].state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst[emptyIdx].bdAddr), pBdAddr);
				pConnStatusInfo->connInst[emptyIdx].state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetHfpBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr)
{
	BtAppGapHfpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHfpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(instanceIdx < BTAPP_GAP_HFP_MAX_CONN_INST)
	{
		if(pBdAddr != NULL)
		{
			TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst[instanceIdx].bdAddr));
			ret = BTAPP_GAP_FN_RESULT_SUCCESS;
		}
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetA2dpState(const TcBtBdAddr *pBdAddr)
{
	/* return the status for the given bdAddr */
	BtAppGapA2dpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetA2dpConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;

	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.state;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.state;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_SetA2dpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapA2dpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetA2dpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(pConnStatusInfo->connInst.state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
			}
			pConnStatusInfo->connInst.state = state;
		}
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			if(pConnStatusInfo->connInst.state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
				pConnStatusInfo->connInst.state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetA2dpBdAddr(TcBtBdAddr *pBdAddr)
{
	BtAppGapA2dpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetA2dpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetA2dpStreamState(const TcBtBdAddr *pBdAddr)
{
	BtAppGapA2dpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetA2dpConnStatusInfo();
	TcBtUint16 ret;
	
	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.streamState;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.streamState;
		}
	}

	return ret;
}

TcBtInt32 BtApp_GAP_SetA2dpStreamState(const TcBtBdAddr *pBdAddr, TcBtUint16 streamState)
{
	BtAppGapA2dpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetA2dpConnStatusInfo();
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(streamState == BTAPP_GAP_STREAM_STATE_CLOSED)
		{
			TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
		}
		pConnStatusInfo->connInst.streamState = streamState;
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
			pConnStatusInfo->connInst.streamState = streamState;
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint16 BtApp_GAP_GetAvrcpState(const TcBtBdAddr *pBdAddr)
{
	/* return the status for the given bdAddr */
	BtAppGapAvrcpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetAvrcpConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;
	
	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.state;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.state;
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetAvrcpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapAvrcpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetAvrcpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(pConnStatusInfo->connInst.state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
			}
			pConnStatusInfo->connInst.state = state;
		}
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			if(pConnStatusInfo->connInst.state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
				pConnStatusInfo->connInst.state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetAvrcpBdAddr(TcBtBdAddr *pBdAddr)
{
	BtAppGapAvrcpConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetAvrcpConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetPacState(const TcBtBdAddr *pBdAddr)
{
	/* return the status for the given bdAddr */
	BtAppGapPacConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPacConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;
	
	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.state;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.state;
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetPacState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapPacConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPacConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(pConnStatusInfo->connInst.state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
			}
			pConnStatusInfo->connInst.state = state;
		}
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			if(pConnStatusInfo->connInst.state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
				pConnStatusInfo->connInst.state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetPacBdAddr(TcBtBdAddr *pBdAddr)
{
	BtAppGapPacConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPacConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetMapcState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx)
{
	/* return the status for the given bdAddr */
	BtAppGapMapcConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetMapcConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;

	if(pBdAddr == NULL)
	{
		if(instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST)
		{
			ret = pConnStatusInfo->connInst[instanceIdx].state;
		}
	}
	else
	{
		if(instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST)
		{
			if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr) == TRUE)
			{
				ret = pConnStatusInfo->connInst[instanceIdx].state;
			}
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetMapcState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapMapcConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetMapcConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(pBdAddr != NULL)
	{
		if(instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST)
		{
			if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr) == TRUE)
			{
				if(pConnStatusInfo->connInst[instanceIdx].state != state)
				{
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						pConnStatusInfo->idleInstCount++;
						TC_BT_BdAddrZero(&(pConnStatusInfo->connInst[instanceIdx].bdAddr));
					}
					pConnStatusInfo->connInst[instanceIdx].state = state;
				}
			}
			else
			{
				if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst[instanceIdx].bdAddr)) == TRUE)
				{
					if(pConnStatusInfo->connInst[instanceIdx].state != state)
					{
						pConnStatusInfo->idleInstCount--;
						TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr);
						pConnStatusInfo->connInst[instanceIdx].state = state;
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetMapcBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr)
{
	BtAppGapMapcConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetMapcConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM; /* 0:success,1:param error */
	
	if(instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST)
	{
		if(pBdAddr != NULL)
		{
			TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst[instanceIdx].bdAddr));
			ret = BTAPP_GAP_FN_RESULT_SUCCESS;
		}
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetSppState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx)
{
	/* return the status for the given bdAddr */
	BtAppGapSppConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetSppConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;

	if(pBdAddr == NULL)
	{
		if(instanceIdx < BTAPP_GAP_SPP_MAX_CONN_INST)
		{
			ret = pConnStatusInfo->connInst[instanceIdx].state;
		}
	}
	else
	{
		if(instanceIdx < BTAPP_GAP_SPP_MAX_CONN_INST)
		{
			if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr) == TRUE)
			{
				ret = pConnStatusInfo->connInst[instanceIdx].state;
			}
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetSppState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapSppConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetSppConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS; /* 0:Success,1:Fail */

	if(pBdAddr != NULL)
	{
		if(instanceIdx < BTAPP_GAP_SPP_MAX_CONN_INST)
		{
			if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr) == TRUE)
			{
				if(pConnStatusInfo->connInst[instanceIdx].state != state)
				{
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						pConnStatusInfo->idleInstCount++;
						TC_BT_BdAddrZero(&(pConnStatusInfo->connInst[instanceIdx].bdAddr));
					}
					pConnStatusInfo->connInst[instanceIdx].state = state;
				}
			}
			else
			{
				if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst[instanceIdx].bdAddr)) == TRUE)
				{
					if(pConnStatusInfo->connInst[instanceIdx].state != state)
					{
						pConnStatusInfo->idleInstCount--;
						TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst[instanceIdx].bdAddr), pBdAddr);
						pConnStatusInfo->connInst[instanceIdx].state = state;
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetSppBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr)
{
	BtAppGapSppConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetSppConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM; /* 0:success,1:param error */
	
	if(instanceIdx < BTAPP_GAP_SPP_MAX_CONN_INST)
	{
		if(pBdAddr != NULL)
		{
			TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst[instanceIdx].bdAddr));
			ret = BTAPP_GAP_FN_RESULT_SUCCESS;
		}
	}
	
	return ret;
}

TcBtUint8 BtApp_GAP_GetSppIdleInstIdx(void)
{
	TcBtInt32 i;
	TcBtUint16 state;
	TcBtUint16 ret = BTAPP_GAP_UNUSED_INST_IDX;	
	
	for(i = 0; i < BTAPP_GAP_SPP_MAX_CONN_INST; i++)
	{
		state = BtApp_GAP_GetSppState(NULL, i);
		if(state == BTAPP_GAP_STATE_DISCONNECTED)
		{
			ret = i;
			break;
		}
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetIapState(const TcBtBdAddr *pBdAddr)
{
	/* return the status for the given bdAddr */
	BtAppGapIapConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetIapConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;
	
	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.state;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.state;
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetIapState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapIapConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetIapConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(pConnStatusInfo->connInst.state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
			}
			pConnStatusInfo->connInst.state = state;
		}
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			if(pConnStatusInfo->connInst.state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
				pConnStatusInfo->connInst.state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetIapBdAddr(TcBtBdAddr *pBdAddr)
{
	BtAppGapIapConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetIapConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetHiddState(const TcBtBdAddr *pBdAddr)
{
	/* return the status for the given bdAddr */
	BtAppGapHiddConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHiddConnStatusInfo();
	TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;
	
	if(pBdAddr == NULL)
	{
		ret = pConnStatusInfo->connInst.state;
	}
	else
	{
		if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
		{
			ret = pConnStatusInfo->connInst.state;
		}
	}
	
	return ret;
}

TcBtUint32 BtApp_GAP_SetHiddState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
	/* step1) check the identical bdAddr. */
	/* step1.a) if it exist, set the state as given one. */
	/* step1.b) if it does not exist, use the empty instance. */
	BtAppGapHiddConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHiddConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
	{
		if(pConnStatusInfo->connInst.state != state)
		{
			if(state == BTAPP_GAP_STATE_DISCONNECTED)
			{
				pConnStatusInfo->idleInstCount++;
				TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
			}
			pConnStatusInfo->connInst.state = state;
		}
	}
	else
	{
		if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
		{
			if(pConnStatusInfo->connInst.state != state)
			{
				pConnStatusInfo->idleInstCount--;
				TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
				pConnStatusInfo->connInst.state = state;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
		}
	}

	return ret;
}

TcBtUint32 BtApp_GAP_GetHiddBdAddr(TcBtBdAddr *pBdAddr)
{
	BtAppGapHiddConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetHiddConnStatusInfo();
	TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
	
	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}
	
	return ret;
}

TcBtUint16 BtApp_GAP_GetPANState(const TcBtBdAddr *pBdAddr)
{
    /* return the status for the given bdAddr */
    BtAppGapPanConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPanConnStatusInfo();
    TcBtUint16 ret = BTAPP_GAP_STATE_DISCONNECTED;

    if(pBdAddr == NULL)
    {
        ret = pConnStatusInfo->connInst.state;
    }
    else
    {
        if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
        {
            ret = pConnStatusInfo->connInst.state;
        }
    }

    return ret;
}

TcBtUint32 BtApp_GAP_SetPANState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state)
{
    /* step1) check the identical bdAddr. */
    /* step1.a) if it exist, set the state as given one. */
    /* step1.b) if it does not exist, use the empty instance. */
    BtAppGapPanConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPanConnStatusInfo();
    TcBtUint32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

    if(TC_BT_BdAddrEq(&(pConnStatusInfo->connInst.bdAddr), pBdAddr) == TRUE)
    {
        if(pConnStatusInfo->connInst.state != state)
        {
            if(state == BTAPP_GAP_STATE_DISCONNECTED)
            {
                pConnStatusInfo->idleInstCount++;
                TC_BT_BdAddrZero(&(pConnStatusInfo->connInst.bdAddr));
            }
            pConnStatusInfo->connInst.state = state;
        }
    }
    else
    {
        if(TC_BT_BdAddrEqZero(&(pConnStatusInfo->connInst.bdAddr)) == TRUE)
        {
            if(pConnStatusInfo->connInst.state != state)
            {
                pConnStatusInfo->idleInstCount--;
                TC_BT_BdAddrCopyIfExist(&(pConnStatusInfo->connInst.bdAddr), pBdAddr);
                pConnStatusInfo->connInst.state = state;
            }
        }
        else
        {
            ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
        }
    }

    return ret;
}

TcBtUint32 BtApp_GAP_GetPANBdAddr(TcBtBdAddr *pBdAddr)
{
    BtAppGapPanConnStatusInfo_t *pConnStatusInfo = BtApp_GAP_GetPanConnStatusInfo();
    TcBtUint32 ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;

    if(pBdAddr != NULL)
    {
        TC_BT_BdAddrCopyIfExist(pBdAddr, &(pConnStatusInfo->connInst.bdAddr));
        ret = BTAPP_GAP_FN_RESULT_SUCCESS;
    }

    return ret;
}

static TcBtBool BtApp_GAP_IsConnectable(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid)
{
	BtAppGapMainConnStatusInst_t *pMainConnStatusInst = BtApp_GAP_GetMainConnStatusInst();
	TcBtUint16 state;
	TcBtBool ret = FALSE;
	
	switch(profileUuid)
	{
		case TC_BT_HF_PROFILE_UUID:
			if(pMainConnStatusInst->hfp.idleInstCount > 0)
			{
				state = BtApp_GAP_GetHfpState(pBdAddr, BTAPP_GAP_UNUSED_INST_IDX);
				if(state == BTAPP_GAP_STATE_DISCONNECTED)
				{
					ret = TRUE;
				}
			}
			break;
		case TC_BT_AUDIO_SINK_UUID:
			if(pMainConnStatusInst->a2dp.idleInstCount > 0)
			{
				ret = TRUE;
			}
			break;
		case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
			if(pMainConnStatusInst->avrcp.idleInstCount > 0)
			{
				ret = TRUE;
			}
			break;
		case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
			if(pMainConnStatusInst->pac.idleInstCount > 0)
			{
				ret = TRUE;
			}
			break;
		case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
			if(pMainConnStatusInst->mapc.idleInstCount == BTAPP_GAP_MAPC_MAX_CONN_INST)
			{
				ret = TRUE;
			}
			break;
 		case TC_BT_SPP_PROFILE_UUID:
			if(pMainConnStatusInst->spp.idleInstCount > 0)
			{
				TcBtInt32 i;
				for(i = 0; i < BTAPP_GAP_SPP_MAX_CONN_INST; i++)
				{
					state = BtApp_GAP_GetSppState(pBdAddr, i);
					if(state != BTAPP_GAP_STATE_DISCONNECTED)
					{
						break;
					}
				}
				if(i == BTAPP_GAP_SPP_MAX_CONN_INST)
				{
					ret = TRUE;
				}
			}
			break;
		case TC_BT_SPP_IAP_SERVICE_UUID:
			if(pMainConnStatusInst->iap.idleInstCount > 0)
			{
				ret = TRUE;
			}
			break;
		case TC_BT_HID_PROFILE_UUID:
			if(pMainConnStatusInst->hidd.idleInstCount > 0)
			{
				ret = TRUE;
			}
			break;
        case TC_BT_PAN_PANU_PROFILE_UUID :
        case TC_BT_PAN_NAP_PROFILE_UUID :
        case TC_BT_PAN_GN_PROFILE_UUID :
            if(pMainConnStatusInst->pan.idleInstCount > 0)
            {
                ret = TRUE;
            }
            break;            

		default:
			break;
	}

	return ret;
}

#ifdef ______________________________________________________________________________
#endif
/* Check whether there are corresponding profile in the service list or not */
static TcBtBool BtApp_GAP_IsInServiceList(TcBtUint16 profileUuid, const TcBtUint32 *pServiceList, TcBtUint32 serviceCount)
{
	TcBtUint32 i;
	TcBtBool found = 0;
	TcBtInt32 ret = FALSE;
	
	for(i = 0; i < serviceCount; i++)
	{
		/* find corresponding profile */
		switch(profileUuid)
		{
		#ifdef BT_HANDSFREE_UNIT
			case TC_BT_HF_PROFILE_UUID:
				if(pServiceList[i] == TC_BT_HFG_PROFILE_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#ifdef BT_AUDIO_SINK
			case TC_BT_AUDIO_SINK_UUID:
				if(pServiceList[i] == TC_BT_AUDIO_SOURCE_UUID)
				{
					found = 1;
				}
				break;
			case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
				if(pServiceList[i] == TC_BT_AV_REMOTE_CONTROL_TARGET_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#ifdef BT_PBAP_CLIENT
			case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
				if(pServiceList[i] == TC_BT_OBEX_PBA_SERVER_PROFILE_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#ifdef BT_MAP_CLIENT
			case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
				if(pServiceList[i] == TC_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#ifdef BT_SPP
			case TC_BT_SPP_PROFILE_UUID:
				if(pServiceList[i] == TC_BT_SPP_PROFILE_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
			case TC_BT_SPP_IAP_SERVICE_UUID:
				if(pServiceList[i] == TC_BT_SPP_PROFILE_UUID)
				{
					found = 1;
				}
				break;
		#endif
		#ifdef BT_HIDD
			case TC_BT_HID_PROFILE_UUID:
				if(pServiceList[i] == TC_BT_HID_PROFILE_UUID)
				{
					found = 1;
				}
				break;

		#endif
        #ifdef BT_PAN
            case TC_BT_PAN_PANU_PROFILE_UUID :
            case TC_BT_PAN_NAP_PROFILE_UUID :
            case TC_BT_PAN_GN_PROFILE_UUID :
                if ( pServiceList[i] == TC_BT_PAN_PANU_PROFILE_UUID ||
                     pServiceList[i] == TC_BT_PAN_NAP_PROFILE_UUID ||
                     pServiceList[i] == TC_BT_PAN_GN_PROFILE_UUID )
                {
                    found = 1;
                }
                break;
        #endif
 			default:
				break;
		}

		if(found == 1)
		{
			ret = TRUE;
			break;
		}
	}

	return ret;
}

static TcBtUint32 BtApp_GAP_DeactivateCommonFunc(TcBtUint16 profileUuid, TcBtBdAddr *pBdAddr, TcBtUint8 instanceIdx)
{
	TcBtUint32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;

	switch(profileUuid)
	{
		case TC_BT_HF_PROFILE_UUID:
		{
#ifdef BT_HANDSFREE_UNIT
			BtApp_GAP_SetHfpState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
            fr = G_BtHfCmd->TC_BT_HF_Deactivate();
#endif
			break;
		}
		case TC_BT_AUDIO_SINK_UUID:
		{
#ifdef BT_AUDIO_SINK
			BtApp_GAP_SetA2dpState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
			fr = TC_BT_A2DP_Deactivate();
#endif
			break;
		}
		case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
		{
#ifdef BT_AUDIO_SINK
			BtApp_GAP_SetAvrcpState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
			fr = TC_BT_AVRCP_Deactivate();
#endif
			break;
		}
		case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
		{
			/* PAC does not provide deactivate function. Then...BT Off?? */
			BtApp_GAP_SetPacState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
			break;
		}
		case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
		{
			/* MAPC does not provide deactivate function. Then...BT Off?? */
			BtApp_GAP_SetMapcState(pBdAddr, instanceIdx, BTAPP_GAP_STATE_DISCONNECTED);
			break;
		}
		case TC_BT_SPP_PROFILE_UUID:
		{
#ifdef BT_SPP
			BtApp_GAP_SetSppState(pBdAddr, instanceIdx, BTAPP_GAP_STATE_DISCONNECTED);
			fr = TC_BT_SPP_Deactivate(instanceIdx);
#endif
			break;
		}
		case TC_BT_SPP_IAP_SERVICE_UUID:
		{
#ifdef BT_SPP
			BtApp_GAP_SetIapState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
			fr = TC_BT_SPP_IAP_Deactivate();
#endif
			break;
		}
		case TC_BT_HID_PROFILE_UUID:
		{
#ifdef BT_HIDD
			/* HIDD does not provide deactivate function. Then...BT Off?? */
			BtApp_GAP_SetHiddState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
#endif
			break;
		}
        case TC_BT_PAN_PANU_PROFILE_UUID :
        case TC_BT_PAN_NAP_PROFILE_UUID :
        case TC_BT_PAN_GN_PROFILE_UUID :
        {

#ifdef BT_PAN
            BtApp_GAP_SetPANState(pBdAddr, BTAPP_GAP_STATE_DISCONNECTED);
			fr = TC_BT_PAN_Deactivate();
            break;
#endif
        }
 		default:
			break;
	}

	return fr;
}

static void BtApp_GAP_ConnectForTimer(TcBtUint8 id, TcBtUint8 loc)
{
    printf("======================BtApp_GAP_ConnectForTimer =============================\n");
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtUint32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	TcBtUint16 state;
	TcBtBool moveLast = FALSE;

	pConnProf = BtApp_GAP_GetConnProfile((const TcBtBdAddr *)&pConnInst->targetBdAddr, pConnInst->targetProfile, pConnInst->targetInstanceIdx);
	if(pConnProf != NULL)
	{
		pConnDev = pConnProf->pDevice;
		if(pConnProf->valid == FALSE)
		{
            (void)TC_BT_SD_ReadAvailableServices(pConnInst->targetBdAddr);
			BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_RAS);
		}
		else
		{
			switch(pConnInst->targetProfile)
			{
			#ifdef BT_HANDSFREE_UNIT
				case TC_BT_HF_PROFILE_UUID:
				{
					state = BtApp_GAP_GetHfpState((const TcBtBdAddr *)&pConnInst->targetBdAddr, BTAPP_GAP_UNUSED_INST_IDX);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
                        fr = G_BtHfCmd->TC_BT_HF_ServiceConnect(&pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetHfpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_AUDIO_SINK
				case TC_BT_AUDIO_SINK_UUID:
				{
					state = BtApp_GAP_GetA2dpState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_A2DP_Connect((const TcBtBdAddr *)&pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetA2dpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
				case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
				{
					state = BtApp_GAP_GetAvrcpState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_AVRCP_Connect((const TcBtBdAddr *)&pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetAvrcpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_PBAP_CLIENT
				case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
				{
					state = BtApp_GAP_GetPacState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
                        fr = BtAPp_PAC_ConnectReq(&pConnInst->targetBdAddr, PAC_MAX_OBEX_PACKET_SIZE);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetPacState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
  			#ifdef BT_MAP_CLIENT
				case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
				{
					state = BtApp_GAP_GetMapcState((const TcBtBdAddr *)&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_MAPC_ConnectReq(&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetMapcState(&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_SPP
				case TC_BT_SPP_PROFILE_UUID:
				{
					state = BtApp_GAP_GetSppState((const TcBtBdAddr *)&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_SPP_Connect(pConnInst->targetInstanceIdx, pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetSppState(&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
			#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
				case TC_BT_SPP_IAP_SERVICE_UUID:
				{
					state = BtApp_GAP_GetIapState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_SPP_IAP_Connect(pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetIapState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_HIDD
				case TC_BT_HID_PROFILE_UUID:
				{
					state = BtApp_GAP_GetHiddState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(state == BTAPP_GAP_STATE_DISCONNECTED)
					{
						fr = TC_BT_HIDD_Connect(pConnInst->targetBdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetHiddState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
						}
					}
					break;
				}
			#endif
            #ifdef BT_PAN
                case TC_BT_PAN_PANU_PROFILE_UUID :
                case TC_BT_PAN_NAP_PROFILE_UUID :
                case TC_BT_PAN_GN_PROFILE_UUID :
                {
                    state = BtApp_GAP_GetPANState((const TcBtBdAddr *)&pConnInst->targetBdAddr);
                    if(state == BTAPP_GAP_STATE_DISCONNECTED)
                    {
                        // PANTODO We use only PAN to NAP
                        fr = TC_BT_PAN_Connect(pConnInst->targetBdAddr, TC_BT_PAN_ROLE_PAN, TC_BT_PAN_ROLE_NAP);
                        if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
                        {
                            (void)BtApp_GAP_SetPANState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECTING);
                        }
                    }
                    break;
                }
            #endif
				default:
					break;
			}

			if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
			{/* connect function success */
                ///modify for deactive HF while timeout  by xieyh 2017.02.05
                TCCAPI_Timer_SetTime(TC_BT_CONNECT_TIMEOUT, TIMER_BT_CONNECT_TIMEOUT_ID, BtApp_GAP_ConnectTimeoutForTimer);
                //modify end
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_CONNECTING);
					
				pConnProf = BtApp_GAP_GetConnProfile((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pConnInst->targetProfile, pConnInst->targetInstanceIdx);
				if(pConnProf != NULL)
				{
					pConnDev = pConnProf->pDevice;
					pConnProf->tryCount--;
					if(pConnProf->tryCount == 0)
					{
						BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
						if(pConnDev->connProfList.count == 0)
						{
							BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
						}
					}
				}
			}
			else if(moveLast == TRUE)
			{
				pConnProf = BtApp_GAP_GetConnProfile((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pConnInst->targetProfile, pConnInst->targetInstanceIdx);
				if(pConnProf != NULL)
				{
					pConnDev = pConnProf->pDevice;
					BtApp_GAP_ListElementMoveLast((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
				}

				TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				
				(void)BtApp_GAP_Connect();
			}
			else
			{
				pConnProf = BtApp_GAP_GetConnProfile((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pConnInst->targetProfile, pConnInst->targetInstanceIdx);
				if(pConnProf != NULL)
				{
					pConnDev = pConnProf->pDevice;
					BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
					if(pConnDev->connProfList.count == 0)
					{
						BtAppGapDevConnStatusListElm_t *pDevConnStatus;
						BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
						pDevConnStatus = BtApp_GAP_GetDevConnStatus(&pConnInst->targetBdAddr);
						if(pDevConnStatus != NULL)
						{
							if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
							{
#if 1
								printf("BtApp_GAP_ConnectForTimer ConnectCfm\n");
#else
                                DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
							}
							BtApp_GAP_UnsetMainConnStatus(&pConnInst->targetBdAddr, BTAPP_GAP_STATUS_CONNECTING);
						}
					}
				}

				TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				
				(void)BtApp_GAP_Connect();
			}
		}
	}
}

static void BtApp_GAP_ConnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtBool processEnd = TRUE;
	TcBtInt32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;

	
	if(pRecoveryInst->doOffOn != TRUE)
	{
		/* Deactivate the profile(s) in the waiting list */
		fr = BtApp_GAP_DeactivateCommonFunc(pConnInst->targetProfile, &pConnInst->targetBdAddr, pConnInst->targetInstanceIdx);
		
		if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			BtAppGapProfUuidInstIdx searchData;

			TCCAPI_Timer_SetTime(TC_BT_DEACTIVATE_TIMEOUT, TIMER_BT_DEACTIVATE_TIMEOUT_ID, BtApp_GAP_DeactivateTimeoutHandler);

			searchData.profileUuid = pConnInst->targetProfile;
			searchData.instanceIdx = pConnInst->targetInstanceIdx;
            if(BtApp_GAP_ListSearch((const BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, BtApp_GAP_RecoveryProfileFind, (void *)&searchData) == NULL)
			{
                BtAppGapRecoveryListElm_t *pRecoveryProf = (BtAppGapRecoveryListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapRecoveryListElm_t));
				if(pRecoveryProf != NULL)
				{
					pRecoveryProf->profileUuid = pConnInst->targetProfile;
					pRecoveryProf->instanceIdx = pConnInst->targetInstanceIdx;
					BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, (BtAppGapListElm_t *)pRecoveryProf);
				}
			}
		}
		else
		{
			pRecoveryInst->doOffOn = TRUE;
		}
	}

	/* remove from connProfList */
	pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)&pConnInst->targetBdAddr);
	if(pConnDev != NULL)
	{
		BtAppGapProfUuidInstIdx searchData;

		searchData.profileUuid = pConnInst->targetProfile;
		searchData.instanceIdx = pConnInst->targetInstanceIdx;
        pConnProf = (BtAppGapConnProfListElm_t  *)BtApp_GAP_ListSearch(( BtAppGapList_t *)&pConnDev->connProfList, BtApp_GAP_ConnProfileFind, (const void *)&searchData);
		if(pConnProf != NULL)
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
		}
		
		if(pConnDev->connProfList.count == 0)
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
		}
		else
		{
			processEnd = FALSE;
		}
	}
	else
	{
		processEnd = FALSE;
	}

	if(processEnd == TRUE)
	{
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(&pConnInst->targetBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
				{
#if 1
					printf("BtApp_GAP_ConnectTimeoutForTimer ConnectCfm\n");
#else
                    DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
				}
				BtApp_GAP_UnsetMainConnStatus(&pConnInst->targetBdAddr, BTAPP_GAP_STATUS_CONNECTING);
			}
		}
	}

	/* reset connInst */
	TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
	pConnInst->targetProfile = 0;
	pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
	BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);

	/* continue to connect */
	(void)BtApp_GAP_Connect();
}

static TcBtUint16 BtApp_GAP_GetConnStatus(void)
{
	return connInst.connStatus;
}

static void BtApp_GAP_SetConnStatus(TcBtUint32 connStatus)
{
	connInst.connStatus = connStatus;
}

static TcBtUint16 BtApp_GAP_GetCcanStatus(void)
{
	return ccanInst.ccanStatus;
}

static void BtApp_GAP_SetCcanStatus(TcBtUint32 ccanStatus)
{
	ccanInst.ccanStatus = ccanStatus;
}

TcBtBdAddr *BtApp_GAP_GetCcanBdAddr(void)
{
	return &ccanInst.targetBdAddr;
}

/*********************
 *  PUBLIC FUNCTION  *
 *********************/
void BtApp_GAP_InitConnInst(void)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	
	TC_BT_BdAddrZero(&(pConnInst->targetBdAddr));
	pConnInst->targetProfile = 0;
	pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
	BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceRemove);
}

void BtApp_GAP_InitCcanInst(void)
{
	BtAppGapCcanInstance_t *pCcanInst = BtApp_GAP_GetCcanInst();
	
	TC_BT_BdAddrZero(&(pCcanInst->targetBdAddr));
	pCcanInst->targetProfile = 0;
	pCcanInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
	BtApp_GAP_SetCcanStatus(BTAPP_GAP_CCAN_STATUS_IDLE);
}

TcBtInt32 BtApp_GAP_AddToConnProfList(const TcBtBdAddr *pBdAddr, const BtAppGapConnProfListElm_t *pConnProfile)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtBool doNotAdd = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if((pBdAddr == NULL) || (pConnProfile == NULL))
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((const BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
	if(pConnDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;
		
		searchData.profileUuid = pConnProfile->profileUuid;
		searchData.instanceIdx = pConnProfile->instanceIdx;
        pConnProf = (BtAppGapConnProfListElm_t *)BtApp_GAP_ListSearch((const BtAppGapList_t *)&pConnDev->connProfList, BtApp_GAP_ConnProfileFind, (const void *)&searchData);
		if(pConnProf != NULL)
		{
			foundProf = 1;
		}
	}
	else
	{/* add new device */
        pConnDev = (BtAppGapConnDevListElm_t*)TC_BT_Pmalloc(sizeof(BtAppGapConnDevListElm_t));
		TC_BT_BdAddrCopyIfExist(&(pConnDev->bdAddr), pBdAddr);
		pConnDev->pServiceList = NULL;
		pConnDev->serviceCount = 0;
		pConnDev->retryCountForRas = TC_BT_RETRY_RAS_MAX;
        BtApp_GAP_ListInit((BtAppGapList_t*)&pConnDev->connProfList, NULL);
		
		(void)BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
	}

	if(foundProf == 1)
	{
		ret = BTAPP_GAP_FN_RESULT_ALREADY_EXIST;
	}
	else
	{/* add new profile */
		BtAppGapConnProfListElm_t tempConnProf;
		memcpy(&tempConnProf, pConnProfile, sizeof(BtAppGapConnProfListElm_t));
		
		/* check service list in order to determine validity */
		if(pConnProfile->valid == FALSE)
		{
			if(pConnDev->pServiceList != NULL)
			{
				if(BtApp_GAP_IsInServiceList(pConnProfile->profileUuid, (const TcBtUint32 *)pConnDev->pServiceList, pConnDev->serviceCount) == TRUE)
				{
					tempConnProf.valid = TRUE;
				}
				else
				{
					doNotAdd = 1;
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
			}
		}

		if(doNotAdd != 1)
		{
            pConnProf = (BtAppGapConnProfListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapConnProfListElm_t));
			pConnProf->pDevice = pConnDev;
			pConnProf->profileUuid = tempConnProf.profileUuid;
			pConnProf->instanceIdx = tempConnProf.instanceIdx;
			pConnProf->timerSettingVal = tempConnProf.timerSettingVal;
			pConnProf->tryCount = tempConnProf.tryCount;
			pConnProf->valid = tempConnProf.valid;
			
			(void)BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
		}
	}

	return ret;
}

TcBtInt32 BtApp_GAP_RemoveFromConnProfList(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pBdAddr == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
	if(pConnDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;
		
		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pConnProf = (BtAppGapConnProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnDev->connProfList, BtApp_GAP_ConnProfileFind, (const void *)&searchData);
		if(pConnProf != NULL)
		{
			foundProf = 1;
		}
		
		if(foundProf == 1)
		{/* remove found profile */
			(void)BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
			if(pConnDev->connProfList.count == 0)
			{/* remove empty device */
				(void)BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_PROF_NOT_FOUND;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND;
	}

	return ret;
}

void BtApp_GAP_PairingIndHandler(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();

	/* param check */
	if(pBdAddr == NULL)
	{
		return;
	}

	if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
	{
        TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_TIMEOUT_ID);
	}
}

void BtApp_GAP_BondFailHandler(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	TcBtBool connFlag = FALSE;

	/* param check */
	if(pBdAddr == NULL)
	{
		return;
	}

	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pBdAddr) == TRUE)
	{
        TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_STREAM_ID);
		
		TC_BT_BdAddrZero(&(pConnInst->targetBdAddr));
		pConnInst->targetProfile = 0;
		pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
		connFlag = TRUE;
	}

	/* find a device that matches bdAddr */
    pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
	if(pConnDev != NULL)
	{
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
		connFlag = TRUE;
	}

	if(connFlag == TRUE)
	{
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
				{
					if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
					{
						if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
						{
#if 1
							printf("GAP ConnectCfm\n");
#else
                            DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
					}

					/* resume connection */
					(void)BtApp_GAP_Connect();
				}
			}
		}
	}
}

BtAppGapConnProfListElm_t *BtApp_GAP_GetConnProfile(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtBool foundDev = 0;
	BtAppGapConnDevListElm_t *pRet = NULL;

	/* param check */
	if(pBdAddr == NULL)
	{/* invalid parameter */
        return (BtAppGapConnProfListElm_t *)pRet;
	}

	/* find a device that matches bdAddr */
    pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
	if(pConnDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;

		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pConnProf = (BtAppGapConnProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnDev->connProfList, BtApp_GAP_ConnProfileFind, (const void *)&searchData);
		if(pConnProf != NULL)
		{
            pRet = (BtAppGapConnDevListElm_t *)pConnProf;
		}
	}
	
    return (BtAppGapConnProfListElm_t *)pRet;
}

TcBtInt32 BtApp_GAP_SetConnProfile(const TcBtBdAddr *pBdAddr, const BtAppGapConnProfListElm_t *pConnProfile)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if((pBdAddr == NULL) || (pConnProfile == NULL))
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
	if(pConnDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
        pConnProf = (BtAppGapConnProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnDev->connProfList, BtApp_GAP_ConnProfileFind, (const void *)&pConnProfile->profileUuid);
		if(pConnProf != NULL)
		{
			foundProf = 1;
		}
		if(foundProf == 1)
		{
			pConnProf->timerSettingVal = pConnProfile->timerSettingVal;
			pConnProf->tryCount = pConnProfile->tryCount;
			pConnProf->valid = pConnProfile->valid;
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_PROF_NOT_FOUND;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND;
	}
	
	return ret;
}

void BtApp_GAP_AvoidConnectConflict(const TcBtBdAddr *pBdAddr, TcBtUint16 incomingProfUuid)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	TcBtBool found = 0;
	
	if(incomingProfUuid == TC_BT_HF_PROFILE_UUID)
	{
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&pConnInst->targetBdAddr, pBdAddr) == TRUE)
		{
			if(pConnInst->targetProfile == TC_BT_HF_PROFILE_UUID)
			{
				found = 1;
			}
		}
	}
	else if(incomingProfUuid == TC_BT_ADVANCED_AUDIO_PROFILE_UUID)
	{
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&pConnInst->targetBdAddr, pBdAddr) == TRUE)
		{
			if(pConnInst->targetProfile == TC_BT_AUDIO_SINK_UUID)
			{
				found = 1;
			}
		}
	}
	else
	{
		/* reserved */
	}

	if(found == 1)
	{
		if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_DELAYING)
		{
            TCCAPI_Timer_SetTime(20, TIMER_BT_CONNECT_STREAM_ID, BtApp_GAP_ConnectForTimer);
		}
	}
}

TcBtInt32 BtApp_GAP_Connect(void)
{

	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* get first device */
	pConnDev = pConnInst->connDevList.first;
	if(pConnDev != NULL)
	{
		if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
		{
			/* get first profile */
			pConnProf = pConnDev->connProfList.first;
			if(pConnProf != NULL)
			{
                TCCAPI_Timer_SetTime(pConnProf->timerSettingVal, TIMER_BT_CONNECT_STREAM_ID, BtApp_GAP_ConnectForTimer);
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_DELAYING);
				
				pConnInst->targetBdAddr = pConnDev->bdAddr;
				pConnInst->targetProfile = pConnProf->profileUuid;
				pConnInst->targetInstanceIdx = pConnProf->instanceIdx;
			}
			else
			{
				ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_BUSY;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_NONE;
	}

	return ret;
}

static void BtApp_GAP_CancelConnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc)
{
    BtAppGapConnInstance_t *pCcanInst = (BtAppGapConnInstance_t *)BtApp_GAP_GetCcanInst();
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtInt32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	

	if(pRecoveryInst->doOffOn != TRUE)
	{
		/* Deactivate the profile(s) in the waiting list */
		fr = BtApp_GAP_DeactivateCommonFunc(pCcanInst->targetProfile, &pCcanInst->targetBdAddr, pCcanInst->targetInstanceIdx);
		
		if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			BtAppGapProfUuidInstIdx searchData;

			TCCAPI_Timer_SetTime(TC_BT_DEACTIVATE_TIMEOUT, TIMER_BT_DEACTIVATE_TIMEOUT_ID, BtApp_GAP_DeactivateTimeoutHandler);

			searchData.profileUuid = pCcanInst->targetProfile;
			searchData.instanceIdx = pCcanInst->targetInstanceIdx;
			if(BtApp_GAP_ListSearch((const BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, BtApp_GAP_RecoveryProfileFind, &searchData) == NULL)
			{
                BtAppGapRecoveryListElm_t *pRecoveryProf = (BtAppGapRecoveryListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapRecoveryListElm_t));
				if(pRecoveryProf != NULL)
				{
					pRecoveryProf->profileUuid = pCcanInst->targetProfile;
					pRecoveryProf->instanceIdx = pCcanInst->targetInstanceIdx;
					BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, (BtAppGapListElm_t *)pRecoveryProf);
				}
			}
		}
		else
		{
			pRecoveryInst->doOffOn = TRUE;
		}
	}

	pDevConnStatus = BtApp_GAP_GetDevConnStatus(&pCcanInst->targetBdAddr);
	if(pDevConnStatus != NULL)
	{
		if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
		{
			if((pDevConnStatus->ccanSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
			{
#if 1
				printf("BtApp_GAP_CancelConnectTimeoutForTimer CancelConnectCfm\n");
#else
                DMP_BT_SYSEvtCancelConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
			}
			BtApp_GAP_UnsetMainConnStatus(&pCcanInst->targetBdAddr, BTAPP_GAP_STATUS_CONNECT_CANCELLING);
		}
	}

	/* reset ccanInst */
	BtApp_GAP_InitCcanInst();
}

static TcBtInt32 BtApp_GAP_CancelCurrentOperation(void)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapCcanInstance_t *pCcanInst = BtApp_GAP_GetCcanInst();
	TcBtInt32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	
	if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
	{
		ret = BTAPP_GAP_FN_RESULT_NONE; /* nothing to cancel */
	}
	else if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_RAS)
	{
        fr = TC_BT_SD_CancelReadAvailableServices();
	}
	else if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_DELAYING)
    {
        TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_STREAM_ID);
	}
	else if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_CONNECTING)
	{
        TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_TIMEOUT_ID);
		
		/* cancel connect */
		switch(pConnInst->targetProfile)
		{
		#ifdef BT_HANDSFREE_UNIT
			case TC_BT_HF_PROFILE_UUID:
				if(BtApp_GAP_GetHfpState((const TcBtBdAddr *)&pConnInst->targetBdAddr, BTAPP_GAP_UNUSED_INST_IDX) == BTAPP_GAP_STATE_CONNECTING)
				{
                    fr = G_BtHfCmd->TC_BT_HF_CancelConnect((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetHfpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#ifdef BT_AUDIO_SINK
			case TC_BT_AUDIO_SINK_UUID:
				if(BtApp_GAP_GetA2dpState((const TcBtBdAddr *)&pConnInst->targetBdAddr) == BTAPP_GAP_STATE_CONNECTING)
				{
					fr = TC_BT_A2DP_CancelConnect((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetA2dpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
			case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
				if(BtApp_GAP_GetAvrcpState((const TcBtBdAddr *)&pConnInst->targetBdAddr) == BTAPP_GAP_STATE_CONNECTING)
				{
					fr = TC_BT_AVRCP_CancelConnect((const TcBtBdAddr *)&pConnInst->targetBdAddr);
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetAvrcpState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#ifdef BT_PBAP_CLIENT
			case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
				if(BtApp_GAP_GetPacState((const TcBtBdAddr *)&pConnInst->targetBdAddr) == BTAPP_GAP_STATE_CONNECTING)
				{
                    fr = BtApp_PAC_CancelConnectReq();
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetPacState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#ifdef BT_MAP_CLIENT
			case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
				if(BtApp_GAP_GetMapcState((const TcBtBdAddr *)&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx) == BTAPP_GAP_STATE_CONNECTING)
				{
					fr = TC_BT_MAPC_CancelConnectReq(0);
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetMapcState(&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#if 0//def BT_SPP /* SPP don't support cancel connect */
			case TC_BT_SPP_PROFILE_UUID:
				if(BtApp_GAP_GetSppState((const TcBtBdAddr *)&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx) == BTAPP_GAP_STATE_CONNECTING)
				{
					fr = TC_BT_SPP_CancelConnect(pConnInst->targetInstanceIdx);
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetSppState(&pConnInst->targetBdAddr, pConnInst->targetInstanceIdx, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#if 0//defined(BT_SPP) && defined(IPOD_BT_INCLUDE) /* IAP don't support cancel connect */
			case TC_BT_SPP_IAP_SERVICE_UUID:
				if(BtApp_GAP_GetIapState((const TcBtBdAddr *)&pConnInst->targetBdAddr) == BTAPP_GAP_STATE_CONNECTING)
				{
					fr = TC_BT_SPP_IAP_CancelConnect();
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						(void)BtApp_GAP_SetIapState(&pConnInst->targetBdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				}
				break;
		#endif
		#if 0//def BT_HIDD /* HIDD don't support cancel connect */		
		#endif
        #if 0// def BT_PAN /* PAN don't supprot cancel connect */
        #endif
  			default:
				ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				break;
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
	}
	if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
	{
        TCCAPI_Timer_SetTime(TC_BT_CANCEL_CONNECT_TIMEOUT, TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID, BtApp_GAP_CancelConnectTimeoutForTimer);
		BtApp_GAP_SetCcanStatus(BTAPP_GAP_CCAN_STATUS_CANCELLING);
		TC_BT_BdAddrCopyIfExist(&pCcanInst->targetBdAddr, (const TcBtBdAddr *)&pConnInst->targetBdAddr);
		pCcanInst->targetProfile = pConnInst->targetProfile;
		pCcanInst->targetInstanceIdx = pConnInst->targetInstanceIdx;
	}
	else if(fr == TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM)
	{
		ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
	}
	else
	{
		/* don't care */
	}

	if(ret == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
		pConnInst->targetProfile = 0;
		pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
	}

	return ret;
}

TcBtInt32 BtApp_GAP_CancelConnect(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapConnDevListElm_t *pConnDev;
	TcBtBdAddr bdAddr;
	TcBtInt32 removed = FALSE;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_NONE; /* nothing to cancel */

	if(pBdAddr == NULL)
	{
		if(BtApp_GAP_GetConnStatus() != BTAPP_GAP_CONN_STATUS_IDLE)
		{
			TC_BT_BdAddrCopyIfExist(&bdAddr, &pConnInst->targetBdAddr);
			
			/* cancel current operation */
			ret = BtApp_GAP_CancelCurrentOperation();
		}
		
		/* remove all element of device list */
		if(pConnInst->connDevList.count > 0)
		{
			BtApp_GAP_ListDeinit((BtAppGapList_t *)&pConnInst->connDevList);
			removed = TRUE;
		}
	}
	else
	{
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnInst->targetBdAddr), (const TcBtBdAddr *)pBdAddr) == TRUE)
		{
			TC_BT_BdAddrCopyIfExist(&bdAddr, pBdAddr);
			
			/* cancel current operation */
			ret = BtApp_GAP_CancelCurrentOperation();
		}
		
		/* find a device that matches bdAddr */
        pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
		if(pConnDev != NULL)
		{
            BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
			removed = TRUE;
		}
	}

	if(ret == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		BtAppGapDevConnStatusListElm_t *pDevConnStatus;
		TcBtUint16 sendConnResult = 0;
		
		pDevConnStatus = BtApp_GAP_GetDevConnStatus(&bdAddr);
		if(pDevConnStatus != NULL)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				sendConnResult = pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT;
				BtApp_GAP_UnsetMainConnStatus(&bdAddr, BTAPP_GAP_STATUS_CONNECTING);
			}
		}
		
		if(BtApp_GAP_GetCcanStatus() == BTAPP_GAP_CCAN_STATUS_CANCELLING)
		{
			BtApp_GAP_SetMainConnStatus(
				&bdAddr,
				BTAPP_GAP_STATUS_CONNECT_CANCELLING,
				sendConnResult|BTAPP_GAP_CONN_SIDE_LOCAL);
		}
		
		TC_BT_BdAddrZero(&(pConnInst->targetBdAddr));
		pConnInst->targetProfile = 0;
		pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
	}
	
	if(removed == TRUE)
	{
		if(pConnInst->connDevList.count == 0)
		{
			BtApp_GAP_InitConnInst();
		}
		ret = BTAPP_GAP_FN_RESULT_SUCCESS;
	}

	return ret;
}

TcBtInt32 BtApp_GAP_FullConnectionProcess(const TcBtBdAddr *pBdAddr, TcBtUint16 side)
{
	BtAppGapConnProfListElm_t connProf;
#if defined(BT_PBAP_CLIENT)
 	TcBtBool doNotAdd = FALSE;
#endif
	TcBtBool added = FALSE;

	connProf.next = NULL;
	connProf.tryCount = 2;
	connProf.valid = FALSE;

#ifdef BT_HANDSFREE_UNIT
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_HF_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_HF_PROFILE_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		connProf.timerSettingVal = 1;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_AUDIO_SINK
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_AUDIO_SINK_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_AUDIO_SINK_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		connProf.timerSettingVal = 1; /* Nokia 6110: When we try to connect A2DP, this phone disconnect pre-connected HFP and connect A2DP only. */
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#ifndef USE_BTBR
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		connProf.timerSettingVal = 1;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#endif
#ifdef BT_PBAP_CLIENT
 	if(doNotAdd != TRUE)
	{
		if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID) == TRUE)
		{
			BtApp_PB_SetConnectingBdAddr(pBdAddr);
			connProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
			connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
			connProf.timerSettingVal = 1;
			if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_MAP_CLIENT
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
		connProf.instanceIdx = 0;
		connProf.timerSettingVal = 1;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_SPP
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_SPP_PROFILE_UUID) == TRUE)
	{
		TcBtUint8 instIdx = BtApp_GAP_GetSppIdleInstIdx();
		if(instIdx != BTAPP_GAP_UNUSED_INST_IDX)
		{
			connProf.profileUuid = TC_BT_SPP_PROFILE_UUID;
			connProf.instanceIdx = instIdx;
			connProf.timerSettingVal = 1;
			if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_SPP_IAP_SERVICE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_SPP_IAP_SERVICE_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		connProf.timerSettingVal = 1;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_HIDD
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_HID_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_HID_PROFILE_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		connProf.timerSettingVal = 1;
		connProf.valid = TRUE; /* Caution!!!! : HID Don't support SDP. So put into force. */
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_PAN
    if ( BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_PANU_PROFILE_UUID) == TRUE ||
         BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_NAP_PROFILE_UUID) == TRUE ||
         BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_GN_PROFILE_UUID) == TRUE )
    {
        connProf.profileUuid = TC_BT_PAN_PANU_PROFILE_UUID;
        connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
        connProf.timerSettingVal = 1;
		connProf.valid = FALSE;

        if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
        {
            added = TRUE;
        }
    }
#endif

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			side);
	}

	return BtApp_GAP_Connect();
}

void BtApp_GAP_ObexSubConnectionProcess(const TcBtBdAddr *pBdAddr)
{
#if defined(BT_PBAP_CLIENT)
 	TcBtBool doNotAdd = FALSE;
#endif
	BtAppGapConnProfListElm_t connProf;
	TcBtBool added = FALSE;

	connProf.next = NULL;
	connProf.timerSettingVal = 1;
	connProf.tryCount = 2;
	connProf.valid = FALSE;

#ifdef BT_PBAP_CLIENT
 	if(doNotAdd != TRUE)
	{
		if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID) == TRUE)
		{
			BtApp_PB_SetConnectingBdAddr(pBdAddr);
			connProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
			connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
			if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_MAP_CLIENT
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
		connProf.instanceIdx = 0;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_PAN
    if ( BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_PANU_PROFILE_UUID) == TRUE ||
         BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_NAP_PROFILE_UUID) == TRUE ||
         BtApp_GAP_IsConnectable(pBdAddr, TC_BT_PAN_GN_PROFILE_UUID) == TRUE )
    {
        connProf.profileUuid = TC_BT_PAN_PANU_PROFILE_UUID;
        connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;

        if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
        {
            added = TRUE;
        }
    }
#endif


	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			BTAPP_GAP_CONN_SIDE_REMOTE);
	}

	(void)BtApp_GAP_Connect();
}

void BtApp_GAP_IncomingConnectionBackupProcess(const TcBtBdAddr *pBdAddr)
{
#if defined(BT_PBAP_CLIENT)
 	TcBtBool doNotAdd = 0;
#endif
	BtAppGapConnProfListElm_t connProf;
	TcBtBool added = FALSE;

	connProf.next = NULL;
	connProf.timerSettingVal = 6;
	connProf.tryCount = 2;
	connProf.valid = FALSE;

#ifdef BT_HANDSFREE_UNIT
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_HF_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_HF_PROFILE_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_AUDIO_SINK
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_AUDIO_SINK_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_AUDIO_SINK_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#ifndef USE_BTBR
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;		
		connProf.timerSettingVal = 4;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#endif
#ifdef BT_PBAP_CLIENT
  	if(doNotAdd != 1)
	{
		if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID) == TRUE)
		{
			BtApp_PB_SetConnectingBdAddr(pBdAddr);
			connProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
			connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;		
			connProf.timerSettingVal = 1;
			if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_MAP_CLIENT
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
		connProf.instanceIdx = 0;
		connProf.timerSettingVal = 1;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			BTAPP_GAP_CONN_SIDE_REMOTE);
	}

	(void)BtApp_GAP_Connect();
}

void BtApp_GAP_AvrcpSubConnectionProcess(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnProfListElm_t connProf;
	TcBtBool added = FALSE;

	connProf.next = NULL;
	connProf.timerSettingVal = 4; /* Exception Case : PTS v.4.5.0.10 (Use timer to avoid connection initiated by slave in some test cases.) */
	connProf.tryCount = 2;
	connProf.valid = FALSE;

#ifdef BT_AUDIO_SINK
#ifndef USE_BTBR
	if(BtApp_GAP_IsConnectable(pBdAddr, TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID) == TRUE)
	{
		connProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#endif

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			BTAPP_GAP_CONN_SIDE_REMOTE);
	}

	(void)BtApp_GAP_Connect();
}

void BtApp_GAP_PacConnect(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnProfListElm_t connProf;
	TcBtBool added = FALSE;
	
	connProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
	connProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
	connProf.timerSettingVal = 6;
	connProf.tryCount = 1;
	connProf.valid = TRUE;

	if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		added = TRUE;
	}

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			BTAPP_GAP_CONN_SIDE_LOCAL);
	}
	
	(void)BtApp_GAP_Connect();
}

void BtApp_GAP_MapcConnect(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx)
{
	BtAppGapConnProfListElm_t connProf;
	TcBtBool added = FALSE;
	
	connProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
	connProf.instanceIdx = instanceIdx;
	connProf.timerSettingVal = 1;
	connProf.tryCount = 1;
	connProf.valid = TRUE;

	if(BtApp_GAP_AddToConnProfList(pBdAddr, &connProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		added = TRUE;
	}

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_CONNECTING,
			BTAPP_GAP_CONN_SIDE_LOCAL);
	}

	(void)BtApp_GAP_Connect();
}
 
TcBtInt32 BtApp_GAP_ReflectConnResult(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapCcanInstance_t *pCcanInst = BtApp_GAP_GetCcanInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtBool connFlag = FALSE;
	TcBtBool ccanFlag = FALSE;
	TcBtBool discFlag = FALSE;
	TcBtBool resultDiscFlag = FALSE;
	TcBtBool resultDiscAllFlag = FALSE;
	TcBtBool removeDone = FALSE;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pBdAddr == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* 1. Check Process Status */
	/* 1.1 Connection Process */
	/* 1.1.a Check Current Processing */
	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pBdAddr) == TRUE)
	{
		if(pConnInst->targetProfile == profileUuid)
		{
			if(pConnInst->targetInstanceIdx == instanceIdx)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_TIMEOUT_ID);
			#if 0/* This might be excessive. */
				BtApp_GAP_CancelCurrentOperation();
			#endif
				
				TC_BT_BdAddrZero(&(pConnInst->targetBdAddr));
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				connFlag = TRUE;
			}
		}
	}
	/* 1.1.b Check connProfList */
	pConnProf = BtApp_GAP_GetConnProfile(pBdAddr, profileUuid, instanceIdx);
	if(pConnProf != NULL)
	{
		pConnDev = pConnProf->pDevice;

		switch(result)
		{
			case BTAPP_GAP_CONN_RESULT_SUCCESS:
			{/* remove corresponding element from the list. */
				BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
				if(pConnDev->connProfList.count == 0)
				{
					BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
					connFlag = TRUE;
				}
				break;
			}
			case BTAPP_GAP_CONN_RESULT_FAILURE:
			{/* move corresponding element to last of the list. */
				pConnProf->timerSettingVal = pConnProf->timerSettingVal + 2;
				BtApp_GAP_ListElementMoveLast((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
				break;
			}
			case BTAPP_GAP_CONN_RESULT_CANCELLED:
			{
				BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
				if(pConnDev->connProfList.count == 0)
				{
					BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
					connFlag = TRUE;
				}
				break;
			}
			default:
				ret = BTAPP_GAP_FN_RESULT_UNEXPECTED;
				break;
		}
	}
	/* 1.2 Connect Cancellation Process */
	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pCcanInst->targetBdAddr), pBdAddr) == TRUE)
	{
		if(pCcanInst->targetProfile == profileUuid)
		{
			if(pCcanInst->targetInstanceIdx == instanceIdx)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID);
				BtApp_GAP_InitCcanInst();
				ccanFlag = TRUE;
			}
		}
	}
	/* 1.3 Disconnection Process */
	pDiscProf = BtApp_GAP_GetDiscProfileFromRTD((const TcBtBdAddr *)pBdAddr, profileUuid, instanceIdx);
	if(pDiscProf != NULL)
	{
		pDiscDev = pDiscProf->pDevice;
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->readyToDiscProfList,(BtAppGapListElm_t *)pDiscProf);
		removeDone = TRUE;
	}

	pDiscProf = BtApp_GAP_GetDiscProfileFromWFR((const TcBtBdAddr *)pBdAddr, profileUuid, instanceIdx);
	if(pDiscProf != NULL)
	{
		pDiscDev = pDiscProf->pDevice;
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->waitForRespProfList,(BtAppGapListElm_t *)pDiscProf);
		removeDone = TRUE;
	}
	if(removeDone == TRUE)
	{
		if((pDiscDev->readyToDiscProfList.count == 0) && (pDiscDev->waitForRespProfList.count == 0)) /* QAC Msg(4:3353) */
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscInst->discDevList,(BtAppGapListElm_t *)pDiscDev);
			discFlag = TRUE;

			if(pDiscInst->discDevList.count > 0)
			{
				if(pDiscInst->discAllFlag != TRUE)
				{
					resultDiscFlag = TRUE;
				}
			}
			else
			{
				if(pDiscInst->discAllFlag != TRUE)
				{
					resultDiscFlag = TRUE;
				}
				else
				{
					resultDiscAllFlag = TRUE;
				}
                TCCAPI_Timer_CancelTime(TIMER_BT_DISCONNECT_TIMEOUT_ID);
				BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_IDLE);
				pDiscInst->discAllFlag = FALSE;
			}
		}
	}

	/* 2. Check overallStatus & devConnStatus */
	/* If necessary, send the result to UI & Update mainConnStatusInst. */
	pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
	if(pDevConnStatus != NULL)
	{
		/* 2.1 Connection Process */
		if(connFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
				{
					if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
					{
						if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
						{
#if 1
							printf("BtApp_GAP_ReflectConnResult ConnectCfm\n");
#else
                            DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
					}
				}
			}
		}
		/* 2.2 Connect Cancellation Process */
		if(ccanFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
				{
#if 1
					printf("BtApp_GAP_ReflectConnResult CancelConnectCfm\n");
#else
                    DMP_BT_SYSEvtCancelConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECT_CANCELLING);
			}
		}
		/* 2.3 Disconnection Process */
		if(discFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
			{
				if((resultDiscFlag == TRUE) || (resultDiscAllFlag == TRUE))
				{
					if((pDevConnStatus->discSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
					{
						if(resultDiscFlag == TRUE)
						{
#if 1
							printf("BtApp_GAP_ReflectConnResult DiconnectCfm\n");
#else
                            DMP_BT_SYSEvtDisconnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						else
						{
#if 1
							printf("BtApp_GAP_ReflectConnResult DisconnectAllCfm\n");
#else
                            DMP_BT_SYSEvtDisconnectAllCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
					}
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_DISCONNECTING);
			}
		}
	}

	(void)BtApp_GAP_Connect();
    TcBtEvt_DMEvt(BT_DM_EVT_REFRESH,NULL);
	return ret;
}

TcBtInt32 BtApp_GAP_ValidateConnProfList(const TcBtBdAddr *pBdAddr, const TcBtUint32 *pServiceList, TcBtUint32 serviceCount, TcBtUint16 result)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapCcanInstance_t *pCcanInst = BtApp_GAP_GetCcanInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf, *pNextConnProf;
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtBool connFlag = FALSE;
	TcBtBool ccanFlag = FALSE;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* 1. Check Process Status */
	switch(result)
	{
		case BTAPP_GAP_RAS_RESULT_SUCCESS:
		{
			/* 1. Check Process Status */
			/* 1.1 Connection Process */
			/* param check */
			if(pBdAddr == NULL)
			{/* invalid parameter */
				ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
				return ret;
			}

			/* Check target */
			if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
			{
				TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				connFlag = TRUE;
			}

			/* find a device that matches bdAddr */
            pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
			if(pConnDev != NULL)
			{
				if((pServiceList != NULL) && (serviceCount > 0))
				{
					/* If there is not service list, store it. */
					if((pConnDev->pServiceList == NULL) && (pConnDev->serviceCount == 0))
					{
                        pConnDev->pServiceList = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32)*serviceCount);
						memcpy(pConnDev->pServiceList, pServiceList, sizeof(TcBtUint32)*serviceCount);
						pConnDev->serviceCount = serviceCount;
					}

					/* Validate profile list of found device */
					for(pConnProf = pConnDev->connProfList.first; pConnProf != NULL; pConnProf = pNextConnProf)
					{
						pNextConnProf = pConnProf->next;
						if(pConnProf->valid == FALSE)
						{
							if(BtApp_GAP_IsInServiceList(pConnProf->profileUuid, pServiceList, serviceCount) == TRUE)
							{/* remote device support this profile, so set the valid as TRUE */
								pConnProf->valid = TRUE;
							}
							else
							{/* remote device does NOT support this profile, so remove this from the list */
								BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
								if(pConnDev->connProfList.count == 0)
								{
									BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
									connFlag = TRUE;
								}
							}
						}
					}
				}
				else
				{
					ret = BTAPP_GAP_FN_RESULT_UNEXPECTED; /* unexpected */
				}
			}
			else
			{
				ret = BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND; /* device not found */
			}

			/* 1.2 Connect Cancellation Process */
			if(TC_BT_BdAddrEq(&pCcanInst->targetBdAddr, pBdAddr) == TRUE)
			{
				TCCAPI_Timer_CancelTime(TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID);
				BtApp_GAP_InitCcanInst();
				ccanFlag = TRUE;
			}
			break;
		}
		case BTAPP_GAP_RAS_RESULT_FAILURE:
		{
			/* 1. Check Process Status */
			/* 1.1 Connection Process */
			/* Check target */
			if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
			{
				TC_BT_BdAddrZero(&pConnInst->targetBdAddr);
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				connFlag = TRUE;
			}
			
            pConnDev = (BtAppGapConnDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr);
			if(pConnDev != NULL)
			{
				pConnDev->retryCountForRas--;
				if(pConnDev->retryCountForRas == 0)
				{
					BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
					connFlag = TRUE;
				}
			}
			else
			{
				ret = BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND; /* device not found */
			}

			/* 1.2 Connect Cancellation Process */
			if(TC_BT_BdAddrEq(&pCcanInst->targetBdAddr, pBdAddr) == TRUE)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID);
				BtApp_GAP_InitCcanInst();
				ccanFlag = TRUE;
			}
			break;
		}
		case BTAPP_GAP_RAS_RESULT_CANCELLED:
		{
			if(BtApp_GAP_GetCcanStatus() == BTAPP_GAP_CCAN_STATUS_CANCELLING)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID);
				BtApp_GAP_InitCcanInst();
				ccanFlag = TRUE;
			}
			else
			{
				ret = BTAPP_GAP_FN_RESULT_UNEXPECTED; /* unexpected */
			}
			break;
		}
		default:
			break;
	}

	/* 2. Check overallStatus & devConnStatus */
	/* If necessary, send the result to UI & Update mainConnStatusInst. */
	pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
	if(pDevConnStatus != NULL)
	{
		/* 2.1 Connection Process */
		if(connFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
				{
					if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
					{
						if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
						{
#if 1
							printf("BtApp_GAP_ValidateConnProfList ConnectCfm\n");
#else
                            DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
					}
				}
			}
		}
		/* Connect Cancellation Process */
		if(ccanFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
				{
#if 1
					printf("BtApp_GAP_ValidateConnProfList CacelConnectCfm\n");
#else
                    DMP_BT_SYSEvtCancelConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECT_CANCELLING);
			}
		}
	}

	(void)BtApp_GAP_Connect();

	return ret;
}

/**************************************************************************
								FOR DISCONNECT
**************************************************************************/
static TcBtUint16 BtApp_GAP_GetDiscStatus(void)
{
	return discInst.discStatus;
}

static void BtApp_GAP_SetDiscStatus(TcBtUint32 discStatus)
{
	discInst.discStatus = discStatus;
}

void BtApp_GAP_InitDiscInst(void)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	
	BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_IDLE);
	pDiscInst->discAllFlag = FALSE;
	BtApp_GAP_ListInit((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceRemove);
}

TcBtInt32 BtApp_GAP_AddToDiscProfList(const TcBtBdAddr *pBdAddr, const BtAppGapDiscProfListElm_t *pDiscProfile)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if((pBdAddr == NULL) || (pDiscProfile == NULL))
	{/* invalid parameter */
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;
		
		searchData.profileUuid = pDiscProfile->profileUuid;
		searchData.instanceIdx = pDiscProfile->instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
			foundProf = 1;
		}
		else
		{
            pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->waitForRespProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
			if(pDiscProf != NULL)
			{
				foundProf = 1;
			}
		}
	}
	else
	{/* add new device */
        pDiscDev = (BtAppGapDiscDevListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDiscDevListElm_t));
		TC_BT_BdAddrCopyIfExist(&(pDiscDev->bdAddr), pBdAddr);
        BtApp_GAP_ListInit((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, NULL);
        BtApp_GAP_ListInit((BtAppGapList_t *)&pDiscDev->waitForRespProfList, NULL);
		
		(void)BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
	}

	if(foundProf == 1)
	{
		ret = BTAPP_GAP_FN_RESULT_ALREADY_EXIST; /* profile exist already */
	}
	else
	{/* add new profile */
        pDiscProf = (BtAppGapDiscProfListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDiscProfListElm_t));
		pDiscProf->pDevice = pDiscDev;
		pDiscProf->profileUuid = pDiscProfile->profileUuid;
		pDiscProf->instanceIdx = pDiscProfile->instanceIdx;
		pDiscProf->timerSettingVal = pDiscProfile->timerSettingVal;
		pDiscProf->tryCount = pDiscProfile->tryCount;
		
		(void)BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
	}

	return ret;
}

TcBtInt32 BtApp_GAP_AddToDiscProfListFirst(const TcBtBdAddr *pBdAddr, const BtAppGapDiscProfListElm_t *pDiscProfile)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if((pBdAddr == NULL) || (pDiscProfile == NULL))
	{/* invalid parameter */
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;
		
		searchData.profileUuid = pDiscProfile->profileUuid;
		searchData.instanceIdx = pDiscProfile->instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
			foundProf = 1;
		}
		else
		{
            pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->waitForRespProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
			if(pDiscProf != NULL)
			{
				foundProf = 1;
			}
		}
	}
	else
	{/* add new device */
        pDiscDev = (BtAppGapDiscDevListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDiscDevListElm_t));
		TC_BT_BdAddrCopyIfExist(&(pDiscDev->bdAddr), pBdAddr);
        BtApp_GAP_ListInit((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, NULL);
        BtApp_GAP_ListInit((BtAppGapList_t *)&pDiscDev->waitForRespProfList, NULL);
		
		(void)BtApp_GAP_ListElementAddFirst((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
	}

	if(foundProf == 1)
	{
		ret = BTAPP_GAP_FN_RESULT_ALREADY_EXIST; /* profile exist already */
	}
	else
	{/* add new profile */
        pDiscProf = (BtAppGapDiscProfListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapDiscProfListElm_t));
		pDiscProf->pDevice = pDiscDev;
		pDiscProf->profileUuid = pDiscProfile->profileUuid;
		pDiscProf->instanceIdx = pDiscProfile->instanceIdx;
		pDiscProf->timerSettingVal = pDiscProfile->timerSettingVal;
		pDiscProf->tryCount = pDiscProfile->tryCount;
		
		(void)BtApp_GAP_ListElementAddFirst((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
	}

	return ret;
}

TcBtInt32 BtApp_GAP_RemoveFromDiscProfList(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	TcBtBool foundProf = 0;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pBdAddr == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;
		
		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
			foundProf = 1;
		}
		
		if(foundProf == 1)
		{/* remove found profile */
			(void)BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
			if(pDiscDev->readyToDiscProfList.count == 0)
			{/* remove empty device */
				(void)BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
			}
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_PROF_NOT_FOUND; /* profile not found */
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND; /* device not found */
	}

	return ret;
}

BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfile(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	BtAppGapDiscDevListElm_t *pRet = NULL;

	/* param check */
	if(pBdAddr == NULL)
	{/* invalid parameter */
        return (BtAppGapDiscProfListElm_t *)pRet;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;

		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
            pRet = (BtAppGapDiscDevListElm_t *)pDiscProf;
		}
		else
		{
            pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->waitForRespProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
			if(pDiscProf != NULL)
			{
                pRet = (BtAppGapDiscDevListElm_t *)pDiscProf;
			}
		}
	}
	
    return (BtAppGapDiscProfListElm_t *)pRet;
}

BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfileFromRTD(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	BtAppGapDiscDevListElm_t *pRet = NULL;

	/* param check */
	if(pBdAddr == NULL)
	{/* invalid parameter */
        return (BtAppGapDiscProfListElm_t *)pRet;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;

		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
            pRet = (BtAppGapDiscDevListElm_t *)pDiscProf;
		}
	}
	
    return (BtAppGapDiscProfListElm_t *)pRet;
}

BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfileFromWFR(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtBool foundDev = 0;
	BtAppGapDiscDevListElm_t *pRet = NULL;

	/* param check */
	if(pBdAddr == NULL)
	{/* invalid parameter */
        return (BtAppGapDiscProfListElm_t *)pRet;
	}

	/* find a device that matches bdAddr */
    pDiscDev = (BtAppGapDiscDevListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscInst->discDevList, BtApp_GAP_DiscDeviceFind, (const void *)pBdAddr);
	if(pDiscDev != NULL)
	{
		foundDev = 1;
	}

	if(foundDev == 1)
	{/* find a profile that matches uuid */
		BtAppGapProfUuidInstIdx searchData;

		searchData.profileUuid = profileUuid;
		searchData.instanceIdx = instanceIdx;
        pDiscProf = (BtAppGapDiscProfListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pDiscDev->waitForRespProfList, BtApp_GAP_DiscProfileFind, (const void *)&searchData);
		if(pDiscProf != NULL)
		{
            pRet = (BtAppGapDiscDevListElm_t *)pDiscProf;
		}
	}
	
    return (BtAppGapDiscProfListElm_t *)pRet;
}

static void BtApp_GAP_DisconnectCommonFunc(void)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	TcBtUint32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	TcBtUint16 state;
	TcBtBool streamClosing = FALSE;
	TcBtBool discFlag = FALSE;

	/* get first device */
	pDiscDev = pDiscInst->discDevList.first;
	if(pDiscDev != NULL)
	{
		/* get first profile from the ready list */
		pDiscProf = pDiscDev->readyToDiscProfList.first;
		if(pDiscProf != NULL)
		{
			switch(pDiscProf->profileUuid)
			{
			#ifdef BT_HANDSFREE_UNIT
				case TC_BT_HF_PROFILE_UUID:
				{
					state = BtApp_GAP_GetHfpState((const TcBtBdAddr *)&pDiscDev->bdAddr, BTAPP_GAP_UNUSED_INST_IDX);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
                        fr = G_BtHfCmd->TC_BT_HF_Disconnect(&pDiscDev->bdAddr);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetHfpState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_AUDIO_SINK
				case TC_BT_AUDIO_SINK_UUID:
				{
#if 0
					if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_OPEN) == TRUE)
					{
						streamClosing = TRUE;
					}
					else
#endif
					{
						state = BtApp_GAP_GetA2dpState((const TcBtBdAddr *)&pDiscDev->bdAddr);
						if(state == BTAPP_GAP_STATE_CONNECTED)
						{
							fr = TC_BT_A2DP_Disconnect();
							if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
							{
								(void)BtApp_GAP_SetA2dpState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
							}
						}
					}
					break;
				}
				case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
				{
					state = BtApp_GAP_GetAvrcpState((const TcBtBdAddr *)&pDiscDev->bdAddr);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
						fr = TC_BT_AVRCP_Disconnect();
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetAvrcpState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_PBAP_CLIENT
				case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
				{
					state = BtApp_GAP_GetPacState((const TcBtBdAddr *)&pDiscDev->bdAddr);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
                        fr = BtApp_PAC_DisconnectReq();
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetPacState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
 			#ifdef BT_MAP_CLIENT
				case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
				{
					state = BtApp_GAP_GetMapcState((const TcBtBdAddr *)&pDiscDev->bdAddr, pDiscProf->instanceIdx);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
						fr = TC_BT_MAPC_DisconnectReq(pDiscProf->instanceIdx);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetMapcState(&pDiscDev->bdAddr, pDiscProf->instanceIdx, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_SPP
				case TC_BT_SPP_PROFILE_UUID:
				{
					state = BtApp_GAP_GetSppState((const TcBtBdAddr *)&pDiscDev->bdAddr, pDiscProf->instanceIdx);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
						fr = TC_BT_SPP_Disconnect(pDiscProf->instanceIdx);
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetSppState(&pDiscDev->bdAddr, pDiscProf->instanceIdx, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
			#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
				case TC_BT_SPP_IAP_SERVICE_UUID:
				{
					state = BtApp_GAP_GetIapState((const TcBtBdAddr *)&pDiscDev->bdAddr);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
						fr = TC_BT_SPP_IAP_Disconnect();
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetIapState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
			#ifdef BT_HIDD
				case TC_BT_HID_PROFILE_UUID:
				{
					state = BtApp_GAP_GetHiddState((const TcBtBdAddr *)&pDiscDev->bdAddr);
					if(state == BTAPP_GAP_STATE_CONNECTED)
					{
						fr = TC_BT_HIDD_Disconnect();
						if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
						{
							(void)BtApp_GAP_SetHiddState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
						}
					}
					break;
				}
			#endif
            #ifdef BT_PAN
                case TC_BT_PAN_PANU_PROFILE_UUID:
                {
                    state = BtApp_GAP_GetPANState((const TcBtBdAddr *)&pDiscDev->bdAddr);
                    if(state == BTAPP_GAP_STATE_CONNECTED)
                    {
                        // PANTODO At this time we will use only one connection for PAN
                        fr = TC_BT_PAN_Disconnect(0);
                        if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
                        {
                            (void)BtApp_GAP_SetPANState(&pDiscDev->bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
                        }
                    }
                    break;
                }
            #endif
				default:
					break;
			}

			if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
			{/* connect function success */
				TCCAPI_Timer_SetTime(TC_BT_DISCONNECT_TIMEOUT, TIMER_BT_DISCONNECT_TIMEOUT_ID, BtApp_GAP_DisconnectTimeoutForTimer);
				
				pDiscProf->tryCount--;
				BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
				BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pDiscDev->waitForRespProfList, (BtAppGapListElm_t *)pDiscProf);

				if(pDiscDev->readyToDiscProfList.count == 0)
				{
					BtApp_GAP_ListElementMoveLast((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
				}
			}
#if 0
			else if(streamClosing == TRUE)
			{/* waiting for closing stream. */
				/* do nothing */
			}
#endif
			else
			{/* connect function failure */
				BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
				if(pDiscDev->readyToDiscProfList.count == 0)
				{
					if(pDiscDev->waitForRespProfList.count == 0)
					{
						TcBtBdAddr discBdAddr;
						TC_BT_BdAddrCopyIfExist(&discBdAddr, &pDiscDev->bdAddr);
						BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
						
						if(pDiscInst->discDevList.count > 0)
						{
							if(pDiscInst->discAllFlag != TRUE)
							{
								discFlag = TRUE;
							}
						}
						else
						{
							discFlag = TRUE;
                            TCCAPI_Timer_CancelTime(TIMER_BT_DISCONNECT_TIMEOUT_ID);
							BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_IDLE);
						}
						if(discFlag == TRUE)
						{
							BtAppGapDevConnStatusListElm_t *pDevConnStatus;
							pDevConnStatus = BtApp_GAP_GetDevConnStatus(&discBdAddr);
							if(pDevConnStatus != NULL)
							{
								if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
								{
									if((pDevConnStatus->discSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
									{
										if(pDiscInst->discAllFlag == FALSE)
										{
#if 1
											printf("BtApp_GAP_DisconnectCommonFunc DisconnectCfm\n");
#else
                                            DMP_BT_SYSEvtDisconnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
										}
										else
										{
#if 1
											printf("BtApp_GAP_DisconnectCommonFunc DisconnectAllCfm\n");
#else
                                            DMP_BT_SYSEvtDisconnectAllCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
										}
									}
									pDiscInst->discAllFlag = FALSE;
									BtApp_GAP_UnsetMainConnStatus(&discBdAddr, BTAPP_GAP_STATUS_DISCONNECTING);
								}
							}
						}
					}
					else
					{
						BtApp_GAP_ListElementMoveLast((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
					}
				}
			}

			/* Try to disconnect next profile */
			pDiscDev = pDiscInst->discDevList.first;
			if(pDiscDev != NULL)
			{
				pDiscProf = pDiscDev->readyToDiscProfList.first;
				if(pDiscProf != NULL)
				{
                    TCCAPI_Timer_SetTime(pDiscProf->timerSettingVal, TIMER_BT_DISCONNECT_STREAM_ID, BtApp_GAP_DisconnectForTimer);
				}
			}
		}
	}
}

static void BtApp_GAP_DisconnectForTimer(TcBtUint8 id, TcBtUint8 loc)
{
	BtApp_GAP_DisconnectCommonFunc();
}

static void BtApp_GAP_DisconnectTimeoutForTimer(TcBtUint8 id, TcBtUint8 loc)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapDiscDevListElm_t *pDiscDev, *pDiscNextDev;
	BtAppGapDiscProfListElm_t *pDiscProf, *pDiscNextProf;
	TcBtInt32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;

	
	if(pRecoveryInst->doOffOn != TRUE)
	{
		/* Deactivate the profile(s) in the waiting list */
		for(pDiscDev = pDiscInst->discDevList.first; pDiscDev != NULL; pDiscDev = pDiscNextDev)
		{
			pDiscNextDev = pDiscDev->next;
			for(pDiscProf = pDiscDev->waitForRespProfList.first; pDiscProf != NULL; pDiscProf = pDiscNextProf)
			{
				pDiscNextProf = pDiscProf->next;
				if(pRecoveryInst->doOffOn != TRUE)
				{
					fr = BtApp_GAP_DeactivateCommonFunc(pDiscProf->profileUuid, &pDiscDev->bdAddr, pDiscProf->instanceIdx);
					
					if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
					{
						BtAppGapProfUuidInstIdx searchData;

						TCCAPI_Timer_SetTime(TC_BT_DEACTIVATE_TIMEOUT, TIMER_BT_DEACTIVATE_TIMEOUT_ID, BtApp_GAP_DeactivateTimeoutHandler);
				
						searchData.profileUuid = pDiscProf->profileUuid;
						searchData.instanceIdx = pDiscProf->instanceIdx;
						if(BtApp_GAP_ListSearch((const BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, BtApp_GAP_RecoveryProfileFind, &searchData) == NULL)
						{
                            BtAppGapRecoveryListElm_t *pRecoveryProf = (BtAppGapRecoveryListElm_t *)TC_BT_Pmalloc(sizeof(BtAppGapRecoveryListElm_t));
							if(pRecoveryProf != NULL)
							{
								pRecoveryProf->profileUuid = pDiscProf->profileUuid;
								pRecoveryProf->instanceIdx = pDiscProf->instanceIdx;
								BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, (BtAppGapListElm_t *)pRecoveryProf);
							}
						}
					}
					else
					{
						/* clean up waitingForDactRespList */
						BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList);

						pRecoveryInst->doOffOn = TRUE;
					}
				}
				
				BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->waitForRespProfList, (BtAppGapListElm_t *)pDiscProf);
				if(pDiscDev->waitForRespProfList.count == 0)
				{
					TcBtBdAddr bdAddr;
					TcBtBool resultDiscFlag = FALSE;
					TcBtBool resultDiscAllFlag = FALSE;
					BtAppGapDevConnStatusListElm_t *pDevConnStatus;

					TC_BT_BdAddrCopyIfExist(&bdAddr, &pDiscDev->bdAddr); /* copy bdAddr before remove device */
					BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
					
					if(pDiscInst->discDevList.count > 0)
					{
						if(pDiscInst->discAllFlag != TRUE)
						{
							resultDiscFlag = TRUE;
						}
					}
					else
					{
						if(pDiscInst->discAllFlag != TRUE)
						{
							resultDiscFlag = TRUE;
						}
						else
						{
							resultDiscAllFlag = TRUE;
						}
						BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_IDLE);
						pDiscInst->discAllFlag = FALSE;
					}

					pDevConnStatus = BtApp_GAP_GetDevConnStatus(&bdAddr);
					if(pDevConnStatus != NULL)
					{
						if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
						{
							if((resultDiscFlag == TRUE) || (resultDiscAllFlag == TRUE))
							{
								if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
								{
									if(resultDiscFlag == TRUE)
									{
#if 1
										printf("BtApp_GAP_DisconnectTimeoutForTimer DisconnectCfm\n");
#else
                                        DMP_BT_SYSEvtDisconnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
									}
									else
									{
#if 1
										printf("BtApp_GAP_DisconnectTimeoutForTimer DisconnectAllCfm\n");
#else
                                        DMP_BT_SYSEvtDisconnectAllCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
									}
								}
							}
							BtApp_GAP_UnsetMainConnStatus(&bdAddr, BTAPP_GAP_STATUS_DISCONNECTING);
						}
					}
				}
			}
		}
	}
}

static void BtApp_GAP_ActivateForRefreshing(void)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapRecoveryListElm_t *pActvProf, *pActvNextProf;
	TcBtInt32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
	
	/* Activate the profile(s) in the waiting list */
	for(pActvProf = pRecoveryInst->dactResultSuccessList.first; pActvProf != NULL; pActvProf = pActvNextProf)
	{
		pActvNextProf = pActvProf->next;
		switch(pActvProf->profileUuid)
		{
			case TC_BT_HF_PROFILE_UUID:
			{
#ifdef BT_HANDSFREE_UNIT
                fr = G_BtHfCmd->TC_BT_HF_Activate();
#endif
				break;
			}
			case TC_BT_AUDIO_SINK_UUID:
			{
#ifdef BT_AUDIO_SINK
				fr = TC_BT_A2DP_Activate();
#endif
				break;
			}
			case TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID:
			{
#ifdef BT_AUDIO_SINK
                fr = TC_BT_AVRCP_Activate((unsigned char *)"AVRCP CT", (unsigned char *)"TeleChips");
#endif
				break;
			}
			case TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID:
			{
				/* PAC does not provide activate function. */
				break;
			}
			case TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID:
			{
				/* MAPC does not provide activate function. */
				break;
			}
			case TC_BT_SPP_PROFILE_UUID:
			{
#ifdef BT_SPP
				fr = TC_BT_SPP_Activate(pActvProf->instanceIdx);
#endif
				break;
			}
			case TC_BT_SPP_IAP_SERVICE_UUID:
			{
#ifdef BT_SPP
				fr = TC_BT_SPP_IAP_Activate();
#endif
				break;
			}
			case TC_BT_HID_PROFILE_UUID:
			{
				/* HIDD does not provide activate function. */
				break;
			}
            case TC_BT_PAN_PANU_PROFILE_UUID :
            case TC_BT_PAN_NAP_PROFILE_UUID :
            case TC_BT_PAN_GN_PROFILE_UUID :
            {
#ifdef BT_PAN
                fr = TC_BT_PAN_Activate();
#endif
                break;
            }
 			default:
				break;
		}

		if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			TCCAPI_Timer_SetTime(TC_BT_ACTIVATE_TIMEOUT, TIMER_BT_ACTIVATE_TIMEOUT_ID, BtApp_GAP_ActivateTimeoutHandler);
			
			BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList, (BtAppGapListElm_t *)pActvProf);
			BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, (BtAppGapListElm_t *)pActvProf);
		}
		else
		{
			/* clean up dactResultSuccessList */
			BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList);
			
			pRecoveryInst->doOffOn = TRUE;
			break;
		}
	}
}

TcBtInt32 BtApp_GAP_Disconnect(void)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	if(BtApp_GAP_GetDiscStatus() == BTAPP_GAP_DISC_STATUS_IDLE)
	{
		if(pDiscInst->discDevList.count > 0)
		{
			BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_DISCONNECTING);
			BtApp_GAP_DisconnectCommonFunc();
		}
		else
		{
			ret = BTAPP_GAP_FN_RESULT_NONE; /* nothing to disconnect */
		}
	}
	else if(BtApp_GAP_GetDiscStatus() == BTAPP_GAP_DISC_STATUS_DISCONNECTING)
	{
        if(TCCAPI_Timer_BT_IsActiveTime(TIMER_BT_DISCONNECT_STREAM_ID) != TRUE)
		{
			if(pDiscInst->discDevList.count > 0)
			{
				BtApp_GAP_DisconnectCommonFunc();
			}
			else
			{
				ret = BTAPP_GAP_FN_RESULT_NONE; /* nothing to disconnect */
			}
		}
        else
		{
			ret = BTAPP_GAP_FN_RESULT_BUSY; /* disconnecting already */
		}
	}
	else
	{
		ret = BTAPP_GAP_FN_RESULT_BUSY; /* refreshing */
	}

	return ret;
}

TcBtInt32 BtApp_GAP_ReflectDiscResult(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	BtAppGapCcanInstance_t *pCcanInst = BtApp_GAP_GetCcanInst();
	BtAppGapConnDevListElm_t *pConnDev;
	BtAppGapConnProfListElm_t *pConnProf;
	BtAppGapDiscDevListElm_t *pDiscDev;
	BtAppGapDiscProfListElm_t *pDiscProf;
	BtAppGapDevConnStatusListElm_t *pDevConnStatus;
	TcBtBool connFlag = FALSE;
	TcBtBool ccanFlag = FALSE;
	TcBtBool discFlag = FALSE;
	TcBtBool resultDiscFlag = FALSE;
	TcBtBool resultDiscAllFlag = FALSE;
	TcBtBool removeDone = FALSE;
	TcBtInt32 ret = BTAPP_GAP_FN_RESULT_SUCCESS;

	/* param check */
	if(pBdAddr == NULL)
	{
		ret = BTAPP_GAP_FN_RESULT_INVALID_PARAM;
		return ret;
	}

	/* 1. Check Process Status */
	/* 1.1 Connection Process */
	/* 1.1.a Check Current Processing */
	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pConnInst->targetBdAddr), pBdAddr) == TRUE)
	{
		if(pConnInst->targetProfile == profileUuid)
		{
			if(pConnInst->targetInstanceIdx == instanceIdx)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CONNECT_TIMEOUT_ID);
			#if 0/* This might be excessive. */
				BtApp_GAP_CancelCurrentOperation();
			#endif
				
				TC_BT_BdAddrZero(&(pConnInst->targetBdAddr));
				pConnInst->targetProfile = 0;
				pConnInst->targetInstanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
				BtApp_GAP_SetConnStatus(BTAPP_GAP_CONN_STATUS_IDLE);
				connFlag = TRUE;
			}
		}
	}
	/* 1.1.b Check connProfList */
	pConnProf = BtApp_GAP_GetConnProfile(pBdAddr, profileUuid, instanceIdx);
	if(pConnProf != NULL)
	{
		pConnDev = pConnProf->pDevice;
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnDev->connProfList, (BtAppGapListElm_t *)pConnProf);
		if(pConnDev->connProfList.count == 0)
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pConnInst->connDevList, (BtAppGapListElm_t *)pConnDev);
			connFlag = TRUE;
		}
	}
	/* 1.2 Connect Cancellation Process */
	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pCcanInst->targetBdAddr), pBdAddr) == TRUE)
	{
		if(pCcanInst->targetProfile == profileUuid)
		{
			if(pCcanInst->targetInstanceIdx == instanceIdx)
			{
                TCCAPI_Timer_CancelTime(TIMER_BT_CANCEL_CONNECT_TIMEOUT_ID);
				BtApp_GAP_InitCcanInst();
				ccanFlag = TRUE;
			}
		}
	}
	/* 1.3 Disconnection Process */
	pDiscProf = BtApp_GAP_GetDiscProfileFromRTD((const TcBtBdAddr *)pBdAddr, profileUuid, instanceIdx);
	if(pDiscProf != NULL)
	{
		pDiscDev = pDiscProf->pDevice;
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->readyToDiscProfList, (BtAppGapListElm_t *)pDiscProf);
		removeDone = TRUE;
	}

	pDiscProf = BtApp_GAP_GetDiscProfileFromWFR((const TcBtBdAddr *)pBdAddr, profileUuid, instanceIdx);
	if(pDiscProf != NULL)
	{
		pDiscDev = pDiscProf->pDevice;
		BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscDev->waitForRespProfList, (BtAppGapListElm_t *)pDiscProf);
		removeDone = TRUE;
	}
	if(removeDone == TRUE)
	{
		if((pDiscDev->readyToDiscProfList.count == 0) && (pDiscDev->waitForRespProfList.count == 0)) /* QAC Msg(4:3353) */
		{
			BtApp_GAP_ListElementRemove((BtAppGapList_t *)&pDiscInst->discDevList, (BtAppGapListElm_t *)pDiscDev);
			discFlag = TRUE;

			if(pDiscInst->discDevList.count > 0)
			{
				if(pDiscInst->discAllFlag != TRUE)
				{
					resultDiscFlag = TRUE;
				}
			}
			else
			{
				if(pDiscInst->discAllFlag != TRUE)
				{
					resultDiscFlag = TRUE;
				}
				else
				{
					resultDiscAllFlag = TRUE;
				}
                TCCAPI_Timer_CancelTime(TIMER_BT_DISCONNECT_TIMEOUT_ID);
				BtApp_GAP_SetDiscStatus(BTAPP_GAP_DISC_STATUS_IDLE);
				pDiscInst->discAllFlag = FALSE;
			}
		}
	}

	/* 2. Check overallStatus & devConnStatus */
	/* If necessary, send the result to UI & Update mainConnStatusInst. */
	pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
	if(pDevConnStatus != NULL)
	{
		/* 2.1 Connection Process */
		if(connFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECTING) != 0)
			{
				if(BtApp_GAP_GetConnStatus() == BTAPP_GAP_CONN_STATUS_IDLE)
				{
					if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
					{
						if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
						{
#if 1
							printf("BtApp_GAP_ReflectDiscResult ConnectCfm\n");
#else
                            DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
					}

					/* resume connection */
					(void)BtApp_GAP_Connect();
				}
			}
		}
		/* Connect Cancellation Process */
		if(ccanFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_CONNECT_CANCELLING) != 0)
			{
				if((pDevConnStatus->ccanSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
				{
#if 1
					printf("BtApp_GAP_ReflectDiscResult CancelConnectCfm\n");
#else
                    DMP_BT_SYSEvtCancelConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECT_CANCELLING);
			}
		}
		/* Disconnection Process */
		if(discFlag == TRUE)
		{
			if((pDevConnStatus->status & BTAPP_GAP_STATUS_DISCONNECTING) != 0)
			{
				if((resultDiscFlag == TRUE) || (resultDiscAllFlag == TRUE))
				{
					if((pDevConnStatus->discSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
					{
						if(resultDiscFlag == TRUE)
						{
#if 1
							printf("BtApp_GAP_ReflectDiscResult DisconnectCfm\n");
#else
                            DMP_BT_SYSEvtDisconnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
						else
						{
#if 1
							printf("BtApp_GAP_ReflectDiscResult DisconnectAllCfm\n");
#else
                            DMP_BT_SYSEvtDisconnectAllCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
						}
					}
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_DISCONNECTING);
			}
		}
	}

        TcBtEvt_DMEvt(BT_DM_EVT_REFRESH,NULL);
	return ret;
}

void BtApp_GAP_ReflectActivateResult(TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapRecoveryListElm_t *pActvProf;
	BtAppGapProfUuidInstIdx searchData;

	searchData.profileUuid = profileUuid;
	searchData.instanceIdx = instanceIdx;
    pActvProf = (BtAppGapRecoveryListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, BtApp_GAP_RecoveryProfileFind, (const void *)&searchData);
	if(pActvProf != NULL)
	{
        TCCAPI_Timer_CancelTime(TIMER_BT_ACTIVATE_TIMEOUT_ID);
		
		switch(result)
		{
			case BTAPP_GAP_ACTV_RESULT_SUCCESS:
			{
				BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, (BtAppGapListElm_t *)pActvProf);
				BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->actResultSuccessList, (BtAppGapListElm_t *)pActvProf);
				break;
			}
			case BTAPP_GAP_ACTV_RESULT_FAILURE:
			{
				BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, (BtAppGapListElm_t *)pActvProf);
				BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->actResultFailureList, (BtAppGapListElm_t *)pActvProf);
				break;
			}
			default:
				break;
		}

		if(pRecoveryInst->waitingForActRespList.count == 0)
		{
			if(pRecoveryInst->actResultFailureList.count > 0)
			{
				pRecoveryInst->doOffOn = TRUE;
			}
			BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultSuccessList);
			BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList);
		}
	}
}

void BtApp_GAP_ReflectDeactivateResult(TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapRecoveryListElm_t *pDactProf;
	BtAppGapProfUuidInstIdx searchData;

	searchData.profileUuid = profileUuid;
	searchData.instanceIdx = instanceIdx;
    pDactProf = (BtAppGapRecoveryListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, BtApp_GAP_RecoveryProfileFind, (const void *)&searchData);
	if(pDactProf != NULL)
	{
        TCCAPI_Timer_CancelTime(TIMER_BT_DEACTIVATE_TIMEOUT_ID);
		
		switch(result)
		{
			case BTAPP_GAP_DACT_RESULT_SUCCESS:
			{
				BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, (BtAppGapListElm_t *)pDactProf);
				BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList, (BtAppGapListElm_t *)pDactProf);
				break;
			}
			case BTAPP_GAP_DACT_RESULT_FAILURE:
			{
				BtApp_GAP_ListElementRelease((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, (BtAppGapListElm_t *)pDactProf);
				BtApp_GAP_ListElementAddLast((BtAppGapList_t *)&pRecoveryInst->dactResultFailureList, (BtAppGapListElm_t *)pDactProf);
				break;
			}
			default:
				break;
		}

		if(pRecoveryInst->waitingForDactRespList.count == 0)
		{
			if(pRecoveryInst->dactResultFailureList.count > 0)
			{
				BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList);
				BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultFailureList);

				pRecoveryInst->doOffOn = TRUE;
			}
			else
			{
				BtApp_GAP_ActivateForRefreshing();
			}
		}
	}
}

TcBtInt32 BtApp_GAP_DisconnectionProcess(const TcBtBdAddr *pBdAddr)
{
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
#if defined(BT_SPP) || defined(BT_MAP_CLIENT)
	TcBtUint8 instanceIdx;
#endif
	TcBtBool added = FALSE;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;

#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
	state = BtApp_GAP_GetIapState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_SPP_IAP_SERVICE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_SPP
	for(instanceIdx = 0; instanceIdx <= SPP_MAX_IDX; instanceIdx++)
	{
		state = BtApp_GAP_GetSppState(pBdAddr, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_SPP_PROFILE_UUID;
			discProf.instanceIdx = instanceIdx;
			if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_MAP_CLIENT
	for(instanceIdx = 0; instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST; instanceIdx++)
	{
		state = BtApp_GAP_GetMapcState(pBdAddr, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
			discProf.instanceIdx = instanceIdx;
			if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_PBAP_CLIENT
	state = BtApp_GAP_GetPacState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_AUDIO_SINK
#ifndef USE_BTBR
	state = BtApp_GAP_GetAvrcpState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
	state = BtApp_GAP_GetA2dpState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
#if 0
		(void)BtApp_AV_SNK_CloseStream();
#endif
		discProf.profileUuid = TC_BT_AUDIO_SINK_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_HANDSFREE_UNIT
	state = BtApp_GAP_GetHfpState(pBdAddr, BTAPP_GAP_UNUSED_INST_IDX);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_HF_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		discProf.timerSettingVal = 2;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_HIDD
	state = BtApp_GAP_GetHiddState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_HID_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_PAN
    state = BtApp_GAP_GetPANState(pBdAddr);
    if(state == BTAPP_GAP_STATE_CONNECTED)
    {
        discProf.profileUuid = TC_BT_PAN_PANU_PROFILE_UUID;
        discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
        if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
        {
            added = TRUE;
        }
    }
#endif

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_DISCONNECTING,
			BTAPP_GAP_SEND_CONN_RESULT|BTAPP_GAP_CONN_SIDE_LOCAL);
	}

	return BtApp_GAP_Disconnect();
}

TcBtInt32 BtApp_GAP_DisconnectionAllProcess(void)
{
	BtAppGapDiscInstance_t *pDiscInst = BtApp_GAP_GetDiscInst();
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
	TcBtBdAddr bdAddr;
#if defined(BT_SPP) || defined(BT_MAP_CLIENT) || defined(BT_HANDSFREE_UNIT)
	TcBtUint8 instanceIdx;
#endif
	TcBtBool added = FALSE;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;

#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
	state = BtApp_GAP_GetIapState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_SPP_IAP_SERVICE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		(void)BtApp_GAP_GetIapBdAddr(&bdAddr);
		if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_SPP
	for(instanceIdx = 0; instanceIdx <= SPP_MAX_IDX; instanceIdx++)
	{
		state = BtApp_GAP_GetSppState(NULL, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_SPP_PROFILE_UUID;
			discProf.instanceIdx = instanceIdx;
			(void)BtApp_GAP_GetSppBdAddrByIdx(instanceIdx, &bdAddr);
			if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_MAP_CLIENT
	for(instanceIdx = 0; instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST; instanceIdx++)
	{
		state = BtApp_GAP_GetMapcState(NULL, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
			discProf.instanceIdx = instanceIdx;
			(void)BtApp_GAP_GetMapcBdAddrByIdx(instanceIdx, &bdAddr);
			if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_PBAP_CLIENT
	state = BtApp_GAP_GetPacState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		(void)BtApp_GAP_GetPacBdAddr(&bdAddr);
		if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_AUDIO_SINK
#ifndef USE_BTBR
	state = BtApp_GAP_GetAvrcpState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		(void)BtApp_GAP_GetAvrcpBdAddr(&bdAddr);
		if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
	state = BtApp_GAP_GetA2dpState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
#if 0
		(void)BtApp_AV_SNK_CloseStream();
#endif
		discProf.profileUuid = TC_BT_AUDIO_SINK_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		(void)BtApp_GAP_GetA2dpBdAddr(&bdAddr);
		if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_HANDSFREE_UNIT
	for(instanceIdx = 0; instanceIdx < BTAPP_GAP_HFP_MAX_CONN_INST; instanceIdx++)
	{
		state = BtApp_GAP_GetHfpState(NULL, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_HF_PROFILE_UUID;
			discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
			//discProf.timerSettingVal = 2;
			(void)BtApp_GAP_GetHfpBdAddrByIdx(instanceIdx, &bdAddr);
			if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				added = TRUE;
			}
		}
	}
#endif
#ifdef BT_HIDD
	state = BtApp_GAP_GetHiddState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_HID_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		(void)BtApp_GAP_GetHiddBdAddr(&bdAddr);
		if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}
#endif
#ifdef BT_PAN
    state = BtApp_GAP_GetPANState(NULL);
    if(state == BTAPP_GAP_STATE_CONNECTED)
    {
        discProf.profileUuid = TC_BT_PAN_PANU_PROFILE_UUID;
        discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
        (void)BtApp_GAP_GetPANBdAddr(&bdAddr);
        if(BtApp_GAP_AddToDiscProfList(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
        {
            added = TRUE;
        }
    }
#endif

	if(added == TRUE)
	{
		BtAppGapDiscDevListElm_t *pDiscDev;
		for(pDiscDev = pDiscInst->discDevList.first; pDiscDev != NULL; pDiscDev = pDiscDev->next)
		{
			BtApp_GAP_SetMainConnStatus(
				(const TcBtBdAddr *)&(pDiscDev->bdAddr),
				BTAPP_GAP_STATUS_DISCONNECTING,
				BTAPP_GAP_SEND_CONN_RESULT|BTAPP_GAP_CONN_SIDE_LOCAL);
		}
		pDiscInst->discAllFlag = TRUE;
	}

	return BtApp_GAP_Disconnect();
}

void BtApp_GAP_AvrcpSubDisconnectionProcess(const TcBtBdAddr *pBdAddr)
{
#ifndef USE_BTBR
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
	TcBtBool connectCancelling = FALSE;
	TcBtBool disconnecting = FALSE;
	TcBtBool removeDone = FALSE;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;

	/* Remove from the connProfList */
	if(BtApp_GAP_RemoveFromConnProfList(pBdAddr, TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_UNUSED_INST_IDX) == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		removeDone = TRUE;
	}

	/* Cancel Connect */
	if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
	{
		if(pConnInst->targetProfile == TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID)
		{
			if(BtApp_GAP_CancelCurrentOperation() == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				connectCancelling = TRUE;
			}
		}
	}

	/* Disconnect */
	state = BtApp_GAP_GetAvrcpState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			disconnecting = TRUE;
		}
	}
	
	if(connectCancelling == TRUE)
	{
		/* Check status in order to distinguish timer cancellation case.
		In this case we don't need to change the mainConnStatus */
		if(BtApp_GAP_GetCcanStatus() == BTAPP_GAP_CCAN_STATUS_CANCELLING)
		{
			BtApp_GAP_SetMainConnStatus(
				pBdAddr,
				BTAPP_GAP_STATUS_CONNECT_CANCELLING,
				BTAPP_GAP_CONN_SIDE_REMOTE);
		}

		(void)BtApp_GAP_Connect();
	}
	if(disconnecting == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_DISCONNECTING,
			BTAPP_GAP_CONN_SIDE_REMOTE);

		(void)BtApp_GAP_Disconnect();
	}

	if(removeDone == TRUE)
	{
		if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
		{
			if(TC_BT_BdAddrEqZero(&pConnInst->targetBdAddr) == TRUE)
			{
				BtAppGapDevConnStatusListElm_t *pDevConnStatus;
				pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
				if(pDevConnStatus != NULL)
				{
					if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
					{
#if 1
                        printf("BtApp_GAP_AvrcpSubDisconnectionProcess ConnectCfm\n");
#else
                        DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
					}
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
			}
		}
	}
#endif
}

void BtApp_GAP_ObexSubDisconnectionProcess(const TcBtBdAddr *pBdAddr)
{
	BtAppGapConnInstance_t *pConnInst = BtApp_GAP_GetConnInst();
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
	TcBtBool disconnecting = FALSE;
	TcBtBool connectCancelling = FALSE;
	TcBtBool removeDone = FALSE;
	TcBtUint32 i;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;

	/* for PAC */
	/* Remove from connProfList */
	if(BtApp_GAP_RemoveFromConnProfList(pBdAddr, TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID, BTAPP_GAP_UNUSED_INST_IDX) == BTAPP_GAP_FN_RESULT_SUCCESS)
	{
		removeDone = TRUE;
	}
	
	/* Cancel Connect */
	if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
	{
		if(pConnInst->targetProfile == TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID)
		{
			if(BtApp_GAP_CancelCurrentOperation() == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				connectCancelling = TRUE;
			}
		}
	}

	/* Disconnect */
	state = BtApp_GAP_GetPacState(pBdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			disconnecting = TRUE;
		}
	}
 
	/* for MAPC */
	/* Cancel Connect */
	if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) == TRUE)
	{
		if(pConnInst->targetProfile == TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID)
		{
			if(BtApp_GAP_CancelCurrentOperation() == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				connectCancelling = TRUE;
			}
		}
	}
	for(i = 0; i < BTAPP_GAP_MAPC_MAX_CONN_INST; i++)
	{
		/* Remove from connProfList */
		if(BtApp_GAP_RemoveFromConnProfList(pBdAddr, TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID, i) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			removeDone = TRUE;
		}

		/* Disconnect */
		state = BtApp_GAP_GetMapcState(pBdAddr, i);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
			discProf.instanceIdx = i;
			if(BtApp_GAP_AddToDiscProfList(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
			{
				disconnecting = TRUE;
			}
		}
	}
	
	if(connectCancelling == TRUE)
	{
		/* To distinguish timer cancellation case. In this case we don't need to change the mainConnStatus */
		if(BtApp_GAP_GetCcanStatus() == BTAPP_GAP_CCAN_STATUS_CANCELLING)
		{
			BtApp_GAP_SetMainConnStatus(
				pBdAddr,
				BTAPP_GAP_STATUS_CONNECT_CANCELLING,
				BTAPP_GAP_CONN_SIDE_REMOTE);
		}

		(void)BtApp_GAP_Connect();
	}
	if(disconnecting == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			pBdAddr,
			BTAPP_GAP_STATUS_DISCONNECTING,
			BTAPP_GAP_CONN_SIDE_REMOTE);

		(void)BtApp_GAP_Disconnect();
	}

	if(removeDone == TRUE)
	{
		if(BtApp_GAP_ListSearch((BtAppGapList_t *)&pConnInst->connDevList, BtApp_GAP_ConnDeviceFind, (const void *)pBdAddr) == NULL)
		{
			if(TC_BT_BdAddrEq(&pConnInst->targetBdAddr, pBdAddr) != TRUE)
			{
				BtAppGapDevConnStatusListElm_t *pDevConnStatus;
				pDevConnStatus = BtApp_GAP_GetDevConnStatus(pBdAddr);
				if(pDevConnStatus != NULL)
				{
					if((pDevConnStatus->connSide & BTAPP_GAP_SEND_CONN_RESULT) != 0)
					{
#if 1
                        printf("BtApp_GAP_ObexSubDisconnectionProcess ConnectCfm\n");
#else
                        DMP_BT_SYSEvtConnectCfm(BTAPP_SYS_RESULT_DONE, 0);
#endif
					}
				}
				BtApp_GAP_UnsetMainConnStatus(pBdAddr, BTAPP_GAP_STATUS_CONNECTING);
			}
		}
	}
}

void BtApp_GAP_PacDisconnect(const TcBtBdAddr *pBdAddr)
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
	TcBtBool added = FALSE;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;

	if(pBdAddr != NULL)
	{
		TC_BT_BdAddrCopyIfExist(&bdAddr, pBdAddr);
	}
	else
	{
		(void)BtApp_GAP_GetPacBdAddr(&bdAddr);
	}
	
	state = BtApp_GAP_GetPacState(&bdAddr);
	if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		discProf.profileUuid = TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID;
		discProf.instanceIdx = BTAPP_GAP_UNUSED_INST_IDX;
		if(BtApp_GAP_AddToDiscProfListFirst(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
		{
			added = TRUE;
		}
	}

	if(added == TRUE)
	{
		BtApp_GAP_SetMainConnStatus(
			&bdAddr,
			BTAPP_GAP_STATUS_DISCONNECTING,
			BTAPP_GAP_CONN_SIDE_LOCAL);
	}

	(void)BtApp_GAP_Disconnect();
}

void BtApp_GAP_MapcDisconnect(const TcBtBdAddr *pBdAddr, const TcBtUint16 side)
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
	BtAppGapDiscProfListElm_t discProf;
	TcBtUint8 instanceIdx;
	TcBtBool added = FALSE;

	discProf.next = NULL;
	discProf.prev = NULL;
	discProf.pDevice = NULL;
	discProf.timerSettingVal = 1;
	discProf.tryCount = 1;
	
	for(instanceIdx = 0; instanceIdx < BTAPP_GAP_MAPC_MAX_CONN_INST; instanceIdx++)
	{
		state = BtApp_GAP_GetMapcState(pBdAddr, instanceIdx);
		if(state == BTAPP_GAP_STATE_CONNECTED)
		{
			discProf.profileUuid = TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID;
			discProf.instanceIdx = instanceIdx;
			if(pBdAddr != NULL)
			{
				if(BtApp_GAP_AddToDiscProfListFirst(pBdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
				{
					added = TRUE;
				}
			}
			else
			{
				(void)BtApp_GAP_GetMapcBdAddrByIdx(instanceIdx, &bdAddr);
				if(BtApp_GAP_AddToDiscProfListFirst(&bdAddr, &discProf) == BTAPP_GAP_FN_RESULT_SUCCESS)
				{
					added = TRUE;
				}
			}
		}
	}

	if(added == TRUE)
	{
		if(pBdAddr != NULL)
		{
			BtApp_GAP_SetMainConnStatus(
				pBdAddr,
				BTAPP_GAP_STATUS_DISCONNECTING,
				side);
		}
		else
		{
			BtApp_GAP_SetMainConnStatus(
				&bdAddr,
				BTAPP_GAP_STATUS_DISCONNECTING,
				side); /* QAC Msg(4:3354) */
		}
	}

	(void)BtApp_GAP_Disconnect();
}

#ifdef ______________________________________________________________________________
#endif
void BtApp_GAP_InitRecoveryInst(void)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList, NULL);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList, NULL);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->dactResultFailureList, NULL);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, NULL);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->actResultSuccessList, NULL);
	BtApp_GAP_ListInit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList, NULL);
	pRecoveryInst->doOffOn = FALSE;
}

static void BtApp_GAP_DeactivateTimeoutHandler(TcBtUint8 id, TcBtUint8 loc)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	
	/* clean up recovery list */
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultFailureList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList);

	/* set bt reset flag */
	pRecoveryInst->doOffOn = TRUE;
}

static void BtApp_GAP_ActivateTimeoutHandler(TcBtUint8 id, TcBtUint8 loc)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	
	/* clean up recovery list */
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->waitingForDactRespList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultSuccessList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->dactResultFailureList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList);
	BtApp_GAP_ListDeinit((BtAppGapList_t *)&pRecoveryInst->actResultFailureList);

	/* set bt reset flag */
	pRecoveryInst->doOffOn = TRUE;
}

void BtApp_GAP_SetBtOffOnFlag(TcBtBool flag)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	
	pRecoveryInst->doOffOn = flag;
}

TcBtBool BtApp_GAP_GetBtOffOnFlag(void)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	
	return pRecoveryInst->doOffOn;
}

void BtApp_GAP_CheckBtOffOn(void)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	if(pRecoveryInst->doOffOn == TRUE)
	{
		if(BtApp_GAP_GetAclConnCnt() == 0)
		{
			pRecoveryInst->doOffOn = FALSE;
		}
	}
}

TcBtBool BtApp_GAP_IsInRecoveryList(TcBtUint16 profileUuid)
{
	BtAppGapRecoveryInstance_t *pRecoveryInst = BtApp_GAP_GetRecoveryInst();
	BtAppGapRecoveryListElm_t *pRecoveryProf;
	TcBtBool ret = FALSE;

	pRecoveryProf = (BtAppGapRecoveryListElm_t *)BtApp_GAP_ListSearch((BtAppGapList_t *)&pRecoveryInst->waitingForActRespList, BtApp_GAP_RecoveryProfileFind, &profileUuid);
	if(pRecoveryProf != NULL)
	{
		ret = TRUE;
	}

	return ret;
}

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
								FOR DISCONNECT
**************************************************************************/
#ifdef BT_AUDIO_SINK
#if 0
void BtApp_GAP_A2DP_StopStreamForTimer(unsigned char id, unsigned char loc)
{
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_OPEN) == TRUE)
	{
		(void)TC_BT_A2DP_StopStream();
	}
}
#endif
void BtApp_GAP_A2DP_DisconnectForTimer(unsigned char id, unsigned char loc)
{
    TC_BT_CM_ServiceStatus eServiceStatus = (TC_BT_CM_ServiceStatus)TC_BT_CM_GetA2DPStatus(NULL);
	unsigned int TimerCnt = 0;

#if 1
	if(eServiceStatus >= TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_A2DP_Disconnect();
	}
#else
	else if(eServiceStatus == TC_BT_CM_EXTRA)
	{
		/* suspend stream */
		if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_STREAM) == TRUE)
		{
			(void)TC_BT_A2DP_SuspendStream();
			TimerCnt += 5;
		}

		/* stop stream */
		if(TimerCnt == 0)
		{
			(void)TC_BT_A2DP_StopStream();
		}
		else
		{
			TCCAPI_Timer_SetTime(TimerCnt, TIMER_BT_AV_STREAM_STOP_ID, BtApp_GAP_A2DP_StopStreamForTimer);
		}
		TimerCnt += 10;

		/* re-try disconnection */
		TCCAPI_Timer_SetTime(TimerCnt, TIMER_BT_DISCONNECT_ID5, BtApp_GAP_A2DP_DisconnectForTimer);
	}
	else
	{
	}	
#endif
}
#endif

int BtApp_GAP_DisconnectAllAtBTDeactivate(void)
{
//#ifdef BT_HANDSFREE_UNIT
	TC_BT_CM_ServiceStatus eServiceStatus = TC_BT_CM_UNKNOWNSTATUS;
//#endif
	unsigned int		TimerCnt = 0,
						i;
	
	(void)BtApp_GAP_CancelConnect((const TcBtBdAddr *)NULL);


#ifdef BT_HANDSFREE_UNIT
	for(i = 0; i < BT_HFP_MAX_CONNECTIONS; i++)
	{
        TcBtBdAddr *pBtAddrTemp = TC_BT_HF_DEV_GetBdAddr(i);

		if((pBtAddrTemp != NULL))
		{
			if(TC_BT_BdAddrEqZero((const TcBtBdAddr *)pBtAddrTemp) == FALSE)
			{
                eServiceStatus = (TC_BT_CM_ServiceStatus)TC_BT_CM_GetHFStatus(pBtAddrTemp);
				if(eServiceStatus == TC_BT_CM_CONNECTING)
				{
                    (void)G_BtHfCmd->TC_BT_HF_CancelConnect(pBtAddrTemp);
						
					TimerCnt++;
				}
				else if(eServiceStatus == TC_BT_CM_CONNECTED)
				{
                    (void)G_BtHfCmd->TC_BT_HF_Disconnect(pBtAddrTemp);

					TimerCnt++;
				}
				else
				{
				}
			}
		}
	}
#endif

#ifdef BT_AUDIO_SINK
    eServiceStatus = (TC_BT_CM_ServiceStatus)TC_BT_CM_GetA2DPStatus(NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTING)
	{
		(void)TC_BT_A2DP_CancelConnect((const TcBtBdAddr *)NULL);	// TC_BT_EX_A2DPCancelConnect();
		TimerCnt++;
	}
#if 1
	else if(eServiceStatus >= TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_A2DP_Disconnect();
		TimerCnt++;
	}
#else
	else if(eServiceStatus == TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_A2DP_Disconnect();
		TimerCnt++;
	}
	else if(eServiceStatus == TC_BT_CM_EXTRA)
	{
		int innerTimerCnt = 0;

		/* suspend stream */
		if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_STREAM) == TRUE)
		{
			(void)TC_BT_A2DP_SuspendStream();
			innerTimerCnt++;
			TimerCnt++;
		}

		/* stop stream */
		if(innerTimerCnt == 0)
		{
			(void)TC_BT_A2DP_StopStream();
		}
		else
		{
			TCCAPI_Timer_SetTime(innerTimerCnt, TIMER_BT_DISCONNECT_ID1, BtApp_GAP_A2DP_StopStreamForTimer);
		}
		innerTimerCnt += 2;
		TimerCnt++;

		/* disconnect */
		TCCAPI_Timer_SetTime(innerTimerCnt, TIMER_BT_DISCONNECT_ID2, BtApp_GAP_A2DP_DisconnectForTimer);
		TimerCnt++;
	}
	else
	{
	}
#endif
    eServiceStatus = (TC_BT_CM_ServiceStatus)TC_BT_CM_GetAVRCPStatus(NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTING)
	{
		(void)TC_BT_AVRCP_CancelConnect((const TcBtBdAddr *)NULL);
		TimerCnt++;
	}
	else if(eServiceStatus == TC_BT_CM_CONNECTED)
	{

		(void)TC_BT_AVRCP_Disconnect();
		TimerCnt++;
	}
	else
	{
	}
#endif
#ifdef BT_PBAP_CLIENT
    eServiceStatus = (TC_BT_CM_ServiceStatus)TC_BT_CM_GetPACStatus(NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTING)
	{
        (void)BtApp_PAC_CancelConnectReq();

		TimerCnt++;
	}
	else if(eServiceStatus == TC_BT_CM_CONNECTED)
	{
        (void)BtApp_PAC_DisconnectReq();
		TimerCnt++;
	}
	else
	{
	}
#endif
#ifdef BT_SPP
	eServiceStatus = TC_BT_CM_GetSPPStatus(0, NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_SPP_Disconnect(0);
		TimerCnt++;
	}
#endif
#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
	eServiceStatus = TC_BT_CM_GetSPPIAPStatus(NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_SPP_IAP_Disconnect();
		TimerCnt++;
	}
#endif
#ifdef BT_HIDD
	eServiceStatus = TC_BT_CM_GetHIDDStatus(NULL);
	if(eServiceStatus == TC_BT_CM_CONNECTED)
	{
		(void)TC_BT_HIDD_Disconnect();
		TimerCnt++;
	}
#endif
#ifdef BT_PAN
    // PANTODO At this time, We will use only one connection for PAN
    eServiceStatus = TC_BT_CM_GetPANStatus(0);
    if(eServiceStatus == TC_BT_CM_CONNECTED)
    {
        // PANTODO At this time, We will use only one connection for PAN
        (void)TC_BT_PAN_Disconnect(0);
        TimerCnt++;
    }
    else
    {
	    TC_BT_PAN_Deactivate();
	}
#endif

	if(TimerCnt > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
								FOR ACL CONNECT INDICATION
**************************************************************************/
static TcBtBdAddr	gBtAppGapAclConnList[SC_DB_MAX_NUM_OF_RECORD_INFO];
static TcBtUint32	gBtAppGapAclConnCnt = 0;

void BtApp_GAP_InitAclConnList(void)
{
	memset(gBtAppGapAclConnList, 0x00, sizeof(gBtAppGapAclConnList));
	gBtAppGapAclConnCnt = 0;
}

int BtApp_GAP_GetAclConnCnt(void)
{
	return gBtAppGapAclConnCnt;
}

int BtApp_GAP_IsInAclConnList(const TcBtBdAddr *pBdAddr)
{
	int i;
	int ret = FALSE;

	if(pBdAddr != NULL)
	{
		for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
		{
			if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(gBtAppGapAclConnList[i]), pBdAddr) != 0)
			{
				ret = TRUE;
				break;
			}
		}
	}

	return ret;
}

int BtApp_GAP_AddToAclConnList(const TcBtBdAddr *pBdAddr)
{
	int i;
	int ret = -1; /* 1:already exist,0:success,-1:invalid param */
	int found = FALSE;
	int emptyIndex = -1;

	if(pBdAddr != NULL)
	{
		/* find in the list */
		for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
		{
			if(TC_BT_BdAddrEqZero(&(gBtAppGapAclConnList[i])) != 0)
			{/* store an empty place's index */
				emptyIndex = i;
			}
			if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(gBtAppGapAclConnList[i]), pBdAddr) != 0)
			{/* already exist */
				found = TRUE;
				ret = 1;
				break;
			}
		}

		/* if not exist in the list and there are an empty place */
		if((found == FALSE) && (emptyIndex >= 0))
		{/* insert at the empty place */
			gBtAppGapAclConnList[emptyIndex].lap = pBdAddr->lap;
			gBtAppGapAclConnList[emptyIndex].uap = pBdAddr->uap;
			gBtAppGapAclConnList[emptyIndex].nap = pBdAddr->nap;
			gBtAppGapAclConnCnt++;
			ret = 0;
		}
	}

	return ret;
}

int BtApp_GAP_RemoveFromAclConnList(const TcBtBdAddr *pBdAddr)
{
	int i;
	int ret = -1; /* 1:not exist,0:success,-1:invalid param */

	if(pBdAddr != NULL)
	{
		for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
		{
			if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(gBtAppGapAclConnList[i]), pBdAddr) != 0)
			{
				memset(&gBtAppGapAclConnList[i], 0x00, sizeof(TcBtBdAddr));
				gBtAppGapAclConnCnt--;
				ret = 0;
				break;
			}
		}
		if(i == SC_DB_MAX_NUM_OF_RECORD_INFO)
		{/* not exist */
			ret = 1;
		}
	}

	return ret;
}

#ifdef ______________________________________________________________________________
#endif
/**************************************************************************
								FOR AUTOCONNECT
**************************************************************************/

#ifdef BT_AUTO_CONNECTION
#define AUTO_CONNECTION_PERIOD	(10*2)	/* About 10sec */
static TcBtUint8 gBtAppGap_RecoveryLinkLostFlag = 0;
static TcBtUint32 gBtAppGap_RecoveryLinkLostCnt = 0;

void BtApp_GAP_AutoConnect(void)
{
	TcBtBdAddr *pAddr = TC_BT_SC_DB_ReadLastDevAddr();

	if((pAddr != NULL) && (TC_BT_BdAddrEqZero((const TcBtBdAddr *)pAddr)==FALSE))
	{
		(void)BtApp_GAP_FullConnectionProcess((const TcBtBdAddr *)pAddr, BTAPP_GAP_CONN_SIDE_LOCAL);
	}
}

void BtApp_GAP_SetRecoveryLinkLostFlag(TcBtUint8 Flag)
{
	gBtAppGap_RecoveryLinkLostFlag = Flag;
	gBtAppGap_RecoveryLinkLostCnt = AUTO_CONNECTION_PERIOD - (1*2);	/* Connect after 1sec for the first time */
	
}

TcBtUint8 BtApp_GAP_GetRecoveryLinkLostFlag(void)
{
	return gBtAppGap_RecoveryLinkLostFlag;
}

void BtApp_GAP_RecoveryLinkLostCancelTimer(void)
{
	TCCAPI_Timer_CancelTime(TIMER_BT_AUTOCONNECT_CANCEL_ID);
}

void BtApp_GAP_RecoveryLinkLostCancelChkProximity(void)
{
	TcBtBdAddr	*pbdAddr = TC_BT_SC_DB_ReadLastDevAddr();

	if((pbdAddr != NULL) && (TC_BT_BdAddrEqZero((const TcBtBdAddr *)pbdAddr)==FALSE))
	{
		(void)TC_BT_CM_CancelReadRemoteName(*pbdAddr);
		BtApp_GAP_RecoveryLinkLostCancelTimer();
	}
}

void BtApp_GAP_RecoveryLinkLostCancelChkProximityForTimer(unsigned char id, unsigned char loc)
{
	BtApp_GAP_RecoveryLinkLostCancelChkProximity();
}

void BtApp_GAP_RecoveryLinkLostChkProximity(void)
{
	TcBtBdAddr	*pbdAddr = TC_BT_SC_DB_ReadLastDevAddr();

	if((pbdAddr != NULL) && (TC_BT_BdAddrEqZero((const TcBtBdAddr *)pbdAddr)==FALSE))
	{
		if(gSD_SearchFlag != TRUE)
		{
			(void)TC_BT_CM_ReadRemoteName(*pbdAddr);
			TCCAPI_Timer_SetTime(50, TIMER_BT_AUTOCONNECT_CANCEL_ID, BtApp_GAP_RecoveryLinkLostCancelChkProximityForTimer);
		}
	}
	else
	{
		BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_OUT);
	}
}

void BtApp_GAP_RecoveryLinkLostEvent(void)
{
	if(BtApp_GAP_GetRecoveryLinkLostFlag() == BTAPP_GAP_RECOVERY_LINK_LOST_IN)
	{
        if(G_BtSyscmd->TC_BT_SYS_GetBTStatus() != TC_BT_SYS_Activated)
        {
			BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_OUT);
			gBtAppGap_RecoveryLinkLostCnt = 0;
		}
        usleep(1);
		gBtAppGap_RecoveryLinkLostCnt++;
		if(gBtAppGap_RecoveryLinkLostCnt > AUTO_CONNECTION_PERIOD)
        {
			BtApp_GAP_RecoveryLinkLostChkProximity();
			gBtAppGap_RecoveryLinkLostCnt = 0;
		}
	}
}

void BtApp_GAP_RecoveryLinkLostStop(void)
{
	BtApp_GAP_RecoveryLinkLostCancelChkProximity();
	BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_OUT);
}
#endif

void BtApp_GAP_SetAuthoriseFlag(TcBtBool	bAuthorFlag)
{
	bBtAppGapAuthoriseFlag = bAuthorFlag;
}

TcBtBool BtApp_GAP_GetAuthoriseFlag(void)
{
	return bBtAppGapAuthoriseFlag;
}
