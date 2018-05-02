/****************************************************************************
 *   FileName    : BTAPP_PHONEBOOK.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************/
/******************************************************************************
*  Description : This file contains functions for a Bluetooth PAC
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    12/09/01     0.1            created                     KBS
*
*******************************************************************************/

#ifndef __TC_BT_PHONEBOOK_COMMON_H__
#define __TC_BT_PHONEBOOK_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "TC_BT_PAC_EVT.h"
/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* data types
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define FILE_LFN_LEN    (255 * 2)

#define MAX_MIME_LENGTH		30
#define OPS_MAX_DEVICE_CNT	0	/* Should be zero. OPP can not support multi devices.  */
extern TcBtObexResponseCode obexResult;
#if 1
/////////////////////////////////////////////////////
#define PBAP_XML_VALUE_DELIMITER '"' /* This is correct XML delimiter */
#define PBAP_VCARD_HANDLE_ATTR "<card"
#define PBAP_VCARD_LIST_END_ATTR "/>"
/////////////////////////////////////////////////////

#define MaxFamilyNameSize 32
#define MaxGivenNameSize 32

#if 0
#define DRAW_NAME_SIZE	15
#else
#define DRAW_NAME_SIZE	31 /* This value is bigger than 16 */
#define UTF8_BUFFER_SIZE 15
#endif

#define MaxCallNumberSize 21

#define DRAW_NAME_UNICODE_SIZE	40
#define DRAW_NAME_UTF8_SIZE	20

#define MAX_ATC_PBNAME_SIZE 34
#define MAX_ATC_STRING_SIZE 256

#define BT_PB_SEARCH_CHAR_SIZE					4
#define BT_PB_SEARCH_KOR_COUNT					20
#define BT_PB_SEARCH_ENG_COUNT					27
//#define BT_CONTACTS_SEARCH_MAX_COUNT					30

#ifdef BT_PBAP_MULTISERVER
typedef enum
{
	TC_BT_PB_MULTI_BEGIN = 0,
	TC_BT_PB_MULTI_SWITCHING,
	TC_BT_PB_MULTI_TERMINATE
} MultiServerEvt;
#endif

#define	SYNCML_IDLE_STATE				0
#define	SYNCML_ACTIVATED_STATE			1
#define	SYNCML_CONNECTED_STATE			2
#define	SYNCML_INIT_PACKAGE_STATE		3
#define	SYNCML_SYNC_PACKAGE_STATE		4
#define	SYNCML_UPDATE_PACKAGE_STATE		5

typedef enum {
	/*ASCII */
	CH_SET_NONE_and_ENC_NONE = 0,
	/* CHARSET=EUC-KR */
	CH_SET_EUC_KR_and_ENC_NONE,
	/* CHARSET=UTF-8 */
	CH_SET_UTF_8_and_ENC_NONE,
	/* ENCODING=QUOTED-PRINTABLE */
	CH_SET_CP949_and_ENC_Q_P,
	/* CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE */
	CH_SET_UTF_8_and_ENC_Q_P
} CharSetType;

typedef enum {
	VCF_IMG_TYPE_OTHER = 0,
	VCF_IMG_TYPE_JPG,
	VCF_IMG_TYPE_PNG,
	VCF_IMG_TYPE_MAX
} VCF_IMG_TYPE;

typedef enum {
	SEARCH_LANGUAGE_ENG = 0,
	SEARCH_LANGUAGE_KOR,
	SEARCH_LANGUAGE_MAX
} SEARCH_LANGUAGE_TYPE;

typedef struct 
{
	unsigned char FamilyName[MaxFamilyNameSize];
	unsigned char GivenName[MaxGivenNameSize];
	unsigned char DrawName[DRAW_NAME_SIZE];
	
	unsigned char TelHome[MaxCallNumberSize];
	unsigned char TelWork[MaxCallNumberSize];	
	unsigned char TelOther[MaxCallNumberSize];
	unsigned char TelCell[MaxCallNumberSize];
#if 1 /* VCARD_IMAGE_DISPLAY */
	unsigned char* image;
	unsigned int   imageSize;
	unsigned char  imageType;
#endif
	int		CharSet;
} VCARD_OBJ_TYPE;

typedef struct 
{
	unsigned char	FamilyName[MaxFamilyNameSize];
	unsigned char	GivenName[MaxGivenNameSize];
	unsigned char	DrawName[DRAW_NAME_SIZE];
	unsigned char	CallHistoryTel[MaxCallNumberSize];
	unsigned char	CallHistoryDate[MaxCallNumberSize];
	unsigned char	CallHistoryTime[MaxCallNumberSize];

	int				CharSet;
	unsigned int 	vCardNum;
	unsigned char	callHistoryType;
} CALLHISTORY_OBJ_TYPE;

#ifndef BT_PBAP_MAX_CONNECTIONS
#define BT_PBAP_MAX_CONNECTIONS		1
#endif

typedef struct
{
	unsigned short			service;
	int						state;
	unsigned char			anchor[18]; /* for syncML */
	int						vCardType;
	unsigned char		   *vCard;
	int						vCardLen;
	int						deviceId;
	int						deviceCnt;
	TcBtBdAddr				deviceAddr;
	int						vCardObjNum[BT_PBAP_MAX_CONNECTIONS];
	VCARD_OBJ_TYPE			vCardObj[BT_PBAP_MAX_CONNECTIONS][TCC_MAX_VCARD_NUMBER];
	CALLHISTORY_OBJ_TYPE	CallHistory[TCC_MAX_CALLHISTORY_FOLDER_NUMBER][TCC_MAX_CALLHISTORY_NUMBER];
	unsigned int 			listStartOffset;
	unsigned int 			maxListCnt;
	int						index;
/* parsing information */	
	int						receivedvCardNum;
	int						noNumbervCardNum;
	int						parsingErrvCardNum;
/* search */
	unsigned char			searchKeyword;

} PHONEBOOK_INST_TYPE;

typedef struct
{
	char handle[DRAW_NAME_SIZE];
	char name[DRAW_NAME_SIZE];
}VCARD_LIST_INST;

typedef struct
{
	VCARD_LIST_INST 		vCardListObj[TCC_MAX_VCARD_LIST_NUMBER];
	unsigned int 			numOfHandle;
	unsigned int 			numOfName;
	unsigned int 			listStartOffset;
	unsigned int 			maxListCnt;
	unsigned int 			currentFolder;
}PHONEBOOK_LIST_TYPE;

typedef struct
{
	VCARD_OBJ_TYPE			vCardObj;
	CALLHISTORY_OBJ_TYPE	CallHistory;

	unsigned char name[DRAW_NAME_UNICODE_SIZE];
	unsigned char telNumberCell[MaxCallNumberSize];
	unsigned char telNumberHome[MaxCallNumberSize];
	unsigned char telNumberWork[MaxCallNumberSize];	
	unsigned char telNumberOther[MaxCallNumberSize];
	unsigned char date[MaxCallNumberSize];
	unsigned char time[MaxCallNumberSize];	
}PHONEBOOK_ENTRY_TYPE;

typedef struct
{
	unsigned int			searchIdxListCount;
	unsigned char			searchKeyword[BT_PB_SEARCH_CHAR_SIZE];
	unsigned char			searchLanguage;
	unsigned int 			searchIdxList[TCC_MAX_VCARD_NUMBER];
}PHONEBOOK_SEARCH_INDEX_TYPE;
#endif


typedef struct
{
	unsigned int vCardListTotalCount;
	unsigned int vCardListPageCount;
	unsigned int vCardListPageRemain;
	unsigned int vCardListCurPageNum;
} BT_VCARD_LIST_BROWSING_INFO;

typedef struct
{
	unsigned int contactsTotalCount;
	unsigned int contactsPageCount;
	unsigned int contactsPageRemain;
	unsigned int contactsCurPageNum;
} BT_CONTACTS_BROWSING_INFO;

typedef struct
{
	unsigned int callhistoryTotalCount;
	unsigned int callhistoryPageCount;
	unsigned int callhistoryPageRemain;
	unsigned int callhistoryCurPageNum;
} BT_CALLHISTORY_BROWSING_INFO;

typedef struct
{
	unsigned int mTotalCnt;
	unsigned char name[DRAW_NAME_UNICODE_SIZE]; /* unicode string*/
	unsigned char telNumberCell[MaxCallNumberSize];
	unsigned char telNumberHome[MaxCallNumberSize];
	unsigned char telNumberWork[MaxCallNumberSize];
	unsigned char telNumberOther[MaxCallNumberSize];
	unsigned char date[MaxCallNumberSize];
	unsigned char time[MaxCallNumberSize];
	unsigned char callHistoryType;
	unsigned char nameInfo;
#if 1 /*VCARD_IMAGE_DISPLAY*/
	unsigned char imageType;
#endif
} BT_PB_OBJ_INFO;

typedef struct
{
	//unsigned char handle[DRAW_NAME_UNICODE_SIZE];
	unsigned char handle[DRAW_NAME_UTF8_SIZE];
	unsigned char name[DRAW_NAME_SIZE];
} BT_PB_VCARD_LIST;

typedef struct
{
	unsigned char handle[DRAW_NAME_UTF8_SIZE];
	unsigned char name[DRAW_NAME_SIZE];
} UI_BT_PB_VCARD_LIST;

typedef struct
{
	unsigned int  vCardObjNum;
	unsigned int  receivedvCardNum;
	unsigned int  noNumbervCardNum;
	unsigned int  parsingErrvCardNum;
} BT_CONTACTS_PARSING_INFO;

typedef struct
{
	unsigned int  vCardObjNum;
	unsigned int  currentPage;
} BT_CONTACTS_COUNT_INFO;

typedef struct
{
	unsigned short	service;
	int				data1;
	int				data2;
} BT_CONTACTS_EVENT_INFO;

typedef struct
{
	unsigned char name[DRAW_NAME_UNICODE_SIZE];
	unsigned char nameInfo;
#if 1 /* VCARD_IMAGE_DISPLAY */
	unsigned char vcfImgType;
#endif
} BT_PB_UI_LIST;

typedef struct
{
    unsigned short		service;
    unsigned short		currentStatus;
    unsigned char       *payload;
    unsigned long int   payloadLength;
} TcBtPacParsing;

/* for AT-CMD */
typedef struct
{
	TcBtBool				atPBStorageME;	/* CPBS : Phone Storage */
	TcBtBool				atPBStorageSM;	/* CPBS : SIM Storage */
	TcBtBool				atPBStorageMT; 	/* CPBS : Phone+SIM Storage */
	TcBtBool				atPBStorageDC;	/* CPBS : Dialled CallHistory Storage */
	TcBtBool				atPBStorageRC;	/* CPBS : Received CallHistory Storage */
	TcBtBool				atPBStorageMC;	/* CPBS : Missed CallHistory Storage */
	TcBtBdAddr				deviceAddr;
	int						Token;
	int						additionalCmd;
	int						nextToken;
	int						CharSet;
	char					CharSetData[60]; // for utf-8
	int						Manufact;
	int						CpbrNum;
	int						CpbrSum;   		  /* Total contacts count(ME+SM) */
	int						CpbrCHNum; 		  /* Call history count */
	int						CpbrCHSum; 		  /* Total call history count(ME+SM) */
	int						receivedCpbrNum;  /* received contacts count */
	int						finalSendCpbrNum; /* Contacts count for final CPBR command */
	int						CpbrCmdCount; 	  /* CPBR command count */
	TcBtBool				currentStorage;   /* Current storage(ME:0, SM:1 CH:2) */
} AtCMDPBData_t;

typedef void (*ATCMD_FUNC_t)(void);

typedef struct 
{
	int	Cmd;
	int	NextCmd;
	ATCMD_FUNC_t fpSend;
} ATCMD_EVNT_t;

#define BTAPP_HF_CALLNUM_MAX_LEN 32

typedef struct
{
	unsigned char		unicodeOrNot;			/* 0:ASCII, 1:Unicode */
	unsigned char		entryInPhonebook;		/* 0:Not exist, 1:exist */
	unsigned char		callNumber[BTAPP_HF_CALLNUM_MAX_LEN];
	unsigned char		phonebookName[BTAPP_HF_CALLNUM_MAX_LEN];
} Call_Entry_PB_Data;

extern Call_Entry_PB_Data	CurrentCallEntryPBData;
extern unsigned char 		gBTPBAPSelectSorting;  /* 0:First name first, 1:Last name first */
extern unsigned char 		gPBAPSelectSource;
extern unsigned char 		select_callhistory[2]; /* [0] = 0:download, 1:browsing */ /* [1] = 0:Dialled CH, 1:Received CH, 2:Missed CH, 3:Reserved, 4:contacts */
extern int 					gBT_OPP_CONFIRM_ALL_OK;
extern unsigned char 	       gBTPBAPvCardFilterIdx;
extern unsigned char 		gBTPBAPAutoDownIdx;
extern unsigned char		gBTPBVcardDumpFlag;
extern unsigned char		gBTPBVcardDumpFirstFlag;
extern unsigned int			gPBAPCHSelectedIndex;

extern unsigned char *pgVCardListBufPb; /* vCard list raw data */
extern unsigned int ngObjListLenPb; /* vCard list raw data size */

extern unsigned char *pgVCardEntryBufPb; /* vCard entry raw data */
extern unsigned int ngObjEntryLenPb; /* vCard entry raw data size */

extern unsigned char *pgVcardBuffer; /* vCard temp buffer */
extern int ngVcardBufferSize; /* vCard temp buffer size */
extern unsigned int ngVcardTotalSize; /* vCard entry indication data size */
extern unsigned char gVcardOverflowBufFlag;


/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef enum {
	ATCMD_NONE = 0,
	ATCMD_CGMI,
	ATCMD_CSCS,
	ATCMD_SET_CSCS,	
	ATCMD_CPBS,
	ATCMD_CPBS_ME,
	ATCMD_CPBS_DC,
	ATCMD_CPBS_MC,
	ATCMD_CPBS_RC,
	ATCMD_CPBS_NUM,
#ifdef  R_ATCMD_SIM
	ATCMD_CPBS_SM,
	ATCMD_CPBS_SM_NUM,
#endif
	ATCMD_CPBR_INDEX,
	ATCMD_CPBR,
#ifdef  R_ATCMD_SIM
	ATCMD_CPBR_SM,	
#endif
	ATCMD_CPBR_CH,
	ATCMD_CSCS_SONY
} AT_CMD_INDEX;

#define TCC_MAX_AT_PHONE_NUM_LENGTH		20
#define TCC_MAX_AT_NAME_STR_LENGTH		32

#define MAX_ATCMD_STR_SIZE				128

#define ATCMD_CONTACTS_EVENT			6
#ifdef  R_ATCMD_SIM
#define ATCMD_CONTACTS_EVENT_ENTRY		7
#else
#define ATCMD_CONTACTS_EVENT_ENTRY		7
#endif
#define TCC_HF_PROFILE_UUID       			((unsigned short) 0x11E)

#define ATCMD_STATUS_DISCONNECT		0
#define ATCMD_STATUS_PHONEBOOK		1
#define	ATCMD_STATUS_DIALLEDCALL	2
#define	ATCMD_STATUS_RECEIVEDCALL	3
#define	ATCMD_STATUS_MISSEDCALL		4
#define ATCMD_STATUS_IDLE			0xFF

#define	BT_TEXT_CONTACT_NONAME_MSG  			"No Name    "

#define BTAPP_VCARD_PARAM_TYPE_MISSED        ((TcBtUint32) 0x01000000)
#define BTAPP_VCARD_PARAM_TYPE_RECEIVED      ((TcBtUint32) 0x02000000)
#define BTAPP_VCARD_PARAM_TYPE_DIALED        ((TcBtUint32) 0x04000000)

#define BTAPP_PB_ROLE		0
#define BTAPP_PB_STORAGE	1

//#define PB_SOURCE_PHONE	0
//#define PB_SOURCE_SIM	1

#define	PB_CLEAR_DB_CONTACTS		BT_PBAP_MAX_CONNECTIONS	
#define	PB_CLEAR_DB_ALL			BT_PBAP_MAX_CONNECTIONS+1

/* Phonebook cuurrent status */
typedef enum {
	BTAPP_PB_STATUS_PAC_PULLPB_IND = 0,
	BTAPP_PB_STATUS_PAC_PULLPB_CFM,
	BTAPP_PB_STATUS_PAC_PULLPB_ERR,
	BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM,
	BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM,
	BTAPP_PB_STATUS_ATC_PULLPB_CFM,
	BTAPP_PB_STATUS_ATC_PULLPB_ERR,
	BTAPP_PB_STATUS_OPS_PUT_IND,
	BTAPP_PB_STATUS_OPS_DISCONNECT_IND,
	BTAPP_PB_STATUS_SML_PARSE_IND,
	BTAPP_PB_STATUS_SML_PARSE_CFM,

	BTAPP_PB_STATUS_MAX
} TcBtPBStatus;

/* select_callhistory[BTAPP_PB_ROLE] */
typedef enum {
	BTAPP_PB_ROLE_DOWNLOAD = 0,
	BTAPP_PB_ROLE_BROWSING,

	BTAPP_PB_ROLE_MAX
} TcBtPBRole;

/* select_callhistory[BTAPP_PB_STORAGE] */
typedef enum {
	BTAPP_PB_SELECT_OCH = 0,
	BTAPP_PB_SELECT_ICH,
	BTAPP_PB_SELECT_MCH,
	BTAPP_PB_SELECT_CCH,
	BTAPP_PB_SELECT_PB,
	BTAPP_PB_SELECT_NONE,

	BTAPP_PB_SELECT_MAX
} TcBtPBSelectStorage;

/* Phonebook set folder */
typedef enum {
	BTAPP_PB_FOLDER_ROOT = 0,
	BTAPP_PB_FOLDER_TELECOM,
	BTAPP_PB_FOLDER_SIM,
	BTAPP_PB_FOLDER_PB,
	BTAPP_PB_FOLDER_ICH,
	BTAPP_PB_FOLDER_OCH,
	BTAPP_PB_FOLDER_MCH,
	BTAPP_PB_FOLDER_CCH,
	BTAPP_PB_FOLDER_BACK,

	BTAPP_PB_FOLDER_MAX
} TcBtPBFolder;

/*****************************************************************************
*
* extern
*
*****************************************************************************/
extern unsigned short gATCMDStatus; 
extern unsigned char gKorUtf8CharacterTable[BT_PB_SEARCH_KOR_COUNT][BT_PB_SEARCH_CHAR_SIZE];
extern unsigned char gEngCharacterTable[BT_PB_SEARCH_ENG_COUNT][BT_PB_SEARCH_CHAR_SIZE];

#ifdef VCARD_DB_ON_NOR_FLASH
	#ifdef VCARD_DB_SST39VF1601
	#define NOR_SECTOR_VCARD_DB	(0x000f8000) // 0x701f0000
	#define NOR_SECTOR_SIZE		0x800		// size is 2kB
	#define NOR_VCARD_DB_SIZE		0xf000		// size is 60kB
	#elif defined(VCARD_DB_MX29LV320DB)
	#define NOR_SECTOR_VCARD_DB	(0x001F0000) // 0x703E0000
	#define NOR_SECTOR_SIZE		0x10000		// size is 64kB
	#define NOR_VCARD_DB_SIZE		0x10000		// size is 64kB
	#else
	#error Please select NOR flash type !!
	#endif

	extern void WriteVcardDB(unsigned char *DBptr, unsigned int NorOffset, unsigned int uiSize, unsigned int uiSectorSize);
	typedef void tpNORwriteVcardDB(unsigned char*, unsigned int, unsigned int, unsigned int);
	extern tpNORwriteVcardDB *pNORwriteVcardfunc;
	extern void NorFlash_Open(void);
	extern void NorFlash_Close(void);
#endif

/*****************************************************************************
*
* Phonebook common functions - string utils
*
*****************************************************************************/
extern char *BtApp_PB_GetPhonebookNameByNumber(const unsigned char *pStrCallNumber, unsigned char *uniCodeMode); /* for handsfree */
extern char *BtApp_PB_SearchforHfCall(const char *pSrcStr, unsigned char* unicodeOrNot, unsigned int ucDevNo); /* for handsfree */
//extern int atoi(const char *str);
//extern char *strstr( const char *str1, const char *str2);

#ifdef USE_BTBR
extern TcBtUint32 TcBtVcardParse(TcBtUint8* buffer, TcBtUint32 bufferSize, TcBtVcardData** vcardData, TcBtUint32 flags);
#else
extern TcBtUint32 TcBtVcardParse(TcBtUint8* buffer, TcBtUint32 bufferSize, TcBtUint32* vcardDataEntries, TcBtVcardData** vcardData, TcBtUint32 flags);
#endif
extern void TcBtVcardFree(TcBtVcardData* vcardData);
extern char *BtApp_PB_StrStr(TcBtUint8 *s1, const TcBtUint8 *s2, int size);
extern char *BtApp_PB_CmpStr(TcBtUint8 *s1, const TcBtUint8 *s2, int size);
extern int BtApp_PB_Unicode2UTF8(const unsigned char *pUnicode,unsigned char *pUTF8,int maxlen);
extern int BtApp_PB_UTF8toUnicode(const unsigned char *pucUTF8,unsigned char *pucUnicode,int UTF8Length);

/*****************************************************************************
*
* Phonebook common functions - characterset utils
*
*****************************************************************************/
extern void BtApp_PB_FindVcardCharSet(const TcBtUint8* vCardPBData, TcBtUint32 vCardIndex);
extern void BtApp_PB_FindCallHistoryCharSet(const TcBtUint8* vCardCHData, TcBtUint32 vCardIndex);

/*****************************************************************************
*
* Phonebook common functions - database utils
*
*****************************************************************************/
 PHONEBOOK_LIST_TYPE *BtApp_PB_GetPhonebookList(void);
 PHONEBOOK_ENTRY_TYPE *BtApp_PB_GetPhonebookEntry(void);
 PHONEBOOK_INST_TYPE *BtApp_PB_GetPhonebookInstance(void);
 PHONEBOOK_SEARCH_INDEX_TYPE *BtApp_PB_GetPhonebookSearchIndex(void);
 void BtApp_PB_ClearPhonebookList(void);
 void BtApp_PB_ClearPhonebookEntry(void);
 void BtApp_PB_ClearPhonebookInstance(int Opt);
 void BtApp_PB_ClearCallHistory(void);
 void BtApp_PB_ClearContacts(void);
 void BtApp_PB_ClearPhonebookSearchIndex(void);
 char *BtApp_PB_GetPhonebookString(unsigned int loopEntry);
 char *BtApp_PB_GetCallHistoryString(char* pBinaryStr, unsigned int loopEntry);
 int BtApp_PB_AddPhonebook(const TcBtVcardData* vcardData);
 void BtApp_PB_AddCallHistory(const TcBtVcardData* vcardData);
 unsigned short *BtApp_PB_GetNameFromPB(const char* strCLID, int* CharSetEncoding);

/*****************************************************************************
*
* Phonebook common functions - vCard parser utils
*
*****************************************************************************/
 VcardDataModify * BtApp_PB_ModifyVcard(TcBtUint8 * buffer, TcBtUint32 bufferSize);
 void BtApp_PB_DataModifypfree(VcardDataModify *DataModify);
 TcBtUint32 BtApp_PB_vCardFunction(TcBtUint8* vCardData, TcBtUint32 vCardBufferSize);
 char *BtApp_PB_findVcardPrefix(TcBtUint8 *vcf, int size);
 char *BtApp_PB_findVcardLastSuffix(TcBtUint8 *vcf, int size);
 char *BtApp_PB_findVcardSuffix(TcBtUint8 *vcf, int size);
 unsigned char BtApp_PB_CheckvCardCRLF(const TcBtUint8 * buffer, TcBtUint32 bufferSize);
 unsigned char BtApp_PB_vcfDump(TcBtUint8* vcfData, TcBtUint32 vcfSize);
 void BtApp_PB_vcfParsing(TcBtUint8* vcfData, TcBtUint32 vcfSize, PHONEBOOK_INST_TYPE *PbookInst);
/*****************************************************************************
*
* Phonebook common functions - vCard list parser utils
*
*****************************************************************************/
 unsigned char BtApp_PB_checkSpace(int c);
 void BtApp_PB_GetAttributeValue(char *element, TcBtUint16 length, char **value);
 char* BtApp_PB_vCardListNameConversion(const char* name);
 void BtApp_PB_vCardListParseEntry(TcBtUint8* entry, TcBtUint16 length);
 void BtApp_PB_vCardListParsing(TcBtUint8* vcfListData, TcBtUint32 vcfListSize);

/*****************************************************************************
*
* Phonebook common functions - phonebook sorting utils
*
*****************************************************************************/
extern void BtApp_PB_ContactsSortingHandler(void);
extern void BtApp_PB_vCardSortingThread(unsigned short currentStatus);
extern void BtApp_PB_PbSortingThread(void  *ptrInst);

/*****************************************************************************
*
* Phonebook common functions - phonebook common process utils
*
*****************************************************************************/
extern void BtApp_PB_ContactsBgThread(void *ptr);
extern void BtApp_PB_vCardBgThread(void  *ptrInst);

/*****************************************************************************
*
* PBAP common functions - PBAP common process
*
*****************************************************************************/
extern void BtApp_PB_SetCallHistoryFolder(int callhistory);
extern void BtApp_PB_PacConnectHandler(TcBtUint8 source);
extern void BtApp_PB_PacConnectErrHandler(void);
extern void BtApp_PB_SendCallhistoryReceived(void);
extern void BtApp_PB_SendCallhistoryDialled(void);
extern void BtApp_PB_SendCallhistoryMissed(void);
extern void BtApp_PB_SendCallhistoryCombined(void);
extern void BtApp_PB_RoleDisconnect(void);
extern void BtApp_PB_PacDisconnect(void);
extern TcBtBdAddr *BtApp_PB_FindConnectingBdAddr(void);
extern void BtApp_PB_SetConnectingBdAddr(const TcBtBdAddr *SrvAddr);
extern int BtApp_PB_GetDeviceId(void);
extern void BtApp_PB_ClearvCardObj(void);
extern void BtApp_PB_RetryPbDownload(unsigned char param1,unsigned char param2);
extern void BtApp_PB_RetryPbListDownload(unsigned char param1,unsigned char param2);
extern void BtApp_PB_InitVcardTempBuffer(void);
#if 0// This code is not valid in Linux
extern void BtApp_PB_SetVcfImgBuffer(unsigned int vcfImgIdx);
extern int BtApp_PB_GetVcfJpgImgBufferSize(unsigned int * totalDataSize, unsigned int vcfImgIdx);
#endif
extern int BtApp_PB_SearchKorNameFromVcardObjDB(int index);
extern int BtApp_PB_SearchKorNameVcardListDB(int index);
extern unsigned int BtApp_PB_MakeSearchContactsIndexList(void);

#ifdef BT_PBAP_MULTISERVER
extern int BtApp_PB_SetDeviceId(unsigned int stat);
extern int BtApp_PB_GetMultiServerStatus(unsigned int multiStat);
extern void BtApp_PB_MultiServerEvtHandler(int event, const TcBtBdAddr *DevAddr);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_PHONEBOOK_COMMON_H__ */
