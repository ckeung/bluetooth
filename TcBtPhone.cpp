/****************************************************************************
 *   FileName    : TcBtApp.cpp
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
*  Description : This file contains functions and variables for a UI body
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    13/12/22     0.1            created                     Kwon Sunhong
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <QDebug>
#include "BtApp_Gap.h"
#include "TcBtPhone.h"
#include "TcBtEvt.h"
#include "TcBtHFEvt.h"
#include "BT_CFG.h"
#include "TC_BT.h"
#include "BtApp_Av.h"
#include "BTAPP.h"
#include "BtPhoneBookCmd.h"
#include "TC_BT/inc/TC_BT_SC_EVT.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/TC_BT_CM_EVT.h"
#include"TC_BT/inc/BtPlatform.h"
#ifdef BT_HANDSFREE_UNIT
#endif
extern unsigned char gKorUtf8CharacterTable[BT_PB_SEARCH_KOR_COUNT][BT_PB_SEARCH_CHAR_SIZE];
static bool bStart = FALSE;
//TcBtapp global instance
#if defined(__arm__) && !defined(BT_NO_LACUCHER)
int g_appID = -1;
#endif
#define BT_CONTACTS_MAX	9
static void Tc_Bt_StartBTAudio(void);
static void Tc_Bt_StoptBTAudio(void);
static INT32U GetData4Bytes(const INT8U *pBufData)
{
    INT32U lIntData;
    INT32U lTmpData;
    lTmpData = pBufData[1];
    lIntData = pBufData[0] | (lTmpData << 8);
    lTmpData = pBufData[2];
    lIntData |= (lTmpData << 16);
    lTmpData = pBufData[3];
    lIntData |= (lTmpData << 24);
    return lIntData;
}
TcBtPhone::TcBtPhone(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<unsigned short int >("unsigned short int ");
    pMsgForClose = NULL;
    m_DBSignal = new TcBtAppDBSignal;
    callFromPhone = false;
    callIncoming = false;
    outgoingAudioOpened = false;
    callTime =0;
    connect(m_DBSignal, SIGNAL(SYS_Event(int)), this, SLOT(OnSYSHandler(int)), Qt::QueuedConnection);
    connect(m_DBSignal, SIGNAL(HF_Event(int , void *)), this, SLOT(OnHFHandler(int , void *)), Qt::QueuedConnection);
    connect(m_DBSignal, SIGNAL(DM_Event(int,void *)), this, SLOT(OnDMHandler(int, void *)), Qt::QueuedConnection);
    connect(m_DBSignal, SIGNAL(PB_Event(int , void *)), this, SLOT(OnPBHandler(int , void *)), Qt::QueuedConnection);
    connect(m_DBSignal, SIGNAL(MAP_Event(int, void *)), this, SLOT(OnMAPHandler(int, void *)), Qt::QueuedConnection);

}

TcBtPhone::~TcBtPhone()
{

}
TcBtPhone *TcBtPhone::getInstance()
{
    static TcBtPhone tcapp;
    return &tcapp;
}

bool TcBtPhone::sysActivate(void)
{
    G_BtThread->Initilaize();
    G_BtThread->start();
    G_BtSys->BtApp_SYS_Init();

}

void BtApp_SYS_DeinitTimer2(unsigned char id, unsigned char loc)
{
    G_BtSys->BtApp_SYS_Deinit();
}

void BtApp_SYS_DeinitTimer(unsigned char id, unsigned char loc)
{
    if(BtApp_GAP_DisconnectAllAtBTDeactivate() != 0)
    {
        TCCAPI_Timer_SetTime(50, TIMER_BT_DISCONNECT_AT_BTOFF, BtApp_SYS_DeinitTimer2);
    }
    else
    {
        (void)G_BtSys->BtApp_SYS_Deinit();	// request delete all tasks.
    }
}


bool TcBtPhone::sysDeactivate(void)
{
        unsigned char ucDevNo=0;
        int		iIdx;
        int iBTStatus;

        iBTStatus = TC_BT_SYS_GetBTStatus();
    #if 1
        if((iBTStatus == TC_BT_SYS_Idle)
            || (iBTStatus == TC_BT_SYS_Deactivating))
    #else
        if (Bluetooth_GetTaskStatus()!=BT_TASK_ACTIVE/* active */
            || !UI_API_GetBluetoothStatus(BTAPP_STATUS_MASK_TASK_INIT) )
    #endif
        {
            return 0;
        }

    #ifdef BT_AUDIO_SINK
        if (G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_A2DP_SINK)
        {
            BtApp_AV_SNK_AudioEnd();
        }
    #endif

    #ifdef BT_HANDSFREE_UNIT
        ucDevNo=BtApp_HF_GetSelectedDevNum();
        BtApp_HF_SetCurrDevNum(ucDevNo);

        /* handsfree */
    #ifdef BT_HFP_RING_FOR_NO_IN_BAND
        if (G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_HFP_RING)
        {
            //BtApp_HF_RejectCall();
            BtApp_HF_SetIRingEnd();
            //BtApp_HF_EvtPowerOff(ucDevNo);
        }
    #endif

        if (G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_HFP_CALL)
        {
            //BtApp_HF_TerminateCall();
            BtApp_HF_SetCallEnd();
            BtApp_HF_SetOutgoingEnd();
            //BtApp_HF_EvtPowerOff(ucDevNo);
        }

        if(BtApp_HF_VoiceRecognitionLearning() != 0)
        {
            BtApp_HF_VoiceRecognitionLearningToggle();
            //BtApp_HF_TerminateVoiceRecognition();
            BtApp_HF_Close_Call();
            //BtApp_HF_EvtPowerOff(ucDevNo);
        }

        if(BtApp_HF_VoiceRecognition() != 0)
        {
            BtApp_HF_VoiceRecognitionInd(FALSE);
            //BtApp_HF_TerminateVoiceRecognition();
            BtApp_HF_Close_Call();
            //BtApp_HF_EvtPowerOff(ucDevNo);
        }

        /* headset */

    #endif

        BtApp_GAP_SetAuthoriseFlag(FALSE);

        if(BtApp_GAP_DisconnectAllAtBTDeactivate() != 0)
        {
            TCCAPI_Timer_SetTime(50, TIMER_BT_DISCONNECT_AT_BTOFF, BtApp_SYS_DeinitTimer);
        }
        else
        {
            G_BtSys->BtApp_SYS_Deinit();	// request delete all tasks.
        }
}

bool TcBtPhone::deviceSearchStart(int totalSearchTime, int maxDevices, short update)
{
    G_BtDm->StartSearchDevice(totalSearchTime,maxDevices,update);
}

bool TcBtPhone::deviceSearchCancel(void)
{
   G_BtDm->CancelSearchDevice();
}

bool TcBtPhone::deviceConnect(int  DeviceNum)
{
    G_BtDm->ConnectDevice(DeviceNum);
}

bool TcBtPhone::deviceDisconnect(int  DeviceNum)
{
   G_BtDm->DisConnectDevice(DeviceNum);
}

bool TcBtPhone::deviceDelete(int  DeviceNum)
{
   G_BtDm->DeleteDevice(DeviceNum);
}
bool TcBtPhone::deviceAllDelete(void)
{
   G_BtDm->DeleteAll();
}

bool TcBtPhone::deviceCompare(bool bAccept)
{
   G_BtDm->SC_SSP_NumericCompare(bAccept);
}

bool TcBtPhone::getContactInfo(void)
{
    unsigned char					searchKeyword[BT_PB_SEARCH_CHAR_SIZE];
    memcpy(searchKeyword, &gEngCharacterTable[0], BT_PB_SEARCH_CHAR_SIZE);
    getContactCount = 0;
    //G_BtPB->BtApp_PB_CmdGetContactsList(0,9,searchKeyword);
    G_BtPB->BtApp_PB_CmdGetContactsListInfo(getContactCount,searchKeyword);
    getContactCount ++;

}

bool TcBtPhone::getHistoryCalInfo(void)
{
    G_BtPB->BtApp_PB_CmdPACPullCallHistory(BTAPP_PB_SELECT_CCH);
}

bool TcBtPhone::cmdReDial( void)
{
    G_BtHf->BtApp_HF_CmdDialRedial(0);
}
bool TcBtPhone::cmdDialByNum( unsigned char  * contactPhNum,short numSize)
{

    G_BtPB->BtApp_PB_CmdDialPlaceByNumber(0,numSize,contactPhNum);
}

bool TcBtPhone::cmdDialByName(const unsigned char  * contactName,short nameSize)
{
}

bool TcBtPhone::cmdIncomingAnswer(void)
{
    G_BtHf->BtApp_HF_CmdIncomingAnswer(0);
}

bool TcBtPhone::cmdIncomingReject(void)
{
        G_BtHf->BtApp_HF_CmdIncomingReject(0);
}
bool TcBtPhone::cmdEndCal(void)
{
        G_BtHf->BtApp_HF_CmdCallTerminate(0);
}
bool TcBtPhone::cmdHangUp(void)
{
        G_BtHf->BtApp_HF_CmdIncomingResponseAndHold(0,0);
}

bool TcBtPhone::cmdChangeVoiceMode(void)
{
    G_BtHf->BtApp_HF_CmdCallAudioOnOff(0);
}
bool TcBtPhone::cmdSetVoiceMute(void)
{
    G_BtHf->BtApp_HF_CmdCallMicMute(0);
}

void TcBtPhone::cmdVoiceRecognition(void)
{
    G_BtHf->BtApp_HF_CmdVoiceRecognition(0);
}

bool TcBtPhone::cmdNumBtnClicked(int Num)
{
    unsigned char data=0;
    const U8 aucDtmf[]="123456789*0#ABCD";
    data=aucDtmf[Num-BT_HF_MAIN_BTN_DIAL_1];		/* Set data as start enum 0 */
    G_BtHf->BtApp_HF_CmdCallDTMF(0, data);
}
void  TcBtPhone::cmdOpenPhoneAudio()
{             
  // G_BtHf->BtApp_HF_CmdOpenAudio();
}
void  TcBtPhone::cmdStartOutgoingCall()
{
      fprintf(stderr,"come to here %s  %d\n",__FUNCTION__,__LINE__);
   G_BtHf->BtApp_HF_CmdOutgoingStart(0);
}
void  TcBtPhone::cmdStartIncomingCall()
{
            fprintf(stderr,"come to here %s   %d   %d,\n",__FUNCTION__,__LINE__);
    if (outgoingAudioOpened == false)
    {
        G_BtHf->BtApp_HF_CmdIncomingStart(0);
        outgoingAudioOpened = true;
    }
}
void TcBtPhone::OnSYSHandler(int result)
{
    switch (result)
    {
            case BT_SYS_EVT_INIT_SVR_ERR:
            {
                  //  BtappSenddeviceDBusToUI(1,(unsigned char *)"Could not execute to BSA server.");
                emit btinitBtappFaile();
            }
                break;
            case BT_SYS_EVT_INIT_ERR:
            {
                //char *err="Could not connect to BSA server";
                emit  btinitBtappFaile();
            }
                break;
            case BT_SYS_EVT_INIT_CFM:
            {

            }
                break;
            case BT_SYS_EVT_DEINIT_CFM:
            {
                emit btdeInitBtappSuccess();
            }
                break;
            default:
                break;
        }
}


void TcBtPhone::OnDMHandler(int type,void *pTcBtEvt)
{
    printf("===========OnDMHandler===result = %d=============\n",type);
    switch (type)
    {
        case BT_DM_EVT_BOND_SUCCESS:
        {
            //If top widow is DM widow, we need to refresh device list.
            TcBtEvt_DMEvt(BT_DM_EVT_REFRESH,NULL);
        }
            break;
        case BT_DM_EVT_BOND_ERR:
        {

        }
            break;
        case BT_DM_EVT_PINCODE_IND:
        {

        }
            break;
        case BT_DM_EVT_SSP_NUMERIC_COMPARISON_IND:
        {
            //G_BtDm->SC_SSP_NumericCompare(1);
            TcBtScPairingSSPSendEvt *pTcBtSCEvt = (TcBtScPairingSSPSendEvt *)pTcBtEvt;
            char npassword[24];
            char naddr[64];
            sprintf(npassword,"%d",pTcBtSCEvt->numericValue);
            sprintf(naddr,"%02x:%06x:%x",pTcBtSCEvt->deviceAddr.nap,pTcBtSCEvt->deviceAddr.lap,pTcBtSCEvt->deviceAddr.uap);
          //QString name((char*)pTcBtSCEvt->deviceName);
          //const QChar *data =(const QChar *)(pTcBtSCEvt->deviceName);
          QString name((char *)pTcBtSCEvt->deviceName);
            QString password((char*)npassword);
            QString addr((char*)naddr);
            emit btdeviceCompare(name,password,addr);
            if(pTcBtSCEvt->deviceName != NULL)
            {
                TC_BT_Pfree(pTcBtSCEvt->deviceName);
            }

        }
            break;
        case BT_DM_EVT_SSP_PASSKEY_NOTIFICATION_IND:
        {
            TcBtScPairingSSPSendEvt *pTcBtSCEvt = (TcBtScPairingSSPSendEvt *)pTcBtEvt;

        }
            break;
        case BT_DM_EVT_SSP_JUST_WORKS_IND:
        {
            TcBtScPairingSendEvt *pTcBtSCEvt = (TcBtScPairingSendEvt *)pTcBtEvt;
        }
        break;
        case BT_DM_EVT_CONNECTED:
        {
            TcBtScPairingSendEvt *pTcBtSCEvt = (TcBtScPairingSendEvt *)pTcBtEvt;
           // memcpy(deviceName ,pTcBtSCEvt->deviceName,sizeof(pTcBtSCEvt->deviceName));
               char naddr[64];
              // sprintf(npassword,"%d",pTcBtSCEvt->numericValue);
               sprintf(naddr,"%02x:%06x:%x",pTcBtSCEvt->deviceAddr.nap,pTcBtSCEvt->deviceAddr.lap,pTcBtSCEvt->deviceAddr.uap);
            // QString name((char*)pTcBtSCEvt->deviceName);
             //const QChar *data =(const QChar *)(pTcBtSCEvt->deviceName);
             QString name((char *)pTcBtSCEvt->deviceName);
              QString addr((char*)naddr);
              emit btdeviceConnectSuccess(name,addr);
        }break;

    case BT_DM_EVT_SEARCH_RESULT:
    {
        TcBtSdSearchResultInd *pTcBtSDEvt = (TcBtSdSearchResultInd *)pTcBtEvt;
        G_BtDm->AddSearchList(pTcBtSDEvt->deviceAddr, pTcBtSDEvt->deviceClass, pTcBtSDEvt->rssi, (char*)pTcBtSDEvt->deviceName);
        //QString s((char*)pTcBtSDEvt->deviceName);
        //const QChar *data =(const QChar *)(pTcBtSDEvt->deviceName);
        QString s(( char*)pTcBtSDEvt->deviceName);
        //qDebug() <<s;
        emit btdeviceSearchSuccess(s);
       if(pTcBtSDEvt->deviceName != NULL)
       {
               TC_BT_Pfree(pTcBtSDEvt->deviceName);
       }

        printf("==========AddSearchList====Line %d=====\n",__LINE__);
    }break;

        case  BT_DM_EVT_SEARCH_CLOSE:
        {
            emit btdeviceSearchClosed();
        }break;
        case  BT_DM_EVT_REFRESH:
        {
            G_BtDm->Handler(type,pTcBtEvt);
        }break;
    case BT_DM_EVT_READLOCALDEVICE:
    {
        char naddr[64];
        TcBtInt8					LocalDevName[TC_BT_MAX_FRIENDLY_NAME_LEN+1] = {NULL, };
        TcBtCmReadLocalBdAddrCfm 	*pTcBtCMEvt = (TcBtCmReadLocalBdAddrCfm *)pTcBtEvt;

         (void)sprintf(LocalDevName, "BR_LIN_%06x", pTcBtCMEvt->deviceAddr.lap);
        sprintf(naddr,"%02x:%06x:%x",pTcBtCMEvt->deviceAddr.nap,pTcBtCMEvt->deviceAddr.lap,pTcBtCMEvt->deviceAddr.uap);
        QString name(LocalDevName);
         QString addr((char*)naddr);
        emit btinitBtappSuccess(name,addr);
    }break;
        default:
            break;
}
   // if(pTcBtEvt != NULL)
   // {
     //   TC_BT_Pfree(pTcBtEvt);
   // }
}


#if 1
void TcBtPhone::setState(State state)
{
    m_state = state;
}
int TcBtPhone::EventProcess(unsigned int event_type, int param1, const char* param2)
{
    UI_EVENT_TYPE *pstEvent = (UI_EVENT_TYPE *)event_type;
    unsigned char src;
    unsigned char ucDevNo,ucTmpDevNo;
    unsigned char ucValue;

    unsigned char loc;
    const unsigned char *data;
    unsigned short id;
    static char tmp[16];

    int uniStrlength = 0;
    static unsigned char strUniTemp[256];

    unsigned short dataLength = 0;
    int totalLength = 0;

    BT_HF_STATUS status=BT_HF_STATUS_DEACTIVATED;
#ifdef BT_HFP_WIDEBAND_SPEECH
    unsigned char boCodecUpdated=FALSE;
#endif

    src = MSG_SRC_BT_HF;
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_HW_AVAILABLE) == TRUE)
    {
        //location = TccUISys_GetDiskLocation(MSG_SRC_BT_SYS);
        /* All events must be delivered, as if HF is activated */
        loc = BT_HF_WINDOW_ACTIVATED;
     }

    id = pstEvent->id;

    totalLength = param1;
    data = (unsigned char *)param2;

    if(*data == 0xFF && totalLength > 2)
    {
        data++;
        dataLength = (unsigned short)(*data);
        data++;
        dataLength |= ((unsigned short)(*data)) << 8;
        data++;
        totalLength -= 2;
    }
    else
    {
        dataLength = (unsigned short)(*data);
        data++;
        totalLength--;
    }

    if(totalLength < dataLength)
    {
        return -1; /* Message Error */
    }

    ucDevNo=0;
    if(id!=BT_HF_EVT_UPDATE_STATUS)
    {
        ucDevNo=*data;	/* device no */
        data+=4;
        //qDebug() << "ucDevNo" << ucDevNo;
    }

#ifdef BT_AUDIO_SINK
    /* for mode change between BT AUDIO */
    if(G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_A2DP_SINK)
    {
        if((id == BT_HF_EVT_INCOMING_IND)
            || (id == BT_HF_EVT_CIEV_CALL_ONE_IND)
            || (id == BT_HF_EVT_OUTGOING_START_IND)
            || (id == BT_HF_EVT_VOICE_RECOGNITION_START_IND)
            || (id == BT_HF_EVT_VOICE_RECOGNITION_LEARN_START_IND))
        {

        }
    }
#endif
    if ((id != 58) && (id !=1)&& (id !=47))
    {
        printf("=======================1==come to here %s   %d   id =%d   ==================== \n",__FUNCTION__,__LINE__,id);
    }
    switch (id)
    {
        case BT_HF_EVT_UPDATE_STATUS :

            break;
        case BT_HF_EVT_POWEROFF :

            break;
        case BT_HF_EVT_NOT_ACTIVATED :

            break;
        case BT_HF_EVT_ALREADY_ACTIVATED :

            break;
        case BT_HF_EVT_ACTIVATE_CFM :

            break;
        case BT_HF_EVT_ACTIVATE_ERR :

            break;
        case BT_HF_EVT_DEACTIVATE_CFM :


            break;
        case BT_HF_EVT_DEACTIVATE_ERR :

            break;
        case BT_HF_EVT_SERVICE_CONNECT_IND :
         break;
        case BT_HF_EVT_SERVICE_CONNECT_NOTIFICATION_IND:
          break;
        case BT_HF_EVT_SERVICE_CONNECT_ERR :
           break;
        case BT_HF_EVT_ALREADY_CONNECTED :
          break;
        case BT_HF_EVT_NOT_CONNECTED :
           break;
        case BT_HF_EVT_DISCONNECT_BY_REMOTE :
             break;
        case BT_HF_EVT_DISCONNECT_IND :

            break;
        case BT_HF_EVT_DISCONNECT_ERR :
         break;
        case BT_HF_EVT_DISCONNECT_LINK_LOSS :
            break;
        case BT_HF_EVT_AUDIO_IND :
                    if(m_state == IncomingCall)
                        //setState(ConnectCall);
            break;

        case BT_HF_EVT_AUDIO_ACCEPT_IND :
                G_BtHf->BtApp_HF_CmdCallAudioAccept(0, TRUE);
            break;

        case BT_HF_EVT_UPDATE_SUPPORTED_CODEC_CFM :
          break;

#ifdef BT_HFP_WIDEBAND_SPEECH
            /* Need to update icon here											*
             * 1. connect -> first incoming -> answer to update HD voice icon 	*
             * 2. Call -> disconnect -> connect								  	*/
    case BT_HF_EVT_SELECTED_CODEC_IND :

            break;
#endif

        case BT_HF_EVT_INCOMING_IND :
        {
                if (m_state != IncomingCall)
                {
                    emit btRequestBtPhoneAudioSource();
                    emit btStartIncomingCall();
                }
                setState(IncomingCall);

                INT8U boUnicodeOrNot;
                char strUniTemp[256];
                int uniStrlength = 0;
                INT8U aucCallNumber[BTAPP_HF_CALLNUM_MAX_LEN];
                boUnicodeOrNot = data[3];
                memcpy(&aucCallNumber[0],&data[4],BTAPP_HF_CALLNUM_MAX_LEN);
                if (boUnicodeOrNot!=0)
                {
                    memset(&strUniTemp, 0x00, 256);
                    uniStrlength = str_len16(aucCallNumber)*2;
                    strUniTemp[0] = 0x00;
                    strUniTemp[1] = 0x80;
                    memcpy(&(strUniTemp[2]), aucCallNumber, uniStrlength);
                    const QChar *data=(const QChar *)(aucCallNumber);
                    QString callName(data);

                    qDebug()<<callName.size();
                    if(callName.size() != 0)
                    {
                       emit   btincomingCall(callName);
                    }

                }
                else
                {
                    QString callNum((char *)aucCallNumber);
                    qDebug()<<callNum.size();
                    if (callNum.size() != 0)
                    {
                        emit   btincomingCall(callNum);
                    }
                }

            }
            break;

        case BT_HF_EVT_INCOMING_TIMEOUT_IND:
                G_BtHf->BtApp_HF_CmdIncomingEnd(0);
            break;
        case BT_HF_EVT_INCOMING_END_IND:
                emit btendingCall();
                emit btReleaseBtPhoneAudioSource();
                 G_BtHf->BtApp_HF_CmdIncomingEnd(0);
            break;

        case BT_HF_EVT_CIEV_CALL_ZERO_IND:
        unsigned char cmd[3];
        unsigned char pCmd[4];
        cmd[0] = 2;
        cmd[1] = 0 & 0xFF ;		/* data 1 */
        cmd[2] = loc;

           G_BtHf->BtApp_HF_CmdPwrOff(cmd[1]);
           pCmd[0] = 1;
           pCmd[1] = 0 & 0xFF ;
           G_BtHf->BtApp_HF_CmdCallEnd(pCmd[1]);

           //G_BtHf->BtApp_HF_CmdCloseAudio();
           outgoingAudioOpened = false;
            emit btendingCall();
           emit btReleaseBtPhoneAudioSource();
           setState(Unknow);
            break;
        case BT_HF_EVT_CIEV_CALL_ONE_IND:
        {
            emit btconnectCall();
             if(m_state == IncomingCall)
            {
                  emit btAnswerCall();
            G_BtHf->BtApp_HF_CmdCallDTMF(0, 0);
             G_BtHf->BtApp_HF_CmdCallStart(0);
            }
             callTime = 0;
             emit  btchangeCallTime("");
        }
            break;
        case BT_HF_EVT_CIEV_CALLSETUP_IND:
        {
         if(m_state == ConnectCall)
         {

         }
        if(m_state == IncomingCall)
        {
             //emit btendingCall();
            return 1;
        }
            INT8U boUnicodeOrNot;
            char strUniTemp[256];
            int uniStrlength = 0;
            INT8U aucCallNumber[BTAPP_HF_CALLNUM_MAX_LEN];
            boUnicodeOrNot = data[9];
            memcpy(&aucCallNumber[0],&data[10],BTAPP_HF_CALLNUM_MAX_LEN);
            if (boUnicodeOrNot!=0)
            {
                memset(&strUniTemp, 0x00, 256);
                uniStrlength = str_len16(aucCallNumber)*2;
                strUniTemp[0] = 0x00;
                strUniTemp[1] = 0x80;
                memcpy(&(strUniTemp[2]), aucCallNumber, uniStrlength);
                const QChar *data=(const QChar *)(aucCallNumber);
                QString callName(data);

          //  qDebug()<<callName.size();
            if((callName.size() != 0) )
                {
                   emit btoutgoingCall(callName);
                }

            }
            else
            {

                QString callNum((char *)aucCallNumber);
            qDebug()<<callNum.size();
            if ((callNum.size() != 0))
                {
                    emit  btoutgoingCall(callNum);
                }
            }
        }
            break;
        case BT_HF_EVT_CIEV_CALLHELD_IND:

            break;
        case BT_HF_EVT_CIEV_SERVICE_IND :

            break;
        case BT_HF_EVT_CIEV_ROAM_IND :

            break;
        case BT_HF_EVT_CIEV_SIGNAL_IND:

            break;
        case BT_HF_EVT_CIEV_BATTCHG_IND:

            break;
        case BT_HF_EVT_CIEV_MESSAGE_IND:

            break;
        case BT_HF_EVT_ANSWER_REQ:

            break;
        case BT_HF_EVT_REJECT_REQ:

            break;
        case BT_HF_EVT_REJECT_ERR:

            break;
        case BT_HF_EVT_TERMINATE_REQ:

            break;
        case BT_HF_EVT_PLACE_REQ:

            break;
        case BT_HF_EVT_CALL_ALREADY_EXIST:


            break;
        case BT_HF_EVT_CLIP_IND:
        {

        }
            break;
        case BT_HF_EVT_OUTGOING_NUMBER_IND:
        {
    }
            break;
        case BT_HF_EVT_OUTGOING_FROM_HFG_IND:
                setState(OutCall);
                emit btRequestBtPhoneAudioSource();

            break;
        case BT_HF_EVT_OUTGOING_START_IND:
        {
            emit btStartOutgoingCall();
        }
            break;
        case BT_HF_EVT_OUTGOING_END_IND:
            break;
        case BT_HF_EVT_OUTGOING_RETRY :
           break;
        case BT_HF_EVT_OUTGOING_DIAL_INF :
            break;
        case BT_HF_EVT_CALL_TIMESTAMP_COUNT_ID:
    {
        static unsigned char 	cCallTime[12];
        INT32U uiCallSec= GetData4Bytes(data);
        if((uiCallSec != 0) && ((uiCallSec%300) == 0))////FOR TIME CHANGE
        {
            //display hour
            cCallTime[0] = ((unsigned char)((callTime/3600)/10))+0x30;
            cCallTime[1] = ((unsigned char)((callTime/3600)%10))+0x30;
            cCallTime[2] = ':';
            //display minute
            cCallTime[3] = ((unsigned char)(((callTime%3600)/60)/10))+0x30;
            cCallTime[4] = ((unsigned char)(((callTime%3600)/60)%10))+0x30;
            cCallTime[5] = ':';
            //display sec
            cCallTime[6] = ((unsigned char)((callTime%60)/10))+0x30;
            cCallTime[7] = ((unsigned char)((callTime%60)%10))+0x30;
            cCallTime[8] = 0;
            emit btchangeCallTime(QString((char *)cCallTime));
            callTime ++;
        }
        else
        {
             //emit btchangeCallTime("");
        }
    }
                 break;
        case BT_HF_EVT_CALL_TIMESTAMP_CLEAR_ID:
                //emit btendingCall();
                    callTime = 0;
                    emit  btchangeCallTime("");
                       break;
        case BT_HF_EVT_SET_SPEAKER_VOLUME_IND:
        {
         G_BtHf->BtApp_HF_CmdSetVol(0);
        }

            break;
        case BT_HF_EVT_SET_MIC_GAIN_IND:

            break;
#ifdef BT_HFP_LOOPBACK_TEST
        case BT_HF_EVT_LOOPBACK_TEST_START_IND:

            break;
        case BT_HF_EVT_LOOPBACK_TEST_END_IND:

            break;
#endif
        case BT_HF_EVT_SUPPORT_VOICE_RECOGNITION_IND :

            break;
        case BT_HF_EVT_VOICE_RECOGNITION_START_IND:
        {
                G_BtHf->BtApp_HF_CmdVoiceRecognitionStart(0);
                G_BtHf->BtApp_HF_CmdCallDTMF(0,0);
               G_BtHf->BtApp_HF_CmdCallStart(0);
        }
            break;
        case BT_HF_EVT_VOICE_RECOGNITION_END_IND:
        {
            G_BtHf->BtApp_HF_CmdVoiceRecognitionLearnEnd(0);
        }
              break;
        case BT_HF_EVT_VOICE_RECOGNITION_LEARN_START_IND:
             break;
        case BT_HF_EVT_VOICE_RECOGNITION_LEARN_END_IND:
            break;
#if 1 /* Telechips' 2008.12 QA - 3.6 (a), (b) */
        case BT_HF_EVT_CLCC_IND:
    {

    }
              break;
#endif
        case BT_HF_EVT_CCWA_IND:

            break;
        case BT_HF_EVT_CMEE_IND:
              break;
        case BT_HF_EVT_OUTGOING_BUSY_LINE_IND:
             break;

        case BT_HF_EVT_GET_INFORMATION_INF:		/* Get remote device information */
             break;

        case BT_HS_EVT_SERVICE_CONNECT_IND:
           break;
        case BT_HS_EVT_SERVICE_CONNECT_ERR:
            break;
        case BT_HS_EVT_ALREADY_CONNECTED:
            break;
        case BT_HS_EVT_NOT_CONNECTED:
           break;
        case BT_HS_EVT_DISCONNECT_BY_REMOTE:
            break;
        case BT_HS_EVT_DISCONNECT_IND:
              break;
        case BT_HS_EVT_DISCONNECT_ERR:
           break;
        case BT_HS_EVT_DISCONNECT_LINK_LOSS:
             break;

        case BT_HS_EVT_RING_IND:
              break;
        case BT_HS_EVT_RING_START_IND:
            break;
        case BT_HS_EVT_RING_TIMEOUT_IND:
              break;
        case BT_HS_EVT_RING_END_IND:
             break;

        case BT_HS_EVT_AUDIO_ON_IND:
           break;
        case BT_HS_EVT_AUDIO_OFF_IND:
             break;

        case BT_HS_EVT_SET_SPEAKER_VOLUME_IND:

            break;
        case BT_HS_EVT_SET_MIC_GAIN_IND:
            break;

        /* PC Tool Use Home screen */
        case BT_HF_EVT_OPTIONS_AUTOANSWER_INF:
                break;

    case BT_HF_EVT_OPTIONS_MICGAIN_INF:
          break;

    case BT_HF_EVT_OPTIONS_SMSALARM_INF:
          break;

    case BT_HF_EVT_OPTIONS_RING_INF:
               break;

    case BT_HF_EVT_OPTIONS_BUILTIN_INF:
            break;

    case BT_HF_EVT_OPTIONS_NREC_INF:
            break;

        case BT_HF_EVT_DTMF_INF:

           break;

        case BT_HF_EVT_SET_MIC_MUTE_INF:
          break;

        case BT_HF_EVT_OPTIONS_CLCC_INF:
            break;

        case BT_HF_EVT_OPTIONS_BIA_INF:
             break;

        default:
            break;
    }
}
#endif

void TcBtPhone::UIBTHFMsgProcessor(BT_HF_MSG_t *uiMessage)
{
    if (uiMessage != NULL)
    {
        if (uiMessage->type == MSG_EVENT_TYPE)
        {
            UI_EVENT_TYPE event_type;
            event_type.group = 0;
            event_type.id = uiMessage->id;
            event_type.src = MSG_SRC_BT_HF;
            event_type.type = uiMessage->type;
            EventProcess((unsigned int)&event_type,uiMessage->totalLength, (const char *)uiMessage->data);
        }
        else if (uiMessage->type == MSG_INFO_TYPE)
        {
            //objHF.BT_HF_InfoParser(uiMessage);
        }
        else
        {
            printf( "UIBTHFMsgProcessor : Undefined message type\n");
        }
    }
}
void TcBtPhone::OnHFHandler(int type, void *pTcBtEvt)
{
#ifdef BT_HANDSFREE_UNIT
    TcBtHFEvt *pEvent=(TcBtHFEvt *)pTcBtEvt;
    BT_HF_MSG_t uiMessage;
    const unsigned char *uiMsgBuf;

    //printf("OnHFHandler id = %d\n", type);

    uiMsgBuf = (const unsigned char *)pEvent->mAddr;
    uiMessage.type = (unsigned char)uiMsgBuf[0];
    uiMessage.tickCount = (unsigned int)uiMsgBuf[1] | (((unsigned int)uiMsgBuf[2] << 8) & 0xFF00) | (((unsigned int)uiMsgBuf[3] << 16) & 0xFF0000) | (((unsigned int)uiMsgBuf[4] << 24) & 0xFF000000);
    uiMessage.id = (unsigned short)uiMsgBuf[5] | (((unsigned short)uiMsgBuf[6] << 8) & 0xFF00);
    uiMessage.totalLength = (unsigned short)uiMsgBuf[7] | (((unsigned short)uiMsgBuf[8] << 8) & 0xFF00);
    uiMessage.data = &uiMsgBuf[9];

    UIBTHFMsgProcessor(&uiMessage);

    /* Free messages */
    if(pEvent != NULL)
    {
        TC_BT_Pfree(pEvent->mAddr);
        TC_BT_Pfree(pEvent);
    }
#endif
}
void TcBtPhone::OnPBHandler(int type, void *pTcBtEvt)
{
    printf("===========OnPBHandler===result = %d=============\n",type);
    switch (type)
    {
/*------ Connect ---------------------------------------------*/
        case BT_PB_EVT_CONNECT_CFM:
           G_BtPB->BtApp_PB_CmdPACPullPhonebook(BTAPP_PB_SELECT_PB);
            break;

        case BT_PB_EVT_CONNECT_ERR:
            break;

/*------ Disconnect ---------------------------------------------*/
        case BT_PB_EVT_DISCONNECT_IND:
        {
                emit btdeviceDisConnectSuccess();
        }
            break;

/*------ Download ---------------------------------------------*/
        case BT_PB_EVT_PULLPB_IND:
            break;

        case BT_PB_EVT_PULLPB_CFM:
        {
                //QString name((char*)deviceName);
                emit btConnected();

        }
            break;

        case BT_PB_EVT_PULLCH_IND:
            break;

        case BT_PB_EVT_PULLCH_CFM:
        {
                getContactCount = 0;
                G_BtPB->BtApp_PB_CmdGetCallhistoryListInfo(0);

        }
            break;

        case BT_PB_EVT_PULLPB_ERR:
            break;

        case BT_PB_EVT_PULLCH_ERR:
            break;

/*------ PBAP operations ---------------------------------------------*/
        case BT_PB_EVT_PAC_ABORT_CFM:
            break;

        case BT_PB_EVT_PAC_SELECTSORTING_CFM:
            break;

        case BT_PB_EVT_PAC_SELECTFEATURES_CFM:
            break;

        case BT_PB_EVT_PAC_SETFILTER_CFM:
            break;

        case BT_PB_EVT_PAC_SELECTSOURCE_CFM:
            break;

        case BT_PB_EVT_PAC_SETFOLDER_CFM:
            break;

        case BT_PB_EVT_PAC_SETFOLDER_ERR:
            break;

        case BT_PB_EVT_PAC_SETAUTHENTICATION_CFM:
            break;

        case BT_PB_EVT_PAC_PULLVCARDLIST_CFM:
            break;

        case BT_PB_EVT_PAC_PULLVCARDLIST_ERR:
            break;

        case BT_PB_EVT_PAC_PULLVCARDENTRY_CFM:
            break;

        case BT_PB_EVT_PAC_PULLVCARDENTRY_ERR:
            break;

        case BT_PB_EVT_PAC_PULLPB_SIZEONLY_CFM:
            break;

        case BT_PB_EVT_PAC_PULLPB_NODATA_CFM:
            break;

/*------ Browsing operations  ---------------------------------------------*/
        case BT_PB_EVT_GET_CONTACTS_LIST:
             {
                unsigned char* pTcBtEvt2 = NULL;
                pTcBtEvt2 = (unsigned char*)pTcBtEvt;
                unsigned char				searchKeyword[BT_PB_SEARCH_CHAR_SIZE];
                memset(searchKeyword, 0xFF, BT_PB_SEARCH_CHAR_SIZE - 1);
                memcpy(&getContactTotalCount, pTcBtEvt2, sizeof(unsigned int));

                getContactCount = 0;
                G_BtPB->BtApp_PB_CmdGetContactsListInfo(getContactCount,searchKeyword);

                }
            break;
        case BT_PB_EVT_GET_CONTACTS_LIST_INFO:
        {
                unsigned char					searchKeyword[BT_PB_SEARCH_CHAR_SIZE];
                unsigned char               telNumberCell[MaxCallNumberSize];
                BT_PB_OBJ_INFO *contactinfo = (BT_PB_OBJ_INFO *)TC_BT_Pmalloc(sizeof(BT_PB_OBJ_INFO));
                memset(searchKeyword, 0xFF, BT_PB_SEARCH_CHAR_SIZE-1);
                memcpy(contactinfo,pTcBtEvt,sizeof(BT_PB_OBJ_INFO));
#if 0
                fprintf(stderr, "===========1===============name %s=====================\n",contactinfo->name);
                fprintf(stderr, "============2==============telNumberOther %s=====================\n",contactinfo->telNumberOther);

                fprintf(stderr, "============3==============telNumberHome %s=====================\n",contactinfo->telNumberHome);
                fprintf(stderr, "============4==============telNumberWork %s=====================\n",contactinfo->telNumberWork);
#endif
                const QChar *data =(const QChar *)(contactinfo->name);
                QString name(data);

                QString home_num((char*)contactinfo->telNumberHome);

                QString work_num((char*)contactinfo->telNumberWork);

                QString other_num((char*)contactinfo->telNumberOther);
                emit btgetContactsSuccess(name,home_num,work_num,other_num);

                TC_BT_Pfree(contactinfo);
                getContactCount ++;
                G_BtPB->BtApp_PB_CmdGetContactsListInfo(getContactCount,searchKeyword);
               # if 0
                if(getContactCount  <  9)
                {
                    getContactCount ++;
                    G_BtPB->BtApp_PB_CmdGetContactsListInfo(getContactCount,searchKeyword);
                }
                else
                {
                    getContactCount = 0;
                    emit btgetContactsEnd();
                }
#endif
        }
            break;

        case BT_PB_EVT_GET_CONTACTS_LIST_CLOSE:
            getContactCount = 0;
            emit btgetContactsEnd();
           G_BtPB->BtApp_PB_CmdPACPullCallHistory(BTAPP_PB_SELECT_CCH);
            break;

        case BT_PB_EVT_GET_CALLHISTORY_LIST:
            break;

        case BT_PB_EVT_GET_CALLHISTORY_LIST_INFO:
        {
                unsigned char					time[64];
                 int calltype = 0;
                BT_PB_OBJ_INFO *contactinfo = (BT_PB_OBJ_INFO *)TC_BT_Pmalloc(sizeof(BT_PB_OBJ_INFO));
                contactinfo = (BT_PB_OBJ_INFO *)pTcBtEvt;

                 calltype = contactinfo->callHistoryType;

                sprintf((char *)time,"%s-%s",contactinfo->time,contactinfo->date);
                const QChar *data =(const QChar *)(contactinfo->name);
                QString name(data);
                QString num((char*)contactinfo->telNumberCell);

                QString histime((char*)time);
               emit btgetHisCallSuccess(name,num,histime,calltype);

                getContactCount++;
                G_BtPB->BtApp_PB_CmdGetCallhistoryListInfo(getContactCount);
        }
            break;

        case BT_PB_EVT_GET_CALLHISTORY_LIST_CLOSE:
            getContactCount = 0;
            emit btgetHisCallEnd();
            break;

        case BT_PB_EVT_GET_VCARD_LIST:
            break;

        case BT_PB_EVT_GET_VCARD_LIST_CLOSE:
            break;

        case BT_PB_EVT_GET_CONTACTS_LISTUP:
            break;
        case BT_PB_EVT_GET_CONTACTS_LISTDOWN:
            break;
        case BT_PB_EVT_GET_CALLHISTORY_LISTUP:
            break;
        case BT_PB_EVT_GET_CALLHISTORY_LISTDOWN:
            break;

/*------ other ---------------------------------------------*/
        case BT_PB_EVT_SETAUTODOWNLOAD_CFM:
            break;

        case BT_PB_EVT_SETVCARDDUMP_CFM:
            break;

        case BT_PB_EVT_SET_STATUS_CFM:
            break;

        case BT_PB_EVT_GET_SEARCH_CONTACTS_LIST:
            break;

        default :
            break;
    }

}

void TcBtPhone::OnMAPHandler(int type, void *pTcBtEvt)
{

}



/*************** Audio source change function ***************/
static int  currentAudioSource  = AUDIO_SRC_NONE;
static void (*fUnsetCurrentAudioSource)(unsigned char source)  = NULL;
static void (*fSetCurrentAudioSource)(unsigned char source)    = NULL;
static int  previousAudioSource = AUDIO_SRC_NONE;
static void (*fUnsetPreviousAudioSource)(unsigned char source) = NULL;
static void (*fSetPreviousAudioSource)(unsigned char source)   = NULL;

int TcBtAPP_AudioSourceGet(void)
{
	return currentAudioSource;
}

void TcBtAPP_AudioSourceSet(unsigned char source, void (*funcOpen)(unsigned char), void (*funcClose)(unsigned char))
{
	if(currentAudioSource != source)
	{
		if((currentAudioSource != AUDIO_SRC_NONE) && (fUnsetCurrentAudioSource != NULL))
		{
			(fUnsetCurrentAudioSource)(source);
		}
		previousAudioSource			= currentAudioSource;
		fUnsetPreviousAudioSource	= fUnsetCurrentAudioSource;
		fSetPreviousAudioSource		= fSetCurrentAudioSource;
		currentAudioSource			= source;
		fUnsetCurrentAudioSource	= funcClose;
		fSetCurrentAudioSource		= funcOpen;
		if(funcOpen)
		{
			(funcOpen)(source);
		}
	}
}

void TcBtAPP_AudioSourceReset(void)
{
	if(currentAudioSource != AUDIO_SRC_NONE)
	{
		if(fUnsetCurrentAudioSource != NULL)
		{
			(fUnsetCurrentAudioSource)(AUDIO_SRC_NONE);
		}
		previousAudioSource			= AUDIO_SRC_NONE;
		fUnsetPreviousAudioSource	= NULL;
		fSetPreviousAudioSource		= NULL;
		currentAudioSource			= AUDIO_SRC_NONE;
		fUnsetCurrentAudioSource	= NULL;
		fSetCurrentAudioSource		= NULL;
	}
}

void TcBtAPP_AudioSourceBackToLast(void)
{
	if(currentAudioSource != previousAudioSource)
	{
		if ((currentAudioSource != AUDIO_SRC_NONE) && (fUnsetCurrentAudioSource != NULL))
		{
			(fUnsetCurrentAudioSource)(previousAudioSource);
		}
		currentAudioSource			= previousAudioSource;
		fSetCurrentAudioSource		= fSetPreviousAudioSource;
		fUnsetCurrentAudioSource	= fUnsetPreviousAudioSource;
		previousAudioSource			= AUDIO_SRC_NONE;
		fUnsetPreviousAudioSource	= NULL;
		fSetPreviousAudioSource		= NULL;
		if (fSetCurrentAudioSource)
		{
			(fSetCurrentAudioSource)(currentAudioSource);
		}
	}
}

