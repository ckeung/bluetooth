#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/mman.h>
#include "inc/BtSys.h"
#include "inc/TcBtEvt.h"
#include "BT_CFG.h"
#include "BtThread.h"
#include "BtApp_HF.h"
#include "inc/BtApp_Gap.h"
#include "../TC_BT/inc/TC_BT_CM_CMD.h"
#include "../TC_BT/inc/TC_BT_EVT.h"
#include "../TC_BT/inc/TC_BT_HF_DEV.h"
#include "../TC_BT/inc/TC_BT_HF_EVT.h"
#include "../TC_BT/inc/TC_BT_HF_SCO.h"
#include "../TC_BT/inc/TC_BT_A2DP_EVT.h"
#include "../TC_BT/inc/TC_BT_AVRCP_EVT.h"
#include "../TC_BT/inc/TC_BT_PAC_EVT.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "TC_BT/inc/BtPlatform.h"
// static func//
#define _IO(x,y)	(((x)<<8)|y)
#define BT_DEV_MAJOR_NUM 234
#define BT_DEV_MINOR_NUM 1
#define IOCTL_BT_DEV_CTRL_INIT    _IO(BT_DEV_MAJOR_NUM, 106)
#define IOCTL_BT_DEV_CTRL_RESET    _IO(BT_DEV_MAJOR_NUM, 107)

#define BT_DEV_GPIO_HI			1
#define BT_DEV_GPIO_LOW		0
//  main code//
BtSys::BtSys()
{
}
BtSys::~BtSys()
{
}

static BtSys  _btsys;
BtSys *GetglobalBtSys()
{
    return &_btsys;
}
void BtSys::BtApp_SYS_Init()
{

       G_BtSyscmd->TC_BT_SYS_SetLocalDeviceName();
       G_BtSyscmd->TC_BT_SYS_SetLocalDeviceAddr();

#ifdef BT_AUTOMATIC_ON_SUPPORT
        BtApp_SYS_HW_PowerOnCommand();
#endif
    if(G_BtSyscmd->TC_BT_SYS_Activate() != 0)
    {
        BtApp_SYS_ActivateFailCfm(NULL);
    }

}

void BtSys::BtApp_SYS_Deinit()
{
    int iStatus = 0;
    iStatus = G_BtSyscmd->TC_BT_SYS_GetBTStatus();

    if((iStatus == TC_BT_SYS_Idle)
        || (iStatus == TC_BT_SYS_Deactivating))
    {
        return;
    }

    (void)G_BtSyscmd->TC_BT_SYS_Deactivate();
}

bool BtSys::BtApp_SYS_SetStatus(unsigned int uiStatus)
{
        unsigned char ucGroup;
        unsigned char ucDevNo;
        unsigned short usStatus;

        ucGroup=BTAPP_STATUS_MASK_GET_GROUP(uiStatus);
        if(ucGroup>=BTAPP_STATUS_GROUP_MAX)
        {
            return FALSE;
        }
        ucDevNo=BTAPP_STATUS_MASK_GET_DEVNO(uiStatus);
        usStatus=BTAPP_STATUS_MASK_GET_STATUS(uiStatus);

        if (BtApp_SYS_GetStatus(uiStatus)==TRUE)
        {
            return FALSE;
        }

        gBtAppSysCurStatus[ucGroup+ucDevNo] |= usStatus;
        return TRUE;
}

char BtSys::BtApp_SYS_GetStatus(unsigned int uiStatus)
{
    char cRet = FALSE;
    unsigned char ucGroup;
    unsigned char ucDevNo;
    unsigned short usStatus;

    ucGroup=BTAPP_STATUS_MASK_GET_GROUP(uiStatus);
    if(ucGroup>=BTAPP_STATUS_GROUP_MAX)
    {
        return cRet;
    }
    ucDevNo=BTAPP_STATUS_MASK_GET_DEVNO(uiStatus);
    usStatus=BTAPP_STATUS_MASK_GET_STATUS(uiStatus);

    cRet = (((gBtAppSysCurStatus[ucGroup+ucDevNo] & usStatus)==usStatus) ? TRUE : FALSE);
    return cRet;
}

bool BtSys::BtApp_SYS_ClearStatus(unsigned int uiStatus)
{
        unsigned char ucGroup;
        unsigned char ucDevNo;
        unsigned short usStatus;

        ucGroup=BTAPP_STATUS_MASK_GET_GROUP(uiStatus);
        if(ucGroup>=BTAPP_STATUS_GROUP_MAX)
        {
            memset(gBtAppSysCurStatus,0,sizeof(gBtAppSysCurStatus));
            return FALSE;
        }
        ucDevNo=BTAPP_STATUS_MASK_GET_DEVNO(uiStatus);
        usStatus=BTAPP_STATUS_MASK_GET_STATUS(uiStatus);

        if (BtApp_SYS_GetStatus(uiStatus)==FALSE)
        {
            return FALSE;
        }

        switch (uiStatus)
        {
            case BTAPP_STATUS_HW_AVAILABLE:
                memset(gBtAppSysCurStatus,0,sizeof(gBtAppSysCurStatus));
                break;

            default:
                gBtAppSysCurStatus[ucGroup+ucDevNo] &= (~usStatus);
                break;
        }

        return TRUE;
}
unsigned int BtSys::BtApp_SYS_GetTick()
{
    return G_BtSyscmd->TC_BT_SYS_GetTick();
}
void BtSys::BtApp_SYS_HW_PowerOnCommand()
{
#if defined(__arm__) && !defined(BT_EACH_RUN)
    int fd = -1;
    printf("============ %s ============ \n",__FUNCTION__);
    fd= open("/dev/tcc_bt_dev", O_RDWR | O_SYNC);

    if(fd == -1)
    {
        printf("Opening file /dev/tcc_bt_dev -> Faii!!!\n");
    }
    else
    {
        ioctl(fd, IOCTL_BT_DEV_CTRL_INIT, NULL);
        usleep(10000);

        ioctl(fd, IOCTL_BT_DEV_CTRL_RESET, BT_DEV_GPIO_HI);
        usleep(25000);
        ioctl(fd, IOCTL_BT_DEV_CTRL_RESET, BT_DEV_GPIO_LOW);
        usleep(250000);
        ioctl(fd, IOCTL_BT_DEV_CTRL_RESET, BT_DEV_GPIO_HI);
        usleep(25000);

        close(fd);
    }
#endif
        (void)BtApp_SYS_SetStatus(BTAPP_STATUS_HW_AVAILABLE);
}

void BtSys::BtApp_SYS_HW_PowerOffCommand()
{
#if defined(__arm__) && !defined(BT_EACH_RUN)
    int fd = -1;
    printf("============ %s ============ \n",__FUNCTION__);
    fd= open("/dev/tcc_bt_dev", O_RDWR | O_SYNC);

    if(fd == -1)
    {
        printf("Opening file /dev/tcc_bt_dev -> Faii!!!\n");
    }
    else
    {
        ioctl(fd, IOCTL_BT_DEV_CTRL_RESET, BT_DEV_GPIO_LOW);
        usleep(250000);

        close(fd);
    }
#endif
    BtApp_SYS_ClearStatus(BTAPP_STATUS_HW_AVAILABLE);
}

void BtSys::BtApp_SYS_SetBasicMode(unsigned char ucMode)
{
    gBasicMode = ucMode;
}

unsigned char BtSys::BtApp_SYS_GetBasicMode()
{
    return gBasicMode;
}

bool BtSys::BtApp_SYS_Is_BusyStatusHFP(unsigned char ucDevNo)
{
    char boBusy = FALSE;
        //char boStatusHFAudio = FALSE;
        char boStatusHFRing = FALSE;
        char boStatusHFOutgoing = FALSE;
        //char boStatusHFOutgoingByHF = FALSE;
        char boStatusHFCall = FALSE;
        char boStatusHSRing = FALSE;
        char boStatusHSAudio = FALSE;
        //boStatusHFAudio = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_AUDIO,ucDevNo));
        boStatusHFRing = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_RING,ucDevNo));
        boStatusHFOutgoing = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_OUTGOING,ucDevNo));
        //boStatusHFOutgoingByHF = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_OUTGOING_BY_HF,ucDevNo));
        boStatusHFCall = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HFx_CALL,ucDevNo));
        boStatusHSRing = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HSx_RING,ucDevNo));
        boStatusHSAudio = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_HSx_AUDIO,ucDevNo));

        if(0
            //|| boStatusHFAudio
            || (boStatusHFRing == TRUE)
            || (boStatusHFOutgoing == TRUE)
            //|| (boStatusHFOutgoingByHF == TRUE)
            || (boStatusHFCall == TRUE)
            || (boStatusHSRing == TRUE)
            || (boStatusHSAudio == TRUE)
            //|| (gBtAppHfVoiceRecognition[ucDevNo]==TRUE)
            )
        {
            boBusy=TRUE;
        }

        return boBusy;
}

int BtSys::BtApp_SYS_ReceiveEvent(TcBtEvt *event)
{
        unsigned short int evtType;
        evtType = event->type;
        switch(evtType)
        {
            case TC_BT_SYS_HW_CLEAR_WARM_RESET_IND:
            {
               
                BtApp_SYS_HWClearWarmResetInd(event);
            }break;
            case TC_BT_SYS_HW_SET_WARM_RESET_IND:
            {  
                BtApp_SYS_HWSetWarmResetInd(event);
               }break;
            case TC_BT_SYS_BSA_ACTIVATE_FAIL_CFM:
            {  
                BtApp_SYS_BSA_ActivateFailCfm(event);
            }break;
            case TC_BT_SYS_BSA_ACTIVATE_SUCCESS_CFM:
            {  
                BtApp_SYS_BSA_ActivateSuccessCfm(event);
            }break;
            case TC_BT_SYS_ACTIVATE_FAIL_CFM:
            {  
                BtApp_SYS_ActivateFailCfm(event);
            }break;
           case TC_BT_SYS_ACTIVATE_SUCCESS_CFM:
            {  
                BtApp_SYS_ActivateSuccessCfm(event);
             }break;
           case TC_BT_SYS_DEACTIVATE_CFM:
             {  
               BtApp_SYS_DeactivateCfm(event);
            }break;
            default:
                break;
        }
}

int BtSys::BtApp_SYS_HWClearWarmResetInd(void *pTcBtEvt)
{

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_HWSetWarmResetInd(void *pTcBtEvt)
{
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_BSA_ActivateFailCfm(void *pTcBtEvt)
{
    int status=0;
    printf("============ BtApp_SYS_BSA_ActivateFailCfm ============ \n");
   TC_BT_SC_DB_DeInitScDb();

    G_BtSyscmd->TC_BT_SYS_SetBTStatus(TC_BT_SYS_Idle);
    BtApp_SYS_ClearStatus(BTAPP_STATUS_TASK_INIT);
    TcBtEvt_SysEvt(BT_SYS_EVT_INIT_SVR_ERR);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_BSA_ActivateSuccessCfm(void *pTcBtEvt)
{
    int fr = 0;
    printf("============ BtApp_SYS_BSA_ActivateSuccessCfm ============ \n");

    fr = BsaAppEntry(TC_BT_SOCKET_PATH);

    if(fr != 0)
    {
        TcBtEvt*pTcBtSysEvt = NULL;

        pTcBtSysEvt = (TcBtEvt*)TC_BT_Pmalloc(sizeof(TcBtEvt));
        pTcBtSysEvt->type = TC_BT_SYS_ACTIVATE_FAIL_CFM;
        TC_BT_SendEvent(pTcBtSysEvt);
       //BtApp_SYS_BSA_ActivateFailCfm(pTcBtSysEvt);
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_ActivateFailCfm(void *pTcBtEvt)
{
    int status=0;
    printf("============ %s ============ \n",__FUNCTION__);
        TC_BT_SC_DB_DeInitScDb();

        (void)G_BtSyscmd->TC_BT_SYS_SetBTStatus(TC_BT_SYS_Idle);

        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_TASK_INIT);

        //TCCAPI_Timer_SetTime(5, TIMER_BT_DISCONNECT_AT_BTOFF, BtApp_SYS_DeinitTimer);

        TcBtEvt_SysEvt(BT_SYS_EVT_INIT_ERR);

        /*************************** TASK DELETE END ***************************/

        return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_ActivateSuccessCfm(void *pTcBtEvt)
{
    G_BtSyscmd->TC_BT_SYS_SetBTStatus(TC_BT_SYS_Activated);

    BtApp_GAP_InitAclConnList();
    BtApp_GAP_InitMainConnStatusInst();
    BtApp_GAP_InitConnInst();
    BtApp_GAP_InitCcanInst();
    BtApp_GAP_InitDiscInst();
    BtApp_GAP_InitRecoveryInst();

    (void)TC_BT_CM_WriteCod(TC_BT_COD_AUDIO_MAJOR_SERVICE_MASK, TC_BT_COD_AV_MAJOR_DEVICE_MASK, TC_BT_COD_HANDSFREE_AV_MINOR_DEVICE_CLASS);

    TcBtEvt_SysEvt(BT_SYS_EVT_INIT_CFM);

    (void)BtApp_SYS_SetStatus(BTAPP_STATUS_TASK_INIT);
    (void)TC_BT_CM_ReadLocalBdAddr();
#ifdef BT_CHANGE_LSTO
    TC_BT_CM_RoleSwitchConfigReq(TC_BT_ROLE_CONFIG_HYBRID_ROLE_SWITCH);
#endif
    BtApp_GAP_ActivateProfilesAutomatically();

    //TCCAPI_Timer_SetTime(10, TIMER_BT_TEST_ONOFF_ID, BtApp_SYS_Off); //For BT OnOff Test

    BtApp_GAP_SetAuthoriseFlag(TRUE);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_DisconnectAll(void)
{
    int i;
   printf("============ %s ============ \n",__FUNCTION__);
#ifdef BT_HANDSFREE_UNIT
    /* HF disconnect */
    for(i = 0; i < BT_HFP_MAX_CONNECTIONS; i++)
    {
        char Res1, Res2, Res3;

        TC_BT_HF_DEV_SetCurrentNum(i);
        BtApp_HF_SetCurrDevNum(i);

        /* HF incoming/outoing/call */
        Res1 = BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL));
        Res2 = BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING));
        Res3 = BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
        if((Res1!=FALSE) ||	(Res2!=FALSE) || (Res3!=FALSE))
        {
            BtApp_HF_CIEVINDCallHandler(0);
        }

        /* HF audio */
        if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO))==TRUE)
        {
            TcBtHfAudioDisconnectCfm TcBtHfEvtTmp;
            TcBtHfEvtTmp.type=TC_BT_HF_AUDIO_DISCONNECT_IND;
            if(TC_BT_HF_DEV_GetBdAddr(i) != NULL)
            {
                TcBtHfEvtTmp.bdAddr=*TC_BT_HF_DEV_GetBdAddr(i);
            }
            TcBtHfEvtTmp.scoHandle=TC_BT_HF_NO_SCO_HANDLE;
            TcBtHfEvtTmp.resultCode=TC_BT_RESULT_CODE_HF_SUCCESS;
            TcBtHfEvtTmp.resultSupplier=TC_BT_SUPPLIER_HF;
            (void)TC_BT_HF_AudioDisconnectInd(&TcBtHfEvtTmp);
        }

        if(BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CONNECT))==TRUE)
        {
            TcBtHfDisconnectInd TcBtHfEvtTmp;

            TcBtHfEvtTmp.type=TC_BT_HF_DISCONNECT_CFM;
            TcBtHfEvtTmp.bdAddr=*TC_BT_HF_DEV_GetBdAddr(i);
            TcBtHfEvtTmp.reasonCode=TC_BT_RESULT_CODE_HF_SUCCESS;
            TcBtHfEvtTmp.reasonSupplier=TC_BT_SUPPLIER_HF;
            (void)TC_BT_HF_DisconnectCfm(&TcBtHfEvtTmp);
        }
    }

    TC_BT_HF_DEV_SetCurrentNum(0);
    BtApp_HF_SetCurrDevNum(0);

    /* HF deactivate */
    if(BtApp_SYS_GetStatus(BTAPP_STATUS_HF_ACTIVATE)==TRUE)
    {
        TcBtHfDeactivateCfm TcBtHfEvtTmp;
        TcBtHfEvtTmp.type=TC_BT_HF_DEACTIVATE_CFM;
        TcBtHfEvtTmp.resultCode=TC_BT_RESULT_CODE_HF_SUCCESS;
        TcBtHfEvtTmp.resultSupplier=TC_BT_SUPPLIER_HF;
        (void)TC_BT_HF_DeactivateCfm(&TcBtHfEvtTmp);
    }
#endif

#ifdef BT_AUDIO_SINK
    if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_CONNECT)==TRUE)
    {
        TcBtA2dpDisconnectInd TcBtA2dpEvt;

        TcBtA2dpEvt.type=TC_BT_A2DP_DISCONNECT_IND;
        TcBtA2dpEvt.avResultFlag=TRUE;
        TcBtA2dpEvt.localTerminated=TRUE;
        TcBtA2dpEvt.reasonCode=TC_BT_HCI_SUCCESS;
        TcBtA2dpEvt.reasonSupplier=TC_BT_SUPPLIER_HCI;
        (void)TC_BT_A2DP_DisconnectInd(&TcBtA2dpEvt);
    }

    if(BtApp_SYS_GetStatus(BTAPP_STATUS_AVRCP_CTRL_CONNECT)==TRUE)
    {
        TcBtAvrcpDisconnectInd TcBtAvrcpEvt;

        TcBtAvrcpEvt.type=TC_BT_AVRCP_DISCONNECT_IND;
        TcBtAvrcpEvt.flag=TRUE;
        TcBtAvrcpEvt.localTerminated=TRUE;
        TcBtAvrcpEvt.reasonCode=TC_BT_HCI_SUCCESS;
        TcBtAvrcpEvt.reasonSupplier=TC_BT_SUPPLIER_HCI;
        (void)TC_BT_AVRCP_DisconnectInd(&TcBtAvrcpEvt);
    }

    if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_ACTIVATE)==TRUE)
    {
        TcBtA2dpDeactivateCfm TcBtA2dpEvt;

        TcBtA2dpEvt.type=TC_BT_A2DP_DEACTIVATE_CFM;
        TcBtA2dpEvt.avResultSupplier=TC_BT_SUPPLIER_AV;
        TcBtA2dpEvt.avResultCode=TC_BT_HCI_SUCCESS;
        (void)TC_BT_A2DP_DeactivateCfm(&TcBtA2dpEvt);
    }
#endif

#ifdef BT_PBAP_CLIENT
    if(BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT)==TRUE)
    {
        TcBtPacDisconnectInd TcBtPacEvtTmp;

        TcBtPacEvtTmp.type=TC_BT_PAC_DISCONNECT_IND;
        TcBtPacEvtTmp.reasonCode=TC_BT_HCI_SUCCESS;
        TcBtPacEvtTmp.reasonSupplier=TC_BT_SUPPLIER_HCI;
        (void)TC_BT_PAC_DisconnectInd(&TcBtPacEvtTmp);
    }
#endif
#ifdef BT_SPP
    if(TC_BT_CM_GetSPPStatus(0, NULL) == TC_BT_CM_CONNECTED)
    {
#if 0
        if(TimerCnt == 0)
        {
            (void)TC_BT_SPP_Disconnect(0);
        }
        else
        {
            TCCAPI_Timer_SetTime(TimerCnt, TIMER_BT_DISCONNECT_ID5, TC_BT_EX_SPP_Disconnect);
        }

        TimerCnt++;
#endif
    }
#endif

#if defined(BT_SPP) && defined(IPOD_BT_INCLUDE)
    if(TC_BT_CM_GetSPPIAPStatus(NULL) == TC_BT_CM_CONNECTED)
    {
#if 0
        if(TimerCnt == 0)
        {
            (void)TC_BT_SPP_IAP_Disconnect();
        }
        else
        {
            TCCAPI_Timer_SetTime(TimerCnt, TIMER_BT_DISCONNECT_ID6, TC_BT_EX_SPP_IAP_Disconnect);
        }

        TimerCnt++;
#endif
    }
#endif

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
int BtSys::BtApp_SYS_DeactivateCfm(void *pTcBtEvt)
{
    int status=0;
   printf("============ %s ============ \n",__FUNCTION__);
    TC_BT_SC_DB_DeInitScDb();

    (void)BtApp_SYS_DisconnectAll();

    BtApp_SYS_HW_PowerOffCommand();

#ifdef BT_HANDSFREE_UNIT
    (void)TC_BT_HF_SCO_Deinit();

    (void)TC_BT_HF_DEV_Deinit();
#endif

    //TCCAPI_Timer_SetTime(5, TIMER_BT_TEST_ONOFF_ID, BtApp_SYS_On); //For BT OnOff Test

    G_BtSyscmd->TC_BT_SYS_SetBTStatus(TC_BT_SYS_Idle);

    (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_TASK_INIT);

    if(BtApp_SYS_GetStatus(BTAPP_STATUS_HW_AVAILABLE) == TRUE)
    {
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_HW_AVAILABLE);
        (void)BtApp_SYS_SetStatus(BTAPP_STATUS_HW_AVAILABLE);
    }
    else
    {
        (void)BtApp_SYS_ClearStatus(BTAPP_STATUS_HW_AVAILABLE);
    }
    BTAPP_Destory_Unlock();
    G_BtThread->bStarted = FALSE;
    G_BtThread->exit();
    TcBtEvt_SysEvt(BT_SYS_EVT_DEINIT_CFM);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int BtSys::BtApp_SYS_NotSupportCmdCfm(void *pTcBtEvt)
{
    TcBtSysNotSupportFuncEvt *pTcBtSysEvt = (TcBtSysNotSupportFuncEvt*)pTcBtEvt;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
