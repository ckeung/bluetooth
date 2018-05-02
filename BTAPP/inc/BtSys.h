#ifndef BTSYS_H
#define BTSYS_H


#include <QObject>
#include "TC_BT/inc/BtUtil.h"
#include "inc/BtBase.h"
#include "TC_BT/inc/BtSysCmd.h"
#include <QMutex>

class  BtSys: public QObject
{
    Q_OBJECT
public:
        BtSys();
        ~BtSys();
        void BtApp_SYS_Init(void);
       void  BtApp_SYS_Deinit(void);
        bool BtApp_SYS_SetStatus(unsigned int uiStatus);
        char BtApp_SYS_GetStatus(unsigned int uiStatus);
        bool BtApp_SYS_ClearStatus(unsigned int uiStatus);
        unsigned int BtApp_SYS_GetTick(void);
        void BtApp_SYS_HW_PowerOnCommand(void);
        void BtApp_SYS_HW_PowerOffCommand(void);
       unsigned char BtApp_SYS_GetBasicMode(void);
       void BtApp_SYS_SetBasicMode(unsigned char ucMode);
        bool BtApp_SYS_Is_BusyStatusHFP(unsigned char ucDevNo);
        int BtApp_SYS_HWClearWarmResetInd(void *pTcBtEvt);
        int BtApp_SYS_HWSetWarmResetInd(void *pTcBtEvt);
        int BtApp_SYS_BSA_ActivateFailCfm(void *pTcBtEvt);
        int BtApp_SYS_BSA_ActivateSuccessCfm(void *pTcBtEvt);
        int BtApp_SYS_ActivateFailCfm(void *pTcBtEvt);
        int BtApp_SYS_ActivateSuccessCfm(void *pTcBtEvt);
        int BtApp_SYS_DeactivateCfm(void *pTcBtEvt);
        int BtApp_SYS_NotSupportCmdCfm(void *pTcBtEvt);
        int BtApp_SYS_DisconnectAll(void );
         unsigned short		gBtAppSysCurStatus[BTAPP_STATUS_GROUP_MAX];
      int BtApp_SYS_ReceiveEvent(TcBtEvt *event);

 private:

        unsigned char 		gBasicMode ;
        QMutex _mutex;
};
BtSys *GetglobalBtSys();
#define G_BtSys 	(GetglobalBtSys())
#endif // BTSYS_H
