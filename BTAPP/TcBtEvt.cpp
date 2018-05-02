/****************************************************************************
 *   FileName    : TcBtEvt.h
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
*  Description : This file contains functions for a Telechips Bluetooth
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    15/01/06     0.1            created                     Kwon Sunhong
*
*******************************************************************************/
#include <QMessageBox>

#include "TC_BT.h"
#include "TcBtPhone.h"
#include"TcBtMusic.h"
#include "TcBtEvt.h"
#include "TC_BT/inc/TC_BT_SC_EVT.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/BtUtil.h"
/*************** BT Module status ***************/
void TcBtEvt_SysEvt(int type)
{
    emit g_TcBtPhone->m_DBSignal->SYS_Event(type);
}

/*************** SC Event  ***************/
void TcBtEvt_ScEvt(int type, void *pTcBtEvt)
{
	void *pTmpTcBtEvt = NULL;
	
    switch (type)
    {
        case BT_DM_EVT_PINCODE_IND:
        case BT_DM_EVT_SSP_JUST_WORKS_IND:
        {
            TcBtScPairingSendEvt *pTcBtSCEvtT = (TcBtScPairingSendEvt *)TC_BT_Pmalloc(sizeof(TcBtScPairingSendEvt));
			TcBtScPairingSendEvt *pTcBtSCEvtS = (TcBtScPairingSendEvt *)pTcBtEvt;

			if(pTcBtSCEvtT != NULL)
			{
                memcpy(pTcBtSCEvtT, pTcBtEvt, sizeof(TcBtScPairingSendEvt));
				pTcBtSCEvtT->deviceName = TC_BT_StringAllocCopy(pTcBtSCEvtS->deviceName);
				pTmpTcBtEvt = pTcBtSCEvtT;
			}
			break;
        }
        case BT_DM_EVT_SSP_NUMERIC_COMPARISON_IND:
        case BT_DM_EVT_SSP_PASSKEY_NOTIFICATION_IND:
        case BT_DM_EVT_CONNECTED:
        {
            TcBtScPairingSSPSendEvt *pTcBtSCEvtT = (TcBtScPairingSSPSendEvt *)TC_BT_Pmalloc(sizeof(TcBtScPairingSSPSendEvt));
            TcBtScPairingSSPSendEvt *pTcBtSCEvtS = (TcBtScPairingSSPSendEvt *)pTcBtEvt;

			if(pTcBtSCEvtT != NULL)
			{
                memcpy(pTcBtSCEvtT, pTcBtEvt, sizeof(TcBtScPairingSSPSendEvt));
				pTcBtSCEvtT->deviceName = TC_BT_StringAllocCopy(pTcBtSCEvtS->deviceName);
				pTmpTcBtEvt = pTcBtSCEvtT;
			}
            break;
        }			
    case BT_DM_EVT_READLOCALDEVICE:
        pTmpTcBtEvt = pTcBtEvt;
        default:
            break;
    }	
	
    emit g_TcBtPhone->m_DBSignal->DM_Event(type, pTmpTcBtEvt);
}

/*************** SD EventList ***************/
void TcBtEvt_SearchEvt(int type, void *pTcBtEvt)
{
	void *pTmpTcBtEvt = NULL;
	
    switch (type)
    {
        case BT_DM_EVT_SEARCH_RESULT:
        {
            TcBtSdSearchResultInd *pTcBtSDEvtT = (TcBtSdSearchResultInd *)TC_BT_Pmalloc(sizeof(TcBtSdSearchResultInd));
			TcBtSdSearchResultInd *pTcBtSDEvtS = (TcBtSdSearchResultInd *)pTcBtEvt;

            if(pTcBtSDEvtT != NULL)
			{
                memcpy(pTcBtSDEvtT, pTcBtEvt, sizeof(TcBtSdSearchResultInd));
				pTcBtSDEvtT->deviceName = TC_BT_StringAllocCopy(pTcBtSDEvtS->deviceName);
				pTmpTcBtEvt = pTcBtSDEvtT;
			}
			break;
        }
        default:
            break;
    }	

    emit g_TcBtPhone->m_DBSignal->DM_Event(type, pTmpTcBtEvt);
}

/*************** DM Control function ***************/
void TcBtEvt_DMEvt(int type, void *pTcBtEvt)
{
    emit g_TcBtPhone->m_DBSignal->DM_Event(type,pTcBtEvt);
}

/*************** Set BD Addr at SETUP UI ***************/
void TcBtEvt_SetBDAddrAtSetup(int type, void *pTcBtEvt)
{
    g_TcBtPhone->m_DBSignal->DM_Event(type, pTcBtEvt);
}

/*************** A2DP/AVRCP Event ***************/
void TcBtEvt_AvEvt(int type, void *pTcBtEvt)
{
    emit g_TcBtMusic->m_BtmusicSignal->AV_Event(type, pTcBtEvt);
}

/*************** PB EventList ***************/
void TcBtEvt_PBEvt(int type, void *pTcBtEvt)
{
    emit g_TcBtPhone->m_DBSignal->PB_Event(type, pTcBtEvt);
}

/*************** MAP EventList ***************/
void TcBtEvt_MAPEvt(int type, void* pTcBtEvt)
{
    emit g_TcBtPhone->m_DBSignal->MAP_Event(type, pTcBtEvt);
}

