
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "BtAvRcpEvent.h"

#include "BtApp_Gap.h"
#include "BtApp_HF.h"
#include "BtApp_Av.h"
#include "BtSys.h"
#include "TcBtEvt.h"
#include "../TC_BT/inc/TC_BT_AVRCP_EVT.h"
#include "../TC_BT/inc/TC_BT_A2DP_EVT.h"
#include "../TC_BT/inc/TC_BT_SD_EVT.h"
#include "../TC_BT/inc/BtNvDbFile.h"

extern unsigned short					gBtAvRemoteVersion;
#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
extern BTAPP_AV_STREAM_ANALYZER_TYPE	gstBtAvStreamAnalyzer;
#endif
extern unsigned char					gBtAvPasValidList[BTAPP_AV_CT_PAS_ATT_END];
extern unsigned char					gBtAvPasCurrValue[BTAPP_AV_CT_PAS_ATT_END];
BtAvRcpEvent::BtAvRcpEvent()
{
}
BtAvRcpEvent::~BtAvRcpEvent()
{
}
int BtAvRcpEvent::BtApp_BtAvRcp_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;
    evtType = event->type;
    switch(evtType)
    {
        case 	TC_BT_AVRCP_ACTIVATE_CFM:
        {
            TC_BT_AVRCP_ActivateCfm(event);
        }break;
        case 	TC_BT_AVRCP_DEACTIVATE_CFM:
        {
            TC_BT_AVRCP_DeactivateCfm(event);
        }break;
        case 	TC_BT_AVRCP_CONNECT_IND:
        {
            TC_BT_AVRCP_ConnectInd(event);
        }break;
        case 	TC_BT_AVRCP_DISCONNECT_IND:
        {
            TC_BT_AVRCP_DisconnectInd(event);
        }break;
        case 	TC_BT_AVRCP_CONNECT_CFM:
        {
            TC_BT_AVRCP_ConnectCfm(event);
        }break;
        case 	TC_BT_AVRCP_REMOTE_FEATURES_IND:
        {
            TC_BT_AVRCP_RemoteFeaturesInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM:
        {
            TC_BT_AVRCP_CtInformDispCharsetCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_REGISTER_CFM:
        {
            TC_BT_AVRCP_CtNotiRegisterCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_GET_PLAY_STATUS_CFM:
        {
            TC_BT_AVRCP_CtGetPlayStatusCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_ATT_ID_CFM:
        {
            TC_BT_AVRCP_CtPasAttIdCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_VAL_ID_CFM:
        {
            TC_BT_AVRCP_CtPasValIdCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_ATT_TXT_CFM:
        {
            TC_BT_AVRCP_CtPasAttTxtCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_VAL_TXT_CFM:
        {
            TC_BT_AVRCP_CtPasValTxtCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_CURRENT_CFM:
        {
            TC_BT_AVRCP_CtPasCurrentCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_SET_CFM:
        {
            TC_BT_AVRCP_CtPasSetCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PASS_THROUGH_CFM:
        {
            TC_BT_AVRCP_CtPassThroughCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM:
        {
            TC_BT_AVRCP_CtGetFolderItemsCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PLAY_CFM:
        {
            TC_BT_AVRCP_CtPlayCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_SEARCH_CFM:
        {
            TC_BT_AVRCP_CtSearchCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM:
        {
            TC_BT_AVRCP_CtGetAttributesCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_CHANGE_PATH_CFM:
        {
            TC_BT_AVRCP_CtChangePathCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_SET_VOLUME_CFM:
        {
            TC_BT_AVRCP_CtSetVolumeCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM:
        {
            TC_BT_AVRCP_CtSetAddressedPlayerCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM:
        {
            TC_BT_AVRCP_CtSetBrowsedPlayerCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM:
        {
            TC_BT_AVRCP_CtAddToNowPlayingCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM:
        {
            TC_BT_AVRCP_CtInformBatteryStatusCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_UNIT_INFO_CMD_CFM:
        {
            TC_BT_AVRCP_CtUnitInfoCmdCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM:
        {
            TC_BT_AVRCP_CtSubUnitInfoCmdCfm(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_ATT_TXT_IND:
        {
            TC_BT_AVRCP_CtPasAttTxtInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_VAL_TXT_IND:
        {
            TC_BT_AVRCP_CtPasValTxtInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_PAS_SET_IND:
        {
            TC_BT_AVRCP_CtPasSetInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_UIDS_IND:
        {
            TC_BT_AVRCP_CtNotiUidsInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_VOLUME_IND:
        {
            TC_BT_AVRCP_CtNotiVolumeInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND:
        {
            TC_BT_AVRCP_CtNotiBatteryStatusInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND:
        {
            TC_BT_AVRCP_CtNotiPlaybackPosInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND:
        {
            TC_BT_AVRCP_CtNotiSystemStatusInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND:
        {
            TC_BT_AVRCP_CtNotiTrackChangedInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_TRACK_END_IND:
        {
            TC_BT_AVRCP_CtNotiTrackEndInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_TRACK_START_IND:
        {
            TC_BT_AVRCP_CtNotiTrackStartInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND:
        {
            TC_BT_AVRCP_CtNotiPlaybackStatusInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND:
        {
            TC_BT_AVRCP_CtNotiAvailablePlayersInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND:
        {
            TC_BT_AVRCP_CtNotiNowPlayingInd(event);
        }break;
        case 	TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND:
        {
            TC_BT_AVRCP_CtSetAddressedPlayerInd(event);
        }break;

    default :
    {}break;

    }
}
int TC_BT_AVRCP_ActivateCfm(void *pTcBtEvt)
{
    TcBtAvrcpActivateCfm *pTcBtAvrcpEvt = (TcBtAvrcpActivateCfm *)pTcBtEvt;
    TcBtUint16 gapResult;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        gapResult = BTAPP_GAP_ACTV_RESULT_SUCCESS;
    }
    else
    {
        gapResult = BTAPP_GAP_ACTV_RESULT_FAILURE;
    }
    BtApp_GAP_ReflectActivateResult(TC_BT_HF_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);
	printf("TC_BT_AVRCP_ActivateCfm (resultCode = 0x%04x, resultSupplier = 0x%04x)\n",  pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_DeactivateCfm(void *pTcBtEvt)
{
    TcBtAvrcpDeactivateCfm *pTcBtAvrcpEvt = (TcBtAvrcpDeactivateCfm *)pTcBtEvt;
    TcBtUint16 result;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        result = BTAPP_GAP_DACT_RESULT_SUCCESS;
    }
    else
    {
        result = BTAPP_GAP_DACT_RESULT_FAILURE;
    }

    BtApp_GAP_ReflectDeactivateResult(TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, result, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_ConnectInd(void *pTcBtEvt)
{
    TcBtAvrcpConnectInd *pTcBtAvrcpEvt = (TcBtAvrcpConnectInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt->flag == TRUE)
    {
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_TARGET_UUID);
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AVRCP_CTRL_CONNECT);
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_CONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_CONNECT, pTcBtAVEvt);

        }
#endif
        (void)BtApp_GAP_SetAvrcpState(&pTcBtAvrcpEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTED);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_CONN_RESULT_SUCCESS, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else
    {
      (void)BtApp_GAP_SetAvrcpState(&pTcBtAvrcpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_CONN_RESULT_FAILURE, BTAPP_GAP_UNUSED_INST_IDX);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_DisconnectInd(void *pTcBtEvt)
{
    TcBtAvrcpDisconnectInd *pTcBtAvrcpEvt = (TcBtAvrcpDisconnectInd *)pTcBtEvt;
    TcBtUint32 result = BTAPP_GAP_DISC_RESULT_SUCCESS;
    TcBtBdAddr bdAddr;

    if((pTcBtAvrcpEvt->reasonSupplier == TC_BT_SUPPLIER_L2CAP_DISCONNECT) && (pTcBtAvrcpEvt->reasonCode == TC_BT_L2CA_DISCONNECT_NORMAL))
    {
        /* NORMAL DISCONNECT */
    }
    else if((pTcBtAvrcpEvt->reasonSupplier == TC_BT_SUPPLIER_HCI) && (pTcBtAvrcpEvt->reasonCode == TC_BT_HCI_ERROR_CONN_TIMEOUT))
    {
        /* CONNECTION TIME OUT */
    }
    else if((pTcBtAvrcpEvt->reasonSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->reasonCode == TC_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED))
    {
        /* CONNECTION CANCELLED */
        result = BTAPP_GAP_DISC_RESULT_CONN_CANCELLED;
    }
    else
    {
        /* OTHERS */
    }

    if(pTcBtAvrcpEvt->flag == TRUE)
    {
        gBtAvRemoteVersion = 0;
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AVRCP_CTRL_CONNECT);
    #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
        BtApp_AV_CT_MetadataCheckStart(FALSE);
    #endif
    #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        gBtAvBrowPlayerIdForPts = 0xABCD0000;
        BtApp_AV_CT_BrowseSupportEnable(FALSE);
        BtApp_AV_CT_GetMPListEnable(FALSE);
    #endif
    #if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
        gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable = FALSE;
    #endif
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_DISCONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_DISCONNECT, pTcBtAVEvt);

        }
#endif
        (void)BtApp_GAP_GetAvrcpBdAddr(&bdAddr);
        (void)BtApp_GAP_SetAvrcpState(&bdAddr, BTAPP_GAP_STATE_DISCONNECTED);
        (void)BtApp_GAP_ReflectDiscResult((const TcBtBdAddr *)&bdAddr, TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, result, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else
    {
        /* UNKNOWN CONNECTION */
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_ConnectCfm(void *pTcBtEvt)
{
    TcBtAvrcpConnectCfm *pTcBtAvrcpEvt = (TcBtAvrcpConnectCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {/* connect success */
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_TARGET_UUID);
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AVRCP_CTRL_CONNECT);
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_CONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_CONNECT, pTcBtAVEvt);

        }
#endif
        if(pTcBtAvrcpEvt->tgFeatures.roleSupported != NULL)
        {
            gBtAvRemoteVersion      = pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion;
            gTcBtAvrcpNotification  = TC_BT_AVRCP_NOTI_FLAG_NONE;
            gTcBtAvrcpUidCounter    = 0;
            memset(gTcBtAvrcpUidCurrent, 0, 8);

            if(pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion >= TC_BT_AVRCP_CONFIG_SR_VERSION_13)
            {
                (void)TC_BT_AVRCP_CtNotiRegister(TC_BT_AVRCP_NOTI_FLAG_ALL, 0, TC_BT_AVRCP_NOTI_REG_STANDARD);

            #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
                if((pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion >= TC_BT_AVRCP_CONFIG_SR_VERSION_14) && ((pTcBtAvrcpEvt->tgFeatures.srFeatures & TC_BT_AVRCP_CONFIG_SR_FEAT_BROWSING) != 0))
                {
                    BtApp_AV_CT_BrowseSupportEnable(TRUE);
                }
                else
            #endif
                {
                #if defined(BT_AUDIO_SINK_AVRCP_PAS)
                    BtApp_AV_CT_PasValidityCheckStart(TRUE);
                #endif
                #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
                    BtApp_AV_CT_MetadataCheckStart(TRUE);
                #endif
                }
            }
#if 0
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_INFO_VER);
#else
            {
                TcBtUint16 *pTcBtAVEvt = (TcBtUint16 *)TC_BT_Pmalloc(sizeof(TcBtUint16));

                *pTcBtAVEvt = pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion;
                TcBtEvt_AvEvt(BT_AV_EVT_CT_INFO_VER, pTcBtAVEvt);

            }
#endif
        }
        (void)BtApp_GAP_SetAvrcpState(&pTcBtAvrcpEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTED);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_CONN_RESULT_SUCCESS, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED))
    {/* connect cancelled */
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_CONNECT, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetAvrcpState(&pTcBtAvrcpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        (void)BtApp_GAP_RemoveFromConnProfList((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_UNUSED_INST_IDX);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_CONN_RESULT_CANCELLED, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else
    {/* connect failed */
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_CONNECT, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetAvrcpState(&pTcBtAvrcpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID, BTAPP_GAP_CONN_RESULT_FAILURE, BTAPP_GAP_UNUSED_INST_IDX);
    }

    if(pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_EXCESS_CONNECTION)
    {
       }

    TC_BT_Pfree(pTcBtAvrcpEvt->tgFeatures.providerName);
    TC_BT_Pfree(pTcBtAvrcpEvt->tgFeatures.serviceName);
    TC_BT_Pfree(pTcBtAvrcpEvt->ctFeatures.providerName);
    TC_BT_Pfree(pTcBtAvrcpEvt->ctFeatures.serviceName);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_RemoteFeaturesInd(void *pTcBtEvt)
{
    TcBtAvrcpRemoteFeaturesInd *pTcBtAvrcpEvt = (TcBtAvrcpRemoteFeaturesInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt->ctFeatures.roleSupported != NULL)
    {
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_UUID);
    }
    if(pTcBtAvrcpEvt->tgFeatures.roleSupported != NULL)
    {
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtAvrcpEvt->deviceAddr), TC_BT_AV_REMOTE_CONTROL_TARGET_UUID);

        gBtAvRemoteVersion      = pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion;
        gTcBtAvrcpNotification  = TC_BT_AVRCP_NOTI_FLAG_NONE;
        gTcBtAvrcpUidCounter    = 0;
        memset(gTcBtAvrcpUidCurrent, 0, 8);

        if(pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion >= TC_BT_AVRCP_CONFIG_SR_VERSION_13)
        {
            (void)TC_BT_AVRCP_CtNotiRegister(TC_BT_AVRCP_NOTI_FLAG_ALL, 0, TC_BT_AVRCP_NOTI_REG_STANDARD);

        #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
            if((pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion >= TC_BT_AVRCP_CONFIG_SR_VERSION_14) && ((pTcBtAvrcpEvt->tgFeatures.srFeatures & TC_BT_AVRCP_CONFIG_SR_FEAT_BROWSING) != 0))
            {
                BtApp_AV_CT_BrowseSupportEnable(TRUE);
            }
            else
        #endif
            {
            #if defined(BT_AUDIO_SINK_AVRCP_PAS)
                BtApp_AV_CT_PasValidityCheckStart(TRUE);
            #endif
            #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
                BtApp_AV_CT_MetadataCheckStart(TRUE);
            #endif
            }
        }

    #if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
        gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable = FALSE;
    #endif
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_INFO_VER);
#else
        {
            TcBtUint16 *pTcBtAVEvt = (TcBtUint16 *)TC_BT_Pmalloc(sizeof(TcBtUint16));

            *pTcBtAVEvt = pTcBtAvrcpEvt->tgFeatures.srAvrcpVersion;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_INFO_VER, pTcBtAVEvt);

        }
#endif
    }

    TC_BT_Pfree(pTcBtAvrcpEvt->tgFeatures.providerName);
    TC_BT_Pfree(pTcBtAvrcpEvt->tgFeatures.serviceName);
    TC_BT_Pfree(pTcBtAvrcpEvt->ctFeatures.providerName);
    TC_BT_Pfree(pTcBtAvrcpEvt->ctFeatures.serviceName);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtInformDispCharsetCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtInformDispCharsetCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtInformDispCharsetCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_INFORM_CHAR_SET);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_INFORM_CHAR_SET, NULL);
#endif
    }
    else
    {
        /* reserved */
    }
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiRegisterCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiRegisterCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiRegisterCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        /* reserved */
    }
    else
    {
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        if(gBtAvBrowValidity == 1)
        {
        #if defined(BT_AUDIO_SINK_AVRCP_PAS)
            BtApp_AV_CT_PasValidityCheckStart(TRUE);
        #endif
        #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
            BtApp_AV_CT_MetadataCheckStart(TRUE);
        #endif
            BtApp_AV_CT_GetMPListEnable(TRUE);
        }
#endif
    }
#if 0
    DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_NOTIFICATION, (unsigned int)gTcBtAvrcpNotification, 0, 0, 0);
#else
    {
        TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

        *pTcBtAVEvt = gTcBtAvrcpNotification;
        TcBtEvt_AvEvt(BT_AV_EVT_CT_NOTIFICATION, pTcBtAVEvt);

    }
#endif

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtGetPlayStatusCfm(void *pTcBtEvt)
{
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
    TcBtAvrcpCtGetPlayStatusCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtGetPlayStatusCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        gstBtAvMetaData.SongLength = pTcBtAvrcpEvt->songLength;
        gstBtAvMetaData.SongPosition = pTcBtAvrcpEvt->songPosition;
        gstBtAvMetaData.PlayStatus = pTcBtAvrcpEvt->playStatus;
        gBtAvMetaFlag = TRUE;	/* Set the flag to check the response */
    #if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
        gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable = TRUE;
    #endif
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PLAY_LENGTH);
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PLAY_POSITION);
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PLAY_STATUS);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = gstBtAvMetaData.SongLength;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PLAY_LENGTH, pTcBtAVEvt);

        }
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = gstBtAvMetaData.SongPosition;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PLAY_POSITION, pTcBtAVEvt);

        }
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = gstBtAvMetaData.PlayStatus;
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PLAY_STATUS, pTcBtAVEvt);

        }
#endif
    }
    else
    {
        gBtAvMetaFlag = FALSE;	/* Exception Case : Rejection By Target */
    #if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
        gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable = FALSE;
    #endif
     }
#else	/* BT_AUDIO_SINK_AVRCP_METADATA */
    TcBtAvrcpCtGetPlayStatusCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtGetPlayStatusCfm *)pTcBtEvt;
#endif	/* BT_AUDIO_SINK_AVRCP_METADATA */

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasAttIdCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPasAttIdCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasAttIdCfm *)pTcBtEvt;
    TcBtUint8 i;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        if(pTcBtAvrcpEvt->attIdCount > 0)
        {
            for(i = 0; i < pTcBtAvrcpEvt->attIdCount; i++)
            {
                if(pTcBtAvrcpEvt->attId[i] < sizeof(gBtAvPasValidList))
                {	/* Mark the valid attributes */
                    gBtAvPasValidList[pTcBtAvrcpEvt->attId[i]] = BTAPP_AV_CT_PAS_VALID_MARK;
                }
            }
            BtApp_AV_CT_PasValidityCheck();
        }
    #else
        (void)TC_BT_AVRCP_CtPasAttTxt(pTcBtAvrcpEvt->attIdCount, pTcBtAvrcpEvt->attId);
        for(i = 0; i < pTcBtAvrcpEvt->attIdCount; i++)
        {
            printf("TC_BT_AVRCP_CT_PAS_ATT_ID_CFM : Asking for value IDs for attribute ID: %d\n", pTcBtAvrcpEvt->attId[i]);
            (void)TC_BT_AVRCP_CtPasValId(pTcBtAvrcpEvt->attId[i]);
        }
    #endif

        TC_BT_Pfree(pTcBtAvrcpEvt->attId);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasValIdCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPasValIdCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasValIdCfm *)pTcBtEvt;
    TcBtUint8 i;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        if(gTcBtAvrcpPasAttId < sizeof(gBtAvPasValidList))
        {
            for(i = 0; i < pTcBtAvrcpEvt->valIdCount; i++)
            {
                gBtAvPasValidList[gTcBtAvrcpPasAttId] |= (0x01 << (pTcBtAvrcpEvt->valId[i]-1));
            }
            BtApp_AV_CT_PasValidityCheck();
        }
    #else

        for(i = 0; i < pTcBtAvrcpEvt->valIdCount; i++)
        {
            printf(" Value Id(%d): %d.", i, pTcBtAvrcpEvt->valId[i]);
        }
        printf("\n");

        (void)TC_BT_AVRCP_CtPasValTxt(gTcBtAvrcpPasAttId, pTcBtAvrcpEvt->valIdCount, pTcBtAvrcpEvt->valId);
    #endif

        TC_BT_Pfree(pTcBtAvrcpEvt->valId);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasAttTxtCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtPasAttTxtCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasAttTxtCfm *)pTcBtEvt;
    TcBtUint16 i;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        if(pTcBtAvrcpEvt->textCount == 1)
        {
            gBtAvPasEventAtt = pTcBtAvrcpEvt->textData[0].Id;
            if(pTcBtAvrcpEvt->textData[0].textLen > BTAPP_AV_CT_PAS_TEXT_SIZE)
            {
                pTcBtAvrcpEvt->textData[0].text[BTAPP_AV_CT_PAS_TEXT_SIZE-1] = NULL;
            }
            (void)strcpy(gBtAvPasEventText, pTcBtAvrcpEvt->textData[0].text);
            TC_BT_Pfree(pTcBtAvrcpEvt->textData[0].text);

#if 0
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_GET_ATT_TEXT);
#else
            {
                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasEventAtt)+sizeof(gBtAvPasEventText));

                pTcBtAVEvt[0] = gBtAvPasEventAtt;
                memcpy(&pTcBtAVEvt[1], gBtAvPasEventText, sizeof(gBtAvPasEventText));
                TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_GET_ATT_TEXT, pTcBtAVEvt);
            }
#endif
        }
        else
        {
            /* Unexpected Condition */ /* Check the request attribute number */
            i = 0;
        }
    #else
        for(i = 0; i < pTcBtAvrcpEvt->textCount; i++)
        {

            TC_BT_Pfree(pTcBtAvrcpEvt->textData[i].text);
        }
    #endif

        TC_BT_Pfree(pTcBtAvrcpEvt->textData);
    }
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasValTxtCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtPasValTxtCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasValTxtCfm *)pTcBtEvt;
    TcBtUint16 i;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        if(pTcBtAvrcpEvt->textCount == 1)
        {
            gBtAvPasEventAtt = pTcBtAvrcpEvt->attId;
            gBtAvPasEventVal = pTcBtAvrcpEvt->textData[0].Id;
            if(pTcBtAvrcpEvt->textData[0].textLen > BTAPP_AV_CT_PAS_TEXT_SIZE)
            {
                pTcBtAvrcpEvt->textData[0].text[BTAPP_AV_CT_PAS_TEXT_SIZE-1] = NULL;
            }
            (void)strcpy(gBtAvPasEventText, pTcBtAvrcpEvt->textData[0].text);
            TC_BT_Pfree(pTcBtAvrcpEvt->textData[0].text);

#if 0
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_GET_VAL_TEXT);
#else
            {
                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasEventAtt)+sizeof(gBtAvPasEventVal)+sizeof(gBtAvPasEventText));

                pTcBtAVEvt[0] = gBtAvPasEventAtt;
                pTcBtAVEvt[1] = gBtAvPasEventVal;
                memcpy(&pTcBtAVEvt[2], gBtAvPasEventText, sizeof(gBtAvPasEventText));
                TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_GET_VAL_TEXT, pTcBtAVEvt);
            }
#endif
        }
        else
        {
            /* Unexpected Condition */ /* Check the request attribute number */
            i = 0;
        }
    #else
        for(i = 0; i < pTcBtAvrcpEvt->textCount; i++)
        {

            TC_BT_Pfree(pTcBtAvrcpEvt->textData[i].text);
        }
    #endif

        TC_BT_Pfree(pTcBtAvrcpEvt->textData);
    }
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasCurrentCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPasCurrentCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasCurrentCfm *)pTcBtEvt;
    TcBtUint8 i;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        for(i = 0; i < pTcBtAvrcpEvt->attValPairCount; i++)
        {
            if(pTcBtAvrcpEvt->attValPair[i].attribId < sizeof(gBtAvPasCurrValue))
            {
                gBtAvPasCurrValue[pTcBtAvrcpEvt->attValPair[i].attribId] = pTcBtAvrcpEvt->attValPair[i].valueId;
            }
        }
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_GET_SETTINGS);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasCurrValue));

            memcpy(pTcBtAVEvt, gBtAvPasCurrValue, sizeof(gBtAvPasCurrValue));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_GET_SETTINGS, pTcBtAVEvt);

        }
#endif
    #else
        for(i = 0; i < pTcBtAvrcpEvt->attValPairCount; i++)
        {
            printf("TC_BT_AVRCP_CT_PAS_CURRENT_CFM (attrib: %x; value: %x)\n", pTcBtAvrcpEvt->attValPair[i].attribId, pTcBtAvrcpEvt->attValPair[i].valueId);
        }
    #endif

        TC_BT_Pfree(pTcBtAvrcpEvt->attValPair);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasSetCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPasSetCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPasSetCfm *)pTcBtEvt;

    printf("TC_BT_AVRCP_CT_PAS_SET_CFM (res: 0x%04x/0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_SET_SETTINGS);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_SET_SETTINGS, NULL);
#endif
    #endif
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPassThroughCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPassThroughCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPassThroughCfm *)pTcBtEvt;
    unsigned int operation = pTcBtAvrcpEvt->operationId;
    unsigned int state     = pTcBtAvrcpEvt->state;
    unsigned int result;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        switch(pTcBtAvrcpEvt->operationId)
        {
            case TC_BT_AVRCP_PT_OP_ID_FFORWARD:		/* AVRCP Command : Long Key */
            case TC_BT_AVRCP_PT_OP_ID_REWIND:
                {
                    break;
                }
            default:								/* AVRCP Command : Short Key */
                {
                    /* Exception Case : HTC HD2 */
                    if(pTcBtAvrcpEvt->state == TC_BT_AVRCP_PT_STATE_PRESS)
                    {	/* Release-Key will be sent automatically if you send Press-Key. Exception case : FFORWARD, REWIND Key */
                        (void)TC_BT_AVRCP_CtPassThrough(pTcBtAvrcpEvt->operationId, TC_BT_AVRCP_PT_STATE_RELEASE);
                    }
                    break;
                }
        }
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    {
        unsigned char evt_data[12];
        unsigned int  evt_size = 0;

        memcpy(&evt_data[evt_size], &operation, 4);
        evt_size += 4;
        memcpy(&evt_data[evt_size], &state, 4);
        evt_size += 4;
        memcpy(&evt_data[evt_size], &result, 4);
        evt_size += 4;

#if 0
        DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_KEY_RESULT, (void *)evt_data, evt_size);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

            memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_KEY_RESULT, pTcBtAVEvt);

        }
#endif
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtGetFolderItemsCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtGetFolderItemsCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtGetFolderItemsCfm *)pTcBtEvt;
    TcBtUint16 i;

    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    if(pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
    #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        gTcBtAvrcpUidCounter          = pTcBtAvrcpEvt->uidCounter;
        gstBtAvBrowsing.ItemsCount = pTcBtAvrcpEvt->itemsCount;
    #endif
        printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (uidCounter = %d, startItem = %d)\n", pTcBtAvrcpEvt->uidCounter, gTcBtAvrcpFolderStart);
        printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (itemsCount = %d, itemsLength = %d)\n", pTcBtAvrcpEvt->itemsCount, pTcBtAvrcpEvt->itemsDataLen);

        switch(gTcBtAvrcpFolderScope)
        {
            case TC_BT_AVRCP_SCOPE_SEARCH:
            case TC_BT_AVRCP_SCOPE_NPL:
            case TC_BT_AVRCP_SCOPE_MP_FS:
            {
                TcBtUint8				item;
                TcBtAvrcpFolderMedia	*data = (TcBtAvrcpFolderMedia *)pTcBtAvrcpEvt->itemsData;

                for(item = 0; item < pTcBtAvrcpEvt->itemsCount; item++)
                {
                    switch(data[item].itemType)
                    {
                        case TC_BT_AVRCP_ITEM_TYPE_FOLDER:
                        #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
                            gstBtAvBrowsing.Item[item].Index = (gTcBtAvrcpFolderStart+item);
                            gstBtAvBrowsing.Item[item].Type  = 0/*BT_AUDIO_ITEM_TYPE_FOLDER*/;
                            memcpy(gstBtAvBrowsing.Item[item].UID, data[item].uid, sizeof(TcBtAvrcpUid));

                        {
                            unsigned char evt_data[BTAPP_AV_CT_BROWSING_TEXT+16];
                            unsigned int  evt_size = 0;

                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.ItemsCount, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gTcBtAvrcpFolderStart, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.Item[item].Index, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.Item[item].Type, 4);
                            evt_size += 4;
                            if(data[item].nameLen > BTAPP_AV_CT_BROWSING_TEXT)
                            {
                                data[item].nameLen = BTAPP_AV_CT_BROWSING_TEXT;
                            }
                            memcpy(&evt_data[evt_size], data[item].name, data[item].nameLen);
                            evt_size += BTAPP_AV_CT_BROWSING_TEXT;
                            evt_data[evt_size-1] = NULL;	/* Null-terminated String */
#if 0
                            DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_FOLDER_ITEMS, (void *)evt_data, evt_size);
#else
                            {
                                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

                                memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
                                TcBtEvt_AvEvt(BT_AV_EVT_CT_FOLDER_ITEMS, pTcBtAVEvt);

                            }
#endif
                        }
                        #else
                            printf("\n");
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (item idx = %d)\n", item);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (folder UID = 0x");
                            for(i = 0; i < sizeof(TcBtAvrcpUid); i++)
                            {
                                printf("%02X", data[item].uid[i]);
                            }
                            printf(")\n");
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (folder type = 0x%02X)\n", data[item].type);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (playable type = %s)\n", ((data[item].playableType == TC_BT_AVRCP_ITEM_FOLDER_PLAYABLE_YES) ? "yes" : "no"));
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (charset = 0x%04x)\n", data[item].charset);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (folder Name Len = 0x%04x)\n", data[item].nameLen);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (folder Name = %s)\n", data[item].name);
                            usleep(50*1000);	/* For Debug Message */
                        #endif

                            TC_BT_Pfree(data[item].name);
                            break;
                        case TC_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT:
                        #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
                            gstBtAvBrowsing.Item[item].Index = (gTcBtAvrcpFolderStart+item);
                            gstBtAvBrowsing.Item[item].Type  = 1/*BT_AUDIO_ITEM_TYPE_MEDIA*/;
                            memcpy(gstBtAvBrowsing.Item[item].UID, data[item].uid, sizeof(TcBtAvrcpUid));

                        {
                            unsigned char evt_data[BTAPP_AV_CT_BROWSING_TEXT+16];
                            unsigned int  evt_size = 0;

                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.ItemsCount, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gTcBtAvrcpFolderStart, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.Item[item].Index, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &gstBtAvBrowsing.Item[item].Type, 4);
                            evt_size += 4;
                            if(data[item].nameLen > BTAPP_AV_CT_BROWSING_TEXT)
                            {
                                data[item].nameLen = BTAPP_AV_CT_BROWSING_TEXT;
                            }
                            memcpy(&evt_data[evt_size], data[item].name, data[item].nameLen);
                            evt_size += BTAPP_AV_CT_BROWSING_TEXT;
                            evt_data[evt_size-1] = NULL;	/* Null-terminated String */

#if 0
                            DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_FOLDER_ITEMS, (void *)evt_data, evt_size);
#else
                            {
                                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

                                memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
                                TcBtEvt_AvEvt(BT_AV_EVT_CT_FOLDER_ITEMS, pTcBtAVEvt);
                            }
#endif
                        }
                        #else
                            printf("\n");
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (item idx = %d)\n", item);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (charset = 0x%04x)\n", data[item].charset);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (media Name Len = 0x%04x)\n", data[item].nameLen);
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (media Name = %s)\n", data[item].name);

                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (attribute count = %d)\n", data[item].attributeCount);
                            for(i = 0; i < data[item].attributeCount; i++)
                            {
                                TcBtAvrcpItemMediaAttributes *attributeData = data[item].attributeData;

                                printf("\t attribID : 0x%08x\n", attributeData[i].attribId);
                                printf("\t charset  = 0x%04x\n", attributeData[i].charset);
                                printf("\t attLen   = 0x%04x\n", attributeData[i].attLen);
                                printf("\t att      = %s\n", attributeData[i].att);
                                usleep(50*1000);	/* For Debug Message */
                            }
                        #endif

                            for(i = 0; i < data[item].attributeCount; i++)
                            {
                                TcBtAvrcpItemMediaAttributes *attributeData = data[item].attributeData;
                                TC_BT_Pfree(attributeData[i].att);
                            }
                            TC_BT_Pfree(data[item].name);
                            break;
                        case TC_BT_AVRCP_ITEM_TYPE_INVALID:
                        default:
                            printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (Unexpected Item Type)\n");
                            break;
                    }
                }
            }
            break;
            case TC_BT_AVRCP_SCOPE_MP_LIST:
            {
                TcBtUint8				item;
                TcBtAvrcpPlayer			*data = (TcBtAvrcpPlayer *)pTcBtAvrcpEvt->itemsData;

                for(item = 0; item < pTcBtAvrcpEvt->itemsCount; item++)
                {
                #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
                #if 1
                    if((data[item].featureMask[1] & 0x08000000) != 0)
                    {	/* Browsing */
                        if(gBtAvBrowPlayerIdForPts == 0xABCD0000)
                        {
                            gBtAvBrowPlayerIdForPts = data[item].playerId;
                        }
                        if(data[item].playerId == gstBtAvBrowsing.AddressedPlayerID)
                        {
                            (void)TC_BT_AVRCP_CtSetBrowsedPlayer(gstBtAvBrowsing.AddressedPlayerID);

                            break;
                        }
                    }
                    else
                    {
                        if(data[item].playerId == gstBtAvBrowsing.AddressedPlayerID)
                        {
                            unsigned char evt_data[8];
                            unsigned int  evt_size = 0;
                            unsigned int  isBrowsable = 0; /* not browsable */
                            unsigned int  result = TRUE;

                            memcpy(&evt_data[evt_size], &isBrowsable, 4);
                            evt_size += 4;
                            memcpy(&evt_data[evt_size], &result, 4);
                            evt_size += 4;

#if 0
                            DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_SET_PLAYER_RESULT, (void *)evt_data, evt_size);
#else
                            {
                                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

                                memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
                                TcBtEvt_AvEvt(BT_AV_EVT_CT_SET_PLAYER_RESULT, pTcBtAVEvt);
                            }
#endif

                            /* Some times EVENT_TRACK_CHANGED notification (with valid ID) is not received under below case.
                               So we call "BtApp_AV_CT_MetadataCheckStart(TRUE)" function in order to update metadata.
                               Used "uid current value" is referred to the spec. [AVRCP v1.6.0, Table 6.32: Response EVENT_TRACK_CHANGED]

                               step1) A song on the browsable player is selected and playing.
                               step2) Play a song with not browsable player. (control on the phone side) */
                            memset(gTcBtAvrcpUidCurrent, 0, 8);
                            #if defined(BT_AUDIO_SINK_AVRCP_PAS)
                                BtApp_AV_CT_PasValidityCheckStart(TRUE);
                            #endif
                            #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
                                BtApp_AV_CT_MetadataCheckStart(TRUE);
                            #endif
                        }
                    }
                #else
                    if((data[item].featureMask[1] & 0x08000000) != 0)
                    {	/* Browsing */
                        if((data[item].featureMask[1] & 0x80000000) != 0)
                        {	/* OnlyBrowsableWhenAddressed */
                            (void)TC_BT_AVRCP_CtSetAddressedPlayer(data[item].playerId);
                        }
                        (void)TC_BT_AVRCP_CtSetBrowsedPlayer(data[item].playerId);	/* How to get the playerId of default media player? */
                    }
                    else
                    {
                        (void)TC_BT_AVRCP_CtSetAddressedPlayer(data[item].playerId);	/* How to get the playerId of default media player? */
                    }
                #endif
                #else
                    printf("\n");
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (item idx = %d)\n", item);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (player ID = 0x%04x)\n", data[item].playerId);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (majorType = 0x%02x)\n", data[item].majorType);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (subType = 0x%08x)\n", data[item].subType);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (playbackStatus = 0x%x)\n", data[item].playbackStatus);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (featureMask0 = 0x%08x)\n", data[item].featureMask[0]);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (featureMask1 = 0x%08x)\n", data[item].featureMask[1]);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (featureMask2 = 0x%08x)\n", data[item].featureMask[2]);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (featureMask3 = 0x%08x)\n", data[item].featureMask[3]);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (charset = 0x%04x)\n", data[item].charset);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (playerNameLen = 0x%04x)\n", data[item].playerNameLen);
                    printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (playerName = %s)\n", data[item].playerName);
                    usleep(50*1000);	/* For Debug Message */
                #endif

                    TC_BT_Pfree(data[item].playerName);
                }
            }
            break;
            default:
            {
                printf("TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM (Unexpected Scope)\n");
            }
            break;
        }
    }
    else
    {
        /* TC_BT_RESULT_CODE_AVCTP_RANGE_OOB *//* Range Out of Bounds */
        /* TC_BT_RESULT_CODE_AVRCP_TIMEOUT */
        /* TC_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED */
        if((gTcBtAvrcpFolderScope == TC_BT_AVRCP_SCOPE_NPL) || (gTcBtAvrcpFolderScope == TC_BT_AVRCP_SCOPE_MP_FS))
        {
            unsigned char evt_data[4];
            unsigned int  evt_size = 0;

            memset(&evt_data[evt_size], 0x00, 4);
            evt_size += 4;

#if 0
            DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_FOLDER_ITEMS, (void *)evt_data, evt_size);
#else
            {
                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

                memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
               TcBtEvt_AvEvt(BT_AV_EVT_CT_FOLDER_ITEMS, pTcBtAVEvt);
            }
#endif
        }
        if(gTcBtAvrcpFolderScope == TC_BT_AVRCP_SCOPE_MP_LIST)
        {
            unsigned char evt_data[8];
            unsigned int  evt_size = 0;
            unsigned int  isBrowsable = 0; /* not browsable */
            unsigned int  result = TRUE;

            memcpy(&evt_data[evt_size], &isBrowsable, 4);
            evt_size += 4;
            memcpy(&evt_data[evt_size], &result, 4);
            evt_size += 4;

#if 0
            DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_SET_PLAYER_RESULT, (void *)evt_data, evt_size);
#else
            {
                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

                memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
                TcBtEvt_AvEvt(BT_AV_EVT_CT_SET_PLAYER_RESULT, pTcBtAVEvt);


            }
#endif

        #if defined(BT_AUDIO_SINK_AVRCP_PAS)
            BtApp_AV_CT_PasValidityCheckStart(TRUE);
        #endif
        #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
            BtApp_AV_CT_MetadataCheckStart(TRUE);
        #endif
        }
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPlayCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtPlayCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtPlayCfm *)pTcBtEvt;
    unsigned int result;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    {
        unsigned char evt_data[4];
        unsigned int  evt_size = 0;

        memcpy(&evt_data[evt_size], &result, 4);
        evt_size += 4;
#if 0
        DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_PLAY_ITEM_RESULT, (void *)evt_data, evt_size);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

            memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PLAY_ITEM_RESULT, pTcBtAVEvt);

        }
#endif
    }

    printf("TC_BT_AVRCP_CT_PLAY_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSearchCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtSearchCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtSearchCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_SEARCH, 0, 0, 0, 0);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_CT_SEARCH, NULL);
#endif

        if(pTcBtAvrcpEvt->numberOfItems > 0)
        {
            (void)TC_BT_AVRCP_CtGetFolderItems(TC_BT_AVRCP_SCOPE_SEARCH, 0, pTcBtAvrcpEvt->numberOfItems, 0);
        }
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_SEARCH_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    printf("TC_BT_AVRCP_CT_SEARCH_CFM Number of items found: %d\n", pTcBtAvrcpEvt->numberOfItems);
    printf("TC_BT_AVRCP_CT_SEARCH_CFM UID Counter is: %d\n", pTcBtAvrcpEvt->uidCounter);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtGetAttributesCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtGetAttributesCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtGetAttributesCfm *)pTcBtEvt;
    TcBtUint16 i;
    TcBtBool attribIdChk[8] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        gstBtAvMetaData.NumberOfAttribute = (unsigned long)pTcBtAvrcpEvt->attributeCount;
        if(gstBtAvMetaData.NumberOfAttribute == 0)
        {
            gBtAvMetaFlag = FALSE;	/* Exception Case : LG-SV800 */
            printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM : NumberOfAttribute Zero\n");
        }
        else
        {
            gBtAvMetaFlag = TRUE;	/* Set the flag to check the response */

            for(i = 0; i < pTcBtAvrcpEvt->attributeCount; i++)
            {
                if(pTcBtAvrcpEvt->attribData[i].attLen > BTAPP_AV_CT_METADATA_SIZE)
                {
                    pTcBtAvrcpEvt->attribData[i].att[BTAPP_AV_CT_METADATA_SIZE-1] = NULL;
                    pTcBtAvrcpEvt->attribData[i].attLen = BTAPP_AV_CT_METADATA_SIZE;
                }

                if(pTcBtAvrcpEvt->attribData[i].attribId == 1)
                {
                    if(strcmp(gstBtAvMetaData.NameOfTitle, (char *)pTcBtAvrcpEvt->attribData[i].att) != 0)
                    {
                        (void)strcpy(gstBtAvMetaData.NameOfTitle, (char *)pTcBtAvrcpEvt->attribData[i].att);
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TITLE);
#else
                        {
                            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfTitle));

                            memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfTitle, sizeof(gstBtAvMetaData.NameOfTitle));
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TITLE, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[1] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 2)
                {
                    if(strcmp(gstBtAvMetaData.NameOfArtist, (char *)pTcBtAvrcpEvt->attribData[i].att) != 0)
                    {
                        (void)strcpy(gstBtAvMetaData.NameOfArtist, (char *)pTcBtAvrcpEvt->attribData[i].att);
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_ARTIST);
#else
                        {
                            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfArtist));

                            memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfArtist, sizeof(gstBtAvMetaData.NameOfArtist));
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_ARTIST, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[2] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 3)
                {
                    if(strcmp(gstBtAvMetaData.NameOfAlbum, (char *)pTcBtAvrcpEvt->attribData[i].att) != 0)
                    {
                        (void)strcpy(gstBtAvMetaData.NameOfAlbum, (char *)pTcBtAvrcpEvt->attribData[i].att);
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_ALBUM);
#else
                        {
                            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfAlbum));

                            memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfAlbum, sizeof(gstBtAvMetaData.NameOfAlbum));
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_ALBUM, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[3] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 4)
                {
                    long temp = (long)atoi((const char *)pTcBtAvrcpEvt->attribData[i].att);
                    if(temp < 0)
                    {	/* Exception Case : TG Luna -1 */
                        temp = 0;
                    }
                    if(temp > 2147483647)
                    {
                        temp = 2147483647;
                    }

                    if(gstBtAvMetaData.NumberOfTrack != temp)
                    {
                        gstBtAvMetaData.NumberOfTrack = temp;
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TRACK);
#else
                        {
                            TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                            *pTcBtAVEvt = gstBtAvMetaData.NumberOfTrack;
                                TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TRACK, pTcBtAVEvt);
                        }
#endif
                    }
                    attribIdChk[4] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 5)
                {
                    long temp = (long)atoi((const char *)pTcBtAvrcpEvt->attribData[i].att);
                    if(temp < 0)
                    {	/* Exception Case : TG Luna -1 */
                        temp = 0;
                    }
                    if(temp > 2147483647)
                    {
                        temp = 2147483647;
                    }

                    if(gstBtAvMetaData.NumberOfTotal != temp)
                    {
                        gstBtAvMetaData.NumberOfTotal = temp;
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TOTAL);
#else
                        {
                            TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                            *pTcBtAVEvt = gstBtAvMetaData.NumberOfTotal;
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TOTAL, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[5] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 6)
                {
                    if(strcmp(gstBtAvMetaData.NameOfGenre, (char *)pTcBtAvrcpEvt->attribData[i].att) != 0)
                    {
                        (void)strcpy(gstBtAvMetaData.NameOfGenre, (char *)pTcBtAvrcpEvt->attribData[i].att);
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_GENRE);
#else
                        {
                            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfGenre));

                            memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfGenre, sizeof(gstBtAvMetaData.NameOfGenre));
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_GENRE, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[6] = TRUE;
                }
                else if(pTcBtAvrcpEvt->attribData[i].attribId == 7)
                {
                    long temp = (long)atoi((const char *)pTcBtAvrcpEvt->attribData[i].att);
                    if(temp < 0)
                    {	/* Exception Case : TG Luna -1 */
                        temp = 0;
                    }
                    if(temp > 2147483647)
                    {
                        temp = 2147483647;
                    }

                    if(gstBtAvMetaData.NumberOfTime != temp)
                    {
                        gstBtAvMetaData.NumberOfTime = temp;
#if 0
                        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TIME);
#else
                        {
                            TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                            *pTcBtAVEvt = gstBtAvMetaData.NumberOfTime;
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TIME, pTcBtAVEvt);

                        }
#endif
                    }
                    attribIdChk[7] = TRUE;
                }
                else
                {
                    /* Reserved for future use */
                }

                TC_BT_Pfree(pTcBtAvrcpEvt->attribData[i].att);
            }

            /* Update attribute which is once used before but not included in current event. */
            if(attribIdChk[1] == FALSE)
            {/* Title */
                if(gstBtAvMetaData.NameOfTitle[0] != '\0')
                {
                    gstBtAvMetaData.NameOfTitle[0] = '\0';
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TITLE);
#else
                    {
                        TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfTitle));

                        memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfTitle, sizeof(gstBtAvMetaData.NameOfTitle));
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TITLE, pTcBtAVEvt);
                    }
#endif
                }
            }
            if(attribIdChk[2] == FALSE)
            {/* Artist Name */
                if(gstBtAvMetaData.NameOfArtist[0] != '\0')
                {
                    gstBtAvMetaData.NameOfArtist[0] = '\0';
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_ARTIST);
#else
                    {
                        TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfArtist));

                        memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfArtist, sizeof(gstBtAvMetaData.NameOfArtist));
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_ARTIST, pTcBtAVEvt);


                    }
#endif
                }
            }
            if(attribIdChk[3] == FALSE)
            {/* Album Name */
                if(gstBtAvMetaData.NameOfAlbum[0] != '\0')
                {
                    gstBtAvMetaData.NameOfAlbum[0] = '\0';
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_ALBUM);
#else
                    {
                        TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfAlbum));

                        memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfAlbum, sizeof(gstBtAvMetaData.NameOfAlbum));
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_ALBUM, pTcBtAVEvt);

                    }
#endif
                }
            }
            if(attribIdChk[4] == FALSE)
            {/* Track Number */
                if(gstBtAvMetaData.NumberOfTrack != -1)
                {
                    gstBtAvMetaData.NumberOfTrack = -1;
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TRACK);
#else
                    {
                        TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                        *pTcBtAVEvt = gstBtAvMetaData.NumberOfTrack;
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TRACK, pTcBtAVEvt);

                    }
#endif
                }
            }
            if(attribIdChk[5] == FALSE)
            {/* Total Number of Tracks */
                if(gstBtAvMetaData.NumberOfTotal != -1)
                {
                    gstBtAvMetaData.NumberOfTotal = -1;
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TOTAL);
#else
                    {
                        TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                        *pTcBtAVEvt = gstBtAvMetaData.NumberOfTotal;
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TOTAL, pTcBtAVEvt);

                    }
#endif
                }
            }
            if(attribIdChk[6] == FALSE)
            {/* Genre */
                if(gstBtAvMetaData.NameOfGenre[0] != '\0')
                {
                    gstBtAvMetaData.NameOfGenre[0] = '\0';
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_GENRE);
#else
                    {
                        TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gstBtAvMetaData.NameOfGenre));

                        memcpy(pTcBtAVEvt, gstBtAvMetaData.NameOfGenre, sizeof(gstBtAvMetaData.NameOfGenre));
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_GENRE, pTcBtAVEvt);

                    }
#endif
                }
            }
            if(attribIdChk[7] == FALSE)
            {/* Playing Time */
                if(gstBtAvMetaData.NumberOfTime != -1)
                {
                    gstBtAvMetaData.NumberOfTime = -1;
#if 0
                    DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_ELEM_TIME);
#else
                    {
                        TcBtInt32 *pTcBtAVEvt = (TcBtInt32 *)TC_BT_Pmalloc(sizeof(TcBtInt32));

                        *pTcBtAVEvt = gstBtAvMetaData.NumberOfTime;
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_ELEM_TIME, pTcBtAVEvt);
                    }
#endif
                }
            }
        }

        TC_BT_Pfree(pTcBtAvrcpEvt->attribData);
    }
    else
    {
        gBtAvMetaFlag = FALSE;	/* Exception Case : Rejection By Target */
        printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

        for(i = 0; i < pTcBtAvrcpEvt->attributeCount; i++)
        {
            TC_BT_Pfree(pTcBtAvrcpEvt->attribData[i].att);
        }
        TC_BT_Pfree(pTcBtAvrcpEvt->attribData);
    }
#else	/* BT_AUDIO_SINK_AVRCP_METADATA */

    printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM Number of Attributes = %d\n", pTcBtAvrcpEvt->attributeCount);
    for(i = 0; i < pTcBtAvrcpEvt->attributeCount; i++)
    {
        printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM (attribID: 0x%08x)\n", pTcBtAvrcpEvt->attribData[i].attribId);
        printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM (charset: 0x%04x)\n", pTcBtAvrcpEvt->attribData[i].charset);
        printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM (attLen: 0x%04x)\n", pTcBtAvrcpEvt->attribData[i].attLen);

        printf("TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM (att: ");
        printf(pTcBtAvrcpEvt->attribData[i].att);
        printf(" )\n");

        TC_BT_Pfree(pTcBtAvrcpEvt->attribData[i].att);
    }
    TC_BT_Pfree(pTcBtAvrcpEvt->attribData);
#endif	/* BT_AUDIO_SINK_AVRCP_METADATA */

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtChangePathCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtChangePathCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtChangePathCfm *)pTcBtEvt;
    unsigned int result;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        gstBtAvBrowsing.TotalCount = pTcBtAvrcpEvt->itemsCount;
    #endif
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    {
        unsigned char evt_data[8];
        unsigned int  evt_size = 0;

        memcpy(&evt_data[evt_size], &result, 4);
        evt_size += 4;
        if(result == TRUE)
        {
            memcpy(&evt_data[evt_size], &pTcBtAvrcpEvt->itemsCount, 4);
            evt_size += 4;
        }
#if 0
        DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_CHANGE_PATH_RESULT, (void *)evt_data, evt_size);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

            memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_CHANGE_PATH_RESULT, pTcBtAVEvt);

        }
#endif
    }

    printf("TC_BT_AVRCP_CT_CHANGE_PATH_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    printf("TC_BT_AVRCP_CT_CHANGE_PATH_CFM Number of items found: %d\n", pTcBtAvrcpEvt->itemsCount);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSetVolumeCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtSetVolumeCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtSetVolumeCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_SET_VOLUME);
#else
    {
        TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(TcBtUint8));

        *pTcBtAVEvt = pTcBtAvrcpEvt->volume;
        TcBtEvt_AvEvt(BT_AV_EVT_CT_SET_VOLUME, pTcBtAVEvt);
    }
#endif
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_SET_VOLUME_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    printf("TC_BT_AVRCP_CT_SET_VOLUME_CFM volume: %d\n", pTcBtAvrcpEvt->volume);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSetAddressedPlayerCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtSetAddressedPlayerCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtSetAddressedPlayerCfm *)pTcBtEvt;
    unsigned int result;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    {
        unsigned char evt_data[8];
        unsigned int  evt_size = 0;
        unsigned int  isBrowsable = 0; /* not browsable */

        memcpy(&evt_data[evt_size], &isBrowsable, 4);
        evt_size += 4;
        memcpy(&evt_data[evt_size], &result, 4);
        evt_size += 4;
#if 0
        DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_SET_PLAYER_RESULT, (void *)evt_data, evt_size);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

            memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_SET_PLAYER_RESULT, pTcBtAVEvt);

        }
#endif
    }
    printf("TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSetBrowsedPlayerCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtSetBrowsedPlayerCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtSetBrowsedPlayerCfm *)pTcBtEvt;
    unsigned int  result;
    unsigned char evt_data[8];
    unsigned int  evt_size = 0;
    unsigned int  isBrowsable;
    TcBtBool      sendEventFlag = FALSE;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
    #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        gTcBtAvrcpUidCounter       = pTcBtAvrcpEvt->uidCounter;
        gstBtAvBrowsing.TotalCount = pTcBtAvrcpEvt->itemsCount;

        if(gstBtAvBrowsing.BrowsedPlayerID == gstBtAvBrowsing.AddressedPlayerID)
        {
            sendEventFlag = TRUE;

            isBrowsable = 1; /* browsable */
            result = TRUE;
        }
    #endif
    }
    else
    {
    #if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        if(gstBtAvBrowsing.BrowsedPlayerID == gstBtAvBrowsing.AddressedPlayerID)
        {
            sendEventFlag = TRUE;

            isBrowsable = 0; /* not browsable */
            result = FALSE;
        }
    #endif
    }

    if(sendEventFlag == TRUE)
    {
        memcpy(&evt_data[evt_size], &isBrowsable, 4);
        evt_size += 4;
        memcpy(&evt_data[evt_size], &result, 4);
        evt_size += 4;

#if 0
        DMP_EvtBTInfoMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_SET_PLAYER_RESULT, (void *)evt_data, evt_size);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(evt_data));

            memcpy(pTcBtAVEvt, evt_data, sizeof(evt_data));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_SET_PLAYER_RESULT, pTcBtAVEvt);

        }
#endif
    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        BtApp_AV_CT_PasValidityCheckStart(TRUE);
    #endif
    #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
        BtApp_AV_CT_MetadataCheckStart(TRUE);
    #endif
    }

    printf("TC_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    printf("\t uidCounter: %d\n", pTcBtAvrcpEvt->uidCounter);
    printf("\t itemsCount: %d\n", pTcBtAvrcpEvt->itemsCount);
    printf("\t folder depth: %d\n", pTcBtAvrcpEvt->folderDepth);
    printf("\t folder name: %s\n", pTcBtAvrcpEvt->folderNames);
    TC_BT_Pfree(pTcBtAvrcpEvt->folderNames);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtAddToNowPlayingCfm(void *pTcBtEvt)
{
    TcBtAvrcpCtAddToNowPlayingCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtAddToNowPlayingCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_ADD_TO_NPL, 0, 0, 0, 0);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_CT_ADD_TO_NPL, NULL);

#endif
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtInformBatteryStatusCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtInformBatteryStatusCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtInformBatteryStatusCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_INFORM_BATTERY);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_INFORM_BATTERY, NULL);
#endif
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtUnitInfoCmdCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtUnitInfoCmdCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtUnitInfoCmdCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        /* reserved */
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_UNIT_INFO_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    TC_BT_Pfree(pTcBtAvrcpEvt->pData);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSubUnitInfoCmdCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtSubUnitInfoCmdCfm *pTcBtAvrcpEvt = (TcBtAvrcpCtSubUnitInfoCmdCfm *)pTcBtEvt;

    if((pTcBtAvrcpEvt->resultSupplier == TC_BT_SUPPLIER_AVRCP) && (pTcBtAvrcpEvt->resultCode == TC_BT_RESULT_CODE_AVRCP_SUCCESS))
    {
        /* reserved */
    }
    else
    {
        /* reserved */
    }
    printf("TC_BT_AVRCP_CT_SUB_UNIT_INFO_CFM received; result: 0x%04x (0x%04x)\n", pTcBtAvrcpEvt->resultCode, pTcBtAvrcpEvt->resultSupplier);
    TC_BT_Pfree(pTcBtAvrcpEvt->pData);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasAttTxtInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtPasAttTxtInd *pTcBtAvrcpEvt = (TcBtAvrcpCtPasAttTxtInd *)pTcBtEvt;
    TcBtUint16 i;

    for(i = 0; i < pTcBtAvrcpEvt->textCount; i++)
    {
        printf("TC_BT_AVRCP_CT_PAS_ATT_TXT_IND (att ID: 0x%08x)\n", pTcBtAvrcpEvt->textData[i].Id);
        printf("TC_BT_AVRCP_CT_PAS_ATT_TXT_IND (charset: 0x%04x)\n", pTcBtAvrcpEvt->textData[i].charset);
        printf("TC_BT_AVRCP_CT_PAS_ATT_TXT_IND (att textLen: 0x%04x)\n", pTcBtAvrcpEvt->textData[i].textLen);

        printf("TC_BT_AVRCP_CT_PAS_ATT_TXT_IND (att text: ");
        printf("%s", pTcBtAvrcpEvt->textData[i].text);
        printf(" )\n");

        TC_BT_Pfree(pTcBtAvrcpEvt->textData[i].text);
    }
    TC_BT_Pfree(pTcBtAvrcpEvt->textData);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasValTxtInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtAvrcpCtPasValTxtInd *pTcBtAvrcpEvt = (TcBtAvrcpCtPasValTxtInd *)pTcBtEvt;
    TcBtUint16 i;

    for(i = 0; i < pTcBtAvrcpEvt->textCount; i++)
    {
        printf("TC_BT_AVRCP_CT_PAS_VAL_TXT_IND (val ID: 0x%08x)\n", pTcBtAvrcpEvt->textData[i].Id);
        printf("TC_BT_AVRCP_CT_PAS_VAL_TXT_IND (charset: 0x%04x)\n", pTcBtAvrcpEvt->textData[i].charset);
        printf("TC_BT_AVRCP_CT_PAS_VAL_TXT_IND (val textLen: 0x%04x)\n", pTcBtAvrcpEvt->textData[i].textLen);

        printf("TC_BT_AVRCP_CT_PAS_VAL_TXT_IND (val text: ");
        printf("%s", pTcBtAvrcpEvt->textData[i].text);
        printf(" )\n");

        TC_BT_Pfree(pTcBtAvrcpEvt->textData[i].text);
    }
    TC_BT_Pfree(pTcBtAvrcpEvt->textData);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtPasSetInd(void *pTcBtEvt)
{
    TcBtAvrcpCtPasSetInd *pTcBtAvrcpEvt = (TcBtAvrcpCtPasSetInd *)pTcBtEvt;
    TcBtUint8 i;

    #if defined(BT_AUDIO_SINK_AVRCP_PAS)
        for(i = 0; i < pTcBtAvrcpEvt->attValPairCount; i++)
        {
            if(pTcBtAvrcpEvt->attValPair[i].attribId < sizeof(gBtAvPasCurrValue))
            {
                gBtAvPasCurrValue[pTcBtAvrcpEvt->attValPair[i].attribId] = pTcBtAvrcpEvt->attValPair[i].valueId;
            }
        }
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_GET_SETTINGS);
#else
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasCurrValue));

            memcpy(pTcBtAVEvt, gBtAvPasCurrValue, sizeof(gBtAvPasCurrValue));
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_GET_SETTINGS, pTcBtAVEvt);

        }
#endif
    #else
        for(i = 0; i < pTcBtAvrcpEvt->attValPairCount; i++)
        {
            printf("TC_BT_AVRCP_CT_PAS_SET_IND (attrib: %x; value: %x)\n", pTcBtAvrcpEvt->attValPair[i].attribId, pTcBtAvrcpEvt->attValPair[i].valueId);
        }
    #endif
    TC_BT_Pfree(pTcBtAvrcpEvt->attValPair);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiUidsInd(void *pTcBtEvt)
{
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
    TcBtAvrcpCtNotiUidsInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiUidsInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt != NULL)
    {
        printf("TC_BT_AVRCP_CT_NOTI_UIDS_IND: UidCounter: %u\n", pTcBtAvrcpEvt->uidCounter);

        if((gTcBtAvrcpNotification & TC_BT_AVRCP_NOTI_FLAG_UIDS) == 0)
        { /* For INTERIM */
            gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_UIDS;
        }
        else
        {
            (void)TC_BT_AVRCP_CtSetBrowsedPlayer(gstBtAvBrowsing.BrowsedPlayerID);
#if 0
            DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_MEDIA_DB_CHANGED, 0, 0, 0, 0);
#else
            TcBtEvt_AvEvt(BT_AV_EVT_CT_MEDIA_DB_CHANGED, NULL);
#endif
        }
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_UIDS_IND::error::Null pointer\n");
    }
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiVolumeInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiVolumeInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiVolumeInd *)pTcBtEvt;
    gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_VOLUME;

    if(pTcBtAvrcpEvt->volume <= 15)
    {
        printf("TC_BT_AVRCP_CT_NOTI_VOLUME_IND (volume: 0x%02X)\n", pTcBtAvrcpEvt->volume);
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_VOLUME_IND (volume value received invalid: higher than 15)\n");
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiBatteryStatusInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiBatteryStatusInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiBatteryStatusInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt != NULL)
    {
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_BATT_STATUS;
        printf("TC_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND (status: 0x%02X)\n", pTcBtAvrcpEvt->batteryStatus);
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND::error::Null pointer\n");
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiPlaybackPosInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiPlaybackPosInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiPlaybackPosInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt != NULL)
    {
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_PLAYBACK_POS;
//		printf("TC_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND (position: 0x%08X)\n", pTcBtAvrcpEvt->playbackPos);
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND::error::Null pointer\n");
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiSystemStatusInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiSystemStatusInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiSystemStatusInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt != NULL)
    {
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_SYSTEM_STATUS;
        printf("TC_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND (status: 0x%02X)\n", pTcBtAvrcpEvt->systemStatus);
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND::error::Null pointer\n");
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiTrackChangedInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiTrackChangedInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiTrackChangedInd *)pTcBtEvt;
    TcBtUint16 i;

    memcpy(gTcBtAvrcpUidCurrent, pTcBtAvrcpEvt->trackUid, sizeof(TcBtAvrcpUid));
    if((gTcBtAvrcpNotification & TC_BT_AVRCP_NOTI_FLAG_TRACK) == 0)
    { /* For INTERIM */
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_TRACK;
    }
    else
    {
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
        (void)TC_BT_AVRCP_CtGetAttributes(TC_BT_AVRCP_SCOPE_NPL, gTcBtAvrcpUidCurrent, gTcBtAvrcpUidCounter, TC_BT_AVRCP_ITEM_ATT_MASK_ALL);
#endif
    }

    printf("TC_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND (UID: 0x");
    for(i = 0; i < 8; i++)
    {
        printf("%02X", pTcBtAvrcpEvt->trackUid[i]);
    }
    printf(")\n");

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiTrackEndInd(void *pTcBtEvt)
{
    //TcBtAvrcpCtNotiTrackEndInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiTrackEndInd *)pTcBtEvt;
    gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_TRACK_END;

    printf("TC_BT_AVRCP_CT_NOTI_TRACK_END_IND\n");

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiTrackStartInd(void *pTcBtEvt)
{
    //TcBtAvrcpCtNotiTrackStartInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiTrackStartInd *)pTcBtEvt;
    gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_TRACK_START;

    printf("TC_BT_AVRCP_CT_NOTI_TRACK_START_IND\n");

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiPlaybackStatusInd(void *pTcBtEvt)
{
    TcBtAvrcpCtNotiPlaybackStatusInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiPlaybackStatusInd *)pTcBtEvt;

    if(pTcBtAvrcpEvt != NULL)
    {
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_PLAYBACK_STATUS;
        printf("TC_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND (status: 0x%02X)\n", pTcBtAvrcpEvt->playbackStatus);
    }
    else
    {
        printf("TC_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND::error::Null pointer\n");
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiAvailablePlayersInd(void *pTcBtEvt)
{
    //TcBtAvrcpCtNotiAvailablePlayersInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiAvailablePlayersInd *)pTcBtEvt;
    gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_AVAILABLE_PLAYERS;

    printf("TC_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND\n");

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtNotiNowPlayingInd(void *pTcBtEvt)
{
    //TcBtAvrcpCtNotiNowPlayingInd *pTcBtAvrcpEvt = (TcBtAvrcpCtNotiNowPlayingInd *)pTcBtEvt;
    gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_NOW_PLAYING_CONTENT;

    printf("TC_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND\n");

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_AVRCP_CtSetAddressedPlayerInd(void *pTcBtEvt)
{
    TcBtAvrcpCtSetAddressedPlayerInd *pTcBtAvrcpEvt = (TcBtAvrcpCtSetAddressedPlayerInd *)pTcBtEvt;
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
    gTcBtAvrcpUidCounter = pTcBtAvrcpEvt->uidCounter;
    gstBtAvBrowsing.AddressedPlayerID = pTcBtAvrcpEvt->playerId;
#endif
    if((gTcBtAvrcpNotification & TC_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER) == 0)
    { /* For INTERIM */
        gTcBtAvrcpNotification |= TC_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER;
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        if(gBtAvBrowValidity == 1)
        {
            BtApp_AV_CT_GetMPListEnable(TRUE);
        }
#endif
    }
    else
    {
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
        if(gBtAvBrowValidity == 1)
        {
#if 0
            DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_CT_PLAYER_CHANGED, 0, 0, 0, 0);
#else
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PLAYER_CHANGED, NULL);
#endif

        #if defined(BT_AUDIO_SINK_AVRCP_PAS)
            BtApp_AV_CT_PasValidityCheckStart(FALSE);
        #endif
        #if defined(BT_AUDIO_SINK_AVRCP_METADATA)
            BtApp_AV_CT_MetadataCheckStart(FALSE);
        #endif
            //(void)TC_BT_AVRCP_CtGetFolderItems(BTAPP_AV_CT_BROWSING_MP_LIST, 0, 100, TC_BT_AVRCP_ITEM_ATT_MASK_TITLE);
            BtApp_AV_CT_GetMPListEnable(TRUE);
        }
#endif
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
