#ifndef BTHFEVENT_H
#define BTHFEVENT_H
#include <QObject>
#include "inc/BtBase.h"
class BtHfEvent: public QObject
{
           Q_OBJECT
public:
    BtHfEvent();
    ~BtHfEvent();

    int BtApp_HF_ReceiveEvent(TcBtEvt *event);
};

#endif // BTHFEVENT_H
