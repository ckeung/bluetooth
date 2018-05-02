#ifndef BTAVRCPEVENT_H
#define BTAVRCPEVENT_H
#include <QObject>
#include "inc/BtBase.h"
class BtAvRcpEvent: public QObject
{
        Q_OBJECT
public:
    BtAvRcpEvent();
    ~BtAvRcpEvent();

    int  BtApp_BtAvRcp_ReceiveEvent(TcBtEvt *event);
};
#endif // BTAVRCPEVENT_H
