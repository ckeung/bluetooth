#ifndef BTHANDFREE_H
#define BTHANDFREE_H
#include <QObject>

#include "inc/BtBase.h"
#include "BtSys.h"
#include "TcBtHFTypes.h"
class BtHandFree
{
public:
    BtHandFree();
    ~BtHandFree();
    int EventProcess(unsigned int event_type, int param1, const char* param2);
   void UIBTHFMsgProcessor(BT_HF_MSG_t *uiMessage);
   void BtApp_HF_CmdOpenAudio(void);
    void BtApp_HF_CmdCloseAudio(void);
    void BtApp_HF_CmdSetVol(int value);
    void BtApp_HF_CmdAccOn(void);
    void BtApp_HF_CmdAccOff(void);
    void BtApp_HF_CmdPwrOn(void);
    void BtApp_HF_CmdPwrOff(unsigned char devno);
    /*activate*/
    void BtApp_HF_CmdUnitActivate(void);
    void BtApp_HF_CmdUnitDeactivate(void);
    /*connect*/
    void BtApp_HF_CmdUnitConnect(TcBtBdAddr *pBdAddr);
    void BtApp_HF_CmdUnitDisconnect(unsigned char devno);
    /*menu*/
    void BtApp_HF_CmdOptionsAutoAnswer(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsMicGain(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsSMSAlarm(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsRing(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsBuiltIn(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsNREC(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsClcc(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsBia(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdOptionsBiev(unsigned char devno, unsigned char value);
    /* status - DIAL */
    void BtApp_HF_CmdDialRedial(unsigned char devno);
    void BtApp_HF_CmdDialPlaceByNumber(unsigned char devno, unsigned char size, unsigned char* data);
    /* status - VOICE */
     void BtApp_HF_CmdVoiceRecognition(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionStart(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionEnd(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionLearn(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionLearnStart(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionLearnEnd(unsigned char devno);
    void BtApp_HF_CmdVoiceRecognitionTerminate(unsigned char devno);
    /* status - OUTGOING */
    void BtApp_HF_CmdOutgoingStart(unsigned char devno);
    void BtApp_HF_CmdOutgoingEnd(unsigned char devno);
    void BtApp_HF_CmdOutgoingDraw(unsigned char devno);
    /* status - INCOMING */
    void BtApp_HF_CmdIncomingStart(unsigned char devno);
    void BtApp_HF_CmdIncomingInbandStart(unsigned char devno);
    void BtApp_HF_CmdIncomingAudioStart(unsigned char devno);
    void BtApp_HF_CmdIncomingEnd(unsigned char devno);
    void BtApp_HF_CmdIncomingAnswer(unsigned char devno);
    void BtApp_HF_CmdIncomingReject(unsigned char devno);
    void BtApp_HF_CmdIncomingResponseAndHold(unsigned char devno, unsigned char value);
    /* status - CALL */
    void BtApp_HF_CmdCallStart(unsigned char devno);
    void BtApp_HF_CmdCallEnd(unsigned char devno);
    void BtApp_HF_CmdCallTerminate(unsigned char devno);
    void BtApp_HF_CmdCallAudioOnOff(unsigned char devno);
    void BtApp_HF_CmdCallAudioAccept(unsigned char devno, unsigned char value);
     void BtApp_HF_CmdCallMicMute(unsigned char devno);
    void BtApp_HF_CmdCallDTMF(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdReleaseAllHeldCalls(unsigned char devno);
    void BtApp_HF_CmdCallHoldAndAccept(unsigned char devno);
    void BtApp_HF_CmdCallReleaseAndAccept(unsigned char devno);
    void BtApp_HF_CmdCallExcuteEnhancedCall(unsigned char devno, unsigned char value);
    void BtApp_HF_CmdCallLookbackTest(unsigned char devno);
    #ifdef BT_HFP_UPDATE_SUPPORTED_CODEC
    void BtApp_HF_CmdCommonUpdateSupportedCodec(unsigned char ucMask, unsigned char boEnable);
    #endif
    void BtApp_HF_CmdCommonMicUp(unsigned char devno);
    void BtApp_HF_CmdCommonMicDown(unsigned char devno);
    void BtApp_HF_CmdCommonSpeakerUp(unsigned char devno);
    void BtApp_HF_CmdCommonSpeakerDown(unsigned char devno);
    void BtApp_HF_CmdGetInformation(unsigned char devno);
    void BtApp_HF_CmdUtilSimulation(unsigned char devno);


};
BtHandFree *GetglobalBtHandFree();
#define G_BtHf 	(GetglobalBtHandFree())
#endif // BTHANDFREE_H
