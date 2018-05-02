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

#ifndef __BTAPP_HF_EVT_H__
#define __BTAPP_HF_EVT_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void BtApp_HF_EvtPowerOff(unsigned char ucDevNo);
extern void BtApp_HF_EvtNotActivated(unsigned char ucDevNo);
extern void BtApp_HF_EvtAlreadyActivated(unsigned char ucDevNo);
extern void BtApp_HF_EvtActivateCfm(unsigned char ucDevNo);
extern void BtApp_HF_EvtActivateErr(unsigned char ucDevNo);
extern void BtApp_HF_EvtDeactivateCfm(unsigned char ucDevNo);
extern void BtApp_HF_EvtDeactivateErr(unsigned char ucDevNo);
extern void BtApp_HF_EvtServiceConnectInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtServiceConnectErr(unsigned char ucDevNo);
extern void BtApp_HF_EvtAlreadyConnected(unsigned char ucDevNo);
extern void BtApp_HF_EvtServiceConnectNotificationInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtDisconnectByRemote(unsigned char ucDevNo);
extern void BtApp_HF_EvtNotConnected(unsigned char ucDevNo);
extern void BtApp_HF_EvtDisconnectInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtDisconnectErr(unsigned char ucDevNo);
extern void BtApp_HF_EvtDisconnectLinkLoss(unsigned char ucDevNo);
extern void BtApp_HF_EvtAudioInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtAudioAcceptInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtUpdateSupportedCodec(unsigned char ucDevNo);
extern void BtApp_HF_EvtSelectedCodecInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtIncoming(unsigned char id, unsigned char ucDevNo);
extern void BtApp_HF_EvtIncomingInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtIncomingTimeoutInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtIncomingEndInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievCall(unsigned char id, unsigned char ucDevNo);
extern void BtApp_HF_EvtCievCallZeroInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievCallOneInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievCallSetupInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievCallheldInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievServiceInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievRoamInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievSignalInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievBattChgInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCievMessageInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtAnswerReq(unsigned char ucDevNo);
extern void BtApp_HF_EvtRejectReq(unsigned char ucDevNo);
extern void BtApp_HF_EvtRejectErr(unsigned char ucDevNo);
extern void BtApp_HF_EvtTerminateReq(unsigned char ucDevNo);
extern void BtApp_HF_EvtPlaceReq(unsigned char ucDevNo);
extern void BtApp_HF_EvtCallAlreadyExist(unsigned char ucDevNo);
extern void BtApp_HF_EvtClipInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoing(unsigned char id, unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingNumberInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingFromHfgInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingStartInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingEndInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingRetry(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingDialInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtCallTimeStampCountid(unsigned char ucDevNo);
extern void BtApp_HF_EvtCallTimeStampClearId(unsigned char ucDevNo);
extern void BtApp_HF_EvtSetSpeakerVolumeInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtSetMicGainInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtLoopbackTestStartInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtLoopbackTestEndInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtSupportVoiceRecognitionInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtVoiceRecognitionStartInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtVoiceRecognitionEndInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtVoiceRecognitionLearnStartInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtVoiceRecognitionLearnEndInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtClccInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCcwaInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtCmeeInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOutgoingBusyLineInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtServiceConnectInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtServiceConnectErr(unsigned char ucDevNo);
extern void DMP_BT_HSEvtAlreadyConnected(unsigned char ucDevNo);
extern void DMP_BT_HSEvtNotConnected(unsigned char ucDevNo);
extern void DMP_BT_HSEvtDisconnectByRemote(unsigned char ucDevNo);
extern void DMP_BT_HSEvtDisconnectInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtDisconnectErr(unsigned char ucDevNo);
extern void DMP_BT_HSEvtDisconnectLinkLoss(unsigned char ucDevNo);
extern void DMP_BT_HSEvtRingInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtRingStartInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtRingTimeInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtRingEndInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtAudioOnInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtAudioOffInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtSetSpeakerVolumeInd(unsigned char ucDevNo);
extern void DMP_BT_HSEvtSetMicGainInd(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsAutoAnswerInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsMicGainInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsSMSAlaramInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsRingInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsBuiltInInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsNRECInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsClccInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtOptionsBiaInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtDtmfInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtGetFriendlyName(TcBtBdAddr *pBdAddr, unsigned char *FriendlyName);
extern void BtApp_HF_EvtGetInformationInf(unsigned char ucDevNo);
extern void BtApp_HF_EvtSetMicMuteInf(unsigned char ucDevNo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __BTAPP_HF_EX_H__ */
