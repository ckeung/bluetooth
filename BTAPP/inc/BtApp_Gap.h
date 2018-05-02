/****************************************************************************
 *   FileName    : BTAPP_GAP.h
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
*    11/01/06     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __BTAPP_GAP_H__
#define __BTAPP_GAP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "BT_CFG.h"
#include "TC_BT/inc/BtUtil.h"
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
#define BT_PROFILEMASK_HF		0x00000001
#define BT_PROFILEMASK_A2DP		0x00000002
#define BT_PROFILEMASK_AVRCP	0x00000004
#define BT_PROFILEMASK_PBAP		0x00000008
#define BT_PROFILEMASK_SPP		0x00000010
#define BT_PROFILEMASK_IAP		0x00000020
#define BT_PROFILEMASK_MAP		0x00000040
#define BT_PROFILEMASK_HIDD		0x00000080
#define BT_PROFILEMASK_PAN      0x00000100

#define BTAPP_GAP_RECOVERY_LINK_LOST_OUT	0
#define BTAPP_GAP_RECOVERY_LINK_LOST_IN		1

#define BTAPP_GAP_CONN_STATUS_IDLE				((TcBtUint16)0x0000)
#define BTAPP_GAP_CONN_STATUS_RAS				((TcBtUint16)0x0001)
#define BTAPP_GAP_CONN_STATUS_DELAYING			((TcBtUint16)0x0002)
#define BTAPP_GAP_CONN_STATUS_CONNECTING		((TcBtUint16)0x0003)

#define BTAPP_GAP_CCAN_STATUS_IDLE				((TcBtUint16)0x0000)
#define BTAPP_GAP_CCAN_STATUS_CANCELLING		((TcBtUint16)0x0001)

#define BTAPP_GAP_RAS_RESULT_SUCCESS			((TcBtUint16)0x0000)
#define BTAPP_GAP_RAS_RESULT_FAILURE			((TcBtUint16)0x0001)
#define BTAPP_GAP_RAS_RESULT_CANCELLED			((TcBtUint16)0x0002)

#define BTAPP_GAP_CONN_RESULT_SUCCESS			((TcBtUint16)0x0000)
#define BTAPP_GAP_CONN_RESULT_FAILURE			((TcBtUint16)0x0001)
#define BTAPP_GAP_CONN_RESULT_TIMEOUT			((TcBtUint16)0x0002)
#define BTAPP_GAP_CONN_RESULT_CANCELLED			((TcBtUint16)0x0003)
#define BTAPP_GAP_CONN_RESULT_CANCEL_FAILED		((TcBtUint16)0x0004)

#define BTAPP_GAP_DISC_STATUS_IDLE				((TcBtUint16)0x0000)
#define BTAPP_GAP_DISC_STATUS_DISCONNECTING		((TcBtUint16)0x0001)
#define BTAPP_GAP_DISC_STATUS_REFRESHING		((TcBtUint16)0x0002)

#define BTAPP_GAP_DISC_RESULT_SUCCESS			((TcBtUint16)0x0000)
#define BTAPP_GAP_DISC_RESULT_FAILURE			((TcBtUint16)0x0001)
#define BTAPP_GAP_DISC_RESULT_TIMEOUT			((TcBtUint16)0x0002)
#define BTAPP_GAP_DISC_RESULT_CONN_CANCELLED	((TcBtUint16)0x0003)
#define BTAPP_GAP_DISC_RESULT_CANCEL_FAILED		((TcBtUint16)0x0004)

#define BTAPP_GAP_DACT_RESULT_SUCCESS			((TcBtUint16)0x0000)
#define BTAPP_GAP_DACT_RESULT_FAILURE			((TcBtUint16)0x0001)

#define BTAPP_GAP_ACTV_RESULT_SUCCESS			((TcBtUint16)0x0000)
#define BTAPP_GAP_ACTV_RESULT_FAILURE			((TcBtUint16)0x0001)

#define BTAPP_GAP_UNUSED_INST_IDX				((TcBtUint8)0xff)

/* connection state for profile or service */
#define BTAPP_GAP_STATE_DISCONNECTED			((TcBtUint16)0x0000)
#define BTAPP_GAP_STATE_CONNECTING				((TcBtUint16)0x0001)
#define BTAPP_GAP_STATE_CONNECT_CANCELLING		((TcBtUint16)0x0002)
#define BTAPP_GAP_STATE_CONNECTED				((TcBtUint16)0x0003)
#define BTAPP_GAP_STATE_DISCONNECTING			((TcBtUint16)0x0004)

/* stream state for A2DP disconnection */
#define BTAPP_GAP_STREAM_STATE_CLOSED			((TcBtUint16)0x0000)
#define BTAPP_GAP_STREAM_STATE_CLOSING			((TcBtUint16)0x0001)
#define BTAPP_GAP_STREAM_STATE_OPENED			((TcBtUint16)0x0002)
#define BTAPP_GAP_STREAM_STATE_SUSPENDING		((TcBtUint16)0x0003)
#define BTAPP_GAP_STREAM_STATE_STREAMING		((TcBtUint16)0x0004)

/* connection status bit vector */
#define BTAPP_GAP_STATUS_IDLE					((TcBtUint16)0x0000)
#define BTAPP_GAP_STATUS_CONNECTING				((TcBtUint16)0x0001)
#define BTAPP_GAP_STATUS_CONNECT_CANCELLING		((TcBtUint16)0x0002)
#define BTAPP_GAP_STATUS_DISCONNECTING			((TcBtUint16)0x0004)

/* connection side (local or remote) */
#define BTAPP_GAP_CONN_SIDE_LOCAL				((TcBtUint16)0x0001)
#define BTAPP_GAP_CONN_SIDE_REMOTE				((TcBtUint16)0x0002)

/* send result flag bit */
#define BTAPP_GAP_SEND_CONN_RESULT				((TcBtUint16)0x0100)

#define BTAPP_GAP_HFP_MAX_CONN_INST				(2) /* refer to BT_HFP_MAX_CONNECTIONS at main.h */
#define BTAPP_GAP_MAPC_MAX_CONN_INST			(10)
#define BTAPP_GAP_SPP_MAX_CONN_INST				(3) /* refer to SPP_MAX_IDX at TC_BT_SPP_CMD.h */

#define BTAPP_GAP_ACT_CONNECT					((TcBtUint16)0x0000)
#define BTAPP_GAP_ACT_CANCEL_CONNECT			((TcBtUint16)0x0001)
#define BTAPP_GAP_ACT_DISCONNECT				((TcBtUint16)0x0002)

#define BTAPP_GAP_COND_UNKNOWN					((TcBtUint16)0x0000)
#define BTAPP_GAP_COND_NONE						((TcBtUint16)0x0001)
#define BTAPP_GAP_COND_ALLOWED					((TcBtUint16)0x0002)
#define BTAPP_GAP_COND_BUSY						((TcBtUint16)0x0003)

/* function result */
typedef enum
{
	BTAPP_GAP_FN_RESULT_SUCCESS = 0,
	BTAPP_GAP_FN_RESULT_INVALID_PARAM,
	BTAPP_GAP_FN_RESULT_ALREADY_EXIST,
	BTAPP_GAP_FN_RESULT_BUSY,
	BTAPP_GAP_FN_RESULT_NONE,
	BTAPP_GAP_FN_RESULT_DEV_NOT_FOUND,
	BTAPP_GAP_FN_RESULT_PROF_NOT_FOUND,
	BTAPP_GAP_FN_RESULT_NOT_FOUND,
	BTAPP_GAP_FN_RESULT_UNEXPECTED,

	BTAPP_GAP_FN_RESULT_MAX
} BtAppGapFnResult;

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct BtAppGapListElmTag
{
	struct BtAppGapListElmTag *next;
	struct BtAppGapListElmTag *prev;
} BtAppGapListElm_t;

typedef void (*BtAppGapListRemoveFunc_t)(BtAppGapListElm_t *elem);
typedef TcBtBool (*BtAppGapListSearchFunc_t)(const BtAppGapListElm_t *elem, void *value);

typedef struct BtAppGapListTag
{
	BtAppGapListElm_t *first;
	BtAppGapListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapList_t;

typedef struct BtAppGapConnProfListElmTag
{
	struct BtAppGapConnProfListElmTag *next;
	struct BtAppGapConnProfListElmTag *prev;
	struct BtAppGapConnDevListElmTag *pDevice;
	TcBtUint16 profileUuid;
	TcBtUint8 instanceIdx;		/* for multiple instance profile like MAP, SPP */
	TcBtUint16 timerSettingVal;	/* This value should be set properly depending on timer function which is used. */
	TcBtUint16 tryCount;
	TcBtUint16 valid;
} BtAppGapConnProfListElm_t;

typedef struct BtAppGapConnProfListTag
{
	BtAppGapConnProfListElm_t *first;
	BtAppGapConnProfListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapConnProfList_t;

typedef struct BtAppGapConnDevListElmTag
{
	struct BtAppGapConnDevListElmTag *next;
	struct BtAppGapConnDevListElmTag *prev;
	TcBtBdAddr bdAddr;
	TcBtUint32 *pServiceList;
	TcBtUint32 serviceCount;
	TcBtUint32 retryCountForRas;
	BtAppGapConnProfList_t connProfList;
} BtAppGapConnDevListElm_t;

typedef struct BtAppGapConnDevListTag
{
	BtAppGapConnDevListElm_t *first;
	BtAppGapConnDevListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapConnDevList_t;

typedef struct BtAppGapConnInstance_t
{
	TcBtBdAddr targetBdAddr;
	TcBtUint16 targetProfile;
	TcBtUint8 targetInstanceIdx;
	TcBtUint16 connStatus;
	BtAppGapConnDevList_t connDevList;
} BtAppGapConnInstance_t;

typedef struct
{
	TcBtUint16 profileUuid;
	TcBtUint8 instanceIdx;
} BtAppGapProfUuidInstIdx;

/* Disconnection Process */
typedef struct BtAppGapDiscProfListElmTag
{
	struct BtAppGapDiscProfListElmTag *next;
	struct BtAppGapDiscProfListElmTag *prev;
	struct BtAppGapDiscDevListElmTag *pDevice;
	TcBtUint16 profileUuid;
	TcBtUint8 instanceIdx;		/* for multiple instance profile like MAP, SPP */
	TcBtUint16 timerSettingVal;	/* This value should be set properly depending on timer function which is used. */
	TcBtUint16 tryCount;
} BtAppGapDiscProfListElm_t;

typedef struct BtAppGapDiscProfListTag
{
	BtAppGapDiscProfListElm_t *first;
	BtAppGapDiscProfListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapDiscProfList_t;

typedef struct BtAppGapDiscDevListElmTag
{
	struct BtAppGapDiscDevListElmTag *next;
	struct BtAppGapDiscDevListElmTag *prev;
	TcBtBdAddr bdAddr;
	BtAppGapDiscProfList_t readyToDiscProfList;
	BtAppGapDiscProfList_t waitForRespProfList;
} BtAppGapDiscDevListElm_t;

typedef struct BtAppGapDiscDevListTag
{
	BtAppGapDiscDevListElm_t *first;
	BtAppGapDiscDevListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapDiscDevList_t;

typedef struct BtAppGapDiscInstance_t
{
	TcBtUint16 discStatus;
	TcBtUint8 discAllFlag;

	/* reactivation */
	BtAppGapDiscProfList_t waitingForResponseProfList;
	BtAppGapDiscProfList_t resultSuccessProfList;
	BtAppGapDiscProfList_t resultFailureProfList;
	
	BtAppGapDiscDevList_t discDevList;
} BtAppGapDiscInstance_t;

typedef struct BtAppGapCcanInstance_t
{
	TcBtBdAddr targetBdAddr;
	TcBtUint16 targetProfile;
	TcBtUint8 targetInstanceIdx;
	TcBtUint16 ccanStatus;
} BtAppGapCcanInstance_t;

/* Recovery Process */
typedef struct BtAppGapRecoveryListElmTag
{
	struct BtAppGapRecoveryListElmTag *next;
	struct BtAppGapRecoveryListElmTag *prev;
	TcBtUint16 profileUuid;
	TcBtUint8 instanceIdx;		/* for multiple instance profile like MAP, SPP */
} BtAppGapRecoveryListElm_t;

typedef struct BtAppGapRecoveryListTag
{
	BtAppGapRecoveryListElm_t *first;
	BtAppGapRecoveryListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapRecoveryList_t;

typedef struct BtAppGapRecoveryInstance_t
{
	/* for deactivation */
	BtAppGapRecoveryList_t waitingForDactRespList;
	BtAppGapRecoveryList_t dactResultSuccessList;
	BtAppGapRecoveryList_t dactResultFailureList;

	/* for activation */
	BtAppGapRecoveryList_t waitingForActRespList;
	BtAppGapRecoveryList_t actResultSuccessList;
	BtAppGapRecoveryList_t actResultFailureList;

	/* for bluetooth off/on */
	TcBtBool doOffOn;
} BtAppGapRecoveryInstance_t;

typedef struct BtAppGapDevConnStatusListElmTag
{
	struct BtAppGapDevConnStatusListElmTag *next;
	struct BtAppGapDevConnStatusListElmTag *prev;
	TcBtBdAddr bdAddr;
	TcBtUint8 status;
	TcBtUint16 connSide;
	TcBtUint16 ccanSide;
	TcBtUint16 discSide;
} BtAppGapDevConnStatusListElm_t;

typedef struct BtAppGapDevConnStatusListTag
{
	BtAppGapDevConnStatusListElm_t *first;
	BtAppGapDevConnStatusListElm_t *last;
	TcBtUint16 count;
	BtAppGapListRemoveFunc_t removeFunc;
} BtAppGapDevConnStatusList_t;

typedef struct BtAppGapHfpConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapHfpConnInst_t;

typedef struct BtAppGapHfpConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapHfpConnInst_t connInst[BTAPP_GAP_HFP_MAX_CONN_INST];
} BtAppGapHfpConnStatusInfo_t;

typedef struct BtAppGapA2dpConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
	TcBtUint16 streamState; /* reserved */
} BtAppGapA2dpConnInst_t;

typedef struct BtAppGapA2dpConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapA2dpConnInst_t connInst;
} BtAppGapA2dpConnStatusInfo_t;

typedef struct BtAppGapAvrcpConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapAvrcpConnInst_t;

typedef struct BtAppGapAvrcpConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapAvrcpConnInst_t connInst;
} BtAppGapAvrcpConnStatusInfo_t;

typedef struct BtAppGapPacConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapPacConnInst_t;

typedef struct BtAppGapPacConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapPacConnInst_t connInst;
} BtAppGapPacConnStatusInfo_t;

typedef struct BtAppGapMapcConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapMapcConnInst_t;

typedef struct BtAppGapMapcConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapMapcConnInst_t connInst[BTAPP_GAP_MAPC_MAX_CONN_INST];
} BtAppGapMapcConnStatusInfo_t;
 
typedef struct BtAppGapSppConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapSppConnInst_t;

typedef struct BtAppGapSppConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapSppConnInst_t connInst[BTAPP_GAP_SPP_MAX_CONN_INST];
} BtAppGapSppConnStatusInfo_t;

typedef struct BtAppGapIapConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapIapConnInst_t;

typedef struct BtAppGapIapConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapIapConnInst_t connInst;
} BtAppGapIapConnStatusInfo_t;

typedef struct BtAppGapHiddConnInstTag
{
	TcBtBdAddr bdAddr;
	TcBtUint16 state;
} BtAppGapHiddConnInst_t;

typedef struct BtAppGapHiddConnStatusInfoTag
{
	TcBtUint16 idleInstCount;
	BtAppGapHiddConnInst_t connInst;
} BtAppGapHiddConnStatusInfo_t;

typedef struct BtAppGapPanConnInstTag
{
    TcBtBdAddr bdAddr;
    TcBtUint16 state;
} BtAppGapPanConnInst_t;

typedef struct BtAppGapPanConnStatusInfoTag
{
    TcBtUint16 idleInstCount;
    BtAppGapPanConnInst_t connInst;
} BtAppGapPanConnStatusInfo_t;
 
typedef struct BtAppGapMainConnStatusInst_t
{
	TcBtUint16 overallStatus;
	TcBtUint8 connNum;
	TcBtUint8 ccanNum;
	TcBtUint8 discNum;
	BtAppGapDevConnStatusList_t devConnStatusList;
	
	BtAppGapHfpConnStatusInfo_t hfp;
	BtAppGapA2dpConnStatusInfo_t a2dp;
	BtAppGapAvrcpConnStatusInfo_t avrcp;
	BtAppGapPacConnStatusInfo_t pac;
	BtAppGapMapcConnStatusInfo_t mapc;
	BtAppGapSppConnStatusInfo_t spp;
	BtAppGapIapConnStatusInfo_t iap;
	BtAppGapHiddConnStatusInfo_t hidd;
    BtAppGapPanConnStatusInfo_t pan;
} BtAppGapMainConnStatusInst_t;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern void BtApp_GAP_SetMultiProfileActivation(unsigned int profile);
extern unsigned int BtApp_GAP_GetMultiProfileActivation(void);
extern void BtApp_GAP_ClearMultiProfileActivation(unsigned int profile);
extern void BtApp_GAP_SetAllProfileActivation(void);
extern void BtApp_GAP_ActivateProfilesAutomatically(void);
#if 0
extern void BtApp_GAP_A2DP_StopStreamForTimer(unsigned char id, unsigned char loc);
#endif
extern void BtApp_GAP_A2DP_DisconnectForTimer(unsigned char id, unsigned char loc);
extern int BtApp_GAP_DisconnectAllAtBTDeactivate(void);

extern void BtApp_GAP_InitAclConnList(void);
extern int BtApp_GAP_GetAclConnCnt(void);
extern int BtApp_GAP_IsInAclConnList(const TcBtBdAddr *pBdAddr);
extern int BtApp_GAP_AddToAclConnList(const TcBtBdAddr *pBdAddr);
extern int BtApp_GAP_RemoveFromAclConnList(const TcBtBdAddr *pBdAddr);

#ifdef BT_AUTO_CONNECTION
extern void BtApp_GAP_AutoConnect(void);
extern void BtApp_GAP_SetRecoveryLinkLostFlag(TcBtUint8 Flag);
extern TcBtUint8 BtApp_GAP_GetRecoveryLinkLostFlag(void);
extern void BtApp_GAP_RecoveryLinkLostEvent(void);
extern void BtApp_GAP_RecoveryLinkLostCancelTimer(void);
extern void BtApp_GAP_RecoveryLinkLostCancelChkProximity(void);
extern void BtApp_GAP_RecoveryLinkLostCancelChkProximityForTimer(unsigned char id, unsigned char loc);
extern void BtApp_GAP_RecoveryLinkLostStop(void);
#endif

extern void BtApp_GAP_SetAuthoriseFlag(TcBtBool bAuthorFlag);
extern TcBtBool BtApp_GAP_GetAuthoriseFlag(void);

/* for mainConnStatus */
extern void BtApp_GAP_InitMainConnStatusInst(void);
extern TcBtUint16 BtApp_GAP_CheckCondition(const TcBtBdAddr *pBdAddr, const TcBtUint16 action, const TcBtUint16 side);
extern TcBtUint16 BtApp_GAP_CheckConditionForConnectByLocal(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_CheckConditionForConnectByRemote(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_CheckConditionForCancelConnectByLocal(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_CheckConditionForCancelConnectByRemote(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_CheckConditionForDisconnectByLocal(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_CheckConditionForDisconnectByRemote(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_SetMainConnStatus(const TcBtBdAddr *pBdAddr, const TcBtUint16 status, const TcBtUint16 side);
extern void BtApp_GAP_UnsetMainConnStatus(const TcBtBdAddr *pBdAddr, const TcBtUint16 status);
extern TcBtInt32 BtApp_GAP_AddToDevConnStatusList(const BtAppGapDevConnStatusListElm_t *pGivenDevConnStatus);
extern TcBtInt32 BtApp_GAP_RemoveFromDevConnStatusList(const TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetOverallConnStatus(void);
extern BtAppGapDevConnStatusListElm_t *BtApp_GAP_GetDevConnStatus(const TcBtBdAddr *pBdAddr);

/* for Profile State */
extern TcBtUint16 BtApp_GAP_GetHfpState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx);
extern TcBtUint32 BtApp_GAP_SetHfpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetHfpBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetA2dpState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetA2dpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetA2dpBdAddr(TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetA2dpStreamState(const TcBtBdAddr *pBdAddr);
extern TcBtInt32 BtApp_GAP_SetA2dpStreamState(const TcBtBdAddr *pBdAddr, TcBtUint16 streamState);
extern TcBtUint16 BtApp_GAP_GetAvrcpState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetAvrcpState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetAvrcpBdAddr(TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetPacState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetPacState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetPacBdAddr(TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetMapcState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx);
extern TcBtUint32 BtApp_GAP_SetMapcState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetMapcBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetSppState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx);
extern TcBtUint32 BtApp_GAP_SetSppState(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetSppBdAddrByIdx(const TcBtUint8 instanceIdx, TcBtBdAddr *pBdAddr);
extern TcBtUint8 BtApp_GAP_GetSppIdleInstIdx(void);
extern TcBtUint16 BtApp_GAP_GetIapState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetIapState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetIapBdAddr(TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetHiddState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetHiddState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetHiddBdAddr(TcBtBdAddr *pBdAddr);
extern TcBtUint16 BtApp_GAP_GetPANState(const TcBtBdAddr *pBdAddr);
extern TcBtUint32 BtApp_GAP_SetPANState(const TcBtBdAddr *pBdAddr, const TcBtUint16 state);
extern TcBtUint32 BtApp_GAP_GetPANBdAddr(TcBtBdAddr *pBdAddr);

/* for Connection & Connect_Cancellation Process */
extern TcBtBdAddr *BtApp_GAP_GetCcanBdAddr(void);
extern void BtApp_GAP_InitConnInst(void);
extern void BtApp_GAP_InitCcanInst(void);
extern TcBtInt32 BtApp_GAP_AddToConnProfList(const TcBtBdAddr *pBdAddr, const BtAppGapConnProfListElm_t *pConnProfile);
extern TcBtInt32 BtApp_GAP_RemoveFromConnProfList(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern void BtApp_GAP_PairingIndHandler(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_BondFailHandler(const TcBtBdAddr *pBdAddr);
extern BtAppGapConnProfListElm_t *BtApp_GAP_GetConnProfile(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern TcBtInt32 BtApp_GAP_SetConnProfile(const TcBtBdAddr *pBdAddr, const BtAppGapConnProfListElm_t *pConnProfile);
extern void BtApp_GAP_AvoidConnectConflict(const TcBtBdAddr *pBdAddr, TcBtUint16 incomingProfUuid);
extern TcBtInt32 BtApp_GAP_Connect(void);
extern TcBtInt32 BtApp_GAP_CancelConnect(const TcBtBdAddr *pBdAddr);
extern TcBtInt32 BtApp_GAP_FullConnectionProcess(const TcBtBdAddr *pBdAddr, TcBtUint16 side);
extern void BtApp_GAP_ObexSubConnectionProcess(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_IncomingConnectionBackupProcess(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_AvrcpSubConnectionProcess(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_PacConnect(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_MapcConnect(const TcBtBdAddr *pBdAddr, const TcBtUint8 instanceIdx);
extern TcBtInt32 BtApp_GAP_ReflectConnResult(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx);
extern TcBtInt32 BtApp_GAP_ValidateConnProfList(const TcBtBdAddr *pBdAddr, const TcBtUint32 *pServiceList, TcBtUint32 serviceCount, TcBtUint16 result);

/* for Disconnection Process */
extern void BtApp_GAP_InitDiscInst(void);
extern TcBtInt32 BtApp_GAP_AddToDiscProfList(const TcBtBdAddr *pBdAddr, const BtAppGapDiscProfListElm_t *pDiscProfile);
extern TcBtInt32 BtApp_GAP_AddToDiscProfListFirst(const TcBtBdAddr *pBdAddr, const BtAppGapDiscProfListElm_t *pDiscProfile);
extern TcBtInt32 BtApp_GAP_RemoveFromDiscProfList(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfile(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfileFromRTD(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern BtAppGapDiscProfListElm_t *BtApp_GAP_GetDiscProfileFromWFR(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint8 instanceIdx);
extern TcBtInt32 BtApp_GAP_Disconnect(void);
extern TcBtInt32 BtApp_GAP_ReflectDiscResult(const TcBtBdAddr *pBdAddr, TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx);
extern void BtApp_GAP_ReflectActivateResult(TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx);
extern void BtApp_GAP_ReflectDeactivateResult(TcBtUint16 profileUuid, TcBtUint16 result, TcBtUint8 instanceIdx);
extern TcBtInt32 BtApp_GAP_DisconnectionProcess(const TcBtBdAddr *pBdAddr);
extern TcBtInt32 BtApp_GAP_DisconnectionAllProcess(void);
extern void BtApp_GAP_AvrcpSubDisconnectionProcess(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_ObexSubDisconnectionProcess(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_PacDisconnect(const TcBtBdAddr *pBdAddr);
extern void BtApp_GAP_MapcDisconnect(const TcBtBdAddr *pBdAddr, const TcBtUint16 side);

/* for recovery process */
extern void BtApp_GAP_InitRecoveryInst(void);
extern void BtApp_GAP_SetBtOffOnFlag(TcBtBool flag);
extern TcBtBool BtApp_GAP_GetBtOffOnFlag(void);
extern void BtApp_GAP_CheckBtOffOn(void);
extern TcBtBool BtApp_GAP_IsInRecoveryList(TcBtUint16 profileUuid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __BTAPP_GAP_H__ */
