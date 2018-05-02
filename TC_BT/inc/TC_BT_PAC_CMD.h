/****************************************************************************
 *   FileName    : TC_BT_PAC_CMD.h
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
*  Description : This file contains functions for a Bluetooth PBAP client command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/06/02     0.1            created                     jhd
*
*******************************************************************************/

#ifndef __TC_BT_PAC_CMD_H__
#define __TC_BT_PAC_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "TC_BT_CMD.h"
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
typedef enum
{
	TC_BT_PAC_ACTIVATE_REQ = 0,
	TC_BT_PAC_DEACTIVATE_REQ,
	TC_BT_PAC_CONNECT_REQ,
	TC_BT_PAC_DISCONNECT_REQ,
	TC_BT_PAC_CANCELCONNECT_REQ,	
	TC_BT_PAC_PULLPB_REQ,
	TC_BT_PAC_PULLPB_RES,
	TC_BT_PAC_PULLOCH_REQ,
	TC_BT_PAC_PULLICH_REQ,
	TC_BT_PAC_PULLMCH_REQ,
	TC_BT_PAC_PULLCCH_REQ,
	TC_BT_PAC_PULL_VCARDLIST_REQ,
	TC_BT_PAC_PULL_VCARDLIST_RES,
	TC_BT_PAC_PULL_VCARDENTRY_REQ,
	TC_BT_PAC_PULL_VCARDENTRY_RES,
	TC_BT_PAC_SET_FOLDER_REQ,
	TC_BT_PAC_SET_VCARDFILTER_REQ,
	TC_BT_PAC_ABORT_REQ,
	TC_BT_PAC_SET_AUTHENTICATION_REQ
} TcBtPacCmdEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define VCARD_FILTER_PROERTY_ALL                0
#define VCARD_FILTER_PROERTY_NAME_TEL     		1
#define VCARD_FILTER_PROERTY_NONE             	2

#ifndef PAC_MAX_OBEX_PACKET_SIZE
#define PAC_MAX_OBEX_PACKET_SIZE        		(0x1f00)
#endif

typedef enum Contacts_Bg_Thread
{
	BT_VCARD_PARSE = 0,
	BT_VCARD_SORT,
	BG_THREAD_WBXML
}Contacts_Bg_T;

#define BT_TEXT_PBAP_NONAME_MSG  			"No Name"

#define PHONEBOOK_STATE_IDLE				0
#define PHONEBOOK_STATE_CONNECTED		1
#define PHONEBOOK_STATE_DOWNLOADING		2
#define PHONEBOOK_STATE_DOWNLOADED		3
#define PHONEBOOK_STATE_DISCONNECTED	4
#define PHONEBOOK_STATE_ONCE				5

//#define TC_BT_PAC_SRC_NONE    (0x00)
//#define TC_BT_PAC_SRC_PHONE   (0x01)
//#define TC_BT_PAC_SRC_SIM     (0x02)

#define TC_BT_PAC_SRC_PHONE   (0x00)
#define TC_BT_PAC_SRC_SIM     (0x01)

#define TC_BT_PAC_FOLDER_OCH		0
#define TC_BT_PAC_FOLDER_ICH		1
#define TC_BT_PAC_FOLDER_MCH		2
#define TC_BT_PAC_FOLDER_CCH		3
#define TC_BT_PAC_FOLDER_PB			4


/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint16					pacMaxObexPacketSize;
} TcBtPacCmd;

#if 1 /* Telechips' remark */
typedef struct
{
    TcBtCmdType                 type;
} TcBtPacActivateCmd;

typedef struct
{
    TcBtCmdType                 type;
} TcBtPacDeactivateCmd;
#endif

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint32					src;
    TcBtUint32					ObjNum;
    TcBtBool					srmpOn;
} TcBtPacPullPbReqCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtBool                 	srmpOn;
} TcBtPacPullPbResCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint16 					select_pbList;
    TcBtUint8       			order;
    TcBtUint8       			*searchVal;
    TcBtUint16 					searchValLen;
    TcBtUint8       			searchAtt;
    TcBtUint16 					maxListCnt;
    TcBtUint16 					listStartOffset;
    TcBtBool					srmpOn;
} TcBtPacPullVcardListReqCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtBool                 	srmpOn;
} TcBtPacPullVcardListResCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint8          			*ucs2name; /* Unicode name, zero-terminated */
    TcBtUint8                	filter[8];
    TcBtUint8      				format;
    TcBtBool                 	srmpOn;
} TcBtPacPullVcardEntryReqCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtBool                 	srmpOn;
} TcBtPacPullVcardEntryResCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint32					filtertype;
} TcBtPacSetVcardFilterReqCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint16 					set_curfolder;
} TcBtPacSetFolderReqCmd;

typedef struct
{
    TcBtCmdType            		type;
} TcBtPacAbortReqCmd;

typedef struct
{
    TcBtCmdType            		type;
    TcBtUint8 					auth; /* authentication value */
} TcBtPacSetAuthReqCmd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_PAC_CMD_H__ */
