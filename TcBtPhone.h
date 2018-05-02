/****************************************************************************
 *   FileName    : TcBtApp.h
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

#ifndef TCBTAPP_H
#define TCBTAPP_H

#include "BT_CFG.h"
#include "BtHandFree.h"
#include "BtDeviceManage.h"
#include "BtSys.h"
#include "BtThread.h"
#include "BtPhoneBookCmd.h"
#include "BtPhoneBook.h"
#include "BtA2dpEvent.h"
#include "BtAvRcpEvent.h"
#include "BtHfEvent.h"
#include <QWidget>


#if defined(__arm__) && !defined(BT_NO_LACUCHER)
extern int g_debug;
extern int g_appID;
#endif

typedef enum
{
	AUDIO_SRC_NONE = 0,
	AUDIO_SRC_BTAV,
	AUDIO_SRC_BTHF,
	
	AUDIO_SRC_MAX
} AUDIO_SOURCE;

namespace Ui {
class TcBtPhone;
}
class TcBtAppDBSignal : public QObject
{
    Q_OBJECT

public:
    TcBtAppDBSignal() {}
    ~TcBtAppDBSignal() {}

signals:
    void SYS_Event(int result);
    void HF_Event(int type, void *pTcBtEvt);
    void DM_Event(int type,void *pTcBtEvt);
    void PB_Event(int type, void *pTcBtEvt);
    void MAP_Event(int type, void *pTcBtEvt);
#if defined(__arm__) && !defined(BT_NO_LACUCHER)
    void LauncherShowApplication(int app);
    void LauncherHideApplications(int app);
#endif
};
class TcBtPhone : public  QObject
{
    Q_OBJECT

public:
    explicit TcBtPhone(QObject *parent = 0);
    ~TcBtPhone();
#if defined(g_TcBtPhone)
#undef g_TcBtPhone
#endif
#define g_TcBtPhone (TcBtPhone::getInstance())
    static TcBtPhone* getInstance();
public:
    TcBtAppDBSignal *m_DBSignal;
    unsigned int getContactCount;
    unsigned int getContactTotalCount;
    TcBtString				*deviceName;
    bool callFromPhone;
    bool callIncoming;
    bool outgoingAudioOpened;
    INT32U callTime;
public:
    enum State { Unknow, IncomingCall, OutCall, ConnectCall };
public:
    int EventProcess(unsigned int event_type, int param1, const char* param2);
    void UIBTHFMsgProcessor(BT_HF_MSG_t *uiMessage);
    //TC_BT event handler
    bool  sysActivate(void);
    bool sysDeactivate(void);
    bool deviceSearchStart(int totalSearchTime, int maxDevices, short update);
    bool deviceSearchCancel(void);
    bool  deviceConnect(int  DeviceNum);
    bool deviceDisconnect(int  DeviceNum);
    bool deviceDelete(int  DeviceNum);
    bool deviceAllDelete(void);
    bool deviceCompare(bool bAccept);

    bool getContactInfo(void);
    bool getHistoryCalInfo(void);
    bool cmdReDial(void);
    bool cmdDialByNum( unsigned char  * contactPhNum,short numSize);
    bool cmdDialByName(const unsigned char  * contactName,short nameSize);
    bool cmdIncomingAnswer(void);
    bool cmdIncomingReject(void);
     bool cmdEndCal(void);
    bool cmdHangUp(void);
    bool cmdNumBtnClicked(int Num);
    bool cmdChangeVoiceMode(void);
    bool cmdSetVoiceMute(void);
    void cmdVoiceRecognition(void);
    void cmdOpenPhoneAudio();
    void cmdStartOutgoingCall();
    void cmdStartIncomingCall();
public:
    void setState(State state);

#if defined(__arm__) && !defined(BT_NO_LACUCHER)
	void InitializeDBus(void);
#endif
    void		*pMsgForClose;
private:
#ifdef __arm__
    QFont _font;
#endif
signals:
    void btconnectCall();
    void btAnswerCall();
    void btConnected();
    void btdeviceCompare(const QString &deviceName, const QString &passCode, const QString &deviceAddr);
    void btdeviceConnectFaile();
    void btdeviceConnectSuccess(const QString &deviceName, const QString &deviceAddr);
    void btdeviceDeleteSuccess();
    void btdeviceDisConnectSuccess();
    void btdeviceSearchSuccess(const QString &deviceName);
    void btdeviceSearchClosed();
    void btendingCall();
    void btgetContactsFaile();
    void btgetContactsEnd();
    void btgetContactsSuccess(const QString &contactName, const QString &homeNum,const QString &workNum,const QString &otherNum);
    void btgetHisCallFaile();
    void btgetHisCallEnd();
     void btgetHisCallSuccess(const QString &hisCallname, const QString &hisCallnum, const QString &hisCalltime, int hisCalltype);
    void btincomingCall(const QString &comingValue);
    void btoutgoingCall(const QString &outgoingValue);
    void btchangeCallTime(const QString &timeValue);
    void btinitBtappFaile();
    void btdeInitBtappSuccess();
    void btinitBtappSuccess(const QString &deviceName, const QString &deviceAddr);
    void btRequestBtPhoneAudioSource();
    void btReleaseBtPhoneAudioSource();
    void btStartIncomingCall();
    void btStartOutgoingCall();
public slots:
    void OnSYSHandler(int result);
    void OnDMHandler(int type, void *pTcBtEvt);
    void OnHFHandler(int type, void *pTcBtEvt);
    void OnPBHandler(int type, void *pTcBtEvt);
    void OnMAPHandler(int type, void *pTcBtEvt);
private:
    State m_state;
};

extern int TcBtAPP_AudioSourceGet(void);
extern void TcBtAPP_AudioSourceSet(unsigned char source, void (*funcOpen)(unsigned char), void (*funcClose)(unsigned char));
extern void TcBtAPP_AudioSourceReset(void);
extern void TcBtAPP_AudioSourceBackToLast(void);

#endif // TCBTAPP_H
