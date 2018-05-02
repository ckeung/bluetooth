#ifndef BTPHONEBOOKCMD_H
#define BTPHONEBOOKCMD_H
#include <QObject>
#include "inc/BtBase.h"
#include "TC_BT/inc/BtPacCmd.h"
class BtPhoneBookCmd: public QObject
{
        Q_OBJECT
public:
    BtPhoneBookCmd();
    ~BtPhoneBookCmd();
    void BtApp_PB_CmdPACPullPhonebook(unsigned char source);
    void BtApp_PB_CmdPACPullCallHistory(unsigned char source);
    void BtApp_PB_CmdPACPullVcardList(unsigned char source, unsigned int listStartOffset, unsigned int maxListCnt);
    void BtApp_PB_CmdPACPullVcardEntry(unsigned char currentFolder, unsigned char* handle);
    void BtApp_PB_CmdPACSetFolder(unsigned char source);
    void BtApp_PB_CmdPACSetAuthentication(unsigned char value);
    void BtApp_PB_CmdPACAbort(void);
    void BtApp_PB_CmdPACDisconnect(void);
    void BtApp_PB_CmdPACSelectSorting(unsigned char source);
    void BtApp_PB_CmdPACSelectFeatures(unsigned char source);
    void BtApp_PB_CmdPACSetFilter(unsigned char source);
    void BtApp_PB_CmdPACSelectSource(unsigned char source);
    void BtApp_PB_CmdPACMultiServerMsgHandler(unsigned char source);
    /* Browsing command */
    void BtApp_PB_CmdGetContacts(void); /* for 800x */
    void BtApp_PB_CmdGetContactsList(unsigned int listStartOffset, unsigned int maxListCnt, unsigned char* searchKeyword);
    void BtApp_PB_CmdGetContactsListInfo(int index, unsigned char* searchKeyword);
    void BtApp_PB_CmdGetCallhistoryList(unsigned int listStartOffset, unsigned int maxListCnt);
    void BtApp_PB_CmdGetCallhistoryListInfo(int index);
    void BtApp_PB_CmdGetVcardList(unsigned int listStartOffset, unsigned int maxListCnt, unsigned char* searchKeyword);
    /* Other command */
    void BtApp_PB_CmdAutoDownPhonebook(unsigned char value);
    void BtApp_PB_CmdVcardDump(unsigned char value);
    void BtApp_PB_CmdGetSearchContactsList(unsigned char* searchKeyword, unsigned char searchLanguage);
    void BtApp_PBAPCmdSelectSourcePhone(void);
    void BtApp_PBAPCmdSelectSourceSimCard(void);
    #ifdef BT_HANDSFREE_UNIT
    void BtApp_PB_CmdDialPlaceByNumber(unsigned char devno, unsigned char size, unsigned char *hfCallNumber);
    #endif

      int BtApp_PB_ReceiveEvent(TcBtEvt *event);

};
BtPhoneBookCmd *GetglobalBtPhoneBookCmd();
#define G_BtPB 	(GetglobalBtPhoneBookCmd())
#endif // BTPHONEBOOKCMD_H
