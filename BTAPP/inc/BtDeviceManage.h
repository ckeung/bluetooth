#ifndef BTDEVICEMANAGE_H
#define BTDEVICEMANAGE_H
#include <QObject>
#include "TC_BT/inc/BtDmCmd.h"
#include "inc/BtBase.h"

#ifndef MAX_SEARCH_LIST
#define MAX_SEARCH_LIST 36
#endif

typedef struct {
    TcBtBdAddr      devAddr;
    unsigned int	deviceClass;
    char			rssi;
    QString         strDevName;
} stDevicList;

class BtDeviceManage: public QObject
{
   Q_OBJECT
public:
    BtDeviceManage();
    ~BtDeviceManage();
    void StartSearchDevice(unsigned long int  totalSearchTime, unsigned long int  maxDevices, unsigned char update);
    void CancelSearchDevice(void);
    void ConnectDevice(int nDeviceID);
    void DisConnectDevice(int nDeviceID);
    void DisConnectAll(void);
    void DeleteDevice(int nDeviceID);
    void DeleteAll(void);
    int TC_BT_SC_AuthoriseInd(void *pTcBtEvt);
    int TC_BT_SC_BondEvt(void *pTcBtEvt);
    void SC_SSP_NumericCompare(unsigned short int bAccept);
    int SC_LegacyPassKey(const unsigned char *pPassKey, unsigned char PassKeyLen);
    int SC_SSP_PasskeyNotification(unsigned short int bAccept);
    int SC_SSP_JustWork(unsigned short int bAccept);
    void Handler(int type, void *pTcBtEvt);
    int AddSearchList(TcBtBdAddr devAddr, unsigned int	deviceClass, char rssi, QString  strDevName);
     int ClearSearchList(void);
    void InitDeviceList(void);

      int BtApp_SD_ReceiveEvent(TcBtEvt *event);
      int BtApp_SC_ReceiveEvent(TcBtEvt *event);
      int BtApp_CM_ReceiveEvent(TcBtEvt *event);
public:
    BtDmCmd  m_BtSdCmd;
 public:
    stDevicList objDevList[MAX_SEARCH_LIST];
    int         DevListIdx;
};
BtDeviceManage *GetglobalBtDeviceManage();
#define G_BtDm 	(GetglobalBtDeviceManage())
#endif // BTDEVICEMANAGE_H

