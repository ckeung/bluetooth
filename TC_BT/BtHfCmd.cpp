// **********************heard file ********************//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "BtHfCmd.h"
#include "TC_BT_CMD.h"
#include "TC_BT_HF_DEV.h"
#include "TC_BT_HF_SCO.h"
#include "BtUtil.h"
#include "BTAPP/inc/BT_CFG.h"
//***************************static param****************//

//****************************static fun ******************//
static int TC_BT_HF_DefaultCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr);
static int TC_BT_HF_EnDisCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr, TcBtBool enable);

/*****************************************************************************
*
* debug
*
*****************************************************************************/

static int TC_BT_HF_DefaultCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr)
{
    TcBtHfCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfCmd *)TC_BT_Pmalloc(sizeof(TcBtHfCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = type;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

static int TC_BT_HF_EnDisCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    TcBtHfEnDisCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfEnDisCmd *)TC_BT_Pmalloc(sizeof(TcBtHfEnDisCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = type;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->enable = enable;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}





//*************************interface ********************//
BtHfCmd::BtHfCmd()
{
}
BtHfCmd::~BtHfCmd()
{
}
static BtHfCmd  _bthfcmd;
BtHfCmd *GetglobalBtHfCmd()
{
    return &_bthfcmd;
}
int BtHfCmd::TC_BT_HF_Activate(void)
{
    TcBtHfActivateCmd *pCmdStr;
    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfActivateCmd *)TC_BT_Pmalloc(sizeof(TcBtHfActivateCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_ACTIVATE_REQ;
        pCmdStr->maxHFConnections = BT_HFP_HF_CONNECTIONS;
        pCmdStr->maxHSConnections = BT_HFP_HS_CONNECTIONS;
        /* 8010 Supports Codec Negotiation */
    #ifdef BT_HFP_WIDEBAND_SPEECH
        pCmdStr->supportedFeatures = 0xFF; 		/* v1.6 - Support All : +Codec Negotiation(128) */
    #else
        pCmdStr->supportedFeatures = 0x7F; 	/* v1.5 - Support All */
    #endif

        pCmdStr->AtCmdRespTimeout = 2; /* 1 seconds */

        (void)TC_BT_HF_SCO_Init(pCmdStr->maxHFConnections+pCmdStr->maxHSConnections);

        (void)TC_BT_HF_DEV_Init(pCmdStr->maxHFConnections+pCmdStr->maxHSConnections);

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_Deactivate(void)
{
    TcBtHfDeactivateCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfDeactivateCmd *)TC_BT_Pmalloc(sizeof(TcBtHfDeactivateCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_DEACTIVATE_REQ;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_ConfigLowPower(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
}
int BtHfCmd::TC_BT_HF_ConfigAudio(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_CONFIG_AUDIO_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_ServiceConnect(const TcBtBdAddr *pBdAddr)
{
    TcBtHfServiceConnectCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfServiceConnectCmd *)TC_BT_Pmalloc(sizeof(TcBtHfServiceConnectCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_SERVICE_CONNECT_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->connectionType = TC_BT_HF_CONNECTION_HF;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_CancelConnect(const TcBtBdAddr *pBdAddr)
{
    if(pBdAddr!=NULL)
    {
   }

    return TC_BT_HF_DefaultCommand(TC_BT_HF_CANCEL_CONNECT_REQ, pBdAddr);
}
int BtHfCmd::TC_BT_HF_Disconnect(const TcBtBdAddr *pBdAddr)
{
 #ifdef BT_HFP_2xHF_MULTICALL_NOT_SUPPORTED
    if(pBdAddr!=NULL)
    {
        BtApp_HF_CloseSCO(pBdAddr);
    }
#else
    //modify   for  disconnect   must close SCO   xieyh
   // BtApp_HF_CloseSCO();
#endif
    return TC_BT_HF_DefaultCommand(TC_BT_HF_DISCONNECT_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_AudioConnect(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_AUDIO_CONNECT_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_AudioAcceptConnect(const TcBtBdAddr *pBdAddr,TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_AUDIO_ACCEPT_CONNECT_RES, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_AudioDisconnect(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_AUDIO_DISCONNECT_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_GetAllStatusInd(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_GET_ALL_STATUS_INDICATORS_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_GetCurrentOperatorSelection(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_GetSubscriberNumberInformation(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_GetCurrentCallList(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_GET_CURRENT_CALL_LIST_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_SetExtendedAgErrorResultCode(const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_SetCallNotificationInd(const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_SetCallWaitingNotification(const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_SetStatusIndicatorUpdate(const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_SetEchoAndNoise(const TcBtBdAddr *pBdAddr, TcBtBool enable)
{
    return TC_BT_HF_EnDisCommand(TC_BT_HF_SET_ECHO_AND_NOISE_REQ, pBdAddr, enable);
}

int BtHfCmd::TC_BT_HF_SetVoiceRecognition(const TcBtBdAddr *pBdAddr, TcBtBool start)
{
    TcBtHfSetVoiceRecognitionCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfSetVoiceRecognitionCmd *)TC_BT_Pmalloc(sizeof(TcBtHfSetVoiceRecognitionCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_SET_VOICE_RECOGNITION_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->start = start;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_BtInput(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_BT_INPUT_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_GenerateDtmf(const TcBtBdAddr *pBdAddr, TcBtUint8 dtmf)
{
    TcBtHfGenerateDtmfCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfGenerateDtmfCmd *)TC_BT_Pmalloc(sizeof(TcBtHfGenerateDtmfCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_GENERATE_DTMF_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->dtmf = dtmf;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_SpeakerGainStatus(const TcBtBdAddr *pBdAddr, TcBtUint8 gain)
{
    TcBtHfSpeakerGainStatusCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfSpeakerGainStatusCmd *)TC_BT_Pmalloc(sizeof(TcBtHfSpeakerGainStatusCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_SPEAKER_GAIN_STATUS_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->gain = gain;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_MicGainStatus(const TcBtBdAddr *pBdAddr, TcBtUint8 gain)
{
    TcBtHfMicGainStatusCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfMicGainStatusCmd *)TC_BT_Pmalloc(sizeof(TcBtHfMicGainStatusCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_MIC_GAIN_STATUS_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->gain = gain;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}


int BtHfCmd::TC_BT_HF_Dial(const TcBtBdAddr *pBdAddr, TcBtHfDialEnum command, const TcBtString *number)
{
    TcBtHfDialCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfDialCmd *)TC_BT_Pmalloc(sizeof(TcBtHfDialCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_DIAL_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->command = command;
        if(number!=NULL)
        {
            int l_strLen;
            l_strLen = strlen((char *)number);
            pCmdStr->number = (TcBtCharString *)TC_BT_Pmalloc(l_strLen+1);
            memcpy(pCmdStr->number, number, l_strLen);
            pCmdStr->number[l_strLen] = 0;
        }
        else
        {
            pCmdStr->number = 0; /* NULL */
        }

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_CallAnswer(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_CALL_ANSWER_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_CallEnd(const TcBtBdAddr *pBdAddr)
{
    return TC_BT_HF_DefaultCommand(TC_BT_HF_CALL_END_REQ, pBdAddr);
}

int BtHfCmd::TC_BT_HF_CallHandling(const TcBtBdAddr *pBdAddr, TcBtHfCallHandlingEnum command, TcBtUint8 index)
{
    TcBtHfCallHandlingCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfCallHandlingCmd *)TC_BT_Pmalloc(sizeof(TcBtHfCallHandlingCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_CALL_HANDLING_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->command = command;
        pCmdStr->index = index;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_AtCmd(const TcBtBdAddr *pBdAddr, const TcBtString *atCmdString, TcBtUint32 atCmdIndex)
{
    TcBtHfAtCmdCmd *pCmdStr;
    unsigned int atCmdStrLength = 0;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */
    if(pBdAddr == NULL)
    {
        return TC_BT_CMD_RESULT_INVALID_IDX_ENUM;
    }

    pCmdStr = (TcBtHfAtCmdCmd *)TC_BT_Pmalloc(sizeof(TcBtHfAtCmdCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_AT_CMD_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);

        atCmdStrLength = strlen((char *)atCmdString);
        //atCmdStrLength = (atCmdStrLength >= MAX_ATCMD_STR_SIZE) ? MAX_ATCMD_STR_SIZE : atCmdStrLength;

        atCmdStrLength = atCmdStrLength + 1;
        pCmdStr->atCmdString = (TcBtCharString *)TC_BT_Pmalloc(atCmdStrLength);
        (void)strcpy((char *)pCmdStr->atCmdString, (char *)atCmdString);

        pCmdStr->atCmdIndex = atCmdIndex;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}


int BtHfCmd::TC_BT_HF_SecurityIn(void)
{
    return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
}

int BtHfCmd::TC_BT_HF_SecurityOut(void)
{
    return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
}

int BtHfCmd::TC_BT_HF_DeregisterTime(TcBtUint8 waitSeconds)
{
    TcBtHfDeregisterTimeCmd *pCmdStr;

    pCmdStr = (TcBtHfDeregisterTimeCmd *)TC_BT_Pmalloc(sizeof(TcBtHfDeregisterTimeCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_DEREGISTER_TIME_REQ;
        //TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->waitSeconds=waitSeconds;	// waitseconds
        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtHfCmd::TC_BT_HF_IndicatorActivation(const TcBtBdAddr *pBdAddr,  TcBtUint16 indicatorBitMask)
{
    TcBtHfIndicatorActivationCmd *pCmdStr;

    pCmdStr = (TcBtHfIndicatorActivationCmd *)TC_BT_Pmalloc(sizeof(TcBtHfIndicatorActivationCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_INDICATOR_ACTIVATION_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->indicatorBitMask=indicatorBitMask;	// AT+BIA command
                                                    // bitmask with the indicators to enable/disable

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

/*	parameters : codecMask : TC_BT_WBS_CVSD_CODEC_MASK, TC_BT_WBS_MSBC_CODEC_MASK */
int BtHfCmd::TC_BT_HF_UpdateSupportedCodec(const TcBtBdAddr *pBdAddr, TcBtUint32 codecMask, TcBtBool enable)
{
    TcBtHfUpdateSupportedCodecCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtHfUpdateSupportedCodecCmd *)TC_BT_Pmalloc(sizeof(TcBtHfUpdateSupportedCodecCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_UPDATE_SUPPORTED_CODEC_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), pBdAddr);
        //pCmdStr->connectionType = TC_BT_HF_CONNECTION_HF;
        pCmdStr->codecMask=codecMask;
        pCmdStr->enable=enable;
        pCmdStr->sendUpdate=TRUE;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

}

int BtHfCmd::TC_BT_HF_UpdateHFIndicator(const TcBtBdAddr *pBdAddr, TcBtUint16 ind)
{

    TcBtHfUpdateHFIndicatorsCmd *pCmdStr;

    pCmdStr = (TcBtHfUpdateHFIndicatorsCmd *)TC_BT_Pmalloc(sizeof(TcBtHfUpdateSupportedCodecCmd));

    if(pCmdStr!=NULL)
    {
        pCmdStr->type = TC_BT_HF_UPDATE_HF_INDICATORS_REQ;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);
        pCmdStr->ind = ind;

        (void)TC_BT_SendCommand(TC_BT_HF_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
