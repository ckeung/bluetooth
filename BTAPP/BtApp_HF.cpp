/****************************************************************************
 *   FileName    : BTAPP_HF.c
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
*  Description : This file contains functions and variables for a Bluetooth HF application
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    09/07/13     0.1            created                     Hwang, Jin Ho.
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*    13/11/13     1.0            renamed                     Hwang, Jin-Ho.
*******************************************************************************/

#include "BT_CFG.h"
#ifdef BT_HANDSFREE_UNIT


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
#include <pthread.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "BtSys.h"
#include "BtApp_HF.h"
#include "BtApp_HfEvt.h"
#include "BtApp_Gap.h"
#include "BtPhoneBook.h"
#include "TcBtEvt.h"
#include "TcBtHFEvt.h"
#include "TC_BT/inc/BtHfCmd.h"
#include "TC_BT/inc/TC_BT_HF_EVT.h"
#include "TC_BT/inc/TC_BT_CMD.h"
#include "TC_BT/inc/TC_BT_HF_DEV.h"
#include "TC_BT/inc/TC_BT_HF_CMD.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/TC_BT_A2DP_CMD.h"
#include "TC_BT/inc/BtPlatform.h"
/* AEC */
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
#include "aecif.h"
#include <poll.h>
#include <sched.h>
#include <alsa/asoundlib.h>

/* AEC */
#include "../Codec/AEC/include/aecaudio.h"

#if defined(LINUX_3_4)
static const char *neardevice = "hw:0";
static const char *fardevice  = "hw:1";
#else
static const char *neardevice = "hw:0,0";
static const char *fardevice  = "hw:0,2";
#endif

#define NEAREND_PLAYBACK_DEVICE	neardevice
#define NEAREND_CAPTURE_DEVICE	neardevice
#define FAREND_PLAYBACK_DEVICE	fardevice
#define FAREND_CAPTURE_DEVICE	fardevice
#endif  /* BT_AEC_INCLUDE */

/* HF ring is common function */
#define HF_RING_DEVICE          "plug:tcc_raw"
#include <alsa/asoundlib.h>
#include "hfring.h"
#include "hf_ring1_8khz_mono.h"
#include "hf_ring2_8khz_mono.h"
#include "hf_ring3_8khz_mono.h"
#endif  /* Linux or ADS or WinCE */
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#ifndef HF_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

#define BTAPP_HF_PERIODIC_EVT_NUM	(1)

#if 1	/* Telechips Linux HF remark */
#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ __FILE__
#endif


/* TODO: Add your specialized code here */

/*****************************************************************************
*
* structures
*
*****************************************************************************/ 
#if defined(BT_HCI_UART_INCLUDE) && !defined(BT_HFP_SCO_PCM_INTERFACE)
void (*pBtAppHfFuncTimer)(void);
#endif

/*****************************************************************************
*
* externs
*
*****************************************************************************/
extern unsigned char gboBTHfEvent;
#if 0   /* Telechips Linux HF remark */
extern unsigned char gboBTHfEvent;
#ifdef BT_HFP_SCO_PCM_INTERFACE
extern BufferState sBTHfBuffer;
#endif
Call_Entry_PB_Data CurrentCallEntryPBData;
#ifdef BT_HFP_SRC
extern BufferState sPlayBuffer, sRecordBuffer;
extern BufferState sPlayShadow, sRecordShadow;
#endif

#ifdef TCC860x
extern int RightChannelOff;		// This prevents CODEC data in FIQ
#endif

#ifdef HF_MIC_IN_ADC
extern int Mic_In_ADC;
#endif

#ifdef TCC860x
extern void InitFIQReg(void);
extern void EnableDAIRx(void);
extern void EnableDAITx(void);
#endif
#endif
/*****************************************************************************
*
* static variables
*
*****************************************************************************/
static volatile unsigned int gBtAppHfCurrentDevNum=0;
static volatile unsigned int gBtAppHfSeletedDevNum=0;

static stBtAppHfRingPCMData gpstBtAppHfRingPCMData;

static unsigned char gboBtAppAtCmdCfm[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_TOKEN_MAX];

static void BtApp_HF_AutoAnswer(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutOutgoingEnd(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutCallEnd(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutHoldActiveAccept(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutRequestPrivateWithSpecified(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutReleaseActiveAccept(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutReleaseSpecifiedCall(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutAddCall(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutConnectTwoCalls(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutReleaseAllHeldCall(unsigned char id, unsigned char loc);
static void BtApp_HF_TimeoutCLCCINDCallheldTwo(unsigned char id, unsigned char loc);


static char BtApp_HF_GetAudioStatus(void);
//	static int Bluetooth_HsScoDataCallback(TcBtBdAddr *pBdAddr, char * theData);
static char BtApp_HF_Is_Active(void);
static void BtApp_HF_CLCCToken(char *pStrOutgoingNumber, int len);
//static unsigned long BtApp_HF_GetKey(void);		/* Test Func */
static void BtApp_HF_SetCallStartIOInit(char boReset);
static unsigned char BtApp_HF_DTMF_UpdateNumber(unsigned char number);

#ifdef __arm__
static hf_ring_array ghf_ring_array[3] =
{
    { (const short *)PCM_HF_RING1_8KHZ_MONO, (PCM_HF_RING1_8KHZ_MONO_SIZE/sizeof(short)) },
    { (const short *)PCM_HF_RING2_8KHZ_MONO, (PCM_HF_RING2_8KHZ_MONO_SIZE/sizeof(short)) },
    { (const short *)PCM_HF_RING3_8KHZ_MONO, (PCM_HF_RING3_8KHZ_MONO_SIZE/sizeof(short)) }
};
#endif

/*****************************************************************************
*
* global variables
*
*****************************************************************************/
#ifdef BT_HFP_WIDEBAND_SPEECH
unsigned int gBtAppHfCodec[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfLastCodec[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfCurCodec=TC_BT_WBS_INVALID_CODEC;
#endif	
#ifdef BT_HFP_MULTI_RING_SUPPORT
unsigned int gBtAppHfBuiltInRingIdx[BT_HFP_MAX_CONNECTIONS];
#endif
#ifdef BT_HFP_LOOPBACK_TEST
unsigned int gBtAppHfLoopbackTest[BT_HFP_MAX_CONNECTIONS];
#endif
unsigned int gBtAppHfRingSec[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfCallSec[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfInbandRingPriority[BT_HFP_MAX_CONNECTIONS]/*={1,1}*/; /* 1: Higher than built-in ring, 0: Lower */
unsigned int gBtAppHfInbandRing[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfInbandRingType[BT_HFP_MAX_CONNECTIONS];				/* 1: ACL, 0: SCO */
unsigned int gBtAppHfAutoAnswerDelay[BT_HFP_MAX_CONNECTIONS]/*={4,4}*/; /* Unit : second, 0 means disable */
unsigned int gBtAppHfAutoAnswerStart[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfVoiceRecognitionAbility[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfVoiceRecognitionLearning[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfVoiceRecognition[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfRejectAbility[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportChld[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportThreeWayCalling[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportEcNrFunction[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportEnhancedCallStatus[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportEnhancedCallControl[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportExtendedErrorCodes[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSupportCodecNegotiation[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfRemoteVersion[BT_HFP_MAX_CONNECTIONS];
volatile int gBtAppHfCallStatus[BT_HFP_MAX_CONNECTIONS]/*={-1, -1}*/;
volatile int gBtAppHfCallsetupStatus[BT_HFP_MAX_CONNECTIONS]/*={-1, -1}*/;
volatile int gBtAppHfCallheldStatus[BT_HFP_MAX_CONNECTIONS]/*={-1, -1}*/;
volatile int gBtAppHfCallclccStatus[BT_HFP_MAX_CONNECTIONS]/*={-1, -1}*/;
volatile int gBtAppHfCallclccPrevStatus[BT_HFP_MAX_CONNECTIONS]/*={-1, -1}*/;
unsigned int gBtAppHfEchoOnOffStatus[BT_HFP_MAX_CONNECTIONS]/*={TRUE,TRUE}*/;
unsigned int gBtAppHfClccStatus[BT_HFP_MAX_CONNECTIONS]/*={TRUE,TRUE}*/;
char *gBtAppHfStrNetworkOperator[BT_HFP_MAX_CONNECTIONS]/*={NULL, NULL}*/;

char gBtAppHfCallingLineID[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
unsigned char gBtAppHfCallIdRcvd[BT_HFP_MAX_CONNECTIONS];
char gBtAppHfCcwaNumberID[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
unsigned char gBtAppHfCallNumPos[BT_HFP_MAX_CONNECTIONS];
unsigned char gBtAppHfCallNumber[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
unsigned int gBtAppHfCallsetupIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfServiceUse[BT_HFP_MAX_CONNECTIONS]/*={1, 1}*/
				, gBtAppHfServiceSupport[BT_HFP_MAX_CONNECTIONS]
				, gBtAppHfServiceIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfRoamUse[BT_HFP_MAX_CONNECTIONS]/*={1, 1}*/
				, gBtAppHfRoamSupport[BT_HFP_MAX_CONNECTIONS]
				, gBtAppHfRoamIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfSignalUse[BT_HFP_MAX_CONNECTIONS]/*={1, 1}*/
				, gBtAppHfSignalSupport[BT_HFP_MAX_CONNECTIONS]
				, gBtAppHfSignalIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfMessageUse[BT_HFP_MAX_CONNECTIONS]/*={1, 1}*/
				, gBtAppHfMessageSupport[BT_HFP_MAX_CONNECTIONS]
				, gBtAppHfMessageIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfBattchgUse[BT_HFP_MAX_CONNECTIONS]/*={1, 1}*/
				, gBtAppHfBattchgSupport[BT_HFP_MAX_CONNECTIONS]
				, gBtAppHfBattchgIndex[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfCmee[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfMicGainIndex[BT_HFP_MAX_CONNECTIONS]/*={AEC_MIC_VOL_11, AEC_MIC_VOL_11}*/;
unsigned int gBtAppHfSpkVolIndex[BT_HFP_MAX_CONNECTIONS]/*={AEC_SPK_VOL_11, AEC_SPK_VOL_11}*/;
unsigned int gBtAppHfMicMuteStatus[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfAudioTransferStatus[BT_HFP_MAX_CONNECTIONS];
unsigned int gBtAppHfDtmfPos[BT_HFP_MAX_CONNECTIONS];
unsigned char gBtAppHfDtmfNumber[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_DTMFNUM_MAX_LEN];
int gBtAppHfResponseAndHoldControlIdx[BT_HFP_MAX_CONNECTIONS];
#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
unsigned char gBtAppHfEnhancedCallControlIdx[BT_HFP_MAX_CONNECTIONS];	/* AT+CHLD = 0, 1, 11, 12, 2, 21, 22, 3, 4 */
#endif

unsigned char gBtAppHfAtCmdString[24]; /* AT-CMD */
unsigned int gBtAppHfAtCmdIndex;

unsigned int gBtAppHfPeriodicEvent[BT_HFP_MAX_CONNECTIONS];

#ifdef HF_MIC_IN_ADC
int isr_key = 0;
#endif

pthread_mutex_t 	gBtAppHFMutex = PTHREAD_MUTEX_INITIALIZER;

/*****************************************************************************
*
* debug
*
*****************************************************************************/
/*****************************************************************************
* 
* entry points
*
*****************************************************************************/
char BRWS_ToUpper(char c)
{
    char lUpper;
    if ((c < 'a') || (c > 'z'))
    {
        lUpper = c;
    }
    else
    {
        /* 'a' - 'A' = 0x20 */
        lUpper = (c - 0x20);
    }
    return lUpper;
}

#ifdef BT_HCI_UART_INCLUDE
#ifndef BT_HFP_SCO_PCM_INTERFACE
/*************************************************************************************
*  BtApp_HF_Timer_Callback
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
#ifndef BT_HFP_SCO_PCM_INTERFACE
void BtApp_HF_Timer_Callback(void)
{
#if defined(BT_HCI_UART_INCLUDE)
	UARTTransferTxScoFifo();
	//hcisu_write_wakeup(&hcisu_bcspSCO_cb);
#endif
}
#endif

/*************************************************************************************
*  BtApp_HF_Timer_Init
*-------------------------------------------------------------------------------------
* Description  : It init Bluetooth Av Timer
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_Timer_Init(unsigned int uiPeriod)
{
}

/*************************************************************************************
*  BtApp_HF_Timer_DeInit
*-------------------------------------------------------------------------------------
* Description  : It de-init Bluetooth Av Timer
* Parameters   :
* Return	   :
**************************************************************************************/
void BtApp_HF_Timer_DeInit(void)
{
}
#endif

#endif	/* BT_HCI_UART_INCLUDE */

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionOnOff
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_VoiceRecognitionOnOff(void)
{
	if(BtApp_HF_VoiceRecognition()!=0)
	{
        if(G_BtHfCmd->TC_BT_HF_SetVoiceRecognition(BtApp_HF_GetSelectedDevBdAddr(), FALSE)==TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			BtApp_HF_VoiceRecognitionInd(FALSE);
		}
	}
	else
	{
        if(G_BtHfCmd->TC_BT_HF_SetVoiceRecognition(BtApp_HF_GetSelectedDevBdAddr(), TRUE)==TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			BtApp_HF_VoiceRecognitionInd(TRUE);
		}
    }
}




/*************************************************************************************
*  BtApp_HF_GetCurrDevNum
*-------------------------------------------------------------------------------------
* Description  : Linux event comes from recv thread, command comes from UI thread.
*                so, CMD-EVT must seperated with BTApp_HF_Lock & UnLock
*                to current dev number maintain the atomicity.
*                Event : TC_BT_HF_DEV_GetCurrentNum -> BtApp_HF_SetCurrDevNum -> BtApp_HF_GetCurrDevNum
*                Cmd : BtApp_HF_SetCurrDevNum -> BtApp_HF_GetCurrDevNum
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned int BtApp_HF_GetCurrDevNum(void)
{	
#if 1   /* Telechips Linux HF remark */
    if(gboBTHfEvent == FALSE)   /* Cmd */
    {
		return gBtAppHfCurrentDevNum;
	}

    return (unsigned int)TC_BT_HF_DEV_GetCurrentNum();  /* Evt */
#endif
}

/*************************************************************************************
*  BtApp_HF_SetCurrDevNum
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SetCurrDevNum(unsigned int currentDevNum)
{
	gBtAppHfCurrentDevNum = currentDevNum;
	//printf("gBtAppHfCurrentDevNum == %d\n", gBtAppHfCurrentDevNum);
}

/*************************************************************************************
*  BtApp_HF_GetSelectedDevNum
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned int BtApp_HF_GetSelectedDevNum(void)
{
	return gBtAppHfSeletedDevNum;
}

/*************************************************************************************
*  BtApp_HF_SetSelectedDevNum
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SetSelectedDevNum(unsigned int selectedDevNum)
{
	gBtAppHfSeletedDevNum = selectedDevNum;
	printf("[%s : %05d] Info : BtApp_HF_SetSelectedDevNum : %d\n",__FILENAME__,__LINE__,gBtAppHfSeletedDevNum);	
}




/*************************************************************************************
*  BtApp_HF_GetSelectedDevBdAddr
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
TcBtBdAddr *BtApp_HF_GetSelectedDevBdAddr(void)
{
	//return TC_BT_HF_DEV_GetBdAddr(BtApp_HF_GetSelectedDevNum());
	return TC_BT_HF_DEV_GetBdAddr(BtApp_HF_GetCurrDevNum());	
}


/*************************************************************************************
*  BtApp_HF_Clip
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_Clip(unsigned char id, unsigned char loc)
{	
    (void)G_BtHfCmd->TC_BT_HF_SetCallNotificationInd(BtApp_HF_GetSelectedDevBdAddr(), TRUE);
}

/*************************************************************************************
*  BtApp_HF_Clcc
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_Clcc(void)
{	
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfClccStatus[ucDevNo]==FALSE)
	{
		return;
	}
	
	//printf("BtApp_HF_Clcc : %d\n",ucDevNo);
	
    if(G_BtSys->BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==FALSE)
	{
		return;
	}
	{
		#if 0
		if(BtApp_HF_MakeLinkActive(BtApp_HF_Clcc))
			return;
		#endif

        (void)G_BtHfCmd->TC_BT_HF_GetCurrentCallList(BtApp_HF_GetSelectedDevBdAddr());
	}
}

void BtApp_HF_Timer_Clcc(unsigned char id, unsigned char loc)
{
	BtApp_HF_Clcc();
}

/*************************************************************************************
*  BtApp_HF_GetCurrRemoteAddr
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void *BtApp_HF_GetCurrRemoteAddr(void)
{
	unsigned char ucProfileUUID = FALSE;
	char cStatusConnect = FALSE;

	ucProfileUUID = TC_BT_SC_DB_CheckSupportProfile(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HFG_PROFILE_UUID);
    cStatusConnect = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT));
		
	if ((ucProfileUUID!=FALSE) && (cStatusConnect!=FALSE))
	{
		return BtApp_HF_GetSelectedDevBdAddr();
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************************
*  BtApp_HF_AutoAnswer
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_AutoAnswer(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfAutoAnswerStart[ucDevNo] = 0;

    if (G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallAnswer(BtApp_HF_GetSelectedDevBdAddr());
}


/*************************************************************************************
*  BtApp_HF_TimeoutOutgoingEnd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutOutgoingEnd(unsigned char id, unsigned char loc)
{
	BtApp_HF_CIEVINDCallsetupHandler(4);
}

/*************************************************************************************
*  BtApp_HF_TimeoutCallEnd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutCallEnd(unsigned char id, unsigned char loc)
{
	//unsigned char ucDevNo;

	//ucDevNo=BtApp_HF_GetCurrDevNum();

	//if(gBtAppHfCallheldStatus[ucDevNo]<=0)		//add callclccstatus condition here?
	{
		BtApp_HF_CIEVINDCallHandler(2);
	}
}

/*************************************************************************************
*  BtApp_HF_TimeoutHoldActiveAccept
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutHoldActiveAccept(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
}

/*************************************************************************************
*  BtApp_HF_TimeoutRequestPrivateWithSpecified
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutRequestPrivateWithSpecified(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
}

/*************************************************************************************
*  BtApp_HF_TimeoutReleaseActiveAccept
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutReleaseActiveAccept(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);

}

/*************************************************************************************
*  BtApp_HF_TimeoutReleaseSpecifiedCall
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutReleaseSpecifiedCall(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	
	
	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
	
}

/*************************************************************************************
*  BtApp_HF_TimeoutAddCall
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutAddCall(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
	
}

/*************************************************************************************
*  BtApp_HF_TimeoutConnectTwoCalls
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutConnectTwoCalls(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
}

/*************************************************************************************
*  BtApp_HF_TimeoutReleaseAllHeldCall
*-------------------------------------------------------------------------------------
* Description  : Enhanced(3way) call fail, back to the 1 call status
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_TimeoutReleaseAllHeldCall(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
	BtApp_HF_CIEVINDCallHandler(1);
	BtApp_HF_CIEVINDCallsetupHandler(0);
}

/*****************************************************************************
* Function Name : BtApp_HF_TimeoutCLCCINDCallheldTwo()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
static void BtApp_HF_TimeoutCLCCINDCallheldTwo(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	if(gBtAppHfCallheldStatus[ucDevNo] != 2)
	{
		BtApp_HF_CIEVINDCallheldHandler(2);
	}
}

/*************************************************************************************
*  BtApp_HF_Reject
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_Reject(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
    (void)G_BtHfCmd->TC_BT_HF_CallEnd(BtApp_HF_GetSelectedDevBdAddr());

	if((gBtAppHfCallheldStatus[ucDevNo]!=1) && (gBtAppHfCallclccStatus[ucDevNo]<=1))
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CALL_END_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,ucDevNo),
			BtApp_HF_TimeoutCallEnd);
	}

	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
}



/*************************************************************************************
*  BtApp_HF_AudioOnOff
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_AudioOnOff(void)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();	

	if(TCCAPI_Timer_BT_IsActiveTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_AUDIO_TRANSFER_ID,ucDevNo))==TRUE)
	{
		return;
	}
	
	TCCAPI_Timer_SetTime(10/*1s*/,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_AUDIO_TRANSFER_ID,ucDevNo), BtApp_HF_Timer_AudioOnOff);	
	
    (void)G_BtHfCmd->TC_BT_HF_ConfigAudio(BtApp_HF_GetSelectedDevBdAddr());

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO))!=FALSE)
	{
        (void)G_BtHfCmd->TC_BT_HF_AudioDisconnect(BtApp_HF_GetSelectedDevBdAddr());
	}
	else
	{
        (void)G_BtHfCmd->TC_BT_HF_AudioConnect(BtApp_HF_GetSelectedDevBdAddr());
	}
}

void BtApp_HF_Timer_AudioOnOff(unsigned char id, unsigned char loc)
{
	//BtApp_HF_AudioOnOff();
}

void BtApp_HF_AudioAccept(unsigned char enable)
{
    (void)G_BtHfCmd->TC_BT_HF_AudioAcceptConnect(BtApp_HF_GetSelectedDevBdAddr(),enable);
}

#ifdef BT_HFP_UPDATE_SUPPORTED_CODEC
/*************************************************************************************
*  BtApp_HF_UpdateSupportedCodec
*-------------------------------------------------------------------------------------
* Description  : Checks all connectd AudioGateway's support Codec : CVSD, mSBC
* Parameters   : TC_BT_WBS_CVSD_CODEC_MASK, TC_BT_WBS_MSBC_CODEC_MASK
* Return	   : 
**************************************************************************************/
void BtApp_HF_UpdateSupportedCodec(unsigned int uiMask,unsigned char boEnable)
{	
	(void)TC_BT_HF_UpdateSupportedCodec(BtApp_HF_GetSelectedDevBdAddr(),uiMask, boEnable);
}
#endif

/*************************************************************************************
*  BtApp_HF_CloseSCO
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/

void BtApp_HF_CloseSCO(void)
{
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO))!=FALSE)
	{
        (void)G_BtHfCmd->TC_BT_HF_ConfigAudio(BtApp_HF_GetSelectedDevBdAddr());
        (void)G_BtHfCmd->TC_BT_HF_AudioDisconnect(BtApp_HF_GetSelectedDevBdAddr());
	}
}


/*************************************************************************************
*  BtApp_HF_MicUp
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_MicUp(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)G_BtHfCmd->TC_BT_HF_MicGainStatus(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfMicGainIndex[ucDevNo]);
}

/*************************************************************************************
*  BtApp_HF_MicDown
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_MicDown(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)G_BtHfCmd->TC_BT_HF_MicGainStatus(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfMicGainIndex[ucDevNo]);
}


/*************************************************************************************
*  BtApp_HF_SpeakerUp
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SpeakerUp(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)G_BtHfCmd->TC_BT_HF_SpeakerGainStatus(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfSpkVolIndex[ucDevNo]);
}

/*************************************************************************************
*  BtApp_HF_SpeakerDown
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SpeakerDown(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)G_BtHfCmd->TC_BT_HF_SpeakerGainStatus(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfSpkVolIndex[ucDevNo]);
}

/*************************************************************************************
*  BtApp_HF_CallbackPeriodicEvent
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CallbackPeriodicEvent(void)
{
#ifdef BT_HFP_PERIODICEVENT
	unsigned char ucDevNo;
#endif

#ifdef BT_HFP_PERIODICEVENT
	// AT-CMD / ETC	
	for(ucDevNo = 0; ucDevNo < BT_HFP_MAX_CONNECTIONS; ucDevNo++)	
	{
		BtApp_HF_SetCurrDevNum(ucDevNo);
		if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT_NOTIFICATION))!=FALSE)
		{
			if(TCCAPI_Timer_BT_IsActiveTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PERIODIC_EVENT_ID,ucDevNo))==FALSE)
			{
				gBtAppHfPeriodicEvent[ucDevNo]=TRUE;
				
				TCCAPI_Timer_SetTime(50,
					TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PERIODIC_EVENT_ID,ucDevNo), BtApp_HF_Timer_RunAtCmdEvent);
			}
			else
			{
				// none
			}
			
			if(gBtAppHfPeriodicEvent[ucDevNo]==TRUE)
			{
				if(BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==FALSE)
				{
					(void)TC_BT_HF_GetCurrentOperatorSelection(BtApp_HF_GetSelectedDevBdAddr());
					BtApp_HF_AtCmdSendBattCharReq();
					BtApp_HF_AtCmdSendSignQualReq();
					gBtAppHfPeriodicEvent[ucDevNo]=FALSE;
				}
			}
		}
		else
		{
			// none
		}
	}
#endif
}

#ifdef BT_HFP_PERIODICEVENT
void BtApp_HF_Timer_RunAtCmdEvent(unsigned char id, unsigned char loc)
{
	// none
}
#endif

/*************************************************************************************
*  BtApp_HF_ActivateCFMHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_ActivateCFMHandler(unsigned char bCfmResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(bCfmResult == TRUE)
	{
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_HF_ACTIVATE);
        BtApp_HF_EvtActivateCfm(ucDevNo);
	}
	else
	{
        BtApp_HF_EvtActivateErr(ucDevNo);
	}
}

/*************************************************************************************
*  BtApp_HF_DeactivateCFMHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_DeactivateCFMHandler(unsigned char bCfmResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(bCfmResult == TRUE)
	{
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_HF_ACTIVATE);
 		BtApp_HF_EvtDeactivateCfm(ucDevNo);
	}
	else
	{
		BtApp_HF_EvtDeactivateErr(ucDevNo);
	}
}

/*************************************************************************************
*  BtApp_HF_ServiceConnectINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_ServiceConnectINDHandler(unsigned char bIndResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(bIndResult == TRUE)
	{
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT));

#ifdef BT_HFP_WIDEBAND_SPEECH
		BtApp_HF_SetCodecInd(TC_BT_WBS_CVSD_CODEC);
#endif	

		gBtAppHfCallStatus[ucDevNo]=-1;
		gBtAppHfCallsetupStatus[ucDevNo]=-1;
		gBtAppHfCallheldStatus[ucDevNo]=-1;
		gBtAppHfCallclccStatus[ucDevNo]=-1;
		gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
		gBtAppHfEchoOnOffStatus[ucDevNo]=TRUE;

		gBtAppHfClccStatus[ucDevNo]=TRUE;

		memset(gboBtAppAtCmdCfm,TRUE,sizeof(gboBtAppAtCmdCfm));

 		BtApp_HF_EvtServiceConnectInd(ucDevNo);
	}
	else
	{
		BtApp_HF_EvtServiceConnectErr(ucDevNo);
	}
}

void BtApp_HF_ServiceConnectNotificationINDHandler(void)
{
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_ServiceConnectNotificationINDHandler \n",__FILENAME__,__LINE__);

    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT_NOTIFICATION));
		
	ucDevNo=BtApp_HF_GetCurrDevNum();

    BtApp_HF_EvtServiceConnectNotificationInd(ucDevNo);
}

/*************************************************************************************
*  BtApp_HF_DisconnectINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_DisconnectINDHandler(unsigned char bIndResult)
{
//#ifdef BT_HFP_2xHF
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

#ifdef BT_AUDIO_SINK
	(void)TC_BT_A2DP_SetBypass(FALSE);// To prevent A2DP sound during a call code here!
#if 0
	DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_BYPASS);
#else
	{
		TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

		*pTcBtAVEvt = FALSE;
        TcBtEvt_AvEvt(BT_AV_EVT_SNK_BYPASS, pTcBtAVEvt);
	}
#endif 
#endif	/* BT_AUDIO_SINK */
	
    if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
	{
		BtApp_HF_EvtCievCallZeroInd(ucDevNo);
	}
	
    if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
		BtApp_HF_EvtOutgoingEndInd(ucDevNo);
	}
    if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=FALSE)
	{
		BtApp_HF_EvtIncomingEndInd(ucDevNo);
	}

    (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT_NOTIFICATION));

    (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT));
 
#if defined(BT_HCI_UART_INCLUDE) && !defined(BT_HFP_SCO_PCM_INTERFACE)
 	if(BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO))!=FALSE)
	{
		char boAudioConnect;

		boAudioConnect=BtApp_HF_GetAudioStatus();

		if(boAudioConnect==FALSE)
		{
			BtApp_HF_Timer_DeInit();		
			pBtAppHfFuncTimer = NULL;
			UartDrv_FlushScoQueue();
		}
	}
#else
    (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO));
#endif	

#if 1 /* Telechips' 2008.12 QA - 3.19 */
	gBtAppHfCallStatus[ucDevNo]=-1;
	gBtAppHfCallsetupStatus[ucDevNo]=-1;
	gBtAppHfCallheldStatus[ucDevNo]=-1;
	gBtAppHfCallclccStatus[ucDevNo]=-1;
	gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
#endif

	BtApp_HF_EvtDisconnectInd(ucDevNo);

	switch(bIndResult)
	{
		case TC_BT_RESULT_CODE_HF_CONNECTION_TERM_BY_REMOTE_HOST:
			BtApp_HF_EvtDisconnectByRemote(ucDevNo);
			break;
		case TC_BT_HCI_ERROR_CONN_TIMEOUT:
			BtApp_HF_EvtDisconnectLinkLoss(ucDevNo);
			break;
		default:
			break;
	}
}

static char BtApp_HF_GetAudioStatus(void)
{
	unsigned char ucDevNo;
	char cStatusHFAudio=FALSE;
	char cStatusHSAudio=FALSE;

	for(ucDevNo=0;ucDevNo<BT_HFP_MAX_CONNECTIONS;ucDevNo++)
	{
        cStatusHFAudio = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_AUDIO,ucDevNo));
        cStatusHSAudio = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HSx_AUDIO,ucDevNo));

		if((cStatusHFAudio==TRUE) || (cStatusHSAudio==TRUE))
		{
			return TRUE;
		}
	}
 	
	return FALSE;
}

/*************************************************************************************
*  BtApp_HF_AudioINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_AudioINDHandler(unsigned char bIndResult, unsigned short scoHandle)
{
	char boAudioConnect=FALSE;
	
	unsigned char ucDevNo;


	ucDevNo=BtApp_HF_GetCurrDevNum();

	/* transfer status to UI Task a status of AudioTransfer */
	gBtAppHfAudioTransferStatus[ucDevNo] = bIndResult;
	BtApp_HF_EvtAudioInd(ucDevNo);

	if(bIndResult==TRUE)
	{// on
		boAudioConnect=BtApp_HF_GetAudioStatus();

        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO));
 		if(boAudioConnect==FALSE)
		{
#if defined(BT_HCI_UART_INCLUDE) && !defined(BT_HFP_SCO_PCM_INTERFACE)
			UartDrv_FlushScoQueue();
			pBtAppHfFuncTimer = BtApp_HF_Timer_Callback;
			BtApp_HF_Timer_Init(7498/*7500*/);		// Set 7.5mS for timer5
#endif
#ifdef BT_AUDIO_SINK
            (void)TC_BT_A2DP_SetBypass(TRUE);// To prevent A2DP sound during a call code here!
#if 0
	        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_BYPASS);
#else
			{
				TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

                *pTcBtAVEvt = TRUE;
                TcBtEvt_AvEvt(BT_AV_EVT_SNK_BYPASS, pTcBtAVEvt);

			}
#endif            
#endif/* BT_AUDIO_SINK*/
		}
		
		if(BtApp_HF_VoiceRecognitionLearning()!=FALSE)
		{
            BtApp_HF_EvtVoiceRecognitionLearnStartInd(ucDevNo);
			return;
		}
	}
	else
	{// off
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO));
 		boAudioConnect=BtApp_HF_GetAudioStatus();
		
		if(boAudioConnect==FALSE)
		{
#if defined(BT_HCI_UART_INCLUDE) && !defined(BT_HFP_SCO_PCM_INTERFACE)
			BtApp_HF_Timer_DeInit();		
			pBtAppHfFuncTimer = NULL;
			UartDrv_FlushScoQueue();
#endif

#ifdef BT_AUDIO_SINK
	        (void)TC_BT_A2DP_SetBypass(FALSE);// To prevent A2DP sound during a call code here!
#if 0
	        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_BYPASS);
#else
			{
				TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

				*pTcBtAVEvt = FALSE;
                TcBtEvt_AvEvt(BT_AV_EVT_SNK_BYPASS, pTcBtAVEvt);

			}
#endif 
#endif	/* BT_AUDIO_SINK */
		}
		
		if(BtApp_HF_VoiceRecognitionLearning()!=FALSE)
		{
			BtApp_HF_VoiceRecognitionLearningToggle();
            BtApp_HF_EvtVoiceRecognitionLearnEndInd(ucDevNo);
			return;
		}

		/* Workaround - Motorola V3x */
		//if(BtApp_HF_VoiceRecognition())
		//	BtApp_HF_VoiceRecognitionOnOff();
	}
}

/*************************************************************************************
*  BtApp_HF_AudioAcceptINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_AudioAcceptINDHandler(unsigned char linkType)
{
	unsigned char boAudioConnect=TRUE;	
	//unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_AudioAcceptINDHandler \n",__FILENAME__,__LINE__);

	//ucDevNo=BtApp_HF_GetCurrDevNum();

    //TC_TimeDly(20);
    usleep(100000);
	
    (void)G_BtHfCmd->TC_BT_HF_AudioAcceptConnect(BtApp_HF_GetSelectedDevBdAddr(),boAudioConnect);
	//BtApp_HF_EvtAudioAcceptInd(ucDevNo);
}

int mem_cmp(const void *sA, const void *sB, int len)
{
    int nI,lRet;
    const unsigned char *pA = (const unsigned char *)sA;
    const unsigned char *pB = (const unsigned char *)sB;

    lRet = 0;
    for (nI=0; nI<len ; nI++)
    {
        if (pA[nI] != pB[nI])
        {
            lRet = (-1);
            break;
        }
    }

    return lRet;
}

int str_len(void *pStr)
{
    int count;
    unsigned char *str = (unsigned char *)pStr;

    if(str==NULL)
    {
        return 0;
    }

    for (count=0; str[count] != 0; count++)
    {
        ;
    }

    return count;
}

char *str_str(char *s1, char *s2)
{
    int             l1,	l2;

    if((s1 == NULL) || (s2 == NULL))
    {
        return NULL;
    }

    l2 = str_len(s2);

    if (!l2)
    {
        return (char *) s1;
    }

    l1 = str_len(s1);

    while (l1 >= l2)
    {
        l1--;

        if (!mem_cmp((void *)s1, (void *)s2, l2))
        {
            return (char *) s1;
        }

        s1++;
    }

    return NULL;
}


/*************************************************************************************
*  BtApp_HF_CindSupportIndHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CindSupportIndHandler(unsigned char *pSupportStr, const unsigned char *pSupportValue)
{
#if 1	/* Telechips Linux HF remark */
	int CindSupportCount;
	unsigned char *CindSupportStr[20];
	unsigned char CindSupportValue[20];
	unsigned char *currentStr;
	unsigned char *tmpChar;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    if(str_str((char *)pSupportStr,"service")!=0)
	{
		gBtAppHfServiceSupport[ucDevNo] = 1;
	}
	else
	{
		gBtAppHfServiceSupport[ucDevNo] = 0;
	}
    if(str_str((char *)pSupportStr,"roam")!=0)
	{
		gBtAppHfRoamSupport[ucDevNo] = 1;
	}
	else
	{
		gBtAppHfRoamSupport[ucDevNo] = 0;
	}
    if(str_str((char *)pSupportStr,"signal")!=0)
	{
		gBtAppHfSignalSupport[ucDevNo] = 1;
	}
	else
	{
		gBtAppHfSignalSupport[ucDevNo] = 0;
	}
    if(str_str((char *)pSupportStr,"battchg")!=0)
	{
		gBtAppHfBattchgSupport[ucDevNo] = 1;
	}
	else
	{
		gBtAppHfBattchgSupport[ucDevNo] = 0;
	}	
    if(str_str((char *)pSupportStr,"message")!=0)
	{
		gBtAppHfMessageSupport[ucDevNo] = 1;
	}
	else
	{
		gBtAppHfMessageSupport[ucDevNo] = 0;
	}
	
	for(CindSupportCount = 0; CindSupportCount<20; CindSupportCount++)
	{
		CindSupportStr[CindSupportCount] = 0;
		CindSupportValue[CindSupportCount] = 0;
	}

	currentStr = pSupportStr;

	for(CindSupportCount = 0; CindSupportCount<20; CindSupportCount++)
	{
        if(str_str((char *)currentStr,"(\"")!=0)
		{
            CindSupportStr[CindSupportCount] = (unsigned char *)str_str((char *)currentStr,"(\"") + 2;
            tmpChar=(unsigned char *)str_str((char *)CindSupportStr[CindSupportCount],"\"");
			if(tmpChar!=NULL)
			{
				*tmpChar='\0';	
				currentStr = tmpChar+1;
			}
			else
			{
				currentStr = CindSupportStr[CindSupportCount];
			}
			CindSupportValue[CindSupportCount] = *(pSupportValue+(CindSupportCount*2)) -'0';
		}
		else
		{
			break;
		}
	}

	for(CindSupportCount = 0; CindSupportCount<20; CindSupportCount++)
	{
		if(!CindSupportStr[CindSupportCount])
		{
			break;
		}
		
		if(mem_cmp(CindSupportStr[CindSupportCount], "service", 8)==0)
		{
			BtApp_HF_CIEVINDServiceHandler(CindSupportValue[CindSupportCount]);
		}
		else if(mem_cmp(CindSupportStr[CindSupportCount], "call", 5)==0)
		{
			if(CindSupportValue[CindSupportCount]!=0)
			{
				BtApp_HF_CIEVINDCallHandler(CindSupportValue[CindSupportCount]);
			}
		}
		else if((mem_cmp(CindSupportStr[CindSupportCount], "callsetup", 10)==0)
				|| (memcmp(CindSupportStr[CindSupportCount],"call setup",11)==0)
				|| (memcmp(CindSupportStr[CindSupportCount],"call_setup",11)==0))
		{
			if(CindSupportValue[CindSupportCount]!=0)
			{
				BtApp_HF_CIEVINDCallsetupHandler(CindSupportValue[CindSupportCount]);
			}
		}
		else if(mem_cmp(CindSupportStr[CindSupportCount], "callheld", 8)==0)
		{
			if(CindSupportValue[CindSupportCount]!=0)
			{
				BtApp_HF_CIEVINDCallheldHandler(CindSupportValue[CindSupportCount]);
			}
		}		
		else if(mem_cmp(CindSupportStr[CindSupportCount], "signal", 7)==0)
		{
			BtApp_HF_CIEVINDSingalHandler(CindSupportValue[CindSupportCount]);
		}
		else if(mem_cmp(CindSupportStr[CindSupportCount], "roam", 5)==0)
		{
			BtApp_HF_CIEVINDRoamHandler(CindSupportValue[CindSupportCount]);
		}
		else if(mem_cmp(CindSupportStr[CindSupportCount], "battchg", 8)==0)
		{
			BtApp_HF_CIEVINDBattchgHandler(CindSupportValue[CindSupportCount]);
		}
		else
		{
			/* none */
		}
	}
#endif
}

/*************************************************************************************
*  BtApp_HF_CmeeIndHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CmeeIndHandler(unsigned char cmeeErrorCode)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfCmee[ucDevNo] = cmeeErrorCode;

    BtApp_HF_EvtCmeeInd(ucDevNo);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDCallHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDCallHandler(unsigned long ind)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
    printf("[%s : %05d] Info : BtApp_HF_CIEVINDCallHandler : %x,%d,%d\n",__FILENAME__,__LINE__,G_BtSys->gBtAppSysCurStatus[BTAPP_STATUS_GROUP_HFx+ucDevNo],ind,ucDevNo);

	if (ind == 2)
	{
		/* If BtApp_HF_TimeoutCallEnd() use this function, */
		/* just carry out case 2:. */
		;
	}
	else
	{
#if 1 /* Telechips' 2008.12 QA - 3.19 */
		//if (ind == gBtAppHfCallStatus[ucDevNo])
		//	return;
		//else
			gBtAppHfCallStatus[ucDevNo] = ind;
#endif
	}

	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_END_TIMEOUT_ID,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,ucDevNo));

	/* Enhanced call timer cancelation */	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ALL_HELD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_HOLD_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_ADD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CONNECT_TWO_CALLS,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_CALL_HELD_TWO,ucDevNo));
 
	switch(ind)
	{
		case 0:
		case 2:
			/* call ind 2 : BtApp_HF_TimeoutCallEnd() position															*	
			 * Need to reset call, callsetup, call held here, due to auto enhanced call control							*
			 * Model : IM-A830S make callend timeout. Cannot get 2nd call at HFP when terminate 1st call at HFP			*/
			gBtAppHfCallStatus[ucDevNo]=-1;
			gBtAppHfCallsetupStatus[ucDevNo]=-1;
			gBtAppHfCallheldStatus[ucDevNo]=-1;	
			gBtAppHfCallclccStatus[ucDevNo]=-1;
			gBtAppHfCallclccPrevStatus[ucDevNo]=-1;
			
			TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo));

            if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=FALSE)
			{
                BtApp_HF_EvtIncomingEndInd(ucDevNo);
			}
			
            if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
			{
                (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
                BtApp_HF_EvtOutgoingEndInd(ucDevNo);
			}
			
			if(BtApp_HF_VoiceRecognition()!=FALSE)
			{
				BtApp_HF_VoiceRecognitionOnOff();
			}
			
            if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
			{
                BtApp_HF_EvtCievCallZeroInd(ucDevNo);
			}
			break;
		case 1:
            if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=FALSE)
			{
				TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RING_ID,ucDevNo)); //BtApp_HF_SetIRingEnd();
			}
			
            if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
			{
                (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
			}
            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL));

            if(G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
			{
                BtApp_HF_EvtCievCallOneInd(ucDevNo);
			}
			break;
		default:
			break;
	}
}

/*************************************************************************************
*  BtApp_HF_CIEVINDCallsetupHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDCallsetupHandler(unsigned long ind)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();


	if (ind == 4)
	{
		/* If BtApp_HF_TimeoutOutgoingEnd() use this function, */
		/* just carry out (ind == 4) */
		;
	}
	else
	{
#if 1 /* Telechips' 2008.12 QA - 3.19 */
		//if (ind == gBtAppHfCallsetupStatus[ucDevNo])
		//	return;
		//else
			gBtAppHfCallsetupStatus[ucDevNo] = ind;
#endif
	}

	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_END_TIMEOUT_ID,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,ucDevNo));

	/* Enhanced call timer cancelation */	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ALL_HELD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_HOLD_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_ADD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CONNECT_TWO_CALLS,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_CALL_HELD_TWO,ucDevNo));

	if((ind == 0) || (ind == 4)) 
	{
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))==FALSE)
		{
			TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo));
		}
		
        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=FALSE)
		{		
            BtApp_HF_EvtIncomingEndInd(ucDevNo);
		}
		
        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
		{
            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
            if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))==FALSE)
			{
                BtApp_HF_EvtOutgoingEndInd(ucDevNo);
			}
			else
			{
                BtApp_HF_EvtCievCallOneInd(ucDevNo);
			}
		}
	}
	else if(ind == 1) {
		if(BtApp_HF_VoiceRecognition()!=FALSE)
		{
			BtApp_HF_VoiceRecognitionOnOff();
		}
		
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		//else
		{
			//if(BtApp_HF_InbandRing()!=FALSE)		 /* Empty if Statement */
#endif
			{
				#if 0 //def  BT_HFP_NOKIA_SECOND_CALL
				if( 0
				|| BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))
				|| BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))
				
				//|| BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_CALL,BtApp_HF_GetSelectedDevNum()))
				//|| BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_OUTGOING,BtApp_HF_GetSelectedDevNum()))
	//				|| BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_HF_SELECTED_CALL)
	//				|| BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_HF_SELECTED_OUTGOING)
				)
				{
					;
				}
				else
				{
					/* in-band ring */
					//BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
					gBtAppHfInbandRingType[ucDevNo] = 0;	/* 1: ACL, 0: SCO */ /* To support A2DP In-band ring : Motorola RAZR V3X */ 		
					//BtApp_HF_EvtIncomingInd(ucDevNo);
					//UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_SET_HFP(BT_LCDUPDATE_HFx_INBAND_RING_START_IND,BtApp_HF_GetCurrDevNum())); //BtApp_HF_SetCallStart(BLUETOOTH_HF_RING_ID); /* Telechips' 2009.08 QA - Ver. 6.24 */
 				}
				#else
				//gBtAppHfInbandRingType[ucDevNo] = 0;	/* 1: ACL, 0: SCO */ /* To support A2DP In-band ring : Motorola RAZR V3X */			
 				#endif
			}

			gBtAppHfInbandRingType[ucDevNo] = 0;	/* 1: ACL, 0: SCO */ /* To support A2DP In-band ring : Motorola RAZR V3X */			

			BtApp_HF_CallRingINDHandler();	// Does this function needs in here?

#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		}
#endif		
	}
	else if(ind == 2) {
		if(BtApp_HF_VoiceRecognition()!=FALSE)
		{
			BtApp_HF_VoiceRecognitionOnOff();
		}
		
        if(G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
		{
			BtApp_HF_SetOutgoingStart();
		}
		
		TCCAPI_Timer_SetTime(
			TIMER_BT_OUTGOING_END_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_END_TIMEOUT_ID,ucDevNo),
			BtApp_HF_TimeoutOutgoingEnd);
 	}
	else if(ind == 3) {
		if(BtApp_HF_VoiceRecognition()!=FALSE)
		{
			BtApp_HF_VoiceRecognitionOnOff();
		}
		
        if(G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
		{
 			BtApp_HF_SetOutgoingStart();
 		}
	}
	else
	{
		/* none */
	}
	BtApp_HF_SetCallsetupIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDCallheldHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDCallheldHandler(unsigned long ind)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    printf("[%s : %05d] Info : BtApp_HF_CIEVINDCallheldHandler : %x,%d,%d\n",__FILENAME__,__LINE__,G_BtSys->gBtAppSysCurStatus[BTAPP_STATUS_GROUP_HFx+ucDevNo],ind,ucDevNo);
	
    gBtAppHfCallheldStatus[ucDevNo] = ind;

	/* Enhanced call timer cancelation */	
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ALL_HELD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_HOLD_ACTIVE_ACCEPT,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_ADD_CALL,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CONNECT_TWO_CALLS,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_CALL_HELD_TWO,ucDevNo));	

	switch(ind)
	{
		case 1 :
			TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,ucDevNo));			
			break;
		case 2 :
			//TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CALL_END_TIMEOUT_ID,ucDevNo));			
			break;
		default :
			break;
	}
	
    BtApp_HF_EvtCievCallheldInd(ucDevNo);
}

/*************************************************************************************
*  BtApp_HF_CLCCINDCallheldHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CLCCINDCallheldHandler(unsigned long ind)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    printf("BtApp_HF_CLCCINDCallheldHandler : %x,%d,%d\n", G_BtSys->gBtAppSysCurStatus[BTAPP_STATUS_GROUP_HFx+ucDevNo],ind,ucDevNo);
	
	switch(ind)
	{
		case 2 :
			TCCAPI_Timer_SetTime(TIMER_BT_CLCC_CALL_HELD_TIMEOUT_TIME,
				TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_CALL_HELD_TWO,ucDevNo),
				BtApp_HF_TimeoutCLCCINDCallheldTwo);
			break;
		default :
			break;
	}
}

void BtApp_HF_RingThroughA2dpStartINDHandler(TcBtBdAddr *pBdAddr)
{
	unsigned long ulInbandRing = FALSE;
	char cStatusRing = FALSE;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	ulInbandRing = BtApp_HF_InbandRing();
    cStatusRing = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
	
	if((ulInbandRing!=FALSE) && (cStatusRing!=FALSE))
	{
		//int iDevNo = TC_BT_HF_DEV_GetNum(pBdAddr);

		//if(iDevNo<0)	// invalid index
		//{
		//	return;
		//}
		
		gBtAppHfInbandRingType[ucDevNo] = 1;	/* 1: ACL, 0: SCO */ /* To support A2DP In-band ring : Motorola RAZR V3X */
        BtApp_HF_EvtIncomingInd(ucDevNo);
	}
}

void BtApp_HF_RingThroughA2dpSuspendINDHandler(const TcBtBdAddr *pBdAddr)
{
	unsigned long ulInbandRing = FALSE;
	char cStatusRing = FALSE;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	ulInbandRing = BtApp_HF_InbandRing();
    cStatusRing = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
	
	if((ulInbandRing!=FALSE) && (cStatusRing!=FALSE))
	{
		//int iDevNo = TC_BT_HF_DEV_GetNum(pBdAddr);

		//if(iDevNo<0)	// invalid index
		//{
		//	return;
		//}

		if(gBtAppHfInbandRingType[ucDevNo]==1)
		{
			gBtAppHfInbandRingType[ucDevNo] = 0;	/* 1: ACL, 0: SCO */ /* To support SCO In-band ring : Nokia N9 */
            BtApp_HF_EvtIncomingInd(ucDevNo);
		}
	}
}

/*************************************************************************************
*  BtApp_HF_CIEVINDServiceHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDServiceHandler(unsigned long ind)
{
	BtApp_HF_SetServiceIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDRoamHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDRoamHandler(unsigned long ind)
{
	BtApp_HF_SetRoamIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDSingalHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDSingalHandler(unsigned long ind)
{
	BtApp_HF_SetSignalIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDBattchgHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDBattchgHandler(unsigned long ind)
{
	BtApp_HF_SetBattchgIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CIEVINDMsgRcvdHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CIEVINDMsgRcvdHandler(unsigned long ind)
{
	BtApp_HF_SetMsgRcvdIndex(ind);
}

/*************************************************************************************
*  BtApp_HF_CallRingINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CallRingINDHandler(void)
{
	char cStatusCall = FALSE;
	char cStatusOutgoing = FALSE;
	
	printf("[%s : %05d] Info : BtApp_HF_CallRingINDHandler \n",__FILENAME__,__LINE__);

    cStatusCall = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL));		/* NEC N750, N850 */
    cStatusOutgoing = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING));

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))==FALSE /* Jolla Sailfish 2014 UPF */)
	{
		return;
	}

	if((cStatusCall!=FALSE)	|| (cStatusOutgoing!=FALSE))		
	{
		return;
	}
	
	printf("[%s : %05d] Info : BtApp_HF_CallRingINDHandler : BTAPP_STATUS_HFx_RING\n",__FILENAME__,__LINE__);
	
    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
	BtApp_HF_SetRing();
	BtApp_HF_AutoAnswerHandler();	
}

/*************************************************************************************
*  BtApp_HF_AutoAnswerHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_AutoAnswerHandler(void)
{
	unsigned char ucDevNo;
	char cStatusCall = FALSE;
	char cStatusOutgoing = FALSE;

	printf("[%s : %05d] Info : BtApp_HF_AutoAnswerHandler\n",__FILENAME__,__LINE__);

    cStatusCall = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL));		/* NEC N750, N850 */
    cStatusOutgoing = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING));

	if((cStatusCall!=FALSE) || (cStatusOutgoing!=FALSE))
	{
		return;
	}

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(gBtAppHfAutoAnswerDelay[ucDevNo]!=0)
	{
		if(gBtAppHfAutoAnswerStart[ucDevNo]==0)
		{
			printf("[%s : %05d] Info : BtApp_HF_AutoAnswerHandler: %d\n",__FILENAME__,__LINE__,gBtAppHfAutoAnswerStart[ucDevNo]);
			
			gBtAppHfAutoAnswerStart[ucDevNo] = 1;
			TCCAPI_Timer_SetTime(gBtAppHfAutoAnswerDelay[ucDevNo] * 10,
				TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_AUTO_ANSWER_ID,ucDevNo),
				BtApp_HF_AutoAnswer);
		}
	}
}

/*************************************************************************************
*  BtApp_HF_ClipTokenINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_ClipTokenINDHandler(const char *pStrCallLineID)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	printf("[%s : %05d] Info : BtApp_HF_ClipTokenINDHandler\n",__FILENAME__,__LINE__);
	
    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));

	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PRIVATE_NUMBER_ID,ucDevNo));
	
	BtApp_HF_SetCallLineID(pStrCallLineID);

    BtApp_HF_EvtClipInd(ucDevNo);
}

/*************************************************************************************
*  BtApp_HF_CLCCToken
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
static void BtApp_HF_CLCCToken(char *pStrOutgoingNumber, int len)
{
#if 1 /* Telechips' 2008.12 QA - 3.6 (a), (b) */
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		if((pStrOutgoingNumber == NULL) || (len == 0))
		{
	 		BtApp_HF_SetOutgoing_CID_NotSupport();
		}
		else
		{
			BtApp_HF_SetOutgoingNumber(pStrOutgoingNumber, len);
		}
	}
	else
	{
		if((pStrOutgoingNumber == NULL) || (len == 0))
		{
	 		BtApp_HF_SetCallLineID_Private_Number();
		}
		else
		{
			BtApp_HF_SetCallLineID(pStrOutgoingNumber);
		}
	}
#else
	BtApp_HF_SetOutgoingNumber(pStrOutgoingNumber, len);
#endif
}

void BtApp_HF_CLCCTokenINDHandler(unsigned char idx,unsigned char status,char *pStrOutgoingNumber, int len)
{
	static unsigned char boStatus=0;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfCallheldStatus[ucDevNo]==2)
	{
		if((idx==1/*first*/) || ((boStatus==1) || (boStatus==6)))
		{
			boStatus=status;
			BtApp_HF_CLCCToken(pStrOutgoingNumber, len);
		}
	}
	else
	{
#if 0	/* Unreachable Call */
		if((status == 4) || (status == 5)) 		/* incoming */
		{
			/* 
				3-way call : refer to TC_BT_HF_CallWaitingNotificationInd 
				note) remove the below call condition to get all types of call numbers.
			*/
			if(0/*BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE*/)
			{
				BtApp_HF_CLCCToken(pStrOutgoingNumber, len);
			}
		}
		else if((status == 0) || (status == 2)  || (status == 3)) 	
		{
			BtApp_HF_CLCCToken(pStrOutgoingNumber, len);
		}
		else
		{
			/* none */
		}
#else
		if((status == 0) || (status == 2)  || (status == 3)) 	
		{
			BtApp_HF_CLCCToken(pStrOutgoingNumber, len);
		}
		else
		{
			/* none */
		}
#endif		
	}
}

/*************************************************************************************
*  BtApp_HF_CLCCTokenCFMHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CLCCTokenCFMHandler(int bIndResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		if(bIndResult==TC_BT_CME_AG_FAILURE)
		{
 			BtApp_HF_SetOutgoing_CID_NotSupport();
 		}
 		
        BtApp_HF_EvtOutgoingNumberInd(ucDevNo);
	}
	else
	{
		if(bIndResult==TC_BT_CME_AG_FAILURE)
		{
	 		BtApp_HF_SetCallLineID_Private_Number();
		}
	 	
        BtApp_HF_EvtClccInd(ucDevNo);
	}
}

/*************************************************************************************
*  BtApp_HF_CCWATokenINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CCWATokenINDHandler(char *pStrCcwaNumber)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(pStrCcwaNumber == NULL)
	{
 		BtApp_HF_SetCcwaNumberID_Private_Number();
	}
	else
	{
		int nLength=0;
	
		nLength = strlen(pStrCcwaNumber);
		if(nLength>=BTAPP_HF_CALLNUM_MAX_LEN)
		{
			nLength=BTAPP_HF_CALLNUM_MAX_LEN-1;
		}

		if(nLength == 0)
		{
			BtApp_HF_SetCcwaNumberID_Private_Number();
		}
		else
		{	
			BtApp_HF_SetCcwaNumberID(pStrCcwaNumber);
		}
	}

    BtApp_HF_EvtCcwaInd(ucDevNo);
}

/*************************************************************************************
*  BtApp_HF_COPSTokenINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_COPSTokenINDHandler(char *pStrNetworkOperator, int len)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfStrNetworkOperator[ucDevNo]!=NULL)
	{
        TC_BT_Pfree(gBtAppHfStrNetworkOperator[ucDevNo]);
		gBtAppHfStrNetworkOperator[ucDevNo] = NULL;
	}

   gBtAppHfStrNetworkOperator[ucDevNo] = (char *)TC_BT_Pmalloc(len);

	(void)sprintf(gBtAppHfStrNetworkOperator[ucDevNo], "%s", pStrNetworkOperator);
}

/*************************************************************************************
*  BtApp_HF_CindSupportTokenINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_CindSupportTokenINDHandler(char *pStrCindSupport, int len)
{
	BtApp_HF_SetCindSupport(pStrCindSupport, len);
}

/*************************************************************************************
*  BtApp_HF_SpkVolumeINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SpkVolumeINDHandler(unsigned char HfSpkVol)
{
	BtApp_HF_SetSpkVolumeIndex(HfSpkVol);
}

/*************************************************************************************
*  BtApp_HF_MicGainINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_MicGainINDHandler(unsigned char HfMicGain)
{
	BtApp_HF_SetMicGainIndex(HfMicGain);
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionLearningToggle
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_VoiceRecognitionLearningToggle(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfVoiceRecognitionLearning[ucDevNo]!=FALSE)
	{
		gBtAppHfVoiceRecognitionLearning[ucDevNo] = FALSE;
	}
	else
	{
		gBtAppHfVoiceRecognitionLearning[ucDevNo] = TRUE;
	}
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionLearning
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_VoiceRecognitionLearning(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfVoiceRecognitionLearning[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_VoiceRecognitionINDHandler(unsigned long ind)
{
#ifdef BT_HFP_EXCLUDE_VOICE_RECOGNITION
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfVoiceRecognition[ucDevNo] = ind;

	if(gBtAppHfVoiceRecognition[ucDevNo]!=FALSE)
	{
        BtApp_HF_EvtVoiceRecognitionStartInd(ucDevNo);
	}
	else
	{
        BtApp_HF_EvtVoiceRecognitionEndInd(ucDevNo);
	}

	//UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_VOICE_RECOGNITION_IND);
#endif
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionAbility
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_VoiceRecognitionAbility(void)
{
#ifdef BT_HFP_EXCLUDE_VOICE_RECOGNITION
	return 0;
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfVoiceRecognitionAbility[ucDevNo];
#endif
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionAbilityInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_VoiceRecognitionAbilityInd(unsigned long indResult)
{
#ifdef BT_HFP_EXCLUDE_VOICE_RECOGNITION
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfVoiceRecognitionAbility[ucDevNo] = indResult;

    BtApp_HF_EvtSupportVoiceRecognitionInd(ucDevNo);
#endif
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognition
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_VoiceRecognition(void)
{
#ifdef BT_HFP_EXCLUDE_VOICE_RECOGNITION
	return 0;
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfVoiceRecognition[ucDevNo];
#endif
}

/*************************************************************************************
*  BtApp_HF_VoiceRecognitionInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_VoiceRecognitionInd(unsigned long indResult)
{
#ifdef BT_HFP_EXCLUDE_VOICE_RECOGNITION
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfVoiceRecognition[ucDevNo] = indResult;
#endif
}

/*************************************************************************************
*  BtApp_HF_InbandRingINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_InbandRingINDHandler(unsigned long ind)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfInbandRing[ucDevNo] = ind;
}

/*************************************************************************************
*  BtApp_HF_InbandRing
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_InbandRing(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfInbandRingPriority[ucDevNo]!=FALSE)
	{
		return gBtAppHfInbandRing[ucDevNo];
	}
	else
	{
		return 0;
	}
}

/*************************************************************************************
*  BtApp_HF_InbandRingPriority
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_InbandRingPriority(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfInbandRingPriority[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_InbandRingChangedInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_InbandRingChangedInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	printf("[%s : %05d] Info : BtApp_HF_InbandRingChangedInd : %d\n",__FILENAME__,__LINE__,indResult);

	gBtAppHfInbandRing[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_RejectAbility
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_RejectAbility(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfRejectAbility[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_RejectAbilityInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_RejectAbilityInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfRejectAbility[ucDevNo] = indResult;
}

void BtApp_HF_UpdateSupportedCodecCFMHandler(unsigned char bCfmResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
    BtApp_HF_EvtUpdateSupportedCodec(ucDevNo);
}

void BtApp_HF_DialCFMHandler(unsigned long CfmResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(CfmResult != TC_BT_CME_SUCCESS)
	{
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
        BtApp_HF_EvtOutgoingEndInd(ucDevNo);
	}
}

void BtApp_HF_CallAnswerCFMHandler(unsigned long CfmResult)
{
}

void BtApp_HF_CallEndCFMHandler(unsigned long CfmResult)
{
}

#ifdef BT_HFP_WIDEBAND_SPEECH
/*************************************************************************************
*  BtApp_HF_SetCodecInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SetCodecInd(unsigned long indResult)
{
#if 1	/* Telechips Linux HF remark */
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_SetCodecInd : %d\n",__FILENAME__,__LINE__,indResult);

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfCodec[ucDevNo]= indResult;

    BtApp_HF_EvtSelectedCodecInd(ucDevNo);
#endif
}

/*************************************************************************************
*  BtApp_HF_GetCodecInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_GetCodecInd(void)
{
	unsigned char ucDevNo;
	
	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfCodec[ucDevNo];
}
#endif

/*************************************************************************************
*  BtApp_HF_SupportChld
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportChld(int index)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if ((gBtAppHfSupportChld[ucDevNo] & (1 << index)) !=0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************************
*  BtApp_HF_SupportChldInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportChldInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if (indResult==0xFF)
	{
		gBtAppHfSupportChld[ucDevNo] = 0;
	}
	else
	{
		gBtAppHfSupportChld[ucDevNo] |= 1 << indResult;
	}
}

/*************************************************************************************
*  BtApp_HF_SupportThreeWayCalling
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportThreeWayCalling(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportThreeWayCalling[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportThreeWayCallingInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportThreeWayCallingInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportThreeWayCalling[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_SupportEcNrFunction
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportEcNrFunction(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportEcNrFunction[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportEcNrFunctionInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportEcNrFunctionInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportEcNrFunction[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_SupportEnhancedCallStatus
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportEnhancedCallStatus(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportEnhancedCallStatus[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportEnhancedCallStatusInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportEnhancedCallStatusInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportEnhancedCallStatus[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_SupportEnhancedCallControl
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportEnhancedCallControl(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportEnhancedCallControl[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportEnhancedCallControlInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportEnhancedCallControlInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportEnhancedCallControl[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_SupportExtendedErrorCodes
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_SupportExtendedErrorCodes(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportExtendedErrorCodes[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportExtendedErrorCodesInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportExtendedErrorCodesInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportExtendedErrorCodes[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_SupportCodecNegotiation
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long  BtApp_HF_SupportCodecNegotiation(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfSupportCodecNegotiation[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_SupportCodecNegotiationInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_SupportCodecNegotiationInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfSupportCodecNegotiation[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_RemoteVersion
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned long BtApp_HF_RemoteVersion(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    return gBtAppHfRemoteVersion[ucDevNo];
}

/*************************************************************************************
*  BtApp_HF_RemoteVersionInd
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_RemoteVersionInd(unsigned long indResult)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfRemoteVersion[ucDevNo] = indResult;
}

/*************************************************************************************
*  BtApp_HF_EchoOnOff
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_EchoOnOff(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfEchoOnOffStatus[ucDevNo]==TRUE)
	{
		BtApp_HF_EchoOff();
	}
	else
	{
		BtApp_HF_EchoOn();
	}
}

//#endif  //Yootest 20090220

/*************************************************************************************
*  BtApp_HF_EchoOn
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_EchoOn(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	//BtApp_SYS_Protect_Timer();

	gBtAppHfEchoOnOffStatus[ucDevNo] = TRUE;
    (void)G_BtHfCmd->TC_BT_HF_SetEchoAndNoise(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfEchoOnOffStatus[ucDevNo]);

    BtApp_HF_EvtOptionsNRECInf(ucDevNo);

	//BtApp_SYS_Unprotect_Timer();
}

/*************************************************************************************
*  BtApp_HF_Timer_EchoOff
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_EchoOff(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	//BtApp_SYS_Protect_Timer();

	gBtAppHfEchoOnOffStatus[ucDevNo] = FALSE;
    (void)G_BtHfCmd->TC_BT_HF_SetEchoAndNoise(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfEchoOnOffStatus[ucDevNo]);

    BtApp_HF_EvtOptionsNRECInf(ucDevNo);

	//BtApp_SYS_Unprotect_Timer();
}

void BtApp_HF_Timer_EchoOff(unsigned char id, unsigned char loc)
{
	BtApp_HF_EchoOff();
}

/*************************************************************************************
*  BtApp_HF_EnableCMEE
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_EnableCMEE(unsigned char id, unsigned char loc)
{	
	if(BtApp_HF_SupportExtendedErrorCodes()!=0)
	{
        (void)G_BtHfCmd->TC_BT_HF_SetExtendedAgErrorResultCode(BtApp_HF_GetSelectedDevBdAddr(), TRUE);
	}
}

/*************************************************************************************
*  BtApp_HF_GetCops
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_GetCops(unsigned char id, unsigned char loc)
{	
    (void)G_BtHfCmd->TC_BT_HF_GetCurrentOperatorSelection(BtApp_HF_GetSelectedDevBdAddr());
}

int BtApp_HF_ScoDataCallback(const TcBtBdAddr *pBdAddr, char * theData)
{

}

void BtApp_HF_GenerateDtmf(unsigned char dtmf)
{
    (void)G_BtHfCmd->TC_BT_HF_GenerateDtmf(BtApp_HF_GetSelectedDevBdAddr(), dtmf);
}

/*****************************************************************************
* Function Name : BtApp_HF_ActivateHandsfree()
******************************************************************************
* Desription  :
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ActivateHandsfree(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	
#if 1 /* Telechips' 2008.12 QA - 1.6, 1.7, 3.4 */

    if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE)!=FALSE )
	{
        BtApp_HF_EvtAlreadyActivated(ucDevNo);
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_Activate();
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_DeactivateHandsfree()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_DeactivateHandsfree(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	
#if 1 /* Telechips' 2008.12 QA - 1.6, 1.7, 3.4 */

    if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE)!=FALSE )
	{
		unsigned char ucTmpDevNo;
		
		for(ucTmpDevNo=0;ucTmpDevNo<BT_HFP_MAX_CONNECTIONS;ucTmpDevNo++)
		{
            if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_CONNECT,ucTmpDevNo))!=FALSE )
			{
                if(G_BtHfCmd->TC_BT_HF_Disconnect((const TcBtBdAddr *)BtApp_HF_GetSelectedDevBdAddr()) == TC_BT_CMD_RESULT_SUCCESS_ENUM)
				{
					(void)BtApp_GAP_SetHfpState((const TcBtBdAddr *)BtApp_HF_GetSelectedDevBdAddr(), BTAPP_GAP_STATE_DISCONNECTING);
				}
                //TC_TimeDly(50);
                usleep(250000);
			}
		}
		
        if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE)!=FALSE )
		{
            (void)G_BtHfCmd->TC_BT_HF_Deactivate();
		}
	}
	else
	{
        BtApp_HF_EvtNotActivated(ucDevNo);
	}

#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_ConnectHandsfree()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ConnectHandsfree(TcBtBdAddr *pBdAddr)
{
#if 1
        TcBtUint16 state;            
        TcBtUint32 fr = TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
        int ucDevNo = TC_BT_HF_DEV_GetNum(pBdAddr);//if bdaddr is invalid, return -1
        
        /* HF Activate is done only once, not related to device number, no need to use set status for multi-hf */
        if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE) != FALSE )
        {       
            state = BtApp_GAP_GetHfpState(pBdAddr, BTAPP_GAP_UNUSED_INST_IDX);
            
            if(state == BTAPP_GAP_STATE_DISCONNECTED)
            {
                fr = G_BtHfCmd->TC_BT_HF_ServiceConnect(pBdAddr);

                if(fr == TC_BT_CMD_RESULT_SUCCESS_ENUM)
                {
                    (void)BtApp_GAP_SetHfpState(pBdAddr, BTAPP_GAP_STATE_CONNECTING);
                    TC_BT_BdAddrZero(pBdAddr);
                }
                else
                {
                    printf("[%s : %05d] Info : BtApp_HF_ConnectHandsfree ServiceConnect failed\n",__FILENAME__,__LINE__);
                }
            }
            else
            {
                    if( ucDevNo < 0 )
                    {
                        printf("[%s : %05d] Info : BtApp_HF_ConnectHandsfree bdaddr is invalid\n",__FILENAME__,__LINE__);
                        return;
                    }
                    BtApp_HF_EvtAlreadyConnected(ucDevNo);
            }
        }
        else
        {
            if( ucDevNo < 0 )
            {
                printf("[%s : %05d] Info : BtApp_HF_ConnectHandsfree HF is deactivate and bd_addr is invalid\n",__FILENAME__,__LINE__);
                return;
            }
            BtApp_HF_EvtNotActivated(ucDevNo);
        }
         
#else
	unsigned char ucDevNo;
        
	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if( BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE)!=FALSE )
	{
		if( BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT) )==FALSE)
		{
			char cStatusActivate = FALSE;
			char cStatusConnect = FALSE;

			cStatusActivate = BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE);
			cStatusConnect = BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT));
			
			if((cStatusActivate!=FALSE) && (cStatusConnect==FALSE))
			{
#if 0 //reserved
				if (TC_BT_BdAddrEqZero(&gBtAppSysRemoteDevAddr))
					return;
				else
					TC_BT_HF_ServiceConnect((TcBtBdAddr * )&gBtAppSysRemoteDevAddr);
#else
				return;		
#endif
			}
		}
		else
		{
                    BtApp_HF_EvtAlreadyConnected(ucDevNo);
		}
	}
	else
	{
            BtApp_HF_EvtNotActivated(ucDevNo);
	}
#endif	
}

/*****************************************************************************
* Function Name : BtApp_HF_DisconnectHandsfree()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_DisconnectHandsfree(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	

#if 1 /* TEST */
#else
	gBTUserCommand =TC_BT_CMMD_HF_DISCONNECT;
#endif
    if (G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))!=FALSE)
	{
        if(G_BtHfCmd->TC_BT_HF_Disconnect((const TcBtBdAddr *)BtApp_HF_GetSelectedDevBdAddr()) == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			(void)BtApp_GAP_SetHfpState((const TcBtBdAddr *)BtApp_HF_GetSelectedDevBdAddr(), BTAPP_GAP_STATE_DISCONNECTING);
		}
	}
	else
	{
        BtApp_HF_EvtNotConnected(ucDevNo);
	}
}


/*****************************************************************************
* Function Name : BtApp_HF_Info_Ring()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Info_Ring(unsigned char *pboUnicode,unsigned char *pucCallLineID)
{
#if defined(BT_PBAP_CLIENT) 
	unsigned char* pStr = NULL;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
		(void)sprintf(gBtAppHfCallingLineID[ucDevNo], "              ");
		//BtApp_HF_SetCallLineID("\"\"");		

		/* Iphone 5 / incoming(in-band ring) / call id blank issue when private number
		   Iphone 5 does not send clip when ring, so this is private number position */
		if(TCCAPI_Timer_BT_IsActiveTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PRIVATE_NUMBER_ID,ucDevNo))==FALSE)
		{
			TCCAPI_Timer_SetTime(TIMER_BT_PRIVATE_NUMBER_TIMEOUT_TIME,
				TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PRIVATE_NUMBER_ID,ucDevNo), BtApp_HF_TimeoutHfPrivateNumber ); 
		}
	}

    if((strcmp((char *)CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo])==0)
        && (strlen((char *)CurrentCallEntryPBData.callNumber)==strlen(gBtAppHfCallingLineID[ucDevNo]))) /* latest call number */
	{
		if(CurrentCallEntryPBData.entryInPhonebook != 0) /* exist phonebook name */
		{
			memcpy(pucCallLineID, CurrentCallEntryPBData.phonebookName, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCallLineID, CurrentCallEntryPBData.callNumber, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = FALSE;
		}
	}
	else /* no latest call number */
	{
        pStr = (unsigned char *)BtApp_PB_SearchforHfCall(gBtAppHfCallingLineID[ucDevNo], &(CurrentCallEntryPBData.unicodeOrNot), 0);
		if(pStr != NULL) /* exist phonebook name */
		{
			memcpy(pucCallLineID, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.phonebookName, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 1;
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
            (void)TC_BT_Pmalloc(sizeof(pStr));
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCallLineID,gBtAppHfCallingLineID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 0;
			*pboUnicode = FALSE;
		}
	}
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	memcpy(pucCallLineID,gBtAppHfCallingLineID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
	//memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
	//CurrentCallEntryPBData.entryInPhonebook = 0;
	*pboUnicode = FALSE;
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Info_Ccwa()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Info_Ccwa(unsigned char *pboUnicode,unsigned char *pucCcwaNumberID)
{
#if defined(BT_PBAP_CLIENT) 
	unsigned char* pStr = NULL;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
    if((strcmp((char *)CurrentCallEntryPBData.callNumber, gBtAppHfCcwaNumberID[ucDevNo])==0)
        && (strlen((char *)CurrentCallEntryPBData.callNumber)==strlen(gBtAppHfCcwaNumberID[ucDevNo]))) /* latest call number */
	{
		if(CurrentCallEntryPBData.entryInPhonebook != 0) /* exist phonebook name */
		{
			memcpy(pucCcwaNumberID, CurrentCallEntryPBData.phonebookName, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCcwaNumberID, CurrentCallEntryPBData.callNumber, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = FALSE;
		}
	}
	else /* no latest call number */
	{
        pStr = (unsigned char *)BtApp_PB_SearchforHfCall(gBtAppHfCcwaNumberID[ucDevNo], &(CurrentCallEntryPBData.unicodeOrNot), 0);
		if(pStr != NULL) /* exist phonebook name */
		{
			memcpy(pucCcwaNumberID, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.phonebookName, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCcwaNumberID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 1;
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
            (void)TC_BT_Pmalloc(sizeof(pStr));
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCcwaNumberID,gBtAppHfCcwaNumberID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCcwaNumberID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 0;
			*pboUnicode = FALSE;
		}
	}
#else	
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	memcpy(pucCcwaNumberID,gBtAppHfCcwaNumberID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
	//memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCcwaNumberID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
	//CurrentCallEntryPBData.entryInPhonebook = 0;
	*pboUnicode = FALSE;
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Info_Outgoing()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Info_Outgoing(unsigned char *pboUnicode,unsigned char *pucCallNumber)
{
#if defined(BT_PBAP_CLIENT) 
	unsigned char* pStr = NULL;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    if((strcmp((char *)CurrentCallEntryPBData.callNumber, (char *)&gBtAppHfCallNumber[ucDevNo][0])==0)
        && (strlen((char *)CurrentCallEntryPBData.callNumber)==strlen((char *)&gBtAppHfCallNumber[ucDevNo][0]))) /* latest call number */
	{
		if(CurrentCallEntryPBData.entryInPhonebook != 0) /* exist phonebook name */
		{
			memcpy(pucCallNumber, CurrentCallEntryPBData.phonebookName, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCallNumber, CurrentCallEntryPBData.callNumber, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = FALSE;
		}
	}
	else /* no latest call number */
	{
        pStr = (unsigned char *)BtApp_PB_SearchforHfCall((char *)&gBtAppHfCallNumber[ucDevNo][0], &(CurrentCallEntryPBData.unicodeOrNot), 0);
		if(pStr != NULL) /* exist phonebook name */
		{
			memcpy(pucCallNumber, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.phonebookName, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, &gBtAppHfCallNumber[ucDevNo][0], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 1;
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
            (void)TC_BT_Pmalloc(sizeof(pStr));
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCallNumber,&gBtAppHfCallNumber[ucDevNo][0],BTAPP_HF_CALLNUM_MAX_LEN);
			memcpy(CurrentCallEntryPBData.callNumber, &gBtAppHfCallNumber[ucDevNo][0], BTAPP_HF_CALLNUM_MAX_LEN);
			CurrentCallEntryPBData.entryInPhonebook = 0;
			*pboUnicode = FALSE;
		}
	}
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	memcpy(pucCallNumber,&gBtAppHfCallNumber[ucDevNo][0],BTAPP_HF_CALLNUM_MAX_LEN);
	//mcpy(CurrentCallEntryPBData.callNumber, &gBtAppHfCallNumber[ucDevNo][0], BTAPP_HF_CALLNUM_MAX_LEN);
	//rrentCallEntryPBData.entryInPhonebook = 0;
	*pboUnicode = FALSE;
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Info_Call()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Info_Call(unsigned char *pboUnicode,unsigned char *pucCallNumber)
{
#if defined(BT_PBAP_CLIENT) 
	unsigned char boNewCallNumber;	
	unsigned char* pStr = NULL;	
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	boNewCallNumber=FALSE;
	
	if(gBtAppHfCallingLineID[ucDevNo][0] != '\0')
	{
        if(!((strcmp((char *)CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo])==0)
            && (strlen((char *)CurrentCallEntryPBData.callNumber)==strlen(gBtAppHfCallingLineID[ucDevNo]))))
		{
			boNewCallNumber=TRUE;
		}
	}
	else
	{
        if(!((strcmp((char *)CurrentCallEntryPBData.callNumber, (char *)&gBtAppHfCallNumber[ucDevNo][0])==0)
            && (strlen((char *)CurrentCallEntryPBData.callNumber)==strlen((char *)&gBtAppHfCallNumber[ucDevNo][0]))))
		{
			boNewCallNumber=TRUE;
		}
	}
	
	if(boNewCallNumber==FALSE) /* latest call number */
	{
		if(CurrentCallEntryPBData.entryInPhonebook != 0) /* exist phonebook name */
		{
			memcpy(pucCallNumber, CurrentCallEntryPBData.phonebookName, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
		}
		else /* not exist phonebook name */
		{
			memcpy(pucCallNumber, CurrentCallEntryPBData.callNumber, BTAPP_HF_CALLNUM_MAX_LEN);
			*pboUnicode = FALSE;
		}
	}
	else /* no latest call number */
	{
		if(gBtAppHfCallingLineID[ucDevNo][0] != '\0')
		{
            pStr = (unsigned char *)BtApp_PB_SearchforHfCall(gBtAppHfCallingLineID[ucDevNo], &(CurrentCallEntryPBData.unicodeOrNot), 0);
			if(pStr != NULL) /* exist phonebook name */
			{
				memcpy(pucCallNumber, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.phonebookName, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
				CurrentCallEntryPBData.entryInPhonebook = 1;
				*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
                (void)TC_BT_Pmalloc(sizeof(pStr));
			}
			else /* not exist phonebook name */
			{
				memcpy(pucCallNumber,gBtAppHfCallingLineID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.callNumber, gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_CALLNUM_MAX_LEN);
				CurrentCallEntryPBData.entryInPhonebook = 0;
				*pboUnicode = FALSE;
			}
		}
		else
		{
            pStr = (unsigned char *)BtApp_PB_SearchforHfCall((char *)&gBtAppHfCallNumber[ucDevNo][0], &(CurrentCallEntryPBData.unicodeOrNot), 0);
			if(pStr != NULL) /* exist phonebook name */
			{
				memcpy(pucCallNumber, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.phonebookName, pStr, BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.callNumber, &gBtAppHfCallNumber[ucDevNo][0], BTAPP_HF_CALLNUM_MAX_LEN);
				CurrentCallEntryPBData.entryInPhonebook = 1;
				*pboUnicode = CurrentCallEntryPBData.unicodeOrNot;
                (void)TC_BT_Pmalloc(sizeof(pStr));
			}
			else /* not exist phonebook name */
			{
				memcpy(pucCallNumber,&gBtAppHfCallNumber[ucDevNo][0],BTAPP_HF_CALLNUM_MAX_LEN);
				memcpy(CurrentCallEntryPBData.callNumber, &gBtAppHfCallNumber[ucDevNo][0], BTAPP_HF_CALLNUM_MAX_LEN);
				CurrentCallEntryPBData.entryInPhonebook = 0;
				*pboUnicode = FALSE;
			}
		}
	}
#else
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfCallingLineID[ucDevNo][0] != '\0')
	{
		memcpy(pucCallNumber,gBtAppHfCallingLineID[ucDevNo],BTAPP_HF_CALLNUM_MAX_LEN);
		*pboUnicode = FALSE;
	}
	else
	{
		memcpy(pucCallNumber,&gBtAppHfCallNumber[ucDevNo][0],BTAPP_HF_CALLNUM_MAX_LEN);
		*pboUnicode = FALSE;
	}
#endif

}

/*****************************************************************************
* Function Name : BtApp_HF_SetCall()
******************************************************************************
* Desription  : ucinitCallTime Default value at 800x is a TRUE, 
                8010 is selective due to Multi HF toggle mode
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCall(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	BtApp_HF_SetCallStart(BLUETOOTH_HF_CALL_ID); /* Telechips' 2009.08 QA - Ver. 6.24 */

    BtApp_HF_EvtCallTimeStampCountid(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetRing()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetRing(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
    BtApp_HF_EvtIncomingInd(ucDevNo);
	// UI Setting : RING

	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
		gBtAppHfCallingLineID[ucDevNo][0] ='\0';
	}

	TCCAPI_Timer_SetTime(TIMER_BT_RING_TIMEOUT_TIME,
		TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RING_ID,ucDevNo), BtApp_HF_TimeoutHfRing ); 
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallLineID()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCallLineID(const char *pStrCallLineID)
{
	int idx=0;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
		BtApp_HF_SetCallIdRcvd(TRUE);
	}

#if 0
	while (*pStrCallLineID != '\"')
	{
		pStrCallLineID++;
	}
#endif

	if(*pStrCallLineID == '\"')
	{
		if(pStrCallLineID[1] == '\"')
		{
			/* Private Number */
			(void)sprintf(gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_PRIVATE_NUMBER_STRING);
			return;
		}
		else
		{
			pStrCallLineID++;
		}
	}
	
	while ((*pStrCallLineID != '\r') && (idx <16))					// Save call line ID
	{
		if(*pStrCallLineID == '\"')
		{
			break;
		}
		
#if 1 /* Telechips' 2008.12 QA - 3.14 */

		if(*pStrCallLineID == ',')
		{
			break;
		}

#endif
		gBtAppHfCallingLineID[ucDevNo][idx] = *pStrCallLineID;
		idx++;
		pStrCallLineID++;
	}

	gBtAppHfCallingLineID[ucDevNo][idx] ='\0';
	// UI Setting : RING
//	BtApp_HF_SetRingStart();
#if 1 /* Telechips' 2008.12 QA - 3.6 (a), (b) */
#else
	UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_CLIP_IND);
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallLineID_Private_Number()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCallLineID_Private_Number(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
		BtApp_HF_SetCallIdRcvd(TRUE);
		/* Do not put private number copy function here!
  		   3-way calling 2nd call incoming private number 
  		   not updated after call one */
	}

	(void)sprintf(gBtAppHfCallingLineID[ucDevNo], BTAPP_HF_PRIVATE_NUMBER_STRING);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCcwaNumberID()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCcwaNumberID(const char *pStrCcwaNumberID)
{
	int idx=0;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

#if 0
	while (*pStrCallLineID != '\"')
	{
		pStrCallLineID++;
	}
#endif

	if(*pStrCcwaNumberID == '\"')
	{
		if(pStrCcwaNumberID[1] == '\"')
		{
			/* Private Number */
			(void)sprintf(gBtAppHfCcwaNumberID[ucDevNo], BTAPP_HF_PRIVATE_NUMBER_STRING);
			return;
		}
		else
		{
			pStrCcwaNumberID++;
		}
	}
	
	while ((*pStrCcwaNumberID != '\r') && (idx <16))					// Save call line ID
	{
		if(*pStrCcwaNumberID == '\"')
		{
			break;
		}
		
#if 1 /* Telechips' 2008.12 QA - 3.14 */

		if(*pStrCcwaNumberID == ',')
		{
			break;
		}

#endif
		gBtAppHfCcwaNumberID[ucDevNo][idx] = *pStrCcwaNumberID;
		idx++;
		pStrCcwaNumberID++;
	}

	gBtAppHfCcwaNumberID[ucDevNo][idx] ='\0';
	// UI Setting : RING
//	BtApp_HF_SetRingStart();
#if 1 /* Telechips' 2008.12 QA - 3.6 (a), (b) */
#else
	UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_CLIP_IND);
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCcwaNumberID_Private_Number()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCcwaNumberID_Private_Number(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	printf("[%s : %05d] Info : BtApp_HF_SetCcwaNumberID_Private_Number\n",__FILENAME__,__LINE__);

	(void)sprintf(gBtAppHfCcwaNumberID[ucDevNo], BTAPP_HF_PRIVATE_NUMBER_STRING);
}
/*****************************************************************************
* Function Name : BtApp_HF_SetOutgoingNumber()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetOutgoingNumber(const char *pStrOutgoingNumber, int len)
{
	int idx=1,numidx=0;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

#if 0
	while (*pStrOutgoingNumber != '\"')
	{
		pStrOutgoingNumber++;
	}
#endif

	if(*pStrOutgoingNumber == '\"')
	{
		if(pStrOutgoingNumber[1] == '\"')
		{
			BtApp_HF_SetOutgoing_CID_NotSupport();
			return;
		}
		else
		{
			pStrOutgoingNumber++;
		}
	}

	while ((idx < len) && (idx < 16))
	{
		if(*pStrOutgoingNumber == '\"')
		{
			break;
		}
		
#if 1 /* Telechips' 2008.12 QA - 3.14 */

		if(*pStrOutgoingNumber == ',')
		{
			break;
		}
#endif
		gBtAppHfCallNumber[ucDevNo][numidx] = *(unsigned char*)pStrOutgoingNumber;
		numidx++;
		pStrOutgoingNumber++;
	}

	gBtAppHfCallNumber[ucDevNo][numidx] = '\0';
	numidx++;
	gBtAppHfCallNumPos[ucDevNo] = numidx;
}

/*****************************************************************************
* Function Name : BtApp_HF_SetOutgoing_CID_NotSupport()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetOutgoing_CID_NotSupport(void)
{
#if 1	/* Telechips Linux HF remark */
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)sprintf((char *)&(gBtAppHfCallNumber[ucDevNo][0]), BTAPP_HF_UNKNOWN_NUMBER_STRING);
	gBtAppHfCallNumPos[ucDevNo] = 0 + strlen(BTAPP_HF_UNKNOWN_NUMBER_STRING) + 1;
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCindSupport()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCindSupport(char *pStrCindSupport, int len)
{
	/* TBD */
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallsetupIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCallsetupIndex(unsigned long ulCallsetupIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfCallsetupIndex[ucDevNo]=ulCallsetupIndex;

    BtApp_HF_EvtCievCallSetupInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetServiceIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetServiceIndex(unsigned long ulServiceIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(!gBtAppHfServiceUse[ucDevNo])
	{
		return;
	}

	if(!gBtAppHfServiceSupport[ucDevNo])
	{
		return;
	}

	gBtAppHfServiceIndex[ucDevNo]=ulServiceIndex;

    BtApp_HF_EvtCievServiceInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetRoamIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetRoamIndex(unsigned long ulRoamIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(!gBtAppHfRoamUse[ucDevNo])
	{
		return;
	}

	if(!gBtAppHfRoamSupport[ucDevNo])
	{
		return;
	}

	gBtAppHfRoamIndex[ucDevNo]=ulRoamIndex;

    BtApp_HF_EvtCievRoamInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetSignalIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetSignalIndex(unsigned long ulSignalIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(!gBtAppHfSignalUse[ucDevNo])
	{
		return;
	}

#if 1 /* Telechips' 2008.12 QA - 3.2 */

	if(!gBtAppHfSignalSupport[ucDevNo])
	{
		return;
	}

#endif
	gBtAppHfSignalIndex[ucDevNo]=ulSignalIndex;

    BtApp_HF_EvtCievSignalInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetBattchgIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetBattchgIndex(unsigned long ulBattchgIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(!gBtAppHfBattchgUse[ucDevNo])
	{
		return;
	}

#if 1 /* Telechips' 2008.12 QA - 3.2 */

	if(!gBtAppHfBattchgSupport[ucDevNo])
	{
		return;
	}

#endif
	gBtAppHfBattchgIndex[ucDevNo]=ulBattchgIndex;

    BtApp_HF_EvtCievBattChgInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetMsgRcvdIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetMsgRcvdIndex(unsigned long ulMsgRcvdIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(!gBtAppHfMessageUse[ucDevNo])
	{
		return;
	}

#if 1 /* Telechips' 2008.12 QA - 3.2 */

	if(!gBtAppHfMessageSupport[ucDevNo])
	{
		return;
	}

#endif
	gBtAppHfMessageIndex[ucDevNo]=ulMsgRcvdIndex;

    BtApp_HF_EvtCievMessageInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetSpkVolumeIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetSpkVolumeIndex(unsigned long ulSpkVolIndex)
{
	unsigned char ucDevNo;

    //TC_SchedLock();

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfSpkVolIndex[ucDevNo]=ulSpkVolIndex;

//	if ( ulSpkVolIndex > AECIf_GetSpeakerMaximumVolumeIndex())
//	{
//		ulSpkVolIndex = AECIf_GetSpeakerMaximumVolumeIndex();
//	}

	if(BtApp_HF_GetSelectedDevNum()==ucDevNo)
	{
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
        AECIf_SetSpeakerCurrentVolumeIndex(ulSpkVolIndex);
#endif
#endif
	}
    TcBtHFEvt_EvtBtDirectFourDataMessageSend(BT_HF_EVT_SET_SPEAKER_VOLUME_IND, ucDevNo, gBtAppHfSpkVolIndex[ucDevNo], gBtAppHfMicGainIndex[ucDevNo], 0);

	// [2011-11-1 yspark] spk vol index directly send msg to ui task
	//BtApp_HF_EvtSetSpeakerVolumeInd(ucDevNo);
	
    //TC_SchedUnlock();
}

/*****************************************************************************
* Function Name : BtApp_HF_SetMicGainIndex()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetMicGainIndex(unsigned long ulMicGainIndex)
{
	unsigned char ucDevNo;

    //TC_SchedLock();

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfMicGainIndex[ucDevNo]=ulMicGainIndex;

//	if ( ulMicGainIndex > AECIf_GetMicrophoneMaximumVolumeIndex())
//	{
//		ulMicGainIndex = AECIf_GetMicrophoneMaximumVolumeIndex();
//	}

	if(BtApp_HF_GetSelectedDevNum()==ucDevNo)
	{
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
        AECIf_SetMicrophoneCurrentVolumeIndex(ulMicGainIndex);
#endif
#endif
	}
    TcBtHFEvt_EvtBtDirectFourDataMessageSend(BT_HF_EVT_SET_MIC_GAIN_IND, ucDevNo, gBtAppHfSpkVolIndex[ucDevNo], gBtAppHfMicGainIndex[ucDevNo], 0);

	//BtApp_HF_EvtSetMicGainInd(ucDevNo);

    //TC_SchedUnlock();
}

/*****************************************************************************
* Function Name : BtApp_HF_SetMicMuteStatus()
******************************************************************************
* Desription  : 
* Parameter   :
* Return	  :
******************************************************************************/
void BtApp_HF_SetMicMuteStatus(void)
{
#if 0	/* Telechips Linux HF remark - porting */
	unsigned char ucDevNo;

	TC_SchedLock();

	ucDevNo=BtApp_HF_GetCurrDevNum();

	BtApp_HF_MicMuteToggle();
	
		
	// [2011-11-1 yspark] mic vol index directly send msg to ui task
	//UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_SET_MIC_GAIN_IND);
	//TcBtHFEvt_EvtBtDirectFourDataMessageSend(BT_HF_EVT_SET_MIC_MUTE_INF, ucDevNo, gBtAppHfMicMuteStatus[ucDevNo], 0, 0);
	BtApp_HF_EvtSetMicMuteInf(ucDevNo);
	TC_SchedUnlock();
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_TimeoutHfRing()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_TimeoutHfRing(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
    BtApp_HF_EvtIncomingTimeoutInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_TimeoutHfPrivateNumber()
******************************************************************************
* Desription  : if clip is not entered when incoming
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_TimeoutHfPrivateNumber(unsigned char id, unsigned char loc)
{
	//unsigned char ucDevNo;

	//ucDevNo=BtApp_HF_GetCurrDevNum();

	if(BtApp_HF_GetCallIdRcvd() == FALSE)
	{
		BtApp_HF_SetCallLineID_Private_Number();
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_Timer_CallTimeStamp()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Timer_CallTimeStamp(unsigned char id, unsigned char loc)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    gBtAppHfCallSec[ucDevNo]++;

    TcBtHFEvt_EvtBtFourDataMessageSend(BT_HF_EVT_CALL_TIMESTAMP_COUNT_ID, ucDevNo, gBtAppHfCallSec[ucDevNo], 0, 0);

    /* Linux Btapp DMP structure is different */
    //BtApp_HF_EvtCallTimeStampCountid(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetDial()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetDial(void)
{
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))==FALSE)
	{
#if 0	/* Telechips Linux HF remark */
		/* HF connection does not exist */
		UI_Event_Exit_MainMenu();
		UI_Event_DrawPlayScreen();
#endif
        BtApp_HF_EvtNotConnected(ucDevNo);
		return;
	}

	//BtApp_SYS_SetStatus(BTAPP_STATUS_MASK_HF_OUTGOING_BY_HF);
    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case BIDLE:
			break;
#if 0	/* Telechips Linux HF remark */
        case BPLAY:
			break;
		case BRADIO:
			break;
		case BRECORD:
			break;
		case BPLAYPAUSE:	
			break;
		case BDAB:
			break;
#endif
	#ifdef BT_AUDIO_SINK
		case B_BT_A2DP_SINK:
			break;
	#endif
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			break;
	#endif
		case B_BT_HFP_CALL:
			break;
		case B_BT_HSP_AUDIO:
			break;
#if 0	/* Telechips Linux HF remark */
		case BFF:
			break;
		case BREW:
			break;
		case BRECPAUSE:
			break;
#endif
	}

	gBtAppHfCallNumPos[ucDevNo] = 0;	/* reserved for Preset/Memory Dial '>' */
	memset(gBtAppHfCallNumber[ucDevNo], 0 , BTAPP_HF_CALLNUM_MAX_LEN);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetDialEnd()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetDialEnd(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfCallNumPos[ucDevNo] = 0;
	memset(gBtAppHfCallNumber[ucDevNo], 0 , BTAPP_HF_CALLNUM_MAX_LEN);
}

#if 0	/* Test Func */
static unsigned long BtApp_HF_GetKey(void)
{
#if 1 /* TEST */
	/* User Interface (display and mapping of key) was separated for headset */
#else
	if(UI_API_GetBluetoothStatus(BTAPP_STATUS_MASK_HS_CONNECT))
	{
		BtApp_HF_Answer();

		return FALSE;
	}
	else
#endif
	{
		return TRUE;
	}
}
#endif

void BtApp_HF_SetVoiceRecognition(void)
{
	unsigned char lBasicMode;

	if(!BtApp_HF_VoiceRecognitionAbility())
	{
#if 0	/* Telechips Linux HF remark */
		/* HFG does not support voice recognition */
		UI_Event_Exit_MainMenu();
		UI_Event_DrawPlayScreen();
		//UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_VOICE_RECOGNITION_ERR);
#endif
		return;
	}

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();

	switch(lBasicMode)
	{
		case BIDLE:
			break;
#if 0	/* Telechips Linux HF remark */
        case BPLAY:
			break;
		case BRADIO:
			break;
		case BRECORD:
			break;
		case BPLAYPAUSE:
			break;
		case BDAB:
			break;
#endif
	#ifdef BT_AUDIO_SINK
		case B_BT_A2DP_SINK:
			break;
	#endif
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			break;
	#endif
		case B_BT_HFP_CALL:
			break;
		case B_BT_HSP_AUDIO:
			break;
#if 0	/* Telechips Linux HF remark */
		case BFF:
			break;
		case BREW:
			break;
		case BRECPAUSE:
			break;
#endif
	}
}

void BtApp_HF_SetVoiceRecognitionLearning(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(BtApp_HF_VoiceRecognition() == TRUE)
	{
		return;
	}
	
	BtApp_HF_VoiceRecognitionLearningToggle();

	if(BtApp_HF_VoiceRecognitionLearning()!=0)
	{
        BtApp_HF_EvtVoiceRecognitionLearnStartInd(ucDevNo);
	}
	else
	{
        BtApp_HF_EvtVoiceRecognitionLearnEndInd(ucDevNo);
	}
}

void BtApp_HF_SetVoiceRecognitionOnOff(void)
{
	if(BtApp_HF_VoiceRecognitionLearning()!=0)
	{
		return;
	}

	BtApp_HF_VoiceRecognitionOnOff();
}

void BtApp_HF_TerminateVoiceRecognition(void)
{
	unsigned char ucDevNo;
	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	printf("[%s : %05d] Info : BtApp_HF_TerminateVoiceRecognition : %d\n",__FILENAME__,__LINE__,0);

	if(BtApp_HF_VoiceRecognitionLearning()!=0)
	{
		BtApp_HF_VoiceRecognitionLearningToggle();
		//UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_VOICE_RECOGNITION_LEARN_END_IND);		//BtApp_HF_Close_Call();
        BtApp_HF_EvtVoiceRecognitionLearnEndInd(ucDevNo);
	}
	
	if(BtApp_HF_VoiceRecognition()!=0)
	{
		BtApp_HF_SetVoiceRecognitionOnOff();
        BtApp_HF_EvtVoiceRecognitionEndInd(ucDevNo);
	}
	else
	{
        BtApp_HF_EvtVoiceRecognitionEndInd(ucDevNo);
	}
}

void BtApp_HF_AnswerCall(void)
{
    (void)G_BtHfCmd->TC_BT_HF_CallAnswer(BtApp_HF_GetSelectedDevBdAddr());
}

void BtApp_HF_RejectCall(void)
{
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_RejectCall : %d\n",__FILENAME__,__LINE__,0);

	if (!BtApp_HF_RejectAbility())
	{
		return;
	}

	ucDevNo=BtApp_HF_GetCurrDevNum();
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo));

	BtApp_HF_Reject();
}

void BtApp_HF_DrawCall(void)
{
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_DrawCall : %d\n",__FILENAME__,__LINE__,0);

	ucDevNo=BtApp_HF_GetCurrDevNum();
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo));

	BtApp_HF_Reject();
}

void BtApp_HF_TerminateCall(void)
{
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_TerminateCall : %d\n",__FILENAME__,__LINE__,0);

	ucDevNo=BtApp_HF_GetCurrDevNum();
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo));

	BtApp_HF_Reject();
}

void BtApp_HF_Redial(void)
{
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
		TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,ucDevNo));
		
        lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();

		switch(lBasicMode)
		{
			case BIDLE:
#if 1 /* TEST */
#else
				PrevBasicMode = lBasicMode;
				PrevCurrentCodec = CurrentCodec;
#endif
#if 0	/* Telechips Linux HF remark */
				if(OpenFileFlag == FILE_OPENED)
				{
					Playback_API_BackupPlayData();
					Playback_API_CloseTrack();
				}
#endif				
				break;
#if 0	/* Telechips Linux HF remark */
			case BPLAY:
				PrevBasicMode = lBasicMode;
				PrevCurrentCodec = CurrentCodec;
				Playback_API_BackupPlayData();
#if 1 /* Telechips' 2009.08 QA - Ver. 6.24 */
#ifdef TCC860x
				UI_DRV_CloseTrack();
				UI_DRV_StopPlay();
				PlaySec = 0;
				TotalPlaybackSec = 0;
#endif
#else
				Playback_API_StopPlay();
				Playback_API_CloseTrack();
#endif
				break;
			case BRADIO:
				break;
			case BRECORD:
				break;
			case BPLAYPAUSE:
				break;
			case BDAB:
				break;
#endif
#ifdef BT_AUDIO_SINK
			case B_BT_A2DP_SINK:
				break;
#endif
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
			case B_BT_HFP_RING:
				break;
#endif
			case B_BT_HFP_CALL:
				break;
			case B_BT_HSP_AUDIO:
				break;
#if 0	/* Telechips Linux HF remark */
			case BFF:
				break;
			case BREW:
				break;
			case BRECPAUSE:
				break;			
#endif
		}		

    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
    (void)G_BtHfCmd->TC_BT_HF_Dial(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_DIAL_REDIAL, NULL);
}

void BtApp_HF_InputNumberDial(void)
{
#if 1	/* Telechips Linux HF remark */
    unsigned char  a;
        unsigned char  b;
    BtApp_HF_Timer_InputNumberDial(a,b);
#endif
}

void BtApp_HF_Timer_InputNumberDial(unsigned char param1, unsigned char param2)
{
#if 1	/* Telechips Linux HF remark */
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_OUTGOING_RETRY_ID,ucDevNo));

	//if(BtApp_HF_GetCurrDevNum() == 0)
	//{
	//	TCCAPI_Timer_CancelTime(TIMER_BT_HF1_OUTGOING_RETRY_ID);
	//}
	//else
	//{
	//	TCCAPI_Timer_CancelTime(TIMER_BT_HF2_OUTGOING_RETRY_ID);
	//}

#if 1

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case BIDLE:
	#if 1 /* TEST */
	#else
			PrevBasicMode = lBasicMode;
			PrevCurrentCodec = CurrentCodec;
	#endif
#if 0	/* Telechips Linux HF remark */
            if(OpenFileFlag == FILE_OPENED)
			{
				Playback_API_BackupPlayData();
				Playback_API_CloseTrack();
			}
#endif
			break;
#if 0	/* Telechips Linux HF remark */
    case BPLAY:
			PrevBasicMode = lBasicMode;
			PrevCurrentCodec = CurrentCodec;
			Playback_API_BackupPlayData();
#if 1 /* Telechips' 2009.08 QA - Ver. 6.24 */
	#ifdef TCC860x
			UI_DRV_CloseTrack();
			UI_DRV_StopPlay();
			PlaySec = 0;
			TotalPlaybackSec = 0;
	#endif
#else
			Playback_API_StopPlay();
			Playback_API_CloseTrack();
#endif
			break;
		case BRADIO:
			break;
		case BRECORD:
			break;
		case BPLAYPAUSE:
			break;
		case BDAB:
			break;
#endif
#ifdef BT_AUDIO_SINK
		case B_BT_A2DP_SINK:
			break;
#endif
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			break;
#endif
		case B_BT_HFP_CALL:
			break;
		case B_BT_HSP_AUDIO:
			break;
#if 0	/* Telechips Linux HF remark */
        case BFF:
			break;
		case BREW:
			break;
		case BRECPAUSE:
			break;			
#endif
	}
#endif

	if(gBtAppHfCallNumPos[ucDevNo]==0)
	{
		/* Redial */
        (void) G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));
        (void) G_BtHfCmd->TC_BT_HF_Dial(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_DIAL_REDIAL, NULL);
	}
	else
	{
		/* Input Number Dial */
		/* NULL : String termination */
		if(gBtAppHfCallNumPos[ucDevNo]>=BTAPP_HF_CALLNUM_MAX_LEN)
		{
			gBtAppHfCallNumPos[ucDevNo]=BTAPP_HF_CALLNUM_MAX_LEN-1;
		}
		
		gBtAppHfCallNumber[ucDevNo][gBtAppHfCallNumPos[ucDevNo]] = NULL;

        (void) G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF));

        if(strcmp((char *)&(gBtAppHfCallNumber[ucDevNo][0]), BTAPP_HF_UNKNOWN_NUMBER_STRING) == 0)
		{
			/* Just redial OR Don't dial */

            BtApp_HF_EvtOutgoingDialInf(ucDevNo);
			return;
		}

        if(strcmp((char *)&(gBtAppHfCallNumber[ucDevNo][0]), BTAPP_HF_PRIVATE_NUMBER_STRING) == 0)
		{
			/* Just redial OR Don't dial */

            BtApp_HF_EvtOutgoingDialInf(ucDevNo);
			return;
		}

		/* Not use reservation for Preset/Memory Dial '>' */
		if(gBtAppHfCallNumber[ucDevNo][0] == '>')
		{
            (void)G_BtHfCmd->TC_BT_HF_Dial(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_DIAL_MEMORY, &gBtAppHfCallNumber[ucDevNo][1]);
		}
		else
		{
            (void)G_BtHfCmd->TC_BT_HF_Dial(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_DIAL_NUMBER, &gBtAppHfCallNumber[ucDevNo][0]);
		}
	}

#if 0 /* If this is changed to 1, it carries out retry outgoing call */
	if(gBtAppHfCallsetupIndex[ucDevNo]<2)
		UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HF_OUTGOING_RETRY);
#endif

#endif
}



#ifdef BT_HFP_MIC_MUTE
void BtApp_HF_MicMuteToggle(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfMicMuteStatus[ucDevNo] == TRUE)
	{
		gBtAppHfMicMuteStatus[ucDevNo] = FALSE;
	}
	else
	{
		gBtAppHfMicMuteStatus[ucDevNo] = TRUE;
	}
}
#endif

/*****************************************************************************
* Function Name : BtApp_HF_RHPutOnHold()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RHPutOnHold(void)
{
    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_BTRH_PUT_ON_HOLD, 0);
}

/*****************************************************************************
* Function Name : BtApp_HF_RHAcceptIncoming()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RHAcceptIncoming(void)
{
    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_BTRH_ACCEPT_INCOMING, 0);
}

/*****************************************************************************
* Function Name : BtApp_HF_RHRejectIncoming()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RHRejectIncoming(void)
{
    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_BTRH_REJECT_INCOMING, 0);
}

/*****************************************************************************
* Function Name : BtApp_HF_RHReadStatus()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RHReadStatus(void)
{
    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_BTRH_READ_STATUS, 0);
}

/*****************************************************************************
* Function Name : BtApp_HF_HoldAndAcceptCall()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_HoldAndAcceptCall(void)
{
	unsigned char ucDevNo;
	unsigned long ulSupportThreewayCalling = FALSE;
	unsigned long ulSupportEnhancedCallCtrl = FALSE;

	ulSupportThreewayCalling = BtApp_HF_SupportThreeWayCalling();
	ulSupportEnhancedCallCtrl = BtApp_HF_SupportEnhancedCallControl();

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!(ulSupportThreewayCalling || ulSupportEnhancedCallCtrl))
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_HOLD_ACTIVE_ACCEPT, 0);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_HOLD_ACTIVE_ACCEPT,ucDevNo),
			BtApp_HF_TimeoutHoldActiveAccept);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall1()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall1(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportEnhancedCallControl())
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_REQUEST_PRIVATE_WITH_SPECIFIED, 1);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED,ucDevNo),
			BtApp_HF_TimeoutRequestPrivateWithSpecified);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall2()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall2(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportEnhancedCallControl())
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_REQUEST_PRIVATE_WITH_SPECIFIED, 2);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_REQUEST_PRIVATE_WITH_SPECIFIED,ucDevNo),
			BtApp_HF_TimeoutRequestPrivateWithSpecified);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_ReleaseAndAcceptCall()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ReleaseAndAcceptCall(void)
{
	unsigned char ucDevNo;
	unsigned long ulSupportThreewayCalling = FALSE;
	unsigned long ulSupportEnhancedCallCtrl = FALSE;

	ulSupportThreewayCalling = BtApp_HF_SupportThreeWayCalling();
	ulSupportEnhancedCallCtrl = BtApp_HF_SupportEnhancedCallControl();

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!(ulSupportThreewayCalling || ulSupportEnhancedCallCtrl))
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_RELEASE_ACTIVE_ACCEPT, 0);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ACTIVE_ACCEPT,ucDevNo),
			BtApp_HF_TimeoutReleaseActiveAccept);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_ReleasesSpecifiedActiveCall1()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ReleasesSpecifiedActiveCall1(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportEnhancedCallControl())
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_RELEASE_SPECIFIED_CALL, 1);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,ucDevNo),
			BtApp_HF_TimeoutReleaseSpecifiedCall);
	}	
}

/*****************************************************************************
* Function Name : BtApp_HF_ReleasesSpecifiedActiveCall2()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ReleasesSpecifiedActiveCall2(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportEnhancedCallControl())
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_RELEASE_SPECIFIED_CALL, 2);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_SPECIFIED_CALL,ucDevNo),
			BtApp_HF_TimeoutReleaseSpecifiedCall);
	}		
}

//#ifdef BT_PTS_TEST2	//Yootest 20090220
/*****************************************************************************
* Function Name : BtApp_HF_AddHeldCallToConversation()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_AddHeldCallToConversation(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportChld(3))
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_ADD_CALL, 0);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_ADD_CALL,ucDevNo),
			BtApp_HF_TimeoutAddCall);
	}		
}

/*****************************************************************************
* Function Name : BtApp_HF_ExplicitCallTransfer()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ExplicitCallTransfer(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!BtApp_HF_SupportChld(4))
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_CONNECT_TWO_CALLS, 0);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CONNECT_TWO_CALLS,ucDevNo),
			BtApp_HF_TimeoutConnectTwoCalls);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_ReleaseAllHeldCalls()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ReleaseAllHeldCalls(void)
{
	unsigned char ucDevNo;
	unsigned long ulSupportThreewayCalling = FALSE;
	unsigned long ulSupportEnhancedCallCtrl = FALSE;

	ulSupportThreewayCalling = BtApp_HF_SupportThreeWayCalling();
	ulSupportEnhancedCallCtrl = BtApp_HF_SupportEnhancedCallControl();

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!(ulSupportThreewayCalling || ulSupportEnhancedCallCtrl))
	{
		return;
	}

	if(!BtApp_HF_SupportChld(0))
	{
		return;
	}

    (void)G_BtHfCmd->TC_BT_HF_CallHandling(BtApp_HF_GetSelectedDevBdAddr(), TC_BT_HF_RELEASE_ALL_HELD_CALL, 0);

	//if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=FALSE)
	{
		TCCAPI_Timer_SetTime(
			TIMER_BT_CHLD_TIMEOUT_TIME,
			TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RELEASE_ALL_HELD_CALL,ucDevNo),
			BtApp_HF_TimeoutReleaseAllHeldCall);
	}	
}

/*****************************************************************************
* Function Name : BtApp_HF_ExcuteResponseAndHoldControl()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ExcuteResponseAndHoldControl(void)
{
	const pBtAppHfFuncResponseAndHold pArrayUiBtHfResponseAndHoldControl[BTAPP_HF_RESPONSE_AND_HOLD_CONTROL_MAX] = 
	{
		BtApp_HF_RHPutOnHold,							/* 0 */
		BtApp_HF_RHAcceptIncoming,						/* 1 */
		BtApp_HF_RHRejectIncoming,						/* 2 */
		BtApp_HF_RHReadStatus							/* ? */
	};
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if((gBtAppHfResponseAndHoldControlIdx[ucDevNo] < BTAPP_HF_RESPONSE_AND_HOLD_CONTROL_MAX)
		&& (pArrayUiBtHfResponseAndHoldControl[gBtAppHfResponseAndHoldControlIdx[ucDevNo]]))
	{
		pArrayUiBtHfResponseAndHoldControl[gBtAppHfResponseAndHoldControlIdx[ucDevNo]]();
	}
}

#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
/*****************************************************************************
* Function Name : BtApp_HF_ExcuteEnhancedCallControl()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_ExcuteEnhancedCallControl(void)
{
	/* AT+CHLD = 0, 1, 11, 12, 2, 21, 22, 3, 4 */

	/*
	unsigned char *pStrArrayUiBtHfEnhancedCallControl[BTAPP_HF_ENHANCED_CALL_CONTROL_MAX] =
	{
		"Selected AT+CHLD=0",
		"Selected AT+CHLD=1",
		"Selected AT+CHLD=11",
		"Selected AT+CHLD=12",
		"Selected AT+CHLD=2",
		"Selected AT+CHLD=21",
		"Selected AT+CHLD=22",
		"Selected AT+CHLD=3",
		"Selected AT+CHLD=4"
	};	
	*/

	const pBtAppHfFuncEnhancedCallControl pArrayUiBtHfEnhancedCallControl[BTAPP_HF_ENHANCED_CALL_CONTROL_MAX] = 
	{
		BtApp_HF_ReleaseAllHeldCalls,								/* 0 */
		BtApp_HF_ReleaseAndAcceptCall,								/* 1 */
		BtApp_HF_ReleasesSpecifiedActiveCall1,						/* 11 */
		BtApp_HF_ReleasesSpecifiedActiveCall2,						/* 12 */
		BtApp_HF_HoldAndAcceptCall,									/* 2 */
		BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall1,	/* 21 */
		BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall2,	/* 22 */
		BtApp_HF_AddHeldCallToConversation,							/* 3 */
		BtApp_HF_ExplicitCallTransfer								/* 4 */
	};
	unsigned char ucDevNo;
	unsigned long ulSupportThreewayCalling = FALSE;
	unsigned long ulSupportEnhancedCallCtrl = FALSE;

	ulSupportThreewayCalling = BtApp_HF_SupportThreeWayCalling();
	ulSupportEnhancedCallCtrl = BtApp_HF_SupportEnhancedCallControl();

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(!(ulSupportThreewayCalling || ulSupportEnhancedCallCtrl))
	{
		return;
	}

	if((gBtAppHfEnhancedCallControlIdx[ucDevNo] < BTAPP_HF_ENHANCED_CALL_CONTROL_MAX)
		&& (pArrayUiBtHfEnhancedCallControl[gBtAppHfEnhancedCallControlIdx[ucDevNo]]))
	{
		pArrayUiBtHfEnhancedCallControl[gBtAppHfEnhancedCallControlIdx[ucDevNo]]();
	}
}
#endif


/*****************************************************************************
* Function Name : BtApp_HF_GetClcc()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_GetClcc(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfClccStatus[ucDevNo]=TRUE;
	BtApp_HF_Clcc();
	gBtAppHfClccStatus[ucDevNo]=FALSE;
	
    BtApp_HF_EvtOptionsClccInf(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetBia()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetBia(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    (void)G_BtHfCmd->TC_BT_HF_IndicatorActivation(BtApp_HF_GetSelectedDevBdAddr(),0x1F/*e.g AT+BIA=1,1,1,1,1,0,0 */);

    BtApp_HF_EvtOptionsBiaInf(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetBiev()
******************************************************************************
* Desription  : 
* Parameter   :
* Return	  :
******************************************************************************/

void BtApp_HF_SetBiev(void)
{
	TcBtUint16 anum;
	TcBtUint32 value;
	
    (void)G_BtHfCmd->TC_BT_HF_UpdateHFIndicator(BtApp_HF_GetSelectedDevBdAddr(), 1/*Enhanced Driver Safety*/);/*e.g AT+BIEV=1,x*/
}


//Yootest
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
/*****************************************************************************
* Function Name : BtApp_HF_Ring_DRV_Open()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Ring_DRV_Open(void)
{
#if 1	/* Telechips Linux HF remark */
#ifdef __arm__
    unsigned char ucDevNo;

    ucDevNo=BtApp_HF_GetCurrDevNum();

    hf_ring_start(HF_RING_DEVICE,8000/*8kHz*/,
                        ghf_ring_array[0].data,ghf_ring_array[0].len);

    G_BtSys->BtApp_SYS_SetBasicMode(B_BT_HFP_RING);		//B_BT_HFP_RING
#endif
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Ring_DRV_Close()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Ring_DRV_Close(void)
{
#if 0	/* Telechips Linux HF remark - No one calls this function in DMP_HOST */

#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Open_Ring()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int BtApp_HF_Open_Ring(void)	// OpenTrackForHFRing
{
/* Telechips Linux HF remark                                          *
 * Linux has no audio task, directly calls in BtApp_HF_Ring_DRV_Open  */
}

/*****************************************************************************
* Function Name : BtApp_HF_Close_RingEx()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Close_RingEx(void)	// ClockTrack
{
/* Telechips Linux HF remark                                          *
 * Linux has no audio task, directly calls in BtApp_HF_Close_Ring  */
}

/*****************************************************************************
* Function Name : BtApp_HF_Close_Ring()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Close_Ring(void)	// CloseTrack
{
	printf("[%s : %05d] Info : BtApp_HF_Close_Ring : %d\n",__FILENAME__,__LINE__,0);

    if(G_BtSys->BtApp_SYS_GetBasicMode()==B_BT_HFP_RING)
	{
#if 1	/* Telechips Linux HF remark */
#ifdef __arm__
    unsigned char ucDevNo;

    ucDevNo=BtApp_HF_GetCurrDevNum();

	BtApp_HF_Close_RingEx();
    hf_ring_stop();
G_BtSys->BtApp_SYS_SetBasicMode(BIDLE);		//B_BT_HFP_RING
#endif	/* arm */
#endif
	}
}
#endif

/*************************************************************************************
*  BtApp_HF_RingSetParameter
*-------------------------------------------------------------------------------------
* Description  :
* Parameters   :
* Return	   :
**************************************************************************************/
void BtApp_HF_RingSetParameter(unsigned short uSampleRate, unsigned char uCh)
{
	if (uSampleRate!=0)
	{
		gpstBtAppHfRingPCMData.usSampleRate	= uSampleRate;
	}
	else
	{
		gpstBtAppHfRingPCMData.usSampleRate	= 8000;
	}
	
	if (uCh!=0)
	{
		gpstBtAppHfRingPCMData.ucChannel		= uCh;
	}
	else
	{
		gpstBtAppHfRingPCMData.ucChannel		= 1;
	}
}

/*************************************************************************************
*  BtApp_HF_RingGetParameter
*-------------------------------------------------------------------------------------
* Description  :
* Parameters   :
* Return	   :
**************************************************************************************/
void BtApp_HF_RingGetParameter(unsigned short *uSampleRate, unsigned char *uCh)
{
	if (gpstBtAppHfRingPCMData.usSampleRate!=0)
	{
		*uSampleRate	= gpstBtAppHfRingPCMData.usSampleRate;
	}
	else
	{
		*uSampleRate	= 8000;
	}
	
	if (gpstBtAppHfRingPCMData.ucChannel!=0)
	{
		*uCh 			= gpstBtAppHfRingPCMData.ucChannel;
	}
	else
	{
		*uCh 			= 1;
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_SetRingStart()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetRingStart(unsigned int iID)
{
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	BtApp_HF_SetSelectedDevNum(ucDevNo);

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case BIDLE:
			break;
#if 0//def BT_AUDIO_SINK
		case B_BT_A2DP_SINK:
			#if 1 /* TEST */
			if(PrevBasicMode != lBasicMode)
			{
				PrevBasicMode = lBasicMode;
			}
			#else
			PrevBasicMode = lBasicMode;
			#endif
			PrevCurrentCodec = CurrentCodec;
			Playback_API_StopPlay();
			Playback_API_CloseTrack();
			break;
#endif
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			//BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);	
			return; // Nothing to do
	#endif
		case B_BT_HFP_CALL:
			BtApp_HF_Close_Call();
			break;
		case B_BT_HSP_AUDIO:
			//UI_BT_Close_HSAudio();
			break;			
	}

	printf("[%s : %05d] Info : BtApp_HF_SetRingStart : %d\n",__FILENAME__,__LINE__,lBasicMode);


	/* AEC parameters */
	(void)BtApp_HF_SetCallParameters(BLUETOOTH_HF_RING_ID);

#if 0	/* Telechips Linux HF remark */ 		
		// TO DO : Set Ring CODEC
#ifdef BT_HFP_SRC
	BtApp_HF_RingSetParameter(BT_HFP_SAMPLERATE, 1);
#else
	BtApp_HF_RingSetParameter(AEC_PCM_BASE_SAMPLERATE, 1);
#endif
#endif

	BtApp_HF_Ring_DRV_Open();

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case B_BT_HFP_RING:
			break;
		default:
			BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);
			return;
	}

#endif	/* BT_HFP_RING_FOR_NO_IN_BAND */

	BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetIRingEnd()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetIRingEnd(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	printf("[%s : %05d] Info : BtApp_HF_SetIRingEnd : %d\n",__FILENAME__,__LINE__,0);

	gBtAppHfCallingLineID[ucDevNo][0] ='\0';
	BtApp_HF_SetCallIdRcvd(FALSE);

#ifdef BT_HFP_RING_FOR_NO_IN_BAND
	BtApp_HF_Close_Ring();
#endif

#if 1 /* TEST */
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_RING_ID,ucDevNo));
	TCCAPI_Timer_CancelTime(TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_PRIVATE_NUMBER_ID,ucDevNo));

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
	{
		/* for incoming call case of many Nokia phones except 6280, 6230i, 7370, 7380 */
		/* for incoming call case of Motorola U6, RAZR V3i, SLVR L7 */
	}
	else
	{
		BtApp_HF_Close_Call();
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))!=FALSE)
		{
#if 0	/* Telechips Linux HF remark */
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_SYS, BT_SYS_EVT_RESTORE_BASIC_MODE);
#endif
		}
	}
#else
	if(!UI_API_GetBluetoothStatus(BTAPP_STATUS_MASK_HF_CALL))
		BtApp_HF_Close_Call();

	TCCAPI_Timer_CancelTime(TIMER_BT_HF_RING_ID);

	UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_RESTORE_BASIC_MODE);
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_SetOutgoingStart()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetOutgoingStart(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	gBtAppHfCallingLineID[ucDevNo][0] ='\0';
	BtApp_HF_SetCallIdRcvd(FALSE);

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF))==FALSE)
	{
		gBtAppHfCallNumPos[ucDevNo] = 0;
		memset(gBtAppHfCallNumber[ucDevNo], 0 , BTAPP_HF_CALLNUM_MAX_LEN);
	}

#if !defined(BT_PTS_TEST) /* Telechips' 2008.12 QA - 3.6 (a), (b) */
	TCCAPI_Timer_SetTime(TIMER_BT_CLCC_TIMEOUT_TIME,
					TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_CLCC_ID,ucDevNo), BtApp_HF_Timer_Clcc );
#endif

    BtApp_HF_EvtOutgoingFromHfgInd(ucDevNo);
	BtApp_HF_EvtOutgoingStartInd(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetOutgoingEnd()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetOutgoingEnd(void)
{
	printf("[%s : %05d] Info : BtApp_HF_SetOutgoingEnd : %d\n",__FILENAME__,__LINE__,0);


    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
	{
		return;
	}

	BtApp_HF_Close_Call();

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))!=FALSE)
	{
#if 0	/* Telechips Linux HF remark - porting */
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_SYS, BT_SYS_EVT_RESTORE_BASIC_MODE);
#endif
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_Call_DRV_Open()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Call_DRV_Open(void)
{
#if 1	/* Telechips Linux HF remark */
	BtApp_HF_Open_Call();
#else
	AUDIO_COMMAND_t	KeyIn;
	UNSIGNED		Cmd;
	KeyIn.usCommand = BT_HFP_CALL_MODE;
	KeyIn.usSyncOption = YES;
	Cmd = (UNSIGNED)&KeyIn;
	(void)TC_Send_To_Queue(&TCC_AUDIO_Q, (void *)&Cmd);
	(void)TC_Obtain_Semaphore(&TCC_AUDIO_SEM,TC_SUSPEND);
#endif

}

/*****************************************************************************
* Function Name : BtApp_HF_Call_DRV_Close()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Call_DRV_Close(void)
{
#if 0	/* Telechips Linux HF remark - No one calls this function in DMP_HOST */
	AUDIO_COMMAND_t	KeyIn;
	UNSIGNED		Cmd;
	KeyIn.usCommand = STOP_PROCESS;
	KeyIn.usSyncOption = YES;
	Cmd = (UNSIGNED)&KeyIn;
	(void)TC_Send_To_Queue(&TCC_AUDIO_Q, (void *)&Cmd);
	(void)TC_Obtain_Semaphore(&TCC_AUDIO_SEM,TC_SUSPEND);
#endif
}	


/*****************************************************************************
* Function Name : BtApp_HF_Open_Call()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int BtApp_HF_Open_Call(void)	// OpenTrackForHFCall
{
#if 1	/* Telechps Linux HF remark */
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
    // WBS(16kHz) -----------------------------------------
    unsigned int aec_samplerate;
	unsigned int codec_samplerate;

    printf("[%s : %05d] Info : BtApp_HF_Open_Call : %d\n",__FILENAME__,__LINE__,0);

#ifdef BT_HFP_WIDEBAND_SPEECH
	if(BtApp_HF_GetCurCodec()==TC_BT_WBS_MSBC_CODEC)
	{
		aec_samplerate=16000;
#ifdef BT_HFP_SRC			
		codec_samplerate = BT_HFP_SAMPLERATE;
#else
        codec_samplerate = aec_samplerate;		
#endif

	    aec_audio_start( NEAREND_PLAYBACK_DEVICE,codec_samplerate,
	                        NEAREND_CAPTURE_DEVICE,codec_samplerate,
	                        FAREND_PLAYBACK_DEVICE,aec_samplerate,
	                        FAREND_CAPTURE_DEVICE,aec_samplerate,
	                        aec_samplerate/*WBS*/);
	}
	else
#endif		
	{
        aec_samplerate=8000;
#ifdef BT_HFP_SRC			
		codec_samplerate = BT_HFP_SAMPLERATE;
#else
		codec_samplerate = aec_samplerate;
#endif

	    aec_audio_start( NEAREND_PLAYBACK_DEVICE,codec_samplerate,
	                        NEAREND_CAPTURE_DEVICE,codec_samplerate,
	                        FAREND_PLAYBACK_DEVICE,aec_samplerate,
	                        FAREND_CAPTURE_DEVICE,aec_samplerate,
	                        aec_samplerate/*NBS*/);
	}
#endif  /* BT_AEC_INCLUDE*/
#endif  /* arm */	
    G_BtSys->BtApp_SYS_SetBasicMode(B_BT_HFP_CALL);		//B_BT_HFP_CALL
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_Close_CallEx()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Close_CallEx(void)	// ClockTrack
{
}

/*****************************************************************************
* Function Name : BtApp_HF_Close_Call()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_Close_Call(void)
{
#if 1	/* Telechips Linux HF remark  */
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
    printf("[%s : %05d] Info : BtApp_HF_Close_Call : %d\n",__FILENAME__,__LINE__,0);

	BtApp_HF_Close_CallEx();
    aec_audio_stop();
    G_BtSys->BtApp_SYS_SetBasicMode(BIDLE);		//B_BT_HFP_CALL
#endif
#endif	/* __arm__ */
#endif
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallParameters()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/

unsigned char BtApp_HF_SetCallParameters(unsigned int iID)
{
	unsigned char boReset=FALSE;

#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
	eAECSampleRate eSampleRate=AEC_SAMPLERATE_8KHZ;


	/* samplerate ? */
#ifdef BT_HFP_WIDEBAND_SPEECH
	BtApp_HF_SetCurCodec(BtApp_HF_GetLastCodec());

	/* iID disabled, support WBS inband-ring */
	if(/*iID!=BLUETOOTH_HF_RING_ID && */BtApp_HF_GetCurCodec()==TC_BT_WBS_MSBC_CODEC)
	{
		eSampleRate=AEC_SAMPLERATE_16KHZ;
	}
#endif

	/* parameter */	
	AECIf_SetParameters((char)((iID == BLUETOOTH_HF_VOICE_DIAL_ID) ? 1 : 0),eSampleRate);
#endif
#endif

	return boReset;
}

static char BtApp_HF_Is_Active(void)
{
/* Telechips Linux HF remark    *
 * Linux jitter is different    */
#if 0
	if (AEC_DRV_GetMode()!=AEC_MODE_ACTIVE)
	{
		return FALSE;
	}	

	return TRUE;	
#endif
}

static void BtApp_HF_SetCallStartIOInit(char boReset)
{
/* Telechips Linux HF remark - No need porting in Linux *
 * AEC device driver dependancy codes           	    */
}

void BtApp_HF_SetCallStart(unsigned int iID) /* Telechips' 2009.08 QA - Ver. 6.24 */
{
	unsigned char lBasicMode;
	unsigned char boIoReset;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	printf("[%s : %05d] Info : BtApp_HF_SetCallStart\n",__FILENAME__,__LINE__);	

#ifdef BT_HFP_WIDEBAND_SPEECH
	printf("[%s : %05d] Info : gBtAppHfCurCodec = %d, gBtAppHfLastCodec[ucDevNo] = %d\n",__FILENAME__,__LINE__,BtApp_HF_GetCurCodec(), BtApp_HF_GetLastCodec());
#endif

	/* If BtApp_HF_SetCallIdRcvd(FALSE) enabled at this position 
	   blinking call number(id) when in-band ring incoming */
	//BtApp_HF_SetCallIdRcvd(FALSE);

	BtApp_HF_SetSelectedDevNum(BtApp_HF_GetCurrDevNum());

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case BIDLE:
			break;
#if 0//def BT_AUDIO_SINK
		case B_BT_A2DP_SINK:
			#if 1 /* TEST */
			if(PrevBasicMode != lBasicMode)
			{
				PrevBasicMode = lBasicMode;
			}
			#else
			PrevBasicMode = lBasicMode;
			#endif
			PrevCurrentCodec = CurrentCodec;
			Playback_API_StopPlay();
			Playback_API_CloseTrack();
			break;
#endif
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			BtApp_HF_Close_Ring();
			break;
	#endif
		case B_BT_HFP_CALL:
			if(iID == BLUETOOTH_HS_AUDIO_ID)
			{
				BtApp_HF_Close_Call();
			}
#ifdef BT_HFP_WIDEBAND_SPEECH
			else if(BtApp_HF_GetCurCodec()!=BtApp_HF_GetLastCodec())			
			{
				BtApp_HF_Close_Call();
			}
#endif
			else
			{
				// HF MIC Volume must be directly set to the UI_TASK	
				BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[ucDevNo]);

				/* Set volume */
				// HF Volume must be directly set to the UI_TASK
				BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);				
				return; // Nothing to do
			}
			break;
		case B_BT_HSP_AUDIO:
			if(iID == BLUETOOTH_HS_AUDIO_ID)
			{
				// HF MIC Volume must be directly set to the UI_TASK	
				BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[ucDevNo]);

				/* Set volume */
				// HF Volume must be directly set to the UI_TASK
				BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);			
				return; // Nothing to do
			}
			else
			{
				//UI_BT_Close_HSAudio();
			}
			break;			
	}

	/* AEC parameters */
	boIoReset=BtApp_HF_SetCallParameters(iID);

	/* Open AEC */
	if(iID == BLUETOOTH_HS_AUDIO_ID)
	{
		//HS_AUDIO_DRV_Open();
	}
	else
	{
		BtApp_HF_Call_DRV_Open();
	}

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case B_BT_HFP_CALL:
		case B_BT_HSP_AUDIO:
			break;
		default:
			// HF MIC Volume must be directly set to the UI_TASK	
			BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[ucDevNo]);


			/* Set volume */
			// HF Volume must be directly set to the UI_TASK
			BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);		
			return;
	}

#ifdef TCC860x
	{
		InitFIQReg();
	}
	RightChannelOff = TRUE;		// This prevents CODEC data in FIQ
#endif

//		if(boIoReset==FALSE)
//		{
//			BtApp_HF_SetCallStartIOInit(boIoReset);
//		}
	
	// HF MIC Volume must be directly set to the UI_TASK	
	BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[ucDevNo]);

	/* Set volume */
	// HF Volume must be directly set to the UI_TASK
	BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);
	if(boIoReset==FALSE)
	{
		BtApp_HF_SetCallStartIOInit((char)boIoReset);
	}
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallEnd()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCallEnd(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	printf("[%s : %05d] Info : BtApp_HF_SetCallEnd : %d\n",__FILENAME__,__LINE__,0);

	gBtAppHfCallNumPos[ucDevNo] = 0;
	memset(gBtAppHfCallNumber[ucDevNo], 0 , BTAPP_HF_CALLNUM_MAX_LEN);
	gBtAppHfCallingLineID[ucDevNo][0] ='\0';
	BtApp_HF_SetCallIdRcvd(FALSE);

	BtApp_HF_Close_Call();

    BtApp_HF_EvtCallTimeStampClearId(ucDevNo);

#if 0	/* Telechips Linux HF remark - porting */
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))!=FALSE)
	{
		DMP_MessageEvtUpdate(DISK_DEVICE_BT_SYS, BT_SYS_EVT_RESTORE_BASIC_MODE);
	}
#endif
}

void BtApp_HF_SetAudioStart(unsigned int iID)
{
/* Telechips Linux HF remark - porting A2DP inband-ring */			
#if 0//def BT_AUDIO_SINK
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	BtApp_HF_SetSelectedDevNum(BtApp_HF_GetCurrDevNum());

	lBasicMode = BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case BIDLE:
			#if 1 /* TEST */
			#else
			PrevBasicMode = lBasicMode;
			PrevCurrentCodec = CurrentCodec;
			#endif
#if 0	/* Telechips Linux HF remark */			
			if(OpenFileFlag == FILE_OPENED)
			{
				Playback_API_BackupPlayData();
				Playback_API_CloseTrack();
			}
#endif			
			break;
#if 0	/* Telechips Linux HF remark */
        case BPLAY:
		case BPLAYPAUSE:
		case BFF:
		case BREW:
		case BRADIO:
		case BDAB:
			PrevBasicMode = lBasicMode;
			PrevCurrentCodec = CurrentCodec;
			Playback_API_BackupPlayData();
#if defined(TCC801X)
			Playback_API_StopPlay();
			Playback_API_CloseTrack();
#elif defined(TCC80XX)
			UI_DRV_StopPlay();
			UI_DRV_CloseTrack();
#elif defined(TCC860x)
			UI_DRV_CloseTrack();
			UI_DRV_StopPlay();
			PlaySec = 0;
			TotalPlaybackSec = 0;
#endif
			break;
		case BRECORD:
		case BRECPAUSE:
			PrevBasicMode = lBasicMode;
			PrevCurrentCodec = CurrentCodec;
			UI_DRV_StopRec();
			break;
#endif
		case B_BT_A2DP_SINK:
			//PrevBasicMode = lBasicMode;
			if (BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=FALSE)
			{
		#ifdef BT_HFP_LINE_IN_TEST
			#if defined(TCC801X)
				(void)HAL_Ioctl(gpDacHalHandle[BT_PLAYBACK_LOCATION],HAL_IO_DAC_LINE_IN_VOL,31, 0, 0, 0);
			#elif defined(TCC80XX)
				Line_In_Volume(31);
			#elif defined(TCC860x)
				Line_In_Volume(31);
			#endif
		#else
				// [2011-11-1 yspark] HF MIC Volume must be directly set to the UI_TASK	
				BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[ucDevNo]);
		#endif

				// [2011-11-1 yspark] HF Volume must be directly set to the UI_TASK
				BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);					
				return;
			}

#if 0	/* Telechips Linux HF remark - porting */
			Playback_API_StopPlay();
			Playback_API_CloseTrack();
#endif			
			break;
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[ucDevNo]);	
			return; // Nothing to do
	#endif
		case B_BT_HFP_CALL:
			BtApp_HF_Close_Call();
			break;
		case B_BT_HSP_AUDIO:
			//UI_BT_Close_HSAudio();
			break;			
	}

	printf("[%s : %05d] Info : BtApp_HF_SetRingStart : %d\n",__FILENAME__,__LINE__,lBasicMode);


#if 0	/* Telechips Linux HF remark - porting */
	BtApp_AV_SNK_DRV_Open();
#endif
	lBasicMode = BtApp_SYS_GetBasicMode();

	switch (lBasicMode)
	{
		case B_BT_A2DP_SINK:
			break;
		default:
			return;
	}

#if 0	/* Telechips Linux HF remark */
	ModePlayInit();
#endif	
#endif	/* BT_AUDIO_SINK */
}

void BtApp_HF_SetAudioEnd(void)
{
#ifdef BT_AUDIO_SINK
	/* reserved */
#endif	
}

#ifdef BT_HFP_WIDEBAND_SPEECH
unsigned int BtApp_HF_GetLastCodec(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	return gBtAppHfLastCodec[ucDevNo];
}

void BtApp_HF_SetLastCodec(unsigned int uiCodec)
{
	unsigned char ucDevNo;

	printf("[%s : %05d] Info : BtApp_HF_SetLastCodec = %d\n",__FILENAME__,__LINE__,uiCodec);
	

	ucDevNo=BtApp_HF_GetCurrDevNum();

	gBtAppHfLastCodec[ucDevNo]=uiCodec;
}

unsigned int BtApp_HF_GetCurCodec(void)
{
	return gBtAppHfCurCodec;
}

void BtApp_HF_SetCurCodec(unsigned int uiCodec)
{
	printf("[%s : %05d] Info : BtApp_HF_SetCurCodec = %d\n",__FILENAME__,__LINE__,uiCodec);
	gBtAppHfCurCodec=uiCodec;
}
#endif


void BtApp_HF_SetAutoAnswerIndex(unsigned long ulIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if (ulIndex > 10)
		gBtAppHfAutoAnswerDelay[ucDevNo]=10;
	else
		gBtAppHfAutoAnswerDelay[ucDevNo] = ulIndex;

	BtApp_HF_EvtOptionsAutoAnswerInf(ucDevNo);
}

void BtApp_HF_SetDefaultMicGainIndex(unsigned long ulIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if (ulIndex > 15)
		gBtAppHfMicGainIndex[ucDevNo]=15;
	else
		gBtAppHfMicGainIndex[ucDevNo] = ulIndex;

#ifdef TCC860x
	UI_API_SetHfUiMicGainLevel(gBtAppHfMicGainIndex[ucDevNo]);
#endif
    (void)G_BtHfCmd->TC_BT_HF_MicGainStatus(BtApp_HF_GetSelectedDevBdAddr(), gBtAppHfMicGainIndex[ucDevNo]);

	BtApp_HF_EvtOptionsMicGainInf(ucDevNo);
}

void BtApp_HF_SetSmsAlarmIndex(unsigned long ulIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if (ulIndex > 1)
		gBtAppHfMessageUse[ucDevNo] = 1;
	else
		gBtAppHfMessageUse[ucDevNo] = ulIndex;

	BtApp_HF_EvtOptionsSMSAlaramInf(ucDevNo);
}

void BtApp_HF_SetRingPriorityIndex(unsigned long ulIndex)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if (ulIndex > 1)
		gBtAppHfInbandRingPriority[ucDevNo] = 1;
	else
		gBtAppHfInbandRingPriority[ucDevNo] = ulIndex;

	BtApp_HF_EvtOptionsRingInf(ucDevNo);
}

void BtApp_HF_SetRingSelectIndex(unsigned long ulIndex)
{
#ifdef BT_HFP_MULTI_RING_SUPPORT
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfBuiltInRingIdx[ucDevNo] > 2)
		gBtAppHfBuiltInRingIdx[ucDevNo] = 2;
	else
		gBtAppHfBuiltInRingIdx[ucDevNo] = ulIndex;

	BtApp_HF_EvtOptionsBuiltInInf(ucDevNo);
#endif
}


#ifdef BT_HFP_LOOPBACK_TEST
void Timer_LoopbackCallback(unsigned long id)
{
}

void BtApp_HF_ToggleLoopbackTest(void)
{
}
#endif

static unsigned char BtApp_HF_DTMF_UpdateNumber(unsigned char number)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gBtAppHfDtmfPos[ucDevNo]==0)
	{
		memset(gBtAppHfDtmfNumber[ucDevNo],0,BTAPP_HF_DTMFNUM_MAX_LEN);
	}
	
	gBtAppHfDtmfNumber[ucDevNo][gBtAppHfDtmfPos[ucDevNo]] = number;

	/* -2 for array index and string termination */
	if(gBtAppHfDtmfPos[ucDevNo] < BTAPP_HF_DTMFNUM_MAX_LEN-1)
	{
		gBtAppHfDtmfPos[ucDevNo]++;
	}
	else
	{
		gBtAppHfDtmfPos[ucDevNo]=0;
	}
	
	return number;
}


/*****************************************************************************
* Function Name : BtApp_HF_SetDtmf()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetDtmf(unsigned char data)
{
	unsigned char lBasicMode;
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

    lBasicMode = G_BtSys->BtApp_SYS_GetBasicMode();
	switch(lBasicMode)
	{
		case B_BT_HFP_CALL:
			break;
		default:
			return;
	}

	if(data==0)		/* reset */
	{
		gBtAppHfDtmfPos[ucDevNo] = 0;
		memset(&(gBtAppHfDtmfNumber[ucDevNo][0]), 0, BTAPP_HF_DTMFNUM_MAX_LEN);
	}
	else			/* action */
	{
		BtApp_HF_GenerateDtmf(BtApp_HF_DTMF_UpdateNumber(data/* '0'~'9','*'.'#','A'~'D' */));		
	}
	

    BtApp_HF_EvtDtmfInf(ucDevNo);
}

/*****************************************************************************
* Function Name : BtApp_HF_SetCallIdRcvd()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void BtApp_HF_SetCallIdRcvd(unsigned char ucRcvd)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	printf("[%s : %05d] Info : BtApp_HF_SetCallIdRcvd[%d] = %d\n",__FILENAME__,__LINE__, ucDevNo, ucRcvd);
	
	gBtAppHfCallIdRcvd[ucDevNo] = ucRcvd;
}

/*****************************************************************************
* Function Name : BtApp_HF_GetCallIdRecv()
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
unsigned char BtApp_HF_GetCallIdRcvd(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	return gBtAppHfCallIdRcvd[ucDevNo];
}

#ifdef BT_HFP_AT_SIRI
void BtApp_HF_AtCmdSendSiri(void)
{
	(void)TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(BtApp_HF_GetCurrDevNum()), "AT+XAPL=ABCD-1234-0100,10",BTAPP_HF_SIRI_XAPL_TOKEN);
}
#endif

void BtApp_HF_AtCmdSendBattCharReq(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();

	if(gboBtAppAtCmdCfm[ucDevNo][BTAPP_HF_CBC_TOKEN]==TRUE)
	{
        (void)G_BtHfCmd->TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(ucDevNo), (TcBtString *)"AT+CBC",BTAPP_HF_CBC_TOKEN);
	}
}

void BtApp_HF_AtCmdSendSignQualReq(void)
{
	unsigned char ucDevNo;

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(gboBtAppAtCmdCfm[ucDevNo][BTAPP_HF_CSQ_TOKEN]==TRUE)
	{
        (void)G_BtHfCmd->TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(ucDevNo), (TcBtString *)"AT+CSQ",BTAPP_HF_CSQ_TOKEN);
	}
}

static char BtApp_HF_AtCmdCompareStr(const char *theString, char **line, unsigned char size)
{
    unsigned char i;

    for (i=0; i < size; i++)
    {
        if ((*(*line+i) == '\0') || (BRWS_ToUpper(theString[i]) != BRWS_ToUpper(*(*line+i))))
        {
            return FALSE;
        }
    }
	
    (*line) += size;
	
     return TRUE;
}

int BtApp_HF_AtCmdGetToken(unsigned char *AtCmdString, char **String, unsigned int index)
{
    int  Token=BTAPP_HF_NONE_TOKEN;
	
    if (BtApp_HF_AtCmdCompareStr("Apple",(char **) &AtCmdString, 5))
    {
        Token = BTAPP_HF_PHONEMF_NOKIA_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("SAMSUNG", (char **) &AtCmdString, 7))
    {
        Token = BTAPP_HF_PHONEMF_SAMSUNG_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("Sony", (char **)&AtCmdString, 4))
    {
        Token = BTAPP_HF_PHONEMF_SONY_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("Nokia", (char **)&AtCmdString, 5))
    {
        Token = BTAPP_HF_PHONEMF_NOKIA_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("LG", (char **)&AtCmdString, 2))
    {
        Token = BTAPP_HF_PHONEMF_LG_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CGMI: HTC",(char **) &AtCmdString, 10))
    {
        Token = BTAPP_HF_PHONEMF_HTC_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CBC:",(char **) &AtCmdString, 5))
    {
        Token = BTAPP_HF_CBC_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CSQ:",(char **) &AtCmdString, 5))
    {
        Token = BTAPP_HF_CSQ_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CSCS",(char **) &AtCmdString, 5))
    {
        Token = BTAPP_HF_CSCS_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CPBR", (char **)&AtCmdString, 5))
    {
    	
        Token = BTAPP_HF_CPBR_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+CPBS",(char **) &AtCmdString, 5))
    {
        Token = BTAPP_HF_CPBS_TOKEN;
    }
#ifdef BT_HFP_AT_SIRI	
    else if (BtApp_HF_AtCmdCompareStr("+XAPL=", (char **)&AtCmdString, 6))
    {
        Token = BTAPP_HF_SIRI_XAPL_TOKEN;
    }
    else if (BtApp_HF_AtCmdCompareStr("+APLSIRI:", &AtCmdString, 9))
    {
        Token = BTAPP_HF_SIRI_APLSIRI_TOKEN;
    }	
#endif
//	    else if (BtApp_HF_AtCmdCompareStr("+CME", &AtCmdString, 4))
//	    {
//	        Token = BTAPP_HF_CME_TOKEN;
//	    }
//	    else if (BtApp_HF_AtCmdCompareStr("ERROR", &AtCmdString, 5))
//	    {
//	        Token = BTAPP_HF_ERROR_TOKEN;
//	    }
//	    else if (BtApp_HF_AtCmdCompareStr("OK", &AtCmdString, 2))
//	    {
//	        Token = BTAPP_HF_OK_TOKEN;
//	    }
	else
	{
	}

    *String = (char *)AtCmdString;
	
    return Token;
}

void BtApp_HF_AtCmdINDHandler(unsigned int Token, const unsigned char *atCmdString)
{
	//unsigned char ucDevNo;
	

	//ucDevNo=BtApp_HF_GetCurrDevNum();
	
	switch(Token)
	{
		case BTAPP_HF_CBC_TOKEN:
        {
            char bcl[4];
            //int val = 0;
			int i = 0;
			TcBtInt8 *pstr=(char *)atCmdString;
				
            while (*pstr != ',')
            {
                //printf("%c", *pstr);
                pstr++;
            }
            pstr += 1; /* Skip ','  */
            while (*pstr != 0x0D && i<3)
            {
                bcl[i] = *pstr;
                pstr++;
                i++;
            }
            bcl[i] = 0;
            //val = atoi(bcl); 
		}
 	       break;

        case BTAPP_HF_CSQ_TOKEN:
        {
            char CellRssi[4];
            //int val = 0;
            int i = 0;
			TcBtInt8 *pstr=(char *)atCmdString;

            while (*pstr != ',' && i<3)
            {
                CellRssi[i] = *pstr;
                pstr++;
                i++;
            }
            CellRssi[i] = 0;
            //val = atoi(CellRssi);
			
            pstr += 1; /* Skip ','  */
            //printf("\n   Channel bit error rate : ");
            while (*pstr != 0x0D)
            {
                //printf("%c", *pstr);
                pstr++;
            }
        }
			break;
#ifdef BT_HFP_AT_SIRI				
		case BTAPP_HF_SIRI_XAPL_TOKEN:
			break;
		case BTAPP_HF_SIRI_APLSIRI_TOKEN:
			break;
		case BTAPP_HF_SIRI_APLNRSTAT_TOKEN:
			break;
		case BTAPP_HF_SIRI_APLEFM_TOKEN:
			break;
#endif
		default:
			/* invalid cond. */
			printf("[%s : %05d] Info : BtApp_HF_AtCmdINDHandler (ERROR) : %d, %s \n",__FILENAME__,__LINE__,Token,atCmdString);
			break;
	}
	
}

void BtApp_HF_AtCmdCFMHandler(int result,unsigned int index)
{
	unsigned char ucDevNo;
	

	ucDevNo=BtApp_HF_GetCurrDevNum();
	
	if(result==TC_BT_CME_SUCCESS)
	{
		switch(index)
		{
			case BTAPP_HF_CBC_TOKEN:
				break;				
			case BTAPP_HF_CSQ_TOKEN:
				break;
#ifdef BT_HFP_AT_SIRI
			case BTAPP_HF_SIRI_XAPL_TOKEN:
				(void)TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(ucDevNo), "AT+APLSIRI?",BTAPP_HF_SIRI_APLSIRI_TOKEN);				
				(void)TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(ucDevNo), "AT+APLNRSTAT=1",BTAPP_HF_SIRI_APLNRSTAT_TOKEN);				
				(void)TC_BT_HF_AtCmd(TC_BT_HF_DEV_GetBdAddr(ucDevNo), "AT+APLEFM=1",BTAPP_HF_SIRI_APLEFM_TOKEN);
				break;
			case BTAPP_HF_SIRI_APLSIRI_TOKEN:
				break;
			case BTAPP_HF_SIRI_APLNRSTAT_TOKEN:
				break;
			case BTAPP_HF_SIRI_APLEFM_TOKEN:
				break;
#endif				
			default:
				/* invalid cond. */
				printf("[%s : %05d] Info : BtApp_HF_AtCmdCFMHandler : %d, %d \n",__FILENAME__,__LINE__,result,index);
				break;
		}
	}
	else if(result!=TC_BT_CME_RESPONSE_TIMEOUT)
	{
		switch(index)
		{
			case BTAPP_HF_CBC_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
			case BTAPP_HF_CSQ_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
#ifdef BT_HFP_AT_SIRI					
			case BTAPP_HF_SIRI_XAPL_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
			case BTAPP_HF_SIRI_APLSIRI_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
			case BTAPP_HF_SIRI_APLNRSTAT_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
			case BTAPP_HF_SIRI_APLEFM_TOKEN:
				gboBtAppAtCmdCfm[ucDevNo][index]=FALSE;
				break;
#endif				
			default:
				/* invalid condition */
				printf("[%s : %05d] Info : BtApp_HF_AtCmdCFMHandler : %d, %d \n",__FILENAME__,__LINE__,result,index);
				break;
		}
	}
	else
	{
		// invalid condition
		printf("[%s : %05d] Info : BtApp_HF_AtCmdCFMHandler : %d, %d \n",__FILENAME__,__LINE__,result,index);		
	}
}

#else	/* BT_HANDSFREE_UNIT  */
/*************************************************************************************
*  BtApp_HF_AudioINDHandler
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void BtApp_HF_AudioINDHandler(int bIndResult, unsigned short scoHandle)
{}
#endif	/* BT_HANDSFREE_UNIT  */

/* End Of File */
