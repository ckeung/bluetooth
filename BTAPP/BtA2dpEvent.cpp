#include "BtA2dpEvent.h"
#include "BtApp_Gap.h"
#include "BtSys.h"
#include "BtApp_Av.h"
#include "BtApp_HF.h"
#include "TcBtEvt.h"
#include "TC_BT/inc/TC_BT_A2DP_EVT.h"
#include "TC_BT/inc/TC_BT_AVRCP_EVT.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/TC_BT_A2DP_CMD.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "../TC_BT/inc/BtPlatform.h"

#ifdef __arm__

#include "common.h"
#include "adec.h"
#include "tcc_latm_dmx.h"

#define AAC_DECODER		TCC_AAC_DEC
#define _cdk_malloc		(void* (*) (unsigned int))malloc
#define _cdk_realloc	(void* (*) (void*, unsigned int))realloc
#define _cdk_free		(void  (*) (void*))free
#define _cdk_memcpy		(void* (*) (void*, const void*, unsigned int))memcpy
#define _cdk_memmove	(void* (*) (void*, const void*, unsigned int))memmove
#define _cdk_memset		(void  (*) (void*, int, unsigned int))memset

 extern pthread_mutex_t	gBtAvPcmSync;
 extern unsigned char	ulReady;		/* Flag to check if audio is ready */

 extern BT_SRBUFFER		stBuffer;						/* Buffer structure for PCM Data  */
typedef int (cdk_func_t) ( int iOpCode, int* pHandle, void* pParam1, void* pParam2 );
typedef struct _aac_decode_t
{
    cdk_callback_func_t	m_sCallback;

    TcBtInt32			m_iAudioHandle;
    adec_init_t			m_stAdecInit;
    adec_input_t		m_stAdecInput;
    adec_output_t		m_stAdecOutput;
    cdk_func_t			*m_pfAdec;

    void				*m_pLatmHandle;
} aac_decode_t;
static aac_decode_t *gpTcBtA2dpAACInst;

extern int TCC_AAC_DEC(int iOpCode, int* pHandle, void* pParam1, void* pParam2);
#endif
#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
extern BTAPP_AV_STREAM_ANALYZER_TYPE	gstBtAvStreamAnalyzer;
#endif
BtA2dpEvent::BtA2dpEvent()
{
}
BtA2dpEvent::~BtA2dpEvent()
{
}
int BtA2dpEvent::BtApp_A2dp_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;

    evtType = event->type;
    switch(evtType)
    {
        case TC_BT_A2DP_DEACTIVATE_CFM:
        {
            TC_BT_A2DP_DeactivateCfm(event);
        }
        break;
        case TC_BT_A2DP_ACTIVATE_CFM:
        {
            TC_BT_A2DP_ActivateCfm(event);
        }
        break;
        case TC_BT_A2DP_STREAM_DATA_IND:
        {
            TC_BT_A2DP_StreamDataInd(event);
        }
        break;
        case TC_BT_A2DP_CONNECT_IND:
        {
            TC_BT_A2DP_ConnectInd(event);
        }
        break;
        case TC_BT_A2DP_CONNECT_CFM:
        {
            TC_BT_A2DP_ConnectCfm(event);
        }
        break;
        case TC_BT_A2DP_DISCOVER_IND:
        {
            TC_BT_A2DP_DiscoverInd(event);
        }
        break;
        case TC_BT_A2DP_GET_CAPABILITIES_IND:
        {
            TC_BT_A2DP_GetCapabilitiesInd(event);
        }
        break;
        case TC_BT_A2DP_SET_CONFIGURATION_IND:
        {
            TC_BT_A2DP_SetConfigurationInd(event);
        }
        break;
        case TC_BT_A2DP_OPEN_CFM:
        {
            TC_BT_A2DP_OpenCfm(event);
        }
        break;
        case TC_BT_A2DP_OPEN_IND:
        {
            TC_BT_A2DP_OpenInd(event);
        }
        break;
        case TC_BT_A2DP_START_CFM:
        {
            TC_BT_A2DP_StartCfm(event);
        }
        break;
        case TC_BT_A2DP_START_IND:
        {
            TC_BT_A2DP_StartInd(event);
        }
        break;
        case TC_BT_A2DP_CLOSE_CFM:
        {
            TC_BT_A2DP_CloseCfm(event);
        }
        break;
        case TC_BT_A2DP_CLOSE_IND:
        {
            TC_BT_A2DP_CloseInd(event);
        }
        break;
        case TC_BT_A2DP_SUSPEND_CFM:
        {
            TC_BT_A2DP_SuspendCfm(event);
        }
        break;
        case TC_BT_A2DP_SUSPEND_IND:
        {
            TC_BT_A2DP_SuspendInd(event);
        }
        break;
        case TC_BT_A2DP_RECONFIGURE_IND:
        {
            TC_BT_A2DP_ReconfigureInd(event);
        }
        break;
        case TC_BT_A2DP_ABORT_CFM:
        {
            TC_BT_A2DP_AbortCfm(event);
        }
        break;
        case TC_BT_A2DP_ABORT_IND:
        {
            TC_BT_A2DP_AbortInd(event);
        }
        break;
        case TC_BT_A2DP_DISCONNECT_IND:
        {
            TC_BT_A2DP_DisconnectInd(event);
        }
        break;
        default :
        {}
            break;
    }
}
int TC_BT_A2DP_DeactivateCfm(void *pTcBtEvt)
{
    TcBtA2dpDeactivateCfm *pTcBtA2dpEvt = (TcBtA2dpDeactivateCfm *)pTcBtEvt;
    TcBtUint16 result;
    printf("============ %s ============ \n",__FUNCTION__);
    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_ACTIVATE);
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_DEACTIVATE, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_DEACTIVATE, pTcBtAVEvt);

        }
#endif
    }
    else
    {
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_DEACTIVATE, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_DEACTIVATE, pTcBtAVEvt);
        }
#endif
    }

    BtApp_GAP_ReflectDeactivateResult(TC_BT_AUDIO_SINK_UUID, result, BTAPP_GAP_UNUSED_INST_IDX);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_ActivateCfm(void *pTcBtEvt)
{
    TcBtA2dpActivateCfm *pTcBtA2dpEvt = (TcBtA2dpActivateCfm *)pTcBtEvt;
    TcBtUint32 dmpEvtResult;
    TcBtUint16 gapResult;
    printf("============ %s ============ \n",__FUNCTION__);
    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_ACTIVATE);
    #ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
        gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PLAY;
    #endif
        dmpEvtResult = BTAPP_AV_EVT_SUCCESS;
        gapResult = BTAPP_GAP_ACTV_RESULT_SUCCESS;
    }
    else
    {
        dmpEvtResult = BTAPP_AV_EVT_FAILURE;
        gapResult = BTAPP_GAP_ACTV_RESULT_FAILURE;
    }
#if 0
    DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_ACTIVATE, dmpEvtResult, 0, 0, 0);
#else
    {
        TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
        *pTcBtAVEvt = dmpEvtResult;
        TcBtEvt_AvEvt(BT_AV_EVT_SNK_ACTIVATE, pTcBtAVEvt);
    }
#endif
    BtApp_GAP_ReflectActivateResult(TC_BT_AUDIO_SINK_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);
    printf("TC_BT_A2DP_ActivateCfm (avResultCode = 0x%04x, avResultSupplier = 0x%04x)\n", pTcBtA2dpEvt->avResultCode, pTcBtA2dpEvt->avResultSupplier);

  return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_StreamDataInd(void *pTcBtEvt)
{
    TcBtA2dpStreamDataInd *pTcBtA2dpEvt = (TcBtA2dpStreamDataInd *)pTcBtEvt;

    gTcBtA2dpFootprint = G_BtSys->BtApp_SYS_GetTick();
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
        /* Indication is received when it is under streaming state */
        gTcBtA2dpStreamState = 1;	/* Automatic Play/Pause Function : BtApp_AV_SNK_StreamCheckStart */ /* Set state for stream check */
    #ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
        if(gstBtAvStreamAnalyzer.analyzeFlag == TRUE)
        {
            gstBtAvStreamAnalyzer.streamCounters[gstBtAvStreamAnalyzer.counterIndex]++;
        }
    #endif
    }
    else
    {
        /* Indication is received when it isn't under streaming state */
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_ConnectInd(void *pTcBtEvt)
{
    TcBtA2dpConnectInd *pTcBtA2dpEvt = (TcBtA2dpConnectInd *)pTcBtEvt;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
        (void)TC_BT_SC_DB_ToTheTop((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr), TC_BT_AUDIO_SOURCE_UUID);
    #ifdef BT_AUTO_CONNECTION
        TC_BT_SC_DB_WriteLastDevAddr((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
    #endif

        TC_BT_BdAddrCopyIfExist(&gTCBtA2dpCurrentAddr, (const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));

        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_CONNECT);

    #ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
        BtApp_AV_SNK_StreamCheckStart(TRUE);	/* Automatic Play/Pause Function : Stream & Mode Check Start */
    #endif

#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_CONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTED);
        (void)BtApp_GAP_AvrcpSubConnectionProcess((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
        (void)BtApp_GAP_IncomingConnectionBackupProcess((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr), TC_BT_AUDIO_SINK_UUID, BTAPP_GAP_CONN_RESULT_SUCCESS, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else
    {
        /* Only allowed one connection. */
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_CONNECT, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr), TC_BT_AUDIO_SINK_UUID, BTAPP_GAP_CONN_RESULT_FAILURE, BTAPP_GAP_UNUSED_INST_IDX);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_ConnectCfm(void *pTcBtEvt)
{
    TcBtA2dpConnectCfm *pTcBtA2dpEvt = (TcBtA2dpConnectCfm *)pTcBtEvt;
    TcBtUint16 gapResult;
    printf("============ %s ============ \n",__FUNCTION__);
    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTED);
        (void)TC_BT_SC_DB_ToTheTop((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr), TC_BT_AUDIO_SOURCE_UUID);
    #ifdef BT_AUTO_CONNECTION
        TC_BT_SC_DB_WriteLastDevAddr((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));
    #endif

        TC_BT_BdAddrCopyIfExist(&gTCBtA2dpCurrentAddr, (const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr));

        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_CONNECT);
        (void)TC_BT_A2DP_OpenConfig(); /* Exception Case : Symbian smartphone (Don't do anything if it's connected by remote) */
    #ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
        BtApp_AV_SNK_StreamCheckStart(TRUE);	/* Automatic Play/Pause Function : Stream & Mode Check Start */
    #endif
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_CONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_CONNECT, pTcBtAVEvt);
        }
#endif
        gapResult = BTAPP_GAP_CONN_RESULT_SUCCESS;
    }
    else if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT))
    {
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_CONNECT, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        gapResult = BTAPP_GAP_CONN_RESULT_CANCELLED;
    }
    else if(pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_BSAS_CANCEL_CONNECT)
    {
        if(BtApp_GAP_GetA2dpState(&pTcBtA2dpEvt->deviceAddr) == BTAPP_GAP_STATE_CONNECT_CANCELLING)
        {
            (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTING);
        }
        gapResult = BTAPP_GAP_CONN_RESULT_CANCEL_FAILED;
    }
    else
    {
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_CONNECT, BTAPP_AV_EVT_FAILURE, 0, 0, 0);
#else
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
            *pTcBtAVEvt = BTAPP_AV_EVT_FAILURE;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_CONNECT, pTcBtAVEvt);
        }
#endif
        (void)BtApp_GAP_SetA2dpState(&pTcBtA2dpEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        gapResult = BTAPP_GAP_CONN_RESULT_FAILURE;
    }

    (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtA2dpEvt->deviceAddr), TC_BT_AUDIO_SINK_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);


    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_DiscoverInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpDiscoverInd *pTcBtA2dpEvt = (TcBtA2dpDiscoverInd *)pTcBtEvt;

    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_DISCOVER);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_GetCapabilitiesInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpGetCapabilitiesInd *pTcBtA2dpEvt = (TcBtA2dpGetCapabilitiesInd *)pTcBtEvt;

    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
        /* reserved */
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_SetConfigurationInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpSetConfigurationInd *pTcBtA2dpEvt = (TcBtA2dpSetConfigurationInd *)pTcBtEvt;

    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_SET_CONFIGURATION);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_OpenCfm(void *pTcBtEvt)
{
    TcBtA2dpOpenCfm *pTcBtA2dpEvt = (TcBtA2dpOpenCfm *)pTcBtEvt;
    printf("============ %s ============ \n",__FUNCTION__);
    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_OPEN);
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_OPEN, 0, 0, 0, 0);
#endif
    }
    else
    {
        /* reserved */
    }
   return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_OpenInd(void *pTcBtEvt)
{
    TcBtA2dpOpenInd *pTcBtA2dpEvt = (TcBtA2dpOpenInd *)pTcBtEvt;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_OPEN);
#if 0
        DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_OPEN, 0, 0, 0, 0);
#endif
    }
    else
    {
        /* reserved */
    }
   return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_StartCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpStartCfm *pTcBtA2dpEvt = (TcBtA2dpStartCfm *)pTcBtEvt;

    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_STREAM);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_START);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_StartInd(void *pTcBtEvt)
{
    TcBtA2dpStartInd *pTcBtA2dpEvt = (TcBtA2dpStartInd *)pTcBtEvt;
    printf("============ %s ====avResultFlag=%d======= \n",__FUNCTION__,pTcBtA2dpEvt->avResultFlag);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
    #ifdef BT_HANDSFREE_UNIT
        BtApp_HF_RingThroughA2dpStartINDHandler(&gTCBtA2dpCurrentAddr);	/* To support A2DP In-band ring : Motorola RAZR V3X */
    #endif
        (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_AV_SINK_STREAM);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_START);
#endif
    }
    else
    {
        /* reserved */
    }
    TcBtEvt_AvEvt(BT_AV_EVT_SNK_START, pTcBtA2dpEvt);
     return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_CloseCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpCloseCfm *pTcBtA2dpEvt = (TcBtA2dpCloseCfm *)pTcBtEvt;

    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_STREAM);
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_OPEN);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_CLOSE);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_CloseInd(void *pTcBtEvt)
{
    TcBtA2dpCloseInd *pTcBtA2dpEvt = (TcBtA2dpCloseInd *)pTcBtEvt;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
#if 0
        TCCAPI_Timer_CancelTime(TIMER_BT_AV_STREAM_STOP_ID);
#endif
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_STREAM);
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_OPEN);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_CLOSE);
#endif
    }
    else
    {
        /* reserved */
    }
    TcBtEvt_AvEvt(BT_AV_EVT_SNK_CLOSE, pTcBtA2dpEvt);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_SuspendCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpSuspendCfm *pTcBtA2dpEvt = (TcBtA2dpSuspendCfm *)pTcBtEvt;

    if((pTcBtA2dpEvt->avResultSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->avResultCode == TC_BT_RESULT_CODE_AV_SUCCESS))
    {
#ifdef BT_HANDSFREE_UNIT
        BtApp_HF_RingThroughA2dpSuspendINDHandler(&gTCBtA2dpCurrentAddr);
#endif
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_STREAM);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_SUSPEND);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_SuspendInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
    TcBtA2dpSuspendInd *pTcBtA2dpEvt = (TcBtA2dpSuspendInd *)pTcBtEvt;

    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
#ifdef BT_HANDSFREE_UNIT
        BtApp_HF_RingThroughA2dpSuspendINDHandler(&gTCBtA2dpCurrentAddr);
#endif
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_STREAM);
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_SUSPEND);
#endif
    }
    else
    {
        /* reserved */
    }

#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_ReconfigureInd(void *pTcBtEvt)
{
    TcBtA2dpReconfigureInd *pTcBtA2dpEvt = (TcBtA2dpReconfigureInd *)pTcBtEvt;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
#if 0
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_SNK_RECONFIGURATION);
#else
        TcBtEvt_AvEvt(BT_AV_EVT_SNK_RECONFIGURATION, NULL);

#endif
    }
    else
    {
        /* reserved */
    }
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_AbortCfm(void *pTcBtEvt)
{
#if 0	/* No longer supported */
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_AbortInd(void *pTcBtEvt)
{
#if 0	/* No longer supported */
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_A2DP_DisconnectInd(void *pTcBtEvt)
{
    TcBtA2dpDisconnectInd *pTcBtA2dpEvt = (TcBtA2dpDisconnectInd *)pTcBtEvt;
    TcBtUint16 gapResult;
    TcBtBdAddr bdAddr;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pTcBtA2dpEvt->avResultFlag == TRUE)
    {
        (void)BtApp_GAP_GetA2dpBdAddr(&bdAddr);

        if(pTcBtA2dpEvt->reasonSupplier == TC_BT_SUPPLIER_BSAS_DISCONNECT)
        {
            if(BtApp_GAP_GetA2dpState(&bdAddr) == BTAPP_GAP_STATE_DISCONNECTING)
            {
                (void)BtApp_GAP_SetA2dpState(&bdAddr, BTAPP_GAP_STATE_CONNECTED);
            }
            gapResult = BTAPP_GAP_DISC_RESULT_FAILURE;
        }
        else
        {
            (void)BtApp_GAP_SetA2dpState(&bdAddr, BTAPP_GAP_STATE_DISCONNECTED);

            if((pTcBtA2dpEvt->reasonSupplier == TC_BT_SUPPLIER_L2CAP_DISCONNECT) && (pTcBtA2dpEvt->reasonCode == TC_BT_L2CA_DISCONNECT_NORMAL))
            {
                /* NORMAL DISCONNECT */
                gapResult = BTAPP_GAP_DISC_RESULT_SUCCESS;
            }
            else if((pTcBtA2dpEvt->reasonSupplier == TC_BT_SUPPLIER_HCI) && (pTcBtA2dpEvt->reasonCode == TC_BT_HCI_ERROR_CONN_TIMEOUT))
            {
                /* CONNECTION TIME OUT */
                gapResult = BTAPP_GAP_DISC_RESULT_TIMEOUT;
            }
            else if((pTcBtA2dpEvt->reasonSupplier == TC_BT_SUPPLIER_AV) && (pTcBtA2dpEvt->reasonCode == TC_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT))
            {
                /* CONNECTION CANCELLED */
                gapResult = BTAPP_GAP_DISC_RESULT_CONN_CANCELLED;
            }
            else
            {
                /* OTHERS */
                gapResult = BTAPP_GAP_DISC_RESULT_SUCCESS;
            }

            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_STREAM);
            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_OPEN);
            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_AV_SINK_CONNECT);

            /* AVRCP Disconnection is needed, only if disconnection is initiated by remote device */
            if(pTcBtA2dpEvt->localTerminated == 0)
            {
                BtApp_GAP_AvrcpSubDisconnectionProcess((const TcBtBdAddr *)&bdAddr);
            }
        #ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
            BtApp_AV_SNK_StreamCheckStart(FALSE);	/* Automatic Play/Pause Function : Stream & Mode Check Stop */
        #endif
#if 0
            DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_DISCONNECT, BTAPP_AV_EVT_SUCCESS, 0, 0, 0);
#else
            {
                TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
                *pTcBtAVEvt = BTAPP_AV_EVT_SUCCESS;
                TcBtEvt_AvEvt(BT_AV_EVT_SNK_DISCONNECT, pTcBtAVEvt);

            }
#endif
            TC_BT_BdAddrZero(&gTCBtA2dpCurrentAddr);
        }

        (void)BtApp_GAP_ReflectDiscResult((const TcBtBdAddr *)&bdAddr, TC_BT_AUDIO_SINK_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);
    }
    else
    {
        /* Unknown A2DP connection was disconnected */
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

void TC_BT_A2DP_SBC_SetBitpoolCallBack(TcBtUint8 *locMinBitpool, TcBtUint8 *locMaxBitpool)
{	/* SBC Bitpool range must be from 2 to 250. */
#if 0	/* No longer supported */
    *locMinBitpool = 2;
    *locMaxBitpool = 250;
#endif
}

void TC_BT_A2DP_SBC_GetBitpoolCallBack(TcBtUint8 curMinBitpool, TcBtUint8 curMaxBitpool)
{	/* Just test code to confirm SBC Bitpool range (It's called in sbc_open) */
#if 0	/* No longer supported */
    unsigned char uRefofBitpool;

    uRefofBitpool = curMinBitpool;
    uRefofBitpool = curMaxBitpool;
#endif
}

#ifdef __arm__
int TC_BT_A2DP_AAC_InitCallBack(void)
{
    TcBtInt32 ret = 0;
    printf("============ %s ============ \n",__FUNCTION__);
    if(gpTcBtA2dpAACInst == NULL)
    {
        gpTcBtA2dpAACInst = (aac_decode_t *)TC_BT_Pmalloc(sizeof(aac_decode_t));
        if(gpTcBtA2dpAACInst == NULL)
        {
            printf("TC_BT_A2DP_AAC_Init::TC_BT_Pmalloc::error!\n");

            ret = -1;
        }
        else
        {
            memset(gpTcBtA2dpAACInst, 0x00, sizeof(aac_decode_t));
            ret = 0;
        }
    }
    else
    {
        ret = 1;	/* It is already initialized */
    }

    return ret;
}

int TC_BT_A2DP_AAC_DeinitCallBack(void)
{
    aac_decode_t *pInst = gpTcBtA2dpAACInst;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pInst != NULL)
    {
        /* Deinit the LATM parser*/
        if(pInst->m_pLatmHandle != NULL)
        {
          //  latm_parser_close(pInst->m_pLatmHandle);
        }

        /* Deinit AAC Decoder */
        if(pInst->m_iAudioHandle != 0)
        {
            (void)pInst->m_pfAdec(AUDIO_CLOSE, (int *)&pInst->m_iAudioHandle, NULL, NULL);
        }

        TC_BT_Pfree(pInst);
    }
    gpTcBtA2dpAACInst = NULL;

    return 0;
}

int TC_BT_A2DP_AAC_DecodeCallBack(TcBtUint8 *pStreamData, TcBtUint32 streamDataLength, TcBtUint16 *pLeft, TcBtUint16 *pRight)
{
    aac_decode_t *pInst = gpTcBtA2dpAACInst;
    TcBtUint8 *pucAACRawData;
    TcBtInt32 iAACDataLength = 0, iSamplingFrequency = 0, iChannelNumber = 0;
    TcBtInt32 ret;
   // printf("============ %s ============ \n",__FUNCTION__);
    /* LATM parser init */
    if(pInst->m_pLatmHandle == 0)
    {
        pInst->m_pLatmHandle = latm_parser_init(pStreamData, streamDataLength, (int *)&iSamplingFrequency, (int *)&iChannelNumber, &pInst->m_sCallback, TF_AAC_LATM_MCP1);
        if(pInst->m_pLatmHandle != 0)
        {
            printf("TC_BT_A2DP_AAC_DecodeCallBack::latm_parser_init::SamplingRate: %d, Channel: %d\n", iSamplingFrequency, iChannelNumber);
            ret = 0;
        }
        else
        {
            printf("TC_BT_A2DP_AAC_DecodeCallBack::latm_parser_init::error!\n");
            ret = -1;
        }
    }
    else
    {
        ret = 0;
    }

    /* LATM layer parsing */
    if(ret == 0)
    {
       ret = latm_parser_get_frame(pInst->m_pLatmHandle, pStreamData, (int )streamDataLength, &pucAACRawData, (int *)&iAACDataLength, 0);
        if(ret < 0)
        {
            printf("TC_BT_A2DP_AAC_DecodeCallBack::latm_parser_get_frame::error [%d]!\n", ret);
            ret = -1;
        }
        else
        {
            if(iAACDataLength <= 0)
            {
                printf("TC_BT_A2DP_AAC_DecodeCallBack::latm_parser_get_frame::Need more data!\n");
                ret = -1;
            }
            else
            {
                ret = 0;
            }
        }
    }

    /* Decode audio data */
    if(ret == 0)
    {
        pInst->m_stAdecInput.m_eSampleRate = pInst->m_stAdecOutput.m_eSampleRate;
        pInst->m_stAdecInput.m_uiNumberOfChannel = pInst->m_stAdecOutput.m_uiNumberOfChannel;

        pInst->m_stAdecInput.m_pcStream = (TCAS_S8* )pucAACRawData;
        pInst->m_stAdecInput.m_iStreamLength = iAACDataLength;

        pInst->m_stAdecOutput.m_pvChannel[0] = (void *)pLeft;
        pInst->m_stAdecOutput.m_pvChannel[1] = (void *)pRight;

        ret = pInst->m_pfAdec(AUDIO_DECODE, (int *)&pInst->m_iAudioHandle, &pInst->m_stAdecInput, &pInst->m_stAdecOutput);
        if(ret < 0)
        {
            printf("TC_BT_A2DP_AAC_DecodeCallBack::m_pfAdec AUDIO_DECODE error!\n");
            ret = -1;
        }
        else
        {
            ret = 0;
        }
    }

    return ret;
}

int TC_BT_A2DP_AAC_SetConfigCallBack(TcBtUint32 samplerate, TcBtUint32 channel, TcBtUint32 bitrate, TcBtBool supportVBR)
{
    aac_decode_t *pInst = gpTcBtA2dpAACInst;
    TcBtInt32 ret;
    printf("============ %s ============ \n",__FUNCTION__);
    if(pInst != NULL)
    {
        memset(pInst, 0x00, sizeof(aac_decode_t));

        pInst->m_pfAdec = AAC_DECODER;

        pInst->m_stAdecInput.m_eSampleRate = (enum_samplerate_t)samplerate;
        pInst->m_stAdecInput.m_uiNumberOfChannel = channel;
        pInst->m_stAdecInput.m_uiBitRates = bitrate;

//		pInst->m_stAdecInput.m_iStreamLength = 0;
//		pInst->m_stAdecInput.m_uiBitsPerSample = 16;
        pInst->m_stAdecOutput.m_uiBitsPerSample = 16;

        pInst->m_stAdecInit.m_pucExtraData = NULL;
        pInst->m_stAdecInit.m_iExtraDataLen = 0;

        pInst->m_stAdecOutput.m_ePcmInterLeaved = TCAS_DISABLE;
        pInst->m_stAdecOutput.m_iNchannelOrder[CH_LEFT_FRONT] = 1;
        pInst->m_stAdecOutput.m_iNchannelOrder[CH_RIGHT_FRONT] = 2;

        pInst->m_sCallback.m_pfMalloc = _cdk_malloc;
        pInst->m_sCallback.m_pfRealloc = _cdk_realloc;
        pInst->m_sCallback.m_pfFree = _cdk_free;
        pInst->m_sCallback.m_pfMemcpy = _cdk_memcpy;
        pInst->m_sCallback.m_pfMemmove = _cdk_memmove;
        pInst->m_sCallback.m_pfMemset = _cdk_memset;

        pInst->m_stAdecInit.m_pfMalloc = _cdk_malloc;
        pInst->m_stAdecInit.m_pfRealloc = _cdk_realloc;
        pInst->m_stAdecInit.m_pfFree = _cdk_free;
        pInst->m_stAdecInit.m_pfMemcpy = _cdk_memcpy;
        pInst->m_stAdecInit.m_pfMemmove = _cdk_memmove;
        pInst->m_stAdecInit.m_pfMemset = _cdk_memset;

        pInst->m_stAdecInit.m_psAudiodecInput = &pInst->m_stAdecInput;
        pInst->m_stAdecInit.m_psAudiodecOutput = &pInst->m_stAdecOutput;

        pInst->m_stAdecInit.m_iDownMixMode = 1;
        pInst->m_stAdecInit.m_unAudioCodecParams.m_unAAC.m_iAACForceUpmix = 1;
        pInst->m_stAdecInit.m_unAudioCodecParams.m_unAAC.m_iAACForceUpsampling = 1;

        ret = pInst->m_pfAdec(AUDIO_INIT, (int *)&pInst->m_iAudioHandle, &pInst->m_stAdecInit, NULL);
        if(ret < 0)
        {
              (void)pInst->m_pfAdec(AUDIO_CLOSE, (int *)&pInst->m_iAudioHandle, NULL, NULL);

            ret = -1;
        }
    }
    else
    {
              ret = -1;
    }

    return ret;
}
#endif	/* __arm__ */

void TC_BT_A2DP_GetCodecParamCallBack(TcBtUint8 codecType, TcBtUint16 sampleRate, TcBtUint8 ch)
{
    printf("============ %s ============ \n",__FUNCTION__);
    BtApp_AV_SNK_AlsaSetParam(sampleRate, ch);
}

int TC_BT_A2DP_GetPCMDataCallBack(TcBtUint16 *pAudioData, TcBtUint32 audioSize)
{
    TcBtInt32 iRemainSize;

    if(ulReady == FALSE)
    {	/* Check execution of AudioStart */
        return BTAPP_AV_ERROR_OPEN;
    }
   // printf("============ %s ============ \n",__FUNCTION__);
    pthread_mutex_lock(&gBtAvPcmSync);
    iRemainSize = TC_Q_GetDataSize(&stBuffer);

    if((iRemainSize+audioSize) > (BTAPP_AV_PCM_BUF_SIZE))
    {	/* Buffer overflow error */

        if(iRemainSize < (BTAPP_AV_PCM_BUF_SIZE/2))
        {	/* audioSize is abnormal */
       }
        else
        {	/* Buffer flush */
            TcBtUint8 *pTempBuffer;
            TcBtInt32 iFlushSize;

            iFlushSize  = (iRemainSize - BTAPP_AV_PCM_BUF_SIZE/2);
            pTempBuffer = (TcBtUint8 *)TC_BT_Pmalloc(iFlushSize);

            (void)TC_Q_GetData(&stBuffer, (char *)pTempBuffer, iFlushSize);
            TC_BT_Pfree(pTempBuffer);
          }

        pthread_mutex_unlock(&gBtAvPcmSync);
        return BTAPP_AV_ERROR_OVERFLOW;
    }
    else
    {
        (void)TC_Q_PutData(&stBuffer, (char *)pAudioData, audioSize);
    }

    pthread_mutex_unlock(&gBtAvPcmSync);
    return BTAPP_AV_ERROR_SUCCESS;
}

#if 0	/* reserved */
void TC_BT_A2DP_DumpOpenCallBack(unsigned char codecType)
{
#ifdef _TRACE_DECOUT_DUMP_
    streamBufIdx = 0;
    isFull = 0;

    if(TC_Set_Current_Dir(g_Part_ID, "\\", SC_DB_DIR_NUM) != SC_DB_DIR_NUM)
    {
        return;
    }
    if(TC_Set_Current_Dir(g_Part_ID, "A2DP_DUMP", SC_DB_DIR_NUM) != SC_DB_DIR_NUM)
    {
        if(TC_Make_Dir("A2DP_DUMP", SC_DB_DIR_NUM) < 0)
        {
            return;
        }
        if(TC_Set_Current_Dir(g_Part_ID, "A2DP_DUMP", SC_DB_DIR_NUM) != SC_DB_DIR_NUM)
        {
            return;
        }
    }

    if(codecType == A2DP_CODEC_SBC)
    {
        TcBtA2DPFileHandle = TC_Open("A2DP_SBC_DUMP.PCM", TC_O_CREAT|TC_O_RDWR, TC_A_READ|TC_A_WRITE/*only for NU FS*/, TC_A_NORMAL, SC_DB_DIR_NUM);
    }
    else if(codecType == A2DP_CODEC_MPEG24_AAC)
    {
        TcBtA2DPFileHandle = TC_Open("A2DP_AAC_DUMP.PCM", TC_O_CREAT|TC_O_RDWR, TC_A_READ|TC_A_WRITE/*only for NU FS*/, TC_A_NORMAL, SC_DB_DIR_NUM);
    }
    else
    {
    }

    if(TcBtA2DPFileHandle < 0)
    {
    }
#endif /*_TRACE_DECOUT_DUMP_*/
}

void TC_BT_A2DP_DumpWriteCallBack(TcBtUint16 *pAudioDataL, TcBtUint16 *pAudioDataR, TcBtUint32 audioSize, TcBtUint32 allocSize)
{
#ifdef _TRACE_DECOUT_DUMP_

#if DUMP_NUM_OF_CH == 2/* Stereo */
    TcBtUint32 uiPCMData_Temp, SHIFTOP_Temp;
    TcBtUint32 audioDataSizeInBytes = audioSize * 2;
    TcBtUint32 remainingSize = audioDataSizeInBytes;
    TcBtUint32 availableSize = sizeof(streamBuffer) - streamBufIdx;

    if(availableSize < audioDataSizeInBytes)
    {
        if(isFull == 0)
        {
            isFull = 1;
        }
        return;
    }

    while(remainingSize > 0)
    {
        uiPCMData_Temp = *pAudioDataL;
        pAudioDataL++;
        SHIFTOP_Temp = 0x0000ffffUL & *pAudioDataR;
        pAudioDataR++;
        uiPCMData_Temp |= (SHIFTOP_Temp << 16);
        memcpy(&streamBuffer[streamBufIdx], &uiPCMData_Temp, 4);
        remainingSize -= 4;
        streamBufIdx += 4;
    }
#elif DUMP_NUM_OF_CH == 1/* Mono */
    if((sizeof(streamBuffer) - streamBufIdx) < audioSize)
    {
        if(isFull == 0)
        {
            isFull = 1;
        }
        return;
    }
    memcpy(&streamBuffer[streamBufIdx], pAudioDataL, audioSize);
    streamBufIdx += audioSize;
#endif

#endif /*_TRACE_DECOUT_DUMP_*/
}

void TC_BT_A2DP_DumpCloseCallBack(void)
{
#ifdef _TRACE_DECOUT_DUMP_
    TcBtInt32 result;

    if(TcBtA2DPFileHandle > 0)
    {
        result = TC_Write(TcBtA2DPFileHandle, streamBuffer, streamBufIdx);
        if(result < 0)
        {
        }
        if(TC_Close(TcBtA2DPFileHandle) < 0)
        {
        }

        streamBufIdx = 0;
        isFull = 0;
    }
    else
    {
    }
#endif
}
#endif
