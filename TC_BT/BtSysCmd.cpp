#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "BT_CFG.h"
#include "BtApp_Gap.h"
#include "inc/BtSysCmd.h"
#include "inc/BD_ADDR.h"
#include "inc/BtUtil.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "TC_BT/inc/BtPlatform.h"
#include "TC_BT/inc/TC_BT_SYS_CMD.h"
//     static   func   ///
 static      int      gBTStatus = TC_BT_SYS_Idle;
BtSysCmd::BtSysCmd()
{
}
BtSysCmd::~BtSysCmd()
{
}
static BtSysCmd  _btsyscmd;
BtSysCmd *GetglobalBtSyscmd()
{
    return &_btsyscmd;
}
void BtSysCmd::TC_BT_SYS_SetLocalDeviceName(void)
{
    char pLocaDeviceName[256];
#ifdef TCM3800
    TcBtBdAddr BtLocalDeviceAddr;
    int fr = 0;
    fr = BTAPP_GetBDAddr(&BtLocalDeviceAddr.lap, &BtLocalDeviceAddr.uap, &BtLocalDeviceAddr.nap);
    if(fr == 0)
    {
        (void)sprintf(gBtLocalDeviceName, "BR_LINUX_%x", BtLocalDeviceAddr.lap);
        printf("TC_BT_SYS_SetLocalDeviceName : BTAPP_GetBDAddr call is success : %s \n", gBtLocalDeviceName);
    }
    else
#endif
    {
        printf("TC_BT_SYS_SetLocalDeviceName : BTAPP_GetBDAddr call is failed!!!! \n");
        (void)sprintf(pLocaDeviceName, "BR_LINUX_%s", TELECHIPS_FRIENDLY_NAME);

        if(strlen(pLocaDeviceName) <= TC_BT_MAX_FRIENDLY_NAME_LEN)
        {
            (void)strcpy(gBtLocalDeviceName, pLocaDeviceName);
        }
        else
        {
            (void)strncpy(gBtLocalDeviceName, pLocaDeviceName, TC_BT_MAX_FRIENDLY_NAME_LEN);
        }
    }
}
void BtSysCmd::TC_BT_SYS_GetLocalDeviceName(char *deviceName)
{
     strcpy(deviceName,gBtLocalDeviceName );
}
void BtSysCmd::TC_BT_SYS_SetLocalDeviceAddr(void)
{
#ifdef TCM3800
    int fr = 0;
    fr = BTAPP_GetBDAddr(&gBtLocalDeviceAddr.lap, &gBtLocalDeviceAddr.uap, &gBtLocalDeviceAddr.nap);
    if(fr != 0)
    {
        gBtLocalDeviceAddr.nap = TELECHIPS_NAP;
        gBtLocalDeviceAddr.uap = TELECHIPS_UAP;
        gBtLocalDeviceAddr.lap = TELECHIPS_LAP;
    }
#else
    gBtLocalDeviceAddr.nap = TELECHIPS_NAP;
    gBtLocalDeviceAddr.uap = TELECHIPS_UAP;
    gBtLocalDeviceAddr.lap = TELECHIPS_LAP;
#endif
}

int BtSysCmd::TC_BT_SYS_GetBTStatus(void)
{
    return gBTStatus;
}

void BtSysCmd::TC_BT_SYS_SetBTStatus(int Status)
{
    gBTStatus = Status;
}

int BtSysCmd::TC_BT_SYS_Activate(void)
{
    int	status=0;
    TcBtEvt	*pTcBtSysEvt = NULL;
    if(TC_BT_SYS_GetBTStatus() != TC_BT_SYS_Idle)
    {
        return  -1;
    }
    TC_BT_SYS_SetBTStatus(TC_BT_SYS_Activating);
#ifdef TC_BT_CHK_MEM_LEAK
    TC_BT_ChkML_Start();
#endif
    TC_BT_SC_DB_InitScDb();
    //usleep(2000); //Wait for thread create

    status = fork();
    if(status == 0)
    {
        execlp("bsa_server",  "bsa_server","-d", "/dev/ttyS1"  ,"-p", "/lib/modules/bt/patchram/TCM3800.hcd","-u", TC_BT_SOCKET_PATH,0);
    }
    else if (status > 0)
    {
        usleep(10);
        pTcBtSysEvt = (TcBtEvt *)TC_BT_Pmalloc(sizeof(TcBtEvt));
        if(pTcBtSysEvt)
        {
            pTcBtSysEvt->type = TC_BT_SYS_BSA_ACTIVATE_SUCCESS_CFM;

            /* TO DO: Send Msg */
            TC_BT_SendEvent(pTcBtSysEvt);
        }
    }
    return 0;
}

int BtSysCmd::TC_BT_SYS_Deactivate(void)
{
    TC_BT_Status eBTStatus = (TC_BT_Status)TC_BT_SYS_GetBTStatus();
    int fr = 0;

    if(eBTStatus != TC_BT_SYS_Activated)
    {
        return -1;
    }

    /* Need to kill server and close bsa client */
    TC_BT_SYS_SetBTStatus(TC_BT_SYS_Deactivating);
    fr = BsaAppExit();
    //TC_BT_SC_DB_DeInitScDb();
    if(fr != 0)
    {

    }
#ifdef BT_AUTO_CONNECTION
    if(BtApp_GAP_GetRecoveryLinkLostFlag() == BTAPP_GAP_RECOVERY_LINK_LOST_IN)
    {
        BtApp_GAP_SetRecoveryLinkLostFlag(BTAPP_GAP_RECOVERY_LINK_LOST_OUT);
        BtApp_GAP_RecoveryLinkLostCancelTimer();
    }
#endif

    return 0;
}

unsigned int BtSysCmd::TC_BT_SYS_GetTick(void)
{
    unsigned int tick_5ms;
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    tick_5ms = (unsigned int)(ts.tv_sec*1000 + ts.tv_nsec/1000000)/5;	/* tick = 5msec */

    return tick_5ms;
}
 int TC_BT_SYS_GetBTStatus(void)
 {
     return gBTStatus;
 }
