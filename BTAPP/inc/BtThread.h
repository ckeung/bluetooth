#ifndef BTTHREAD_H
#define BTTHREAD_H

//#include <QObject>
#include <QThread>
#include "inc/BtBase.h"
#include "BtDeviceManage.h"
#include "BtSys.h"
#include "BtThread.h"
#include "BtPhoneBookCmd.h"
#include "BtA2dpEvent.h"
#include "BtAvRcpEvent.h"
#include "BtHfEvent.h"
class BtThread : public QThread
{
    Q_OBJECT
public:
            BtThread();
            ~BtThread();
            void run();
            void Initilaize(void);
            bool bStarted;
        BtA2dpEvent m_BtA2dpEvent;
        BtAvRcpEvent m_BtAvRcpEvent;
        BtHfEvent m_BtHfEvent;
signals:
            void send_msg_to_BtSys(TcBtEvt *event);
            void send_msg_to_BtHf(TcBtEvt *event);
            void send_msg_to_BtPb(TcBtEvt *event);
            void send_msg_to_BtSC(TcBtEvt *event);
            void send_msg_to_BtSD(TcBtEvt *event);
            void send_msg_to_BtCM(TcBtEvt *event);
            void send_msg_to_BtA2DP(TcBtEvt *event);
            void send_msg_to_BtAVRCP(TcBtEvt *event);
};
BtThread *GetglobalBtThread();
#define G_BtThread 	(GetglobalBtThread())
#endif // BTTHREAD_H
