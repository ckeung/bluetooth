#include "inc/BtDeviceManage.h"
#include "inc/TcBtEvt.h"
#include "../BTAPP/inc/BtApp_Gap.h"
#include "../TC_BT/inc/BtNvDbFile.h"
#include "../TC_BT/inc/TC_BT_SD_EVT.h"
#include "../TC_BT/inc/TC_BT_SC_EVT.h"
#include "../TC_BT/inc/TC_BT_CM_EVT.h"
#include "../TC_BT/inc/TC_BT_CM_CMD.h"
/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
int TC_BT_SD_SearchResultInd(void *pTcBtEvt);
int TC_BT_SD_CloseSearchInd(void *pTcBtEvt);
int TC_BT_SD_ReadAvailableServiceCfm(void *pTcBtEvt);
int TC_BT_SD_CancelReadAvailableServiceCfm(void *pTcBtEvt);

/**********************************  Device Identification  **********************************/
int TC_BT_SD_DiReadServiceRecordInd(void *pTcBtEvt);
int TC_BT_SD_DiReadServiceRecordCfm(void *pTcBtEvt);
int TC_BT_SD_DiRegisterServiceRecordCfm(void *pTcBtEvt);
int TC_BT_SD_DiUnregisterServiceRecordCfm(void *pTcBtEvt);

//  static param
static TcBtBdAddr gDevAddr = {0,0,0};

BtDeviceManage::BtDeviceManage()
{
}
BtDeviceManage::~BtDeviceManage()
{
}
static BtDeviceManage  _btdm;
BtDeviceManage *GetglobalBtDeviceManage()
{
    return &_btdm;
}
//******************************start search device********************************//

void BtDeviceManage::StartSearchDevice(unsigned long int  totalSearchTime, unsigned long int  maxDevices, unsigned char update)
{
      InitDeviceList();
    m_BtSdCmd.TC_BT_SD_Search(totalSearchTime,maxDevices,update);
}
void BtDeviceManage::CancelSearchDevice(void)
{
    m_BtSdCmd.TC_BT_SD_CancelSearch();
}
void BtDeviceManage::InitDeviceList(void)
{
    for(int Cnt = 0; Cnt < MAX_SEARCH_LIST; Cnt++)
    {
        memset(&objDevList[Cnt].devAddr, 0x00, sizeof(TcBtBdAddr));
        objDevList[Cnt].deviceClass = 0;
        objDevList[Cnt].rssi = 0;
        objDevList[Cnt].strDevName = "";
    }
    DevListIdx = 0;
}
//********************************connect device ********************************//

void BtDeviceManage::ConnectDevice(int nDeviceID)
{
    TcBtBdAddr      bdAddr;

    memcpy(&bdAddr, &objDevList[nDeviceID].devAddr, sizeof(TcBtBdAddr));

    if(BtApp_GAP_CheckCondition(&bdAddr, BTAPP_GAP_ACT_CONNECT, BTAPP_GAP_CONN_SIDE_LOCAL) == BTAPP_GAP_COND_ALLOWED)
    {
        TcBtInt32 fr = 0;

        fr = BtApp_GAP_FullConnectionProcess(&bdAddr, BTAPP_GAP_SEND_CONN_RESULT|BTAPP_GAP_CONN_SIDE_LOCAL);
        if(fr == BTAPP_GAP_FN_RESULT_NONE)
        {
           //show msg
        }
    }
    else
    {
        //show msg
    }
}

void BtDeviceManage::DisConnectDevice(int nDeviceID)
{
    TcBtBdAddr      bdAddr;
    memcpy(&bdAddr, &objDevList[nDeviceID].devAddr, sizeof(TcBtBdAddr));

    if(BtApp_GAP_CheckCondition(&bdAddr, BTAPP_GAP_ACT_DISCONNECT, BTAPP_GAP_CONN_SIDE_LOCAL) == BTAPP_GAP_COND_ALLOWED)
   {
       TcBtInt32 fr = 0;
       fr = BtApp_GAP_DisconnectionProcess(&bdAddr);
       if(fr == BTAPP_GAP_FN_RESULT_NONE)
       {
          //
       }
   }
   else
   {
       //
   }
}

void BtDeviceManage::DisConnectAll(void)
{
    if(BtApp_GAP_CheckCondition(NULL, BTAPP_GAP_ACT_DISCONNECT, BTAPP_GAP_CONN_SIDE_LOCAL) == BTAPP_GAP_COND_ALLOWED)
    {
      TcBtInt32 fr = 0;
      fr = BtApp_GAP_DisconnectionAllProcess();
      if(fr == BTAPP_GAP_FN_RESULT_NONE)
      {
         //
      }
    }
    else
    {
     //
    }
}

void BtDeviceManage::DeleteDevice(int nDeviceID)
{
    TcBtBdAddr      bdAddr;

  memcpy(&bdAddr, &objDevList[nDeviceID].devAddr, sizeof(TcBtBdAddr));

    if(BtApp_GAP_GetAclConnCnt() > 0)
   {
       if(BtApp_GAP_IsInAclConnList(&bdAddr) == TRUE)
       {
            //to notice UI
           return;
       }
   }
   m_BtSdCmd.TC_BT_SC_Debond(bdAddr);
   TC_BT_NV_DB_Remove(&bdAddr);
}

void BtDeviceManage::DeleteAll(void)
{
    TcBtSecDevRecrd theRecord;

    if(BtApp_GAP_GetAclConnCnt() > 0)
    {
        //
        return;
    }

    if(TC_BT_NV_DB_ReadFirst(&theRecord) == TRUE)
    {
        m_BtSdCmd.TC_BT_SC_Debond(theRecord.deviceAddr);

        while(TC_BT_NV_DB_ReadNext(&theRecord) == TRUE)
        {
            m_BtSdCmd.TC_BT_SC_Debond(theRecord.deviceAddr);
        }
    }

    TC_BT_NV_DB_RemoveAll();
}

int BtDeviceManage::AddSearchList(TcBtBdAddr devAddr, unsigned int deviceClass, char rssi, QString  strDevName)
{
    int fr = 0;
    //Add DevList
    if(DevListIdx < MAX_SEARCH_LIST)
    {
        for(int Cnt = 0; Cnt < DevListIdx; Cnt++)
        {
            if(TC_BT_BdAddrEq(&objDevList[Cnt].devAddr, &devAddr))
                return fr;
        }
        //Add internal DB
        objDevList[DevListIdx].devAddr = devAddr;
        objDevList[DevListIdx].deviceClass = deviceClass;
        objDevList[DevListIdx].rssi = rssi;
        objDevList[DevListIdx].strDevName = strDevName;
        DevListIdx++;
    }
    else
    {
        fr = -2;
    }

ErrExit:
    return fr;
}

//***********************safty connect control********************************//

void BtDeviceManage::SC_SSP_NumericCompare(unsigned short int bAccept)
{
        m_BtSdCmd.TC_BT_SC_SSP_NumericCompare(bAccept);
}
int BtDeviceManage::SC_LegacyPassKey(const unsigned char *pPassKey, unsigned char PassKeyLen)
{
        m_BtSdCmd.TC_BT_SC_LegacyPassKey(pPassKey,PassKeyLen);
}
int BtDeviceManage::SC_SSP_PasskeyNotification(unsigned short int bAccept)
{
        m_BtSdCmd.TC_BT_SC_SSP_PasskeyNotification(bAccept);
}
int BtDeviceManage::SC_SSP_JustWork(unsigned short int bAccept)
{
        m_BtSdCmd.TC_BT_SC_SSP_JustWork(bAccept);
}
int BtDeviceManage::TC_BT_SC_AuthoriseInd(void *pTcBtEvt)
{
    TcBtScAuthoriseInd	*pTcBtSCEvt = (TcBtScAuthoriseInd *)pTcBtEvt;

    if((pTcBtSCEvt != NULL) && (pTcBtSCEvt->deviceName != NULL))
    {
                 printf("TC_BT_SC_AuthoriseInd (bdAddr = %04x:%02x:%06x)\n", pTcBtSCEvt->deviceAddr.nap, pTcBtSCEvt->deviceAddr.uap, pTcBtSCEvt->deviceAddr.lap);
                printf(" DeviceName = %s\n", pTcBtSCEvt->deviceName);
                printf(" ServiceId = %d\n", pTcBtSCEvt->serviceId);


        if(BtApp_GAP_CheckCondition(&(pTcBtSCEvt->deviceAddr), BTAPP_GAP_ACT_CONNECT, BTAPP_GAP_CONN_SIDE_REMOTE) == BTAPP_GAP_COND_ALLOWED)
        {
            BtApp_GAP_SetAuthoriseFlag(TRUE);
        }
        else
        {
            BtApp_GAP_SetAuthoriseFlag(FALSE);
        }

        if(BtApp_GAP_GetAuthoriseFlag() == TRUE)
        {
            m_BtSdCmd.TC_BT_SC_Authorise(pTcBtSCEvt->deviceAddr, TRUE);	/* Do not ask UI, is authorise automatically */
            BtApp_GAP_AvoidConnectConflict((const TcBtBdAddr *)&pTcBtSCEvt->deviceAddr, pTcBtSCEvt->serviceId);
        }
        else
        {
            m_BtSdCmd.TC_BT_SC_Authorise(pTcBtSCEvt->deviceAddr, FALSE);	/* Do not ask UI, is authorise automatically */
       }

#ifdef BT_AUTO_CONNECTION /* To prevent 'Read Remote Name' command interferes incoming connection process */
        if((BtApp_GAP_GetRecoveryLinkLostFlag() == BTAPP_GAP_RECOVERY_LINK_LOST_IN))
        {
            BtApp_GAP_RecoveryLinkLostStop();
        }
#endif
        TcBtEvt_ScEvt(BT_DM_EVT_CONNECTED,pTcBtEvt);
        TC_BT_Pfree(pTcBtSCEvt->deviceName);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtDeviceManage::TC_BT_SC_BondEvt(void *pTcBtEvt)
{
    TcBtScBondEvt 	*pTcBtSCEvt = (TcBtScBondEvt *)pTcBtEvt;

    if(pTcBtSCEvt != NULL)
    {
        if (pTcBtSCEvt->status == TC_BT_SUCCESS)
        {
            TcBtEvt_ScEvt(BT_DM_EVT_BOND_SUCCESS, NULL);
            (void)TC_BT_SC_DB_ToTheTop((const TcBtBdAddr *)&(pTcBtSCEvt->deviceAddr));
        }
        else
        {
             TcBtEvt_ScEvt(BT_DM_EVT_BOND_ERR, NULL);
            BtApp_GAP_BondFailHandler((const TcBtBdAddr *)&(pTcBtSCEvt->deviceAddr));

            if(BtApp_GAP_IsInAclConnList((const TcBtBdAddr *)&(pTcBtSCEvt->deviceAddr)) == TRUE)
            {
            //   TC_BT_CM_AclDetachReq(pTcBtSCEvt->deviceAddr);
            }
        }

        (void)TC_BT_SC_DB_PrintLinkKey((const TcBtBdAddr *)&pTcBtSCEvt->deviceAddr);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

//********************************receive  search device message**********************//

int BtDeviceManage::BtApp_SD_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;

    evtType = event->type;
    switch(evtType)
    {
        case TC_BT_SD_SEARCH_RESULT_IND:
        {
            TC_BT_SD_SearchResultInd(event);
        }
        break;
        case TC_BT_SD_CLOSE_SEARCH_IND:
        {
            TC_BT_SD_CloseSearchInd(event);
        }
        break;
        case TC_BT_SD_READ_AVAILABLE_SERVICES_CFM:
        {
            TC_BT_SD_ReadAvailableServiceCfm(event);
        }
        break;
        case TC_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM:
        {
            TC_BT_SD_CancelReadAvailableServiceCfm(event);
        }
        break;
        case TC_BT_SD_DI_READ_SERVICE_RECORD_IND:
        {
            TC_BT_SD_DiReadServiceRecordInd(event);
        }
        break;
        case TC_BT_SD_DI_READ_SERVICE_RECORD_CFM:
        {
            TC_BT_SD_DiReadServiceRecordCfm(event);
        }
        break;
        case TC_BT_SD_DI_REGISTER_SERVICE_RECORD_CFM:
        {
            TC_BT_SD_DiRegisterServiceRecordCfm(event);
        }
        break;
    default:
        break;
    }
}

//**********************************receive  Security Controller message***********************//

int BtDeviceManage::BtApp_SC_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;

    evtType = event->type;
    switch(evtType)
    {
        case TC_BT_SC_PASSKEY_IND:
        {
            TC_BT_SC_PasskeyInd(event);
        }
        break;
        case TC_BT_SC_DEBOND_CFM:
        {
            TC_BT_SC_DebondCfm(event);
        }
        break;
        case TC_BT_SC_AUTHORISE_IND:
        {
            TC_BT_SC_AuthoriseInd(event);
        }
        break;
        case TC_BT_SC_BOND_EVT:
        {
            TC_BT_SC_BondEvt(event);
        }
        break;
        case TC_BT_SC_SSP_NOTIFICATION_IND:
        {
            TC_BT_SC_SSP_NotificationInd(event);
        }
        break;
        case TC_BT_SC_SSP_COMPARE_IND:
        {
            TC_BT_SC_SSP_CompareInd(event);
        }
        break;
        case TC_BT_SC_SSP_JUSTWORKS_IND:
        {
            TC_BT_SC_SSP_JustworkInd(event);
        }
        break;
        case TC_BT_SC_DEBUG_MODE_CFM:
        {
            TC_BT_SC_DebugModeCfm(event);
        }
    break;

    default:
        break;
    }
}

//**************************receive connect manage msg **********************//

int BtDeviceManage::BtApp_CM_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;

    evtType = event->type;
    switch(evtType)
    {
        case TC_BT_CM_SET_LOCAL_NAME_CFM:
        {
            TC_BT_CM_SetLocalNameCfm(event);
        }
        break;
        case TC_BT_CM_READ_LOCAL_BD_ADDR_CFM:
        {
            TC_BT_CM_ReadLocalBdAddrCfm(event);
        }
        break;
        case TC_BT_CM_WRITE_SCAN_ENABLE_CFM:
        {
            TC_BT_CM_WriteScanEnableCfm(event);
        }
        break;
        case TC_BT_CM_READ_SCAN_ENABLE_CFM:
        {
            TC_BT_CM_ReadScanEnableCfm(event);
        }
        break;
        case TC_BT_CM_ENABLE_DUT_MODE_CFM:
        {
            TC_BT_CM_EnableDutModeCfm(event);
        }
        break;
        case TC_BT_CM_WRITE_PAGE_TO_CFM:
        {
            TC_BT_CM_WritePageToCfm(event);
        }
        break;
        case TC_BT_CM_WRITE_COD_CFM:
        {
            TC_BT_CM_WriteCodCfm(event);
        }
        break;
        case TC_BT_CM_READ_COD_CFM:
        {
            TC_BT_CM_ReadCodCfm(event);
        }
        break;
        case TC_BT_CM_READ_REMOTE_NAME_CFM:
        {
            TC_BT_CM_ReadRemoteNameCfm(event);
        }
        break;
        case TC_BT_CM_SDC_CLOSE_IND:
        {
            TC_BT_CM_SdcCloseInd(event);
        }
        break;
        case TC_BT_CM_WRITE_LINK_POLICY_ERROR_IND:
        {
            TC_BT_CM_WriteLinkPolicyErrorInd(event);
        }
        break;
        case TC_BT_CM_READ_LINK_POLICY_CFM:
        {
            TC_BT_CM_ReadLinkPolicyCfm(event);
        }
        break;
        case TC_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM:
        {
            TC_BT_CM_WriteLinkSuperVisionTimeoutCfm(event);
        }
        break;
        case TC_BT_CM_GET_LINK_QUALITY_CFM:
        {
            TC_BT_CM_GetLinkQualityCfm(event);
        }
        break;
        case TC_BT_CM_READ_RSSI_CFM:
        {
            TC_BT_CM_ReadRssiCfm(event);
        }
        break;
        case TC_BT_CM_ACL_CONNECT_IND:
        {
            TC_BT_CM_AclConnectInd(event);
        }
        break;
        case TC_BT_CM_ACL_DISCONNECT_IND:
        {
            TC_BT_CM_AclDisconnectInd(event);
        }
        break;
        case TC_BT_CM_QOS_SETUP_CFM:
        {
            TC_BT_CM_QOSSetupCfm(event);
        }
        break;
        case TC_BT_CM_SET_AFH_CHANNELS_CFM:
        {
                TC_BT_CM_SetAfhChannelsCfm(event);
        }
        break;
        case TC_BT_CM_RESET_CFM:
        {
            TC_BT_CM_ResetCfm(event);
        }
        break;
        case TC_BT_CM_VSC_CFM:
        {
            TC_BT_CM_VSCCfm(event);
        }
        break;
            default:
                break;
       }
}

void BtDeviceManage::Handler(int type, void *pTcBtEvt)
{
    switch (type)
    {
        case BT_DM_EVT_SEARCH_RESULT:

            break;
        case BT_DM_EVT_SEARCH_CLOSE:
        {
            //EvtSearchClose();
        }break;
        case BT_DM_EVT_REFRESH:
        {

        }break;
        default:
            break;
    }
}

//*******************************   search device msg*************************//

int TC_BT_SD_SearchResultInd(void *pTcBtEvt)
{
    TcBtSdSearchResultInd *pTcBtSDEvt = (TcBtSdSearchResultInd *)pTcBtEvt;

    TcBtEvt_SearchEvt(BT_DM_EVT_SEARCH_RESULT, pTcBtSDEvt);
    if(pTcBtSDEvt->deviceName != NULL)
    {
        TC_BT_Pfree(pTcBtSDEvt->deviceName);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_CloseSearchInd(void *pTcBtEvt)
{
    TcBtSdCloseSearchInd *pTcBtSDEvt = (TcBtSdCloseSearchInd *)pTcBtEvt;
   TcBtEvt_SearchEvt(BT_DM_EVT_SEARCH_CLOSE, NULL);
   return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_ReadAvailableServiceCfm(void *pTcBtEvt)
{
    TcBtSdReadAvailableServicesCfm	*pTcBtSDEvt = (TcBtSdReadAvailableServicesCfm *)pTcBtEvt;

    if (pTcBtSDEvt->status == TC_BT_SUCCESS)
    {
        //Do something....
                printf(" TC_BT_SD_ReadAvailableServiceCfm (status = %d)\n",  pTcBtSDEvt->status);
                printf(" DeviceName = %s\n",  pTcBtSDEvt->deviceName);
                printf(" bdAddr = %04x:%02x:%06x\n",  pTcBtSDEvt->deviceAddr.nap, pTcBtSDEvt->deviceAddr.uap, pTcBtSDEvt->deviceAddr.lap);
                printf(" Cod = 0x%08x\n",  pTcBtSDEvt->deviceClass);
                printf(" ServiceCount = 0x%08x\n",  pTcBtSDEvt->ServiceCount);

        {
            int i;
            for(i = 0; i < pTcBtSDEvt->ServiceCount; i++)
            {
                TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtSDEvt->deviceAddr), pTcBtSDEvt->ServiceList[i]);
            }
        }

        (void)BtApp_GAP_ValidateConnProfList((const TcBtBdAddr *)&(pTcBtSDEvt->deviceAddr), (const TcBtUint32 *)pTcBtSDEvt->ServiceList, pTcBtSDEvt->ServiceCount, BTAPP_GAP_RAS_RESULT_SUCCESS);

        if(pTcBtSDEvt->deviceName != NULL)
        {
            TC_BT_Pfree(pTcBtSDEvt->deviceName);
        }

        if(pTcBtSDEvt->ServiceList != NULL)
        {
            TC_BT_Pfree(pTcBtSDEvt->ServiceList);
        }
    }
    else
    {
         (void)BtApp_GAP_ValidateConnProfList((const TcBtBdAddr *)&(pTcBtSDEvt->deviceAddr), (const TcBtUint32 *)NULL, 0, BTAPP_GAP_RAS_RESULT_FAILURE);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_CancelReadAvailableServiceCfm(void *pTcBtEvt)
{
    TcBtBdAddr bdAddr;
    TC_BT_BdAddrCopyIfExist(&bdAddr, (const TcBtBdAddr *)BtApp_GAP_GetCcanBdAddr());

    (void)BtApp_GAP_ValidateConnProfList((const TcBtBdAddr *)&bdAddr, (const TcBtUint32 *)NULL, 0, BTAPP_GAP_RAS_RESULT_CANCELLED);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

/**********************************  Device Identification  **********************************/

int TC_BT_SD_DiReadServiceRecordInd(void *pTcBtEvt)
{
    TcBtSdDiReadServiceRecordInd 	*pTcBtSDEvt = (TcBtSdDiReadServiceRecordInd *)pTcBtEvt;
    TcBtSdDiServiceRecordV13Struct	stDiSR;

    if(pTcBtSDEvt->data != NULL)
    {
        memcpy(&stDiSR, pTcBtSDEvt->data, sizeof(TcBtSdDiServiceRecordV13Struct));

        TC_BT_Pfree(pTcBtSDEvt->data);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_DiReadServiceRecordCfm(void *pTcBtEvt)
{
    TcBtSdDiReadServiceRecordCfm 	*pTcBtSDEvt = (TcBtSdDiReadServiceRecordCfm *)pTcBtEvt;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_DiRegisterServiceRecordCfm(void *pTcBtEvt)
{
    TcBtSdDiRegisterServiceRecordCfm 	*pTcBtSDEvt = (TcBtSdDiRegisterServiceRecordCfm *)pTcBtEvt;

    //gDiServiceHandle = pTcBtSDEvt->serviceHandle;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

//***********************************Security Controller msg **************************//

int TC_BT_SC_PasskeyInd(void *pTcBtEvt)
{
    TcBtScPairingSendEvt	*pTcBtSCEvt = (TcBtScPairingSendEvt *)pTcBtEvt;
    TcBtBdAddr *devAddr = &pTcBtSCEvt->deviceAddr;

    if((pTcBtSCEvt != NULL) && (pTcBtSCEvt->deviceName != NULL))
    {
//		TC_BT_SC_LegacyPassKey("0000",4);
        BtApp_GAP_PairingIndHandler(devAddr);
        TcBtEvt_ScEvt(BT_DM_EVT_PINCODE_IND ,pTcBtSCEvt);
        TC_BT_Pfree(pTcBtSCEvt->deviceName);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SC_DebondCfm(void *pTcBtEvt)
{
    TcBtScDebondCfm *pTcBtSCEvt = (TcBtScDebondCfm *)pTcBtEvt;

    if(pTcBtSCEvt != NULL)
    {
        if (pTcBtSCEvt->status == TC_BT_SUCCESS)
        {
        }
        else
        {
        }
  }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}


int TC_BT_SC_SSP_NotificationInd(void *pTcBtEvt)
{
    TcBtScPairingSSPSendEvt	*pTcBtSCEvt = (TcBtScPairingSSPSendEvt *)pTcBtEvt;
    TcBtBdAddr *devAddr = &pTcBtSCEvt->deviceAddr;

    if((pTcBtSCEvt != NULL) && (pTcBtSCEvt->deviceName != NULL))
    {
//		TC_BT_SC_SSP_PasskeyNotification(TRUE);
        TcBtEvt_ScEvt(BT_DM_EVT_SSP_PASSKEY_NOTIFICATION_IND, pTcBtSCEvt);
        BtApp_GAP_PairingIndHandler(devAddr);

        TC_BT_Pfree(pTcBtSCEvt->deviceName);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SC_SSP_CompareInd(void *pTcBtEvt)
{
    TcBtScPairingSSPSendEvt *pTcBtSCEvt = (TcBtScPairingSSPSendEvt *)pTcBtEvt;
    TcBtBdAddr *devAddr = &pTcBtSCEvt->deviceAddr;

    if((pTcBtSCEvt != NULL) && (pTcBtSCEvt->deviceName != NULL))
    {
//		TC_BT_SC_SSP_NumericCompare(TRUE);
        TcBtEvt_ScEvt(BT_DM_EVT_SSP_NUMERIC_COMPARISON_IND, pTcBtSCEvt);
        BtApp_GAP_PairingIndHandler(devAddr);

        TC_BT_Pfree(pTcBtSCEvt->deviceName);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SC_SSP_JustworkInd(void *pTcBtEvt)
{
    TcBtScPairingSendEvt	*pTcBtSCEvt = (TcBtScPairingSendEvt *)pTcBtEvt;
    TcBtBdAddr *devAddr = &pTcBtSCEvt->deviceAddr;

    if((pTcBtSCEvt != NULL) && (pTcBtSCEvt->deviceName != NULL))
    {
         TcBtEvt_ScEvt(BT_DM_EVT_SSP_JUST_WORKS_IND, pTcBtSCEvt);
        BtApp_GAP_PairingIndHandler(devAddr);
        TC_BT_Pfree(pTcBtSCEvt->deviceName);
    }
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_SC_DebugModeCfm(void *pTcBtEvt)
{
    TcBtScDebugModeCfm *pTcBtSCEvt = (TcBtScDebugModeCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

//****************************connect manage msg **********************//

int TC_BT_CM_SetLocalNameCfm(void *pTcBtEvt)
{
    TcBtCmCfm 	*pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadLocalBdAddrCfm(void *pTcBtEvt)
{
    TcBtCmReadLocalBdAddrCfm 	*pTcBtCMEvt = (TcBtCmReadLocalBdAddrCfm *)pTcBtEvt;

   // gLocalAddrFromRead = pTcBtCMEvt->deviceAddr;

    fprintf(stderr,"TC_BT_CM_ReadLocalBdAddrCfm (bdAddr = %04x:%02x:%06x)\n",  pTcBtCMEvt->deviceAddr.nap, pTcBtCMEvt->deviceAddr.uap, pTcBtCMEvt->deviceAddr.lap);
     TcBtInt8					LocalDevName[TC_BT_MAX_FRIENDLY_NAME_LEN+1] = {NULL, };
    (void)sprintf(LocalDevName, "BR_LIN_%06x", pTcBtCMEvt->deviceAddr.lap);
    TcBtEvt_ScEvt(BT_DM_EVT_READLOCALDEVICE, pTcBtCMEvt);
    TC_BT_CM_SetLocalName((unsigned char *)LocalDevName);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteScanEnableCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadScanEnableCfm(void *pTcBtEvt)
{
    TcBtCmReadScanEnableCfm *pTcBtCMEvt = (TcBtCmReadScanEnableCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_EnableDutModeCfm(void *pTcBtEvt)
{
    TcBtCmEnableDutModeCfm	*pTcBtCMEvt = (TcBtCmEnableDutModeCfm *)pTcBtEvt;

    if (pTcBtCMEvt->status == TC_BT_SUCCESS)
    {
    }
    else
    {
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WritePageToCfm(void *pTcBtEvt)
{
    TcBtCmCfm	*pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteCodCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
	printf("TC_BT_CM_WriteCodCfm (status = %d)\n",pTcBtCMEvt->status);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadCodCfm(void *pTcBtEvt)
{
    TcBtCmReadCodCfm *pTcBtCMEvt = (TcBtCmReadCodCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadRemoteNameCfm(void *pTcBtEvt)
{
    TcBtCmReadRemoteNameCfm *pTcBtCMEvt = (TcBtCmReadRemoteNameCfm *)pTcBtEvt;

   if (pTcBtCMEvt->status == TC_BT_SUCCESS)
    {
#ifdef BT_AUTO_CONNECTION
        if((BtApp_GAP_GetRecoveryLinkLostFlag() == BTAPP_GAP_RECOVERY_LINK_LOST_IN))
        {
            TcBtBdAddr *pAddr = TC_BT_SC_DB_ReadLastDevAddr();

            if((pAddr != NULL) && (TC_BT_BdAddrEq((const TcBtBdAddr *)&pTcBtCMEvt->deviceAddr, (const TcBtBdAddr *)pAddr)==TRUE))
            {
                BtApp_GAP_AutoConnect();
                BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_OUT);
                BtApp_GAP_RecoveryLinkLostCancelTimer();
            }
        }
#endif
    }

    if(pTcBtCMEvt->friendlyName != NULL)
    {
        TC_BT_Pfree(pTcBtCMEvt->friendlyName);
    }
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SdcCloseInd(void *pTcBtEvt)
{
    TcBtCmSdcCloseInd *pTcBtCMEvt = (TcBtCmSdcCloseInd *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_WriteLinkPolicyErrorInd(void *pTcBtEvt)
{
    TcBtCmWriteLinkPolicyErrorInd *pTcBtCMEvt = (TcBtCmWriteLinkPolicyErrorInd *)pTcBtEvt;


    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadLinkPolicyCfm(void *pTcBtEvt)
{
    TcBtCmReadLinkPolicyCfm *pTcBtCMEvt = (TcBtCmReadLinkPolicyCfm *)pTcBtEvt;

#if 0
    //Convert sniff modet to active mode test
    if(pTcBtCMEvt->actualMode == TC_BT_ACTIVE_MODE/*TC_BT_SNIFF_MODE*/)
    {
        TcBtUint16 			theLinkPolicySetting = TC_BT_ENABLE_PARK;

        (void)TC_BT_CM_WriteLinkPolicyReqSend(pTcBtCMEvt->deviceAddr, theLinkPolicySetting);
    }
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

#ifdef BT_CHANGE_LSTO
static int gTcBtCmLstoRetryCount = 3;
#endif
int TC_BT_CM_WriteLinkSuperVisionTimeoutCfm(void *pTcBtEvt)
{
    TcBtCmWriteLinkSupervTimeoutCfm *pTcBtCMEvt = (TcBtCmWriteLinkSupervTimeoutCfm *)pTcBtEvt;

#ifdef BT_CHANGE_LSTO
    if(pTcBtCMEvt->status != TC_BT_SUCCESS)
    {
        if(gTcBtCmLstoRetryCount > 0)
        {
            (void)TC_BT_CM_WriteLinkSuperVisionTimeoutReq(pTcBtCMEvt->deviceAddr, BT_LINK_SUPERVISION_TIMEOUT);
            gTcBtCmLstoRetryCount--;
        }
    }
#endif

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_GetLinkQualityCfm(void *pTcBtEvt)
{
    TcBtCmGetLinkQualityCfm *pTcBtCMEvt = (TcBtCmGetLinkQualityCfm *)pTcBtEvt;
   return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ReadRssiCfm(void *pTcBtEvt)
{
    TcBtCmReadRssiCfm *pTcBtCMEvt = (TcBtCmReadRssiCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_AclConnectInd(void *pTcBtEvt)
{
    TcBtCmAclConnectInd *pTcBtCMEvt = (TcBtCmAclConnectInd *)pTcBtEvt;
#ifdef BT_CHANGE_LSTO
    TC_BT_CM_WriteLinkSuperVisionTimeoutReq(pTcBtCMEvt->deviceAddr, BT_LINK_SUPERVISION_TIMEOUT);
#endif

    (void)BtApp_GAP_AddToAclConnList(&(pTcBtCMEvt->deviceAddr));

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_AclDisconnectInd(void *pTcBtEvt)
{
    TcBtCmAclDisconnectInd *pTcBtCMEvt = (TcBtCmAclDisconnectInd *)pTcBtEvt;

    if(pTcBtCMEvt->status == TC_BT_HCI_ERROR_OETC_USER)
    {
        (void)BtApp_GAP_CancelConnect(&pTcBtCMEvt->deviceAddr);
    }
    (void)BtApp_GAP_RemoveFromAclConnList(&(pTcBtCMEvt->deviceAddr));

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_QOSSetupCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_SetAfhChannelsCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
  return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_ResetCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_VSCCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_BluetestTXCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_CM_BluetestRXCfm(void *pTcBtEvt)
{
    TcBtCmCfm *pTcBtCMEvt = (TcBtCmCfm *)pTcBtEvt;
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

void TC_BT_CM_SniffEnableTest(void)
{
    TcBtUint16 			theLinkPolicySetting = TC_BT_ENABLE_SNIFF|TC_BT_ENABLE_PARK;
    (void)TC_BT_CM_WriteLinkPolicyReqSend(gDevAddr, theLinkPolicySetting);
}

void TC_BT_CM_ReadLinkPolicyTest(void)
{
    (void)TC_BT_CM_ReadLinkPolicyReqSend(gDevAddr);
}
