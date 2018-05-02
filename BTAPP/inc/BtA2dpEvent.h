#ifndef BTA2DPEVENT_H
#define BTA2DPEVENT_H
#include <QObject>
#include "inc/BtBase.h"
class BtA2dpEvent: public QObject
{
        Q_OBJECT
public:
    BtA2dpEvent();
    ~BtA2dpEvent();

    int BtApp_A2dp_ReceiveEvent(TcBtEvt *event);
};
#endif // BTA2DPEVENT_H
