/****************************************************************************
 *   FileName    : TC_BT_EVT.h
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
*  Description : This file contains functions for a Bluetooth event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/07     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_EVT_H__
#define __TC_BT_EVT_H__

#include "inc/BtDataType.h"
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

typedef TcBtUint16 TcBtEvtType;
typedef enum
{
	TC_BT_EVT_TYPE_ENUM_BASE = 0x0,
	TC_BT_A2DP_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x100),
	TC_BT_AVRCP_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x200),
	TC_BT_HF_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x300),
	TC_BT_PAC_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x400),
	TC_BT_SPP_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x500),
	TC_BT_SC_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x600),
	TC_BT_SD_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x700),
	TC_BT_CM_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x800),
	TC_BT_MAPC_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0x900),
	TC_BT_HIDD_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0xA00),
	TC_BT_PAN_EVT_TYPE_ENUM = (TC_BT_EVT_TYPE_ENUM_BASE + 0xB00)	
} TcBtEvtTypeEnum;

typedef enum
{
	TC_BT_EVT_RESULT_SUCCESS_ENUM = 0,
	TC_BT_EVT_RESULT_NATIVE_OS_REJECT_ENUM,
	TC_BT_EVT_RESULT_HEAP_EXHAUSTED_ENUM,

	TC_BT_EVT_RESULT_NOT_SUPPORT_ENUM
} TcBtEvtResultEnum;

typedef int (*TcBtEvtCb)(void *pTcBtEvt);

typedef TcBtUint16 TcBtResultCode;
typedef TcBtUint16 TcBtReasonCode;
typedef TcBtUint16 TcBtSupplier;
typedef TcBtUint16 TcBtStatus;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define TC_BT_SYS_EVT (TC_BT_EVT_TYPE_ENUM_BASE)
#define TC_BT_A2DP_EVT (TC_BT_A2DP_EVT_TYPE_ENUM)
#define TC_BT_AVRCP_EVT (TC_BT_AVRCP_EVT_TYPE_ENUM)
#define TC_BT_HF_EVT (TC_BT_HF_EVT_TYPE_ENUM)
#define TC_BT_PAC_EVT (TC_BT_PAC_EVT_TYPE_ENUM)
#define TC_BT_SPP_EVT (TC_BT_SPP_EVT_TYPE_ENUM)
#define TC_BT_SC_EVT (TC_BT_SC_EVT_TYPE_ENUM)
#define TC_BT_SD_EVT (TC_BT_SD_EVT_TYPE_ENUM)
#define TC_BT_CM_EVT (TC_BT_CM_EVT_TYPE_ENUM)
#define TC_BT_MAPC_EVT (TC_BT_MAPC_EVT_TYPE_ENUM)
#define TC_BT_HIDD_EVT (TC_BT_HIDD_EVT_TYPE_ENUM)
#define TC_BT_PAN_EVT (TC_BT_PAN_EVT_TYPE_ENUM)

/* Generic broadcom stack error code */
#define TC_BT_SUCCESS                         0

/*
 * Client Error definition
 */
#define TC_BT_ERROR_CLI_BAD_RSP_SIZE          100 /* Client received Bad response Size */
#define TC_BT_ERROR_CLI_NYI                   101 /* Client API not Yet Implemented */
#define TC_BT_ERROR_CLI_DISC_BUSY             102 /* Discovery Client API Already running */
#define TC_BT_ERROR_CLI_RES_ERROR             103 /* Client API Resource error (e.g. Mutex) */
#define TC_BT_ERROR_CLI_BAD_PARAM             104 /* Client BSA API parameter */
#define TC_BT_ERROR_CLI_NOT_CONNECTED         105 /* Client Not connected to Server */
#define TC_BT_ERROR_CLI_BUSY                  106 /* Client API Busy Already running */
#define TC_BT_ERROR_CLI_UIPC_OPEN             107 /* Client Enable to open UIPC */
#define TC_BT_ERROR_CLI_UIPC_SEND             108 /* Client UIPC send error */
#define TC_BT_ERROR_CLI_BAD_MSG               109 /* Client received bad message */
#define TC_BT_ERROR_CLI_BAD_PING_RSP          110 /* Client received bad ping response */
#define TC_BT_ERROR_CLI_NAME_TOO_LONG         111 /* Client Name too long */
#define TC_BT_ERROR_CLI_ALREADY_WAITING       112 /* Client is trying to send a new request while already waiting for response */
#define TC_BT_ERROR_CLI_TASK_ERROR            113 /* Client could not create a task */
#define TC_BT_ERROR_CLI_INTERNAL              114 /* Internal Client error */
#define TC_BT_ERROR_CLI_ALREADY_CONNECTED     115 /* Client was already connected */
#define TC_BT_ERROR_CLI_MEM_FULL              116 /* Client No more memory */

/*
 * Server Error definition
 */

/*
 * Generic Server errors
 */
#define TC_BT_ERROR_SRV_BAD_PARAM             200 /* Server Bad parameter */
#define TC_BT_ERROR_SRV_BAD_REQ_SIZE          201 /* Server Bad Request Size */
#define TC_BT_ERROR_SRV_BAD_MSG_ID            202 /* Server Bad message identifier */
#define TC_BT_ERROR_SRV_ALREADY_ACTIVE        203 /* Server this profile is already active */
#define TC_BT_ERROR_SRV_NYI                   204 /* Server API not Yet Implemented */
#define TC_BT_ERROR_SRV_BAD_CLIENT            205 /* Server Bad client number */
#define TC_BT_ERROR_SRV_MEM_FULL              206 /* Server No more memory */
#define TC_BT_ERROR_SRV_HW_ERROR              207 /* Server HardWare error */
#define TC_BT_ERROR_SRV_BLUETOOTH_DISABLE     208 /* Bluetooth is disabled */
#define TC_BT_ERROR_SRV_INTERNAL              209 /* Internal error */
#define TC_BT_ERROR_SRV_NOT_COMPILED          210 /* Server Functionality not compiled */
#define TC_BT_ERROR_SRV_WLAN_RESET            211 /* Common USB is not available. WLAN RESET */
/**********************************************************************************************/

/* Result / Reason code supplier */
#define TC_BT_SUPPLIER_HCI                           ((TcBtSupplier) (0x0000))
#define TC_BT_SUPPLIER_DM                            ((TcBtSupplier) (0x0001))
#define TC_BT_SUPPLIER_L2CAP_CONNECT                 ((TcBtSupplier) (0x0002))
#define TC_BT_SUPPLIER_L2CAP_AMP_MOVE                ((TcBtSupplier) (0x0003))
#define TC_BT_SUPPLIER_L2CAP_DISCONNECT              ((TcBtSupplier) (0x0004))
#define TC_BT_SUPPLIER_L2CAP_DATA                    ((TcBtSupplier) (0x0005))
#define TC_BT_SUPPLIER_L2CAP_MISC                    ((TcBtSupplier) (0x0006))
#define TC_BT_SUPPLIER_SDP_SDS                       ((TcBtSupplier) (0x0007))
#define TC_BT_SUPPLIER_SDP_SDC                       ((TcBtSupplier) (0x0008))
#define TC_BT_SUPPLIER_SDP_SDC_OPEN_SEARCH           ((TcBtSupplier) (0x0009))
#define TC_BT_SUPPLIER_RFCOMM                        ((TcBtSupplier) (0x000A))
#define TC_BT_SUPPLIER_BCCMD                         ((TcBtSupplier) (0x000B))
#define TC_BT_SUPPLIER_CM                            ((TcBtSupplier) (0x000C))
#define TC_BT_SUPPLIER_IRDA_OBEX                     ((TcBtSupplier) (0x000D))
#define TC_BT_SUPPLIER_SPP                           ((TcBtSupplier) (0x000E))
#define TC_BT_SUPPLIER_SD                            ((TcBtSupplier) (0x000F))
#define TC_BT_SUPPLIER_HF                            ((TcBtSupplier) (0x0010))
#define TC_BT_SUPPLIER_AVRCP                         ((TcBtSupplier) (0x0011))
#define TC_BT_SUPPLIER_FAX_GW                        ((TcBtSupplier) (0x0012))
#define TC_BT_SUPPLIER_TCS                           ((TcBtSupplier) (0x0013))
#define TC_BT_SUPPLIER_AV                            ((TcBtSupplier) (0x0014))
#define TC_BT_SUPPLIER_BSL                           ((TcBtSupplier) (0x0015))
#define TC_BT_SUPPLIER_DUNC                          ((TcBtSupplier) (0x0016))
#define TC_BT_SUPPLIER_HDP                           ((TcBtSupplier) (0x0017))
#define TC_BT_SUPPLIER_HFG                           ((TcBtSupplier) (0x0018))
#define TC_BT_SUPPLIER_HIDD                          ((TcBtSupplier) (0x0019))
#define TC_BT_SUPPLIER_HIDH                          ((TcBtSupplier) (0x001A))
#define TC_BT_SUPPLIER_SAPC                          ((TcBtSupplier) (0x001B))
#define TC_BT_SUPPLIER_SAPS                          ((TcBtSupplier) (0x001C))
#define TC_BT_SUPPLIER_DG                            ((TcBtSupplier) (0x001D))
#define TC_BT_SUPPLIER_HCRP                          ((TcBtSupplier) (0x001E))
#define TC_BT_SUPPLIER_BNEP                          ((TcBtSupplier) (0x001F))
#define TC_BT_SUPPLIER_BNEP_CONNECT_CFM              ((TcBtSupplier) (0x0020))
#define TC_BT_SUPPLIER_SC                            ((TcBtSupplier) (0x0021))
#define TC_BT_SUPPLIER_MCAP                          ((TcBtSupplier) (0x0022))
#define TC_BT_SUPPLIER_MCAP_PROTOCOL                 ((TcBtSupplier) (0x0023))
#define TC_BT_SUPPLIER_OBEX_PROFILES                 ((TcBtSupplier) (0x0024))
#define TC_BT_SUPPLIER_JSR82                         ((TcBtSupplier) (0x0025))
#define TC_BT_SUPPLIER_AT                            ((TcBtSupplier) (0x0026))
#define TC_BT_SUPPLIER_A2DP                          ((TcBtSupplier) (0x0027))
#define TC_BT_SUPPLIER_AVCTP                         ((TcBtSupplier) (0x0028))
#define TC_BT_SUPPLIER_AVC                           ((TcBtSupplier) (0x0029))
#define TC_BT_SUPPLIER_AMPM                          ((TcBtSupplier) (0x002A))
#define TC_BT_SUPPLIER_PHDC                          ((TcBtSupplier) (0x002B))
#define TC_BT_SUPPLIER_PHDC_AG                       ((TcBtSupplier) (0x002C))
#define TC_BT_SUPPLIER_ATT                           ((TcBtSupplier) (0x002D))
#define TC_BT_SUPPLIER_GATT                          ((TcBtSupplier) (0x002E))
#define TC_BT_SUPPLIER_PROX_SRV                      ((TcBtSupplier) (0x002F))
#define TC_BT_SUPPLIER_THERM_SRV                     ((TcBtSupplier) (0x0030))
#define TC_BT_SUPPLIER_GNSS_CLIENT                   ((TcBtSupplier) (0x0031))
#define TC_BT_SUPPLIER_GNSS_SERVER                   ((TcBtSupplier) (0x0032))
#define TC_BT_SUPPLIER_RFC							 ((TcBtSupplier) (0x0033))
#define TC_BT_SUPPLIER_BSAS_CONNECT                  ((TcBtSupplier) (0x0034))
#define TC_BT_SUPPLIER_BSAS_CANCEL_CONNECT           ((TcBtSupplier) (0x0035))
#define TC_BT_SUPPLIER_BSAS_DISCONNECT               ((TcBtSupplier) (0x0036))

/* HCI result / reason code */
/* Bluetooth Specification V2.1 + EDR,
   Volume 2 Core System Package [Controller Volume],
   Part D Error Codes,
   1.2 HCI Command Errors */
#define TC_BT_HCI_SUCCESS                               ((TcBtResultCode)0x00)
#define TC_BT_HCI_ERROR_ILLEGAL_COMMAND                 ((TcBtResultCode)0x01)
#define TC_BT_HCI_ERROR_NO_CONNECTION                   ((TcBtResultCode)0x02)
#define TC_BT_HCI_ERROR_HARDWARE_FAIL                   ((TcBtResultCode)0x03)
#define TC_BT_HCI_ERROR_PAGE_TIMEOUT                    ((TcBtResultCode)0x04)
#define TC_BT_HCI_ERROR_AUTH_FAIL                       ((TcBtResultCode)0x05)
#define TC_BT_HCI_ERROR_KEY_MISSING                     ((TcBtResultCode)0x06)
#define TC_BT_HCI_ERROR_MEMORY_FULL                     ((TcBtResultCode)0x07)
#define TC_BT_HCI_ERROR_CONN_TIMEOUT                    ((TcBtResultCode)0x08)
#define TC_BT_HCI_ERROR_MAX_NR_OF_CONNS                 ((TcBtResultCode)0x09)
#define TC_BT_HCI_ERROR_MAX_NR_OF_SCO                   ((TcBtResultCode)0x0A)
#define TC_BT_HCI_ERROR_MAX_NR_OF_ACL                   ((TcBtResultCode)0x0B)
#define TC_BT_HCI_ERROR_COMMAND_DISALLOWED              ((TcBtResultCode)0x0C)
#define TC_BT_HCI_ERROR_REJ_BY_REMOTE_NO_RES            ((TcBtResultCode)0x0D)
#define TC_BT_HCI_ERROR_REJ_BY_REMOTE_SEC               ((TcBtResultCode)0x0E)
#define TC_BT_HCI_ERROR_REJ_BY_REMOTE_PERS              ((TcBtResultCode)0x0F)
#define TC_BT_HCI_ERROR_HOST_TIMEOUT                    ((TcBtResultCode)0x10)
#define TC_BT_HCI_ERROR_UNSUPPORTED_FEATURE             ((TcBtResultCode)0x11)
#define TC_BT_HCI_ERROR_ILLEGAL_FORMAT                  ((TcBtResultCode)0x12)
#define TC_BT_HCI_ERROR_OETC_USER                       ((TcBtResultCode)0x13)
#define TC_BT_HCI_ERROR_OETC_LOW_RESOURCE               ((TcBtResultCode)0x14)
#define TC_BT_HCI_ERROR_OETC_POWERING_OFF               ((TcBtResultCode)0x15)
#define TC_BT_HCI_ERROR_CONN_TERM_LOCAL_HOST            ((TcBtResultCode)0x16)
#define TC_BT_HCI_ERROR_AUTH_REPEATED                   ((TcBtResultCode)0x17)
#define TC_BT_HCI_ERROR_PAIRING_NOT_ALLOWED             ((TcBtResultCode)0x18)
#define TC_BT_HCI_ERROR_UNKNOWN_LMP_PDU                 ((TcBtResultCode)0x19)
#define TC_BT_HCI_ERROR_UNSUPPORTED_REM_FEATURE         ((TcBtResultCode)0x1A)
#define TC_BT_HCI_ERROR_SCO_OFFSET_REJECTED             ((TcBtResultCode)0x1B)
#define TC_BT_HCI_ERROR_SCO_INTERVAL_REJECTED           ((TcBtResultCode)0x1C)
#define TC_BT_HCI_ERROR_SCO_AIR_MODE_REJECTED           ((TcBtResultCode)0x1D)
#define TC_BT_HCI_ERROR_INVALID_LMP_PARAMETERS          ((TcBtResultCode)0x1E)
#define TC_BT_HCI_ERROR_UNSPECIFIED                     ((TcBtResultCode)0x1F)
#define TC_BT_HCI_ERROR_UNSUPP_LMP_PARAM                ((TcBtResultCode)0x20)
#define TC_BT_HCI_ERROR_ROLE_CHANGE_NOT_ALLOWED         ((TcBtResultCode)0x21)
#define TC_BT_HCI_ERROR_LMP_RESPONSE_TIMEOUT            ((TcBtResultCode)0x22)
#define TC_BT_HCI_ERROR_LMP_TRANSACTION_COLLISION       ((TcBtResultCode)0x23)
#define TC_BT_HCI_ERROR_LMP_PDU_NOT_ALLOWED             ((TcBtResultCode)0x24)
#define TC_BT_HCI_ERROR_ENC_MODE_NOT_ACCEPTABLE         ((TcBtResultCode)0x25)
#define TC_BT_HCI_ERROR_UNIT_KEY_USED                   ((TcBtResultCode)0x26)
#define TC_BT_HCI_ERROR_QOS_NOT_SUPPORTED               ((TcBtResultCode)0x27)
#define TC_BT_HCI_ERROR_INSTANT_PASSED                  ((TcBtResultCode)0x28)
#define TC_BT_HCI_ERROR_PAIR_UNIT_KEY_NO_SUPPORT        ((TcBtResultCode)0x29)
#define TC_BT_HCI_ERROR_DIFFERENT_TRANSACTION_COLLISION ((TcBtResultCode)0x2A)
#define TC_BT_HCI_ERROR_SCM_INSUFFICIENT_RESOURCES      ((TcBtResultCode)0x2B)
#define TC_BT_HCI_ERROR_QOS_UNACCEPTABLE_PARAMETER      ((TcBtResultCode)0x2C)
#define TC_BT_HCI_ERROR_QOS_REJECTED                    ((TcBtResultCode)0x2D)
#define TC_BT_HCI_ERROR_CHANNEL_CLASS_NO_SUPPORT        ((TcBtResultCode)0x2E)
#define TC_BT_HCI_ERROR_INSUFFICIENT_SECURITY           ((TcBtResultCode)0x2F)
#define TC_BT_HCI_ERROR_PARAM_OUT_OF_MAND_RANGE         ((TcBtResultCode)0x30)
#define TC_BT_HCI_ERROR_SCM_NO_LONGER_REQD              ((TcBtResultCode)0x31)
#define TC_BT_HCI_ERROR_ROLE_SWITCH_PENDING             ((TcBtResultCode)0x32)
#define TC_BT_HCI_ERROR_SCM_PARAM_CHANGE_PENDING        ((TcBtResultCode)0x33)
#define TC_BT_HCI_ERROR_RESVD_SLOT_VIOLATION            ((TcBtResultCode)0x34)
#define TC_BT_HCI_ERROR_ROLE_SWITCH_FAILED              ((TcBtResultCode)0x35)
#define TC_BT_HCI_ERROR_INQUIRY_RESPONSE_DATA_TOO_LARGE ((TcBtResultCode)0x36)
#define TC_BT_HCI_ERROR_SP_NOT_SUPPORTED_BY_HOST        ((TcBtResultCode)0x37)
#define TC_BT_HCI_ERROR_HOST_BUSY_PAIRING               ((TcBtResultCode)0x38)

#define TC_BT_LINK_STATUS_DISCONNECTED                  (0xAB)
#define TC_BT_LINK_STATUS_MAX_RECONNECT_TIME            (0xCA)
#define TC_BT_LINK_STATUS_CONNECTED                     (0x0001)
#define TC_BT_LINK_STATUS_HOLD                          (0x0002)
#define TC_BT_LINK_STATUS_SNIFF                         (0x0003)
#define TC_BT_LINK_STATUS_PARK                          (0x0004)

#define TC_BT_RESULT_CODE_OBEX_SUCCESS_RESPONSE_CODE          ((TcBtResultCode) (0x20))

#define TC_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED              ((TcBtResultCode) (0x0002))

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    TcBtEvtType                 type;
} TcBtEvt;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_SendEvent(TcBtEvt *pTcBtEvt);
extern unsigned long*  TC_BT_ReceiveEvent(void);
extern int TC_BT_InitEvent(void);
extern int TC_BT_CleanEvent(void);


#endif	/* __TC_BT_EVT_H__ */
