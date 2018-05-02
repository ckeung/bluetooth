/****************************************************************************
 *   FileName    : TC_MSG_BT_HFEvt.c
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
*    yy/mm/dd     ver            descriptions                Author
*   ---------     --------       -----------------           -----------------
*    09/07/23     0.1            created                     Hwang, Jin Ho.
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*******************************************************************************/

#include "BT_CFG.h"

#if defined(BT_HANDSFREE_UNIT)

#include "TC_BT.h"
#include <stdio.h>
#include "BTAPP.h"
#include "BtApp_HF.h"
#include "BtApp_HfEvt.h"
#include "TcBtHFEvt.h"
#include "BtSys.h"
#include "BtPhoneBook.h"
#include "TC_BT/inc/BtPlatform.h"
#include "TC_BT/inc/TC_BT_SC_CMD.h"
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
#include "aecif.h"
#endif
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
* variables
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
#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ __FILE__

#ifndef HF_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif
/*****************************************************************************
*
* entry points
*
*****************************************************************************/
void BtApp_HF_EvtPowerOff(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtPowerOff : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_POWEROFF, devno, 0, 0, 0);

}

void BtApp_HF_EvtNotActivated(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtNotActivated : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
   TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_NOT_ACTIVATED, devno, 0, 0, 0);
 }

void BtApp_HF_EvtAlreadyActivated(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtAlreadyActivated : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_ALREADY_ACTIVATED, devno, 0, 0, 0);
}

void BtApp_HF_EvtActivateCfm(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtActivateCfm : %d\n",__FILENAME__,__LINE__,ucDevNo);

#if 1 //def DMP_MESSAGE_INCLUDE
	for(devno=0;devno<BT_HFP_MAX_CONNECTIONS;devno++)
	{
#ifdef BT_HFP_MULTI_RING_SUPPORT
		gBtAppHfInbandRingPriority[devno]=0;
#endif				
		gBtAppHfInbandRingPriority[devno]=1;

#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS
		gBtAppHfAutoAnswerDelay[devno]=4;
#else
		gBtAppHfAutoAnswerDelay[devno]=0;
#endif
		gBtAppHfServiceUse[devno]=TRUE;
		gBtAppHfRoamUse[devno]=TRUE;
		gBtAppHfSignalUse[devno]=TRUE;
		gBtAppHfMessageUse[devno]=TRUE;
		gBtAppHfBattchgUse[devno]=TRUE;
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
        gBtAppHfMicGainIndex[devno]=AEC_MIC_VOL_11;
        gBtAppHfSpkVolIndex[devno]=AEC_SPK_VOL_11;
#else
        gBtAppHfMicGainIndex[devno]=11;
        gBtAppHfSpkVolIndex[devno]=11;
#endif
#endif
		gBtAppHfEchoOnOffStatus[devno]=TRUE;
		gBtAppHfClccStatus[devno]=TRUE;
		gBtAppHfMicMuteStatus[devno] = FALSE;
		gBtAppHfAudioTransferStatus[devno] = TRUE;
	}
#else
	UI_BT_DrawPopUp(BT_TEXT_HF_ACTIVATED);
#endif

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_ACTIVATE_CFM, devno, 0, 0, 0);
}

void BtApp_HF_EvtActivateErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtActivateErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
     TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_ACTIVATE_ERR, devno, 0, 0, 0);
}

void BtApp_HF_EvtDeactivateCfm(unsigned char ucDevNo)
{
	unsigned char devno;

   // fprintf(stderr,"[%s : %05d] Info : BtApp_HF_EvtDeactivateCfm : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DEACTIVATE_CFM, devno, 0, 0, 0);
}

void BtApp_HF_EvtDeactivateErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDeactivateErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DEACTIVATE_ERR, devno, 0, 0, 0);
}

void BtApp_HF_EvtServiceConnectInd(unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;
	TcBtBdAddr              *pBdAddr;

    printf("[%s : %05d] Info : BtApp_HF_EvtServiceConnectInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	if(devno>=BT_HFP_MAX_CONNECTIONS)
	{
		return;
	}

	BtApp_HF_SetCurrDevNum(devno);

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))!=FALSE)
	{
		#if 1
		TCCAPI_Timer_SetTime(25 /* 2.5 sec */,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_COPS_ID,devno), BtApp_HF_GetCops);
		#endif
	}
	
#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif

	info_size = 0;
	
	info[info_size]=devno;
	info_size+=4;
	info[info_size]=gBtAppHfSupportThreeWayCalling[devno];
	info_size++;
	info[info_size]=gBtAppHfSupportEnhancedCallStatus[devno];
	info_size++;
	info[info_size]=gBtAppHfSupportEnhancedCallControl[devno];
	info_size++;
//	info[info_size]=gBtAppHfSupportChld[devno];
//	info_size++;
	info[info_size]=gBtAppHfEnhancedCallControlIdx[devno];
	info_size++;
	info[info_size]=gBtAppHfVoiceRecognitionAbility[devno];
	info_size++;
	
	pBdAddr  = BtApp_HF_GetSelectedDevBdAddr();

	if(pBdAddr)
	{
		memcpy(&info[info_size], pBdAddr, sizeof(TcBtBdAddr));      /* RemoteDevAddr */
	}
	else
	{
		TcBtBdAddr BdAddr = {0,0,0};    
		memcpy(&info[info_size], &BdAddr, sizeof(TcBtBdAddr));      /* RemoteDevAddr */
	}

	info_size += sizeof(TcBtBdAddr);
    TcBtHFEvt_EvtBTInfoMessageSend(BT_HF_EVT_SERVICE_CONNECT_IND, (unsigned char  *)info, info_size);
}

void BtApp_HF_EvtServiceConnectErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtServiceConnectErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
     TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SERVICE_CONNECT_ERR, devno, 0, 0, 0);
}

void BtApp_HF_EvtAlreadyConnected(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtAlreadyConnected : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_ALREADY_CONNECTED, devno, 0, 0, 0);
}

void BtApp_HF_EvtServiceConnectNotificationInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtServiceConnectNotificationInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SERVICE_CONNECT_NOTIFICATION_IND, devno, gBtAppHfSupportChld[devno], 0, 0);
}

void BtApp_HF_EvtDisconnectByRemote(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDisconnectByRemote : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
     TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DISCONNECT_BY_REMOTE, devno, 0, 0, 0);
}

void BtApp_HF_EvtNotConnected(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtNotConnected : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
     TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_NOT_CONNECTED, devno, 0, 0, 0);
}

void BtApp_HF_EvtDisconnectInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDisconnectInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	if(devno>=BT_HFP_MAX_CONNECTIONS)
	{
		return;
	}

	BtApp_HF_SetCurrDevNum(devno);
	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_COPS_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RING_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_TIMESTAMP_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_AUTO_ANSWER_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,devno));
 	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_AUDIO_TRANSFER_ID,devno));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PRIVATE_NUMBER_ID,devno));

 	gBtAppHfCallNumPos[devno] = 0;
	memset(gBtAppHfCallNumber[devno], 0 , BTAPP_HF_CALLNUM_MAX_LEN);
	gBtAppHfCallingLineID[devno][0] ='\0';
	BtApp_HF_SetCallIdRcvd(FALSE);
	gBtAppHfCcwaNumberID[devno][0] ='\0';
	gBtAppHfCallsetupIndex[devno]=0;
	gBtAppHfServiceSupport[devno]=0;
	gBtAppHfServiceIndex[devno]=0;
	gBtAppHfRoamSupport[devno]=0;
	gBtAppHfRoamIndex[devno]=0;
	gBtAppHfSignalSupport[devno]=0; /* Telechips' 2008.12 QA - 3.2 */
	gBtAppHfSignalIndex[devno]=0;
	gBtAppHfMessageSupport[devno]=0; /* Telechips' 2008.12 QA - 3.2 */
	gBtAppHfMessageIndex[devno]=0;
	gBtAppHfBattchgSupport[devno]=0; /* Telechips' 2008.12 QA - 3.2 */
	gBtAppHfBattchgIndex[devno]=0;
	gBtAppHfPeriodicEvent[devno]=FALSE;
#if defined(BT_PBAP_CLIENT) 
	memset(&CurrentCallEntryPBData, 0, sizeof(Call_Entry_PB_Data));
#endif
  TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DISCONNECT_IND, devno, 0, 0, 0);
}

void BtApp_HF_EvtDisconnectErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDisconnectErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
      TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DISCONNECT_ERR, devno, 0, 0, 0);
}

void BtApp_HF_EvtDisconnectLinkLoss(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDisconnectLinkLoss : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
     TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_DISCONNECT_LINK_LOSS, devno, 0, 0, 0);
}

void BtApp_HF_EvtAudioInd(unsigned char ucDevNo)
{
	unsigned char devno;
//#ifdef BT_HFP_WIDEBAND_SPEECH
	unsigned char boCodecUpdated=FALSE;
//#endif
	unsigned int uiCodec=0;

    printf("[%s : %05d] Info : BtApp_HF_EvtAudioInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#ifdef BT_HFP_WIDEBAND_SPEECH
	if(BtApp_HF_GetLastCodec()!=BtApp_HF_GetCurCodec())
	{
		boCodecUpdated=TRUE;
	}
	
	uiCodec=BtApp_HF_GetCodecInd();
	BtApp_HF_SetLastCodec(uiCodec);
#endif

    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_AUDIO_IND, devno, gBtAppHfAudioTransferStatus[devno], boCodecUpdated, uiCodec);

}

void BtApp_HF_EvtAudioAcceptInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtAudioInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_AUDIO_ACCEPT_IND, devno, 0, 0, 0);

}

void BtApp_HF_EvtUpdateSupportedCodec(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtUpdateSupportedCodec : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_UPDATE_SUPPORTED_CODEC_CFM, devno, 0, 0, 0);

}


void BtApp_HF_EvtSelectedCodecInd(unsigned char ucDevNo)
{
#ifdef BT_HFP_WIDEBAND_SPEECH
	unsigned char devno;
	unsigned char boCodecUpdated=FALSE;
	unsigned int uiCodec=0;

    printf("[%s : %05d] Info : BtApp_HF_EvtSelectedCodecInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

	if(BtApp_HF_GetLastCodec()!=BtApp_HF_GetCurCodec())
	{
		boCodecUpdated=TRUE;
	}

	uiCodec=BtApp_HF_GetCodecInd();
	BtApp_HF_SetLastCodec(uiCodec);

    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SELECTED_CODEC_IND, devno, gBtAppHfAudioTransferStatus[devno], boCodecUpdated, uiCodec);

#endif	
}


void BtApp_HF_EvtIncoming(unsigned char id, unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtIncoming : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	info[info_size]=BtApp_HF_InbandRing();
	info_size++;
	info[info_size]=gBtAppHfInbandRingType[devno];
	info_size++;	
	info[info_size]=BtApp_HF_RejectAbility();
	info_size++;
	BtApp_HF_Info_Ring(&info[info_size],&info[info_size+1]);
	info_size+=1+BTAPP_HF_CALLNUM_MAX_LEN;
        TcBtHFEvt_EvtBTInfoMessageSend(id, (unsigned char  *)info, info_size);
}

void BtApp_HF_EvtIncomingInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtIncomingInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif
    BtApp_HF_EvtIncoming(BT_HF_EVT_INCOMING_IND,devno);
#if !defined(BT_PTS_TEST) /* Telechips' 2008.12 QA - 3.6 (a), (b) */
	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno), BtApp_HF_Timer_Clcc );
	}
#endif	
}


void BtApp_HF_EvtIncomingTimeoutInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtIncomingTimeoutInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_INCOMING_TIMEOUT_IND, devno, 0, 0, 0);
}

void BtApp_HF_EvtIncomingEndInd(unsigned char ucDevNo)
{
	char boService,boCall;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtIncomingEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

	boService=(char)((gBtAppHfServiceSupport[devno]!=0) && (gBtAppHfServiceIndex[devno]==0));

    boCall=G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL));
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_INCOMING_END_IND, devno, boService, boCall, 0);
}

void BtApp_HF_EvtCievCall(unsigned char id, unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	info[info_size]=gBtAppHfCallStatus[devno];
	info_size++;
	info[info_size]=gBtAppHfCallsetupStatus[devno];
	info_size++;
	info[info_size]=gBtAppHfCallheldStatus[devno];
	info_size++;
	info[info_size]=gBtAppHfEnhancedCallControlIdx[devno];
	info_size++;
	info[info_size]=(char)((gBtAppHfServiceSupport[devno]!=0) && (gBtAppHfServiceIndex[devno]==0));
	info_size++;	
	info[info_size] = ((unsigned char)(gBtAppHfCallSec[devno])) & 0xFF;
	info_size++;
	info[info_size] = ((unsigned char)(gBtAppHfCallSec[devno])>>8) & 0xFF;
	info_size++;
	info[info_size] = ((unsigned char)(gBtAppHfCallSec[devno])>>16) & 0xFF;
	info_size++;
	info[info_size] = ((unsigned char)(gBtAppHfCallSec[devno])>>24) & 0xFF;
	info_size++;	
	//*((unsigned int*)(&info[info_size]))=gBtAppHfCallSec[devno];
	//info_size+=4;
	BtApp_HF_Info_Call(&info[info_size],&info[info_size+1]);
	info_size+=1+BTAPP_HF_CALLNUM_MAX_LEN;
    TcBtHFEvt_EvtBTInfoMessageSend(id, (unsigned char  *)info, info_size);
}

void BtApp_HF_EvtCievCallZeroInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievCallZeroInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

	gBtAppHfCallSec[devno]=0;
    BtApp_HF_EvtCievCall(BT_HF_EVT_CIEV_CALL_ZERO_IND,devno);
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,devno));
}

void BtApp_HF_EvtCievCallOneInd(unsigned char ucDevNo)
{
	//unsigned char info[256];
	//unsigned int info_size;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievCallOneInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif

	if(gBtAppHfCallheldStatus[devno]<=0)
	{
		gBtAppHfCallSec[devno]=0;
	}
BtApp_HF_EvtCievCall(BT_HF_EVT_CIEV_CALL_ONE_IND,devno);
#if !defined(BT_PTS_TEST) /* Telechips' 2008.12 QA - 3.6 (a), (b) */
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno), BtApp_HF_Timer_Clcc  );
#endif
}

void BtApp_HF_EvtCievCallSetupInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievCallSetupInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    BtApp_HF_EvtCievCall(BT_HF_EVT_CIEV_CALLSETUP_IND,devno);
}

void BtApp_HF_EvtCievCallheldInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievCallheldInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    BtApp_HF_EvtCievCall(BT_HF_EVT_CIEV_CALLHELD_IND,devno);
#if !defined(BT_PTS_TEST) /* Telechips' 2008.12 QA - 3.6 (a), (b) */
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno), BtApp_HF_Timer_Clcc );
#endif	
}

void BtApp_HF_EvtCievServiceInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievServiceInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,devno));

    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CIEV_SERVICE_IND, devno, gBtAppHfServiceSupport[devno], gBtAppHfServiceIndex[devno], 0);

}

void BtApp_HF_EvtCievRoamInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievRoamInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CIEV_ROAM_IND, devno, gBtAppHfRoamSupport[devno], gBtAppHfRoamIndex[devno], 0);

}

void BtApp_HF_EvtCievSignalInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievSignalInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CIEV_SIGNAL_IND, devno, gBtAppHfSignalSupport[devno], gBtAppHfSignalIndex[devno], 0);

}

void BtApp_HF_EvtCievBattChgInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievBattChgInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CIEV_BATTCHG_IND, devno, gBtAppHfBattchgSupport[devno], gBtAppHfBattchgIndex[devno], 0);

}

void BtApp_HF_EvtCievMessageInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCievMessageInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CIEV_MESSAGE_IND, devno, gBtAppHfMessageSupport[devno], gBtAppHfMessageIndex[devno], 0);

}

void BtApp_HF_EvtAnswerReq(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtAnswerReq : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_ANSWER_REQ, devno, 0, 0, 0);

}

void BtApp_HF_EvtRejectReq(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtRejectReq : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_REJECT_REQ, devno, 0, 0, 0);

}

void BtApp_HF_EvtRejectErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtRejectErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_REJECT_ERR, devno, 0, 0, 0);

}

void BtApp_HF_EvtTerminateReq(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtTerminateReq : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_TERMINATE_REQ, devno, 0, 0, 0);

}

void BtApp_HF_EvtPlaceReq(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtPlaceReq : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_PLACE_REQ, devno, 0, 0, 0);

}

void BtApp_HF_EvtCallAlreadyExist(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCallAlreadyExist : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CALL_ALREADY_EXIST, devno, 0, 0, 0);

}

void BtApp_HF_EvtClipInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtClipInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    BtApp_HF_EvtIncoming(BT_HF_EVT_CLIP_IND,devno);
 //	TcBtHFEvt_EvtBtFourDataMessagedevnoSend(BT_HF_EVT_CLIP_IND, 0, 0, 0, 0);
}

void BtApp_HF_EvtOutgoing(unsigned char id, unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoing : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	info[info_size]=(unsigned char)((gBtAppHfBattchgSupport[devno]!=0) && (gBtAppHfBattchgIndex[devno]<2));
	info_size++;
	BtApp_HF_Info_Outgoing(&info[info_size],&info[info_size+1]);
	info_size+=1+BTAPP_HF_CALLNUM_MAX_LEN;
    TcBtHFEvt_EvtBTInfoMessageSend(id, (unsigned char  *)info, info_size);

}

void BtApp_HF_EvtOutgoingNumberInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingNumberInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#if 0	/* Telechips Linux HF remark */
	UI_Event_DrawPlayScreen();
#endif

#ifdef BT_HFP_PERIODIC_AT_CLCC
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
					TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno), BtApp_HF_Timer_Clcc );
#endif


}

void BtApp_HF_EvtOutgoingFromHfgInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingFromHfgInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;
	
	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OUTGOING_FROM_HFG_IND, devno, 0, 0, 0);
}

void BtApp_HF_EvtOutgoingStartInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingStartInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif
     BtApp_HF_EvtOutgoing(BT_HF_EVT_OUTGOING_START_IND, devno);
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,devno));
}

void BtApp_HF_EvtOutgoingEndInd(unsigned char ucDevNo)
{
	char boService;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,devno));
	
	boService=(char)((gBtAppHfServiceSupport[devno]!=0) && (gBtAppHfServiceIndex[devno]==0));
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OUTGOING_END_IND, devno, boService, 0, 0);

}

void BtApp_HF_EvtOutgoingRetry(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingRetry : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	TCCAPI_Timer_SetTime(20 /* 2 sec */,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,devno), BtApp_HF_Timer_InputNumberDial);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OUTGOING_RETRY, devno, 0, 0, 0);

}

void BtApp_HF_EvtOutgoingDialInf(unsigned char ucDevNo)
{
	int value=0;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingDialInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
    if(strcmp((char *)&(gBtAppHfCallNumber[devno][0]), 	BTAPP_HF_UNKNOWN_NUMBER_STRING) == 0)
	{
		value=1;
	}

    if(strcmp((char *)&(gBtAppHfCallNumber[devno][0]), BTAPP_HF_PRIVATE_NUMBER_STRING) == 0)
	{
		value=2;
	}
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OUTGOING_DIAL_INF, devno, value, 0, 0);

}

void BtApp_HF_EvtCallTimeStampCountid(unsigned char ucDevNo)
{
	unsigned char devno;

    //printf("[%s : %05d] Info : BtApp_HF_EvtCallTimeStampCountid : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
	{
        if(TCCAPI_Timer_BT_IsActiveTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_TIMESTAMP_ID,devno)) == FALSE)
        {
            /* Telechips Linux HF remark */
            TCCAPI_Timer_SetRepeatedTime(TIMER_BT_CALL_TIMESTAMP_TIME,
                TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_TIMESTAMP_ID,devno), BTAPP_TIMERMODE_REPEAT, BtApp_HF_Timer_CallTimeStamp );
    #if 0	/* Telechips Linux HF remark */
            UI_FNT_DrawPlayTimeInPlay();
    #endif
        }
	}


}

void BtApp_HF_EvtCallTimeStampClearId(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCallTimeStampClearId : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	gBtAppHfCallSec[devno] = 0;

	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_TIMESTAMP_ID,devno));
#if 0	/* Telechips Linux HF remark */
	UI_FNT_DrawPlayTimeInPlay();
#endif
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CALL_TIMESTAMP_CLEAR_ID, devno, gBtAppHfCallSec[devno], 0, 0);

}

void BtApp_HF_EvtSetSpeakerVolumeInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtSetSpeakerVolumeInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SET_SPEAKER_VOLUME_IND, devno, gBtAppHfSpkVolIndex[devno], gBtAppHfMicGainIndex[devno], 0);

}

void BtApp_HF_EvtSetMicGainInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtSetMicGainInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SET_MIC_GAIN_IND, devno, gBtAppHfSpkVolIndex[devno], gBtAppHfMicGainIndex[devno], 0);

}

void BtApp_HF_EvtLoopbackTestStartInd(unsigned char ucDevNo)
{
#ifdef BT_HFP_LOOPBACK_TEST
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtLoopbackTestStartInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//BtApp_HF_SetCallStart(BLUETOOTH_HF_CALL_ID);
 TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_LOOPBACK_TEST_START_IND, devno, gBtAppHfLoopbackTest[devno], 0, 0);
#endif	


}

void BtApp_HF_EvtLoopbackTestEndInd(unsigned char ucDevNo)
{
#ifdef BT_HFP_LOOPBACK_TEST
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtLoopbackTestEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//BtApp_HF_Close_Call();
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_LOOPBACK_TEST_END_IND, devno, gBtAppHfLoopbackTest[devno], 0, 0);

#endif
}

void BtApp_HF_EvtSupportVoiceRecognitionInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtSupportVoiceRecognitionInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SUPPORT_VOICE_RECOGNITION_IND
        , devno, gBtAppHfVoiceRecognitionAbility[devno], gBtAppHfVoiceRecognition[devno], gBtAppHfVoiceRecognitionLearning[devno]);

}

void BtApp_HF_EvtVoiceRecognitionStartInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtVoiceRecognitionStartInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_VOICE_RECOGNITION_START_IND
        , devno, gBtAppHfVoiceRecognition[devno], gBtAppHfVoiceRecognitionLearning[devno], 0);

}

void BtApp_HF_EvtVoiceRecognitionEndInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtVoiceRecognitionEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_VOICE_RECOGNITION_END_IND
        , devno, gBtAppHfVoiceRecognition[devno], gBtAppHfVoiceRecognitionLearning[devno], 0);

	/* Close Call status ocassionally */
}

void BtApp_HF_EvtVoiceRecognitionLearnStartInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtVoiceRecognitionLearnStartInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetLastCodec(BtApp_HF_GetCodecInd());
#endif
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_VOICE_RECOGNITION_LEARN_START_IND
        , devno, gBtAppHfVoiceRecognition[devno], gBtAppHfVoiceRecognitionLearning[devno], 0);

}

void BtApp_HF_EvtVoiceRecognitionLearnEndInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtVoiceRecognitionLearnEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_VOICE_RECOGNITION_LEARN_END_IND
        , devno, gBtAppHfVoiceRecognition[devno], gBtAppHfVoiceRecognitionLearning[devno], 0);

}

void BtApp_HF_EvtClccInd(unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;
    unsigned char ucTmpDevNo = 0;
	unsigned char ucStatus;

    //printf("[%s : %05d] Info : BtApp_HF_EvtClccInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

    ucTmpDevNo = BtApp_HF_GetCurrDevNum();
		
#ifdef BT_HFP_PERIODIC_AT_CLCC
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
					TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,devno), BtApp_HF_Timer_Clcc );
#endif
	
	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	
	ucStatus=2;
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=FALSE)
	{
		ucStatus=0;
	}
    else if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		ucStatus=1;
	}
	else
	{
	}
	info[info_size]=ucStatus;
	info_size++;
	
	if(ucStatus==0)
	{
		BtApp_HF_Info_Ring(&info[info_size],&info[info_size+1]);
	}
	else if(ucStatus==1)
	{
		BtApp_HF_Info_Outgoing(&info[info_size],&info[info_size+1]);
	}
	else 
	{
		BtApp_HF_Info_Call(&info[info_size],&info[info_size+1]);
	}

	info_size+=1+BTAPP_HF_CALLNUM_MAX_LEN;

    memcpy(&info[info_size], gBtAppHfCallingLineID[ucTmpDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
	info_size += BTAPP_HF_CALLNUM_MAX_LEN;
        TcBtHFEvt_EvtBTInfoMessageSend(BT_HF_EVT_CLCC_IND, (unsigned char  *)info, info_size);
}

void BtApp_HF_EvtCcwaInd(unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;

    //printf("[%s : %05d] Info : BtApp_HF_EvtCcwaInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
		
	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	
	BtApp_HF_Info_Ccwa(&info[info_size],&info[info_size+1]);

	info_size+=1+BTAPP_HF_CALLNUM_MAX_LEN;
        TcBtHFEvt_EvtBTInfoMessageSend(BT_HF_EVT_CCWA_IND, (unsigned char  *)info, info_size);
}

void BtApp_HF_EvtCmeeInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtCmeeInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CMEE_IND, devno, gBtAppHfCmee[devno], 0, 0);

}

void BtApp_HF_EvtOutgoingBusyLineInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOutgoingBusyLineInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OUTGOING_BUSY_LINE_IND, devno, 0, 0, 0);

}

/* headset - reserved */
void DMP_BT_HSEvtServiceConnectInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtServiceConnectInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_SERVICE_CONNECT_IND, devno, 0, 0, 0);

	//BtApp_HF_SetCallStart(BLUETOOTH_HS_AUDIO_ID);
}

void DMP_BT_HSEvtServiceConnectErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtServiceConnectErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_SERVICE_CONNECT_ERR, devno, 0, 0, 0);

}

void DMP_BT_HSEvtAlreadyConnected(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtAlreadyConnected : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_ALREADY_CONNECTED, devno, 0, 0, 0);

}

void DMP_BT_HSEvtNotConnected(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtNotConnected : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_NOT_CONNECTED, devno, 0, 0, 0);

}

void DMP_BT_HSEvtDisconnectByRemote(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtDisconnectByRemote : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_DISCONNECT_BY_REMOTE, devno, 0, 0, 0);

}

void DMP_BT_HSEvtDisconnectInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtDisconnectInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//UI_BT_Close_HSAudio();
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_DISCONNECT_IND, devno, 0, 0, 0);

}

void DMP_BT_HSEvtDisconnectErr(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtDisconnectErr : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_DISCONNECT_ERR, devno, 0, 0, 0);

}
void DMP_BT_HSEvtDisconnectLinkLoss(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtDisconnectLinkLoss : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_DISCONNECT_LINK_LOSS, devno, 0, 0, 0);

}
void DMP_BT_HSEvtRingInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtRingInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_RING_IND, devno, 0, 0, 0);

}
void DMP_BT_HSEvtRingStartInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtRingStartInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//BtApp_HF_SetRingStart(BLUETOOTH_HS_RING_ID);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_RING_START_IND, devno, 0, 0, 0);

}
void DMP_BT_HSEvtRingTimeInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtRingTimeInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

#if 0
    TcBtHFEvt_MessageEvtUpdate(BTAPP_EVT_MASK_SET_DEVNO(BT_HS_EVT_RING_END_IND,devno));
#else
#endif
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_RING_TIMEOUT_IND, devno, 0, 0, 0);

}
void DMP_BT_HSEvtRingEndInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtRingEndInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//#ifdef BT_HFP_RING_FOR_NO_IN_BAND
	//BtApp_HF_Close_Ring();
	//#endif

	//if(!UI_API_GetBluetoothStatus(BTAPP_STATUS_MASK_HS_AUDIO))
	//	BtApp_HF_Close_Call();
	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HSx_RING_ID,devno));
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_RING_END_IND, devno, 0, 0, 0);

}

void DMP_BT_HSEvtAudioOnInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtAudioOnInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HSx_RING_ID,devno));
	
	//BtApp_HF_SetCallStart(BLUETOOTH_HS_AUDIO_ID);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_AUDIO_ON_IND, devno, 0, 0, 0);

}

void DMP_BT_HSEvtAudioOffInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtAudioOffInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//UI_BT_Close_HSAudio();
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_AUDIO_OFF_IND, devno, 0, 0, 0);

}

void DMP_BT_HSEvtSetSpeakerVolumeInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtSetSpeakerVolumeInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//BtApp_HF_SetSpkVolumeIndex();		
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_SET_SPEAKER_VOLUME_IND, devno, gBtAppHfSpkVolIndex[devno], gBtAppHfMicGainIndex[devno], 0);

}

void DMP_BT_HSEvtSetMicGainInd(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : DMP_BT_HSEvtSetMicGainInd : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
	//BtApp_HF_SetMicGainIndex();		
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HS_EVT_SET_MIC_GAIN_IND, devno, gBtAppHfSpkVolIndex[devno], gBtAppHfMicGainIndex[devno], 0);

}

void BtApp_HF_EvtOptionsAutoAnswerInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsAutoAnswerInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_AUTOANSWER_INF, devno, gBtAppHfAutoAnswerDelay[devno], 0, 0);

}

void BtApp_HF_EvtOptionsMicGainInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsMicGainInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_MICGAIN_INF, devno, gBtAppHfMicGainIndex[devno], 0, 0);

}

void BtApp_HF_EvtOptionsSMSAlaramInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsSMSAlaramInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_SMSALARM_INF, devno, gBtAppHfMessageUse[devno],  0, 0);

}

void BtApp_HF_EvtOptionsRingInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsRingInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_RING_INF, devno, gBtAppHfInbandRingPriority[devno], 0, 0);

}

void BtApp_HF_EvtOptionsBuiltInInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsBuiltInInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
	
#ifdef BT_HFP_MULTI_RING_SUPPORT
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_BUILTIN_INF, devno, gBtAppHfBuiltInRingIdx[devno], 0, 0);
#endif
}

void BtApp_HF_EvtOptionsNRECInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsNRECInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_NREC_INF, devno, gBtAppHfEchoOnOffStatus[devno], 0, 0);

}

void BtApp_HF_EvtOptionsClccInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsCLCCInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_CLCC_INF, devno, 0, 0, 0);

}

void BtApp_HF_EvtOptionsBiaInf(unsigned char ucDevNo)
{
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtOptionsBiaInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_OPTIONS_BIA_INF, devno, 0, 0, 0);

}

void BtApp_HF_EvtDtmfInf(unsigned char ucDevNo)
{
	unsigned char info[256];
	unsigned int info_size;
	unsigned char devno;

    printf("[%s : %05d] Info : BtApp_HF_EvtDtmfInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

    devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

	info_size = 0;
	info[info_size]=devno;
	info_size+=4;
	memcpy(&info[info_size],&gBtAppHfDtmfNumber[devno],BTAPP_HF_DTMFNUM_MAX_LEN);
	info_size+=BTAPP_HF_DTMFNUM_MAX_LEN;
    TcBtHFEvt_EvtBTInfoMessageSend(BT_HF_EVT_DTMF_INF, (unsigned char   *)info, info_size);

}

void BtApp_HF_EvtGetFriendlyName(TcBtBdAddr *pBdAddr, unsigned char *FriendlyName)
{
	if(pBdAddr)
	{
		TcBtString *pRemoteName = NULL;
		
        pRemoteName = (TcBtString *)TC_BT_SC_DB_GetRemoteDeviceName(pBdAddr);
        
		if(pRemoteName != NULL)
		{
            strcpy((char *)FriendlyName, (char *)pRemoteName);
		}
	}
}
void BtApp_HF_EvtGetInformationInf(unsigned char ucDevNo)
{
	unsigned char 			devno;
	unsigned char 			info[256];
	unsigned int 			     info_size = 0;
	TcBtBdAddr 				*pBdAddr;
	unsigned char 			FriendlyName[TC_BT_MAX_FRIENDLY_NAME_LEN+1];
	unsigned long           RemoteVersion=0;
	
	printf("[%s : %05d] Info : BtApp_HF_EvtGetInformationInf : %d\n",__FILENAME__,__LINE__,ucDevNo);

	memset(FriendlyName, 0x00, TC_BT_MAX_FRIENDLY_NAME_LEN+1);
	memset(info, 0x00, 256);

	devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);

	#ifdef BT_HANDSFREE_UNIT
    pBdAddr  = (TcBtBdAddr *)BtApp_HF_GetCurrRemoteAddr();
	BtApp_HF_EvtGetFriendlyName(pBdAddr, FriendlyName);

	#endif
	{
#if 0 // reserved
		strcpy(FrienlyName, &gBtAppSysRemoteFriendlyName);
		pBdAddr = &gBtAppSysRemoteDevAddr;
#endif
	}
	
	info_size = 0;
	info[info_size] = devno;
	info_size+=4;
#if 1 // reserved
	if(pBdAddr)
	{
		memcpy(&info[info_size], pBdAddr, sizeof(TcBtBdAddr));		/* RemoteDevAddr */
	}
	else
	{
		TcBtBdAddr BdAddr = {0,0,0};	
		memcpy(&info[info_size], &BdAddr, sizeof(TcBtBdAddr));		/* RemoteDevAddr */
	}
#else
	memcpy(&info[info_size], pBdAddr, sizeof(TcBtBdAddr));		/* RemoteDevAddr */
#endif
	info_size += sizeof(TcBtBdAddr);
    //memcpy(&info[info_size],(const void *)BtApp_HF_RemoteVersion(), sizeof(unsigned long));
	RemoteVersion = BtApp_HF_RemoteVersion();
	memcpy(&info[info_size], &RemoteVersion, sizeof(unsigned long));
	info_size += sizeof(unsigned long);
	memcpy(&info[info_size], FriendlyName, TC_BT_MAX_FRIENDLY_NAME_LEN+1/*strlen(FrienlyName)*/);
	info_size += TC_BT_MAX_FRIENDLY_NAME_LEN+1/*strlen(FrienlyName)*/;
    TcBtHFEvt_EvtBTInfoMessageSend(BT_HF_EVT_GET_INFORMATION_INF, (unsigned char   *)info, info_size);

}

void BtApp_HF_EvtSetMicMuteInf(unsigned char ucDevNo)
{
	unsigned char devno;

	devno=(unsigned char)ucDevNo;

	BtApp_HF_SetCurrDevNum(devno);
    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_SET_MIC_MUTE_INF, devno, gBtAppHfMicMuteStatus[devno], 0, 0);

}

#endif	/* BT_HANDSFREE_UNIT */


/* End Of File */
