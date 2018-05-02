#ifndef BTPACCMD_H
#define BTPACCMD_H
#include "inc/BtDataType.h"
#include "BtUtil.h"
#include "TC_BT_PAC_CMD.h"

class BtPacCmd
{
public:
    BtPacCmd();
    ~BtPacCmd();
    int TC_BT_PAC_ConnectReq(const TcBtBdAddr *BdAddr, TcBtUint16 pacMaxObexPacketSize);
    int TC_BT_PAC_ActivateReq(void);
    int TC_BT_PAC_DeactivateReq(void);
    int TC_BT_PAC_DisconnectReq(void);
    int TC_BT_PAC_CancelConnectReq(void);
    int TC_BT_PAC_PullPhonePB(TcBtUint32 objNum, TcBtUint32 source);
    int TC_BT_PAC_PullPbResponse(void);
    int TC_BT_PAC_PullDialledCall(TcBtUint32 objNum, TcBtUint32 source);
    int TC_BT_PAC_PullReceivedCall(TcBtUint32 objNum, TcBtUint32 source);
    int TC_BT_PAC_PullMissedCall(TcBtUint32 objNum, TcBtUint32 source);
    int TC_BT_PAC_PullCombinedCall(TcBtUint32 objNum, TcBtUint32 source);
    int TC_BT_PAC_PullVcardList(const TcBtString* searchVal, TcBtUint16 select_pbList, TcBtUint16 listStartOffset, TcBtUint16 maxListCnt);
    int TC_BT_PAC_PullVcardListResponse(void);
    int TC_BT_PAC_PullVcardEntry(const TcBtString* handle);
    int TC_BT_PAC_PullVcardEntryResponse(void);
    int TC_BT_PAC_SetFolder(TcBtUint16 set_curfolder);
    int TC_BT_PAC_SetVcardFilter(TcBtUint32 filtertype);
    int TC_BT_PAC_AbortReq(void);
    int TC_BT_PAC_SetAuthentication(TcBtUint8 value);
};
BtPacCmd *GetglobalBtPacCmd();
#define G_BtPacCmd 	(GetglobalBtPacCmd())
#endif // BTPACCMD_H
