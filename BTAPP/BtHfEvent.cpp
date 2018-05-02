//******************************header file*******************//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "BtHfEvent.h"
#include "BtApp_HF.h"
#include "BtApp_Gap.h"
#include "BtPhoneBook.h"
#include "BtSys.h"
#include "BtHfCmd.h"
#include "TC_BT/inc/TC_BT_HF_DEV.h"
#include "TC_BT/inc/TC_BT_HF_EVT.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/TC_BT_CMD.h"
#include "TC_BT/inc/TC_BT_HF_SCO.h"
#include "TC_BT/inc/BtNvDbFile.h"
//********************************static fun***************************//

static int TC_BT_HF_ServiceConnectEvt(void *pTcBtEvt);
static int TC_BT_HF_DisconnectEvt(void *pTcBtEvt);
#if 1	/* Telechips Linux HF remark */
#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ __FILE__
#endif
BtHfEvent::BtHfEvent()
{
}
BtHfEvent::~BtHfEvent()
{
}
int BtHfEvent::BtApp_HF_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;
    TcBtEvtResultEnum eResult = TC_BT_EVT_RESULT_SUCCESS_ENUM;
    evtType = event->type;
    switch(evtType)
    {
        case TC_BT_HF_ACTIVATE_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_ActivateCfm(event);
        }break;
        case TC_BT_HF_DEACTIVATE_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_DeactivateCfm(event);
        }break;
        case TC_BT_HF_STATUS_LOW_POWER_IND:
        {
            TC_BT_HF_SetDevice(event);
        }break;
        case TC_BT_HF_SERVICE_CONNECT_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_ServiceConnectInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_RegisterDevice(event);
             }
        }break;
        case TC_BT_HF_SERVICE_CONNECT_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_ServiceConnectCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_RegisterDevice(event);
             }
        }break;
        case TC_BT_HF_DISCONNECT_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_DisconnectInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_DISCONNECT_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_DisconnectCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_AUDIO_CONNECT_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_AudioConnectInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_AUDIO_CONNECT_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_AudioConnectCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_AUDIO_ACCEPT_CONNECT_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_AudioAcceptConnectInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_AUDIO_DISCONNECT_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_AudioDisconnectInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_AUDIO_DISCONNECT_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_AudioDisconnectCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_STATUS_INDICATOR_UPDATE_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_StatusIndicatorUpdateInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GET_ALL_STATUS_INDICATORS_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_GetAllStatusIndicatorsCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_GetCurrentOperatorSelectionCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_GetSubscriberNumberInformationInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_GetSubscriberNumberInformationCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GET_CURRENT_CALL_LIST_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_GetCurrentCallListInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GET_CURRENT_CALL_LIST_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_GetCurrentCallListCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SetExtendedAgErrorResultCodeCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_SetCallNotificationIndicationCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SetCallWaitingNotificationCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SetStatusIndicatorUpdateCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SET_ECHO_AND_NOISE_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SetEchoAndNoiseCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SET_VOICE_RECOGNITION_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_SetVoiceRecognitionCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_SET_VOICE_RECOGNITION_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SetVoiceRecognitionInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_BT_INPUT_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_BtInputCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_INBAND_RING_SETTING_CHANGED_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_InbandRingSettingChangedInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_GENERATE_DTMF_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_GenerateDtmfCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SPEAKER_GAIN_STATUS_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_SpeakerGainStatusCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_SPEAKER_GAIN_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SpeakerGainInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_MIC_GAIN_STATUS_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_MicGainStatusCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_MIC_GAIN_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_MicGainInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_DIAL_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_DialCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_CALL_ANSWER_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_CallAnswerCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_CALL_END_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_CallEndCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_CALL_HANDLING_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_CallHandlingInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_CALL_HANDLING_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_CallHandlingCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_AT_CMD_IND:
        {
           eResult =  (TcBtEvtResultEnum) TC_BT_HF_AtCmdInd(event);
           if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
           {
                  TC_BT_HF_SetDevice(event);
           }
        }break;
        case TC_BT_HF_AT_CMD_CFM:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_AtCmdCfm(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_CALL_RINGING_IND:
        {
            eResult =  (TcBtEvtResultEnum)TC_BT_HF_CallRingingInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_CALL_NOTIFICATION_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_CallNotificationInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_CALL_WAITING_NOTIFICATION_IND:
        {
            eResult = (TcBtEvtResultEnum) TC_BT_HF_CallWaitingNotificationInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_SECURITY_IN_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SecurityInCfm(event);
        }break;
        case TC_BT_HF_SECURITY_OUT_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SecurityOutCfm(event);
        }break;
        case TC_BT_HF_DEREGISTER_TIME_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_DeregisterTimeCfm(event);
        }break;
        case TC_BT_HF_INDICATOR_ACTIVATION_CFM:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_IndicatorActivationCfm(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_UPDATE_SUPPORTED_CODEC_CFM:
        {
           eResult =   (TcBtEvtResultEnum)TC_BT_HF_UpdateSupportedCodecCfm(event);
        }break;
        case TC_BT_HF_SELECTED_CODEC_IND:
        {
             eResult = (TcBtEvtResultEnum)TC_BT_HF_SelectedCodecInd(event);
             if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
             {
                    TC_BT_HF_SetDevice(event);
             }
        }break;
        case TC_BT_HF_SERVICE_CONNECT_NOTIFICATION_IND:
        {
            eResult = (TcBtEvtResultEnum) TC_BT_HF_ServiceConnectNotificationInd(event);
            if (eResult == TC_BT_EVT_RESULT_SUCCESS_ENUM )
            {
                   TC_BT_HF_SetDevice(event);
            }
        }break;
        case TC_BT_HF_C2C_SF_IND:
        {
                TC_BT_HF_SetDevice(event);
           }break;
        default :
        {}break;
    }
}

int TC_BT_HF_RegisterDevice(void *pTcBtEvt)
{
    TcBtHfEvt *pTcBtHfEvt;
    int eResult=TC_BT_EVT_RESULT_SUCCESS_ENUM;

    pTcBtHfEvt = (TcBtHfEvt *)pTcBtEvt;

    (void)TC_BT_HF_DEV_Register(&(pTcBtHfEvt->bdAddr));

    eResult=TC_BT_HF_DEV_SetNum(&(pTcBtHfEvt->bdAddr));

    return eResult;
}

int TC_BT_HF_DeregisterDevice(void *pTcBtEvt)
{
    TcBtHfEvt *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfEvt *)pTcBtEvt;

    (void)TC_BT_HF_DEV_Deregister(&(pTcBtHfEvt->bdAddr));

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetDevice(void *pTcBtEvt)
{
    TcBtHfEvt *pTcBtHfEvt;
    int eResult=TC_BT_EVT_RESULT_SUCCESS_ENUM;
    pTcBtHfEvt = (TcBtHfEvt *)pTcBtEvt;

    eResult=TC_BT_HF_DEV_SetNum(&(pTcBtHfEvt->bdAddr));

    return eResult;
}

int TC_BT_HF_ActivateCfm(void *pTcBtEvt)
{
    TcBtHfActivateCfm *pTcBtHfEvt;
    TcBtUint16 gapResult;

    pTcBtHfEvt = (TcBtHfActivateCfm *)pTcBtEvt;

    if (pTcBtHfEvt->resultCode == TC_BT_RESULT_CODE_HF_SUCCESS)
    {
#if 0	// reserved
        TC_BT_HF_DeregisterTime(40);
#endif
        BtApp_HF_ActivateCFMHandler(TRUE);
#ifdef BT_AUTO_CONNECTION
        //BtApp_GAP_AutoConnect();
        if(BtApp_GAP_IsInRecoveryList(TC_BT_HF_PROFILE_UUID) != TRUE)
        {
            TcBtBdAddr *pAddr = TC_BT_SC_DB_ReadLastDevAddr();

            if((pAddr != NULL) && (TC_BT_BdAddrEqZero((const TcBtBdAddr *)pAddr)==FALSE))
            {
                BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_IN);
            }
        }
#endif
        gapResult = BTAPP_GAP_ACTV_RESULT_SUCCESS;
    }
    else
    {
        BtApp_HF_ActivateCFMHandler(FALSE);
        gapResult = BTAPP_GAP_ACTV_RESULT_FAILURE;
    }
    BtApp_GAP_ReflectActivateResult(TC_BT_HF_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);
	printf("TC_BT_HF_ActivateCfm (resultCode = 0x%04x, resultSupplier = 0x%04x)\n", pTcBtHfEvt->resultCode, pTcBtHfEvt->resultSupplier);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_DeactivateCfm(void *pTcBtEvt)
{
    TcBtHfDeactivateCfm *pTcBtHfEvt;
    TcBtUint16 result;

    pTcBtHfEvt = (TcBtHfDeactivateCfm *)pTcBtEvt;

    if (pTcBtHfEvt->resultCode == TC_BT_RESULT_CODE_HF_SUCCESS)
    {
        BtApp_HF_DeactivateCFMHandler(TRUE);
        result = BTAPP_GAP_DACT_RESULT_SUCCESS;
    }
    else
    {
        BtApp_HF_DeactivateCFMHandler(FALSE);
        result = BTAPP_GAP_DACT_RESULT_FAILURE;
    }

    BtApp_GAP_ReflectDeactivateResult(TC_BT_HF_PROFILE_UUID, result, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_ConfigLowPowerCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_NOT_SUPPORT_ENUM;
}

int TC_BT_HF_ConfigAudioInd(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_ConfigAudioCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

static int TC_BT_HF_ServiceConnectEvt(void *pTcBtEvt)
{
    TcBtHfServiceConnectInd *pTcBtHfEvt;
    TcBtUint16 gapResult;

    pTcBtHfEvt = (TcBtHfServiceConnectInd *)pTcBtEvt;

    if ((pTcBtHfEvt->resultCode == TC_BT_RESULT_CODE_HF_SUCCESS) &&
        (pTcBtHfEvt->resultSupplier == TC_BT_SUPPLIER_HF))
    {
            (void)BtApp_GAP_SetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_STATE_CONNECTED);
            gapResult = BTAPP_GAP_CONN_RESULT_SUCCESS;

            /* Telechips Linux HFP remark - Event routine changed									*
             * DMP excuted after TC_BT runs, but Linux system DMP changed BtApp and call directly 	*
             * So, send connect IND after parsing supported features							  	*/
#if 0	/* Telechips Linux HFP remark */
            BtApp_HF_ServiceConnectINDHandler(TRUE);
#endif

            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_INBAND_RINGING)!=0)
            {
                BtApp_HF_InbandRingChangedInd(1);
            }
            else
            {
                BtApp_HF_InbandRingChangedInd(0);
            }

            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_VOICE_RECOGNITION)!=0)
            {
                BtApp_HF_VoiceRecognitionAbilityInd(1);
            }
            else
            {
                BtApp_HF_VoiceRecognitionAbilityInd(0);
            }
            if ((((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL)!=0)
                || (pTcBtHfEvt->remoteVersion==TC_BT_FIRST_HFP_NO_ESCO))
                || (pTcBtHfEvt->remoteVersion==TC_BT_LAST_HFP_NO_ESCO))
            {
                BtApp_HF_RejectAbilityInd(1);
            }
            else
            {
                BtApp_HF_RejectAbilityInd(0);
            }
            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_THREE_WAY_CALLING)!=0)
            {
                BtApp_HF_SupportThreeWayCallingInd(1);
            }
            else
            {
                BtApp_HF_SupportThreeWayCallingInd(0);
            }
            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_EC_NR_FUNCTION)!=0)
            {
                BtApp_HF_SupportEcNrFunctionInd(1);
            }
            else
            {
                BtApp_HF_SupportEcNrFunctionInd(0);
            }
            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS)!=0)
            {
                BtApp_HF_SupportEnhancedCallStatusInd(1);
            }
            else
            {
                BtApp_HF_SupportEnhancedCallStatusInd(0);
            }
            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL)!=0)
            {
                BtApp_HF_SupportEnhancedCallControlInd(1);
            }
            else
            {
                BtApp_HF_SupportEnhancedCallControlInd(0);
            }
            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES)!=0)
            {
                BtApp_HF_SupportExtendedErrorCodesInd(1);
            }
            else
            {
                BtApp_HF_SupportExtendedErrorCodesInd(0);
            }

            if ((pTcBtHfEvt->supportedFeatures & TC_BT_HFG_SUPPORT_CODEC_NEGOTIATION)!=0)
            {
                BtApp_HF_SupportCodecNegotiationInd(1);
            }
            else
            {
                BtApp_HF_SupportCodecNegotiationInd(0);
            }


#if 0   /* Linux HFP remark */
            if ((pTcBtHfEvt->connectionType == TC_BT_HF_CONNECTION_HF)!=0)
            {
                BtApp_HF_RemoteVersionInd(pTcBtHfEvt->remoteVersion);
            }
            else
            {
                BtApp_HF_RemoteVersionInd(0);
            }
#else
            BtApp_HF_RemoteVersionInd(pTcBtHfEvt->remoteVersion);
#endif

            if(pTcBtHfEvt->chldString != NULL)
            {
                BtApp_HF_SupportChldInd(0xFF);
                if(strstr(&(pTcBtHfEvt->chldString[0]), "0")!=NULL)
                {
                    BtApp_HF_SupportChldInd(0);
                }
                if(strstr(&(pTcBtHfEvt->chldString[0]), "1x")!=NULL)
                {
                    BtApp_HF_SupportChldInd(1);
                }
                if(strstr(&(pTcBtHfEvt->chldString[0]), "2x")!=NULL)
                {
                    BtApp_HF_SupportChldInd(2);
                }
                if(strstr(&(pTcBtHfEvt->chldString[0]), "3")!=NULL)
                {
                    BtApp_HF_SupportChldInd(3);
                }
                if(strstr(&(pTcBtHfEvt->chldString[0]), "4")!=NULL)
                {
                    BtApp_HF_SupportChldInd(4);
                }
            }

#if 1	/* Telechips Linux HFP remark */
            BtApp_HF_ServiceConnectINDHandler(TRUE);
#endif

            BtApp_HF_CindSupportIndHandler((unsigned char *)pTcBtHfEvt->indicatorSupported, (unsigned char *)pTcBtHfEvt->indicatorValue);

#if 0
            BtApp_SYS_UpdateRemoteBdAddr(&(pTcBtHfEvt->bdAddr));
#if 1 /* Telechips' 2008.12 QA - 3.12, 3.16 */
            BtApp_SYS_UpdateRemoteFriendlyNameByScDbAddr(&(pTcBtHfEvt->bdAddr));
#else
            BtApp_HF_GetRemoteFriendlyName(&(myPrim->deviceAddr));
#endif
#endif
            (void)TC_BT_SC_DB_ToTheTop((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr));
#if 1 /* Telechips' 2008.12 QA - 3.12, 3.16 */
            TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr), TC_BT_HFG_PROFILE_UUID);
#endif
#ifdef BT_AUTO_CONNECTION
            TC_BT_SC_DB_WriteLastDevAddr((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr));
#endif
//			BtApp_HF_ServiceConnectINDHandler(TRUE);

        if(pTcBtHfEvt->type == TC_BT_HF_SERVICE_CONNECT_IND)
        {
#if defined(BT_PBAP_CLIENT) || defined(BT_MAP_CLIENT)
            BtApp_GAP_ObexSubConnectionProcess((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr));
#endif
            BtApp_GAP_IncomingConnectionBackupProcess((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr));
        }

        //Test
        //TC_BT_CM_SetQOS(pTcBtHfEvt->bdAddr, 0, 2, 400, 800, 11250*8, 11250);
    }
    else if(pTcBtHfEvt->resultSupplier == TC_BT_SUPPLIER_BSAS_CANCEL_CONNECT)
    {
        if(BtApp_GAP_GetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_UNUSED_INST_IDX) == BTAPP_GAP_STATE_CONNECT_CANCELLING)
        {
            (void)BtApp_GAP_SetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_STATE_CONNECTING);
        }
        gapResult = BTAPP_GAP_CONN_RESULT_CANCEL_FAILED;
    }
    else
    {
        if((pTcBtHfEvt->resultSupplier == TC_BT_SUPPLIER_HF) && (pTcBtHfEvt->resultCode == TC_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT))
        {
            (void)BtApp_GAP_SetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_STATE_DISCONNECTED);
            gapResult = BTAPP_GAP_CONN_RESULT_CANCELLED;
        }
        else
        {
            (void)BtApp_GAP_SetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_STATE_DISCONNECTED);
                gapResult = BTAPP_GAP_CONN_RESULT_FAILURE;

            if (1 /*pTcBtHfEvt->bdAddr ... */ )
            {
                BtApp_HF_ServiceConnectINDHandler(FALSE);
            }
            else
            {
                //Bluetooth_HsServiceConnectINDHandler(FALSE);
            }
        }

        (void)TC_BT_HF_DeregisterDevice(pTcBtEvt);
    }

    (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtHfEvt->bdAddr), TC_BT_HF_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);

   printf("resultCode = 0x%04x, resultSupplier = 0x%04x\n",  pTcBtHfEvt->resultCode, pTcBtHfEvt->resultSupplier);
        printf(" BdAddr = %04x:%02x:%06x \n",  pTcBtHfEvt->bdAddr.nap, pTcBtHfEvt->bdAddr.uap, pTcBtHfEvt->bdAddr.lap);
        printf(" Network = %02d\n",  pTcBtHfEvt->network);
        printf(" ServiceName = %s\n", pTcBtHfEvt->serviceName);
        printf(" SupportedFeatures = %02x\n", pTcBtHfEvt->supportedFeatures);
        printf(" ConnectionType = %02d\n",  pTcBtHfEvt->connectionType);
        printf(" IndicatorSupported = %s\n",  pTcBtHfEvt->indicatorSupported);
        printf("IndicatorValue = %s\n",  pTcBtHfEvt->indicatorValue);
        printf("ChldString = %s\n",  pTcBtHfEvt->chldString);
        printf("%RemoteVersion = %04x\n",  pTcBtHfEvt->remoteVersion);
    TC_BT_Pfree(pTcBtHfEvt->serviceName);
    TC_BT_Pfree(pTcBtHfEvt->indicatorSupported);
    TC_BT_Pfree(pTcBtHfEvt->indicatorValue);
    TC_BT_Pfree(pTcBtHfEvt->chldString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_ServiceConnectInd(void *pTcBtEvt)
{

    return TC_BT_HF_ServiceConnectEvt(pTcBtEvt);
}

int TC_BT_HF_ServiceConnectCfm(void *pTcBtEvt)
{

    return TC_BT_HF_ServiceConnectEvt(pTcBtEvt);
}

static int TC_BT_HF_DisconnectEvt (void *pTcBtEvt)
{
    TcBtHfDisconnectInd *pTcBtHfEvt;
    unsigned char ucDevNo;
    TcBtUint16 gapResult;

    ucDevNo=BtApp_HF_GetCurrDevNum();

    pTcBtHfEvt = (TcBtHfDisconnectInd *)pTcBtEvt;

    if(pTcBtHfEvt->reasonSupplier == TC_BT_SUPPLIER_BSAS_DISCONNECT)
    {
        if(BtApp_GAP_GetHfpState(&pTcBtHfEvt->bdAddr, BTAPP_GAP_UNUSED_INST_IDX) == BTAPP_GAP_STATE_DISCONNECTING)
        {
            (void)BtApp_GAP_SetHfpState(&(pTcBtHfEvt->bdAddr), BTAPP_GAP_STATE_CONNECTED);
        }
        gapResult = BTAPP_GAP_DISC_RESULT_FAILURE;
    }
    else
    {
        (void)BtApp_GAP_SetHfpState(&(pTcBtHfEvt->bdAddr), BTAPP_GAP_STATE_DISCONNECTED);

        if (1 /*pTcBtHfEvt->bdAddr ... */ )
        {
            BtApp_HF_InbandRingChangedInd(0);
            BtApp_HF_VoiceRecognitionAbilityInd(0);
            BtApp_HF_VoiceRecognitionInd(0);
            BtApp_HF_RejectAbilityInd(0);
            BtApp_HF_SupportChldInd(0xFF);
            BtApp_HF_SupportThreeWayCallingInd(0);
            BtApp_HF_SupportEcNrFunctionInd(0);
            BtApp_HF_SupportEnhancedCallStatusInd(0);
            BtApp_HF_SupportEnhancedCallControlInd(0);
            BtApp_HF_SupportExtendedErrorCodesInd(0);
            BtApp_HF_RemoteVersionInd(0);
            BtApp_HF_DisconnectINDHandler(pTcBtHfEvt->reasonCode);
        }
        else
        {
            //Bluetooth_HsDisconnectINDHandler(pTcBtHfEvt->reasonCode);
        }

        if(gBtAppHfStrNetworkOperator[ucDevNo]!=NULL)
        {
            TC_BT_Pfree(gBtAppHfStrNetworkOperator[ucDevNo]);
            gBtAppHfStrNetworkOperator[ucDevNo] = NULL;
        }

        (void)TC_BT_HF_DeregisterDevice(pTcBtEvt);

        if ((pTcBtHfEvt->reasonCode == TC_BT_HCI_ERROR_CONN_TIMEOUT) &&
            (pTcBtHfEvt->reasonSupplier == TC_BT_SUPPLIER_HCI))
        {
        #ifdef BT_AUTO_CONNECTION
            BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_IN);
        #endif
            gapResult = BTAPP_GAP_DISC_RESULT_TIMEOUT;
        }
        else if((pTcBtHfEvt->reasonSupplier == TC_BT_SUPPLIER_HF) && (pTcBtHfEvt->reasonCode == TC_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT))
        {
            gapResult = BTAPP_GAP_DISC_RESULT_CONN_CANCELLED;
        }
        else
        {
            gapResult = BTAPP_GAP_DISC_RESULT_SUCCESS;
        }
    }

    (void)BtApp_GAP_ReflectDiscResult((const TcBtBdAddr *)&pTcBtHfEvt->bdAddr, TC_BT_HF_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);


    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_DisconnectInd(void *pTcBtEvt)
{
#if defined(BT_PBAP_CLIENT)
    TcBtHfDisconnectInd *evt = (TcBtHfDisconnectInd *)pTcBtEvt;

#ifdef BT_PBAP_MULTISERVER
    BtApp_PB_MultiServerEvtHandler(TC_BT_PB_MULTI_TERMINATE, (const TcBtBdAddr *)&evt->bdAddr);
#else
    BtApp_PB_PacDisconnect(); /* for PBAP PTS TEST */
#endif
    BtApp_GAP_ObexSubDisconnectionProcess((const TcBtBdAddr *)&evt->bdAddr);
#endif

    return TC_BT_HF_DisconnectEvt(pTcBtEvt);
}

int TC_BT_HF_DisconnectCfm(void *pTcBtEvt)
{

    return TC_BT_HF_DisconnectEvt(pTcBtEvt);
}

int TC_BT_HF_AudioConnectInd(void *pTcBtEvt)
{
    unsigned char retVal;
    TcBtHfAudioConnectInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfAudioConnectInd *)pTcBtEvt;

    if ((pTcBtHfEvt->resultSupplier == TC_BT_SUPPLIER_HF) && (pTcBtHfEvt->resultCode == TC_BT_RESULT_CODE_HF_SUCCESS))
    {
        retVal = TRUE;
        (void)TC_BT_HF_SCO_RegisterHandle(&(pTcBtHfEvt->bdAddr), pTcBtHfEvt->scoHandle);
    }
    else
    {
        retVal = FALSE; /* TO DO: */
    }

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_AudioINDHandler(retVal, pTcBtHfEvt->scoHandle);
    }
    else
    {
        //(void)Bluetooth_HsAudioINDHandler(retVal, pTcBtHfEvt->scoHandle);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_AudioConnectCfm(void *pTcBtEvt)
{
    return TC_BT_HF_AudioConnectInd(pTcBtEvt);
}

int TC_BT_HF_AudioAcceptConnectInd(void *pTcBtEvt)
{
    TcBtHfAudioAcceptConnectInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfAudioAcceptConnectInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_AudioAcceptINDHandler(pTcBtHfEvt->linkType);
    }
    else
    {
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_AudioDisconnectInd(void *pTcBtEvt)
{
    TcBtHfAudioDisconnectInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfAudioDisconnectInd *)pTcBtEvt;

    (void)TC_BT_HF_SCO_DeregisterHandle(&(pTcBtHfEvt->bdAddr), pTcBtHfEvt->scoHandle);

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_AudioINDHandler(FALSE, pTcBtHfEvt->scoHandle);
    }
    else
    {
        //(void)Bluetooth_HsAudioINDHandler(FALSE, pTcBtHfEvt->scoHandle);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_AudioDisconnectCfm(void *pTcBtEvt)
{
    TcBtHfAudioDisconnectCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfAudioDisconnectCfm *)pTcBtEvt;

    (void)TC_BT_HF_SCO_DeregisterHandle(&(pTcBtHfEvt->bdAddr), pTcBtHfEvt->scoHandle);

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_AudioINDHandler(FALSE, pTcBtHfEvt->scoHandle);
    }
    else
    {
        //(void)Bluetooth_HsAudioINDHandler(FALSE, pTcBtHfEvt->scoHandle);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_StatusIndicatorUpdateInd(void *pTcBtEvt)
{
    TcBtHfStatusIndicatorUpdateInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfStatusIndicatorUpdateInd *)pTcBtEvt;

    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))==FALSE /* Jolla Sailfish 2014 UPF */)
    {
        return TC_BT_EVT_RESULT_NATIVE_OS_REJECT_ENUM;
    }

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        if(memcmp(pTcBtHfEvt->name,"call",5)==0)
        {
            BtApp_HF_CIEVINDCallHandler(pTcBtHfEvt->value);
        }
        else if( ((memcmp(pTcBtHfEvt->name,"callsetup",10)==0)
        || (memcmp(pTcBtHfEvt->name,"call setup",11)==0))
        || (memcmp(pTcBtHfEvt->name,"call_setup",11)==0) )
        {
            BtApp_HF_CIEVINDCallsetupHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"callheld",8)==0)
        {
            BtApp_HF_CIEVINDCallheldHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"signal",6)==0)
        {
            BtApp_HF_CIEVINDSingalHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"battchg",7)==0)
        {
            BtApp_HF_CIEVINDBattchgHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"message",7)==0)
        {
            BtApp_HF_CIEVINDMsgRcvdHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"service",7)==0)
        {
            BtApp_HF_CIEVINDServiceHandler(pTcBtHfEvt->value);
        }
        else if(memcmp(pTcBtHfEvt->name,"roam",5)==0)
        {
            BtApp_HF_CIEVINDRoamHandler(pTcBtHfEvt->value);
        }
        else
        {
            /* none */
        }
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->name);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}


int TC_BT_HF_GetAllStatusIndicatorsCfm(void *pTcBtEvt)
{
    TcBtHfGetAllStatusIndicatorsCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfGetAllStatusIndicatorsCfm *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        if (pTcBtHfEvt->cmeeResultCode == TC_BT_CME_SUCCESS)
        {
            BtApp_HF_CindSupportIndHandler((unsigned char *)pTcBtHfEvt->indicatorSupported, (unsigned char *)pTcBtHfEvt->indicatorValue);
        }
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->indicatorSupported);
    TC_BT_Pfree(pTcBtHfEvt->indicatorValue);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

/* Show Network Name */
int TC_BT_HF_GetCurrentOperatorSelectionCfm(void *pTcBtEvt)
{
    TcBtHfGetCurrentOperatorSelectionCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfGetCurrentOperatorSelectionCfm *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        if(pTcBtHfEvt->cmeeResultCode == TC_BT_CME_SUCCESS)
        {
            /* 1st comes Network Name */
            if (pTcBtHfEvt->copsString != NULL)
            {
               BtApp_HF_COPSTokenINDHandler(pTcBtHfEvt->copsString, strlen(pTcBtHfEvt->copsString)+1);
            }
            /* 2nd comes success msg with copsString 0 */
            else
            {
         }
        }
        else
        {

        }
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->copsString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_GetSubscriberNumberInformationInd(void *pTcBtEvt)
{
    TcBtHfGetSubscriberNumberInformationInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfGetSubscriberNumberInformationInd *)pTcBtEvt;

    TC_BT_Pfree(pTcBtHfEvt->cnumString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_GetSubscriberNumberInformationCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_GetCurrentCallListInd(void *pTcBtEvt)
{
    TcBtHfGetCurrentCallListInd *pTcBtHfEvt;
    unsigned char ucDevNo;

    ucDevNo=BtApp_HF_GetCurrDevNum();

    pTcBtHfEvt = (TcBtHfGetCurrentCallListInd *)pTcBtEvt;

    /*		When changes call in 3-way call, call number displayed didn't changed issue patch 					*
     *		CLCC Status - 0 : Active, 1 : Held, 2 : Dialing(outgoing only), 3 : Alerting(outgoing only)			*
     *					  4 : Incoming(Incoming only), 5 : Waiting(Incoming only) 								*
     *					  6 : Call held by Response and Hold													*
     *		Note : Shows call number only call & active status, if not, shows only index 1(first connected) 	*/

#if 1
    /*		This condition means at 3way calling, 1 call has terminated : 3way calling call end detection		*
     *		1->2->1->1, 1->2->3->1->2->1->1 : only works at 1 call status										*
     *      after few secs phone don't send call held, call held maybe remain as 1								*
     *      after few secs phone send wrong call held, call held maybe remain as 0 or 1							*
     *      if system & phone's held is different, check clcc held status first and send CHLD=2					*
     *																											*/
    if(gBtAppHfCallclccStatus[ucDevNo] == pTcBtHfEvt->idx)
    {
        if(gBtAppHfCallclccPrevStatus[ucDevNo] > gBtAppHfCallclccStatus[ucDevNo])
        {
            /* if system held & phone held is not mismatch, check call held after few sec */
            if((gBtAppHfCallheldStatus[ucDevNo] != 2) && (pTcBtHfEvt->status == 1/*Held*/))
            {
                BtApp_HF_CLCCINDCallheldHandler(2);
            }
        }
    }

    gBtAppHfCallclccPrevStatus[ucDevNo] = gBtAppHfCallclccStatus[ucDevNo];
    gBtAppHfCallclccStatus[ucDevNo]=pTcBtHfEvt->idx;
#else
    if(gBtAppHfCallclccStatus[ucDevNo] < pTcBtHfEvt->idx)
    {
        gBtAppHfCallclccStatus[ucDevNo] = pTcBtHfEvt->idx;
    }
#endif

    BtApp_HF_CLCCTokenINDHandler(pTcBtHfEvt->idx,pTcBtHfEvt->status
                                        ,(char *)pTcBtHfEvt->data
                                        ,pTcBtHfEvt->dataLength);

    TC_BT_Pfree(pTcBtHfEvt->clccString);

    if((pTcBtHfEvt->dataLength!=0) && (pTcBtHfEvt->data!=NULL))
    {
        TC_BT_Pfree(pTcBtHfEvt->data);
    }
    if((pTcBtHfEvt->bodyLength!=0) && (pTcBtHfEvt->body!=NULL))
    {
        TC_BT_Pfree(pTcBtHfEvt->body);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_GetCurrentCallListCfm(void *pTcBtEvt)
{
    TcBtHfGetCurrentCallListCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfGetCurrentCallListCfm *)pTcBtEvt;

    BtApp_HF_CLCCTokenCFMHandler(pTcBtHfEvt->cmeeResultCode);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetExtendedAgErrorResultCodeCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetCallNotificationIndicationCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetCallWaitingNotificationCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetStatusIndicatorUpdateCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetEchoAndNoiseCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetVoiceRecognitionCfm(void *pTcBtEvt)
{
    TcBtHfSetVoiceRecognitionCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfSetVoiceRecognitionCfm *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        if (pTcBtHfEvt->cmeeResultCode == TC_BT_CME_SUCCESS)
        {
            if(BtApp_HF_VoiceRecognition()!=FALSE)
            {
                BtApp_HF_VoiceRecognitionINDHandler(TRUE);
            }
            else
            {
                BtApp_HF_VoiceRecognitionINDHandler(FALSE);
            }
        }
        else
        {
            if(BtApp_HF_VoiceRecognition()!=FALSE)
            {
                BtApp_HF_VoiceRecognitionInd(FALSE);
            }
            else
            {
                BtApp_HF_VoiceRecognitionInd(TRUE);
            }
        }
    }
    else
    {
        /* TO DO: */
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SetVoiceRecognitionInd(void *pTcBtEvt)
{
    TcBtHfSetVoiceRecognitionInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfSetVoiceRecognitionInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        if(pTcBtHfEvt->started!=FALSE)
        {
            BtApp_HF_VoiceRecognitionINDHandler(TRUE);
        }
        else
        {
            BtApp_HF_VoiceRecognitionINDHandler(FALSE);
        }
    }
    else
    {
        /* TO DO: */
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_BtInputCfm(void *pTcBtEvt)
{
    TcBtHfBtInputCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfBtInputCfm *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        /* TO DO: */
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->dataRespString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_InbandRingSettingChangedInd(void *pTcBtEvt)
{
    TcBtHfInbandRingSettingChangedInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfInbandRingSettingChangedInd *)pTcBtEvt;

    BtApp_HF_InbandRingChangedInd(pTcBtHfEvt->inbandRingingActivated);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_GenerateDtmfCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SpeakerGainStatusCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SpeakerGainInd(void *pTcBtEvt)
{
    TcBtHfSpeakerGainInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfSpeakerGainInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_SpkVolumeINDHandler(pTcBtHfEvt->gain);
    }
    else
    {
        //Bluetooth_HsSpkVolumeINDHandler(pTcBtHfEvt->gain);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_MicGainStatusCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_MicGainInd(void *pTcBtEvt)
{
    TcBtHfMicGainInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfMicGainInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_MicGainINDHandler(pTcBtHfEvt->gain);
    }
    else
    {
        //Bluetooth_HsMicGainINDHandler(pTcBtHfEvt->gain);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}


int TC_BT_HF_DialCfm(void *pTcBtEvt)
{
    TcBtHfDialCfm *pTcBtHfEvt = (TcBtHfDialCfm *)pTcBtEvt;

    BtApp_HF_DialCFMHandler(pTcBtHfEvt->cmeeResultCode);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallAnswerCfm(void *pTcBtEvt)
{
    TcBtHfCallAnswerCfm *pTcBtHfEvt = (TcBtHfCallAnswerCfm *)pTcBtEvt;

    BtApp_HF_CallAnswerCFMHandler(pTcBtHfEvt->cmeeResultCode);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallEndCfm(void *pTcBtEvt)
{
    TcBtHfCallEndCfm *pTcBtHfEvt = (TcBtHfCallEndCfm *)pTcBtEvt;


    BtApp_HF_CallEndCFMHandler(pTcBtHfEvt->cmeeResultCode);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallHandlingInd(void *pTcBtEvt)
{
    TcBtHfCallHandlingInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfCallHandlingInd *)pTcBtEvt;

  switch(pTcBtHfEvt->event)
    {
        case TC_BT_BTRH_INCOMING_ON_HOLD:
            {
                break;
            }
        case TC_BT_BTRH_INCOMING_ACCEPTED:
            {
               break;
            }
        case TC_BT_BTRH_INCOMING_REJECTED:
            {
               break;
            }
        default:
            {

            }
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallHandlingCfm(void *pTcBtEvt)
{
    TcBtHfCallHandlingCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfCallHandlingCfm *)pTcBtEvt;

    if (pTcBtHfEvt->cmeeResultCode == TC_BT_CME_SUCCESS)
    {
    }
    else
    {
   }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_AtCmdInd(void *pTcBtEvt)
{
    TcBtHfAtCmdInd *pTcBtHfEvt;
    char *String;
    int Token=BTAPP_HF_NONE_TOKEN;

    pTcBtHfEvt = (TcBtHfAtCmdInd *)pTcBtEvt;

    Token=BtApp_HF_AtCmdGetToken((unsigned char *)pTcBtHfEvt->atCmdString,&String, pTcBtHfEvt->atCmdIndex);

    if(pTcBtHfEvt->atCmdIndex>=BTAPP_HF_TOKEN)
    {
        // hf
        BtApp_HF_AtCmdINDHandler(Token, (unsigned char *)String);
    }
    else if(pTcBtHfEvt->atCmdIndex>=BTAPP_HF_PB_TOKEN)
    {
        // phonebook
    }
    else
    {
        // invalid token;
        printf("[%s : %05d] Info : TC_BT_HF_AtCmdInd (invalid) : %d, %d, %s \n",__FILENAME__,__LINE__
            ,pTcBtHfEvt->atCmdIndex, Token, pTcBtHfEvt->atCmdString);
    }

    TC_BT_Pfree(pTcBtHfEvt->atCmdString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_AtCmdCfm(void *pTcBtEvt)
{
    TcBtHfAtCmdCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfAtCmdCfm *)pTcBtEvt;

    printf("[%s : %05d] Info : TC_BT_HF_AtCmdCfm : %d, %d \n",__FILENAME__,__LINE__
        ,pTcBtHfEvt->cmeeResultCode,pTcBtHfEvt->atCmdIndex);

    if(pTcBtHfEvt->atCmdIndex>=BTAPP_HF_TOKEN)
    {
        // hf
        BtApp_HF_AtCmdCFMHandler(pTcBtHfEvt->cmeeResultCode, pTcBtHfEvt->atCmdIndex);
    }
    else if(pTcBtHfEvt->atCmdIndex>=BTAPP_HF_PB_TOKEN)
    {
        // phonebook
    }
    else
    {
        // invalid token;
        printf("[%s : %05d] Info : TC_BT_HF_AtCmdCfm (invalid) : %d, %d \n",__FILENAME__,__LINE__
            ,pTcBtHfEvt->cmeeResultCode,pTcBtHfEvt->atCmdIndex);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallRingingInd(void *pTcBtEvt)
{
    TcBtHfCallRingingInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfCallRingingInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_InbandRingChangedInd(pTcBtHfEvt->inBand);
        BtApp_HF_CallRingINDHandler();
    }
    else
    {
        //Bluetooth_HsRingINDHandler();
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallNotificationInd(void *pTcBtEvt)
{
    TcBtHfCallNotificationInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfCallNotificationInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_ClipTokenINDHandler(pTcBtHfEvt->clipString);
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->clipString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_CallWaitingNotificationInd(void *pTcBtEvt)
{
    TcBtHfCallWaitingNotificationInd *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfCallWaitingNotificationInd *)pTcBtEvt;

    if (1 /*pTcBtHfEvt->bdAddr ... */ )
    {
        BtApp_HF_CCWATokenINDHandler(pTcBtHfEvt->ccwaString);
    }
    else
    {
        /* TO DO: */
    }

    TC_BT_Pfree(pTcBtHfEvt->ccwaString);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}


int TC_BT_HF_SecurityInCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_NOT_SUPPORT_ENUM;
}

int TC_BT_HF_SecurityOutCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_NOT_SUPPORT_ENUM;
}

int TC_BT_HF_DeregisterTimeCfm(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_NOT_SUPPORT_ENUM;
}

int TC_BT_HF_IndicatorActivationCfm(void *pTcBtEvt)
{
    TcBtHfIndicatorActivationCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfIndicatorActivationCfm *)pTcBtEvt;

    if (pTcBtHfEvt->result == TC_BT_CME_SUCCESS)
    {
     }
    else
    {
     }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_UpdateSupportedCodecCfm(void *pTcBtEvt)
{
    TcBtHfUpdateSupportedCodecCfm *pTcBtHfEvt;

    pTcBtHfEvt = (TcBtHfUpdateSupportedCodecCfm *)pTcBtEvt;

    BtApp_HF_UpdateSupportedCodecCFMHandler(pTcBtHfEvt->resultCode);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_SelectedCodecInd(void *pTcBtEvt)
{
#ifdef BT_HFP_WIDEBAND_SPEECH
    TcBtHfSelectedCodecInd *pTcBtHfEvt;
#endif


#ifdef BT_HFP_WIDEBAND_SPEECH
    pTcBtHfEvt = (TcBtHfSelectedCodecInd *)pTcBtEvt;

    BtApp_HF_SetCodecInd(pTcBtHfEvt->codecToUse);
#endif

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_HF_ServiceConnectNotificationInd(void *pTcBtEvt)
{
    TcBtHfServiceConnectNotificationInd *pTcBtHfEvt;


    pTcBtHfEvt = (TcBtHfServiceConnectNotificationInd *)pTcBtEvt;

    if(pTcBtHfEvt->chldString != NULL)
    {
        if(strstr(&(pTcBtHfEvt->chldString[0]), "0")!=NULL)
        {
            BtApp_HF_SupportChldInd(0);
        }
        if(strstr(&(pTcBtHfEvt->chldString[0]), "1x")!=NULL)
        {
            BtApp_HF_SupportChldInd(1);
        }
        if(strstr(&(pTcBtHfEvt->chldString[0]), "2x")!=NULL)
        {
            BtApp_HF_SupportChldInd(2);
        }
        if(strstr(&(pTcBtHfEvt->chldString[0]), "3")!=NULL)
        {
            BtApp_HF_SupportChldInd(3);
        }
        if(strstr(&(pTcBtHfEvt->chldString[0]), "4")!=NULL)
        {
            BtApp_HF_SupportChldInd(4);
        }
    }

    TC_BT_Pfree(pTcBtHfEvt->chldString);

#ifdef BT_HFP_WIDEBAND_SPEECH
    (void)G_BtHfCmd->TC_BT_HF_UpdateSupportedCodec(BtApp_HF_GetSelectedDevBdAddr(),/*TC_BT_WBS_CVSD_CODEC_MASK|*/TC_BT_WBS_MSBC_CODEC_MASK,TRUE);
#endif

    //move to COPS connect ind event, COPS=3,0 -> COPS=? is correct
    //(void)TC_BT_HF_GetCurrentOperatorSelection(BtApp_HF_GetSelectedDevBdAddr());

    /* Send AT+CCWA when connected. If not, AT+CHLD=2 rejected from Audio Gateway while 3-way call */
    /* Set Before CIND Settings*/
    (void)G_BtHfCmd->TC_BT_HF_SetCallWaitingNotification(BtApp_HF_GetSelectedDevBdAddr(), BtApp_HF_SupportThreeWayCalling());

    (void)G_BtHfCmd->TC_BT_HF_SetCallNotificationInd(BtApp_HF_GetSelectedDevBdAddr(), TRUE);

    if(BtApp_HF_SupportExtendedErrorCodes()!=0)
    {
        (void)G_BtHfCmd->TC_BT_HF_SetExtendedAgErrorResultCode(BtApp_HF_GetSelectedDevBdAddr(), TRUE);
    }

    (void)G_BtHfCmd->TC_BT_HF_SetEchoAndNoise(BtApp_HF_GetSelectedDevBdAddr(), FALSE);

#ifdef BT_HFP_AT_SIRI	/* Reserved */
    BtApp_HF_AtCmdSendSiri();
#endif

    BtApp_HF_ServiceConnectNotificationINDHandler();

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
