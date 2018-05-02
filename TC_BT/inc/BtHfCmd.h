#ifndef BTHFCMD_H
#define BTHFCMD_H
#include "inc/BtBase.h"
#include "TC_BT_HF_CMD.h"
class BtHfCmd
{
public :
    BtHfCmd();
    ~BtHfCmd();
    int TC_BT_HF_Activate(void);
    int TC_BT_HF_Deactivate(void);
    int TC_BT_HF_ConfigLowPower(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_ConfigAudio(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_ServiceConnect(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_CancelConnect(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_Disconnect(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_AudioConnect(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_AudioAcceptConnect(const TcBtBdAddr *pBdAddr,TcBtBool enable);
    int TC_BT_HF_AudioDisconnect(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_GetAllStatusInd(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_GetCurrentOperatorSelection(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_GetSubscriberNumberInformation(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_GetCurrentCallList(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_SetExtendedAgErrorResultCode(const TcBtBdAddr *pBdAddr, TcBtBool enable);
    int TC_BT_HF_SetCallNotificationInd(const TcBtBdAddr *pBdAddr, TcBtBool enable);
    int TC_BT_HF_SetCallWaitingNotification(const TcBtBdAddr *pBdAddr, TcBtBool enable);
    int TC_BT_HF_SetStatusIndicatorUpdate(const TcBtBdAddr *pBdAddr, TcBtBool enable);
    int TC_BT_HF_SetEchoAndNoise(const TcBtBdAddr *pBdAddr, TcBtBool enable);
    int TC_BT_HF_SetVoiceRecognition(const TcBtBdAddr *pBdAddr, TcBtBool start);
    int TC_BT_HF_BtInput(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_GenerateDtmf(const TcBtBdAddr *pBdAddr, TcBtUint8 dtmf);
    int TC_BT_HF_SpeakerGainStatus(const TcBtBdAddr *pBdAddr, TcBtUint8 gain);
    int TC_BT_HF_MicGainStatus(const TcBtBdAddr *pBdAddr, TcBtUint8 gain);
    int TC_BT_HF_Dial(const TcBtBdAddr *pBdAddr, TcBtHfDialEnum command, const TcBtString *number);
    int TC_BT_HF_CallAnswer(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_CallEnd(const TcBtBdAddr *pBdAddr);
    int TC_BT_HF_CallHandling(const TcBtBdAddr *pBdAddr, TcBtHfCallHandlingEnum command, TcBtUint8 index);
    int TC_BT_HF_AtCmd(const TcBtBdAddr *pBdAddr, const TcBtString *atCmdString, TcBtUint32 atCmdIndex);
    int TC_BT_HF_SecurityIn(void);
    int TC_BT_HF_SecurityOut(void);
    int TC_BT_HF_DeregisterTime(TcBtUint8 waitSeconds);
    int TC_BT_HF_IndicatorActivation(const TcBtBdAddr *pBdAddr,  TcBtUint16 indicatorBitMask);
    int TC_BT_HF_UpdateSupportedCodec(const TcBtBdAddr *pBdAddr, TcBtUint32 codecMask, TcBtBool enable);
    int TC_BT_HF_UpdateHFIndicator(const TcBtBdAddr *pBdAddr, TcBtUint16 ind);
    };
BtHfCmd *GetglobalBtHfCmd();
#define G_BtHfCmd 	(GetglobalBtHfCmd())
#endif // BTHFCMD_H
