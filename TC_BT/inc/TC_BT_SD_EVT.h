/****************************************************************************
 *   FileName    : TC_BT_SD_EVT.h
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
*  Description : This file contains functions for a Bluetooth SD event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/19     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_SD_EVT_H__
#define __TC_BT_SD_EVT_H__

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
    TC_BT_SD_SEARCH_RESULT_IND = TC_BT_SD_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x800 */
    TC_BT_SD_CLOSE_SEARCH_IND,
    TC_BT_SD_READ_AVAILABLE_SERVICES_CFM,
    TC_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM,

    /* Device Identification */
    TC_BT_SD_DI_READ_SERVICE_RECORD_IND,
    TC_BT_SD_DI_READ_SERVICE_RECORD_CFM,
    TC_BT_SD_DI_REGISTER_SERVICE_RECORD_CFM,

    TC_BT_SD_EVT_TYPE_MAX
} TcBtSDEvtEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* UUIDs for specified protocols */
#define TC_BT_PROTOCOL_AVCTP_UUID                ((TcBtUint16) 0x0017)
#define TC_BT_PROTOCOL_AVDTP_UUID                ((TcBtUint16) 0x0019)
#define TC_BT_PROTOCOL_MCAP_CONTROL_UUID         ((TcBtUint16) 0x001E)
#define TC_BT_PROTOCOL_MCAP_DATA_UUID            ((TcBtUint16) 0x001F)

/* UUIDs for specified service classes */
#define TC_BT_OBEX_SYNCML_TRANSFER_UUID          ((TcBtUint16) 0x1000)	/* Watch out: This UUID is not defined by TC */
#define TC_BT_SPP_IAP_SERVICE_UUID               ((TcBtUint16) 0x1001)	/* Watch out: This UUID is not defined by TC */
#define TC_BT_ATCMD_PB_SERVICE_UUID              ((TcBtUint16) 0x1002)	/* Watch out: This UUID is not defined by TC */

#define TC_BT_SPP_PROFILE_UUID                   ((TcBtUint16) 0x1101)
#define TC_BT_LAP_PROFILE_UUID                   ((TcBtUint16) 0x1102)
#define TC_BT_DUN_PROFILE_UUID                   ((TcBtUint16) 0x1103)
#define TC_BT_OBEX_IR_MC_SYNC_SERVICE_UUID       ((TcBtUint16) 0x1104)
#define TC_BT_OBEX_OBJECT_PUSH_SERVICE_UUID      ((TcBtUint16) 0x1105)
#define TC_BT_OBEX_FILE_TRANSFER_UUID            ((TcBtUint16) 0x1106)
#define TC_BT_HS_PROFILE_UUID                    ((TcBtUint16) 0x1108)
#define TC_BT_CTP_PROFILE_UUID                   ((TcBtUint16) 0x1109)
#define TC_BT_AUDIO_SOURCE_UUID                  ((TcBtUint16) 0x110A)
#define TC_BT_AUDIO_SINK_UUID                    ((TcBtUint16) 0x110B)
#define TC_BT_AV_REMOTE_CONTROL_TARGET_UUID      ((TcBtUint16) 0x110C)
#define TC_BT_ADVANCED_AUDIO_PROFILE_UUID        ((TcBtUint16) 0x110D)
#define TC_BT_AV_REMOTE_CONTROL_UUID             ((TcBtUint16) 0x110E)
#define TC_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID  ((TcBtUint16) 0x110F)
#define TC_BT_ICP_PROFILE_UUID                   ((TcBtUint16) 0x1110)
#define TC_BT_FAX_PROFILE_UUID                   ((TcBtUint16) 0x1111)
#define TC_BT_HEADSET_AG_SERVICE_UUID            ((TcBtUint16) 0x1112)
#define TC_BT_PAN_PANU_PROFILE_UUID              ((TcBtUint16) 0x1115)
#define TC_BT_PAN_NAP_PROFILE_UUID               ((TcBtUint16) 0x1116)
#define TC_BT_PAN_GN_PROFILE_UUID                ((TcBtUint16) 0x1117)
#define TC_BT_DIRECT_PRINTING_UUID               ((TcBtUint16) 0x1118)
#define TC_BT_REFERENCE_PRINTING_UUID            ((TcBtUint16) 0x1119)
#define TC_BT_OBEX_IMAGING_UUID                  ((TcBtUint16) 0x111A)
#define TC_BT_OBEX_IMAGING_RESPONDER_UUID        ((TcBtUint16) 0x111B)
#define TC_BT_OBEX_IMAGING_AUTOMATIC_ARCHIVE_UUID ((TcBtUint16) 0x111C)
#define TC_BT_HF_PROFILE_UUID                    ((TcBtUint16) 0x111E)
#define TC_BT_HFG_PROFILE_UUID                   ((TcBtUint16) 0x111F)
#define TC_BT_DIRECT_PRINTING_REFERENCE_OBJ_UUID ((TcBtUint16) 0x1120)
#define TC_BT_BASIC_PRINTING_UUID                ((TcBtUint16) 0x1122)
#define TC_BT_PRINTING_STATUS_UUID               ((TcBtUint16) 0x1123)
#define TC_BT_HID_PROFILE_UUID                   ((TcBtUint16) 0x1124)
#define TC_BT_HCR_PROFILE_UUID                   ((TcBtUint16) 0x1125)
#define TC_BT_SIM_ACCESS_PROFILE_UUID            ((TcBtUint16) 0x112D)
#define TC_BT_OBEX_PBA_CLIENT_PROFILE_UUID       ((TcBtUint16) 0x112E)
#define TC_BT_OBEX_PBA_SERVER_PROFILE_UUID       ((TcBtUint16) 0x112F)
#define TC_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID         ((TcBtUint16) 0x1132)
#define TC_BT_OBEX_MESSAGE_NOTIFICATION_SERVER_UUID   ((TcBtUint16) 0x1133)
#define TC_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID        ((TcBtUint16) 0x1134)
#define TC_BT_PNP_INFORMATION_UUID               ((TcBtUint16) 0x1200)
#define TC_BT_VIDEO_SOURCE_UUID                  ((TcBtUint16) 0x1303)
#define TC_BT_VIDEO_SINK_UUID                    ((TcBtUint16) 0x1304)
#define TC_BT_VIDEO_DISTRIBUTION_UUID            ((TcBtUint16) 0x1305)
#define TC_BT_HDP_PROFILE_UUID                   ((TcBtUint16) 0x1400)
#define TC_BT_HDP_SOURCE_UUID                    ((TcBtUint16) 0x1401)
#define TC_BT_HDP_SINK_UUID                      ((TcBtUint16) 0x1402)
#define TC_BT_PUBLIC_BROWSE_GROUP_UUID           ((TcBtUint16) 0x1002)

/* ---------- Defines the Service Manager (SD) TcBtStatus Code ----------*/
/*
 * Discovery error
 */
#define TC_BT_ERROR_SRV_DEV_DISC_BUSY         900 /* Device Discovery already ongoing */
#define TC_BT_ERROR_SRV_DEV_INFO_BUSY         901 /* Device Info already ongoing */
#define TC_BT_ERROR_SRV_DISC_ABORT_ERROR      902 /* Device Discovery Cancel not allowed */
#define TC_BT_ERROR_SRV_SDP_ERROR      		  903 /* SDP error */

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef struct
{
	unsigned short int				type;
} TcBtSdEvt;

typedef struct
{
	unsigned short int				type;
	TcBtBdAddr				deviceAddr;
	TcBtUint32				deviceClass;
	char					rssi;
	TcBtString				*deviceName;	
} TcBtSdSearchResultInd;

typedef struct
{
	unsigned short int				type;
	TcBtStatus				status;
} TcBtSdCloseSearchInd, TcBtSdDiReadServiceRecordCfm;

typedef struct
{
	unsigned short int				type;
	TcBtStatus				status;
	TcBtBdAddr				deviceAddr;
	TcBtUint32				deviceClass;
	TcBtUint32				*ServiceList;
	TcBtUint32				ServiceCount;
	TcBtString				*deviceName;
} TcBtSdReadAvailableServicesCfm;

typedef struct
{
	unsigned short int				type;
	TcBtUint16				dataLen;
	TcBtUint8				*data;
} TcBtSdDiReadServiceRecordInd;

typedef struct
{
	unsigned short int				type;
	TcBtUint32				serviceHandle;
	TcBtStatus				status;
} TcBtSdDiRegisterServiceRecordCfm;

typedef struct
{
	TcBtBool   	specificationIdValid;
	TcBtUint16 	specificationIdValue;

	TcBtBool   	vendorIdValid;
	TcBtUint16 	vendorIdValue;

	TcBtBool   	productIdValid;
	TcBtUint16 	productIdValue;

	TcBtBool   	versionValid;
	TcBtUint16 	versionValue;

	TcBtBool   	primaryRecordValid;
	TcBtBool   	primaryRecordValue;

	TcBtBool   	vendorIdSourceValid;
	TcBtUint16 	vendorIdSourceValue;

	TcBtBool    clientExecutableUrlValid;
	TcBtUint8  	*clientExecutableUrlValue;
	TcBtUint16  clientExecutableUrlValueLen;

	TcBtBool    serviceDescriptionValid;
	TcBtUint8  	*serviceDescriptionValue;
	TcBtUint16  serviceDescriptionValueLen;

	TcBtBool    documentationUrlValid;
	TcBtUint8  	*documentationUrlValue;
	TcBtUint16  documentationUrlValueLen;
} TcBtSdDiServiceRecordV13Struct;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_SD_EVT_H__ */

