//*****************heard file********************//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <alsa/asoundlib.h>
#include "BT_CFG.h"
#include "BtPhoneBookCmd.h"
#include "BtSys.h"
#include "BtPhoneBook.h"
#include "BtPhoneBookEvt.h"
#include "BtApp_Gap.h"
#include "BtApp_HF.h"
#include "TC_BT/inc/BtNvDbFile.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/BtPacCmd.h"
#include "TC_BT/inc/TC_BT_HF_DEV.h"
//** *****************static param ***********************//

//***********************static fun ******************//


//******************* interface *********************//

BtPhoneBookCmd::BtPhoneBookCmd()
{
}
BtPhoneBookCmd::~BtPhoneBookCmd()
{

}
static BtPhoneBookCmd  _btPb;
BtPhoneBookCmd *GetglobalBtPhoneBookCmd()
{
    return &_btPb;
}
void BtPhoneBookCmd::BtApp_PB_CmdPACPullPhonebook(unsigned char source)
{
#ifdef BT_PBAP_MULTISERVER
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
#endif

#ifdef BT_PBAP_MULTISERVER
    //BtApp_PB_ClearContacts();
    BtApp_PB_ClearPhonebookInstance(PB_CLEAR_DB_CONTACTS);
    PBInst->receivedvCardNum = 0;
    PBInst->noNumbervCardNum = 0;
    PBInst->parsingErrvCardNum = 0;
#else
    BtApp_PB_ClearPhonebookInstance(PB_CLEAR_DB_ALL); /* Clear contacts list */
#endif

    /* phonebook temp buffer initialize */
    if(pgVcardBuffer != NULL)
    {
        TC_BT_Pfree(pgVcardBuffer);
        pgVcardBuffer = NULL;
    }
    ngVcardBufferSize = 0;
    ngVcardTotalSize = 0;
    gVcardOverflowBufFlag = FALSE;

#ifdef BT_PBAP_CLIENT
    if(source == BTAPP_PB_SELECT_PB)
    {
        BtApp_PB_PacConnectHandler(gPBAPSelectSource); /* Download Contacts */
    }
    else
    {
        (void)G_BtPacCmd->TC_BT_PAC_PullPhonePB(0, gPBAPSelectSource); /* Download 0 size Phonebook */
    }
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACPullCallHistory(unsigned char source)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACPullCallHistory",__FILENAME__,__LINE__);

    BtApp_PB_ClearCallHistory(); /* Clear contacts list */
    PBInst->receivedvCardNum = 0;
    PBInst->noNumbervCardNum = 0;
    PBInst->parsingErrvCardNum = 0;

#ifdef BT_PBAP_CLIENT
    if(source == BTAPP_PB_SELECT_OCH)
    {
        BtApp_PB_SendCallhistoryDialled(); /* Download Outgoing calls */
    }
    else if(source == BTAPP_PB_SELECT_ICH)
    {
        BtApp_PB_SendCallhistoryReceived(); /* Download Incoming calls */
    }
    else if(source == BTAPP_PB_SELECT_MCH)
    {
        BtApp_PB_SendCallhistoryMissed(); /* Download Missed calls */
    }
    else if(source == BTAPP_PB_SELECT_CCH)
    {
        BtApp_PB_SendCallhistoryCombined(); /* Download Combined calls */
    }
    else
    {
        /* TO DO */
    }
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACPullVcardList(unsigned char source, unsigned int listStartOffset, unsigned int maxListCnt)
{
    unsigned char searchVal[2];

    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACPullVcardList",__FILENAME__,__LINE__);

    searchVal[0] = '*';
    searchVal[1] = NULL;

    if(pgVCardListBufPb != NULL)
    {
        TC_BT_Pfree(pgVCardListBufPb);
        pgVCardListBufPb = NULL;
    }
    ngObjListLenPb = 0;

#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_PullVcardList(searchVal, source, listStartOffset, maxListCnt);
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACPullVcardEntry(unsigned char currentFolder, unsigned char* handle)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACPullVcardEntry",__FILENAME__,__LINE__);

    BtApp_PB_ClearPhonebookEntry(); /* clear pb entry DB */
    PBInst->receivedvCardNum = 0;
    PBInst->noNumbervCardNum = 0;
    PBInst->parsingErrvCardNum = 0;

    select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_BROWSING;
    select_callhistory[BTAPP_PB_STORAGE] = currentFolder;

    if(pgVCardEntryBufPb != NULL)
    {
        TC_BT_Pfree(pgVCardEntryBufPb);
        pgVCardEntryBufPb = NULL;
    }
    ngObjEntryLenPb = 0;

#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_PullVcardEntry(handle);
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSetFolder(unsigned char source)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSetFolder",__FILENAME__,__LINE__);

    //select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_BROWSING;

    if(source == BTAPP_PB_FOLDER_PB)
    {
        select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_PB; /* current folder is phonebook */
    }
    else if(source == BTAPP_PB_FOLDER_ICH)
    {
        select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_ICH; /* current folder is call history */
    }
    else if(source == BTAPP_PB_FOLDER_OCH)
    {
        select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_OCH; /* current folder is call history */
    }
    else if(source == BTAPP_PB_FOLDER_MCH)
    {
        select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_MCH; /* current folder is call history */
    }
    else
    {
        /* TO DO */
    }

#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_SetFolder(source);
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSetAuthentication(unsigned char value)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSetAuthentication",__FILENAME__,__LINE__);

    (void)G_BtPacCmd->TC_BT_PAC_SetAuthentication(value);

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SETAUTHENTICATION_CFM);
    BtApp_PB_EvtPACSetAuthenticationCfm();
}
void BtPhoneBookCmd::BtApp_PB_CmdPACAbort(void)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACAbort",__FILENAME__,__LINE__);

#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_AbortReq();
#endif
}
void BtPhoneBookCmd::BtApp_PB_CmdPACDisconnect(void)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACDisconnect",__FILENAME__,__LINE__);

    BtApp_GAP_PacDisconnect(NULL);
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSelectSorting(unsigned char source)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSelectSorting",__FILENAME__,__LINE__);

    gBTPBAPSelectSorting = source;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SELECTSORTING_CFM);
    BtApp_PB_EvtPACSelectSortingCfm();
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSelectFeatures(unsigned char source)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSelectFeatures",__FILENAME__,__LINE__);

    select_callhistory[BTAPP_PB_ROLE] = source;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SELECTFEATURES_CFM);
    BtApp_PB_EvtPACSelectFeaturesCfm();
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSetFilter(unsigned char source)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSetFilter",__FILENAME__,__LINE__);

    gBTPBAPvCardFilterIdx = source;
#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_SetVcardFilter(gBTPBAPvCardFilterIdx);
#endif
    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SETFILTER_CFM);
    BtApp_PB_EvtPACSetFilterCfm();
}
void BtPhoneBookCmd::BtApp_PB_CmdPACSelectSource(unsigned char source)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACSelectSource",__FILENAME__,__LINE__);

    if(source == 0)
    {
        BtApp_PBAPCmdSelectSourcePhone();
    }
    else
    {
        BtApp_PBAPCmdSelectSourceSimCard();
    }

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SELECTSOURCE_CFM);
    BtApp_PB_EvtPACSelectSourceCfm();
}

/* BT_PBAP_MULTISERVER */
void BtPhoneBookCmd::BtApp_PB_CmdPACMultiServerMsgHandler(unsigned char source)
{
    unsigned char  devnum;
    char logicalLink = FALSE;

    ////printf("[%s : %05d] Info : BtApp_PB_CmdPACMultiServerMsgHandler\n",__FILENAME__,__LINE__);

    devnum = source;

    logicalLink = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_PB_PAC_CONNECT, devnum));

    /* If device is not intended, try to connect another */
    if(logicalLink == 0)
    {
#if 0
#ifdef BT_PBAP_CLIENT
        (void)TC_BT_PAC_DisconnectReq();
#endif
#ifdef BT_HANDSFREE_UNIT
        BtApp_PB_SetConnectingBdAddr(TC_BT_HF_DEV_GetBdAddr((TcBtUint32)devnum));
#endif
        BtApp_GAP_ConnectProfileAdd(BT_PROFILEMASK_PBAP);
        BtApp_GAP_Timer_ConnectStream(0,BT_PLAYBACK_LOCATION);
#endif
#ifdef BT_PBAP_MULTISERVER
        BtApp_PB_MultiServerEvtHandler(TC_BT_PB_MULTI_SWITCHING, (const TcBtBdAddr *)TC_BT_HF_DEV_GetBdAddr((TcBtUint32)devnum));
#endif
    }

}

/* Browsing command */

void BtPhoneBookCmd::BtApp_PB_CmdGetContacts(void)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetContacts",__FILENAME__,__LINE__);

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_CONTACTS);
    BtApp_PB_EvtGetContacts();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetContactsList(unsigned int listStartOffset, unsigned int maxListCnt, unsigned char* searchKeyword)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
    PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetContactsList",__FILENAME__,__LINE__);

    PBInst->listStartOffset = listStartOffset;
    PBInst->maxListCnt = maxListCnt;
    memcpy(PBSearchIdx->searchKeyword, searchKeyword, BT_PB_SEARCH_CHAR_SIZE);

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_CONTACTS_LIST);
    BtApp_PB_EvtGetContactsList();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetContactsListInfo(int index, unsigned char* searchKeyword)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
    PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetContactsListInfo",__FILENAME__,__LINE__);
    PBInst->index = index;
    memcpy(PBSearchIdx->searchKeyword, searchKeyword, BT_PB_SEARCH_CHAR_SIZE);

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_CONTACTS_LIST_INFO);
    BtApp_PB_EvtGetContactsListInfo();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetCallhistoryList(unsigned int listStartOffset, unsigned int maxListCnt)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetCallhistoryList",__FILENAME__,__LINE__);

    PBInst->listStartOffset = listStartOffset;
    PBInst->maxListCnt= maxListCnt;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_CALLHISTORY_LIST);
    BtApp_PB_EvtGetCallHistoryList();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetCallhistoryListInfo(int index)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetCallhistoryListInfo",__FILENAME__,__LINE__);

    PBInst->index = index;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_CALLHISTORY_LIST_INFO);
    BtApp_PB_EvtGetCallHistoryListInfo();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetVcardList(unsigned int listStartOffset, unsigned int maxListCnt, unsigned char* searchKeyword)
{
    PHONEBOOK_LIST_TYPE *PBList = BtApp_PB_GetPhonebookList();
    PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetVcardList",__FILENAME__,__LINE__);

    PBList->listStartOffset = listStartOffset;
    PBList->maxListCnt = maxListCnt;
    memcpy(PBSearchIdx->searchKeyword, searchKeyword, BT_PB_SEARCH_CHAR_SIZE);

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_VCARD_LIST);
    BtApp_PB_EvtGetVcardList();
}


/* Other command */

void BtPhoneBookCmd::BtApp_PB_CmdAutoDownPhonebook(unsigned char value)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdAutoDownPhonebook",__FILENAME__,__LINE__);

    gBTPBAPAutoDownIdx = value;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_AUTODOWNPB_CFM);
    BtApp_PB_EvtAutoDownPBCfm();
}

void BtPhoneBookCmd::BtApp_PB_CmdVcardDump(unsigned char value)
{
    ////printf("[%s : %05d] Info : BtApp_PB_CmdVcardDump",__FILENAME__,__LINE__);

    gBTPBVcardDumpFlag = value;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_VCARDDUMP_CFM);
    BtApp_PB_EvtVcardDumpCfm();
}

void BtPhoneBookCmd::BtApp_PB_CmdGetSearchContactsList(unsigned char* searchKeyword, unsigned char searchLanguage)
{
    PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();

    ////printf("[%s : %05d] Info : BtApp_PB_CmdGetSearchContactsList\n",__FILENAME__,__LINE__);

    memcpy(PBSearchIdx->searchKeyword, searchKeyword, BT_PB_SEARCH_CHAR_SIZE);

    PBSearchIdx->searchLanguage = searchLanguage;

    //DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_GET_SEARCH_CONTACTS_LIST);
    BtApp_PB_EvtGetSearchContactsList();
}

void BtPhoneBookCmd::BtApp_PBAPCmdSelectSourcePhone(void)
{
    gPBAPSelectSource = 0; /* source : phone */
}

void BtPhoneBookCmd::BtApp_PBAPCmdSelectSourceSimCard(void)
{
    gPBAPSelectSource = 1; /* source : sim card */
}

#ifdef BT_HANDSFREE_UNIT
void BtPhoneBookCmd::BtApp_PB_CmdDialPlaceByNumber(unsigned char devno, unsigned char size, unsigned char *hfCallNumber)
{
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //printf("[%s : %05d] Info : BtApp_HF_CmdDialPlaceByNumber",__FILENAME__,__LINE__);

    //pvalue=(unsigned char *)pData;

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetDial();

    gBtAppHfCallNumPos[devno] = size;	/* NUM / MEMORY Dial '>' */
    memcpy(gBtAppHfCallNumber[devno], hfCallNumber , size);

    BtApp_HF_InputNumberDial();
}
#endif /*BT_HANDSFREE_UNIT*/
int BtPhoneBookCmd::BtApp_PB_ReceiveEvent(TcBtEvt *event)
{
    unsigned short int evtType;

    evtType = event->type;
    switch(evtType)
    {
            case TC_BT_PAC_ACTIVATE_CFM :
            {
                TC_BT_PAC_ActivateCfm(event);
            }
            break;
            case    TC_BT_PAC_DEACTIVATE_CFM:
            {
                TC_BT_PAC_DeactivateCfm(event);
            }
            break;
            case TC_BT_PAC_CONNECT_CFM:
            {
                TC_BT_PAC_ConnectCfm(event);
            }
            break;

            case   TC_BT_PAC_PULL_PB_CFM:
            {
                TC_BT_PAC_PullPbCfm(event);
            }
            break;
            case TC_BT_PAC_PULL_PB_IND:
            {
                TC_BT_PAC_PullPbInd(event);
            }
            break;
            case    TC_BT_PAC_SET_FOLDER_CFM:
            {
                TC_BT_PAC_SetFolderCfm(event);
            }
            break;
             case TC_BT_PAC_PULL_VCARDLIST_CFM:
            {
                TC_BT_PAC_PullVcardListCfm(event);
            }
            break;
            case  TC_BT_PAC_PULL_VCARDLIST_IND:
            {
                TC_BT_PAC_PullVcardListInd(event);
            }
            break;
            case  TC_BT_PAC_PULL_VCARDENTRY_CFM:
            {
                TC_BT_PAC_PullVcardEntryCfm(event);
            }
            break;
            case  TC_BT_PAC_PULL_VCARDENTRY_IND:
            {
                TC_BT_PAC_PullVcardEntryInd(event);
            }
            break;
            case TC_BT_PAC_ABORT_CFM:
            {
                TC_BT_PAC_AbortCfm(event);
            }
            break;
            case   TC_BT_PAC_DISCONNECT_IND:
            {
                TC_BT_PAC_DisconnectInd(event);
            }
            break;
        default:
            break;
    }
}
int TC_BT_PAC_ActivateCfm(void *pTcBtEvt)
{
    TcBtPacActivateCfm *pTcBtPacEvt = (TcBtPacActivateCfm *)pTcBtEvt;
    TcBtUint16 gapResult;
	printf(" TC_BT_PAC_ActivateCfm (resultCode = 0x%04x, resultSupplier = 0x%04x)\n",  pTcBtPacEvt->resultCode, pTcBtPacEvt->resultSupplier);

    if((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_IRDA_OBEX) && (pTcBtPacEvt->resultCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE))
    {
        //TcBtEvt_PBEvt(BT_PB_EVT_PAC_ACTIVATE_CFM, pTcBtPacEvt);

        gapResult = BTAPP_GAP_ACTV_RESULT_SUCCESS;;
    }
    else
    {
        //TcBtEvt_PBEvt(BT_PB_EVT_PAC_ACTIVATE_ERR, pTcBtPacEvt);

        gapResult = BTAPP_GAP_ACTV_RESULT_FAILURE;;
    }

       BtApp_GAP_ReflectActivateResult(TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_DeactivateCfm(void *pTcBtEvt)
{
    TcBtPacDeactivateCfm *pTcBtPacEvt = (TcBtPacDeactivateCfm *)pTcBtEvt;
    TcBtUint16 gapResult;

    if((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_IRDA_OBEX) && (pTcBtPacEvt->resultCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE))
    {
        //TcBtEvt_PBEvt(BT_PB_EVT_PAC_DEACTIVATE_CFM, pTcBtPacEvt);

        gapResult = BTAPP_GAP_DACT_RESULT_SUCCESS;;
    }
    else
    {
        //TcBtEvt_PBEvt(BT_PB_EVT_PAC_DEACTIVATE_ERR, pTcBtPacEvt);

        gapResult = BTAPP_GAP_DACT_RESULT_FAILURE;;
    }

       BtApp_GAP_ReflectDeactivateResult(TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_ConnectCfm(void *pTcBtEvt)
{
    TcBtPacConnectCfm *pTcBtPacEvt = (TcBtPacConnectCfm *)pTcBtEvt;
    PHONEBOOK_INST_TYPE *PBInst;
    TcBtUint16 gapResult;
    printf(" TC_BT_PAC_ConnectCfm (resultCode = 0x%04x, resultSupplier = 0x%04x)\n",  pTcBtPacEvt->resultCode, pTcBtPacEvt->resultSupplier);

    /* success */
    if((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_IRDA_OBEX) && (pTcBtPacEvt->resultCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE))
    {
        TC_BT_SC_DB_UpdateSupportProfile((const TcBtBdAddr *)&(pTcBtPacEvt->deviceAddr), TC_BT_OBEX_PBA_SERVER_PROFILE_UUID);
        PBInst = BtApp_PB_GetPhonebookInstance();
        TC_BT_BdAddrCopyIfExist(&PBInst->deviceAddr, (const TcBtBdAddr *)&pTcBtPacEvt->deviceAddr);

        BtApp_PB_EvtPACConnectCfm();

        (void)BtApp_GAP_SetPacState(&pTcBtPacEvt->deviceAddr, BTAPP_GAP_STATE_CONNECTED);
        gapResult = BTAPP_GAP_CONN_RESULT_SUCCESS;
    }
    /* USER cancel */
    else if(((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_RFCOMM) && (pTcBtPacEvt->resultCode == TC_BT_RFC_NORMAL_DISCONNECT))
        /*|| ((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_CM) && (pTcBtPacEvt->resultCode == TC_BT_RESULT_CODE_CM_INTERNAL_ERROR))*/) /* for HTC phones */
    {
        BtApp_PB_EvtPACConnectErr();

        (void)BtApp_GAP_SetPacState(&pTcBtPacEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        gapResult = BTAPP_GAP_CONN_RESULT_CANCELLED;
    }
    else if((pTcBtPacEvt->resultSupplier == TC_BT_SUPPLIER_OBEX_PROFILES) && (pTcBtPacEvt->resultCode == TC_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED))
    {
        (void)BtApp_GAP_SetPacState(&pTcBtPacEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);
        gapResult = BTAPP_GAP_CONN_RESULT_CANCELLED;
    }
    /* fail */
    else
    {
        (void)BtApp_GAP_SetPacState(&pTcBtPacEvt->deviceAddr, BTAPP_GAP_STATE_DISCONNECTED);

        BtApp_PB_EvtPACConnectErr();

        gapResult = BTAPP_GAP_CONN_RESULT_FAILURE;
    }

    (void)BtApp_GAP_ReflectConnResult((const TcBtBdAddr *)&(pTcBtPacEvt->deviceAddr), TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_PullPbCfm(void *pTcBtEvt)
{
    TcBtPacPullPbCfm	*pTcBtPacEvt = (TcBtPacPullPbCfm *)pTcBtEvt;
    TcBtPacParsing* pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));
#if 1 /* vCard Dump */
    if(gBTPBVcardDumpFlag == TRUE)
    {
        gBTPBVcardDumpFirstFlag = 0;
        printf("vCard DUMP END!\n");
    }
#endif

    if(ngVcardTotalSize != FALSE)
    {
        if(pgVcardBuffer != NULL)
        {
            TC_BT_Pfree(pgVcardBuffer);
            pgVcardBuffer = NULL;
            ngVcardBufferSize = 0;
        }

        if(pTcBtPacEvt->responseCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE)
        {
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLPB_CFM;
        }
        else
        {
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLPB_ERR;
        }
        pTcBtPacParsing->service = BT_VCARD_PARSE;
        pTcBtPacParsing->payloadLength = 0;
        pTcBtPacParsing->payload = NULL;
#if 1 //for Linux
        BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
        CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
    }
    else
    {
        if(pTcBtPacParsing != NULL)
        {
            TC_BT_Pfree(pTcBtPacParsing);
        }

        if(pTcBtPacEvt->pbSize != FALSE)
        {
#if 1 //for Linux
                    BtApp_PB_EvtPACPullPBSizeOnlyCfm();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_SIZEONLY_CFM);
#endif
        }
        else
        {
#if 1 //for Linux
                    BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_PullPbInd(void *pTcBtEvt)
{
    TcBtPacPullPbInd*		pTcBtPacEvt = (TcBtPacPullPbInd *)pTcBtEvt;
    TcBtPacParsing* 		pTcBtPacParsing = NULL;

    unsigned char*   		vCardBufStartPtr = NULL;
    unsigned char*   		vCardBufEndPtr = NULL;
    TcBtUint8*				tempPtr = NULL;
    int 					vCardLen = 0;
    //unsigned char			overflowbuf = FALSE;
    unsigned char			vCardDumpResult = 0;

    if(pTcBtPacEvt->finalIndFlag == FALSE)
    {

        if((ngVcardBufferSize < TCC_MAX_VCARD_SIZE) && (gVcardOverflowBufFlag == FALSE))
        {
            if(CONNECTED_STATE == pTcBtPacEvt->state)
            {
                vCardBufStartPtr = pTcBtPacEvt->bodyOffset+pTcBtPacEvt->payload;
                vCardBufEndPtr = (unsigned char *)BtApp_PB_findVcardLastSuffix((TcBtUint8 *)vCardBufStartPtr, pTcBtPacEvt->bodyLength);

#if 1 /* vCard Dump */
                if(gBTPBVcardDumpFlag == TRUE)
                {
                    vCardDumpResult = BtApp_PB_vcfDump(vCardBufStartPtr, pTcBtPacEvt->bodyLength);
                    if(vCardDumpResult == TRUE)
                    {
                        printf("vCard DUMP result = SUCCESS!\n");
                    }
                    else
                    {
                        printf("vCard DUMP result = FAIL!\n");
                    }
                }
#endif

                if(vCardBufEndPtr == NULL) /* Not found "END:VCARD" */
                {
                    vCardLen = 0;

                    /* temporary buffer count */
                    tempPtr = (TcBtUint8 *)TC_BT_Pmalloc(ngVcardBufferSize + pTcBtPacEvt->bodyLength);
                    if(tempPtr != NULL)
                    {
                        memcpy(tempPtr, pgVcardBuffer, ngVcardBufferSize);
                        memcpy(&tempPtr[ngVcardBufferSize], vCardBufStartPtr, pTcBtPacEvt->bodyLength);
                        ngVcardBufferSize = ngVcardBufferSize + pTcBtPacEvt->bodyLength; /* accrue buffer size! */
                    }
                    else
                    {
                        printf("No Free Memory... \n");
                    }
                }
                else /* Found "END:VCARD" */
                {
                    vCardLen = (unsigned int)vCardBufEndPtr - (unsigned int)vCardBufStartPtr;
                    vCardLen = (vCardLen <= 0) ? 0 : vCardLen;

                    /* temporary buffer count */
                    tempPtr = (TcBtUint8 *)TC_BT_Pmalloc(ngVcardBufferSize + vCardLen);
                    if(tempPtr != NULL)
                    {
                        memcpy(tempPtr, pgVcardBuffer, ngVcardBufferSize);
                        memcpy(&tempPtr[ngVcardBufferSize], vCardBufStartPtr, vCardLen);

                        /* parsing */
                        pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));
                        if(pTcBtPacParsing != NULL)
                        {
                            pTcBtPacParsing->payload = (unsigned char *)TC_BT_Pmalloc(vCardLen+ngVcardBufferSize);
                            if(pTcBtPacParsing->payload != NULL)
                            {
                                memcpy(pTcBtPacParsing->payload, tempPtr, (vCardLen+ngVcardBufferSize));
                            }
                            else
                            {
                                printf("No Free Memory... \n");
                            }

                            pTcBtPacParsing->service = BT_VCARD_PARSE;
                            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLPB_IND; /* PBAP */
                            pTcBtPacParsing->payloadLength = vCardLen+ngVcardBufferSize;
#if 1 //for Linux
                            BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
                            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
                        }
                        else
                        {
                            printf("No Free Memory... \n");
                        }

                        TC_BT_Pfree(tempPtr);
                        tempPtr = NULL;
                    }
                    else
                    {
                        printf("No Free Memory... \n");
                    }

                    /* temporary buffer count */
                    ngVcardBufferSize = pTcBtPacEvt->bodyLength - vCardLen;
                    ngVcardBufferSize = (ngVcardBufferSize <= 0) ? 0 : ngVcardBufferSize;

                    if(ngVcardBufferSize > 0)
                    {
                        tempPtr = (TcBtUint8 *)TC_BT_Pmalloc(ngVcardBufferSize);
                        if(tempPtr != NULL)
                        {
                            memcpy(tempPtr, vCardBufEndPtr, ngVcardBufferSize);
                        }
                        else
                        {
                            printf("No Free Memory... \n");
                        }
                    }
                }

                if(pgVcardBuffer != NULL)
                {
                    TC_BT_Pfree(pgVcardBuffer);
                    pgVcardBuffer = NULL;
                }

                pgVcardBuffer = tempPtr;
            }
            else
            {
                /* not connected */
            }
        }
        else
        {
            ngVcardBufferSize = 0;
            if(pgVcardBuffer != NULL)
            {
                TC_BT_Pfree(pgVcardBuffer);
                pgVcardBuffer = NULL;
            }
            gVcardOverflowBufFlag = TRUE;
        }

        ngVcardTotalSize += pTcBtPacEvt->bodyLength;
    }
    else
    {
        if(gVcardOverflowBufFlag != TRUE)
        {
#if 1 //for Linux
            (void)G_BtPacCmd->TC_BT_PAC_PullPbResponse();
#else
            if(BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==FALSE)
            {
                (void)TC_BT_PAC_PullPbResponse();
            }
            else
            {
                BtApp_PB_RetryPbDownload();
            }
#endif
        }
        else
        {
            (void)G_BtPacCmd->TC_BT_PAC_AbortReq();
        }
    }

    if (pTcBtPacEvt->payload != NULL)
    {
        TC_BT_Pfree(pTcBtPacEvt->payload);
        pTcBtPacEvt->payload = NULL;
    }

    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_SetFolderCfm(void *pTcBtEvt)
{
    TcBtPacSetFolderCfm	*pTcBtPacEvt = (TcBtPacSetFolderCfm *)pTcBtEvt;

    if(pTcBtPacEvt->responseCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
#if 1 //for Linux
        BtApp_PB_EvtPACSetFolderCfm();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SETFOLDER_CFM);
#endif
    }
    else
    {
#if 1 //for Linux
        BtApp_PB_EvtPACSetFolderErr();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_SETFOLDER_ERR);
#endif
    }

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_PullVcardListInd(void *pTcBtEvt)
{
    TcBtPacPullVcardListInd	*pTcBtPacEvt = (TcBtPacPullVcardListInd *)pTcBtEvt;
    TcBtUint8 *lpPtr;

    if(NULL == pgVCardListBufPb)
    {
        /* first indication, create file */
        pgVCardListBufPb = (unsigned char  *)TC_BT_Pmalloc(pTcBtPacEvt->bodyLength);
        if(pgVCardListBufPb != NULL)
        {
            memcpy(pgVCardListBufPb, pTcBtPacEvt->bodyOffset+pTcBtPacEvt->payload, pTcBtPacEvt->bodyLength);
            ngObjListLenPb = pTcBtPacEvt->bodyLength;
        }
    }
    else
    {
        lpPtr = (TcBtUint8 *)TC_BT_Pmalloc(ngObjListLenPb + pTcBtPacEvt->bodyLength);
        if(lpPtr != NULL)
        {
            memcpy(lpPtr, pgVCardListBufPb, ngObjListLenPb);
            memcpy(&lpPtr[ngObjListLenPb], pTcBtPacEvt->bodyOffset+pTcBtPacEvt->payload, pTcBtPacEvt->bodyLength);
            ngObjListLenPb += pTcBtPacEvt->bodyLength;
            TC_BT_Pfree(pgVCardListBufPb);
            pgVCardListBufPb = lpPtr;
        }
    }

    if(NULL != pTcBtPacEvt->payload)
    {
        TC_BT_Pfree(pTcBtPacEvt->payload);
        pTcBtPacEvt->payload = NULL;
    }

#if 1

#if 1 //for Linux
#else
    if(BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==FALSE)
    {
        (void)TC_BT_PAC_PullVcardListResponse();
    }
    else
    {

        BtApp_PB_RetryPbDownload();
    }
#endif

#else
    (void)TC_BT_PAC_PullVcardListResponse();
#endif
    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_LIST);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_PullVcardListCfm(void *pTcBtEvt)
{
    TcBtPacPullVcardListCfm	*pTcBtPacEvt = (TcBtPacPullVcardListCfm *)pTcBtEvt;
    TcBtPacParsing* pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));

    if(pTcBtPacEvt->responseCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        if(ngObjListLenPb != FALSE)
        {
            /* Parsing vCard */
            pTcBtPacParsing->payload = (unsigned char *)TC_BT_Pmalloc(ngObjListLenPb);
            if(pTcBtPacParsing->payload != NULL)
            {
                memcpy(pTcBtPacParsing->payload, pgVCardListBufPb, ngObjListLenPb);
            }

            pTcBtPacParsing->service = BT_VCARD_PARSE;
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM;
            pTcBtPacParsing->payloadLength = ngObjListLenPb;
#if 1 //for Linux
                     BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
        }
        else
        {
            if(pTcBtPacParsing != NULL)
            {
                TC_BT_Pfree(pTcBtPacParsing);
            }
#if 1 //for Linux
                     BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }
    else
    {
        if(pTcBtPacParsing != NULL)
        {
            TC_BT_Pfree(pTcBtPacParsing);
        }
#if 1 //for Linux
        BtApp_PB_EvtPACPullVcardListErr();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLVCARDLIST_ERR);
#endif
    }

    if(pgVCardListBufPb != NULL)
    {
        TC_BT_Pfree(pgVCardListBufPb);
        pgVCardListBufPb = NULL;
    }
    ngObjListLenPb = 0;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_PullVcardEntryInd(void *pTcBtEvt)
{
    TcBtPacPullVcardEntryInd	*pTcBtPacEvt = (TcBtPacPullVcardEntryInd *)pTcBtEvt;
    TcBtUint8 *lpPtr;

    if(NULL == pgVCardEntryBufPb)
    {
        /* first indication, create file */
        pgVCardEntryBufPb = (unsigned char *)TC_BT_Pmalloc(pTcBtPacEvt->bodyLength);
        memcpy(pgVCardEntryBufPb, pTcBtPacEvt->bodyOffset+pTcBtPacEvt->payload, pTcBtPacEvt->bodyLength);
        ngObjEntryLenPb = pTcBtPacEvt->bodyLength;
    }
    else
    {
        lpPtr = (TcBtUint8 *)TC_BT_Pmalloc(ngObjEntryLenPb + pTcBtPacEvt->bodyLength);
        memcpy(lpPtr, pgVCardEntryBufPb, ngObjEntryLenPb);
        memcpy(&lpPtr[ngObjEntryLenPb], pTcBtPacEvt->bodyOffset+pTcBtPacEvt->payload, pTcBtPacEvt->bodyLength);
        ngObjEntryLenPb += pTcBtPacEvt->bodyLength;
        TC_BT_Pfree(pgVCardEntryBufPb);
        pgVCardEntryBufPb = lpPtr;
    }

    if(NULL != pTcBtPacEvt->payload)
    {
        TC_BT_Pfree(pTcBtPacEvt->payload);
        pTcBtPacEvt->payload = NULL;
    }

    (void)G_BtPacCmd->TC_BT_PAC_PullVcardEntryResponse();

    (void)G_BtSys->BtApp_SYS_SetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_ENTRY);
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
int TC_BT_PAC_PullVcardEntryCfm(void *pTcBtEvt)
{
    TcBtPacPullVcardEntryCfm	*pTcBtPacEvt = (TcBtPacPullVcardEntryCfm *)pTcBtEvt;
    TcBtPacParsing* pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));

    if(pTcBtPacEvt->responseCode == TC_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        if(ngObjEntryLenPb != FALSE)
        {
            /* Parsing vCard */
            pTcBtPacParsing->payload = (unsigned char *)TC_BT_Pmalloc(ngObjEntryLenPb);
            if(pTcBtPacParsing->payload != NULL)
            {
                memcpy(pTcBtPacParsing->payload, pgVCardEntryBufPb, ngObjEntryLenPb);
            }

            pTcBtPacParsing->service = BT_VCARD_PARSE;
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM; /* PBAP */
            pTcBtPacParsing->payloadLength = ngObjEntryLenPb;
#if 1 //for Linux
                     BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
        }
        else
        {
            if(pTcBtPacParsing != NULL)
            {
                TC_BT_Pfree(pTcBtPacParsing);
            }
#if 1 //for Linux
                     BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }
    else
    {
        if(pTcBtPacParsing != NULL)
        {
            TC_BT_Pfree(pTcBtPacParsing);
        }
#if 1 //for Linux
        BtApp_PB_EvtPACPullVcardEntryErr();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLVCARDENTRY_ERR);
#endif
    }

    if(pgVCardEntryBufPb != NULL)
    {
        TC_BT_Pfree(pgVCardEntryBufPb);
        pgVCardEntryBufPb = NULL;
    }
    ngObjEntryLenPb = 0;

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_AbortCfm(void *pTcBtEvt)
{
    //TcBtPacAbortCfm *pTcBtPacEvt; /* codesonar reserved */

    //pTcBtPacEvt = (TcBtPacAbortCfm *)pTcBtEvt; /* codesonar reserved */

    TcBtPacParsing* pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));

#if 1
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD) == TRUE) /* Stop pull pb */
    {
        if(ngVcardTotalSize != FALSE)
        {
            if(pgVcardBuffer != NULL)
            {
                TC_BT_Pfree(pgVcardBuffer);
                pgVcardBuffer = NULL;
            }
            ngVcardBufferSize = 0;

            pTcBtPacParsing->service = BT_VCARD_PARSE;
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLPB_CFM;
            pTcBtPacParsing->payloadLength = 0;
            pTcBtPacParsing->payload = NULL;
#if 1 //for Linux
                     BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
        }
        else
        {
            if(pTcBtPacParsing != NULL)
            {
                TC_BT_Pfree(pTcBtPacParsing);
            }
#if 1 //for Linux
                     BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }
    else if( G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_LIST) == TRUE) /* Stop pull vCard List */
    {
        if(ngObjListLenPb != FALSE)
        {
            /* Parsing vCard */
            pTcBtPacParsing->payload = (unsigned char *)TC_BT_Pmalloc(ngObjListLenPb);
            if(pTcBtPacParsing->payload != NULL)
            {
                memcpy(pTcBtPacParsing->payload, pgVCardListBufPb, ngObjListLenPb);
            }

            pTcBtPacParsing->service = BT_VCARD_PARSE;
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM;
            pTcBtPacParsing->payloadLength = ngObjListLenPb;
#if 1 //for Linux
                     BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif

            if(pgVCardListBufPb != NULL)
            {
                TC_BT_Pfree(pgVCardListBufPb);
                pgVCardListBufPb = NULL;
            }
            ngObjListLenPb = 0;
        }
        else
        {
            if(pTcBtPacParsing != NULL)
            {
                TC_BT_Pfree(pTcBtPacParsing);
            }
#if 1 //for Linux
                     BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }
    else if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_ENTRY) == TRUE) /* Stop pull vCard Entry */
    {
        if(ngObjEntryLenPb != FALSE)
        {
            /* Parsing vCard */
            pTcBtPacParsing->payload = (unsigned char *)TC_BT_Pmalloc(ngObjEntryLenPb);
            if(pTcBtPacParsing->payload != NULL)
            {
                memcpy(pTcBtPacParsing->payload, pgVCardEntryBufPb, ngObjEntryLenPb);
            }

            pTcBtPacParsing->service = BT_VCARD_PARSE;
            pTcBtPacParsing->currentStatus = BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM; /* PBAP */
            pTcBtPacParsing->payloadLength = ngObjEntryLenPb;
#if 1 //for Linux
                     BtApp_PB_vCardBgThread(pTcBtPacParsing);
#else
            CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif

            if(pgVCardEntryBufPb != NULL)
            {
                TC_BT_Pfree(pgVCardEntryBufPb);
                pgVCardEntryBufPb = NULL;
            }
            ngObjEntryLenPb = 0;
        }
        else
        {
            if(pTcBtPacParsing != NULL)
            {
                TC_BT_Pfree(pTcBtPacParsing);
            }
#if 1 //for Linux
                     BtApp_PB_EvtPACPullPBNoData();
#else
            DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_NODATA);
#endif
        }
    }
    else /* etc */
    {
        if(pTcBtPacParsing != NULL)
        {
            TC_BT_Pfree(pTcBtPacParsing);
        }
#if 1 //for Linux
              BtApp_PB_EvtPACAbortCfm();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_ABORT_CFM);
#endif
    }
#else

    DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_ABORT_CFM);
#endif
    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}

int TC_BT_PAC_DisconnectInd(void *pTcBtEvt)
{
    TcBtPacDisconnectInd *pTcBtPacEvt;
    TcBtBdAddr bdAddr;
    TcBtUint16 gapResult;

    pTcBtPacEvt = (TcBtPacDisconnectInd *)pTcBtEvt;


    (void)BtApp_GAP_GetPacBdAddr(&bdAddr);

    if(pTcBtPacEvt->reasonSupplier == TC_BT_SUPPLIER_BSAS_DISCONNECT)
    {
        if(BtApp_GAP_GetPacState(&bdAddr) == BTAPP_GAP_STATE_DISCONNECTING)
        {
            (void)BtApp_GAP_SetPacState(&bdAddr, BTAPP_GAP_STATE_CONNECTED);
        }
        gapResult = BTAPP_GAP_DISC_RESULT_FAILURE;
    }
    else
    {
#ifdef BT_PBAP_MULTISERVER
        //BtApp_PB_ClearvCardObj();
#else
        BtApp_PB_ClearPhonebookInstance(PB_CLEAR_DB_ALL);
#endif

        BtApp_PB_InitVcardTempBuffer();
#if 1 //for Linux
        BtApp_PB_EvtPACDisconnectInd();
#else
        DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_DISCONNECT_IND);
#endif
        (void)BtApp_GAP_SetPacState(&bdAddr, BTAPP_GAP_STATE_DISCONNECTED);
        gapResult = BTAPP_GAP_DISC_RESULT_SUCCESS;
    }

    (void)BtApp_GAP_ReflectDiscResult((const TcBtBdAddr *)&bdAddr, TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID, gapResult, BTAPP_GAP_UNUSED_INST_IDX);

    return TC_BT_EVT_RESULT_SUCCESS_ENUM;
}
