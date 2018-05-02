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

#ifndef __TC_BT_PHONEBOOK_COMMON_EVT_H__
#define __TC_BT_PHONEBOOK_COMMON_EVT_H__

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

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
/* PBAP event */
#ifdef ______________________________________________________________________________ 
#endif
extern void BtApp_PB_EvtPACConnectCfm(void);
extern void BtApp_PB_EvtPACConnectErr(void);
extern void BtApp_PB_EvtPACDisconnectInd(void);
extern void BtApp_PB_EvtPACPullPBInd(void);
extern void BtApp_PB_EvtPACPullPBCfm(void);
extern void BtApp_PB_EvtPACPullCHInd(void);
extern void BtApp_PB_EvtPACPullCHCfm(void);
extern void BtApp_PB_EvtPACPullPBErr(void);
extern void BtApp_PB_EvtPACPullCHErr(void);
extern void BtApp_PB_EvtPACAbortCfm(void);
extern void BtApp_PB_EvtPACSelectSortingCfm(void);
extern void BtApp_PB_EvtPACSelectFeaturesCfm(void);
extern void BtApp_PB_EvtPACSetFilterCfm(void);
extern void BtApp_PB_EvtPACSelectSourceCfm(void);
extern void BtApp_PB_EvtPACSetFolderCfm(void);
extern void BtApp_PB_EvtPACSetFolderErr(void);
extern void BtApp_PB_EvtPACSetAuthenticationCfm(void);
extern void BtApp_PB_EvtPACPullVcardListCfm(void);
extern void BtApp_PB_EvtPACPullVcardListErr(void);
extern void BtApp_PB_EvtPACPullVcardEntryCfm(void);
extern void BtApp_PB_EvtPACPullVcardEntryErr(void);
extern void BtApp_PB_EvtPACPullPBSizeOnlyCfm(void);
extern void BtApp_PB_EvtPACPullPBNoData(void);

/* AT-CMD event */
#ifdef ______________________________________________________________________________ 
#endif
extern void BtApp_PB_EvtATCConnectCfm(void);
extern void BtApp_PB_EvtATCConnectErr(void);
extern void BtApp_PB_EvtATCDisconnectInd(void);
extern void BtApp_PB_EvtATCPullPBInd(void);
extern void BtApp_PB_EvtATCPullPBCfm(void);
extern void BtApp_PB_EvtATCPullPBErr(void);
extern void BtApp_PB_EvtATCPullCHInd(void);
extern void BtApp_PB_EvtATCPullCHCfm(void);
extern void BtApp_PB_EvtATCPullCHErr(void);
 
/* Browsing event */
#ifdef ______________________________________________________________________________ 
#endif
extern void BtApp_PB_EvtGetContacts(void);
extern void BtApp_PB_EvtGetContactsList(void);
extern void BtApp_PB_EvtGetContactsListInfo(void);
extern void BtApp_PB_EvtGetContactsListClose(void);
extern void BtApp_PB_EvtGetCallHistoryList(void);
extern void BtApp_PB_EvtGetCallHistoryListInfo(void);
extern void BtApp_PB_EvtGetCallhistoryListClose(void);
extern void BtApp_PB_EvtGetVcardList(void);

/* Other event */
#ifdef ______________________________________________________________________________ 
#endif
extern void BtApp_PB_EvtAutoDownPBCfm(void);
extern void BtApp_PB_EvtVcardDumpCfm(void);
extern void BtApp_PB_EvtSetStatusCfm(void);
extern void BtApp_PB_EvtGetSearchContactsList(void);
extern void BtApp_PB_EvtReserved(void);

/* sub functions of event handlers*/
#ifdef ______________________________________________________________________________ 
#endif
//extern void BtApp_PB_EvtAddContactsList(unsigned int contactsListCount);
//extern void BtApp_PB_EvtAddContactsListClose(void);
//extern void BtApp_PB_EvtAddVcardList(unsigned int contactsListCount);
//extern void BtApp_PB_EvtAddVcardListClose(void);
//extern void BtApp_PB_EvtAddCallhistoryList(unsigned int callhistoryListCount);
//extern void BtApp_PB_EvtAddCallhistoryListClose(void);
extern unsigned char BtApp_PB_EvtSelectedSorting(void);
extern unsigned char BtApp_PB_EvtSelectedSource(void);
extern unsigned char BtApp_PB_EvtSelectedVcardFilter(void);
extern unsigned char BtApp_PB_EvtSelectedAutoDownload(void);
extern unsigned char BtApp_PB_EvtSelectedFeatures(void);
extern unsigned char BtApp_PB_EvtSelectedVcardDump(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_PHONEBOOK_COMMON_EVT_H__ */
