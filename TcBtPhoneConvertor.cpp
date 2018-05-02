#include "TcBtPhoneConvertor.h"
#include "tcbtphone_adaptor.h"
#include "TcBtPhone.h"
#define TC_BT_SEARCH_UPDATE_NAME    2   /* Send update when Name is available */
TcBtPhoneConvertor *TcBtPhoneConvertor::m_instance = nullptr;

TcBtPhoneConvertor* TcBtPhoneConvertor::getInstance()
{
    if (!m_instance)
    {
        m_instance = new TcBtPhoneConvertor;
    }
      return m_instance;
}

TcBtPhoneConvertor::TcBtPhoneConvertor() :
    m_adaptor(new TcBtPhoneAdaptor(this))
{
    setProperty("interfaceClassName", "TcBtPhone");
    connect(g_TcBtPhone, &TcBtPhone::btconnectCall, [this]() {
        emit connectCall();
    });
    connect(g_TcBtPhone, &TcBtPhone::btAnswerCall, [this]() {
        emit answerCall();
    });

    connect(g_TcBtPhone, SIGNAL(btdeviceCompare(QString,QString,QString)), SIGNAL(deviceCompare(QString,QString,QString)));

    connect(g_TcBtPhone, &TcBtPhone::btdeviceConnectFaile, [this]() {
        emit deviceConnectFaile();
    });
    connect(g_TcBtPhone, &TcBtPhone::btConnected, [this]() {
        emit connectEd();
    });
    connect(g_TcBtPhone, &TcBtPhone::btRequestBtPhoneAudioSource, [this]() {
       //emit requestBtPhoneAudioSource();
    });
    connect(g_TcBtPhone, &TcBtPhone::btStartOutgoingCall, [this]() {
        emit startOutgoingCall();
    });
    connect(g_TcBtPhone, &TcBtPhone::btStartIncomingCall, [this]() {
        emit startIncomingCall();
    });

    connect(g_TcBtPhone, &TcBtPhone::btdeviceDeleteSuccess, [this]() {
        emit deviceDeleteSuccess();
    });
    connect(g_TcBtPhone, &TcBtPhone::btdeviceDisConnectSuccess, [this]() {
        emit deviceDisConnectSuccess();
    });
    connect(g_TcBtPhone, SIGNAL(btdeviceConnectSuccess(QString,QString)), SIGNAL(deviceConnectSuccess(QString,QString)));
    connect(g_TcBtPhone, SIGNAL(btdeviceSearchSuccess(QString)), SIGNAL(deviceSearchSuccess(QString)));

    connect(g_TcBtPhone, &TcBtPhone::btdeviceSearchClosed, [this]() {
        emit deviceSearchClosed();
    });
    connect(g_TcBtPhone, &TcBtPhone::btgetContactsFaile, [this]() {
        emit getContactsFaile();
    });
    connect(g_TcBtPhone, &TcBtPhone::btgetContactsEnd, [this]() {
        emit getContactsEnd();
    });
    connect(g_TcBtPhone, &TcBtPhone::btendingCall, [this]() {
        emit endingCall();
    });

    connect(g_TcBtPhone, SIGNAL(btgetContactsSuccess(QString,QString,QString,QString)), SIGNAL(getContactsSuccess(QString,QString,QString,QString)));

    connect(g_TcBtPhone, SIGNAL(btincomingCall(QString)), SIGNAL(incomingCall(QString)));
    connect(g_TcBtPhone, SIGNAL(btoutgoingCall(QString)), SIGNAL(outgoingCall(QString)));
    connect(g_TcBtPhone, SIGNAL(btchangeCallTime(QString)), SIGNAL(changeCallTime(QString)));

    connect(g_TcBtPhone, SIGNAL(btgetHisCallSuccess(QString,QString,QString,int)), SIGNAL(getHisCallSuccess(QString,QString,QString,int)));
    connect(g_TcBtPhone, SIGNAL(btinitBtappSuccess(QString,QString)), SIGNAL(initBtappSuccess(QString,QString)));

    connect(g_TcBtPhone, &TcBtPhone::btgetHisCallFaile, [this]() {
        emit getHisCallFaile();
    });
    connect(g_TcBtPhone, &TcBtPhone::btgetHisCallEnd, [this]() {
        emit getHisCallEnd();
    });
    connect(g_TcBtPhone, &TcBtPhone::btinitBtappFaile, [this]() {
        emit initBtappFaile();
    });
    connect(g_TcBtPhone, &TcBtPhone::btdeInitBtappSuccess, [this]() {
        emit deInitBtappSuccess();
    });

}

TcBtPhoneConvertor::~TcBtPhoneConvertor()
{
    m_instance = nullptr;
}
void TcBtPhoneConvertor::cmdChangeVoiceMode()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdChangeVoiceMode();
}
void TcBtPhoneConvertor::cmdSetVoiceMute()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdSetVoiceMute();
}

void TcBtPhoneConvertor::cmdDialByName(const QString &contactName, int numSize)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    const char *name = contactName.toStdString().data();
    g_TcBtPhone->cmdDialByName((unsigned char *)name,numSize);
}
void TcBtPhoneConvertor::cmdDialByNum(const QString &contactPhNum, int numSize)
{
    fprintf(stderr,"enter fun %s   line %d \n",__FUNCTION__,__LINE__);

   const char *num = contactPhNum.toStdString().data();
   fprintf(stderr,"num %s   line %d \n",num,__LINE__);
   fprintf(stderr,"numSize %d   line %d \n",numSize,__LINE__);
    g_TcBtPhone->cmdDialByNum((unsigned char *)num,(short)numSize);
}
void TcBtPhoneConvertor::cmdEndCal()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdEndCal();
}

void TcBtPhoneConvertor::cmdHangUp()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdHangUp();
}
void TcBtPhoneConvertor::cmdIncomingAnswer()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdIncomingAnswer();
}
void TcBtPhoneConvertor::cmdIncomingReject()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdIncomingReject();
}
void TcBtPhoneConvertor::cmdNumBtnClicked(int Num)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdNumBtnClicked(Num);
}
void TcBtPhoneConvertor::cmdReDial()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdReDial();
}
void TcBtPhoneConvertor::cmdVoiceRecognition()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdVoiceRecognition();
}
void TcBtPhoneConvertor::deviceAllDelete()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceAllDelete();
}
void TcBtPhoneConvertor::deviceCompareornot(bool bAccept)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceCompare(bAccept);
}
void TcBtPhoneConvertor::deviceConnect(int DeviceNum)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceConnect(DeviceNum);
}
void TcBtPhoneConvertor::deviceDelete(int DeviceNum)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceDelete(DeviceNum);
}
void TcBtPhoneConvertor::deviceDisconnect(int DeviceNum)
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceDisconnect(DeviceNum);
}
void TcBtPhoneConvertor::deviceSearchCancel()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceSearchCancel();
}
void TcBtPhoneConvertor::deviceSearchStart(int totalSearchTime, int maxDevices)
{
    fprintf(stderr,"enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->deviceSearchStart(totalSearchTime,maxDevices,TC_BT_SEARCH_UPDATE_NAME);
}
void TcBtPhoneConvertor::getContactInfo()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->getContactInfo();
}
void TcBtPhoneConvertor::getHistoryCalInfo()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->getHistoryCalInfo();
}
void TcBtPhoneConvertor::sysActivate()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->sysActivate();
}
void TcBtPhoneConvertor::sysDeactivate()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->sysDeactivate();
}
void TcBtPhoneConvertor::cmdOpenPhoneAudio()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdOpenPhoneAudio();
}
void TcBtPhoneConvertor::cmdStartOutgoingCall()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdStartOutgoingCall();
}
void TcBtPhoneConvertor::cmdStartIncomingCall()
{
    printf("enter fun %s   line %d \n",__FUNCTION__,__LINE__);
    g_TcBtPhone->cmdStartIncomingCall();
}
