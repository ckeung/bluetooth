/****************************************************************************
 *   FileName    : BTAPP_PHONEBOOK.c
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
*  Description : This file contains functions for a Bluetooth PAC event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/07     0.1            created                         jhd
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*
*******************************************************************************/
#include "BT_CFG.h"

#if defined(BT_PBAP_CLIENT)

/*****************************************************************************
*
* Header File Include
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <alsa/asoundlib.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "BtApp_Gap.h"
#include "BtPhoneBook.h"
#include "BtPhoneBookEvt.h"
#include "BtSys.h"
#include "TC_BT/inc/TC_BT_PAC_CMD.h"
#include "TC_BT/inc/TC_BT_SD_EVT.h"
#include "TC_BT/inc/BtPlatform.h"
#include "TC_BT/inc/BtPacCmd.h"
/* TODO: Add your specialized code here */
#ifndef PBAP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

/*****************************************************************************
*
* global variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */
static PHONEBOOK_INST_TYPE		PhoneBookInstance; /* phonebook database */
static PHONEBOOK_LIST_TYPE		PhoneBookList; /* phonebook List database */
static PHONEBOOK_ENTRY_TYPE		PhoneBookEntry; /* phonebook Entry database */
static PHONEBOOK_SEARCH_INDEX_TYPE		PhoneBookSearchIdx;


static TcBtVcardData*  		PBAP_vCardData; /* for vCard parsing */

/* Need to fix for QAC(MISRA-C:2004 Rules 8.8) */
unsigned char select_callhistory[2] = {0, 0}; /* [0] = 0:download, 1:browsing */ /* [1] = 0:Dialled CH, 1:Received CH, 2:Missed CH, 3:Reserved, 4:contacts */

unsigned int		gPBAPCHSelectedIndex;
unsigned char 		gBTPBAPSelectSorting = 0;  /* 0:First name first, 1:Last name first */
unsigned char 		gBTPBAPSelectFeatures = 0; /* 0:Download, 1:Browsing */
unsigned char 		gPBAPSelectSource = 0;
unsigned char 	gBTPBAPvCardFilterIdx=0;
unsigned char 		gBTPBAPAutoDownIdx = 0;
unsigned char		gBTPBVcardDumpFlag = 0;
unsigned char		gBTPBVcardDumpFirstFlag = 0;

int		gBT_OPP_CONFIRM_ALL_OK = 0;
unsigned short 		gATCMDStatus = ATCMD_STATUS_IDLE; /* 0 : Disconnect  1: Connect - Phonebook 2 : Connect - Callhistory(Dialled)  3 : Callhistory(Received)  4: Callhistory(Missed) */

Call_Entry_PB_Data CurrentCallEntryPBData;

unsigned char *pgVCardListBufPb = NULL; /* vCard list raw data */
unsigned int ngObjListLenPb = 0; /* vCard list raw data size */

unsigned char *pgVCardEntryBufPb = NULL; /* vCard entry raw data */
unsigned int ngObjEntryLenPb = 0; /* vCard entry raw data size */

unsigned char *pgVcardBuffer = NULL; /* vCard temp buffer */
int ngVcardBufferSize = 0; /* vCard temp buffer size */
unsigned int ngVcardTotalSize = 0; /* vCard entry indication data size */
unsigned char gVcardOverflowBufFlag = FALSE;

//unsigned char *pgVcardListBuffer = NULL; /* vCard list temp buffer */
//int ngVcardListBufferSize = 0; /* vCard list temp buffer size */

unsigned int currentVcfImgIdx = 0;
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
unsigned char gKorUtf8CharacterTable[BT_PB_SEARCH_KOR_COUNT][BT_PB_SEARCH_CHAR_SIZE] =
{
	{0xFF, 0xFF, 0xFF, 0}, /* ALL */
	{225, 132, 128, 0}, /* 丑 */
	{225, 132, 129, 0}, /* 丑丑 */
	{225, 132, 130, 0}, /* 中 */
	{225, 132, 131, 0}, /* 之 */
	{225, 132, 132, 0}, /* 之之 */
	{225, 132, 133, 0}, /* 予 */
	{225, 132, 134, 0}, /* 仃 */
	{225, 132, 135, 0}, /* 仆 */
	{225, 132, 136, 0}, /* 仆仆 */
	{225, 132, 137, 0}, /* 今 */
	{225, 132, 138, 0}, /* 今今 */
	{225, 132, 139, 0}, /* 仄 */
	{225, 132, 140, 0}, /* 元 */
	{225, 132, 141, 0}, /* 元元 */
	{225, 132, 142, 0}, /* 內 */
	{225, 132, 143, 0}, /* 六 */
	{225, 132, 144, 0}, /* 兮 */
	{225, 132, 145, 0}, /* 公 */
	{225, 132, 146, 0}	/* 冗 */
};

unsigned char gEngCharacterTable[BT_PB_SEARCH_ENG_COUNT][BT_PB_SEARCH_CHAR_SIZE] =
{
	{0xFF, 0xFF, 0xFF, 0}, /* ALL */
	{'A', 0, 0, 0}, /* A */
	{'B', 0, 0, 0}, /* B */
	{'C', 0, 0, 0}, /* C */
	{'D', 0, 0, 0}, /* D */
	{'E', 0, 0, 0}, /* E */
	{'F', 0, 0, 0}, /* F */
	{'G', 0, 0, 0}, /* G */
	{'H', 0, 0, 0}, /* H */
	{'I', 0, 0, 0}, /* I */
	{'J', 0, 0, 0}, /* J */
	{'K', 0, 0, 0}, /* K */
	{'L', 0, 0, 0}, /* L */
	{'M', 0, 0, 0}, /* M */
	{'N', 0, 0, 0}, /* N */
	{'O', 0, 0, 0}, /* O */
	{'P', 0, 0, 0}, /* P */
	{'Q', 0, 0, 0}, /* Q */
	{'R', 0, 0, 0}, /* R */
	{'S', 0, 0, 0}, /* S */
	{'T', 0, 0, 0}, /* T */
	{'U', 0, 0, 0}, /* U */
	{'V', 0, 0, 0}, /* V */
	{'W', 0, 0, 0}, /* W */
	{'X', 0, 0, 0}, /* X */
	{'Y', 0, 0, 0}, /* Y */
	{'Z', 0, 0, 0}	/* Z */
};

/*****************************************************************************
*
* extern
*
*****************************************************************************/
static int BtApp_PB_TransAnsitoUni(const unsigned char *arg1, unsigned char *arg2, unsigned int StrLen);


/*****************************************************************************
*
* Phonebook common functions - string utils
*
*****************************************************************************/
char *BtApp_PB_StrStr(TcBtUint8 *s1, const TcBtUint8 *s2, int size)
{   
    int i,j;
    TcBtUint8* retStr = NULL;
  
    if(*s2=='\0') // no source
    {
        return NULL;
    }
    else
    {   
        for(j=0; j<size; j++)
        {
            if(*s1==*s2){
                for(i=1;*(s1+i)==*(s2+i);i++)
                {
                    if(*(s1+i) =='\0')
                    {
                         //i--;
                         break;   
                     }
                }
                if(i==strlen((char *)s2))
                {
                    retStr = s1;
                }
            }
            s1++;
        }
        if(retStr == NULL)
        {
        	return NULL;
        }
    }
    return (char  *)retStr;
}

char *BtApp_PB_CmpStr(TcBtUint8 *s1, const TcBtUint8 *s2, int size)
{   
    int i,j; 
  
    if(*s2=='\0') // no source
    {
        return NULL;
    }
    else
    {   
        for(j=0; j<size; j++)
        {
            if(*s1==*s2){
                for(i=1;*(s1+i)==*(s2+i);i++)
                {
                    if(*(s1+i) =='\0')
                    {
                         //i--;
                         break;   
                     }
                }
                if(i==strlen((char *)s2))
                {
                    return (char *)s1;
                }
            }
            s1++;
        }   
        return NULL;
    }   
}

unsigned char BtApp_PB_CheckvCardCRLF(const TcBtUint8 * buffer, TcBtUint32 bufferSize)
{
	TcBtUint32 count;
	char comp[3] = { '=', '\r', '\n' };

	for(count=0;count<bufferSize;count++)
	{
		if(!(memcmp(buffer+count,comp, 3)))
		{
			return 1;
		}
	}
	return 0;
}

int BtApp_PB_UTF8toUnicode(unsigned char *pucUTF8,unsigned char *pucUnicode,int UTF8Length)
{
	// return the length of unicode string in bytes
	int index;
	unsigned char *p_UTF, *p_Unicode;
	int unicode_size = 0, sizeFlag=0;
	p_UTF = pucUTF8;
	p_Unicode = pucUnicode;
	

	// confirm the BOM UTF -8
	if ( (*p_UTF == 0xEF) && (*(p_UTF + 1) == 0xBB) && (*(p_UTF + 2) == 0xBF) )
	{
		p_UTF += 3;
		UTF8Length -= 3;
	}

	
	index = 0;

	while (index < UTF8Length)	
	{
		/* Read 1 byte operation */
		if ( (*p_UTF & 0x80) == 0x00 )
		{
		

			*p_Unicode = *p_UTF;
			*(p_Unicode + 1) = 0x00;

			p_UTF++;
			index += 1;
			sizeFlag = index;
		}
		/* Read 2 bytes operation */
		else if ( ((*p_UTF & 0xC0) == 0xC0) && ((*p_UTF & 0xE0) != 0xE0))
		{
			*(p_Unicode) = (*p_UTF & 0x03) << 6;
			*(p_Unicode) = *(p_Unicode) | (*(p_UTF + 1) & 0x3F);
			*(p_Unicode+1) = (*p_UTF & 0x1C) >> 2;

			p_UTF += 2;
			index += 2;
			sizeFlag = index;
		}
		/* Read 3 bytes operation */
		else if ( (*p_UTF & 0xE0) == 0xE0 )
		{

			*(p_Unicode) = (*(p_UTF + 1) & 0x03) << 6;
			*(p_Unicode) = *(p_Unicode) | (*(p_UTF + 2) & 0x3F);
			*(p_Unicode+1) = *p_UTF << 4;
			*(p_Unicode+1) = *(p_Unicode +1)| ((*(p_UTF + 1) & 0x3C) >> 2);
			p_UTF += 3;
			index += 3;
			sizeFlag = index;
		}
		/* read 4 bytes operation */
		else
		{
			index += 4;
			// NOT SUPPORT so result value don't generate.
		}

		unicode_size += 2;
		p_Unicode += 2;
	}

	if (sizeFlag == 0)
	{
		return 0;
	}

	return unicode_size;
}

int BtApp_PB_Unicode2UTF8(const unsigned char *pUnicode,unsigned char *pUTF8,int maxlen) 
{
	unsigned short unicode = 0;
	int len=0;

#if 0
	while((unicode != 0) && (len < maxlen))
	{
		unicode=(short)(pUnicode[0])+(((short)pUnicode[1])*256);

		if(unicode<0x80)
		{
			*pUTF8=(unsigned char)unicode;
			pUTF8++;
			++len;

			if(len < maxlen)
			{
				pUnicode+=2;
			}
		}
		else if(unicode<0x0800)
		{
			*pUTF8=(unsigned char)(0xc0 | (((unsigned char)(unicode>>6))&0x1F));
			pUTF8++;
			++len;

			if(len < maxlen)
			{
				*pUTF8=(unsigned char)(0x80 | ((unsigned char)(unicode&0x003F)));
				pUTF8++;
				++len;

				if(len < maxlen)
				{
					pUnicode+=2;
				}
			}
		}
		else
		{
			*pUTF8=(unsigned char)(0xe0 | (((unsigned char)(unicode>>12))&0x0F));
			pUTF8++;
			++len;
			
			if(len < maxlen)
			{
				*pUTF8=(unsigned char)(0x80 | (((unsigned char)(unicode>>6))&0x3F));
				pUTF8++;
				++len;

				if(len < maxlen)
				{
					*pUTF8=(unsigned char)(0x80 | ((unsigned char)(unicode&0x003F)));
					pUTF8++;
					++len;

					if(len < maxlen)
					{
						pUnicode+=2;
					}
				}
			}
		}
	}
#else
	do {
		if (len>=maxlen) {
			break;
		}
		unicode=(short)(pUnicode[0])+(((short)pUnicode[1])*256);
		if (unicode<0x80) {
			*pUTF8=(unsigned char)unicode;
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
		} else if (unicode<0x0800) {
			*pUTF8=(unsigned char)(0xc0 | (((unsigned char)(unicode>>6))&0x1F));
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
			*pUTF8=(unsigned char)(0x80 | ((unsigned char)(unicode&0x003F)));
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
		} else {
			*pUTF8=(unsigned char)(0xe0 | (((unsigned char)(unicode>>12))&0x0F));
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
			*pUTF8=(unsigned char)(0x80 | (((unsigned char)(unicode>>6))&0x3F));
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
			*pUTF8=(unsigned char)(0x80 | ((unsigned char)(unicode&0x003F)));
			pUTF8++;
			++len;
			if (len>=maxlen) {
				break;
			}
		}
		pUnicode+=2;
	} while (unicode!=0);
#endif
	return len;
}

static int BtApp_PB_TransAnsitoUni(const unsigned char *arg1, unsigned char *arg2, unsigned int StrLen)
{
	unsigned int i;
	unsigned short ptTempStr;
	unsigned char TempChar[2];

	if(!StrLen)
	{
        StrLen = strlen((char *)arg1);
	}

	i=0;
	
	while((*arg1) && (i<StrLen))		//arg1 value is not 0, i is short name check 
	{
		if(*arg1 < 0x80)		//arg1 is ANSI
		{
			*arg2 = *arg1;		//arg1 steam name copy to arg2
			arg1++;
			arg2++;
			*arg2 = 0;				//arg2 last value is 0
			arg2++;
		}
		else
		{
			TempChar[0] = *arg1;
			arg1++;
			TempChar[1] = *arg1;
			arg1++;
#if 1 //for Linux
#else
			(void) UNI_MappingFontOneUnicode( TempChar, &ptTempStr);
#endif
			*arg2 = (unsigned char)ptTempStr & 0xff;
			arg2++;
			*arg2 = (unsigned char) (ptTempStr>>8) & 0xff;
			arg2++;
		}
		i++;
	}
	*arg2 = 0;
	arg2++;
	*arg2 = 0;
	/* arg2++; */ /* code sonar - unused value(20140317 - BS Kim) */
	
	return 1;
}

/*****************************************************************************
*
* Phonebook common functions - characterset utils
*
*****************************************************************************/
void BtApp_PB_FindVcardCharSet(const TcBtUint8* vCardPBData, TcBtUint32 vCardIndex)
{
	VCARD_OBJ_TYPE	*vCardObject;
	PHONEBOOK_INST_TYPE *PbookInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	TcBtUint8* retStr1 = NULL;
	TcBtUint8* retStr2 = NULL;
	TcBtUint8* retStr3 = NULL;

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
	{
		vCardObject = &(PBEntry->vCardObj);
	}
	else //if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD) /* entire vCard DB */
	{
		vCardObject = &(PbookInst->vCardObj[PbookInst->deviceId][vCardIndex]);
	}

    retStr1 = (TcBtUint8*)strstr((char *)vCardPBData, "ENCODING=QUOTED-PRINTABLE");
    retStr2 =  (TcBtUint8*)strstr((char *)vCardPBData, "CHARSET=UTF-8");
    retStr3 =  (TcBtUint8*)strstr((char *)vCardPBData, "CHARSET=utf-8");

	if((retStr1 != NULL) && ((retStr2 != NULL) || (retStr3 != NULL)))
	{
	    vCardObject->CharSet = CH_SET_UTF_8_and_ENC_Q_P;
	}
    else if(strstr((char *)vCardPBData, "ENCODING=QUOTED-PRINTABLE") != NULL)
	{
	    vCardObject->CharSet = CH_SET_CP949_and_ENC_Q_P;
	}
    else if(strstr((char *)vCardPBData, "CHARSET=EUC-KR") != NULL)
	{
	    vCardObject->CharSet = CH_SET_EUC_KR_and_ENC_NONE;
	}
    else if(strstr((char *)vCardPBData, "CHARSET=UTF-8") != NULL)
	{
	    vCardObject->CharSet = CH_SET_UTF_8_and_ENC_NONE;
	}
	else 
	{
	    vCardObject->CharSet = CH_SET_NONE_and_ENC_NONE;
	}
}

void BtApp_PB_FindCallHistoryCharSet(const TcBtUint8* vCardCHData, TcBtUint32 vCardIndex)
{
	int		CallHistoryfolder = select_callhistory[BTAPP_PB_STORAGE];
	CALLHISTORY_OBJ_TYPE	*CallHistory;
	PHONEBOOK_INST_TYPE *PbookInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	TcBtUint8* retStr1 = NULL;
	TcBtUint8* retStr2 = NULL;
	TcBtUint8* retStr3 = NULL;

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
	{
		CallHistory = &(PBEntry->CallHistory);
	}
	else //if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD) /* entire vCard DB */
	{
		CallHistory = &(PbookInst->CallHistory[CallHistoryfolder][vCardIndex]);
	}

    retStr1 = (TcBtUint8*)strstr((char *)vCardCHData, "ENCODING=QUOTED-PRINTABLE");
    retStr2 = (TcBtUint8*) strstr((char *)vCardCHData, "CHARSET=UTF-8");
    retStr3 = (TcBtUint8*) strstr((char *)vCardCHData, "CHARSET=utf-8");

	if((retStr1 != NULL) && ((retStr2 != NULL) || (retStr3 != NULL)))
	{
	    CallHistory->CharSet = CH_SET_UTF_8_and_ENC_Q_P;
	}
    else if(strstr((char *)vCardCHData, "ENCODING=QUOTED-PRINTABLE") != NULL)
	{
	    CallHistory->CharSet = CH_SET_CP949_and_ENC_Q_P;
	    
	}
    else if(strstr((char *)vCardCHData, "CHARSET=EUC-KR") != NULL)
	{
	    CallHistory->CharSet = CH_SET_EUC_KR_and_ENC_NONE;
	}
    else if(strstr((char *)vCardCHData, "CHARSET=UTF-8") != NULL)
	{
	    CallHistory->CharSet = CH_SET_UTF_8_and_ENC_NONE;
	}
	else 
	{
	    CallHistory->CharSet = CH_SET_NONE_and_ENC_NONE;
	}
}

/*****************************************************************************
*
* Phonebook common functions - database utils
*
*****************************************************************************/
/* Get data from DB */
PHONEBOOK_LIST_TYPE *BtApp_PB_GetPhonebookList(void)
{
	return &PhoneBookList;
}

PHONEBOOK_ENTRY_TYPE *BtApp_PB_GetPhonebookEntry(void)
{
	return &PhoneBookEntry;
}

PHONEBOOK_INST_TYPE *BtApp_PB_GetPhonebookInstance(void)
{
	return &PhoneBookInstance;
}

PHONEBOOK_SEARCH_INDEX_TYPE *BtApp_PB_GetPhonebookSearchIndex(void)
{
	return &PhoneBookSearchIdx;
}

/* Clear DB */
void BtApp_PB_ClearPhonebookList(void)
{
	memset(&PhoneBookList, 0, sizeof(PHONEBOOK_LIST_TYPE));
}

void BtApp_PB_ClearPhonebookEntry(void)
{
	if(PhoneBookEntry.vCardObj.image)
	{
		TC_BT_Pfree(PhoneBookEntry.vCardObj.image);
		PhoneBookEntry.vCardObj.image = NULL;
	}
	memset(&PhoneBookEntry, 0, sizeof(PHONEBOOK_ENTRY_TYPE));
}

void BtApp_PB_ClearPhonebookInstance(int Opt)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	int i = 0, j = 0;

	if(Opt < BT_PBAP_MAX_CONNECTIONS)
	{
		PBInst->vCardObjNum[Opt] = 0;
		for(i=0; i<TCC_MAX_VCARD_NUMBER; i++)
		{
			if(PBInst->vCardObj[Opt][i].image)
			{
				TC_BT_Pfree(PBInst->vCardObj[Opt][i].image);
				PBInst->vCardObj[Opt][i].image = NULL;
			}
		}
		memset(&(PBInst->vCardObj[Opt][0]), 0, sizeof(VCARD_OBJ_TYPE)*TCC_MAX_VCARD_NUMBER);
		memset(PBInst->CallHistory, 0, sizeof(CALLHISTORY_OBJ_TYPE)*TCC_MAX_CALLHISTORY_NUMBER*TCC_MAX_CALLHISTORY_FOLDER_NUMBER);
	}
	else if(Opt == PB_CLEAR_DB_CONTACTS)
	{
		memset(PBInst->vCardObjNum, 0, sizeof(int)*BT_PBAP_MAX_CONNECTIONS);
		for(i=0; i<BT_PBAP_MAX_CONNECTIONS; i++)
		{
			for(j=0; j<TCC_MAX_VCARD_NUMBER; j++)
			{
				if(PBInst->vCardObj[i][j].image)
				{
					TC_BT_Pfree(PBInst->vCardObj[i][j].image);
					PBInst->vCardObj[i][j].image = NULL;
				}
			}
		}
		memset(PBInst->vCardObj, 0, (sizeof(VCARD_OBJ_TYPE)*TCC_MAX_VCARD_NUMBER)*BT_PBAP_MAX_CONNECTIONS);
		memset(PBInst->CallHistory, 0, sizeof(CALLHISTORY_OBJ_TYPE)*TCC_MAX_CALLHISTORY_NUMBER*TCC_MAX_CALLHISTORY_FOLDER_NUMBER);
	}
	else		/* PB_CLEAR_DB_ALL */
	{
		for(i=0; i<BT_PBAP_MAX_CONNECTIONS; i++)
		{
			for(j=0; j<TCC_MAX_VCARD_NUMBER; j++)
			{
				if(PBInst->vCardObj[i][j].image)
				{
					TC_BT_Pfree(PBInst->vCardObj[i][j].image);
					PBInst->vCardObj[i][j].image = NULL;
				}
			}
		}
		memset(PBInst, 0, sizeof(PHONEBOOK_INST_TYPE));
	}
	
	BtApp_PB_ClearPhonebookSearchIndex();
}

void BtApp_PB_ClearvCardObj(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	int i = 0, j = 0;

	memset(PBInst->vCardObjNum, 0, sizeof(int)*BT_PBAP_MAX_CONNECTIONS);
	for(i=0; i<BT_PBAP_MAX_CONNECTIONS; i++)
	{
		for(j=0; j<TCC_MAX_VCARD_NUMBER; j++)
		{
			if(PBInst->vCardObj[i][j].image)
			{
				TC_BT_Pfree(PBInst->vCardObj[i][j].image);
				PBInst->vCardObj[i][j].image = NULL;
			}
		}
	}
	memset(PBInst->vCardObj, 0, (sizeof(VCARD_OBJ_TYPE)*TCC_MAX_VCARD_NUMBER)*BT_PBAP_MAX_CONNECTIONS);
	memset(PBInst->CallHistory, 0, sizeof(CALLHISTORY_OBJ_TYPE)*TCC_MAX_CALLHISTORY_NUMBER*TCC_MAX_CALLHISTORY_FOLDER_NUMBER);
}

void BtApp_PB_ClearContacts(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	int i = 0, j = 0;

	for(i=0; i<BT_PBAP_MAX_CONNECTIONS; i++)
	{
		for(j=0; j<TCC_MAX_VCARD_NUMBER; j++)
		{
			if(PBInst->vCardObj[i][j].image)
			{
				TC_BT_Pfree(PBInst->vCardObj[i][j].image);
				PBInst->vCardObj[i][j].image = NULL;
			}
		}
	}
	memset(&PhoneBookInstance.vCardObj, 0, (sizeof(VCARD_OBJ_TYPE)*TCC_MAX_VCARD_NUMBER)*BT_PBAP_MAX_CONNECTIONS);
}

void BtApp_PB_ClearCallHistory(void)
{
	memset(&PhoneBookInstance.CallHistory, 0, sizeof(CALLHISTORY_OBJ_TYPE)*TCC_MAX_CALLHISTORY_FOLDER_NUMBER*TCC_MAX_CALLHISTORY_NUMBER);
}

void BtApp_PB_ClearPhonebookSearchIndex(void)
{
	memset(&PhoneBookSearchIdx, 0, sizeof(PHONEBOOK_SEARCH_INDEX_TYPE));
	memset(PhoneBookSearchIdx.searchKeyword, 0xFF, (BT_PB_SEARCH_CHAR_SIZE-1));
	PhoneBookSearchIdx.searchKeyword[BT_PB_SEARCH_CHAR_SIZE-1] = NULL;
}

char *BtApp_PB_GetPhonebookString(unsigned int loopEntry)
{
	char 				pBinaryStr[DRAW_NAME_SIZE];
	int					BinaryStrLen = 0, RetStrLen = 0, uniStrLen = 0;
	char				*pRetStr;
	unsigned char		CharSetEncoding;
	VCARD_OBJ_TYPE *vCardObj;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();

	memset(pBinaryStr, 0x00, DRAW_NAME_SIZE);

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* Entry */
	{
        BinaryStrLen = strlen((char *)PBEntry->vCardObj.DrawName);
		if(BinaryStrLen == 0)
		{
			return NULL;
		}
		BinaryStrLen = (BinaryStrLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : BinaryStrLen;
		memcpy(pBinaryStr, PBEntry->vCardObj.DrawName, BinaryStrLen);
		pBinaryStr[BinaryStrLen] = NULL;
		
#ifdef _LCD_CPU_MONO_
		BinaryStrLen = (BinaryStrLen >= BLUETOOTH_PB_LARGE_FONT_TEXT_HT) ? BLUETOOTH_PB_LARGE_FONT_TEXT_HT : BinaryStrLen;
#endif

		CharSetEncoding = PBEntry->vCardObj.CharSet;

		/* For SKY Phone : IM-S410, U-310, IM-R470S */
		if(CharSetEncoding ==CH_SET_NONE_and_ENC_NONE)
		{
			if(PBEntry->vCardObj.DrawName[0]>0x7f)
			{
				PBEntry->vCardObj.CharSet = CH_SET_CP949_and_ENC_Q_P;
				CharSetEncoding = CH_SET_CP949_and_ENC_Q_P;
			}
		}
	}
	else /* download */
	{
		if(loopEntry >= PBInst->vCardObjNum[PBInst->deviceId]) /* invalid index */
		{
			return NULL;
		}

		vCardObj = &PBInst->vCardObj[PBInst->deviceId][loopEntry];
        BinaryStrLen = strlen((char *)vCardObj->DrawName);
		if(BinaryStrLen == 0)
		{
			return NULL;
		}
		BinaryStrLen = (BinaryStrLen >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : BinaryStrLen;
		memcpy(pBinaryStr, vCardObj->DrawName, BinaryStrLen);
		pBinaryStr[BinaryStrLen] = NULL;

#ifdef _LCD_CPU_MONO_
			BinaryStrLen = (BinaryStrLen >= BLUETOOTH_PB_LARGE_FONT_TEXT_HT) ? BLUETOOTH_PB_LARGE_FONT_TEXT_HT : BinaryStrLen;
#endif

		 	CharSetEncoding = vCardObj->CharSet;

			 /* For SKY Phone : IM-S410, U-310, IM-R470S */
			if(CharSetEncoding ==CH_SET_NONE_and_ENC_NONE)
			{
				if(vCardObj->DrawName[0]>0x7f)
				{
					vCardObj->CharSet = CH_SET_CP949_and_ENC_Q_P;
					CharSetEncoding = CH_SET_CP949_and_ENC_Q_P;
				}
			}
	}
	
	/* Return string size */
	uniStrLen = (2 * BinaryStrLen) + 2;
#if 1 //for Linux
    pRetStr = (char *)TC_BT_Pmalloc(uniStrLen);
#else
    pRetStr = (char *)TC_Allocate_Memory(uniStrLen);
#endif
	
	if(pRetStr != NULL)
	{
		memset(pRetStr, 0, uniStrLen);
	}
	else
	{
		return NULL;
	}

	/* Char Set - EUC-KR or UTF-8 to Unicode */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
			if(BinaryStrLen >= 2)
			{
				if((pBinaryStr[BinaryStrLen-1] >= 224) && (pBinaryStr[BinaryStrLen-1] <= 239))  /*added by KBS for UTF-8 character set*/
				{
					pBinaryStr[BinaryStrLen-1] = NULL;
				}
		
				if((pBinaryStr[BinaryStrLen-2] >= 224) && (pBinaryStr[BinaryStrLen-2] <= 239))
				{
					pBinaryStr[BinaryStrLen-2] = NULL;
					pBinaryStr[BinaryStrLen-1] = NULL;
				}
			}
#if 1 //for Linux
                     RetStrLen = BtApp_PB_UTF8toUnicode((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen);
#else
			RetStrLen = UTF8toUnicode(pBinaryStr, pRetStr, BinaryStrLen);
#endif
			RetStrLen = (RetStrLen >= (uniStrLen-2)) ? (uniStrLen-2) : RetStrLen;
			*(pRetStr + RetStrLen) = NULL;		/* END of string */
			*(pRetStr + RetStrLen+1) = NULL;	/* END of string */
			break;
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
            (void)BtApp_PB_TransAnsitoUni((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen+1);
			break;
		default:
			break;
	}

 	if(pRetStr != NULL)
 	{
		return pRetStr;
	}
	else
	{
		return NULL;
	}
}

char *BtApp_PB_GetCallHistoryString(char* pBinaryStr, unsigned int loopEntry)
{
	int				BinaryStrLen = 0, RetStrLen = 0, uniStrLen = 0;
	char				*pRetStr;
	unsigned char		CharSetEncoding;

	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	CALLHISTORY_OBJ_TYPE *CallHistory = &(PBInst->CallHistory[select_callhistory[BTAPP_PB_STORAGE]][0]);
		
	if(!pBinaryStr)
	{
		return NULL;
	}
		
	BinaryStrLen = strlen(pBinaryStr);
	BinaryStrLen = (BinaryStrLen >= DRAW_NAME_SIZE) ? DRAW_NAME_SIZE : BinaryStrLen;
#ifdef _LCD_CPU_MONO_
	BinaryStrLen = (BinaryStrLen >= BLUETOOTH_PB_LARGE_FONT_TEXT_HT) ? BLUETOOTH_PB_LARGE_FONT_TEXT_HT : BinaryStrLen;
#endif

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* Entry */
	{
		CharSetEncoding = PBEntry->CallHistory.CharSet;

		/* For SKY Phone : IM-S410, U-310, IM-R470S */
		if(CharSetEncoding ==CH_SET_NONE_and_ENC_NONE)
		{
			if(PBEntry->CallHistory.DrawName[0]>0x7f)
			{
				PBEntry->CallHistory.CharSet = CH_SET_CP949_and_ENC_Q_P;
				CharSetEncoding = CH_SET_CP949_and_ENC_Q_P;
			}
		}
	}
	else
	{
		CharSetEncoding = CallHistory[loopEntry].CharSet;

		/* For SKY Phone : IM-S410, U-310, IM-R470S */
		if(CharSetEncoding ==CH_SET_NONE_and_ENC_NONE)
		{
			if(CallHistory[loopEntry].DrawName[0]>0x7f)
			{
				CallHistory[loopEntry].CharSet = CH_SET_CP949_and_ENC_Q_P;
				CharSetEncoding = CH_SET_CP949_and_ENC_Q_P;
			}
		}
	}
	
	/* Return string size */
	uniStrLen = (2 * BinaryStrLen) + 2;

#if 1 //for Linux
    pRetStr = (char *)TC_BT_Pmalloc(uniStrLen);
#else
	pRetStr = TC_Allocate_Memory(uniStrLen);
#endif
	
	if(pRetStr != NULL)
	{
		memset(pRetStr, 0, uniStrLen);
	}
	else
	{
		return NULL;
	}

	/* Char Set - EUC-KR or UTF-8 to Unicode */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
			if(BinaryStrLen >= 2)
			{
				if((pBinaryStr[BinaryStrLen-1] >= 224) && (pBinaryStr[BinaryStrLen-1] <= 239))  /*added by KBS for UTF-8 character set*/
				{
					pBinaryStr[BinaryStrLen-1] = NULL;
				}
		
				if((pBinaryStr[BinaryStrLen-2] >= 224) && (pBinaryStr[BinaryStrLen-2] <= 239))
				{
					pBinaryStr[BinaryStrLen-2] = NULL;
					pBinaryStr[BinaryStrLen-1] = NULL;
				}
			}
#if 1 //for Linux
                     RetStrLen = BtApp_PB_UTF8toUnicode((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen);
#else
			RetStrLen = UTF8toUnicode(pBinaryStr, pRetStr, BinaryStrLen);
#endif
			RetStrLen = (RetStrLen >= (uniStrLen-2)) ? (uniStrLen-2) : RetStrLen;
			*(pRetStr + RetStrLen) = NULL;		/* END of string */
			*(pRetStr + RetStrLen+1) = NULL;		/* END of string */
			break;
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
            (void)BtApp_PB_TransAnsitoUni((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen+1);
			break;
		default:
			break;
	}

 	if(pRetStr != NULL)
 	{
		return pRetStr;
	}
	else
	{
		return NULL;
	}
}

int BtApp_PB_AddPhonebook(const TcBtVcardData* vcardData)
{	
	TcBtVcardDataType* PBAP_vCardIndDataTypeForString = NULL;
	TcBtVcardDataTelephone * PBAP_vcardIndTelephoneData= NULL;
#ifdef VCARD_IMAGE_DISPLAY
	TcBtVcardDataType* PBAP_vCardIndDataTypeForImage = NULL;
	TcBtVcardDataBinary* PBAP_vCardPhotoData = NULL; 
#endif
	TcBtVcardDataType* currentvCardData= NULL;
	TcBtVcardDataName * PBAP_vCardNameData= NULL;
	unsigned int FamilyNameSize,GivenNameSize,FormatNameSize,PhoneNumCellSize,PhoneNumHomeSize,PhoneNumOtherSize ,PhoneNumWorkSize,RestNameSize=0, PhoneNumSize=0;
	TcBtUint8  SP;
	TcBtUint32 vCardIndex;
	VCARD_OBJ_TYPE	*vCardObj;
	const TcBtVcardData* parsed_vcard = (const TcBtVcardData*)vcardData;
	unsigned char PBAP_vcardIndTelephoneDataTmp[MaxCallNumberSize];
	int i=0, j=0;

	PHONEBOOK_INST_TYPE *PbookInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();

	SP = 0x20;
	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
	{
		vCardObj = &(PBEntry->vCardObj);
	}
	else //if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD) /* entire vCard DB */
	{
		vCardIndex = PbookInst->vCardObjNum[PbookInst->deviceId];
		vCardObj = &(PbookInst->vCardObj[PbookInst->deviceId][vCardIndex]);		
	}

	memset(vCardObj->DrawName, 0x00, DRAW_NAME_SIZE);
	if((!(parsed_vcard->name))&&(!(parsed_vcard->formattedName)))
	{
		memcpy(vCardObj->DrawName, BT_TEXT_PBAP_NONAME_MSG, strlen(BT_TEXT_PBAP_NONAME_MSG));
	}
	else if(parsed_vcard->name != NULL) /*Name data*/
	{
		PBAP_vCardIndDataTypeForString = parsed_vcard->name;
		PBAP_vCardNameData = PBAP_vCardIndDataTypeForString->payload.name;
			
		/*Family Name data*/
		if(PBAP_vCardNameData->familyName==0)
		{
			memset(vCardObj->FamilyName, 0x00, MaxFamilyNameSize);
			FamilyNameSize =0;
		}
		else 
		{
            FamilyNameSize= strlen((char *)PBAP_vCardNameData->familyName);
            FamilyNameSize = (FamilyNameSize < MaxFamilyNameSize) ? FamilyNameSize : MaxFamilyNameSize;
            memcpy((char *)vCardObj->FamilyName, (char *)PBAP_vCardNameData->familyName, FamilyNameSize);
        }
			
		/*Given Name data*/
		if(PBAP_vCardNameData->givenName==0)
		{
			memset(vCardObj->GivenName, 0x00, MaxGivenNameSize);
			GivenNameSize =0;
		}
		else
		{
            GivenNameSize= strlen((char *)PBAP_vCardNameData->givenName);
			GivenNameSize = (GivenNameSize < MaxGivenNameSize) ? GivenNameSize : MaxGivenNameSize;
			memcpy(vCardObj->GivenName, PBAP_vCardNameData->givenName, GivenNameSize);
		}

		/*DrawName*/
		if((GivenNameSize<(DRAW_NAME_SIZE-1)) && (GivenNameSize>0)){
			memcpy(vCardObj->DrawName, PBAP_vCardNameData->givenName, GivenNameSize);
			RestNameSize = (DRAW_NAME_SIZE-1)-GivenNameSize-1;
			if(FamilyNameSize<RestNameSize){
				memcpy(vCardObj->DrawName + GivenNameSize, &SP, 1);
				memcpy(vCardObj->DrawName+GivenNameSize+1, PBAP_vCardNameData->familyName, FamilyNameSize);
			}
			else{
				memcpy(vCardObj->DrawName+GivenNameSize,&SP, 1);
				memcpy(vCardObj->DrawName+GivenNameSize+1, PBAP_vCardNameData->familyName, RestNameSize);
			}
		}
		else if(GivenNameSize >=(DRAW_NAME_SIZE-1))
		{
			memcpy(vCardObj->DrawName, PBAP_vCardNameData->givenName, DRAW_NAME_SIZE-1);
		}
		else
		{
			memset(vCardObj->DrawName, 0x00, DRAW_NAME_SIZE);
			if(FamilyNameSize ==0)
			{
				memcpy(vCardObj->DrawName, BT_TEXT_PBAP_NONAME_MSG, strlen(BT_TEXT_PBAP_NONAME_MSG));
			}
			else if(FamilyNameSize<(DRAW_NAME_SIZE-1))
			{
				memcpy(vCardObj->DrawName, PBAP_vCardNameData->familyName, FamilyNameSize);
			}
			else
			{
				memcpy(vCardObj->DrawName, PBAP_vCardNameData->familyName, DRAW_NAME_SIZE-1);
			}
		}
	}	
	else /*Formatted Name data*/
	{
		PBAP_vCardIndDataTypeForString = parsed_vcard->formattedName;
        FormatNameSize= strlen((char *)PBAP_vCardIndDataTypeForString->payload.string);
		FormatNameSize = (FormatNameSize < 16) ? FormatNameSize : 16;
		memset(vCardObj->DrawName, 0x00, DRAW_NAME_SIZE);
		memcpy(vCardObj->DrawName, PBAP_vCardIndDataTypeForString->payload.string, FormatNameSize);
		vCardObj->DrawName[FormatNameSize] = NULL;
	}

	/* TelephoneNum data*/
	PBAP_vCardIndDataTypeForString = parsed_vcard->tel;
	if(PBAP_vCardIndDataTypeForString == NULL)
	{
		PbookInst->noNumbervCardNum++;
		return(NULL);
	}
	
	PBAP_vcardIndTelephoneData =PBAP_vCardIndDataTypeForString->payload.telephone;
    PhoneNumSize = strlen((char *)PBAP_vcardIndTelephoneData->number);
	PhoneNumSize = (PhoneNumSize < MaxCallNumberSize) ? PhoneNumSize : (MaxCallNumberSize-1);

	memset(vCardObj->TelCell, 0x00, MaxCallNumberSize);
	memset(vCardObj->TelHome, 0x00, MaxCallNumberSize);
	memset(vCardObj->TelWork, 0x00, MaxCallNumberSize);
	memset(vCardObj->TelOther, 0x00, MaxCallNumberSize);

	while(PBAP_vCardIndDataTypeForString != NULL)
	{
        PhoneNumSize = strlen((char *)PBAP_vcardIndTelephoneData->number);
		PhoneNumSize = (PhoneNumSize < MaxCallNumberSize) ? PhoneNumSize : (MaxCallNumberSize-1);
		memset(PBAP_vcardIndTelephoneDataTmp, 0x00, MaxCallNumberSize);

		i = 0;
		j = 0;

		while(i < PhoneNumSize)
		{
			if((PBAP_vcardIndTelephoneData->number[i] == '-') || (PBAP_vcardIndTelephoneData->number[i] == ' ') || (PBAP_vcardIndTelephoneData->number[i] == '(') || (PBAP_vcardIndTelephoneData->number[i] == ')'))
			{
				/* No insert */
			}
			else
			{
				PBAP_vcardIndTelephoneDataTmp[j] = PBAP_vcardIndTelephoneData->number[i];
				j++;
			}

			i++;
		}
		
		/*vCard - CELL*/
		if(((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_CELL) != FALSE) && (vCardObj->TelCell[0] == NULL))
		{
            PhoneNumCellSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
			PhoneNumCellSize = (PhoneNumCellSize < MaxCallNumberSize) ? PhoneNumCellSize : (MaxCallNumberSize-1);
			memcpy(vCardObj->TelCell, PBAP_vcardIndTelephoneDataTmp, PhoneNumCellSize);
			vCardObj->TelCell[PhoneNumCellSize]=0;
		}
		/*vCard - HOME*/
		else if(((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_HOME) != FALSE) && (vCardObj->TelHome[0] == NULL))
		{
			if(((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_HOME) != FALSE) || ((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_VOICE) != FALSE))
			{
                PhoneNumHomeSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
				PhoneNumHomeSize = (PhoneNumHomeSize < MaxCallNumberSize) ? PhoneNumHomeSize : (MaxCallNumberSize-1);
				memcpy(vCardObj->TelHome, PBAP_vcardIndTelephoneDataTmp, PhoneNumHomeSize);
				vCardObj->TelHome[PhoneNumHomeSize]=0;
			}
			else if((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_STR) != FALSE)
			{
                PhoneNumOtherSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
				PhoneNumOtherSize = (PhoneNumOtherSize < MaxCallNumberSize) ? PhoneNumOtherSize : (MaxCallNumberSize-1);
				memcpy(vCardObj->TelOther, PBAP_vcardIndTelephoneDataTmp, PhoneNumOtherSize);
				vCardObj->TelOther[PhoneNumOtherSize] = 0;
			}
			else
			{
				/* TO DO */
			}
		}
		/*vCard - WORK*/
		else if(((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_WORK) != FALSE) && (vCardObj->TelWork[0] == NULL))
		{
			if(((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_WORK) != FALSE) || ((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_VOICE) != FALSE))
			{
                PhoneNumWorkSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
				PhoneNumWorkSize = (PhoneNumWorkSize < MaxCallNumberSize) ? PhoneNumWorkSize : (MaxCallNumberSize-1);
				memcpy(vCardObj->TelWork, PBAP_vcardIndTelephoneDataTmp, PhoneNumWorkSize);
				vCardObj->TelWork[PhoneNumWorkSize] = 0;
			}
			else if((PBAP_vcardIndTelephoneData->type & TC_BT_VCARD_PARAM_TYPE_STR) != FALSE)
			{
                PhoneNumOtherSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
				PhoneNumOtherSize = (PhoneNumOtherSize < MaxCallNumberSize) ? PhoneNumOtherSize : (MaxCallNumberSize-1);
				memcpy(vCardObj->TelOther, PBAP_vcardIndTelephoneDataTmp, PhoneNumOtherSize);
				vCardObj->TelOther[PhoneNumOtherSize] = 0;
			}
			else
			{
				/* TO DO */
			}
		}
		/*vCard - VOICE*/
		else if(vCardObj->TelOther[0] == NULL) /*if(PBAP_vcardIndTelephoneData->type == BCHS_VCARD_PARAM_TYPE_STR)*/
		{
            PhoneNumOtherSize= strlen((char *)PBAP_vcardIndTelephoneDataTmp);
			PhoneNumOtherSize = (PhoneNumOtherSize < MaxCallNumberSize) ? PhoneNumOtherSize : (MaxCallNumberSize-1);
			memcpy(vCardObj->TelOther, PBAP_vcardIndTelephoneDataTmp, PhoneNumOtherSize);
			vCardObj->TelOther[PhoneNumOtherSize] = 0;
		}
		else
		{
			/* TO DO */
		}

		currentvCardData = PBAP_vCardIndDataTypeForString->next;
		if(currentvCardData==NULL)
		{
			//return (TRUE);
			break;
		}
		PBAP_vcardIndTelephoneData =0x00000000;
		PBAP_vcardIndTelephoneData = currentvCardData->payload.telephone;

		PBAP_vCardIndDataTypeForString = currentvCardData;
	}

	
#ifdef VCARD_IMAGE_DISPLAY
    PBAP_vCardIndDataTypeForImage = parsed_vcard->photo;
    if(PBAP_vCardIndDataTypeForImage)
    {
        PBAP_vCardPhotoData = PBAP_vCardIndDataTypeForImage->payload.binary;
        vCardObj->image = TC_BT_Pmalloc(PBAP_vCardPhotoData->binarySize);
        if(vCardObj->image)
        {
            memcpy(vCardObj->image, PBAP_vCardPhotoData->binaryData, PBAP_vCardPhotoData->binarySize);
            vCardObj->imageSize = PBAP_vCardPhotoData->binarySize;
            if(!strcmp(PBAP_vCardPhotoData->binaryType, "JPEG"))
            {
                vCardObj->imageType = VCF_IMG_TYPE_JPG;
            }
            else if(!strcmp(PBAP_vCardPhotoData->binaryType, "PNG"))
            {
                vCardObj->imageType = VCF_IMG_TYPE_PNG;
            }
            else
            {
                vCardObj->imageType = VCF_IMG_TYPE_OTHER;
            }
        }
    }
    else
    {
        /* no image */
        vCardObj->imageType = 0;
        vCardObj->image = NULL;
        vCardObj->imageSize = 0;
    }
#endif
	
	return (TRUE);
}

void BtApp_PB_AddCallHistory(const TcBtVcardData* vcardData)
{
	TcBtVcardDataType* PBAP_vCardIndDataType = NULL;
	TcBtVcardDataTelephone * vCardTel = NULL;
	TcBtVcardDataName * vCardName = NULL;
	TcBtVcardDataIso8601DateTime * vCardDateTime = NULL;
	unsigned int FamilyNameSize, GivenNameSize, FormatNameSize, RestNameSize=0;
	unsigned int CallHistoryTelSize, strLength = 0;
	TcBtUint8  SP;
	unsigned int CallHistoryfolder;
	const TcBtVcardData* parsed_vcard = (const TcBtVcardData*)vcardData;
	CALLHISTORY_OBJ_TYPE	*CallHistory;
	TcBtUint32 numOfCallHistory = 0;

	PHONEBOOK_INST_TYPE *PbookInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();

	SP = 0x20;

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
	{
		CallHistory = &(PBEntry->CallHistory);
	}
	else //if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD) /* entire vCard DB */
	{
		CallHistoryfolder = select_callhistory[BTAPP_PB_STORAGE];
		numOfCallHistory = PbookInst->CallHistory[CallHistoryfolder][0].vCardNum;
		CallHistory = &(PbookInst->CallHistory[CallHistoryfolder][numOfCallHistory]);
	}

/*Name data*/
	memset(CallHistory, 0, sizeof(CALLHISTORY_OBJ_TYPE));
	
	if(parsed_vcard != NULL)
	{
		if(parsed_vcard->name != NULL){
			PBAP_vCardIndDataType = parsed_vcard->name;
			vCardName = PBAP_vCardIndDataType->payload.name;

			/*Family Name data*/
			if(vCardName->familyName==0)
			{
				memset(CallHistory->FamilyName, 0x00, MaxFamilyNameSize);
				FamilyNameSize =0;
			}
			else 
			{
                FamilyNameSize= strlen((char *)vCardName->familyName);
				FamilyNameSize = (FamilyNameSize < MaxFamilyNameSize) ? FamilyNameSize : MaxFamilyNameSize;
				memcpy(CallHistory->FamilyName, vCardName->familyName, FamilyNameSize);
			}

			/*Given Name data*/
			if(vCardName->givenName==0)
			{
				memset(CallHistory->GivenName, 0x00, MaxGivenNameSize);
				GivenNameSize =0;
			}
			else
			{
                GivenNameSize= strlen((char *)vCardName->givenName);
				GivenNameSize = (GivenNameSize < MaxGivenNameSize) ? GivenNameSize : MaxGivenNameSize;
				memcpy(CallHistory->GivenName, vCardName->givenName, GivenNameSize);
			}
				
			/*DrawName*/
			if((GivenNameSize<(DRAW_NAME_SIZE-1)) && (GivenNameSize>0)){
				memcpy(CallHistory->DrawName, vCardName->givenName, GivenNameSize);
				RestNameSize = (DRAW_NAME_SIZE-1)-GivenNameSize-1;
				if(FamilyNameSize<RestNameSize){
					memcpy(CallHistory->DrawName+GivenNameSize,&SP, 1);
					memcpy(CallHistory->DrawName+GivenNameSize+1, vCardName->familyName, FamilyNameSize);
				}
				else{
					memcpy(CallHistory->DrawName+GivenNameSize,&SP, 1);
					memcpy(CallHistory->DrawName+GivenNameSize+1, vCardName->familyName, RestNameSize);
				}
			}
			else if(GivenNameSize >=(DRAW_NAME_SIZE-1)){
				memcpy(CallHistory->DrawName, vCardName->givenName, DRAW_NAME_SIZE-1);
			}
			else if(GivenNameSize ==0)
			{
				if(FamilyNameSize ==0)
				{
					strLength = strlen(BT_TEXT_PBAP_NONAME_MSG);
					strLength = (strLength >= DRAW_NAME_SIZE-1) ? DRAW_NAME_SIZE-1 : strLength; 
					memcpy(CallHistory->DrawName,BT_TEXT_PBAP_NONAME_MSG, strLength);
					CallHistory->DrawName[strLength] = NULL;
				}
				else if((FamilyNameSize<(DRAW_NAME_SIZE-1)) && (FamilyNameSize>0))
				{
					memcpy(CallHistory->DrawName, vCardName->familyName, FamilyNameSize);
				}
				else
				{
					memcpy(CallHistory->DrawName, vCardName->familyName, DRAW_NAME_SIZE-1);
				}
			}
			else
			{
				/* TO DO */
			}
		}
		else{
			if(parsed_vcard->formattedName != NULL){
				PBAP_vCardIndDataType = parsed_vcard->formattedName;
                FormatNameSize= strlen((char *)PBAP_vCardIndDataType->payload.string);
				FormatNameSize = (FormatNameSize < 16) ? FormatNameSize : 16;
				memset(CallHistory->DrawName, 0x00, DRAW_NAME_SIZE);
				memcpy(CallHistory->DrawName, PBAP_vCardIndDataType->payload.string, FormatNameSize);
				CallHistory->DrawName[FormatNameSize] = NULL;
			}
			else{
					memset(CallHistory->DrawName, 0x00, DRAW_NAME_SIZE);
					memcpy(CallHistory->DrawName, BT_TEXT_PBAP_NONAME_MSG, strlen(BT_TEXT_PBAP_NONAME_MSG));
			}
		}

	/* TelephoneNum data*/
		if(parsed_vcard->tel != NULL)
		{
			PBAP_vCardIndDataType = parsed_vcard->tel;
			vCardTel = PBAP_vCardIndDataType->payload.telephone;

            CallHistoryTelSize= strlen((char *)vCardTel->number);
			CallHistoryTelSize = (CallHistoryTelSize < 20) ? CallHistoryTelSize : 19;
			memcpy(CallHistory->CallHistoryTel, vCardTel->number, CallHistoryTelSize);
			CallHistory->CallHistoryTel[CallHistoryTelSize] =0;
		}
		else
		{
			memset(CallHistory->CallHistoryTel, 0x00, MaxCallNumberSize);
		}

	/*Date & Time*/
		if(parsed_vcard->xIrmcCallDatetime != NULL)
		{
			PBAP_vCardIndDataType = parsed_vcard->xIrmcCallDatetime;
			vCardDateTime = PBAP_vCardIndDataType->payload.iso8601DateTimeTyped;

            (void)sprintf((char *)CallHistory->CallHistoryDate, "%04d/%02d/%02d", vCardDateTime->iso8601DateTime.date.year, vCardDateTime->iso8601DateTime.date.month, vCardDateTime->iso8601DateTime.date.day);
            (void)sprintf((char *)CallHistory->CallHistoryTime, "%02d:%02d", vCardDateTime->iso8601DateTime.time.hour, vCardDateTime->iso8601DateTime.time.minute);
		}
		else
		{
			memset(CallHistory->CallHistoryDate, 0x00, MaxCallNumberSize);
			memset(CallHistory->CallHistoryTime, 0x00, MaxCallNumberSize);
		}

	/* call history type */
		switch(parsed_vcard->callHistoryType)
		{
			case BTAPP_VCARD_PARAM_TYPE_DIALED:
				CallHistory->callHistoryType = 	BTAPP_PB_SELECT_OCH;
				break;

			case BTAPP_VCARD_PARAM_TYPE_RECEIVED:
				CallHistory->callHistoryType = 	BTAPP_PB_SELECT_ICH;
				break;

			case BTAPP_VCARD_PARAM_TYPE_MISSED:
				CallHistory->callHistoryType = 	BTAPP_PB_SELECT_MCH;
				break;

			default:
				CallHistory->callHistoryType = BTAPP_PB_SELECT_CCH;
				break;
		}
	}
	return;	
}

unsigned short *BtApp_PB_GetNameFromPB(const char* strCLID, int* CharSetEncoding)
{
#if 1 /* improved searching function */
	unsigned int i, strLen = 0;
	unsigned short *PB_name;
	unsigned char tempCallNumber[MaxCallNumberSize];
	char *searchNumber1 = NULL, *searchNumber2 = NULL, *searchNumber3 = NULL, *searchNumber4 = NULL;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	VCARD_OBJ_TYPE *vCardObj = &(PBInst->vCardObj[PBInst->deviceId][0]);
	
	for(i=0; i<PBInst->vCardObjNum[PBInst->deviceId]; i++)
	{
        strLen = strlen((char *)vCardObj[i].TelCell);
		if(strLen > 0){
			memset(tempCallNumber, 0, MaxCallNumberSize);
			
			if(vCardObj[i].TelCell[0] == '0'){
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelCell[1]), (strLen-1));
				tempCallNumber[strLen] = NULL;
			}else{
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelCell[0]), (strLen));
				tempCallNumber[strLen] = NULL;
			}
            searchNumber1 = strstr((char *)strCLID,(char *) tempCallNumber);
		}
        strLen = strlen((char *)vCardObj[i].TelHome);
		if(strLen > 0){
			memset(tempCallNumber, 0, MaxCallNumberSize);
			
			if(vCardObj[i].TelHome[0] == '0'){
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelHome[1]), (strLen-1));
				tempCallNumber[strLen] = NULL;
			}else{
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelHome[0]), (strLen));
				tempCallNumber[strLen] = NULL;
			}
            searchNumber2 = strstr((char *)strCLID, (char *)tempCallNumber);
		}
        strLen = strlen((char *)vCardObj[i].TelWork);
		if(strLen > 0){
			memset(tempCallNumber, 0, MaxCallNumberSize);
			
			if(vCardObj[i].TelWork[0] == '0'){
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelWork[1]), (strLen-1));
				tempCallNumber[strLen] = NULL;
			}else{
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelWork[0]), (strLen));
				tempCallNumber[strLen] = NULL;
			}
            searchNumber3 = strstr((char *)strCLID, (char *)tempCallNumber);
		}
        strLen = strlen((char *)vCardObj[i].TelOther);
		if(strLen > 0){
			memset(tempCallNumber, 0, MaxCallNumberSize);
			
			if(vCardObj[i].TelOther[0] == '0'){
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelOther[1]), (strLen-1));
				tempCallNumber[strLen] = NULL;
			}else{
				strLen = (strLen >= (MaxCallNumberSize-1)) ? (MaxCallNumberSize-1) : strLen;
                (void)strncpy((char *)tempCallNumber, (char *)&(vCardObj[i].TelOther[0]), (strLen));
				tempCallNumber[strLen] = NULL;
			}
            searchNumber4 = strstr((char *)strCLID, (char *)tempCallNumber);
		}
		
		if((searchNumber1 != NULL) || (searchNumber2 != NULL) || (searchNumber3 != NULL) || (searchNumber4 != NULL))
		{
			PB_name = (unsigned short *)vCardObj[i].DrawName;
			*CharSetEncoding = vCardObj[i].CharSet; 
			return PB_name;
		}

		searchNumber1 = NULL;
		searchNumber2 = NULL;
		searchNumber3 = NULL;
		searchNumber4 = NULL;
	}

	return NULL;
#else
	int i;
	unsigned short *PB_name;
	int searchNumber1 = 0, searchNumber2 = 0, searchNumber3 = 0, searchNumber4 = 0;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	VCARD_OBJ_TYPE *vCardObj = &(PBInst->vCardObj[PBInst->deviceId][0]);
	
	for(i= 0; i< PBInst->vCardObjNum[PBInst->deviceId]; i++)
	{
		searchNumber1 = strcmp(vCardObj[i].TelCell, strCLID);
		searchNumber2 = strcmp(vCardObj[i].TelHome, strCLID);
		searchNumber3 = strcmp(vCardObj[i].TelWork, strCLID);
		searchNumber4 = strcmp(vCardObj[i].TelOther, strCLID);

		if((searchNumber1 == 0) || (searchNumber2 == 0) || (searchNumber3 == 0) || (searchNumber4 == 0))
		{
			PB_name = (unsigned short *)vCardObj[i].DrawName;
			*CharSetEncoding = vCardObj[i].CharSet; 
			return PB_name;
		}
	}

	return NULL;
#endif
}

/*****************************************************************************
*
* Phonebook common functions - vCard parser utils
*
*****************************************************************************/
VcardDataModify * BtApp_PB_ModifyVcard(TcBtUint8 * buffer, TcBtUint32 bufferSize)
{
	TcBtUint8*   tempStart = NULL;
	TcBtUint8*   tempEnd = NULL;
	TcBtUint8*   pTempStart = NULL; /* previous point of tempStart */
	VcardDataModify *vcardDataModify;
	TcBtUint32 loop = bufferSize;

	vcardDataModify = (VcardDataModify *) TC_BT_Pmalloc(sizeof(VcardDataModify));

	vcardDataModify->newbufferSize = 0;
	vcardDataModify->pch = NULL;
	
    tempStart =(TcBtUint8 *)BtApp_PB_CmpStr(buffer, (TcBtUint8 *)"=\r\n", bufferSize);
	pTempStart = buffer;

	while(tempStart !=NULL)
	{	
		if(((TcBtUint32)tempStart-(TcBtUint32)pTempStart) < loop)
		{
			tempEnd = tempStart+3;
            (void)strcpy((char *)tempStart, (char *)tempEnd);
			bufferSize -= 3;
			loop = ((TcBtUint32)buffer + bufferSize) - (TcBtUint32)tempStart;
			pTempStart = tempStart;
	 	}
		else
		{
			break;
		}
		
        tempStart = (TcBtUint8*)BtApp_PB_CmpStr(tempStart, (TcBtUint8 *)"=\r\n", (int)loop);
	}
	vcardDataModify->newbufferSize = bufferSize;
	vcardDataModify->pch = buffer;
	
	return vcardDataModify;
}

void BtApp_PB_DataModifypfree(VcardDataModify *DataModify)
{
	if(DataModify != NULL)
	{
		TC_BT_Pfree(DataModify);
	}
}

TcBtUint32 BtApp_PB_vCardFunction(TcBtUint8* vCardData, TcBtUint32 vCardBufferSize)
{
#ifndef USE_BTBR
    VcardDataModify* DataModify;
#endif
    TcBtUint32 pbvcfBufferSize=0;
    TcBtUint32   errorCode=0;
#ifndef USE_BTBR
	unsigned int TC_BT_bchsVcardDataEntries = 0;
#endif

   	pbvcfBufferSize = vCardBufferSize;
#ifdef USE_BTBR
#if 0 //for Linux
#else
	errorCode = TcBtVcardParse(vCardData, pbvcfBufferSize, &PBAP_vCardData, 
                TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES|
                TC_BT_VCARD_FLAGS_NO_MANDATORY_TAGS | TC_BT_VCARD_FLAGS_NO_VERSION_MATCH);
#endif
#else
    if(BtApp_PB_CheckvCardCRLF(vCardData,pbvcfBufferSize) == TRUE)
    {
        DataModify = BtApp_PB_ModifyVcard(vCardData,pbvcfBufferSize);		                  
#if 0 //for Linux
#else
        errorCode = TcBtVcardParse(DataModify->pch, DataModify->newbufferSize, &TC_BT_bchsVcardDataEntries, &PBAP_vCardData, 
                TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES|
                TC_BT_VCARD_FLAGS_NO_MANDATORY_TAGS | TC_BT_VCARD_FLAGS_NO_VERSION_MATCH);	
#endif
        BtApp_PB_DataModifypfree(DataModify);
    }
    else
    {
#if 0 //for Linux
#else
        errorCode = TcBtVcardParse(vCardData, pbvcfBufferSize, &TC_BT_bchsVcardDataEntries, &PBAP_vCardData, 
                TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES|TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES|
                TC_BT_VCARD_FLAGS_NO_MANDATORY_TAGS | TC_BT_VCARD_FLAGS_NO_VERSION_MATCH);
#endif
    }
#endif
    return errorCode;
}

char *BtApp_PB_findVcardPrefix(TcBtUint8 *vcf, int size)
{
    TcBtUint8 *ret = NULL;
    const TcBtUint8 *VCARD_PREFFIX = (TcBtUint8 *)"BEGIN:VCARD";

    ret = (TcBtUint8 *)BtApp_PB_CmpStr((TcBtUint8 *)vcf, VCARD_PREFFIX, size);
    if(ret != NULL)
    {
        return((char *)ret);
    }
    else
    {
        return(NULL);
    }

}

char *BtApp_PB_findVcardLastSuffix(TcBtUint8 *vcf, int size)
{
    TcBtUint8 *ret = NULL;
    const TcBtUint8 *VCARD_SUFFIX = (TcBtUint8 *)"END:VCARD";

    ret = (TcBtUint8 *)BtApp_PB_StrStr(vcf, VCARD_SUFFIX, size);

    if(ret != NULL)
    {
        return(char *)(ret + 9);
    }
    else
    {
        return(NULL);
    }

}

char *BtApp_PB_findVcardSuffix(TcBtUint8 *vcf, int size)
{
    TcBtUint8 *ret = NULL;
    const TcBtUint8 *VCARD_SUFFIX = (TcBtUint8 *)"END:VCARD";

    ret = (TcBtUint8 *)BtApp_PB_CmpStr(vcf, VCARD_SUFFIX, size);
    if(ret != NULL)
    {
        return(char *)(ret + 9); /* Return the first letter of the next vcard */
    }
    else
    {
        return(NULL);
    }

}

#ifndef VCARD_DIR_NUM
#if defined(TCC801X)
#define VCARD_DIR_NUM 				9
#elif defined(TCC80XX)
#define VCARD_DIR_NUM 				7
#elif defined(TCC860x)
#define VCARD_DIR_NUM 				2
#endif
#endif

unsigned char BtApp_PB_vcfDump(TcBtUint8* vcfData, TcBtUint32 vcfSize)
{
	int handle = 0;
#if 1 //for Linux
#else
	/* Create folder */
	{

		if (TC_Set_Current_Dir(g_Part_ID, "\\", VCARD_DIR_NUM)!=VCARD_DIR_NUM)
		{
			printf("BtApp_PB_vcfDump : Can't create folder.\n");
			return FALSE;
		}

		if (TC_Set_Current_Dir(g_Part_ID, (char *)"VCARD", VCARD_DIR_NUM)!=VCARD_DIR_NUM)
		{
			/* DMP Don't call TCCUI - Needs DirectSndMsg */
			//TccUIPopUp_MsgBox(2, "Check AEC Dir", "Error");

			if (TC_Make_Dir("VCARD", VCARD_DIR_NUM)<0)
			{
				printf("BtApp_PB_vcfDump : Can't create folder.\n");
				return FALSE;
			}

			(void) TC_Refresh(VCARD_DIR_NUM);

			if (TC_Set_Current_Dir(g_Part_ID, (char *)"VCARD", VCARD_DIR_NUM)!=VCARD_DIR_NUM)
			{
				printf("BtApp_PB_vcfDump : Can't create folder.\n");
				
				return FALSE;
			}
		}
	}
	
	TC_SchedLock();
	
	/* TC_TimeDly(1); */
	
	/* Create files */
	if(gBTPBVcardDumpFirstFlag == 0)
	{
		handle = TC_Open("vCard_Dump.txt", TC_O_CREAT | TC_O_RDWR, TC_A_WRITE, TC_A_ARCHIVE, VCARD_DIR_NUM);
		printf("vCard DUMP START!\n");
		gBTPBVcardDumpFirstFlag++;
	}
	else
	{
		handle = TC_Open("vCard_Dump.txt", TC_O_RDWR | TC_O_APPEND, TC_A_WRITE, TC_A_ARCHIVE, VCARD_DIR_NUM);
	}
	
	if ( handle <= 0 )
	{
		return FALSE;
	}
	if(TRUE) 
	{
		(void) TC_Write(handle, vcfData, vcfSize);
	}
	
	//(void) TC_Write(handle, "test2", 5);
	//TC_Flush(handle);
	(void) TC_Close(handle);
	(void) TC_Refresh(VCARD_DIR_NUM);
	
#if 0
	for(iDumpIdx=0;iDumpIdx<AEC_DUMP_MAX_NUM_OF_FILE;iDumpIdx++)
	{
		//(void)sprintf(fileName,"%c%c%c_*.???",acDumpFileName[iDumpIdx][0],acDumpFileName[iDumpIdx][1],acDumpFileName[iDumpIdx][2]);
		
		count = FSAPP_GetAvailOffset((unsigned char *)acDumpFileName[iDumpIdx],AEC_DIR_NUM) + 1;
		
		//(void)sprintf(fileName,"%c%c%c_%03d.BIN",acDumpFileName[iDumpIdx][0],acDumpFileName[iDumpIdx][1],acDumpFileName[iDumpIdx][2],count);
		acDumpFileName[iDumpIdx][4]=(char)(((count%1000)/100)+'0');
		acDumpFileName[iDumpIdx][5]=(char)(((count%100)/10)+'0');
		acDumpFileName[iDumpIdx][6]=(char)((count%10)+'0');

		iDumpErr = (AEC_DUMP_File(pAEC,acDumpFileName[iDumpIdx],pAEC->pcDump[iDumpIdx],pAEC->ulDumpIdx[iDumpIdx], pAEC->boDumpOver[iDumpIdx])!=AEC_ERR_NOERR);
			
		if ((pAEC->pcDump[iDumpIdx]!=0)
			&& iDumpErr)
		{
			TC_SchedUnlock();
			printf("[pAEC.c : %06d,%06d] Info : AEC_DUMP_Execute : End : Can't create a file.",__LINE__,BT_HF_CALL_TIMER);
			return AEC_ERR_FAILED;
		}
	}
#endif

	TC_TimeDly(200);
	TC_SchedUnlock();
#endif
	return TRUE;
}

void BtApp_PB_vcfParsing(TcBtUint8* vcfData, TcBtUint32 vcfSize, PHONEBOOK_INST_TYPE *PbookInst)
{		
    TcBtUint8*   	pch;
    TcBtUint32      pbvcfBufferSize;
    TcBtUint32   	errorCode=0;
    TcBtUint8*   	vcardEndPtr = NULL;
    TcBtUint8*   	vcardStartPtr = NULL;
    int 			vcardlen = 0, offset = 0, errNum = 0;
    int				CallHistoryfolder = select_callhistory[BTAPP_PB_STORAGE];

    pbvcfBufferSize = vcfSize;

    if(pbvcfBufferSize != FALSE)
    {
        while(pbvcfBufferSize > 0)
        {
            vcardEndPtr = (TcBtUint8 *)BtApp_PB_findVcardSuffix(&vcfData[offset], pbvcfBufferSize);

            if(vcardEndPtr == NULL)
            {
                break;
            }

            vcardStartPtr = (TcBtUint8 *)BtApp_PB_findVcardPrefix(&vcfData[offset], pbvcfBufferSize);
            vcardlen = (unsigned int)vcardEndPtr - (unsigned int)vcardStartPtr;

            if((vcardStartPtr) && (vcardlen > 0))
            {
            	PbookInst->receivedvCardNum++;
                pch = (TcBtUint8 *)TC_BT_Pmalloc(vcardlen+1);
                memset(pch, 0, vcardlen+1);
                memcpy(pch, vcardStartPtr, vcardlen);
                errorCode = 0;
                errorCode = BtApp_PB_vCardFunction(pch, vcardlen);
#ifdef USE_BTBR
				if(PBAP_vCardData != NULL)
#else
                if(TC_BT_VCARD_SUCCEEDED(errorCode) || (PBAP_vCardData != NULL))
#endif
                {
                	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* vCard Entry */
                	{
						if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_PB) /* contacts */
	                	{
							(void)BtApp_PB_AddPhonebook(PBAP_vCardData);
                            BtApp_PB_FindVcardCharSet(pch, PbookInst->vCardObjNum[PbookInst->deviceId]);
	                	}
	                	else /* call history */
	                	{
							BtApp_PB_AddCallHistory(PBAP_vCardData);
							BtApp_PB_FindCallHistoryCharSet(pch, PbookInst->CallHistory[CallHistoryfolder][0].vCardNum);
	                	}
                	}
                	
                	else /* entire vCard download */
                	{
						if(CallHistoryfolder == 4)
	                    {
	                        if(PbookInst->vCardObjNum[PbookInst->deviceId] < TCC_MAX_VCARD_NUMBER)
	                        {
	                            if(BtApp_PB_AddPhonebook(PBAP_vCardData) == TRUE)
	                            {
	                                BtApp_PB_FindVcardCharSet(pch, PbookInst->vCardObjNum[PbookInst->deviceId]);
	                                PbookInst->vCardObjNum[PbookInst->deviceId]++;
	                            }
	                        }
		             	}
			            else
			            {
			            	if((PbookInst->CallHistory[CallHistoryfolder][0].vCardNum) < TCC_MAX_CALLHISTORY_NUMBER)
			            	{
					          BtApp_PB_AddCallHistory(PBAP_vCardData);
                              BtApp_PB_FindCallHistoryCharSet(pch, PbookInst->CallHistory[CallHistoryfolder][0].vCardNum);
				             (PbookInst->CallHistory[CallHistoryfolder][0].vCardNum)++;
							}
						}
                	}
				}	
                else
                {
					errNum++;
					PbookInst->parsingErrvCardNum++;
                }
#if 1 //for Linux
#else
                TcBtVcardFree(PBAP_vCardData);
#endif
			
                PBAP_vCardData = NULL;

                if(pch != NULL)
                {	
                	TC_BT_Pfree(pch);	
                }

            }
            pbvcfBufferSize -= ((int)vcardEndPtr - (int)(&vcfData[offset]));
            offset += ((int)vcardEndPtr - (int)(&vcfData[offset]));
        }
    }

	printf("=== vCard Parsing Result! ===\n");
    printf("Total vCard count : %d\n", PbookInst->vCardObjNum[PbookInst->deviceId]);
    printf("Received vCard count : %d\n", PbookInst->receivedvCardNum);
    printf("Parsing error count : %d\n", PbookInst->parsingErrvCardNum);
    printf("No number vCard error count : %d\n", PbookInst->noNumbervCardNum);
}

/*****************************************************************************
*
* Phonebook common functions - vCard list parser utils
*
*****************************************************************************/
unsigned char BtApp_PB_checkSpace(int c)
{
  switch (c) {
  case 0x20:
  case 0xD:
  case 0xA:
  case 0x9:
    return 1;
  }
  return 0;
}

void BtApp_PB_GetAttributeValue(char *element, TcBtUint16 length, char **value)
{
    TcBtUint16 i;

    i = 0;
    while((i < length) && (element[i] != PBAP_XML_VALUE_DELIMITER ))
    {
        i++;
    }
    if(i < length)
    {
        i++;
        *value = element + i;
        while((i < length) && (element[i] != PBAP_XML_VALUE_DELIMITER ))
        {
            i++;
        }
        element[i] = '\0';
    }
    else
    {
        *value = NULL;
    }
}

char* BtApp_PB_vCardListNameConversion(const char* name)
{
/* name field format : LastName;FirstName;MiddleName;Prefix;Suffix */

	char* lastName = NULL;
	char* firstName = NULL;
	char* middleName = NULL;
	char* retName = NULL;

	int i = 0, j, index = 0, nameLength = 0, lastNameLength = 0, firstNameLength = 0, middleNameLength = 0;
	int delimiterCnt = 1;

	if(name == NULL) /* Don't need to convert */
	{
		return NULL;
	}

	nameLength = strlen(name);

	if(nameLength == 0)
	{
		return NULL;
	}
	else
	{
        lastName = (char* )TC_BT_Pmalloc(nameLength+1);
        firstName = (char* )TC_BT_Pmalloc(nameLength+1);
        middleName = (char* )TC_BT_Pmalloc(nameLength+1);
        retName = (char* )TC_BT_Pmalloc(nameLength+1);

		memset(lastName, 0x00, nameLength+1);
		memset(firstName, 0x00, nameLength+1);
		memset(middleName, 0x00, nameLength+1);
		memset(retName, 0x00, nameLength+1);
	
		for(j = 0; j < nameLength; j++)
		{
			if(name[j] == ';')
			{
				delimiterCnt++;
			}
		}

	/* search Last name */
		if(delimiterCnt != 0)
		{
			while(name[i] != ';')
			{
				lastName[index] = name[i];
				index++;
				i++;
				if(i >= nameLength)
				{
					break;
				}
			}
			lastName[index] = NULL;
			index = 0;
			i++;
			delimiterCnt--;
		}

	/* search First name */
		if(delimiterCnt != 0)
		{
			while(name[i] != ';')
			{
				firstName[index] = name[i];
				index++;
				i++;
				if(i >= nameLength)
				{
					break;
				}
			}
			firstName[index] = NULL;
			index = 0;
			i++;
			delimiterCnt--;
		}

	/* search Middle name */
		if(delimiterCnt != 0)
		{
			while(name[i] != ';')
			{
				middleName[index] = name[i];
				index++;
				i++;
				if(i >= nameLength)
				{
					break;
				}
			}
			middleName[index] = NULL;
			index = 0;
			delimiterCnt--;
		}

	/* conversion name */
		if(gBTPBAPSelectSorting == 0)
		{
			if(firstName != NULL)
			{
				firstNameLength = strlen(firstName);
			}
			if(firstNameLength != 0)
			{
				memcpy(&retName[index], firstName, firstNameLength);
				index += firstNameLength;
			}

			if(middleName != NULL)
			{
				middleNameLength = strlen(middleName);
			}
			if((middleNameLength != 0) && ((nameLength - index - middleNameLength) >= 0))
			{
				if(index != 0)
				{
					retName[index] = 0x20; /* include space */
					index++;
				}
				memcpy(&retName[index], firstName, middleNameLength);
				index += middleNameLength;
			}

			if(lastName != NULL)
			{
				lastNameLength = strlen(lastName);
			}
			if((lastNameLength != 0)  && ((nameLength - index - lastNameLength) >= 0))
			{
				if(index != 0)
				{
					retName[index] = 0x20; /* include space */
					index++;
				}
				memcpy(&retName[index], lastName, lastNameLength);
				index += lastNameLength;
			}

			retName[index] = NULL;
		}
		else
		{
			if(lastName != NULL)
			{
				lastNameLength = strlen(lastName);
			}
			if(lastNameLength != 0)
			{
				memcpy(&retName[index], lastName, lastNameLength);
				index += lastNameLength;
			}

			if(middleName != NULL)
			{
				middleNameLength = strlen(middleName);
			}
			if((middleNameLength != 0)  && ((nameLength - index - middleNameLength) >= 0))
			{
				if(index != 0)
				{
					retName[index] = 0x20; /* include space */
					index++;
				}
				memcpy(&retName[index], firstName, middleNameLength);
				index += middleNameLength;
			}

			if(firstName != NULL)
			{
				firstNameLength = strlen(firstName);
			}
			if((firstNameLength != 0)  && ((nameLength - index - firstNameLength) >= 0))
			{
				if(index != 0)
				{
					retName[index] = 0x20; /* include space */
					index++;
				}
				memcpy(&retName[index], firstName, firstNameLength);
				index += firstNameLength;
			}

			retName[index] = NULL;
		}
	}

	if(firstName != NULL)
	{
		TC_BT_Pfree(firstName);
	}
	if(lastName != NULL)
	{
		TC_BT_Pfree(lastName);
	}
	if(middleName != NULL)
	{
		TC_BT_Pfree(middleName);
	}

	return retName;
}

void BtApp_PB_vCardListParseEntry(TcBtUint8* entry, TcBtUint16 length)
{
	TcBtUint32 i;
   	char *size;
    char *name;
    char *handle;
    char *retName;

    int lengthOfHandle = 0;
    int lengthOfName = 0;

	TcBtUint8 checkAtt = 0, checkSpace = 0;
    char noName[8] = "No Name";

    i = 0;
    size = NULL;
    name = NULL;
    handle = NULL;
    retName = NULL;

	checkSpace = BtApp_PB_checkSpace(entry[i]);
    while((i < length) && (checkSpace == TRUE))
    {
        i++;
    }
    entry[i] = '\0';
    while((i < length) && ((PhoneBookList.numOfHandle < TCC_MAX_VCARD_LIST_NUMBER) || (PhoneBookList.numOfName < TCC_MAX_VCARD_LIST_NUMBER)))
    {
        /* Find Message Handle entry */
        checkAtt = strncmp((char *)(entry + i), "handle", 6);
        if(((i + 6) < length) && ((!checkAtt) != FALSE))
        {
            i+=6;
            BtApp_PB_GetAttributeValue((char *)(entry + i), (TcBtUint16)(length - i), &handle);
            if(PhoneBookList.numOfHandle < TCC_MAX_VCARD_LIST_NUMBER)
            {
	            if(handle != NULL)
	            {
		            lengthOfHandle = strlen(handle);
		            lengthOfHandle = ((lengthOfHandle >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : lengthOfHandle);
		            memcpy(PhoneBookList.vCardListObj[PhoneBookList.numOfHandle].handle, handle, lengthOfHandle);
		            PhoneBookList.vCardListObj[PhoneBookList.numOfHandle].handle[lengthOfHandle] = NULL;
		            PhoneBookList.numOfHandle++;
		        }
	        }
        }
        /* Find subject entry */
        else if((i + 4) < length)
        {
            if((!strncmp((char *)(entry + i), "name", 4)) != FALSE)
        	{
	            i+=4;
                BtApp_PB_GetAttributeValue((char *)(entry + i), (TcBtUint16)(length - i), &name);

	            if(PhoneBookList.numOfName < TCC_MAX_VCARD_LIST_NUMBER)
	            {
					if(name != NULL)
					{
			            retName = BtApp_PB_vCardListNameConversion(name); /* name parameter needs to be converted */

			            if(retName != NULL)
			            {
				            lengthOfName = strlen(retName);
				            lengthOfName = ((lengthOfName >= (DRAW_NAME_SIZE-1)) ? (DRAW_NAME_SIZE-1) : lengthOfName);
				            if(lengthOfName == 0)
				            {
								memcpy(PhoneBookList.vCardListObj[PhoneBookList.numOfName].name, noName, 8);
								PhoneBookList.vCardListObj[PhoneBookList.numOfName].name[7] = NULL;
				            }
				            else
				            {
								memcpy(PhoneBookList.vCardListObj[PhoneBookList.numOfName].name, retName, lengthOfName);
								PhoneBookList.vCardListObj[PhoneBookList.numOfName].name[lengthOfName] = NULL;
				            }	            
							TC_BT_Pfree(retName);
						}
						else
						{
							memcpy(PhoneBookList.vCardListObj[PhoneBookList.numOfName].name, noName, 8);
							PhoneBookList.vCardListObj[PhoneBookList.numOfName].name[7] = NULL;
						}
						PhoneBookList.numOfName++;
			        }
		        }
			}
        }
        else
		{
			/* TO DO */
		}
        /* Continue scanning */
        i++;
    }

    /* Make sure size is present */
    if(size == NULL)
    {
        size = "0";
    }
}

void BtApp_PB_vCardListParsing(TcBtUint8* vcfListData, TcBtUint32 vcfListSize)
{
	TcBtUint32 index;
    TcBtUint32 start;
    TcBtBool found;
    TcBtUint8 checkAtt = 0;

    /* Dummy checking */
    if((vcfListData == NULL) ||
       (vcfListSize == 0))
    {
        return;
    }

    index = 0;
    start = 0;
    found = FALSE;
    while(index < vcfListSize)
    {
        if(!found)
        {
            start = index;

            /* Scan for vCard element */
            checkAtt = memcmp(vcfListData + index, PBAP_VCARD_HANDLE_ATTR, (sizeof(PBAP_VCARD_HANDLE_ATTR)-1));
            if(((index + (sizeof(PBAP_VCARD_HANDLE_ATTR)-1)) < vcfListSize) && (checkAtt == 0))
            {
                found = TRUE;
                index += (sizeof(PBAP_VCARD_HANDLE_ATTR)-1);
            }

            /* Continue scanning buffer */
        	index++;
        }
        /* Search for end of element */
        else
        {
        	checkAtt = memcmp(vcfListData + index, PBAP_VCARD_LIST_END_ATTR, (sizeof(PBAP_VCARD_LIST_END_ATTR)-1));
            if(((index + (sizeof(PBAP_VCARD_LIST_END_ATTR)-1)) < vcfListSize) && (checkAtt == 0))
            {
                char *raw;
                char *ascii;

                /* Calculate end */
                raw =(char *) (vcfListData + start);
                index += (sizeof(PBAP_VCARD_LIST_END_ATTR)-1);

                /* Parse entry */
                ascii = (char *)TC_BT_Pmalloc((index - start) + 1);
                (void)strncpy(ascii, raw, index - start);
                ascii[index - start] = '\0';
                BtApp_PB_vCardListParseEntry((TcBtUint8 *)ascii,
                                  (TcBtUint16)(index - start));
                TC_BT_Pfree(ascii);

                /* Cull buffer */
                raw =  (char *)TC_BT_Pmalloc(vcfListSize - index);
                memcpy(raw,
                       (vcfListData + index),
                       vcfListSize - index);
                TC_BT_Pfree(vcfListData);
                vcfListData= (TcBtUint8 *)raw;
                vcfListSize -= index;
                //inst->message_idx -= index;

                /* Since buffer was culled, start from beginning */
                found = FALSE;
                index = 0;
            }
            else
            {
				/* Continue scanning buffer */
        		index++;
            }
        }
    }
}

/*****************************************************************************
*
* Phonebook common functions - phonebook sorting utils
*
*****************************************************************************/
void BtApp_PB_ContactsSortingHandler(void)
{
    TcBtPacParsing* pTcBtPacParsing = (TcBtPacParsing *)TC_BT_Pmalloc(sizeof(TcBtPacParsing));

	pTcBtPacParsing->service = BT_VCARD_SORT;
#if 1 //for Linux
#else
	CMD_BluetoothSysContactsThreadSyncFnc(pTcBtPacParsing);
#endif
}

void BtApp_PB_vCardSortingThread(unsigned short currentStatus)
{
	PHONEBOOK_INST_TYPE *PbInstance = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_LIST_TYPE *PbList = BtApp_PB_GetPhonebookList();
	int i, j = 0, maxCnt = 0;
	
	VCARD_OBJ_TYPE *vCardObj;
	VCARD_OBJ_TYPE vCardTmp;

	VCARD_LIST_INST *vCardListObj;
	VCARD_LIST_INST vCardListTmp;

	if(currentStatus == BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM)
	{
		maxCnt = PbList->numOfName;
		vCardListObj = PbList->vCardListObj;

		for(i=0; i<(maxCnt-1); i++)
		{
			for(j = i+1; j < maxCnt; j++)
			{
				if(strcmp(vCardListObj[i].name, vCardListObj[j].name) > 0)
				{
					vCardListTmp = vCardListObj[i];
					vCardListObj[i] = vCardListObj[j];
					vCardListObj[j] = vCardListTmp;
				}
			}
		}
	}
	else if((currentStatus == BTAPP_PB_STATUS_OPS_PUT_IND)
		||  (currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_CFM)
		||  (currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_ERR))
	{
		maxCnt = PbInstance->vCardObjNum[PbInstance->deviceId];
		vCardObj = &(PbInstance->vCardObj[PbInstance->deviceId][0]);

		for(i=0; i<(maxCnt-1); i++)
		{
			for(j = i+1; j < maxCnt; j++)
			{
                if(strcmp((char *)vCardObj[i].DrawName, (char *)vCardObj[j].DrawName) > 0)
				{
					vCardTmp = vCardObj[i];
					vCardObj[i] = vCardObj[j];
					vCardObj[j] = vCardTmp;
				}
			}
		}
	}
	else
	{
		/* TO DO */
	}
}

void BtApp_PB_PbSortingThread(void  *ptrInst)
{
	TcBtPacParsing* pbInst = (TcBtPacParsing *)ptrInst;
	PHONEBOOK_INST_TYPE *PbInstance = BtApp_PB_GetPhonebookInstance();
	int MaxNum = PbInstance->vCardObjNum[PbInstance->deviceId];
	int i, j;
	VCARD_OBJ_TYPE *obj;
	VCARD_OBJ_TYPE tmp;

	obj = &(PbInstance->vCardObj[PbInstance->deviceId][0]);

	for(i=0; i<(MaxNum-1); i++)
	{
		for(j = i+1; j < MaxNum; j++)
		{
            if(strcmp((char *)obj[i].DrawName, (char *)obj[j].DrawName) > 0)
			{
				tmp = obj[i];
				obj[i] = obj[j];
				obj[j] = tmp;
			}
		}
	}

	if(pbInst != NULL)
	{
		if(pbInst->currentStatus == BTAPP_PB_STATUS_ATC_PULLPB_CFM)
		{
#if 1 //for Linux
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_ATC_PULLPB_CFM);
#endif
		}
		
		if(pbInst->currentStatus == BTAPP_PB_STATUS_ATC_PULLPB_ERR)
		{
#if 1 //for Linux
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_ATC_PULLPB_ERR);
#endif
		}

		TC_BT_Pfree(pbInst);
	}
}

/*****************************************************************************
*
* Phonebook common functions - phonebook common process utils
*
*****************************************************************************/
void BtApp_PB_ContactsBgThread(void *ptr)
{
	TcBtPacParsing* pbInst = (TcBtPacParsing *)ptr;

	void (*fnPhonebookThread[])(void *) = {
	BtApp_PB_vCardBgThread,
	BtApp_PB_PbSortingThread,
 	}; /* parsing & sorting thread */

	fnPhonebookThread[pbInst->service](ptr);
}

void BtApp_PB_vCardBgThread(void  *ptrInst)
{
	TcBtPacParsing* pbInst = (TcBtPacParsing *)ptrInst;
	PHONEBOOK_INST_TYPE *PbInstance = BtApp_PB_GetPhonebookInstance();
	unsigned short currentStatus = 0;
	unsigned char* vCardListData = NULL;

	if(pbInst != NULL)
	{
		currentStatus = pbInst->currentStatus;
		pbInst->currentStatus = 0;

		if((currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_IND)
			|| (currentStatus == BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM)
			|| (currentStatus == BTAPP_PB_STATUS_OPS_PUT_IND)
			|| (currentStatus == BTAPP_PB_STATUS_SML_PARSE_CFM)) /* current status is vCard parsing */
		{
			BtApp_PB_vcfParsing(pbInst->payload, pbInst->payloadLength, PbInstance); /* vCard parsing */

                    if((select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD)
				&& (select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_PB)
				&& (PbInstance->vCardObjNum[PbInstance->deviceId] != 0)
				&& (currentStatus != BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM))
			{
				BtApp_PB_vCardSortingThread(currentStatus);
			}
		}
		else if((currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_CFM) /* PBAP */
			|| (currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_ERR))
		{
			if((select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD)
				&& (select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_PB)
				&& (PbInstance->vCardObjNum[PbInstance->deviceId] != 0))
			{
				BtApp_PB_vCardSortingThread(currentStatus);
			}
		}
		else if(currentStatus == BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM)
		{
            vCardListData = (unsigned char *)TC_BT_Pmalloc(pbInst->payloadLength);
			if(vCardListData != NULL)
			{
				memcpy(vCardListData, pbInst->payload, pbInst->payloadLength);
			}
			BtApp_PB_vCardListParsing(vCardListData, pbInst->payloadLength);
			if(select_callhistory[BTAPP_PB_STORAGE] != BTAPP_PB_SELECT_PB)
			{
				/* Call history don't need sorting */
			}
			else
			{
				if(gBTPBAPSelectSorting == 0) /* First name sorting */
				{
					BtApp_PB_vCardSortingThread(currentStatus);
				}
				else /* Last name sorting */
				{
					/* Don't need sorting */
				}
			}

			if(vCardListData != NULL)
			{
				TC_BT_Pfree(vCardListData);
				vCardListData = NULL;
			}
		}
		else
		{
			/* TO DO */
			pbInst->payload = NULL;
		}

		if(pbInst->payload != NULL)
		{
			TC_BT_Pfree(pbInst->payload);
			pbInst->payloadLength = 0;
	       	pbInst->payload = NULL;
		}

		TC_BT_Pfree(pbInst);
	}

	if(currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_CFM) /* current profile is PBAP(PULL_PB_CFM) */
	{
		if(select_callhistory[BTAPP_PB_STORAGE] != BTAPP_PB_SELECT_PB)
		{
#if 1 //for Linux
                    BtApp_PB_EvtPACPullCHCfm();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLCH_CFM);
#endif
		}
		else
		{
#if 1 //for Linux
                    BtApp_PB_EvtPACPullPBCfm();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_CFM);
#endif
			memset(&CurrentCallEntryPBData, 0, sizeof(Call_Entry_PB_Data));
		}

        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD);
	}
       else if(currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_IND) /* current profile is PBAP(PULL_PB_IND) */
	{
		if(select_callhistory[BTAPP_PB_STORAGE] != BTAPP_PB_SELECT_PB)
		{
#if 1 //for Linux
                    BtApp_PB_EvtPACPullCHInd();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLCH_IND);
#endif
		}
		else
		{
#if 1 //for Linux
                    BtApp_PB_EvtPACPullPBInd();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_IND);
#endif
		}
	}

	else if(currentStatus == BTAPP_PB_STATUS_PAC_PULLPB_ERR)
	{
		if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_PB)
		{
			if(PbInstance->vCardObjNum[PbInstance->deviceId] != 0)
			{
				memset(&CurrentCallEntryPBData, 0, sizeof(Call_Entry_PB_Data));
			}
#if 1 //for Linux
                    BtApp_PB_EvtPACPullPBErr();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLPB_ERR);
#endif
		}
		else
		{
#if 1 //for Linux
                    BtApp_PB_EvtPACPullCHErr();
#else
			DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLCH_ERR);
#endif
		}
        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD);
	}
	else if(currentStatus == BTAPP_PB_STATUS_PAC_PULLVCARDLIST_CFM) /* current profile is PBAP(PULL_VCARD_LIST_CFM) */
	{
#if 1 //for Linux
            BtApp_PB_EvtPACPullVcardListCfm();
#else
		DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLVCARDLIST_CFM); /* contacts */
#endif

        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_LIST);
	}
	else if(currentStatus == BTAPP_PB_STATUS_PAC_PULLVCARDENTRY_CFM) /* current profile is PBAP(PULL_VCARD_ENTRY_CFM) */
	{
#if 1 //for Linux
            BtApp_PB_EvtPACPullVcardEntryCfm();
#else
		DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_PAC_PULLVCARDENTRY_CFM); /* contacts */
#endif

        (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_ENTRY);
	}
	else if(currentStatus == BTAPP_PB_STATUS_OPS_PUT_IND) /* current profile is OPP(PUT) */
	{
	}

	else if(currentStatus == BTAPP_PB_STATUS_OPS_DISCONNECT_IND) /* current profile is OPP(DISCONNECT) */
	{
	}
	else if(currentStatus == BTAPP_PB_STATUS_SML_PARSE_CFM) /* current profile is syncML parsing complete */
	{
	}
	else /* current profile is OPP(PUT) || current profile is PBAP(PULL_PB_IND)*/
	{
		/* TO DO */
	}
}

/*****************************************************************************
*
* PBAP common functions - PBAP common process utils
*
*****************************************************************************/
//void TC_BT_PAC_SetCallHistoryFolder(int callhistory)
void BtApp_PB_SetCallHistoryFolder(int callhistory)
{
	select_callhistory[BTAPP_PB_STORAGE] = callhistory;
}

void BtApp_PB_PacConnectHandler(TcBtUint8 source) /* pull PB */
{
#ifdef BT_PBAP_MULTISERVER
    int devnum;
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    devnum = TC_BT_HF_DEV_GetNum(&PBInst->deviceAddr);

    if(devnum < 0)
    {
        return; /* device select fail */
    }
    else
    {
        select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
        select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_PB;
        PBInst->service = TC_BT_OBEX_PBA_SERVER_PROFILE_UUID;

        if((BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_PB_PAC_CONNECT, (unsigned int)devnum)) > 0))
        //&& (BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_PB_PAC_DOWNLOADED, (unsigned int)devnum)) == FALSE))
        {
            TC_BT_PAC_PullPhonePB(TCC_MAX_VCARD_NUMBER, (TcBtUint32)source);
        }
    }
#else
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

    select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
    select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_PB;

    PBInst->service = TC_BT_OBEX_PBA_SERVER_PROFILE_UUID;
#ifdef BT_PBAP_CLIENT
    (void)G_BtPacCmd->TC_BT_PAC_PullPhonePB(TCC_MAX_VCARD_NUMBER, (TcBtUint32)source);
#endif
#endif
    }

void BtApp_PB_PacConnectErrHandler(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	switch(PBInst->service)
	{
 		default:
			break;
	}
}

void BtApp_PB_SendCallhistoryDialled(void) /* pull OCH */
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	
	select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
	select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_OCH;

	gPBAPCHSelectedIndex = 0;

	if(PBInst->service == TC_BT_OBEX_PBA_SERVER_PROFILE_UUID)
	{
	#ifdef BT_PBAP_MULTISERVER
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT)))
	#else
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT) == TRUE)
	#endif
		{
#ifdef BT_PBAP_CLIENT
            (void)G_BtPacCmd->TC_BT_PAC_PullDialledCall(TCC_MAX_CALLHISTORY_NUMBER, TC_BT_PAC_SRC_PHONE);
#endif
		}
	}
	else if(PBInst->service == TC_BT_HF_PROFILE_UUID)
	{
	}
	else
	{
		return;
	}
}

void BtApp_PB_SendCallhistoryReceived(void) /* pull ICH */
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	
	select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
	select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_ICH;

	gPBAPCHSelectedIndex = 0;
	
	if(PBInst->service == TC_BT_OBEX_PBA_SERVER_PROFILE_UUID)
	{
	#ifdef BT_PBAP_MULTISERVER
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT)))
	#else
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT) == TRUE)
	#endif
		{
#ifdef BT_PBAP_CLIENT
            (void)G_BtPacCmd->TC_BT_PAC_PullReceivedCall(TCC_MAX_CALLHISTORY_NUMBER, TC_BT_PAC_SRC_PHONE);
#endif
		}
	}
	else if(PBInst->service == TC_BT_HF_PROFILE_UUID)
	{
	}
	else
	{
		return;
	}
}

void BtApp_PB_SendCallhistoryMissed(void) /* pull MCH */
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	
	select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
	select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_MCH;
	
	gPBAPCHSelectedIndex = 0;

	if(PBInst->service == TC_BT_OBEX_PBA_SERVER_PROFILE_UUID)
	{
	#ifdef BT_PBAP_MULTISERVER
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT)))
	#else
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT) == TRUE)
	#endif
		{
#ifdef BT_PBAP_CLIENT
            (void)G_BtPacCmd->TC_BT_PAC_PullMissedCall(TCC_MAX_CALLHISTORY_NUMBER, TC_BT_PAC_SRC_PHONE);
#endif
		}
	}
	else if(PBInst->service == TC_BT_HF_PROFILE_UUID)
	{
	}
	else
	{
		return;
	}
}

void BtApp_PB_SendCallhistoryCombined(void) /* pull CCH */
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	
	select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD;
	select_callhistory[BTAPP_PB_STORAGE] = BTAPP_PB_SELECT_CCH;
	
	gPBAPCHSelectedIndex = 0;

	if(PBInst->service == TC_BT_OBEX_PBA_SERVER_PROFILE_UUID)
	{
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT) == TRUE)
		{
#ifdef BT_PBAP_CLIENT
            (void)G_BtPacCmd->TC_BT_PAC_PullCombinedCall(TCC_MAX_CALLHISTORY_NUMBER, TC_BT_PAC_SRC_PHONE);
#endif
		}
	}
	else if(PBInst->service == TC_BT_HF_PROFILE_UUID)
	{
	}
	else
	{
		return;
	}
}

void BtApp_PB_RoleDisconnect(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	switch(PBInst->service)
	{
		case TC_BT_OBEX_PBA_SERVER_PROFILE_UUID:
#ifdef BT_PBAP_CLIENT
 			BtApp_PB_PacDisconnect();
#endif
 			break;
			
		case TC_BT_HF_PROFILE_UUID:
			break;

#if 0//def BT_SPP_INCLUDE			
		case TCC_SPP_PROFILE_UUID:
 			Bluetooth_SPP_Disconnect();

 			break;
#endif
 		default :

			break;
	}
}

void BtApp_PB_PacDisconnect(void)
{
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT) == TRUE)
	{
		PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
		PBInst->service = NULL;
#ifdef BT_PBAP_CLIENT
		BtApp_GAP_PacDisconnect(&PBInst->deviceAddr);
#endif
	}
}

TcBtBdAddr *BtApp_PB_FindConnectingBdAddr(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	return(&PBInst->deviceAddr);
}

void BtApp_PB_SetConnectingBdAddr(const TcBtBdAddr *SrvAddr)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	TC_BT_BdAddrCopyIfExist(&PBInst->deviceAddr, (const TcBtBdAddr *)SrvAddr);
}

#ifdef BT_PBAP_MULTISERVER
int BtApp_PB_SetDeviceId(unsigned int stat)
{
	int DevNum = 0;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	if(stat == BT_PB_EVT_PAC_CONNECT_CFM)
	{
		DevNum = TC_BT_HF_DEV_GetNum(&PBInst->deviceAddr);

		if(DevNum >= 0)
		{
			PBInst->deviceId = DevNum;
		}
		else
		{
			PBInst->deviceId = 0;
			DevNum = PBInst->deviceId;
		}
	}

	return(DevNum);
}

int BtApp_PB_GetDeviceId(void)
{
	int DevNum;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	DevNum = PBInst->deviceId; 
	if(DevNum < 0)
	{
		DevNum = 0;
	}
	
	return(DevNum);
}

int BtApp_PB_GetMultiServerStatus(unsigned int multiStat)
{
	int i;
	char ret;
	
	/* Check the connected device number */
	for(i=0; i< BT_PBAP_MAX_CONNECTIONS; i++)
	{
		ret = BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(multiStat, i));
		if(ret > 0)
		{
			return(i);
		}
	}

	return(-1);
}

void BtApp_PB_MultiServerEvtHandler(int event, const TcBtBdAddr *DevAddr)
{
	int devnum;
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();

	switch(event)
	{
		case TC_BT_PB_MULTI_BEGIN:
		{
			if(PBInst->deviceCnt < BT_PBAP_MAX_CONNECTIONS)
			{
				devnum = BtApp_PB_GetMultiServerStatus(BTAPP_STATUS_PB_PAC_CONNECT);
			
				if(devnum >= 0)
				{
					BtApp_GAP_PacDisconnect(NULL);
				}

				//PBInst->deviceCnt++;
				BtApp_PB_SetConnectingBdAddr(DevAddr);
				(void)BtApp_GAP_PacConnect(DevAddr);
			}
			break;
		}
		case TC_BT_PB_MULTI_SWITCHING:
		{
			if(DevAddr != NULL)
			{
				BtApp_GAP_PacDisconnect(NULL);
				PBInst->deviceCnt--;
				BtApp_PB_SetConnectingBdAddr(DevAddr);
				(void)BtApp_GAP_PacConnect(DevAddr);
			}
			break;
		}
		case TC_BT_PB_MULTI_TERMINATE:
		{
			devnum = TC_BT_HF_DEV_GetNum(DevAddr);

			if(devnum < NULL)
			{
				return;
			}

			if(BtApp_SYS_GetStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_PB_PAC_CONNECT, devnum)) > 0)
			{
				BtApp_GAP_PacDisconnect(NULL);
			}
			
			BtApp_PB_ClearPhonebookInstance(devnum);
			BtApp_SYS_ClearStatus(BTAPP_STATUS_MASK_SET_DEVNO(BTAPP_STATUS_PB_PAC_DOWNLOADED, devnum));
			PBInst->deviceCnt--;

			/* Switch the other PBAP server */
			devnum = BtApp_PB_GetMultiServerStatus(BTAPP_STATUS_PB_PAC_DOWNLOADED);

			if(devnum >= 0)
			{
				if(devnum != PBInst->deviceId)
				{
					PBInst->deviceCnt--;
#if 1 //for Linux
#else
					DMP_MessageEvtUpdate(DISK_DEVICE_BT_PB, BT_PB_EVT_SET_STATUS_CFM); /* set ui status to change server status*/
#endif
				}
				BtApp_PB_SetConnectingBdAddr((const TcBtBdAddr *)TC_BT_HF_DEV_GetBdAddr(devnum));
				(void)BtApp_GAP_PacConnect(DevAddr);
			}
			break;
		}	
		default:
			break;
	}

}
#endif

void BtApp_PB_SelectRoleATCMDviaHFP(void) /* contacts */
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	
	BtApp_PB_RoleDisconnect();

	select_callhistory[BTAPP_PB_ROLE] = BTAPP_PB_ROLE_DOWNLOAD; /* download */	

	PBInst->service = TC_BT_HF_PROFILE_UUID;
}

/*****************************************************************************
*
* Phonebook common functions - Phonebook search for incoming/outgoing call.
*
*****************************************************************************/
char *BtApp_PB_GetPhonebookNameByNumber(const unsigned char *pStrCallNumber, unsigned char *uniCodeMode)
{
#if defined(BT_PBAP_CLIENT)
	unsigned char		*pvCardStr=NULL;
	char				*pBinaryStr=NULL;
	int				BinaryStrLen, RetStrLen;
	char				*pRetStr=NULL;
	int				CharSetEncoding=CH_SET_NONE_and_ENC_NONE;
	
	#ifdef BT_PHONEBOOK_UI_DEBUG
	pvCardStr = TEST_GetNamevCard(pStrCallNumber, &CharSetEncoding);
	#else
	#ifdef BT_PBAP_MULTISERVER
    if((G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_PBAP(BTAPP_STATUS_PB_PAC_CONNECT))) || (gATCMDStatus!=0))
	#else
    if((G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_PB_PAC_CONNECT)) || (gATCMDStatus!=0))
	#endif
	{
        pvCardStr = (unsigned char *)BtApp_PB_GetNameFromPB((char *)pStrCallNumber, &CharSetEncoding);
	}
	#endif

	if(!pvCardStr)
	{
		return NULL;
	}

	/* Decoding - Quoted-Printable to Binary String */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
            pBinaryStr=(char *)pvCardStr;
			break;
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
            pBinaryStr=(char *)pvCardStr;
			break;
		default:
			break;
	}

	if(!pBinaryStr)
	{
		return NULL;
	}

	BinaryStrLen = strlen(pBinaryStr);

	/* Return string size */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
			RetStrLen = BinaryStrLen + 1;
			break;
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
			RetStrLen = (2 * BinaryStrLen) + 2;
			break;
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
			#ifdef BT_PHONEBOOK_UI_DEBUG
			RetStrLen = BinaryStrLen + 1;
			#else
			RetStrLen = (2 * BinaryStrLen) + 2;
			#endif
			break;
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
			RetStrLen = (2 * BinaryStrLen) + 2;
			break;
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
			RetStrLen = (2 * BinaryStrLen) + 2;
			break;
		default:
			break;
	}

#if 0
#if 1 //for Linux
	pRetStr = TC_BT_Pmalloc(BTAPP_HF_CALLNUM_MAX_LEN);
#else
	pRetStr = TC_Allocate_Memory(BTAPP_HF_CALLNUM_MAX_LEN);
#endif
	if(pRetStr)
		memset(pRetStr, 0, BTAPP_HF_CALLNUM_MAX_LEN);
#else
#if 1 //for Linux
    pRetStr = (char *)TC_BT_Pmalloc(BTAPP_HF_CALLNUM_MAX_LEN*2);
#else
	pRetStr = TC_Allocate_Memory(BTAPP_HF_CALLNUM_MAX_LEN*2);
#endif
	if(pRetStr != NULL)
	{
		memset(pRetStr, 0, BTAPP_HF_CALLNUM_MAX_LEN*2);
	}
#endif
	else
	{
#if 1 //for Linux
		TC_BT_Pfree(pBinaryStr); //free?
#else
		(void) TC_Deallocate_Memory(pBinaryStr); //free?
#endif
		return NULL;
	}

	/* Char Set - EUC-KR or UTF-8 to Unicode */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
			memcpy(pRetStr, pBinaryStr, BinaryStrLen);
			*(pRetStr + BinaryStrLen) = NULL;		/* END of string */
			break;
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
            (void)BtApp_PB_TransAnsitoUni((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen+1);
			break;
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
			#ifdef BT_PHONEBOOK_UI_DEBUG
			memcpy(pRetStr, pBinaryStr, BinaryStrLen+1);
			*(pRetStr + BinaryStrLen) = NULL;		/* END of string */
			#else
#if 1 //for Linux
                    RetStrLen=BtApp_PB_UTF8toUnicode((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen);
#else
			RetStrLen=UTF8toUnicode(pBinaryStr, pRetStr, BinaryStrLen);
#endif
			RetStrLen = ((RetStrLen >= ((BTAPP_HF_CALLNUM_MAX_LEN-1)*2)) ? ((BTAPP_HF_CALLNUM_MAX_LEN-1)*2) : RetStrLen);
			*(pRetStr + RetStrLen) = NULL;		/* END of string */
			*(pRetStr + RetStrLen+1) = NULL;		/* END of string */
			#endif
			break;
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
            (void)BtApp_PB_TransAnsitoUni((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen+1);
			break;
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
#if 1 //for Linux
                    RetStrLen=BtApp_PB_UTF8toUnicode((unsigned char *)pBinaryStr, (unsigned char *)pRetStr, BinaryStrLen);
#else
			RetStrLen=UTF8toUnicode(pBinaryStr, pRetStr, BinaryStrLen);
#endif
			RetStrLen = ((RetStrLen >= ((BTAPP_HF_CALLNUM_MAX_LEN-1)*2)) ? ((BTAPP_HF_CALLNUM_MAX_LEN-1)*2) : RetStrLen);
			*(pRetStr + RetStrLen) = NULL;		/* END of string */
			*(pRetStr + RetStrLen+1) = NULL;		/* END of string */
			break;
		default:
			break;
	}

	/* Dynamic Memory Relase for Quoted Printable */
	switch(CharSetEncoding)
	{
		case CH_SET_NONE_and_ENC_NONE: /* No Encoding & Char set */
		case CH_SET_EUC_KR_and_ENC_NONE: /* EUC-KR */
		case CH_SET_UTF_8_and_ENC_NONE: /* UTF-8 (Unicode) */
			break;
		case CH_SET_CP949_and_ENC_Q_P: /* Quoted-Printable */
		case CH_SET_UTF_8_and_ENC_Q_P: /* UTF-8 & Quoted-Printable */
			break;
		default:
			break;
	}
		
	*uniCodeMode = (CharSetEncoding != FALSE) ? 1 : 0;

	if(pRetStr != NULL)
	{
		return pRetStr;
	}
	else
	{
		return NULL;
	}
#else
	*uniCodeMode=0;
	return NULL;
#endif
}

char *BtApp_PB_SearchforHfCall(const char *pSrcStr, unsigned char* unicodeOrNot, unsigned int ucDevNo)
{
	char *pResStr = NULL;

	if(!*pSrcStr)
	{
		return NULL;
	}

    pResStr = BtApp_PB_GetPhonebookNameByNumber((unsigned char *)pSrcStr, unicodeOrNot);	 // alloc

	return pResStr;
}

#ifdef BT_PHONEBOOK_UI_DEBUG
typedef struct
{
	unsigned int		PhoneNumberLength;
	unsigned char		PhoneNumber[128];
	int				CharSetEncoding;		/* Refer to enum CharState */
	unsigned char		PhonebookName[128];
} UI_BT_Phonebook_struct;

#define UI_BT_Temp_PB_MAX_ENTRY 10
UI_BT_Phonebook_struct UI_BT_Temp_PB[UI_BT_Temp_PB_MAX_ENTRY] =
{
	/* ASCII */
	/* Telechips Inc. */
	{ 10, "0234436792", 0, { 'T', 'e', 'l', 'e', 'c', 'h', 'i', 'p', 's', ' ', 'I', 'n', 'c', '.', 0 }},

	/* EUC-KR */
	/* 欽牖и旋 */
	{ 4, "1234", 1, { 0xC0, 0xCF, 0xC0, 0xCC, 0xBB, 0xEF, 0xBB, 0xE7, 0 }},

	/* UTF-8 (Unicode) */
	/* 堅偌撫攪 - 0xACE0, 0xAC1D, 0xC13C, 0xD130 */
	{ 3, "114", 2, { 0xE0, 0xAC, 0x1D, 0xAC, 0x3C, 0xC1, 0x30, 0xD1, 0 }},

	/* Quoted Printable */
	/* 堅偌 - =B0=ED=B0=B4 */
	{ 4, "3333", 3, { '=', 'B', '0', '=', 'E', 'D', '=', 'B', '0', '=', 'B', '4', 0 }},

	/* UTF-8, Quoted Printable */
	/* 醞措橫 - =E4=B8=BA =E4=BD=95 =E6=97=B6*/
	{ 4, "4444", 4, {	'=', 'E', '4', '=', 'B', '8', '=', 'B', 'A', 
					'=', 'E', '4', '=', 'B', 'D', '=', '9', '5', 
					'=', 'E', '6', '=', '9', '7', '=', 'B', '6', 0 }},

	/* ADD HERE */

	/* END OF ENTRY */
	{ 0, 0, 0, 0 }
};

/* TEST instead of Bluetooth_GetNamevCard */
static unsigned char *TEST_GetNamevCard(char* strCLID, int* CharSetEncoding)
{
	int i;
	unsigned char *pRetStr=NULL;

	for(i=0; i<UI_BT_Temp_PB_MAX_ENTRY; i++)
	{
		if(UI_BT_Temp_PB[i].PhoneNumberLength==0)
			break;

		if(memcmp(strCLID,
			UI_BT_Temp_PB[i].PhoneNumber,
			UI_BT_Temp_PB[i].PhoneNumberLength) == 0)
		{
			pRetStr = UI_BT_Temp_PB[i].PhonebookName;
			*CharSetEncoding = UI_BT_Temp_PB[i].CharSetEncoding;
			break;
		}
	}

	if(i == UI_BT_Temp_PB_MAX_ENTRY)
		*CharSetEncoding = 0;

	return pRetStr;
}
#endif /* BT_PHONEBOOK_UI_DEBUG */

void BtApp_PB_RetryPbDownload(unsigned char param1,unsigned char param2)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
    int ret = 0;
#if 1 //for Linux
	if(1)	
#else
	if(BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==TRUE) /* HF busy */
#endif
	{
        ret = TCCAPI_Timer_SetTime(30 /* 3 sec */, (unsigned char)TIMER_BT_PB_DOWNLOADTIME_ID, BtApp_PB_RetryPbDownload);
		return;
	}
	else
	{
#if 1
		TCCAPI_Timer_CancelTime(TIMER_BT_PB_DOWNLOADTIME_ID);

		switch(PBInst->service)
		{
			case TC_BT_OBEX_PBA_SERVER_PROFILE_UUID: /* PBAP */
				{
					if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_DOWNLOAD)
					{
#ifdef BT_PBAP_CLIENT
                        (void)G_BtPacCmd->TC_BT_PAC_PullPbResponse();
#endif
					}
					else
					{
#ifdef BT_PBAP_CLIENT
                        (void)G_BtPacCmd->TC_BT_PAC_PullVcardListResponse();
#endif
					}
				}
				break;
			default:
				{
					/* TO DO: */
				}
				break;

		}
#else
		TCCAPI_Timer_CancelTime(TIMER_BT_PB_DOWNLOADTIME_ID);

		if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_OCH)
		{
			BtApp_PB_ClearCallHistory(); /* Clear contacts list */
			BtApp_PB_SendCallhistoryDialled(); /* Download Outgoing calls */
		}
		else if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_ICH)
		{
			BtApp_PB_ClearCallHistory(); /* Clear contacts list */
			BtApp_PB_SendCallhistoryReceived(); /* Download Incoming calls */
		}
		else if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_MCH)
		{
			BtApp_PB_ClearCallHistory(); /* Clear contacts list */
			BtApp_PB_SendCallhistoryMissed(); /* Download Missed calls */
		}
		else if(select_callhistory[BTAPP_PB_STORAGE] == BTAPP_PB_SELECT_CCH)
		{
			BtApp_PB_ClearCallHistory(); /* Clear contacts list */
			BtApp_PB_SendCallhistoryCombined(); /* Download Combined calls */
		}
		else
		{
			BtApp_PB_ClearPhonebookInstance(); /* Clear contacts list */
			BtApp_PB_PacConnectHandler(gPBAPSelectSource); /* Download Contacts */
		}
#endif
	}
}

void BtApp_PB_RetryPbListDownload(unsigned char param1,unsigned char param2)
{
	unsigned char searchVal[2];
	searchVal[0] = '*';
	searchVal[1] = NULL;
#if 1 //for Linux
	if(1)
#else
	if(BtApp_SYS_Is_BusyStatusHFP(BtApp_HF_GetCurrDevNum())==TRUE) /* HF busy */
#endif
	{
        TCCAPI_Timer_SetTime(30 /* 3 sec */, (unsigned char )TIMER_BT_PB_DOWNLOADTIME_ID, BtApp_PB_RetryPbListDownload);
		return;
	}
	else
	{
		TCCAPI_Timer_CancelTime(TIMER_BT_PB_DOWNLOADTIME_ID);
#ifdef BT_PBAP_CLIENT
        (void)G_BtPacCmd->TC_BT_PAC_PullVcardListResponse();
#endif
	}
}

void BtApp_PB_InitVcardTempBuffer(void)
{
/* phonebook temp buffer initialize */
	if(pgVcardBuffer != NULL)
	{
		TC_BT_Pfree(pgVcardBuffer);
		pgVcardBuffer = NULL;
	}
	ngVcardBufferSize = 0;
	ngVcardTotalSize = 0;
       gVcardOverflowBufFlag = FALSE;

/* phonebook entry temp buffer initialize */
	if(pgVCardEntryBufPb != NULL)
	{
		TC_BT_Pfree(pgVCardEntryBufPb);
		pgVCardEntryBufPb = NULL;
	}
    ngObjEntryLenPb = 0;

/* phonebook list temp buffer initialize */
	if(pgVCardListBufPb != NULL)
	{
		TC_BT_Pfree(pgVCardListBufPb);
		pgVCardListBufPb = NULL;
	}
    ngObjListLenPb = 0;

}

#if 0// This code is not valid in Linux
void BtApp_PB_SetVcfImgBuffer(unsigned int vcfImgIdx)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	
	gDeviceOfSImage = DISK_DEVICE_BT_PB;
	stVideoInfo.uiBackgroundFileIndex = 0xFFFFFFFFUL ; /* Membase data */
	gFileHandleOfMultiCodec = JPG_MEMORY_FD; /* Membase data */
	stVideoInfo.uiDisplayWidth = JPGVCARDIMAGEWD;
	stVideoInfo.uiDisplayHeight = JPGVCARDIMAGEHT;

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
	{
		gMembaseReadInfo.paddr = PBEntry->vCardObj.image; /* memory start address for JPG decoding */
		stVideoInfo.uiSuggestedBufferSize = (unsigned int)PBEntry->vCardObj.image; /* memory start address for PNG decoding */
		stVideoInfo.uiFrameRate = PBEntry->vCardObj.imageSize; /* Image data size */
	}
	else /* entire vCards */
	{
		gMembaseReadInfo.paddr = PBInst->vCardObj[PBInst->deviceId][vcfImgIdx].image; /* memory start address for JPG decoding */
		stVideoInfo.uiSuggestedBufferSize = (unsigned int)PBInst->vCardObj[PBInst->deviceId][vcfImgIdx].image; /* memory start address for PNG decoding */
		stVideoInfo.uiFrameRate = PBInst->vCardObj[PBInst->deviceId][vcfImgIdx].imageSize; /* Image data size */
	}
	gMembaseReadInfo.decodedSize = 0;
}

int BtApp_PB_GetVcfJpgImgBufferSize(unsigned int * totalDataSize, unsigned int vcfImgIdx)
{
    PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
    PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();

    if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* one vCard entry */
    {
        *totalDataSize = PBEntry->vCardObj.imageSize;
    }
    else
    {
        *totalDataSize = PBInst->vCardObj[PBInst->deviceId][vcfImgIdx].imageSize;
    }

    return JPG_RET_SUCCESS;
}
#endif //VCARD_IMAGE_DISPLAY

int BtApp_PB_SearchKorNameFromVcardObjDB(int index)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();
	int result = FALSE, compareResult1 = 0, compareResult2 = 0;
	int i;

	for(i = 0; i < BT_PB_SEARCH_KOR_COUNT; i++)
	{
        compareResult1 = strncmp((char *)PBSearchIdx->searchKeyword, (char *)&gKorUtf8CharacterTable[i], (BT_PB_SEARCH_CHAR_SIZE-1));
		if(compareResult1 == 0){
			break;
		}
	}

    compareResult1 = strncmp((char *)PBInst->vCardObj[PBInst->deviceId][index].DrawName, (char *)PBSearchIdx->searchKeyword, (BT_PB_SEARCH_CHAR_SIZE-1));

	switch(i)
	{
		case 0:
			break;
		case 1: /* 丑 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 234)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 176) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 184))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 234)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 139)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 2: /* 丑丑 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 234)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 140) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 143)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 234)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 186) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 191))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 129))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 130)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 151)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 3: /* 中 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 130)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 152) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 131) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 138))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 139)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 163)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 4: /* 之 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 139)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 164) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 140) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 147))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 148)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 175)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 5: /* 之之 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 148)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 176) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 149) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 156))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 157)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 187)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 6: /* 予 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 157)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 188) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 158) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 166))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 167)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 135)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 7: /* 仃 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 167)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 136) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 168) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 175))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 176)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 147)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 8: /* 仆 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 176)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 148) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 177) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 184))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 159)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 9: /* 仆仆 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 160) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 235)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 186) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 191))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 129))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 130)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 171)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 10: /* 今 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 130)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 172) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 131) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 138))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 139)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 183)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 11: /* 今今 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 139)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 184) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 140) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 148))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 149)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 131)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 12: /* 仄 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 149)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 132) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 150) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 157))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 158)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 143)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 13: /* 元 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 158)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 144) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 159) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 166))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 167)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 155)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 14: /* 元元 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 167)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 156) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 168) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 175))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 176)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 167)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 15: /* 內 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 176)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 168) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 177) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 184))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 179)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 16: /* 六 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 185)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 180) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 236)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 186) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 191))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 130))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 17: /* 兮 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 131) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 139))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 140)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 139)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 18: /* 公 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 140)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 140) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 141) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 148))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 149)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 151)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 19: /* 冗 */
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 149)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 152) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] >= 150) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] <= 157))
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBInst->vCardObj[PBInst->deviceId][index].DrawName[0] == 237)
				&& (PBInst->vCardObj[PBInst->deviceId][index].DrawName[1] == 158)
				&& ((PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] >= 128) && (PBInst->vCardObj[PBInst->deviceId][index].DrawName[2] <= 163)))
			{
				compareResult2 = TRUE;
			}
			break;

		default:
			break;
	}

	if((compareResult1 == 0) || (compareResult2 == TRUE))
	{
		result = TRUE;
	}

	return result;
}

int BtApp_PB_SearchKorNameVcardListDB(int index)
{
	PHONEBOOK_LIST_TYPE *PBList = BtApp_PB_GetPhonebookList();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();
	int result = FALSE, compareResult1 = 0, compareResult2 = 0;
	int i;

	for(i = 0; i < BT_PB_SEARCH_KOR_COUNT; i++)
	{
        compareResult1 = strncmp((char *)PBSearchIdx->searchKeyword, (char *)&gKorUtf8CharacterTable[i], (BT_PB_SEARCH_CHAR_SIZE-1));
		if(compareResult1 == 0){
			break;
		}
	}

    compareResult1 = strncmp((char *)PBList->vCardListObj[index].name, (char *)PBSearchIdx->searchKeyword, (BT_PB_SEARCH_CHAR_SIZE-1));

	switch(i)
	{
		case 0:
			break;
			
		case 1: /* 丑 */
			if((PBList->vCardListObj[index].name[0] == 234)
				&& ((PBList->vCardListObj[index].name[1] >= 176) && (PBList->vCardListObj[index].name[1] <= 184))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 234)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 139)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 2: /* 丑丑 */
			if((PBList->vCardListObj[index].name[0] == 234)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 140) && (PBList->vCardListObj[index].name[2] <= 143)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 234)
				&& ((PBList->vCardListObj[index].name[1] >= 186) && (PBList->vCardListObj[index].name[1] <= 191))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 128) && (PBList->vCardListObj[index].name[1] <= 129))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 130)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 151)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 3: /* 中 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 130)
				&& ((PBList->vCardListObj[index].name[2] >= 152) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 131) && (PBList->vCardListObj[index].name[1] <= 138))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 139)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 163)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 4: /* 之 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 139)
				&& ((PBList->vCardListObj[index].name[2] >= 164) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 140) && (PBList->vCardListObj[index].name[1] <= 147))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 148)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 175)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 5: /* 之之 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 148)
				&& ((PBList->vCardListObj[index].name[2] >= 176) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 149) && (PBList->vCardListObj[index].name[1] <= 156))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 157)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 187)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 6: /* 予 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 157)
				&& ((PBList->vCardListObj[index].name[2] >= 188) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 158) && (PBList->vCardListObj[index].name[1] <= 166))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 167)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 135)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 7: /* 仃 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 167)
				&& ((PBList->vCardListObj[index].name[2] >= 136) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 168) && (PBList->vCardListObj[index].name[1] <= 175))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 176)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 147)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 8: /* 仆 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 176)
				&& ((PBList->vCardListObj[index].name[2] >= 148) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 177) && (PBList->vCardListObj[index].name[1] <= 184))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 159)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 9: /* 仆仆 */
			if((PBList->vCardListObj[index].name[0] == 235)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 160) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 235)
				&& ((PBList->vCardListObj[index].name[1] >= 186) && (PBList->vCardListObj[index].name[1] <= 191))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 128) && (PBList->vCardListObj[index].name[1] <= 129))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 130)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 171)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 10: /* 今 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 130)
				&& ((PBList->vCardListObj[index].name[2] >= 172) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 131) && (PBList->vCardListObj[index].name[1] <= 138))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 139)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 183)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 11: /* 今今 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 139)
				&& ((PBList->vCardListObj[index].name[2] >= 184) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 140) && (PBList->vCardListObj[index].name[1] <= 148))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 149)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 131)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 12: /* 仄 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 149)
				&& ((PBList->vCardListObj[index].name[2] >= 132) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 150) && (PBList->vCardListObj[index].name[1] <= 157))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 158)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 143)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 13: /* 元 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 158)
				&& ((PBList->vCardListObj[index].name[2] >= 144) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 159) && (PBList->vCardListObj[index].name[1] <= 166))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 167)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 155)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 14: /* 元元 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 167)
				&& ((PBList->vCardListObj[index].name[2] >= 156) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 168) && (PBList->vCardListObj[index].name[1] <= 175))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 176)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 167)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 15: /* 內 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 176)
				&& ((PBList->vCardListObj[index].name[2] >= 168) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 177) && (PBList->vCardListObj[index].name[1] <= 184))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 179)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 16: /* 六 */
			if((PBList->vCardListObj[index].name[0] == 236)
				&& (PBList->vCardListObj[index].name[1] == 185)
				&& ((PBList->vCardListObj[index].name[2] >= 180) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 236)
				&& ((PBList->vCardListObj[index].name[1] >= 186) && (PBList->vCardListObj[index].name[1] <= 191))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& ((PBList->vCardListObj[index].name[1] >= 128) && (PBList->vCardListObj[index].name[1] <= 130))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 17: /* 兮 */
			if((PBList->vCardListObj[index].name[0] == 237)
				&& ((PBList->vCardListObj[index].name[1] >= 131) && (PBList->vCardListObj[index].name[1] <= 139))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& (PBList->vCardListObj[index].name[1] == 140)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 139)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 18: /* 公 */
			if((PBList->vCardListObj[index].name[0] == 237)
				&& (PBList->vCardListObj[index].name[1] == 140)
				&& ((PBList->vCardListObj[index].name[2] >= 140) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& ((PBList->vCardListObj[index].name[1] >= 141) && (PBList->vCardListObj[index].name[1] <= 148))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& (PBList->vCardListObj[index].name[1] == 149)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 151)))
			{
				compareResult2 = TRUE;
			}
			break;

		case 19: /* 冗 */
			if((PBList->vCardListObj[index].name[0] == 237)
				&& (PBList->vCardListObj[index].name[1] == 149)
				&& ((PBList->vCardListObj[index].name[2] >= 152) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& ((PBList->vCardListObj[index].name[1] >= 150) && (PBList->vCardListObj[index].name[1] <= 157))
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 191)))
			{
				compareResult2 = TRUE;
			}
			if((PBList->vCardListObj[index].name[0] == 237)
				&& (PBList->vCardListObj[index].name[1] == 158)
				&& ((PBList->vCardListObj[index].name[2] >= 128) && (PBList->vCardListObj[index].name[2] <= 163)))
			{
				compareResult2 = TRUE;
			}
			break;

		default:
			break;
	}

	if((compareResult1 == 0) || (compareResult2 == TRUE))
	{
		result = TRUE;
	}

	return result;
}

unsigned int BtApp_PB_MakeSearchContactsIndexList(void)
{
	PHONEBOOK_INST_TYPE *PBInst = BtApp_PB_GetPhonebookInstance();
	PHONEBOOK_LIST_TYPE *PBList = BtApp_PB_GetPhonebookList();
	//PHONEBOOK_ENTRY_TYPE *PBEntry = BtApp_PB_GetPhonebookEntry();
	PHONEBOOK_SEARCH_INDEX_TYPE *PBSearchIdx = BtApp_PB_GetPhonebookSearchIndex();
	int i = 0;
	unsigned int count = 0;
	int compareResult = FALSE;

	if(select_callhistory[BTAPP_PB_ROLE] == BTAPP_PB_ROLE_BROWSING) /* Entry */
	{
		/* search */
		if(PBSearchIdx->searchKeyword[0] == 0xFF)
		{
			count = PBList->numOfHandle;
		}
		else
		{
			for(i=0; i<PBList->numOfHandle; i++)
			{
#if 1
				if(PBSearchIdx->searchLanguage == SEARCH_LANGUAGE_ENG)
				{
					if((PBList->vCardListObj[i].name[0] == PBSearchIdx->searchKeyword[0])
						||(PBList->vCardListObj[i].name[0] == (PBSearchIdx->searchKeyword[0] + 32)))
					{
						PBSearchIdx->searchIdxList[count] = i;
						count++;
					}
				}
				else /* SEARCH_LANGUAGE_KOR */
				{
					compareResult = BtApp_PB_SearchKorNameVcardListDB(i);
					//compareResult = strncmp(PBList->vCardListObj[i].name, PBSearchIdx->searchKeyword, (BT_PB_SEARCH_CHAR_SIZE-1));
					if(compareResult == TRUE)
					{
						PBSearchIdx->searchIdxList[count] = i;
						count++;
					}
				}
#else
				if((PBList->vCardListObj[i].name[0] == PBSearchIdx->searchKeyword)
					||(PBList->vCardListObj[i].name[0] == (PBSearchIdx->searchKeyword + 32)))
				{
					PBSearchIdx->searchIdxList[count] = i;
					count++;
				}
#endif
				if(count >= TCC_MAX_VCARD_LIST_NUMBER){
					break;
				}
			}
		}
	}
	else /* download */
	{
		/* search */
		if(PBSearchIdx->searchKeyword[0] == 0xFF)
		{
			count = PBInst->vCardObjNum[PBInst->deviceId];
		}
		else
		{
			for(i=0; i<PBInst->vCardObjNum[PBInst->deviceId]; i++)
			{
#if 1
				if(PBSearchIdx->searchLanguage == SEARCH_LANGUAGE_ENG)
				{
					if((PBInst->vCardObj[PBInst->deviceId][i].DrawName[0] == PBSearchIdx->searchKeyword[0])
					||(PBInst->vCardObj[PBInst->deviceId][i].DrawName[0] == (PBSearchIdx->searchKeyword[0] + 32)))
					{
						PBSearchIdx->searchIdxList[count] = i;
						count++;
					}
				}
				else /* SEARCH_LANGUAGE_KOR */
				{
					compareResult = BtApp_PB_SearchKorNameFromVcardObjDB(i);
					//compareResult = strncmp(PBInst->vCardObj[PBInst->deviceId][i].DrawName, PBSearchIdx->searchKeyword, (BT_PB_SEARCH_CHAR_SIZE-1));
					if(compareResult == TRUE)
					{
						PBSearchIdx->searchIdxList[count] = i;
						count++;
					}
				}
#else
				if((PBInst->vCardObj[PBInst->deviceId][i].DrawName[0] == PBSearchIdx->searchKeyword)
					||(PBInst->vCardObj[PBInst->deviceId][i].DrawName[0] == (PBSearchIdx->searchKeyword + 32)))
				{
					PBSearchIdx->searchIdxList[count] = i;
					count++;
				}
#endif
				if(count >= TCC_MAX_VCARD_NUMBER){
					break;
				}
			}
		}
	}
	return count;
}
#endif /* defined(BT_PBAP_CLIENT) */
