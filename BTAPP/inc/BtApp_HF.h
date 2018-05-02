/****************************************************************************
 *   FileName    : BTAPP_HF.h
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
*    13/11/13     1.0            renamed                     Hwang, Jin-Ho.
*******************************************************************************/

#ifndef __BTAPP_HF_H__
#define __BTAPP_HF_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if 1	/* Telechips Linux HF remark */
#include "BT_CFG.h"
#endif
#include "inc/BtDataType.h"
#ifdef BT_HANDSFREE_UNIT
/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/
#ifndef  FALSE
#define  FALSE                     0
#endif

#ifndef  TRUE
#define  TRUE                      1
#endif

/*****************************************************************************
*
* data types
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#if defined(DMP_HOST)
#define BTAPP_HF_CONNECTION                    ((unsigned char) 0x00)
#define BTAPP_HS_CONNECTION                    ((unsigned char) 0x01)
#endif

#define BTAPP_HF_CALLNUM_MAX_LEN 32
#define BTAPP_HF_UNKNOWN_NUMBER_STRING "Unknown Number"
#define BTAPP_HF_PRIVATE_NUMBER_STRING "Private Number"

#define BTAPP_HF_DTMFNUM_MAX_LEN 20//32

#define BTAPP_HF_RESPONSE_AND_HOLD_CONTROL_MAX 4
#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
#define BTAPP_HF_ENHANCED_CALL_CONTROL_MAX 9
#endif

/* ATCMD Extension tokens */
typedef enum
{
	BTAPP_HF_NONE_TOKEN = 0x0,

// PB
	BTAPP_HF_PB_TOKEN,
	// CGMI
	BTAPP_HF_CGMI_TOKEN = BTAPP_HF_PB_TOKEN,
	// CSCS
	BTAPP_HF_CSCS_TOKEN,
	BTAPP_HF_SET_CSCS_TOKEN,
	BTAPP_HF_CSCS_SONY_TOKEN,
	BTAPP_HF_PHONEMF_SONY_TOKEN,	// 5
	BTAPP_HF_PHONEMF_NOKIA_TOKEN,
	BTAPP_HF_PHONEMF_LG_TOKEN,
	BTAPP_HF_PHONEMF_SAMSUNG_TOKEN,
	BTAPP_HF_PHONEMF_HTC_TOKEN,
	
	// CPBS
	BTAPP_HF_CPBS_TOKEN,			// 10
	BTAPP_HF_CPBS_ME_TOKEN,			
	BTAPP_HF_CPBS_DC_TOKEN,			
	BTAPP_HF_CPBS_MC_TOKEN,
	BTAPP_HF_CPBS_RC_TOKEN,
	BTAPP_HF_CPBS_NUM_TOKEN,		// 15	
	//#ifdef  R_ATCMD_SIM
	BTAPP_HF_CPBS_SM_TOKEN,			
	BTAPP_HF_CPBS_SM_NUM_TOKEN,
	//#endif
	// CPBR
	BTAPP_HF_CPBR_TOKEN,
	BTAPP_HF_CPBR_INDEX_TOKEN,
	//#ifdef  R_ATCMD_SIM
	BTAPP_HF_CPBR_SM_TOKEN,			// 20
	//#endif
	BTAPP_HF_CPBR_CH_TOKEN,			

// HF
	BTAPP_HF_TOKEN,
	BTAPP_HF_CBC_TOKEN = BTAPP_HF_TOKEN,	
	BTAPP_HF_CSQ_TOKEN,

#ifdef BT_HFP_AT_SIRI	
	BTAPP_HF_SIRI_XAPL_TOKEN,	
	BTAPP_HF_SIRI_APLSIRI_TOKEN,	// 25
	BTAPP_HF_SIRI_APLNRSTAT_TOKEN,	
	BTAPP_HF_SIRI_APLEFM_TOKEN,			
#endif
	BTAPP_HF_TOKEN_MAX
}BTAPP_HF_ATCMD_TOKEN;

/*****************************************************************************
*
* structures
*
*****************************************************************************/
typedef void (*pBtAppHfFuncPeriodicEvent)(void);

#if 1      /* Telechips Linux HF remark */
/*-- Menu ID definition --*/
enum
{
	BLUETOOTH_HF_RING_ID = 0,
	BLUETOOTH_HF_CALL_ID,
	BLUETOOTH_HF_OUTGOING_ID,

	BLUETOOTH_HF_VOICE_DIAL_ID,

	BLUETOOTH_HS_RING_ID,
	BLUETOOTH_HS_AUDIO_ID
};
#endif

typedef void (*pBtAppHfFuncResponseAndHold)(void);

#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
typedef void (*pBtAppHfFuncEnhancedCallControl)(void);
#endif

typedef struct
{
	unsigned uBufSelect;
	unsigned short usSampleRate;
	unsigned char ucChannel;
} stBtAppHfRingPCMData;

/* TODO: Add your specialized code here */

/*****************************************************************************
*
* externs
*
*****************************************************************************/
#if !defined(DMP_HOST)
#ifdef BT_HFP_WIDEBAND_SPEECH
extern unsigned int  gBtAppHfCodec[BT_HFP_MAX_CONNECTIONS];
#endif
#ifdef BT_HFP_LOOPBACK_TEST
extern unsigned int gBtAppHfLoopbackTest[BT_HFP_MAX_CONNECTIONS];
#endif
extern unsigned int gBtAppHfRingSec[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfCallSec[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfInbandRingPriority[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfInbandRing[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfInbandRingType[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfBuiltInRingIdx[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfAutoAnswerDelay[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfAutoAnswerStart[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfVoiceRecognitionAbility[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfVoiceRecognitionLearning[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfVoiceRecognition[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfRejectAbility[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportChld[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportThreeWayCalling[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportEcNrFunction[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportEnhancedCallStatus[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportEnhancedCallControl[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportExtendedErrorCodes[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSupportCodecNegotiation[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfRemoteVersion[BT_HFP_MAX_CONNECTIONS];
extern volatile int gBtAppHfCallStatus[BT_HFP_MAX_CONNECTIONS];
extern volatile int gBtAppHfCallsetupStatus[BT_HFP_MAX_CONNECTIONS];
extern volatile int gBtAppHfCallheldStatus[BT_HFP_MAX_CONNECTIONS];
extern volatile int gBtAppHfCallclccStatus[BT_HFP_MAX_CONNECTIONS];
extern volatile int gBtAppHfCallclccPrevStatus[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfEchoOnOffStatus[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfClccStatus[BT_HFP_MAX_CONNECTIONS];
extern char *gBtAppHfStrNetworkOperator[BT_HFP_MAX_CONNECTIONS];
#endif
extern char gBtAppHfCallingLineID[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
extern unsigned char gBtAppHfCallIdRcvd[BT_HFP_MAX_CONNECTIONS];
extern char gBtAppHfCcwaNumberID[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
#ifdef BT_HFP_WIDEBAND_SPEECH
extern unsigned int gBtAppHfLastCodec[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfCurCodec;
#endif	
extern unsigned char gBtAppHfCallNumPos[BT_HFP_MAX_CONNECTIONS];
extern unsigned char gBtAppHfCallNumber[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_CALLNUM_MAX_LEN];
extern unsigned int gBtAppHfCallsetupIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfServiceUse[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfServiceSupport[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfServiceIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfRoamUse[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfRoamSupport[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfRoamIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSignalUse[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfSignalSupport[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfSignalIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfMessageUse[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfMessageSupport[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfMessageIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfBattchgUse[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfBattchgSupport[BT_HFP_MAX_CONNECTIONS]
					, gBtAppHfBattchgIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfCmee[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfMicGainIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfSpkVolIndex[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfMicMuteStatus[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfAudioTransferStatus[BT_HFP_MAX_CONNECTIONS];
extern unsigned int gBtAppHfDtmfPos[BT_HFP_MAX_CONNECTIONS];
extern unsigned char gBtAppHfDtmfNumber[BT_HFP_MAX_CONNECTIONS][BTAPP_HF_DTMFNUM_MAX_LEN];
extern int gBtAppHfResponseAndHoldControlIdx[BT_HFP_MAX_CONNECTIONS];
#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
extern unsigned char gBtAppHfEnhancedCallControlIdx[BT_HFP_MAX_CONNECTIONS];
#endif

extern unsigned char gBtAppHfAtCmdString[24]; /* AT-CMD */
extern unsigned int gBtAppHfAtCmdIndex; /* AT-CMD */

extern unsigned int gBtAppHfPeriodicEvent[BT_HFP_MAX_CONNECTIONS];
 
extern void BtApp_HF_RecoverLinkEvent(void);
#if defined(BT_HCI_UART_INCLUDE) && !defined(BT_HFP_SCO_PCM_INTERFACE)
extern void BtApp_HF_Timer_Callback(void);	// uart
extern void BtApp_HF_Timer_Init(unsigned int uiPeriod);
extern void BtApp_HF_Timer_DeInit(void);
extern void (*pBtAppHfFuncTimer)(void);
#endif
extern void BtApp_HF_VoiceRecognitionOnOff(void);
extern unsigned int BtApp_HF_GetCurrDevNum(void);
extern void BtApp_HF_SetCurrDevNum(unsigned int currentDevNum);
extern unsigned int BtApp_HF_GetSelectedDevNum(void);
extern void BtApp_HF_SetSelectedDevNum(unsigned int selectedDevNum);
extern TcBtBdAddr *BtApp_HF_GetSelectedDevBdAddr(void);
extern void BtApp_HF_Clip(unsigned char id, unsigned char loc);
extern void BtApp_HF_Clcc(void);
extern void BtApp_HF_Timer_Clcc(unsigned char id, unsigned char loc);
extern void *BtApp_HF_GetCurrRemoteAddr(void);
extern void BtApp_HF_Reject(void);
extern void BtApp_HF_Dialing(char *pcNumber);
extern void BtApp_HF_AudioOnOff(void);
extern void BtApp_HF_Timer_AudioOnOff(unsigned char id, unsigned char loc);
extern void BtApp_HF_AudioAccept(unsigned char enable);
extern void BtApp_HF_UpdateSupportedCodec(unsigned int uiMask,unsigned char boEnable);
extern void BtApp_HF_CloseSCO(void);
extern void BtApp_HF_MicUp(void);
extern void BtApp_HF_MicDown(void);
extern void BtApp_HF_MicGainSet(int mgain);
extern void BtApp_HF_SpeakerUp(void);
extern void BtApp_HF_SpeakerDown(void);
extern void BtApp_HF_AutoConnect(unsigned char id, unsigned char loc);
extern void BtApp_HF_AutoConnectOnce(void);
extern void BtApp_HF_SetLinkStatus(int status);
extern int BtApp_HF_GetLinkStatus(void);
extern void BtApp_HF_CallbackPeriodicEvent(void);
extern void BtApp_HF_ActivateCFMHandler(unsigned char bCfmResult);
extern void BtApp_HF_DeactivateCFMHandler(unsigned char bCfmResult);
extern void BtApp_HF_DisconnectINDHandler(unsigned char bIndResult);
extern void BtApp_HF_DialingINDHandler(void);
extern void BtApp_HF_RedialingINDHandler(void);
extern void BtApp_HF_ServiceConnectINDHandler(unsigned char bIndResult);
extern void BtApp_HF_ServiceConnectNotificationINDHandler(void);
extern void BtApp_HF_AudioINDHandler(unsigned char bIndResult, unsigned short scoHandle);
extern void BtApp_HF_AudioAcceptINDHandler(unsigned char linkType);
#ifdef BT_HFP_WIDEBAND_SPEECH
extern void BtApp_HF_CodecSelectionINDHandler(unsigned char linkType);
#endif
extern void BtApp_HF_CindSupportIndHandler(unsigned char *pSupportStr, const unsigned char *pSupportValue);
extern void BtApp_HF_CmeeIndHandler(unsigned char cmeeErrorCode);
extern void BtApp_HF_CIEVINDCallHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDCallsetupHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDCallheldHandler(unsigned long ind);
extern void BtApp_HF_CLCCINDCallheldHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDServiceHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDRoamHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDSingalHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDBattchgHandler(unsigned long ind);
extern void BtApp_HF_CIEVINDMsgRcvdHandler(unsigned long ind);
extern void BtApp_HF_CallRingINDHandler(void);
extern void BtApp_HF_RingThroughA2dpStartINDHandler(TcBtBdAddr *pBdAddr);
extern void BtApp_HF_RingThroughA2dpSuspendINDHandler(const TcBtBdAddr *pBdAddr);
extern void BtApp_HF_AutoAnswerHandler(void);
extern void BtApp_HF_ClipTokenINDHandler(const char *pStrCallLineID);
extern void BtApp_HF_CLCCTokenINDHandler(unsigned char idx,unsigned char status,char *pStrOutgoingNumber, int len);
extern void BtApp_HF_CLCCTokenCFMHandler(int bIndResult);
extern void BtApp_HF_CCWATokenINDHandler(char *pStrCcwaNumber);
extern void BtApp_HF_COPSTokenINDHandler(char *pStrNetworkOperator, int len);
extern void BtApp_HF_CindSupportTokenINDHandler(char *pStrCindSupport, int len);
extern void BtApp_HF_SpkVolumeINDHandler(unsigned char HfSpkVol);
extern void BtApp_HF_MicGainINDHandler(unsigned char HfMicGain);
extern void BtApp_HF_VoiceRecognitionLearningToggle(void);
extern unsigned long BtApp_HF_VoiceRecognitionLearning(void);
extern void BtApp_HF_VoiceRecognitionINDHandler(unsigned long ind);
extern unsigned long BtApp_HF_VoiceRecognitionAbility(void);
extern void BtApp_HF_VoiceRecognitionAbilityInd(unsigned long indResult);
extern unsigned long BtApp_HF_VoiceRecognition(void);
extern void BtApp_HF_VoiceRecognitionInd(unsigned long indResult);
extern void BtApp_HF_InbandRingINDHandler(unsigned long ind);
extern unsigned long BtApp_HF_InbandRing(void);
extern unsigned long BtApp_HF_InbandRingPriority(void);
extern void BtApp_HF_InbandRingChangedInd(unsigned long indResult);
extern unsigned long BtApp_HF_RejectAbility(void);
extern void BtApp_HF_RejectAbilityInd(unsigned long indResult);
extern void BtApp_HF_UpdateSupportedCodecCFMHandler(unsigned char bCfmResult);
extern void BtApp_HF_DialCFMHandler(unsigned long CfmResult);
extern void BtApp_HF_CallAnswerCFMHandler(unsigned long CfmResult);
extern void BtApp_HF_CallEndCFMHandler(unsigned long CfmResult);
#ifdef BT_HFP_WIDEBAND_SPEECH
extern void BtApp_HF_SetCodecInd(unsigned long indResult);
extern unsigned long BtApp_HF_GetCodecInd(void);
#endif
extern unsigned long BtApp_HF_SupportChld(int index);
extern void BtApp_HF_SupportChldInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportThreeWayCalling(void);
extern void BtApp_HF_SupportThreeWayCallingInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportEcNrFunction(void);
extern void BtApp_HF_SupportEcNrFunctionInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportEnhancedCallStatus(void);
extern void BtApp_HF_SupportEnhancedCallStatusInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportEnhancedCallControl(void);
extern void BtApp_HF_SupportEnhancedCallControlInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportExtendedErrorCodes(void);
extern void BtApp_HF_SupportExtendedErrorCodesInd(unsigned long indResult);
extern unsigned long BtApp_HF_SupportCodecNegotiation(void);
extern void BtApp_HF_SupportCodecNegotiationInd(unsigned long indResult);
extern unsigned long BtApp_HF_RemoteVersion(void);
extern void BtApp_HF_RemoteVersionInd(unsigned long indResult);
extern void BtApp_HF_Timer_RunAtCmdEvent(unsigned char id, unsigned char loc);
extern void BtApp_HF_EchoOnOff(void);
extern void BtApp_HF_EchoOn(void);
extern void BtApp_HF_EchoOff(void);
extern void BtApp_HF_Timer_EchoOff(unsigned char id, unsigned char loc);
extern void BtApp_HF_EnableCMEE(unsigned char id, unsigned char loc);
extern void BtApp_HF_GetCops(unsigned char id, unsigned char loc);
extern int BtApp_HF_ScoDataCallback(const TcBtBdAddr *pBdAddr, char * theData);
extern void BtApp_HF_GenerateDtmf(unsigned char dtmf);
extern void BtApp_HF_ActivateHandsfree(void);
extern void BtApp_HF_DeactivateHandsfree(void);
extern void BtApp_HF_ConnectHandsfree(TcBtBdAddr *pBdAddr);
extern void BtApp_HF_DisconnectHandsfree(void);
extern void BtApp_HF_Info_Ring(unsigned char *pboUnicode,unsigned char *pucCallLineID);
extern void BtApp_HF_Info_Ccwa(unsigned char *pboUnicode,unsigned char *pucCcwaNumberID);
extern void BtApp_HF_Info_Outgoing(unsigned char *pboUnicode,unsigned char *pucCallNumber);
extern void BtApp_HF_Info_Call(unsigned char *pboUnicode,unsigned char *pucCallNumber);
extern void BtApp_HF_SetCall(void);
//extern void BtApp_HF_SetMode(void);
extern void BtApp_HF_SetRing(void);
extern void BtApp_HF_SetCallLineID(const char *pStrCallLineID);
extern void BtApp_HF_SetCallLineID_Private_Number(void);
extern void BtApp_HF_SetCcwaNumberID(const char *pStrCcwaNumberID);
extern void BtApp_HF_SetCcwaNumberID_Private_Number(void);
extern void BtApp_HF_SetOutgoingNumber(const char *pStrOutgoingNumber, int len);
extern void BtApp_HF_SetOutgoing_CID_NotSupport(void);
extern void BtApp_HF_SetCindSupport(char *pStrCindSupport, int len);
extern void BtApp_HF_SetCallsetupIndex(unsigned long ulCallsetupIndex);
extern void BtApp_HF_SetServiceIndex(unsigned long ulServiceIndex);
extern void BtApp_HF_SetRoamIndex(unsigned long ulRoamIndex);
extern void BtApp_HF_SetSignalIndex(unsigned long ulSignalIndex);
extern void BtApp_HF_SetBattchgIndex(unsigned long ulBattchgIndex);
extern void BtApp_HF_SetMsgRcvdIndex(unsigned long ulMsgRcvdIndex);
extern void BtApp_HF_SetSpkVolumeIndex(unsigned long ulSpkVolIndex);
extern void BtApp_HF_SetMicGainIndex(unsigned long ulMicGainIndex);
extern void BtApp_HF_SetMicMuteStatus(void);
extern void BtApp_HF_TimeoutHfRing(unsigned char id, unsigned char loc);
extern void BtApp_HF_TimeoutHfPrivateNumber(unsigned char id, unsigned char loc);
extern void BtApp_HF_Timer_CallTimeStamp(unsigned char id, unsigned char loc);
extern void BtApp_HF_Ring_DRV_Open(void);
extern void BtApp_HF_Ring_DRV_Close(void);
extern int BtApp_HF_Open_Ring(void);
extern void BtApp_HF_Close_RingEx(void);
extern void BtApp_HF_Close_Ring(void);
extern void BtApp_HF_RingSetParameter(unsigned short uSampleRate, unsigned char uCh);
extern void BtApp_HF_RingGetParameter(unsigned short *uSampleRate, unsigned char *uCh);
extern void BtApp_HF_SetRingStart(unsigned int iID);
extern void BtApp_HF_SetIRingEnd(void);
extern void BtApp_HF_Call_DRV_Open(void);
extern void BtApp_HF_Call_DRV_Close(void);
extern int BtApp_HF_Open_Call(void);
extern void BtApp_HF_Close_CallEx(void);
extern void BtApp_HF_Close_Call(void);
extern unsigned char BtApp_HF_SetCallParameters(unsigned int iID);
extern void BtApp_HF_SetCallStart(unsigned int iID); /* Telechips' 2009.08 QA - Ver. 6.24 */
extern void BtApp_HF_SetCallEnd(void);
extern void BtApp_HF_SetAudioStart(unsigned int iID);
extern void BtApp_HF_SetAudioEnd(void);
extern void BtApp_HF_SetDial(void);
extern void BtApp_HF_SetDialEnd(void);
extern void BtApp_HF_SetVoiceRecognition(void);
extern void BtApp_HF_SetVoiceRecognitionLearning(void);
extern void BtApp_HF_SetVoiceRecognitionOnOff(void);
extern void BtApp_HF_TerminateVoiceRecognition(void);
extern void BtApp_HF_AnswerCall(void);
extern void BtApp_HF_RejectCall(void);
extern void BtApp_HF_DrawCall(void);
extern void BtApp_HF_TerminateCall(void);
extern void BtApp_HF_Redial(void);
extern void BtApp_HF_InputNumberDial(void);
extern void BtApp_HF_Timer_InputNumberDial(unsigned char param1, unsigned char param2);
extern void UI_BT_HfPresetNumberDial(void);
#ifdef BT_HFP_MIC_MUTE
extern void BtApp_HF_MicMuteToggle(void);
#endif
extern void BtApp_HF_RHPutOnHold(void);
extern void BtApp_HF_RHAcceptIncoming(void);
extern void BtApp_HF_RHRejectIncoming(void);
extern void BtApp_HF_RHReadStatus(void);
extern void BtApp_HF_HoldAndAcceptCall(void);
extern void BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall1(void);
extern void BtApp_HF_RequestPrivateConsultationModeWithSpecifiedCall2(void);
extern void BtApp_HF_ReleaseAndAcceptCall(void);
extern void BtApp_HF_ReleasesSpecifiedActiveCall1(void);
extern void BtApp_HF_ReleasesSpecifiedActiveCall2(void);
extern void BtApp_HF_AddHeldCallToConversation(void);
extern void BtApp_HF_ExplicitCallTransfer(void);
extern void BtApp_HF_ReleaseAllHeldCalls(void);
extern void BtApp_HF_ExcuteResponseAndHoldControl(void);
#ifdef BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE
extern void BtApp_HF_ExcuteEnhancedCallControl(void);
#endif
extern void BtApp_HF_GetClcc(void);
extern void BtApp_HF_SetBia(void);
extern void BtApp_HF_SetBiev(void);
extern void BtApp_HF_SetOutgoingStart(void);
extern void BtApp_HF_SetOutgoingEnd(void);
#ifdef BT_HFP_WIDEBAND_SPEECH
extern unsigned int BtApp_HF_GetLastCodec(void);
extern void BtApp_HF_SetLastCodec(unsigned int uiCodec);
extern unsigned int BtApp_HF_GetCurCodec(void);
extern void BtApp_HF_SetCurCodec(unsigned int uiCodec);
#endif
extern void BtApp_HF_SetAutoAnswerIndex(unsigned long ulIndex);
extern void BtApp_HF_SetDefaultMicGainIndex(unsigned long ulIndex);
extern void BtApp_HF_SetSmsAlarmIndex(unsigned long ulIndex);
extern void BtApp_HF_SetRingPriorityIndex(unsigned long ulIndex);
extern void BtApp_HF_SetRingSelectIndex(unsigned long ulIndex);
#ifdef BT_HFP_LOOPBACK_TEST
extern void BtApp_HF_ToggleLoopbackTest(void);
#endif
extern void BtApp_HF_SetDtmf(unsigned char data);

extern void BtApp_HF_SetCallIdRcvd(unsigned char ucRcvd);
extern unsigned char BtApp_HF_GetCallIdRcvd(void);
#ifdef BT_HFP_AT_SIRI
extern void BtApp_HF_AtCmdSendSiri(void);
#endif
extern void BtApp_HF_AtCmdSendBattCharReq(void);
extern void BtApp_HF_AtCmdSendSignQualReq(void);
extern int BtApp_HF_AtCmdGetToken(unsigned char *AtCmdString, char **String, unsigned int index);
extern void BtApp_HF_AtCmdINDHandler(unsigned int result, const unsigned char *atCmdString);
extern void BtApp_HF_AtCmdCFMHandler(int result,unsigned int index);

#endif /* BT_HANDSFREE_UNIT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __BTAPP_HF_H__ */
