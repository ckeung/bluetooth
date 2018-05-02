/****************************************************************************
 *   FileName    : TC_BT_CM_EVT.h
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
*  Description : This file contains functions for a Bluetooth CM event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/15     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_CM_EVT_H__
#define __TC_BT_CM_EVT_H__

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
typedef enum
{
	TC_BT_CM_SET_LOCAL_NAME_CFM = TC_BT_CM_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x900 */
	TC_BT_CM_READ_LOCAL_BD_ADDR_CFM,
	TC_BT_CM_WRITE_SCAN_ENABLE_CFM,
	TC_BT_CM_READ_SCAN_ENABLE_CFM,
	TC_BT_CM_ENABLE_DUT_MODE_CFM,
	TC_BT_CM_WRITE_PAGE_TO_CFM,
	TC_BT_CM_WRITE_COD_CFM,
	TC_BT_CM_READ_COD_CFM,
	TC_BT_CM_READ_REMOTE_NAME_CFM,
	TC_BT_CM_SDC_CLOSE_IND,
	TC_BT_CM_WRITE_LINK_POLICY_ERROR_IND,
	TC_BT_CM_READ_LINK_POLICY_CFM,
	TC_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM,
	TC_BT_CM_GET_LINK_QUALITY_CFM,
	TC_BT_CM_READ_RSSI_CFM,
	TC_BT_CM_ACL_CONNECT_IND,
	TC_BT_CM_ACL_DISCONNECT_IND,
	TC_BT_CM_QOS_SETUP_CFM,
	TC_BT_CM_SET_AFH_CHANNELS_CFM,
	TC_BT_CM_RESET_CFM,
	TC_BT_CM_VSC_CFM,
	TC_BT_CM_BLUETEST_TX_CFM,	
	TC_BT_CM_BLUETEST_RX_CFM,	
	
	TC_BT_CM_EVT_TYPE_MAX
} TcBtCMEvtEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
/*-----------------------------------------------------------------------------*
 *
 *   Scan enable values 
 *
 *------------------------------------------------------------------------------*/
#define TC_BT_HCI_SCAN_ENABLE_OFF             ((TcBtUint8)0x00) /* default */
#define TC_BT_HCI_SCAN_ENABLE_INQ             ((TcBtUint8)0x01)
#define TC_BT_HCI_SCAN_ENABLE_PAGE            ((TcBtUint8)0x02)
#define TC_BT_HCI_SCAN_ENABLE_INQ_AND_PAGE    ((TcBtUint8)0x03)

#define TC_BT_MASTER_ROLE						((TcBtUint8) 0)
#define TC_BT_SLAVE_ROLE						((TcBtUint8) 1)
#define TC_BT_UNDEFINED_ROLE					((TcBtUint8) 2)

/* Success code and error codes */
#define  SDP_SUCCESS                        0x0000
#define  SDP_INVALID_VERSION                0x0001
#define  SDP_INVALID_SERV_REC_HDL           0x0002
#define  SDP_INVALID_REQ_SYNTAX             0x0003
#define  SDP_INVALID_PDU_SIZE               0x0004
#define  SDP_INVALID_CONT_STATE             0x0005
#define  SDP_NO_RESOURCES                   0x0006
#define  SDP_DI_REG_FAILED                  0x0007
#define  SDP_DI_DISC_FAILED                 0x0008
#define  SDP_NO_DI_RECORD_FOUND             0x0009
#define  SDP_ERR_ATTR_NOT_PRESENT           0x000A
#define  SDP_ILLEGAL_PARAMETER              0x000B

#define  SDP_NO_RECS_MATCH                  0xFFF0
#define  SDP_CONN_FAILED                    0xFFF1
#define  SDP_CFG_FAILED                     0xFFF2
#define  SDP_GENERIC_ERROR                  0xFFF3
#define  SDP_DB_FULL                        0xFFF4
#define  SDP_INVALID_PDU                    0xFFF5
#define  SDP_SECURITY_ERR                   0xFFF6
#define  SDP_CONN_REJECTED                  0xFFF7
#define  SDP_CANCEL                         0xFFF8

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef struct
{
    unsigned short int				type;
    TcBtStatus				status;
} TcBtCmCfm;

typedef struct
{
    unsigned short int				type;
    TcBtBdAddr				deviceAddr;
} TcBtCmReadLocalBdAddrCfm;

typedef struct
{
    unsigned short int				type;
    TcBtUint8				scanEnable;
    TcBtStatus				status;
} TcBtCmReadScanEnableCfm;

typedef struct
{
    unsigned short int				type;
    TcBtStatus				status;
} TcBtCmEnableDutModeCfm;

typedef struct
{
    unsigned short int				type;
    TcBtUint32				classOfDevice;
    TcBtStatus				status;
} TcBtCmReadCodCfm;

typedef struct
{
    unsigned short int				type;
    TcBtBdAddr				deviceAddr;
    TcBtString				*friendlyName;
    TcBtStatus				status;
} TcBtCmReadRemoteNameCfm;

typedef struct
{
	unsigned short int				type;
    TcBtStatus				status;
} TcBtCmSdcCloseInd;

typedef struct
{
    unsigned short int				type;
    TcBtBdAddr				deviceAddr;
    TcBtStatus				status;
} TcBtCmWriteLinkPolicyErrorInd, TcBtCmWriteLinkSupervTimeoutCfm;

typedef struct
{
	unsigned short int				type;
	TcBtBdAddr				deviceAddr;
	TcBtUint8				actualMode;
	TcBtUint16				linkPolicySetting;
    TcBtStatus				status;
} TcBtCmReadLinkPolicyCfm;

typedef struct
{
	unsigned short int				type;
	TcBtBdAddr				deviceAddr;
	TcBtUint8				linkQuality;
    TcBtStatus				status;
} TcBtCmGetLinkQualityCfm;

typedef struct
{
	unsigned short int				type;
	TcBtBdAddr				deviceAddr;
	TcBtInt8				rssi;
    TcBtStatus				status;
} TcBtCmReadRssiCfm;

typedef struct
{
    unsigned short int				type;
    TcBtBdAddr				deviceAddr;
    TcBtUint32				cod;
} TcBtCmAclConnectInd;

typedef struct
{
    unsigned short int				type;
    TcBtBdAddr				deviceAddr;
    TcBtStatus				status;
} TcBtCmAclDisconnectInd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern TcBtBdAddr gLocalAddrFromRead;

extern int TC_BT_CM_ReceiveEvent(TcBtEvt *pTcBtEvt);
extern int TC_BT_CM_ReadLocalBdAddrCfm(void *pTcBtEvt);
extern int TC_BT_CM_SetLocalNameCfm(void *pTcBtEvt);
extern int TC_BT_CM_WriteScanEnableCfm(void *pTcBtEvt);
extern int TC_BT_CM_ReadScanEnableCfm(void *pTcBtEvt);
extern int TC_BT_CM_EnableDutModeCfm(void *pTcBtEvt);
extern int TC_BT_CM_WritePageToCfm(void *pTcBtEvt);
extern int TC_BT_CM_WriteCodCfm(void *pTcBtEvt);
extern int TC_BT_CM_ReadCodCfm(void *pTcBtEvt);
extern int TC_BT_CM_ReadRemoteNameCfm(void *pTcBtEvt);
extern int TC_BT_CM_SdcCloseInd(void *pTcBtEvt);
extern int TC_BT_CM_WriteLinkPolicyErrorInd(void *pTcBtEvt);
extern int TC_BT_CM_ReadLinkPolicyCfm(void *pTcBtEvt);
extern int TC_BT_CM_WriteLinkSuperVisionTimeoutCfm(void *pTcBtEvt);
extern int TC_BT_CM_GetLinkQualityCfm(void *pTcBtEvt);
extern int TC_BT_CM_ReadRssiCfm(void *pTcBtEvt);
extern int TC_BT_CM_AclConnectInd(void *pTcBtEvt);
extern int TC_BT_CM_AclDisconnectInd(void *pTcBtEvt);
extern int TC_BT_CM_QOSSetupCfm(void *pTcBtEvt);
extern int TC_BT_CM_SetAfhChannelsCfm(void *pTcBtEvt);
extern int TC_BT_CM_ResetCfm(void *pTcBtEvt);
extern int TC_BT_CM_VSCCfm(void *pTcBtEvt);
extern int TC_BT_CM_BluetestTXCfm(void *pTcBtEvt);
extern int TC_BT_CM_BluetestRXCfm(void *pTcBtEvt);

extern void TC_BT_CM_SniffEnableTest(void);
extern void TC_BT_CM_ReadLinkPolicyTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_CM_EVT_H__ */
