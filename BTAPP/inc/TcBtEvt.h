/****************************************************************************
 *   FileName    : TcBtEvt.h
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
*  Description : This file contains functions for a Telechips Bluetooth
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    15/01/06     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef TCBTEVT_H
#define TCBTEVT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

/* system */
enum
{
    BT_SYS_EVT_INIT_SVR_ERR = 0,
    BT_SYS_EVT_INIT_ERR,
    BT_SYS_EVT_INIT_CFM,
    BT_SYS_EVT_DEINIT_CFM,

    BT_SYS_EVT_MAX
};

/* SC */
enum
{
    BT_DM_EVT_BOND_SUCCESS = 0,
    BT_DM_EVT_BOND_ERR,
    BT_DM_EVT_PINCODE_IND,
    BT_DM_EVT_SSP_NUMERIC_COMPARISON_IND,
    BT_DM_EVT_SSP_PASSKEY_NOTIFICATION_IND,
    BT_DM_EVT_SSP_JUST_WORKS_IND,
    BT_DM_EVT_SEARCH_RESULT ,
    BT_DM_EVT_SEARCH_CLOSE,
    BT_DM_EVT_REFRESH ,
    BT_DM_EVT_CONNECTED,
    BT_DM_EVT_READLOCALDEVICE,
    BT_DM_EVT_MAX
};


/* AV */
enum
{
	/* A2DP Sink De/Activation */
	BT_AV_EVT_SNK_ACTIVATE = 0,
	BT_AV_EVT_SNK_DEACTIVATE,

	/* A2DP Sink Dis/Connection */
    BT_AV_EVT_SNK_CONNECT,
    BT_AV_EVT_SNK_DISCONNECT,

	/* A2DP Sink Streaming Set Up */
	BT_AV_EVT_SNK_DISCOVER,
	BT_AV_EVT_SNK_SET_CONFIGURATION,
	BT_AV_EVT_SNK_OPEN,
	BT_AV_EVT_SNK_START,	

     /* A2DP Sink Streaming Control */
	BT_AV_EVT_SNK_CLOSE,		
	BT_AV_EVT_SNK_SUSPEND,
    BT_AV_EVT_SNK_RECONFIGURATION = 10,
	BT_AV_EVT_SNK_STREAM_INVALID,	     
    BT_AV_EVT_SNK_BYPASS,

    /* AVRCP Controller Dis/Connection */
    BT_AV_EVT_CT_CONNECT,
    BT_AV_EVT_CT_DISCONNECT,

    /* AVRCP Cotroller Key */
    BT_AV_EVT_CT_KEY_RESULT,

    /* AVRCP Cotroller Player Application Setting */
    BT_AV_EVT_CT_PAS_SUPPORT,
    BT_AV_EVT_CT_PAS_GET_SETTINGS,
    BT_AV_EVT_CT_PAS_SET_SETTINGS,
    BT_AV_EVT_CT_PAS_GET_ATT_TEXT,
    BT_AV_EVT_CT_PAS_GET_VAL_TEXT = 20,
    BT_AV_EVT_CT_PAS_INFORM_CHAR_SET,
    BT_AV_EVT_CT_PAS_INFORM_BATTERY,

    BT_AV_EVT_CT_INFO_VER,
    BT_AV_EVT_CT_NOTIFICATION,
    BT_AV_EVT_CT_SET_VOLUME,

	/* AVRCP Cotroller Metadata */	
	BT_AV_EVT_CT_META_SUPPORT,		
	BT_AV_EVT_CT_ELEM_TITLE,
	BT_AV_EVT_CT_ELEM_ARTIST,
	BT_AV_EVT_CT_ELEM_ALBUM,
    BT_AV_EVT_CT_ELEM_GENRE = 30,
	BT_AV_EVT_CT_ELEM_TRACK,
	BT_AV_EVT_CT_ELEM_TOTAL,
	BT_AV_EVT_CT_ELEM_TIME,
	BT_AV_EVT_CT_PLAY_LENGTH,
	BT_AV_EVT_CT_PLAY_POSITION,
	BT_AV_EVT_CT_PLAY_STATUS,
	
	/* AVRCP Controller Browsing */
	BT_AV_EVT_CT_BROWSE_SUPPORT,
	BT_AV_EVT_CT_PLAYER_CHANGED,
	BT_AV_EVT_CT_SET_PLAYER_RESULT,
    BT_AV_EVT_CT_CHANGE_PATH_RESULT = 40,
	BT_AV_EVT_CT_FOLDER_ITEMS,
	BT_AV_EVT_CT_PLAY_ITEM_RESULT,
	BT_AV_EVT_CT_SEARCH,
	BT_AV_EVT_CT_ADD_TO_NPL,
	BT_AV_EVT_CT_MEDIA_DB_CHANGED,

    BT_AV_EVT_MAX
};

enum
{
       /* activate */
    	BT_PB_EVT_PAC_ACTIVATE_CFM = 0,
    	BT_PB_EVT_PAC_ACTIVATE_ERR,
    	/* deactivate */
    	BT_PB_EVT_PAC_DEACTIVATE_CFM,
    	BT_PB_EVT_PAC_DEACTIVATE_ERR,
       /* connect */
	BT_PB_EVT_CONNECT_CFM,
	BT_PB_EVT_CONNECT_ERR,
       /* disconnect */
	BT_PB_EVT_DISCONNECT_IND,
       /* PBAP event */
	BT_PB_EVT_PULLPB_IND,
	BT_PB_EVT_PULLPB_CFM,
	BT_PB_EVT_PULLCH_IND,
    BT_PB_EVT_PULLCH_CFM = 10,
	BT_PB_EVT_PULLPB_ERR,
	BT_PB_EVT_PULLCH_ERR,
	BT_PB_EVT_PAC_ABORT_CFM,
	BT_PB_EVT_PAC_SELECTSORTING_CFM,
	BT_PB_EVT_PAC_SELECTFEATURES_CFM,
	BT_PB_EVT_PAC_SETFILTER_CFM,
	BT_PB_EVT_PAC_SELECTSOURCE_CFM,
	BT_PB_EVT_PAC_SETFOLDER_CFM,
	BT_PB_EVT_PAC_SETFOLDER_ERR,
    BT_PB_EVT_PAC_SETAUTHENTICATION_CFM = 20,
	BT_PB_EVT_PAC_PULLVCARDLIST_CFM,
	BT_PB_EVT_PAC_PULLVCARDLIST_ERR,
	BT_PB_EVT_PAC_PULLVCARDENTRY_CFM,
	BT_PB_EVT_PAC_PULLVCARDENTRY_ERR,
	BT_PB_EVT_PAC_PULLPB_SIZEONLY_CFM,
	BT_PB_EVT_PAC_PULLPB_NODATA_CFM,
 /* browsing */
	BT_PB_EVT_GET_CONTACTS, /* for 800x */
	BT_PB_EVT_GET_CONTACTS_LIST,
	BT_PB_EVT_GET_CONTACTS_LIST_INFO,
    BT_PB_EVT_GET_CONTACTS_LIST_CLOSE = 30,
	BT_PB_EVT_GET_CALLHISTORY_LIST,
	BT_PB_EVT_GET_CALLHISTORY_LIST_INFO,
	BT_PB_EVT_GET_CALLHISTORY_LIST_CLOSE,
	BT_PB_EVT_GET_VCARD_LIST,
	BT_PB_EVT_GET_VCARD_LIST_CLOSE,
	BT_PB_EVT_GET_CONTACTS_LISTUP,
	BT_PB_EVT_GET_CONTACTS_LISTDOWN,
	BT_PB_EVT_GET_CALLHISTORY_LISTUP,
	BT_PB_EVT_GET_CALLHISTORY_LISTDOWN,
/* other */
    BT_PB_EVT_SETAUTODOWNLOAD_CFM = 40,
	BT_PB_EVT_SETVCARDDUMP_CFM,
	BT_PB_EVT_SET_STATUS_CFM,
#if 0// This code is not valid in Linux
	BT_PB_EVT_DECODE_VCARD_IMAGE_CFM,
	BT_PB_EVT_DECODE_VCARD_IMAGE_ERR,
#endif
	BT_PB_EVT_GET_SEARCH_CONTACTS_LIST,

	BT_PB_EVT_MAX
};

enum
{
    BT_MAP_EVT_CONNECT_CFM,
    BT_MAP_EVT_DISCONNECT_CFM,
    BT_MAP_EVT_GET_FOLDERLIST,
    BT_MAP_EVT_SET_FOLDERLIST,
    BT_MAP_EVT_GET_MESSAGELIST,
    BT_MAP_EVT_GET_MESSAGE,
    BT_MAP_EVT_UPDATE_INBOX,
    BT_MAP_EVT_SET_MESSAGE_ST,
    BT_MAP_EVT_UPLOAD_MESSAGE,
    BT_MAP_EVT_NOTI_REG,
    BT_MAP_EVT_NOTI_EVT,

    BT_MAP_EVT_MAX
};
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */


typedef struct
{
    TcBtEvtType 				type;
    TcBtUint8					avResultFlag;
} BtA2dpEvt;

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*************** SYS Event ***************/
extern void TcBtEvt_SysEvt(int type);

/*************** SC Event  ***************/
extern void TcBtEvt_ScEvt(int type, void *pTcBtEvt);

/*************** SD EventList ***************/
extern void TcBtEvt_SearchEvt(int type, void *pTcBtEvt);

/*************** DM Control function ***************/
extern void TcBtEvt_DMEvt(int type, void *pTcBtEvt);

/*************** Set BD Addr at SETUP UI ***************/
extern void TcBtEvt_SetBDAddrAtSetup(int type, void *pTcBtEvt);

/*************** A2DP/AVRCP Event ***************/
extern void TcBtEvt_AvEvt(int type, void *pTcBtEvt);

/*************** PB EventList ***************/
extern void TcBtEvt_PBEvt(int type, void *pTcBtEvt);

/*************** MAP EventList ***************/
extern void TcBtEvt_MAPEvt(int type, void *pTcBtEvt);

/*************** Launcher Signal event ***************/
#if defined(__arm__) && !defined(BT_NO_LACUCHER)
extern void TcBtEvt_DbusShowApp(int appID);
extern void TcBtEvt_DbusHideApp(int appID);
#endif

/*****************************************************************************
*
* debug
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // TCBTEVT_H
