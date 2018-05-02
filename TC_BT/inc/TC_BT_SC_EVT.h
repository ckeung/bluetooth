/****************************************************************************
 *   FileName    : TC_BT_SC_EVT.h
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
*  Description : This file contains functions for a Bluetooth SC event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/12     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_SC_EVT_H__
#define __TC_BT_SC_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "TC_BT_EVT.h"
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
	TC_BT_SC_PASSKEY_IND = TC_BT_SC_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x700 */
	TC_BT_SC_DEBOND_CFM,
	TC_BT_SC_AUTHORISE_IND,
	TC_BT_SC_BOND_EVT,
	TC_BT_SC_SSP_NOTIFICATION_IND,
	TC_BT_SC_SSP_COMPARE_IND,
	TC_BT_SC_SSP_JUSTWORKS_IND,
	TC_BT_SC_DEBUG_MODE_CFM,

	TC_BT_SC_EVT_TYPE_MAX
} TcBtSCEvtEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* ---------- Defines the Security Controller (SC) TcBtStatusCode ----------*/
#define TC_BT_ERROR_SRV_SEC_BOND_ACTIVE       300 /* Bond already ongoing */
#define TC_BT_ERROR_SRV_SEC_RM_DEV            301 /* Remove device fail: Device is connected */
#define TC_BT_ERROR_SRV_SEC_BOND_CANCEL_ERROR 302 /* Cancel bonding procedure fail */

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	unsigned short int				type;						/* primitive/message identity */
	TcBtBdAddr				deviceAddr;					/* address of device */
	TcBtString				*deviceName;				/* name of device */
	TcBtUint16				serviceId;					/* the id of the service for which authorisation is required */
} TcBtScAuthoriseInd;

typedef struct
{
    unsigned short int				type;                       /* primitive/message identity */
    TcBtBdAddr				deviceAddr;                 /* address of device to bond with. */
    TcBtStatus				status;
} TcBtScBondEvt;

typedef struct
{
    unsigned short int				type;                       /* primitive/message identity */
    TcBtBdAddr				deviceAddr;                 /* address of device to bond with. */
    TcBtStatus				status;
} TcBtScDebondCfm;

typedef struct
{
    unsigned short int				type;                       /* primitive/message identity */
    TcBtBdAddr				deviceAddr;                 /* address of device to bond with. */
	TcBtString				*deviceName;				/* name of device */    
    TcBtUint32				cod;                        /* The class of device of the remote device */
} TcBtScPairingSendEvt;

typedef struct
{
    unsigned short int				type;                       /* primitive/message identity */
    TcBtBdAddr				deviceAddr;                 /* address of device to bond with. */
	TcBtString				*deviceName;				/* name of device */    
    TcBtUint32				cod;                        /* The class of device of the remote device */
    TcBtUint32				numericValue;
} TcBtScPairingSSPSendEvt;

typedef struct
{
	unsigned short int             type;
	TcBtResultCode			resultCode;
	TcBtSupplier			resultSupplier;
	TcBtBool				enabled;
} TcBtScDebugModeCfm;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_SC_ReceiveEvent(TcBtEvt *pTcBtEvt);

extern int TC_BT_SC_PasskeyInd(void *pTcBtEvt);
extern int TC_BT_SC_DebondCfm(void *pTcBtEvt);
extern int TC_BT_SC_AuthoriseInd(void *pTcBtEvt);
extern int TC_BT_SC_BondEvt(void *pTcBtEvt);
extern int TC_BT_SC_SSP_NotificationInd(void *pTcBtEvt);
extern int TC_BT_SC_SSP_CompareInd(void *pTcBtEvt);
extern int TC_BT_SC_SSP_JustworkInd(void *pTcBtEvt);
extern int TC_BT_SC_DebugModeCfm(void *pTcBtEvt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_SC_EVT_H__ */

