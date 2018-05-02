/****************************************************************************
 *   FileName    : TC_MSG_BT_PBEvt.c
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
*  Description : This file contains functions and variables for a Bluetooth AV application
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*   ---------     --------       -----------------           -----------------
*    09/07/23     0.1            created                     Hwang, Jin Ho.
*******************************************************************************/

#include "BT_CFG.h"

#if defined(BT_PBAP_CLIENT) 

//#include <stdio.h>

/*****************************************************************************
*
* Header File Include
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <alsa/asoundlib.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "BtPhoneBookEvt.h"
#include "BtPhoneBook.h"
#include "BtSys.h"
#include "TcBtEvt.h"
#include "TC_BT/inc/BtPlatform.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#ifndef PBAP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#ifndef NULL
#define NULL 0
#endif

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

/*****************************************************************************
*
* static variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */


/*****************************************************************************
*
* global variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* debug
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* PBAP event */
#ifdef ______________________________________________________________________________ 
#endif

/* PBAP event */
#ifdef ______________________________________________________________________________ 
#endif
void BtApp_PB_EvtPACConnectCfm(void)
{
#ifdef BT_PBAP_MULTISERVER
	int devNum;
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACConnectCfm\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));
        	
        	PBInst->deviceCnt++;
        	PBInst->service = TC_BT_OBEX_PBA_SERVER_PROFILE_UUID;
        	devNum = BtApp_PB_SetDeviceId(BT_CONTACTS_EVT_CONNECT_CFM);

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = devNum;
        	contactsEvtInfo->data2 = PBInst->vCardObjNum[PBInst->deviceId];;

        	BtApp_SYS_SetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT));

        	//DMP_EvtBTInfoMessageSend(MSG_SRC_BT_PBAP, BT_CONTACTS_EVT_CONNECT_CFM, (unsigned char *)&contactsEvtInfo, sizeof(BT_CONTACTS_EVENT_INFO));
        	TcBtEvt_PBEvt(BT_PB_EVT_CONNECT_CFM, contactsEvtInfo);
        }
#else	
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACConnectCfm\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

        	PBInst->deviceCnt++;
        	PBInst->service = TC_BT_OBEX_PBA_SERVER_PROFILE_UUID;

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = NULL;
        	contactsEvtInfo->data2 = NULL;
           TcBtEvt_PBEvt(BT_PB_EVT_CONNECT_CFM, contactsEvtInfo);
            (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_PB_PAC_CONNECT);

        }
#endif
}

void BtApp_PB_EvtPACConnectErr(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACConnectErr\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));
        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = NULL;
        	contactsEvtInfo->data2 = NULL;

        	BtApp_PB_PacConnectErrHandler();
            TcBtEvt_PBEvt(BT_PB_EVT_CONNECT_ERR, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACDisconnectInd(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACDisconnectInd\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));
        	contactsEvtInfo->service = TC_BT_OBEX_PBA_SERVER_PROFILE_UUID;
        	contactsEvtInfo->data1 = NULL;
        	contactsEvtInfo->data2 = NULL;

#ifdef BT_PBAP_MULTISERVER
        	BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT));\
        	
        	//DMP_EvtBTInfoMessageSend(MSG_SRC_BT_PBAP, BT_CONTACTS_EVT_DISCONNECT_IND, (unsigned char *)&contactsEvtInfo, sizeof(BT_CONTACTS_EVENT_INFO));
        	TcBtEvt_PBEvt(BT_PB_EVT_DISCONNECT_IND, contactsEvtInfo);
#else
            if((G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT)) == TRUE)
        	{
                (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_PB_PAC_CONNECT);
        	}
            TcBtEvt_PBEvt(BT_PB_EVT_DISCONNECT_IND, contactsEvtInfo);

#endif
        }
}

void BtApp_PB_EvtPACPullPBInd(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullPBInd\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

#ifdef BT_PBAP_MULTISERVER
        	BtApp_SYS_SetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD));
#endif

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->vCardObjNum[PBInst->deviceId];
        	contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLPB_IND, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACPullPBCfm(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullPBCfm\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

#ifdef BT_PBAP_MULTISERVER
        	BtApp_SYS_SetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_DOWNLOADED));
#endif

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->vCardObjNum[PBInst->deviceId];
        	contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLPB_CFM, contactsEvtInfo);
        }
}

void BtApp_PB_EvtPACPullCHInd(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	unsigned char callHistoryFolder = select_callhistory[BTAPP_PB_STORAGE];
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullCHInd\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->CallHistory[callHistoryFolder][0].vCardNum;
        	contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLCH_IND, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACPullCHCfm(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	unsigned char callHistoryFolder = select_callhistory[BTAPP_PB_STORAGE];
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullCHCfm\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->CallHistory[callHistoryFolder][0].vCardNum;
        	contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLCH_CFM, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACPullPBErr(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
        	
        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullPBErr\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

#ifdef BT_PBAP_MULTISERVER
        	BtApp_SYS_SetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_DOWNLOADED));
#endif

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->vCardObjNum[PBInst->deviceId];
            contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLPB_ERR, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACPullCHErr(void)
{
	BT_CONTACTS_EVENT_INFO *contactsEvtInfo = (BT_CONTACTS_EVENT_INFO *)TC_BT_Pmalloc(sizeof(BT_CONTACTS_EVENT_INFO));
       if(contactsEvtInfo != NULL)
       {
        	unsigned char callHistoryFolder = select_callhistory[BTAPP_PB_STORAGE];
        	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
        	
        	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullCHErr\n",__FILENAME__,__LINE__);

        	memset(contactsEvtInfo, 0x00, sizeof(BT_CONTACTS_EVENT_INFO));

        	contactsEvtInfo->service = PBInst->service;
        	contactsEvtInfo->data1 = PBInst->CallHistory[callHistoryFolder][0].vCardNum;
        	contactsEvtInfo->data2 = NULL;
            TcBtEvt_PBEvt(BT_PB_EVT_PULLCH_ERR, contactsEvtInfo);

        }
}

void BtApp_PB_EvtPACAbortCfm(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACAbortCfm\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_ABORT_CFM, NULL);

}

void BtApp_PB_EvtPACSelectSortingCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedSorting();
       }
       TcBtEvt_PBEvt(BT_PB_EVT_PAC_SELECTSORTING_CFM, info);

#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSelectSortingCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedSorting();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_PAC_SELECTSORTING_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtPACSelectFeaturesCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedFeatures();
            TcBtEvt_PBEvt(BT_PB_EVT_PAC_SELECTFEATURES_CFM, info);
       }
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSelectFeaturesCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedFeatures();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_PAC_SELECTFEATURES_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtPACSetFilterCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedVcardFilter();
           TcBtEvt_PBEvt(BT_PB_EVT_PAC_SETFILTER_CFM, info);

       }
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSetFilterCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedVcardFilter();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_PAC_SETFILTER_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtPACSelectSourceCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedSource();
           TcBtEvt_PBEvt(BT_PB_EVT_PAC_SELECTSOURCE_CFM, info);

       }
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSelectSourceCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedSource();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_PAC_SELECTSOURCE_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtPACSetFolderCfm(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSetFolderCfm\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_SETFOLDER_CFM, NULL);

}

void BtApp_PB_EvtPACSetFolderErr(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSetFolderErr\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_SETFOLDER_ERR, NULL);

}

void BtApp_PB_EvtPACSetAuthenticationCfm(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACSetAuthenticationCfm\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_SETAUTHENTICATION_CFM, NULL);

}

void BtApp_PB_EvtPACPullVcardListCfm(void)
{
	PHONEBOOK_LIST_TYPE *PBList = BtApp_PB_GetPhonebookList();

       unsigned int *info = (unsigned int *)TC_BT_Pmalloc(sizeof(unsigned int));
       if(info != NULL)
       {
            *info = PBList->numOfHandle;
           TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLVCARDLIST_CFM, info);

       }

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullVcardListCfm\n",__FILENAME__,__LINE__);

	//DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_PAC_PULLVCARDLIST_CFM, (unsigned char *)&PBList->numOfHandle, sizeof(unsigned int));	
}

void BtApp_PB_EvtPACPullVcardListErr(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullVcardListErr\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLVCARDLIST_ERR, NULL);

}

void BtApp_PB_EvtPACPullVcardEntryCfm(void)
{
	BT_PB_OBJ_INFO *BtPbEntry = (BT_PB_OBJ_INFO *)TC_BT_Pmalloc(sizeof(BT_PB_OBJ_INFO));
	int unicodeStrLen = 0, strLen = 0;
	char *pRetStr = NULL;
	//char *pBinaryStr = NULL;
	char pBinaryStr[DRAW_NAME_SIZE];
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	
	CALLHISTORY_OBJ_TYPE *CallHistory = &(PBEntry->CallHistory);

	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullVcardEntryCfm\n",__FILENAME__,__LINE__);

       if(BtPbEntry != NULL)
       {
        	memset(BtPbEntry, 0x00, sizeof(BT_PB_OBJ_INFO));
        	memset(pBinaryStr, 0x00, DRAW_NAME_SIZE);

        	if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_PB)  /* contacts */
        	{
        		pRetStr = BtApp_PB_GetPhonebookString(0);

        		unicodeStrLen = str_len16(pRetStr);
        		
        		unicodeStrLen = unicodeStrLen * 2; /* unicodeStrLen is unicode size */
        		
        		unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE-2)) ? (DRAW_NAME_UNICODE_SIZE-2) : unicodeStrLen;
        		
        		memcpy(BtPbEntry->name, pRetStr, unicodeStrLen);
        		BtPbEntry->name[unicodeStrLen] = NULL;
        		BtPbEntry->name[unicodeStrLen+1] = NULL;

        		memcpy(BtPbEntry->telNumberCell, PBEntry->vCardObj.TelCell, MaxCallNumberSize);
        		BtPbEntry->telNumberCell[MaxCallNumberSize-1] = NULL;
        		memcpy(BtPbEntry->telNumberHome, PBEntry->vCardObj.TelHome, MaxCallNumberSize);
        		BtPbEntry->telNumberHome[MaxCallNumberSize-1] = NULL;
        		memcpy(BtPbEntry->telNumberWork, PBEntry->vCardObj.TelWork, MaxCallNumberSize);
        		BtPbEntry->telNumberWork[MaxCallNumberSize-1] = NULL;
        		memcpy(BtPbEntry->telNumberOther, PBEntry->vCardObj.TelOther, MaxCallNumberSize);
        		BtPbEntry->telNumberOther[MaxCallNumberSize-1] = NULL;
        		memset(BtPbEntry->date, 0x00, MaxCallNumberSize);
        		memset(BtPbEntry->time, 0x00, MaxCallNumberSize);
#ifdef VCARD_IMAGE_DISPLAY
        		BtPbEntry->imageType = PBEntry->vCardObj.imageType;
#endif
        	}
        	else  /* call history */
        	{
                if(!strcmp((char *)CallHistory->DrawName, "No Name"))
        		{
        			//pBinaryStr = CallHistory->CallHistoryTel;
                    strLen = strlen((char *)CallHistory->CallHistoryTel);
        			strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        			memcpy(pBinaryStr, CallHistory->CallHistoryTel, strLen);
        			pBinaryStr[strLen] = NULL;
        		}
        		else
        		{
        			//pBinaryStr = CallHistory->DrawName;
                    strLen = strlen((char *)CallHistory->DrawName);
        			strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        			memcpy(pBinaryStr, CallHistory->DrawName, strLen);
        			pBinaryStr[strLen] = NULL;
        		}
        		pRetStr = BtApp_PB_GetCallHistoryString(pBinaryStr, 0);
        		
        		unicodeStrLen = str_len16(pRetStr);

        		unicodeStrLen = unicodeStrLen * 2; /* unicodeStrLen is unicode size */

        		unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE-2)) ? (DRAW_NAME_UNICODE_SIZE-2) : unicodeStrLen;
#ifndef CODESONAR_INCLUDE /* Buffer Overrun - pRetStr */
        		memcpy(BtPbEntry->name, pRetStr, unicodeStrLen);
#endif
        		BtPbEntry->name[unicodeStrLen] = NULL;
        		BtPbEntry->name[unicodeStrLen+1] = NULL;

        		memcpy(BtPbEntry->telNumberCell, CallHistory->CallHistoryTel, MaxCallNumberSize);
        		BtPbEntry->telNumberCell[MaxCallNumberSize-1] = NULL;
        		memset(BtPbEntry->telNumberHome, 0x00, MaxCallNumberSize);
        		BtPbEntry->telNumberHome[MaxCallNumberSize-1] = NULL;
        		memset(BtPbEntry->telNumberWork, 0x00, MaxCallNumberSize);
        		BtPbEntry->telNumberWork[MaxCallNumberSize-1] = NULL;
        		memset(BtPbEntry->telNumberOther, 0x00, MaxCallNumberSize);
        		BtPbEntry->telNumberOther[MaxCallNumberSize-1] = NULL;
        		memcpy(BtPbEntry->date, CallHistory->CallHistoryDate, MaxCallNumberSize);
        		BtPbEntry->date[MaxCallNumberSize-1] = NULL;
        		memcpy(BtPbEntry->time, CallHistory->CallHistoryTime, MaxCallNumberSize);
        		BtPbEntry->time[MaxCallNumberSize-1] = NULL;
        	}

        	if(pRetStr != NULL)
        	{
        		(void)TC_BT_Pfree(pRetStr);
        	}
            TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLVCARDENTRY_CFM, BtPbEntry);

       }
}

void BtApp_PB_EvtPACPullVcardEntryErr(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullVcardEntryErr\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLVCARDENTRY_ERR, NULL);

}

void BtApp_PB_EvtPACPullPBSizeOnlyCfm(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullPBSizeOnlyCfm\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLPB_SIZEONLY_CFM, NULL);

}

void BtApp_PB_EvtPACPullPBNoData(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtPACPullPBNoData\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_PAC_PULLPB_NODATA_CFM, NULL);

}
 
/* Browsing event */
#ifdef ______________________________________________________________________________ 
#endif
void BtApp_PB_EvtGetContacts(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtGetContacts\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_GET_CONTACTS, NULL);

}

void BtApp_PB_EvtGetContactsList(void)
{
	int unicodeStrLen = 0;
	char *pRetStr = NULL;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();

	//unsigned char info[512]; /* event buffer */
	unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char) * 512);
       if(info != NULL)
       {
        	unsigned char ret = 0;
        	unsigned int info_size = 0;

        	BT_PB_UI_LIST contactsList[TCC_MAX_PHONEBOOK_LIST];
        	int i = 0, j = 0, count = 0;

        	//printf("[%s : %05d] Info : BtApp_PB_EvtGetContactsList\n",__FILENAME__,__LINE__);

        	memset(info, 0x00, 512); /* event buffer */
        	memset(&contactsList, 0x00, sizeof(BT_PB_UI_LIST)*TCC_MAX_PHONEBOOK_LIST);

        	info_size = 0;

        	/* count */
        	if(PBSearchIdx->searchKeyword[0] == 0xFF)
        	{
        		count = PBInst->vCardObjNum[PBInst->deviceId] - PBInst->listStartOffset;
        	}
        	else
        	{
        		count = PBSearchIdx->searchIdxListCount - PBInst->listStartOffset;
        	}

        	if((count > 0) && (count <= TCC_MAX_PHONEBOOK_LIST))
        	{
        		info[info_size] = count;
        	}
        	else if(count > TCC_MAX_PHONEBOOK_LIST)
        	{
        		info[info_size] = TCC_MAX_PHONEBOOK_LIST;
        	}
        	else /* exception */
        	{
        		info[info_size] = 0;
        	}

        /* name */
        	j = 0;
        	for(i = PBInst->listStartOffset; i < (PBInst->listStartOffset+info[info_size]); i++)
        	{
        		if(j >= TCC_MAX_PHONEBOOK_LIST)
        		{
        			break;
        		}
        		if(PBSearchIdx->searchKeyword[0] == 0xFF) /* search ALL */
        		{
        			pRetStr = BtApp_PB_GetPhonebookString(i);
        		}
        		else
        		{
        			if(PBSearchIdx->searchIdxList[i] < PBInst->vCardObjNum[PBInst->deviceId])
        			{
        				pRetStr = BtApp_PB_GetPhonebookString(PBSearchIdx->searchIdxList[i]);
        			}
        		}
        		unicodeStrLen = str_len16(pRetStr);
        		unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE/2)) ? (DRAW_NAME_UNICODE_SIZE) : (unicodeStrLen*2);
        		memcpy(contactsList[j].name, pRetStr, unicodeStrLen);
        		contactsList[j].name[DRAW_NAME_UNICODE_SIZE-1] = NULL; /* for unicode string */
        		contactsList[j].name[DRAW_NAME_UNICODE_SIZE-2] = NULL; /* for unicode string */
        		if(pRetStr != NULL)
        		{
        			(void)TC_BT_Pfree(pRetStr);
        		}
#ifdef VCARD_IMAGE_DISPLAY
                if(PBSearchIdx->searchKeyword[0] == 0xFF) /* search ALL */
                {
                    if(i < PBInst->vCardObjNum[PBInst->deviceId]) /* valid index */
                    {
                        contactsList[j].vcfImgType = PBInst->vCardObj[PBInst->deviceId][i].imageType;
                    }
                }
                else
                {
                    if(PBSearchIdx->searchIdxList[i] < PBInst->vCardObjNum[PBInst->deviceId]) /* valid index */
                    {
                        contactsList[j].vcfImgType = PBInst->vCardObj[PBInst->deviceId][PBSearchIdx->searchIdxList[i]].imageType;
                    }
                }
#endif

        		j++;
        	}
        	ret = sizeof(unsigned int);
        	info_size += ret;

        	memcpy(&(info[info_size]), (unsigned char *)&contactsList, sizeof(BT_PB_UI_LIST) * TCC_MAX_PHONEBOOK_LIST);
        	info_size += sizeof(BT_PB_UI_LIST) * TCC_MAX_PHONEBOOK_LIST;
            TcBtEvt_PBEvt(BT_PB_EVT_GET_CONTACTS_LIST, info);

        }
}

void BtApp_PB_EvtGetContactsListInfo(void)
{
	BT_PB_OBJ_INFO *BtPBList = (BT_PB_OBJ_INFO *)TC_BT_Pmalloc(sizeof(BT_PB_OBJ_INFO));
	int unicodeStrLen = 0, typeLength = 0;
	char *pRetStr = NULL;
	VCARD_OBJ_TYPE *vCardObj;
	int index = 0;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();


       if(BtPBList != NULL)
       {
        	memset(BtPBList, 0x00, sizeof(BT_PB_OBJ_INFO));

        /* Received total vCard Number */
        	BtPBList->mTotalCnt = PBInst->vCardObjNum[PBInst->deviceId];

        	if(PBInst->index >= PBInst->vCardObjNum[PBInst->deviceId])
        	{
                TcBtEvt_PBEvt(BT_PB_EVT_GET_CONTACTS_LIST_CLOSE, NULL);
        		return; /* invalid index */
        	}
        	else
        	{
                if(PBSearchIdx->searchKeyword[0] == 0XFF) /* search ALL */
        		{
        			index = PBInst->index;
        		}
        		else
        		{
        			if(PBSearchIdx->searchIdxList[PBInst->index] < PBInst->vCardObjNum[PBInst->deviceId]) /* valid index */
        			{
        				index = PBSearchIdx->searchIdxList[PBInst->index];
        			}
        		}
        	}
        	pRetStr = BtApp_PB_GetPhonebookString(index);
        	unicodeStrLen = str_len16(pRetStr);
        	unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE/2)) ? (DRAW_NAME_UNICODE_SIZE) : (unicodeStrLen*2);

        /* name */
        	memcpy(BtPBList->name, pRetStr, unicodeStrLen);
        	//BtPBList.Name[unicodeStrLen-1] = NULL;
        	BtPBList->name[DRAW_NAME_UNICODE_SIZE-1] = NULL; /* for unicode string */
        	BtPBList->name[DRAW_NAME_UNICODE_SIZE-2] = NULL; /* for unicode string */

        	vCardObj =&(PBInst->vCardObj[PBInst->deviceId][0]);
        /* home type */
        	//typeLength = strlen(PBInst->vCardObj[contactsListCount].TelHome);
            typeLength = strlen((char *)vCardObj[index].TelHome);
        	typeLength = (typeLength >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : typeLength;

        	//memcpy(BtPBList.telNumberHome, PBInst->vCardObj[contactsListCount].TelHome, typeLength);
        	memcpy(BtPBList->telNumberHome, vCardObj[index].TelHome, typeLength);
        	BtPBList->telNumberHome[typeLength] = NULL;
        /* work type */
        	//typeLength = strlen(PBInst->vCardObj[contactsListCount].TelWork);
            typeLength = strlen((char *)vCardObj[index].TelWork);
        	typeLength = (typeLength >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : typeLength;
        	//memcpy(BtPBList.telNumberWork, PBInst->vCardObj[contactsListCount].TelWork, typeLength);
        	memcpy(BtPBList->telNumberWork, vCardObj[index].TelWork, typeLength);
        	BtPBList->telNumberWork[typeLength] = NULL;
        /* other type */
        	//typeLength = strlen(PBInst->vCardObj[contactsListCount].TelOther);
            typeLength = strlen((char *)vCardObj[index].TelOther);
        	typeLength = (typeLength >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : typeLength;
        	//memcpy(BtPBList.telNumberOther, PBInst->vCardObj[contactsListCount].TelOther, typeLength);
        	memcpy(BtPBList->telNumberOther, vCardObj[index].TelOther, typeLength);
        	BtPBList->telNumberOther[typeLength] = NULL;
        /* cell type */
        	//typeLength = strlen(PBInst->vCardObj[contactsListCount].TelCell);
            typeLength = strlen((char *)vCardObj[index].TelCell);
        	typeLength = (typeLength >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : typeLength;
        	//memcpy(BtPBList.telNumberCell, PBInst->vCardObj[contactsListCount].TelCell, typeLength);
        	memcpy(BtPBList->telNumberCell, vCardObj[index].TelCell, typeLength);
        	BtPBList->telNumberCell[typeLength] = NULL;

        	if(pRetStr != NULL)
        	{
        		(void)TC_BT_Pfree(pRetStr);
        	}
            TcBtEvt_PBEvt(BT_PB_EVT_GET_CONTACTS_LIST_INFO, BtPBList);

        }
}

void BtApp_PB_EvtGetContactsListClose(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtGetContactsListClose\n",__FILENAME__,__LINE__);
    TcBtEvt_PBEvt(BT_PB_EVT_GET_CONTACTS_LIST_CLOSE, NULL);

}

void BtApp_PB_EvtGetCallHistoryList(void)
{
	int unicodeStrLen = 0, strLen = 0, strOffset = 0;
	char *pRetStr = NULL;
	char pBinaryStr[DRAW_NAME_SIZE];
	char pBinaryTempStr[DRAW_NAME_SIZE];
	char chType[4][4] = {"(D)", "(R)", "(M)", "(X)"};
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	CALLHISTORY_OBJ_TYPE *CallHistory = &(PBInst->CallHistory[select_callhistory[BTAPP_PB_STORAGE]][0]);

	//unsigned char info[512]; /* event buffer */
	unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char) * 512);
       if(info != NULL)
       {
        	unsigned char ret = 0;
        	unsigned int info_size = 0;

        	BT_PB_UI_LIST callHistoryList[TCC_MAX_PHONEBOOK_LIST];
        	int	callHistoryfolder = select_callhistory[BTAPP_PB_STORAGE];
        	int i = 0, j = 0, count = 0;

        	//printf("[%s : %05d] Info : BtApp_PB_EvtGetCallHistoryList\n",__FILENAME__,__LINE__);

        	memset(info, 0x00, 512); /* event buffer */
        	memset(&callHistoryList, 0x00, sizeof(BT_PB_UI_LIST)*TCC_MAX_PHONEBOOK_LIST);

        	memset(pBinaryStr, 0x00, DRAW_NAME_SIZE); /* clear buffer */
        	memset(pBinaryTempStr, 0x00, DRAW_NAME_SIZE); /* clear buffer */
        	
        	info_size = 0;

        	/* count */
        	count = PBInst->CallHistory[callHistoryfolder][0].vCardNum - PBInst->listStartOffset;
        	if((count > 0) && (count <= TCC_MAX_PHONEBOOK_LIST))
        	{
        		info[info_size] = count;
        	}
        	else if(count > TCC_MAX_PHONEBOOK_LIST)
        	{
        		info[info_size] = TCC_MAX_PHONEBOOK_LIST;
        	}
        	else /* exception */
        	{
        		info[info_size] = 0;
        	}

        /* name */
        	j = 0;
        	for(i = PBInst->listStartOffset; i < (PBInst->listStartOffset+info[info_size]); i++)
        	{
        		if((CallHistory[i].DrawName[0] == NULL) && (CallHistory[i].CallHistoryTel[0] != NULL))
        		{
                    strLen = strlen((char *)CallHistory[i].CallHistoryTel);
        			strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        			memcpy(pBinaryStr, CallHistory[i].CallHistoryTel, strLen);
        			pBinaryStr[strLen] = NULL;
        /* No name in phonebook */
        			callHistoryList[j].nameInfo = FALSE;
        		}
        		else
        		{
                    strLen = strlen((char *)CallHistory[i].DrawName);
        			strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        			memcpy(pBinaryStr, CallHistory[i].DrawName, strLen);
        			pBinaryStr[strLen] = NULL;
        /* name in phonebook */
        			callHistoryList[j].nameInfo = TRUE;
        		}
#if 1
        		/* check for call history type in combined call history */
        		if((pBinaryStr != NULL) && (select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_CCH))
        		{
        			strLen = strlen(pBinaryStr);

        			if(CallHistory[i].callHistoryType == BTAPP_PB_SELECT_OCH)
        			{
        				strOffset = strlen(chType[BTAPP_PB_SELECT_OCH]);
        				memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_OCH], strOffset);
        				
        			}
        			else if(CallHistory[i].callHistoryType == BTAPP_PB_SELECT_ICH)
        			{
        				strOffset = strlen(chType[BTAPP_PB_SELECT_ICH]);
        				memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_ICH], strOffset);
        			}
        			else if(CallHistory[i].callHistoryType == BTAPP_PB_SELECT_MCH)
        			{
        				strOffset = strlen(chType[BTAPP_PB_SELECT_MCH]);
        				memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_MCH], strOffset);
        			}
        			else
        			{
        				strOffset = strlen(chType[BTAPP_PB_SELECT_CCH]);
        				memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_CCH], strOffset);
        			}

        			strLen = (strLen >= (DRAW_NAME_SIZE-strOffset-1)) ? (DRAW_NAME_SIZE-strOffset-1) : strLen;
        			memcpy(&(pBinaryTempStr[strOffset]), pBinaryStr, strLen);

        			memset(pBinaryStr, 0x00, DRAW_NAME_SIZE);
        			memcpy(pBinaryStr, pBinaryTempStr, strOffset+strLen);
        			pBinaryStr[strOffset+strLen] = NULL;
        		}
#endif
        		pRetStr = BtApp_PB_GetCallHistoryString(pBinaryStr, i);
        		unicodeStrLen = str_len16(pRetStr);
        		unicodeStrLen = unicodeStrLen * 2; /* unicodeStrLen is unicode size */
        		unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE-2)) ? (DRAW_NAME_UNICODE_SIZE-2) : unicodeStrLen;
#ifndef CODESONAR_INCLUDE /* Buffer Overrun - pRetStr */
        		memcpy(callHistoryList[j].name, pRetStr, unicodeStrLen);
#endif
        		callHistoryList[j].name[unicodeStrLen] = NULL;
        		callHistoryList[j].name[unicodeStrLen+1] = NULL;

        		if(pRetStr != NULL)
        		{
        			(void)TC_BT_Pfree(pRetStr);
        		}
        		j++;
        	}

        	ret = sizeof(unsigned int);
        	info_size += ret;

        	memcpy(&(info[info_size]), (unsigned char *)&callHistoryList, sizeof(BT_PB_UI_LIST) * TCC_MAX_PHONEBOOK_LIST);
        	info_size += sizeof(BT_PB_UI_LIST) * TCC_MAX_PHONEBOOK_LIST;
            TcBtEvt_PBEvt(BT_PB_EVT_GET_CALLHISTORY_LIST, info);

        }
}

void BtApp_PB_EvtGetCallHistoryListInfo(void)
{
	BT_PB_OBJ_INFO *BtCHList = (BT_PB_OBJ_INFO *)TC_BT_Pmalloc(sizeof(BT_PB_OBJ_INFO));
	int unicodeStrLen = 0, strLen = 0, strOffset = 0;
	char *pRetStr = NULL;
	char pBinaryStr[DRAW_NAME_SIZE];
	char pBinaryTempStr[DRAW_NAME_SIZE];
	char chType[4][4] = {"(D)", "(R)", "(M)", "(X)"};
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	CALLHISTORY_OBJ_TYPE *CallHistory = &(PBInst->CallHistory[select_callhistory[BTAPP_PB_STORAGE]][0]);
	int	callHistoryfolder = select_callhistory[BTAPP_PB_STORAGE];

	//printf("[%s : %05d] Info : BtApp_PB_EvtGetCallHistoryListInfo\n",__FILENAME__,__LINE__);

       if(BtCHList != NULL)
       {
        	memset(BtCHList, 0x00, sizeof(BT_PB_OBJ_INFO));
        	memset(pBinaryStr, 0x00, DRAW_NAME_SIZE);
        	memset(pBinaryTempStr, 0x00, DRAW_NAME_SIZE);

        	if(PBInst->index >= PBInst->CallHistory[callHistoryfolder][0].vCardNum)
        	{
                TcBtEvt_PBEvt(BT_PB_EVT_GET_CALLHISTORY_LIST_CLOSE, NULL);
        		return; /* invalid index */
        	}
        	
            if(((!strcmp((char *)CallHistory[PBInst->index].DrawName, "No Name")) || (CallHistory[PBInst->index].DrawName[0] == NULL)) && (CallHistory[PBInst->index].CallHistoryTel[0] != NULL))
        	{
                strLen = strlen((char *)CallHistory[PBInst->index].CallHistoryTel);
        		strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        		memcpy(pBinaryStr, CallHistory[PBInst->index].CallHistoryTel, strLen);
        		pBinaryStr[strLen] = NULL;
        	}
        	else
        	{
                strLen = strlen((char *)CallHistory[PBInst->index].DrawName);
        		strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        		memcpy(pBinaryStr, CallHistory[PBInst->index].DrawName, strLen);
        		pBinaryStr[strLen] = NULL;
        	}
        	
        	/* check for call history type in combined call history */
        	if((pBinaryStr != NULL) && (select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_CCH))
        	{
        		strLen = strlen(pBinaryStr);

        		if(CallHistory[PBInst->index].callHistoryType == BTAPP_PB_SELECT_OCH)
        		{
        			strOffset = strlen(chType[BTAPP_PB_SELECT_OCH]);
        			memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_OCH], strOffset);
        			
        		}
        		else if(CallHistory[PBInst->index].callHistoryType == BTAPP_PB_SELECT_ICH)
        		{
        			strOffset = strlen(chType[BTAPP_PB_SELECT_ICH]);
        			memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_ICH], strOffset);
        		}
        		else if(CallHistory[PBInst->index].callHistoryType == BTAPP_PB_SELECT_MCH)
        		{
        			strOffset = strlen(chType[BTAPP_PB_SELECT_MCH]);
        			memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_MCH], strOffset);
        		}
        		else
        		{
        			strOffset = strlen(chType[BTAPP_PB_SELECT_CCH]);
        			memcpy(&(pBinaryTempStr[0]), chType[BTAPP_PB_SELECT_CCH], strOffset);
        		}

        		strLen = (strLen >= (DRAW_NAME_SIZE-strOffset-1)) ? (DRAW_NAME_SIZE-strOffset-1) : strLen;
        		memcpy(&(pBinaryTempStr[strOffset]), pBinaryStr, strLen);

        		memset(pBinaryStr, 0x00, DRAW_NAME_SIZE);
        		memcpy(pBinaryStr, pBinaryTempStr, strOffset+strLen);
        		pBinaryStr[strOffset+strLen] = NULL;
        	}
        	pRetStr = BtApp_PB_GetCallHistoryString(pBinaryStr, PBInst->index);
        	unicodeStrLen = str_len16(pRetStr);
        	unicodeStrLen = unicodeStrLen * 2; /* unicodeStrLen is unicode size */
        	unicodeStrLen = (unicodeStrLen >= (DRAW_NAME_UNICODE_SIZE-2)) ? (DRAW_NAME_UNICODE_SIZE-2) : unicodeStrLen;
#ifndef CODESONAR_INCLUDE /* Buffer Overrun - pRetStr */
        	memcpy(BtCHList->name, pRetStr, unicodeStrLen);
#endif
        	BtCHList->name[unicodeStrLen] = NULL;
        	BtCHList->name[unicodeStrLen+1] = NULL;

        /* call history telephone number */
            strLen = strlen((char *)CallHistory[PBInst->index].CallHistoryTel);
        	strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
        	memcpy(BtCHList->telNumberCell, CallHistory[PBInst->index].CallHistoryTel, strLen);
        	BtCHList->telNumberCell[strLen] = NULL;
        /* date & time */
            strLen = strlen((char *)CallHistory[PBInst->index].CallHistoryDate);
        	strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
        	memcpy(BtCHList->date, CallHistory[PBInst->index].CallHistoryDate, strLen);
        	BtCHList->date[strLen] = NULL;
            strLen = strlen((char *)CallHistory[PBInst->index].CallHistoryTime);
        	strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
        	memcpy(BtCHList->time, CallHistory[PBInst->index].CallHistoryTime, strLen);
        	BtCHList->time[strLen] = NULL;
        /* call history type */
        	BtCHList->callHistoryType = CallHistory[PBInst->index].callHistoryType;


        	if(pRetStr != NULL)
        	{
        		(void)TC_BT_Pfree(pRetStr);
        	}
            TcBtEvt_PBEvt(BT_PB_EVT_GET_CALLHISTORY_LIST_INFO, BtCHList);

        }
}

void BtApp_PB_EvtGetCallhistoryListClose(void)
{
#if 1
    TcBtEvt_PBEvt(BT_PB_EVT_GET_CALLHISTORY_LIST_CLOSE, NULL);

      // TcBtEvt_PBEvt(BT_PB_EVT_GET_CALLHISTORY_LIST_CLOSE, NULL);
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtGetCallhistoryListClose\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_GET_CALLHISTORY_LIST_CLOSE, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtGetVcardList(void) /* ADD */
{
	//unsigned char info[512]; /* event buffer */
	unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char) * 512);
	unsigned char ret = 0;
	unsigned int info_size = 0;

	BT_PB_VCARD_LIST vCardList[TCC_MAX_PHONEBOOK_LIST];
	unsigned int strLen = 0;
	PHONEBOOK_LIST_TYPE *PBList = BtApp_PB_GetPhonebookList();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();
	int i = 0, j = 0, count = 0;

	//printf("[%s : %05d] Info : BtApp_PB_EvtGetVcardList\n",__FILENAME__,__LINE__);

       if(info != NULL)
       {
        	memset(info, 0x00, 512); /* event buffer */
        	memset(&vCardList, 0x00, sizeof(BT_PB_VCARD_LIST)*TCC_MAX_PHONEBOOK_LIST);

        	info_size = 0;

        	/* count */
        	if(PBSearchIdx->searchKeyword[0] == 0xFF)
        	{
        		count = PBList->numOfHandle - PBList->listStartOffset;
        	}
        	else
        	{
        		count = PBSearchIdx->searchIdxListCount - PBList->listStartOffset;
        	}
        	
        	if((count > 0) && (count <= TCC_MAX_PHONEBOOK_LIST))
        	{
        		info[info_size] = count;
        	}
        	else if(count > TCC_MAX_PHONEBOOK_LIST)
        	{
        		info[info_size] = TCC_MAX_PHONEBOOK_LIST;
        	}
        	else /* exception */
        	{
        		info[info_size] = 0;
        	}

        	j = 0;
        	for(i = PBList->listStartOffset; i < (PBList->listStartOffset+info[info_size]); i++)
        	{
        		if(j >= TCC_MAX_PHONEBOOK_LIST)
        		{
        			break;
        		}

        		if(PBSearchIdx->searchKeyword[0] == 0xFF) /* search ALL */
        		{
        			strLen = strlen(PBList->vCardListObj[i].handle);
        			strLen = (strLen >= (DRAW_NAME_UTF8_SIZE-1)) ? (DRAW_NAME_UTF8_SIZE-1) : strLen;
        			memcpy(vCardList[j].handle, PBList->vCardListObj[i].handle, strLen);
        			vCardList[j].handle[strLen] = NULL;

        			strLen = strlen(PBList->vCardListObj[i].name);
        			strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        			memcpy(vCardList[j].name, PBList->vCardListObj[i].name, strLen);
        			vCardList[j].name[strLen] = NULL;
        		}
        		else
        		{
        			if(PBSearchIdx->searchIdxList[i] < PBList->numOfHandle)
        			{
        				strLen = strlen(PBList->vCardListObj[PBSearchIdx->searchIdxList[i]].handle);
        				strLen = (strLen >= (DRAW_NAME_UTF8_SIZE-1)) ? (DRAW_NAME_UTF8_SIZE-1) : strLen;
        				memcpy(vCardList[j].handle, PBList->vCardListObj[PBSearchIdx->searchIdxList[i]].handle, strLen);
        				vCardList[j].handle[strLen] = NULL;

        				strLen = strlen(PBList->vCardListObj[PBSearchIdx->searchIdxList[i]].name);
        				strLen = (strLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : strLen;
        				memcpy(vCardList[j].name, PBList->vCardListObj[PBSearchIdx->searchIdxList[i]].name, strLen);
        				vCardList[j].name[strLen] = NULL;
        			}
        		}

        		j++;
        	}
        	
        	ret = sizeof(unsigned int);
        	info_size += ret;

        	memcpy(&(info[info_size]), (unsigned char *)&vCardList, sizeof(BT_PB_VCARD_LIST) * TCC_MAX_PHONEBOOK_LIST);
        	info_size += sizeof(BT_PB_VCARD_LIST) * TCC_MAX_PHONEBOOK_LIST;
            TcBtEvt_PBEvt(BT_PB_EVT_GET_VCARD_LIST, info);

        }
}


/* other event */
#ifdef ______________________________________________________________________________ 
#endif
void BtApp_PB_EvtAutoDownPBCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedAutoDownload();
           TcBtEvt_PBEvt(BT_PB_EVT_SETAUTODOWNLOAD_CFM, info);

       }
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtAutoDownPBCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedAutoDownload();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_SETAUTODOWNLOAD_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtVcardDumpCfm(void)
{
#if 1
       unsigned char *info = (unsigned char *)TC_BT_Pmalloc(sizeof(unsigned char));
       if(info != NULL)
       {
            *info = BtApp_PB_EvtSelectedVcardDump();
           TcBtEvt_PBEvt(BT_PB_EVT_SETVCARDDUMP_CFM, info);

       }
#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtVcardDumpCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = BtApp_PB_EvtSelectedVcardDump();
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_SETVCARDDUMP_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtSetStatusCfm(void)
{
#if 1
    TcBtEvt_PBEvt(BT_PB_EVT_SET_STATUS_CFM, NULL);

#else
	unsigned char info[256];
	unsigned char ret;
	unsigned int info_size;

	//printf("[%s : %05d] Info : BtApp_PB_EvtSetStatusCfm\n",__FILENAME__,__LINE__);

	memset(info, 0x00, 256);
	
	info_size = 0;
	info[info_size] = 0; /* dummy data */
	ret = sizeof(unsigned char);
	info_size += ret;
	DMP_EvtBTInfoMessageSend(src, BT_CONTACTS_EVT_SET_STATUS_CFM, (void *)info, info_size);
#endif
}

void BtApp_PB_EvtReserved(void)
{
	//printf("[%s : %05d] Info : BtApp_PB_EvtReserved\n",__FILENAME__,__LINE__);

	//DMP_EvtBtFourDataMessageSend(src, BT_PB_EVT_RESERVED, 0, 0, 0, 0);
	//TcBtEvt_PBEvt(BT_PB_EVT_MAX, NULL);
}

void BtApp_PB_EvtGetSearchContactsList(void)
{
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();		

	//printf("[%s : %05d] Info : BtApp_PB_EvtGetSearchContactsList\n",__FILENAME__,__LINE__);

	PBSearchIdx->searchIdxListCount = BtApp_PB_MakeSearchContactsIndexList();

       unsigned int *info = (unsigned int *)TC_BT_Pmalloc(sizeof(unsigned int));
       if(info != NULL)
       {
            //DMP_EvtBTInfoMessageSend(MSG_SRC_BT_PBAP, BT_CONTACTS_EVT_GET_SEARCH_CONTACTS_LIST, (unsigned char *)&PBSearchIdx->searchIdxListCount, sizeof(unsigned int));
            *info = PBSearchIdx->searchIdxListCount;
           TcBtEvt_PBEvt(BT_PB_EVT_GET_SEARCH_CONTACTS_LIST, info);

       }
}

/* sub functions of event handlers*/
#ifdef ______________________________________________________________________________ 
#endif

unsigned char BtApp_PB_EvtSelectedSorting(void)
{
	return gBTPBAPSelectSorting; /* selected source */
}

unsigned char BtApp_PB_EvtSelectedSource(void)
{
	return gPBAPSelectSource; /* selected source */
}

unsigned char BtApp_PB_EvtSelectedVcardFilter(void)
{
	return gBTPBAPvCardFilterIdx; /* selected vCard filter */
}

unsigned char BtApp_PB_EvtSelectedAutoDownload(void)
{
	return gBTPBAPAutoDownIdx; /* selected auto download */
}

unsigned char BtApp_PB_EvtSelectedFeatures(void)
{
	return select_callhistory[BTAPP_PB_ROLE]; /* selected source */
}

unsigned char BtApp_PB_EvtSelectedVcardDump(void)
{
	return gBTPBVcardDumpFlag; /* selected auto download */
}

#endif /* defined(BT_PBAP_CLIENT)  
/* End Of File */
