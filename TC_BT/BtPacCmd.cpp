//*************************heard file***********************//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "BtPacCmd.h"
//*******************static param ************************//
#define TC_BT_MAX_NUM 500

//*******************static fun ***************************//
static int TC_BT_PAC_DefaultCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr, TcBtUint16 pacMaxObexPacketSize);

static BtPacCmd  _btpaccmd;
BtPacCmd *GetglobalBtPacCmd()
{
    return &_btpaccmd;
}
//********************* add fun *************************//
static int TC_BT_PAC_DefaultCommand(TcBtCmdType type, const TcBtBdAddr *pBdAddr, TcBtUint16 pacMaxObexPacketSize)
{
    TcBtPacCmd *pCmdStr;

    /* Check for BT ON ??? */

    /* FAIL: No BT ON -1 */

    pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = type;
        pCmdStr->pacMaxObexPacketSize = pacMaxObexPacketSize;
        TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)pBdAddr);

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}


BtPacCmd::BtPacCmd()
{
}

BtPacCmd::~BtPacCmd()
{

}
int BtPacCmd::TC_BT_PAC_ConnectReq(const TcBtBdAddr *BdAddr, TcBtUint16 pacMaxObexPacketSize)
{
    return TC_BT_PAC_DefaultCommand(TC_BT_PAC_CONNECT_REQ, BdAddr, pacMaxObexPacketSize);
}

int BtPacCmd::TC_BT_PAC_ActivateReq(void)
{
    TcBtPacActivateCmd *pCmdStr;

    pCmdStr = (TcBtPacActivateCmd *)TC_BT_Pmalloc(sizeof(TcBtPacActivateCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_ACTIVATE_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtPacCmd::TC_BT_PAC_DeactivateReq(void)
{
    TcBtPacDeactivateCmd *pCmdStr;

    pCmdStr = (TcBtPacDeactivateCmd *)TC_BT_Pmalloc(sizeof(TcBtPacDeactivateCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_DEACTIVATE_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtPacCmd::TC_BT_PAC_DisconnectReq(void)
{
    TcBtPacCmd *pCmdStr;

    pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_DISCONNECT_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

}
int BtPacCmd::TC_BT_PAC_CancelConnectReq(void)
{
    TcBtPacCmd *pCmdStr;

    pCmdStr = (TcBtPacCmd *)TC_BT_Pmalloc(sizeof(TcBtPacCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_CANCELCONNECT_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullPhonePB(TcBtUint32 objNum, TcBtUint32 source)
{
    TcBtPacPullPbReqCmd *pCmdStr;

    if(objNum > TC_BT_MAX_NUM)
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    if((source != TC_BT_PAC_SRC_PHONE) && (source != TC_BT_PAC_SRC_SIM))
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    pCmdStr = (TcBtPacPullPbReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLPB_REQ;
        pCmdStr->ObjNum = objNum;
        pCmdStr->src = source;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullPbResponse(void)
{
    TcBtPacPullPbResCmd *pCmdStr;

    pCmdStr = (TcBtPacPullPbResCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbResCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLPB_RES;
        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullDialledCall(TcBtUint32 objNum, TcBtUint32 source)
{
    TcBtPacPullPbReqCmd *pCmdStr;

    if(objNum > TC_BT_MAX_NUM)
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    if((source != TC_BT_PAC_SRC_PHONE) && (source != TC_BT_PAC_SRC_SIM))
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    pCmdStr = (TcBtPacPullPbReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLOCH_REQ;
        pCmdStr->ObjNum = objNum;
        pCmdStr->src = source;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullReceivedCall(TcBtUint32 objNum, TcBtUint32 source)
{
    TcBtPacPullPbReqCmd *pCmdStr;

    if(objNum > TC_BT_MAX_NUM)
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    if((source != TC_BT_PAC_SRC_PHONE) && (source != TC_BT_PAC_SRC_SIM))
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    pCmdStr = (TcBtPacPullPbReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLICH_REQ;
        pCmdStr->ObjNum = objNum;
        pCmdStr->src = source;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

         /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int BtPacCmd::TC_BT_PAC_PullMissedCall(TcBtUint32 objNum, TcBtUint32 source)
{
    TcBtPacPullPbReqCmd *pCmdStr;

    if(objNum > TC_BT_MAX_NUM)
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    if((source != TC_BT_PAC_SRC_PHONE) && (source != TC_BT_PAC_SRC_SIM))
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    pCmdStr = (TcBtPacPullPbReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLMCH_REQ;
        pCmdStr->ObjNum = objNum;
        pCmdStr->src = source;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullCombinedCall(TcBtUint32 objNum, TcBtUint32 source)
{
    TcBtPacPullPbReqCmd *pCmdStr;

    if(objNum > TC_BT_MAX_NUM)
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    if((source != TC_BT_PAC_SRC_PHONE) && (source != TC_BT_PAC_SRC_SIM))
    {
        return TC_BT_CMD_RESULT_NOT_SUPPORT_ENUM;
    }

    pCmdStr = (TcBtPacPullPbReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullPbReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULLCCH_REQ;
        pCmdStr->ObjNum = objNum;
        pCmdStr->src = source;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullVcardList(const TcBtString* searchVal, TcBtUint16 select_pbList, TcBtUint16 listStartOffset, TcBtUint16 maxListCnt)
{
    TcBtPacPullVcardListReqCmd *pCmdStr;

    //BtApp_PB_ClearPhonebookList();

    pCmdStr = (TcBtPacPullVcardListReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullVcardListReqCmd));

    if(pCmdStr != NULL)
    {
        int strLen = 0;
        pCmdStr->type = TC_BT_PAC_PULL_VCARDLIST_REQ;
        pCmdStr->select_pbList = select_pbList;
        pCmdStr->order = 0x01; /* alphabetical */
        strLen = strlen((char *)searchVal);
        pCmdStr->searchVal = (TcBtUint8 *)TC_BT_Pmalloc(strLen+1);
        memcpy(pCmdStr->searchVal, (TcBtUint8 *)searchVal, strLen);
        pCmdStr->searchVal[strLen] = NULL;
        pCmdStr->searchValLen = strLen;
        pCmdStr->searchAtt = 0x00; /* number */
        pCmdStr->maxListCnt = maxListCnt;
        pCmdStr->listStartOffset = listStartOffset;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);
        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

}
int BtPacCmd::TC_BT_PAC_PullVcardListResponse(void)
{
    TcBtPacPullVcardListResCmd *pCmdStr;

    pCmdStr = (TcBtPacPullVcardListResCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullVcardListResCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULL_VCARDLIST_RES;
        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullVcardEntry(const TcBtString* handle)
{
    TcBtPacPullVcardEntryReqCmd *pCmdStr;

    pCmdStr = (TcBtPacPullVcardEntryReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullVcardEntryReqCmd));

    if(pCmdStr != NULL)
    {
        int strLen = 0;
        pCmdStr->type = TC_BT_PAC_PULL_VCARDENTRY_REQ;
        strLen = strlen((char *)handle);
        pCmdStr->ucs2name = (TcBtUint8 *)TC_BT_Pmalloc(strLen+1);
        memcpy(pCmdStr->ucs2name, handle, strLen);
        pCmdStr->ucs2name[strLen] = NULL;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);
        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_PullVcardEntryResponse(void)
{
    TcBtPacPullVcardEntryResCmd *pCmdStr;

    pCmdStr = (TcBtPacPullVcardEntryResCmd *)TC_BT_Pmalloc(sizeof(TcBtPacPullVcardEntryResCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_PULL_VCARDENTRY_RES;
        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

}
int BtPacCmd::TC_BT_PAC_SetFolder(TcBtUint16 set_curfolder)
{
    TcBtPacSetFolderReqCmd *pCmdStr;

    pCmdStr = (TcBtPacSetFolderReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacSetFolderReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_SET_FOLDER_REQ;
        pCmdStr->set_curfolder = set_curfolder;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);


        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

}
int BtPacCmd::TC_BT_PAC_SetVcardFilter(TcBtUint32 filtertype)
{
    TcBtPacSetVcardFilterReqCmd *pCmdStr;

    pCmdStr = (TcBtPacSetVcardFilterReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacSetVcardFilterReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_SET_VCARDFILTER_REQ;
        pCmdStr->filtertype = filtertype;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_AbortReq(void)
{
    TcBtPacAbortReqCmd *pCmdStr;

    pCmdStr = (TcBtPacAbortReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacAbortReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_ABORT_REQ;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);
        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
int BtPacCmd::TC_BT_PAC_SetAuthentication(TcBtUint8 value)
{
    TcBtPacSetAuthReqCmd *pCmdStr;

    pCmdStr = (TcBtPacSetAuthReqCmd *)TC_BT_Pmalloc(sizeof(TcBtPacSetAuthReqCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_PAC_SET_AUTHENTICATION_REQ;
        pCmdStr->auth = value;

        (void)TC_BT_SendCommand(TC_BT_PAC_CMD, pCmdStr);
        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}
