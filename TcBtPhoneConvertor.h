#ifndef TCBTPHONECONVERTOR_H
#define TCBTPHONECONVERTOR_H

#include <QObject>
class TcBtPhoneAdaptor;
class TcBtPhoneConvertor : public QObject
{
    Q_OBJECT
public:

  static  TcBtPhoneConvertor* getInstance();
signals:
    void connectCall();
    void answerCall();
    void connectEd();
    void deviceCompare(const QString &deviceName, const QString &passCode, const QString &deviceAddr);
    void deviceConnectFaile();
    void deviceConnectSuccess(const QString &deviceName,const QString &deviceAddr);
    void deviceDeleteSuccess();
    void deviceDisConnectSuccess();
    void deviceSearchSuccess(const QString &deviceName);
    void deviceSearchClosed();
    void endingCall();
    void getContactsFaile();
    void getContactsEnd();
    void getContactsSuccess(const QString &contactName, const QString &homeNum, const QString &workNum, const QString &otherNum);
    void getHisCallFaile();
    void getHisCallEnd();
     void getHisCallSuccess(const QString &hisCallname, const QString &hisCallnum, const QString &hisCalltime, int hisCalltype);
    void incomingCall(const QString &comingValue);
    void outgoingCall(const QString &outgoingValue);
    void changeCallTime(const QString &timeValue);
    void initBtappFaile();
    void deInitBtappSuccess();
    void initBtappSuccess(const QString &deviceName, const QString &deviceAddr);
    void requestBtPhoneAudioSource();
    void startOutgoingCall();
    void startIncomingCall();
public slots:
    void cmdChangeVoiceMode();
    void cmdSetVoiceMute();
    void cmdDialByName(const QString &contactName, int numSize);
    void cmdDialByNum(const QString &contactPhNum, int numSize);
    void cmdEndCal();
    void cmdHangUp();
    void cmdIncomingAnswer();
    void cmdIncomingReject();
    void cmdNumBtnClicked(int Num);
    void cmdReDial();
    void cmdVoiceRecognition();
    void deviceAllDelete();
    void deviceCompareornot(bool bAccept);
    void deviceConnect(int DeviceNum);
    void deviceDelete(int DeviceNum);
    void deviceDisconnect(int DeviceNum);
    void deviceSearchCancel();
    void deviceSearchStart(int totalSearchTime, int maxDevices);
    void getContactInfo();
    void getHistoryCalInfo();
    void sysActivate();
    void sysDeactivate();
    void cmdOpenPhoneAudio();
    void cmdStartOutgoingCall();
    void cmdStartIncomingCall();
private:
    TcBtPhoneConvertor();
    ~TcBtPhoneConvertor() override;
    TcBtPhoneConvertor(const TcBtPhoneConvertor&) = delete;
    TcBtPhoneConvertor(TcBtPhoneConvertor&&) = delete;
    TcBtPhoneConvertor& operator= (const TcBtPhoneConvertor&) = delete;
    TcBtPhoneConvertor& operator= (TcBtPhoneConvertor&&) = delete;
    TcBtPhoneAdaptor* m_adaptor;
    static TcBtPhoneConvertor* m_instance;
};

#endif // TCBTPHONECONVERTOR_H
