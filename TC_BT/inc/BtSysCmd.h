#ifndef BTSYSCMD_H
#define BTSYSCMD_H

#include "TC_BT_SYS_CMD.h"

#define TC_BT_MAX_FRIENDLY_NAME_LEN    (50)
class BtSysCmd
{
public:
        BtSysCmd();
        ~BtSysCmd();
        void TC_BT_SYS_SetLocalDeviceName(void);
        void TC_BT_SYS_GetLocalDeviceName(char *deviceName);
        void TC_BT_SYS_SetLocalDeviceAddr(void);
        int TC_BT_SYS_GetBTStatus(void);
        void TC_BT_SYS_SetBTStatus(int Status);
        int TC_BT_SYS_Activate(void);
        int TC_BT_SYS_Deactivate(void);
        unsigned int TC_BT_SYS_GetTick(void);
private:
        typedef struct
        {
            unsigned long int  lap; /* Lower Address Part 00..23 */
            unsigned char   uap; /* upper Address Part 24..31 */
            unsigned short int nap; /* Non-significant    32..47 */
        } TcBtBdAddr;
        //char gBtLocalDeviceName[TC_BT_MAX_FRIENDLY_NAME_LEN+1];
      //  TcBtBdAddr gBtLocalDeviceAddr;
};
BtSysCmd *GetglobalBtSyscmd();
#define G_BtSyscmd 	(GetglobalBtSyscmd())
#endif // BTSYSCMD_H
