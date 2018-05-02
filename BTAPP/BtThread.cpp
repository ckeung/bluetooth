#include "inc/BtThread.h"
#include "TC_BT/inc/BtUtil.h"
#include "TC_BT/inc/BtPlatform.h"
#include "BtApp_Gap.h"
#include "BT_CFG.h"
#define TC_BT_SYS_EVT (TC_BT_EVT_TYPE_ENUM_BASE)
#define TC_BT_A2DP_EVT (TC_BT_A2DP_EVT_TYPE_ENUM)
#define TC_BT_AVRCP_EVT (TC_BT_AVRCP_EVT_TYPE_ENUM)
#define TC_BT_HF_EVT (TC_BT_HF_EVT_TYPE_ENUM)
#define TC_BT_PAC_EVT (TC_BT_PAC_EVT_TYPE_ENUM)
#define TC_BT_SPP_EVT (TC_BT_SPP_EVT_TYPE_ENUM)
#define TC_BT_SC_EVT (TC_BT_SC_EVT_TYPE_ENUM)
#define TC_BT_SD_EVT (TC_BT_SD_EVT_TYPE_ENUM)
#define TC_BT_CM_EVT (TC_BT_CM_EVT_TYPE_ENUM)
#define TC_BT_MAPC_EVT (TC_BT_MAPC_EVT_TYPE_ENUM)
#define TC_BT_HIDD_EVT (TC_BT_HIDD_EVT_TYPE_ENUM)
#define TC_BT_PAN_EVT (TC_BT_PAN_EVT_TYPE_ENUM)

unsigned char gboBTHfEvent;

BtThread::BtThread()
{
}
BtThread::~BtThread()
{
    BTAPP_Destory_Unlock();
}

static BtThread  _btthread;
BtThread *GetglobalBtThread()
{
    return &_btthread;
}
void BtThread::Initilaize()
{
    TC_BT_InitEvent();
    BTAPP_Create_Lock();
      bStarted = TRUE;
}

void BtThread::run()
{
    unsigned long *msg;
    unsigned long int Cnt = 0;
    while(bStarted)
    {
     msg = TC_BT_ReceiveEvent();
     TcBtEvt * pTcBtEvt;
     gboBTHfEvent = TRUE;
     if(msg != NULL)
     {
             pTcBtEvt = (TcBtEvt*)msg;
             switch (pTcBtEvt->type & 0xFF00)
             {
                 case TC_BT_SYS_EVT:

                       G_BtSys->BtApp_SYS_ReceiveEvent(pTcBtEvt);
                     break;
             case TC_BT_HF_EVT:

                       m_BtHfEvent.BtApp_HF_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_SC_EVT:

                  G_BtDm->BtApp_SC_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_SD_EVT:

                    G_BtDm->BtApp_SD_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_CM_EVT:

                   G_BtDm->BtApp_CM_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_A2DP_EVT:

                    m_BtA2dpEvent.BtApp_A2dp_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_AVRCP_EVT:
                    m_BtAvRcpEvent.BtApp_BtAvRcp_ReceiveEvent(pTcBtEvt);
                 break;
             case TC_BT_PAC_EVT:

                   G_BtPB->BtApp_PB_ReceiveEvent(pTcBtEvt);
                 break;

                 default:
                     break;
             }
             TC_BT_Pfree(pTcBtEvt);
             pTcBtEvt = NULL;
             msg = NULL;

         }
         if((Cnt % 5) == 0) /* 0.1 sec */
         {
             (void)TCCAPI_Timer_CheckTime();
    #ifdef BT_AUTO_CONNECTION
            // BtApp_GAP_RecoveryLinkLostEvent();
    #endif /* BLUETOOTH_AUTO_CONNECTION */
             Cnt = 0;
         }
           Cnt++;
         usleep(5);
    }
         gboBTHfEvent = FALSE;

}
