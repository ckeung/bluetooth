#ifndef BTDMCMD_H
#define BTDMCMD_H

#include "inc/BtDataType.h"

class BtDmCmd
{
public:
        BtDmCmd();
        ~BtDmCmd();
         int TC_BT_SD_Search(TcBtUint32 totalSearchTime, TcBtUint32 maxDevices, TcBtUint8 update);
         int TC_BT_SD_CancelSearch(void);
         int TC_BT_SC_Debond(TcBtBdAddr bdAddr);
         int TC_BT_SC_LegacyPassKey(const TcBtUint8 *pPassKey, TcBtUint8 PassKeyLen);
         int TC_BT_SC_Authorise(TcBtBdAddr bdAddr, TcBtUint16 bAuth);
         int TC_BT_SC_SSP_PasskeyNotification(TcBtUint16 bAccept);
         int TC_BT_SC_SSP_NumericCompare(TcBtUint16 bAccept);
         int TC_BT_SC_SSP_JustWork(TcBtUint16 bAccept);
         int TC_BT_SC_DebugModeReqSend(TcBtUint8 enable);
};
int TC_BT_SD_ReadAvailableServices(TcBtBdAddr deviceAddr);
int TC_BT_SD_CancelReadAvailableServices(void);

/**********************************  Device Identification  **********************************/
int TC_BT_SD_DiRegisterServiceRecord(TcBtUint16 	vendorId,
                                    TcBtUint16 		productId,
                                    TcBtUint16 		version,
                                    TcBtBool 		primaryRecord,
                                    TcBtUint16 		vendorIdSource,
                                    TcBtUint8 		*pServiceDescription,
                                    TcBtUint16 		serviceDescriptionLen,
                                    TcBtUint8 		*pClientExecutableUrl,
                                    TcBtUint16 		clientExecutableUrlLen,
                                    TcBtUint8 		*pDocumentationUrl,
                                    TcBtUint16 		documentationUrlLen);
int TC_BT_SD_DiReadServiceRecord(TcBtBdAddr deviceAddr);
int TC_BT_SD_DiCancelReadServiceRecord(void);

void TC_BT_SD_DiRegisterTest(void);
#endif // BTSDCMD_H
