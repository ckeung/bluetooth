/****************************************************************************
 *   FileName    : TC_BT_CM_CMD.h
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
*  Description : This file contains functions for a Bluetooth Connection Manager command
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/15     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_CM_CMD_H__
#define __TC_BT_CM_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtBase.h"
#include "BtUtil.h"
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
	TC_BT_CM_SET_LOCAL_NAME_REQ = 0,
	TC_BT_CM_READ_LOCAL_BD_ADDR_REQ,
	TC_BT_CM_WRITE_SCAN_ENABLE_REQ,
	TC_BT_CM_READ_SCAN_ENABLE_REQ,
	TC_BT_CM_ENABLE_DUT_MODE_REQ,
	TC_BT_CM_WRITE_PAGE_TO_REQ,
	TC_BT_CM_WRITE_COD_REQ,
	TC_BT_CM_READ_COD_REQ,
	TC_BT_CM_READ_REMOTE_NAME_REQ,
	TC_BT_CM_CANCEL_READ_REMOTE_NAME_REQ,
	TC_BT_CM_SDC_SEARCH_REQ,
	TC_BT_CM_SDC_CANCEL_SEARCH_REQ,
	TC_BT_CM_SDC_UUID128_SEARCH_REQ,
	TC_BT_CM_SDC_CANCEL_UUID128_SEARCH_REQ,
	TC_BT_CM_WRITE_LINK_POLICY_REQ,
	TC_BT_CM_READ_LINK_POLICY_REQ,
	TC_BT_CM_SET_MODE_CHANGE_EVENT_MASK_REQ,
	TC_BT_CM_UNSET_MODE_CHANGE_EVENT_MASK_REQ,
	TC_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ,
	TC_BT_CM_ROLE_SWITCH_CONFIG_REQ,
	TC_BT_CM_GET_LINK_QUALITY_REQ,
	TC_BT_CM_READ_RSSI_REQ,
	TC_BT_CM_ACL_DETACH_REQ,
	TC_BT_CM_SET_QOS_SETUP_REQ,
	TC_BT_CM_SET_AFH_CHANNELS_REQ,
	TC_BT_CM_RESET,
	TC_BT_CM_VSC_CMD,
	TC_BT_CM_BLUETEST_TX_REQ,
	TC_BT_CM_BLUETEST_RX_REQ
} TcBtCmCmdEnum;

typedef enum
{
	TC_BT_CM_UNKNOWNSTATUS = 0,
	TC_BT_CM_DISCONNECTED,
	TC_BT_CM_CONNECTING,
	TC_BT_CM_CONNECTED,
	TC_BT_CM_EXTRA
} TC_BT_CM_ServiceStatus;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* Major Service Mask */
#define TC_BT_COD_LIMITED_DISCOVERABLE_MODE_MAJOR_SERVICE_MASK            (0x002000)
#define TC_BT_COD_NETWORKING_MAJOR_SERVICE_MASK                           (0x020000)
#define TC_BT_COD_RENDERING_MAJOR_SERVICE_MASK                            (0x040000)
#define TC_BT_COD_CAPTURING_MAJOR_SERVICE_MASK                            (0x080000)
#define TC_BT_COD_OBJECT_TRANSFER_MAJOR_SERVICE_MASK                      (0x100000)
#define TC_BT_COD_AUDIO_MAJOR_SERVICE_MASK                                (0x200000)
#define TC_BT_COD_TELEPHONY_MAJOR_SERVICE_MASK                            (0x400000)
#define TC_BT_COD_INFORMATION_MAJOR_SERVICE_MASK                          (0x800000)
#define TC_BT_COD_SIGNIFICANT_MAJOR_SERVICE_MASK                          (0xFE2000)

/* Major Device Mask */
#define TC_BT_COD_MISCELLANEOUS_MAJOR_DEVICE_MASK                         (0x000000)
#define TC_BT_COD_COMPUTER_MAJOR_DEVICE_MASK                              (0x000100)
#define TC_BT_COD_PHONE_MAJOR_DEVICE_MASK                                 (0x000200)
#define TC_BT_COD_LAN_MAJOR_DEVICE_MASK                                   (0x000300)
#define TC_BT_COD_AV_MAJOR_DEVICE_MASK                                    (0x000400)
#define TC_BT_COD_PERIPHERAL_MAJOR_DEVICE_MASK                            (0x000500)
#define TC_BT_COD_IMAGING_MAJOR_DEVICE_MASK                               (0x000600)
#define TC_BT_COD_WEARABLE_MAJOR_DEVICE_MASK                              (0x000700)
#define TC_BT_COD_TOY_MAJOR_DEVICE_MASK                                   (0x000800)
#define TC_BT_COD_HEALTH_MAJOR_DEVICE_MASK                                (0x000900)
#define TC_BT_COD_UNCLASSIFIED_MAJOR_DEVICE_MASK                          (0x001F00)
#define TC_BT_COD_SIGNIFICANT_MAJOR_DEVICE_MASK                           (0x001F00)

/* Minor Device Mask under computer major class */
#define TC_BT_COD_DESKTOP_WORKSTATION_MINOR_DEVICE_MASK                   (0x000004)
#define TC_BT_COD_SERVER_CLASS_COMPUTER_MINOR_DEVICE_MASK                 (0x000008)
#define TC_BT_COD_LAPTOP_MINOR_DEVICE_MASK                                (0x00000C)
#define TC_BT_COD_HANDHELP_PC_PDA_MINOR_DEVICE_MASK                       (0x000010)
#define TC_BT_COD_PALM_SIZED_PC_PDA_MINOR_DEVICE_MASK                     (0x000014)
#define TC_BT_COD_UNCLASSIFIED_COMPUTER_MINOR_DEVICE_MASK                 (0x000000)

/* Minor Device Mask under phone major class */
#define TC_BT_COD_CELLULAR_MINOR_DEVICE_MASK                              (0x000004)
#define TC_BT_COD_CORDLESS_MINOR_DEVICE_MASK                              (0x000008)
#define TC_BT_COD_SMART_PHONE_MINOR_DEVICE_MASK                           (0x00000C)
#define TC_BT_COD_WIRED_MODEM_OR_VOICE_GATEWAY_MINOR_DEVICE_MASK          (0x000010)
#define TC_BT_COD_UNCLASSIFIED_PHONE_MINOR_DEVICE_MASK                    (0x000000)

/* Minor Device Mask under audio/video major class */
#define TC_BT_COD_UNCATEGORISED_AV_MINOR_DEVICE_MASK                      (0x000000)
#define TC_BT_COD_WEARABLE_HEADSET_AV_MINOR_DEVICE_CLASS                  (0x000004)
#define TC_BT_COD_HANDSFREE_AV_MINOR_DEVICE_CLASS                         (0x000008)
#define TC_BT_COD_MICROPHONE_AV_MINOR_DEVICE_CLASS                        (0x000010)
#define TC_BT_COD_LOUDSPEAKER_AV_MINOR_DEVICE_CLASS                       (0x000014)
#define TC_BT_COD_HEADPHONES_AV_MINOR_DEVICE_CLASS                        (0x000018)
#define TC_BT_COD_PORTABLE_AUDIO_AV_MINOR_DEVICE_CLASS                    (0x00001C)
#define TC_BT_COD_CAR_AUDIO_AV_MINOR_DEVICE_CLASS                         (0x000020)
#define TC_BT_COD_SETTOP_BOX_AV_MINOR_DEVICE_CLASS                        (0x000024)
#define TC_BT_COD_HIFI_AUDIO_AV_MINOR_DEVICE_CLASS                        (0x000028)
#define TC_BT_COD_VCR_AV_MINOR_DEVICE_CLASS                               (0x00002C)
#define TC_BT_COD_VIDEO_CAMERA_AV_MINOR_DEVICE_CLASS                      (0x000030)
#define TC_BT_COD_CAMCORDER_AV_MINOR_DEVICE_CLASS                         (0x000034)
#define TC_BT_COD_VIDEO_MONITOR_AV_MINOR_DEVICE_CLASS                     (0x000038)
#define TC_BT_COD_VIDEO_DISPLAY_AND_LOUDSPEAKER_AV_MINOR_DEVICE_CLASS     (0x00003C)
#define TC_BT_COD_VIDEO_CONFERENCING_AV_MINOR_DEVICE_CLASS                (0x000040)
#define TC_BT_COD_GAME_TOY_AV_MINOR_DEVICE_CLASS                          (0x000048)

#define TC_BT_COD_HANDS_FREE_MINOR_DEVICE_MASK                            (0x000008)
/* Minor Device Mask under LAN major class */
#define TC_BT_COD_FULLY_AVAILABLE_MINOR_DEVICE_MASK                       (0x000000)
#define TC_BT_COD_ONE_TO_SEVENTEEN_PERCENT_MINOR_DEVICE_MASK              (0x000020)
#define TC_BT_COD_SEVENTEEN_TO_THIRTYTHREE_PERCENT_MINOR_DEVICE_MASK      (0x000040)
#define TC_BT_COD_THIRTYTHREE_TO_FIFTY_PERCENT_MINOR_DEVICE_MASK          (0x000060)
#define TC_BT_COD_FIFTY_TO_SIXTYSEVEN_PERCENT_MINOR_DEVICE_MASK           (0x000080)
#define TC_BT_COD_SIXTYSEVEN_TO_EIGHTYTHREE_PERCENT_MINOR_DEVICE_MASK     (0x0000A0)
#define TC_BT_COD_EIGHTYTHREE_TO_NINETYNINE_PERCENT_MINOR_DEVICE_MASK     (0x0000C0)
#define TC_BT_COD_NO_SERVICE_AVAILABLE_MINOR_DEVICE_MASK                  (0x0000E0)
#define TC_BT_COD_UNCLASSIFIED_LAN_MINOR_DEVICE_MASK                      (0x000000)

/* Minor Device Mask under Peripheral major Class (Keyboard/pointing device filed) */
#define TC_BT_COD_JOYSTICK_PERIPHERALS_MINOR_DEVICE_CLASS                 (0x000004)
#define TC_BT_COD_GAMEPAD_DEVICE_PERIPHERAL_MINOR_DEVICE_CLASS            (0x000008)
#define TC_BT_COD_REMOTE_CONTROL_DEVICE_PERIPHERAL_MINOR_DEVICE_CLASS     (0x00000C)
#define TC_BT_COD_SENSING_DEVICE_PERIPHERAL_MINOR_DEVICE_CLASS            (0x000010)
#define TC_BT_COD_DIGITIZER_TABLET_PERIPHERAL_MINOR_DEVICE_CLASS          (0x000014)
#define TC_BT_COD_CARD_READER_PERIPHERAL_MINOR_DEVICE_CLASS               (0x000018)

/* Minor Device Mask under imaging major class */
#define TC_BT_COD_PRINTER_MINOR_DEVICE_MASK                               (0x000080)

/* Minor Device Mask under Health major class */
#define TC_BT_COD_UNDEFINED_HEALTH_MINOR_DEVICE_CLASS                     (0x000000) 
#define TC_BT_COD_BPM_HEALTH_MINOR_DEVICE_CLASS                           (0x000004)
#define TC_BT_COD_THERMOMETER_HEALTH_MINOR_DEVICE_CLASS                   (0x000008)
#define TC_BT_COD_WEIGHING_SCALE_HEALTH_MINOR_DEVICE_CLASS                (0x00000C)
#define TC_BT_COD_GLUCOSE_METER_HEALTH_MINOR_DEVICE_CLASS                 (0x000010)
#define TC_BT_COD_PULSE_OXIM_HEALTH_MINOR_DEVICE_CLASS                    (0x000014)
#define TC_BT_COD_HRM_HEALTH_MINOR_DEVICE_CLASS                           (0x000018)
#define TC_BT_COD_DATA_DISPLAY_HEALTH_MINOR_DEVICE_CLASS                  (0x00001C)


/*----------------------------------------------------------------------------*
 *
 *   HCI Link Policy Settings
 *
 *---------------------------------------------------------------------------*/
#define TC_BT_DISABLE_ALL_LM_MODES         					((TcBtUint16)0x0000)
#define TC_BT_ENABLE_MS_SWITCH             					((TcBtUint16)0x0001)
#define TC_BT_ENABLE_HOLD                  					((TcBtUint16)0x0002)
#define TC_BT_ENABLE_SNIFF                 					((TcBtUint16)0x0004)
#define TC_BT_ENABLE_PARK                  					((TcBtUint16)0x0008)


/*----------------------------------------------------------------------------*
 *
 *   HCI Link Policy Mode
 *
 *---------------------------------------------------------------------------*/
#define TC_BT_ACTIVE_MODE					               	(0x0000)
#define TC_BT_HOLD_MODE                 					(0x0001)
#define TC_BT_SNIFF_MODE                					(0x0002)
#define TC_BT_PARK_MODE                 					(0x0003)


/*----------------------------------------------------------------------------*
 *
 *   Role Switch Option
 *
 *---------------------------------------------------------------------------*/
/* Role swtich configurations : */
/* No role switch attempted */
#define TC_BT_ROLE_CONFIG_NO_ROLE_SWITCH 					0
/* Incoming connections will be accepted as Master, but subsequent Role switch would be allowed */
#define TC_BT_ROLE_CONFIG_HYBRID_ROLE_SWITCH 				1
/* Accepts link as master and disallows all subsequent role switch */
#define TC_BT_ROLE_CONFIG_FORCE_ROLE_SWITCH 				2


#define TC_BT_VSC_DATA_LEN_MAX			255

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	TcBtCmdType			type;
} TcBtCmCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtUint8 			*pLocalName;
} TcBtCmSetLocalNameCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtUint8			disableInquiryScan;
	TcBtUint8			disablePageScan;
} TcBtCmWriteScanEnableCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBool			enable;
} TcBtCmEnableDUTCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtUint16			pageTimeout;
} TcBtCmWritePageToCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtUint32			ServiceCod;
	TcBtUint32			MajorCod;
	TcBtUint32			MinorCod;
} TcBtCmWriteCodCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr 			bdAddr;
} TcBtCmReadRemoteNameCmd, TcBtCmSdcCancelSearchCmd, TcBtCmReadLinkPolicyCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr 			bdAddr;
	TcBtUuid32  		Service;
} TcBtCmSdcSearchCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr 			bdAddr;
	TcBtUuid128  		Service;
} TcBtCmSdcUuid128SearchCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr 			bdAddr;
	TcBtUint16			theLinkPolicySetting;
} TcBtCmWriteLinkPolicyCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr 			bdAddr;
	TcBtUint16			timeout;
} TcBtCmWriteLinkSupervTimeoutCmd;

typedef struct
{
    TcBtCmdType			type;
    TcBtUint32			config;
} TcBtCmRoleSwitchConfigCmd;

typedef struct
{
	TcBtCmdType			type;
	TcBtBdAddr			deviceAddr;
} TcBtCmGetLinkQualityReq, TcBtCmReadRssiReq;

typedef struct
{
    TcBtCmdType			type;
    TcBtBdAddr			deviceAddr;
} TcBtCmAclDetachReq;

typedef struct
{
    TcBtCmdType			type;
    TcBtBdAddr			deviceAddr;			/* BdAddr to setup QoS */
    TcBtUint8			flag;				/* reserved for future use */
    TcBtUint8			service_type;		/* 0:no traffic, 1:Best Effort, 2:Guaranteed */
    TcBtInt32			token_rate;			/* Token rate in octets per second */
    TcBtInt32			peak_bandwidth;		/* Peak bandwidth in octets per second */
    TcBtInt32			latency;			/* Latency in microseconds */
    TcBtInt32			delay_variation;	/* Delay Variation in microseconds */
} TcBtCmQOSSetupReq;

typedef struct
{
    TcBtCmdType			type;
	TcBtUint8			FirstDisabledCh;
	TcBtUint8			LastDisabledCh;
} TcBtCmSetAfhChReq;

typedef struct
{
    TcBtCmdType			type;
    TcBtUint16			opcode;
    TcBtUint16			length;
    TcBtUint8			data[TC_BT_VSC_DATA_LEN_MAX];
} TcBtCmVSCReq;

typedef struct
{
    TcBtCmdType			type;
    TcBtBdAddr			deviceAddr;
	TcBtUint8			HoppingMode;
	TcBtUint8			Freq;
	TcBtUint8			ModultionType;
	TcBtUint8			LogicalCh;
	TcBtUint8			BBPacketType;
	TcBtUint16			BBPacketLen;
	TcBtUint8			TXPowerLevel;	
	TcBtInt8			TransmitPwoerdBM;
	TcBtUint8			TransmitPowerTableIdx;
} TcBtCmBluetestTxReq;

typedef struct
{
    TcBtCmdType			type;
	TcBtBdAddr 			deviceAddr;
	TcBtUint16 			ReporPeriod;
	TcBtUint8 			Freq;
	TcBtUint8			ModultionType;
	TcBtUint8			LogicalCh;
	TcBtUint8			BBPacketType;
	TcBtUint16			BBPacketLen;
} TcBtCmBluetestRxReq;

/*****************************************************************************
*
* externs
*
*****************************************************************************/

/* TODO: Add your specialized code here */
extern int TC_BT_CM_SetLocalName(unsigned char *pstrLocalName);
extern int TC_BT_CM_ReadLocalBdAddr(void);
extern int TC_BT_CM_WriteScanEnable(unsigned char disableInquiryScan, unsigned char disablePageScan);
extern int TC_BT_CM_ReadScanEnable(void);
extern int TC_BT_CM_EnableDUTMode(TcBtBool enable);
extern int TC_BT_CM_WritePageTo(unsigned short thePageTimeout);
extern int TC_BT_CM_WriteCod(TcBtUint32 ServiceCod, TcBtUint32 MajorCod, TcBtUint32 MinorCod);
extern int TC_BT_CM_ReadCod(void);
extern int TC_BT_CM_ReadRemoteName(TcBtBdAddr bdAddr);
extern int TC_BT_CM_CancelReadRemoteName(TcBtBdAddr bdAddr);
extern int TC_BT_CM_GetHFStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetA2DPStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetAVRCPStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetPACStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetSPPIAPStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetSPPStatus(TcBtUint8 idx, const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetMAPCStatus(TcBtUint8 idx, const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetHIDDStatus(const TcBtBdAddr *pBdAddr);
extern int TC_BT_CM_GetPANStatus(const TcBtBdAddr * pBdAddr);
extern int TC_BT_CM_SdcSearchReqSend(TcBtBdAddr bdAddr, const TcBtUuid32 Service);
extern int TC_BT_CM_SdcCancelSearchReqSend(TcBtBdAddr bdAddr);
extern int TC_BT_CM_SdcUuid128SearchReqSend(TcBtBdAddr bdAddr, const TcBtUuid128 Service);
extern int TC_BT_CM_SdcCancelUuid128SearchReqSend(TcBtBdAddr bdAddr);
extern int TC_BT_CM_WriteLinkPolicyReqSend(TcBtBdAddr bdAddr, TcBtUint16 theLinkPolicySetting);
extern int TC_BT_CM_ReadLinkPolicyReqSend(TcBtBdAddr bdAddr);
extern int TC_BT_CM_WriteLinkSuperVisionTimeoutReq(TcBtBdAddr bdAddr, TcBtUint16 timeout);
extern int TC_BT_CM_RoleSwitchConfigReq(TcBtUint32 config);
extern int TC_BT_CM_GetLinkQualityReq(TcBtBdAddr bdAddr);
extern int TC_BT_CM_ReadRssiReq(TcBtBdAddr bdAddr);
extern int TC_BT_CM_AclDetachReq(TcBtBdAddr bdAddr);
extern int TC_BT_CM_SetQOS(TcBtBdAddr deviceAddr, TcBtUint8 flag, TcBtUint8	service_type, TcBtInt32	token_rate, 
						TcBtInt32 peak_bandwidth, TcBtInt32	latency, TcBtInt32 delay_variation);
extern int TC_BT_CM_SetAfhChannels(TcBtUint8 FirstDisabledCh, TcBtUint8 LastDisabledCh);
extern int TC_BT_CM_Reset(void);
extern int TC_BT_CM_VSC(TcBtUint16 opcode, TcBtUint16 length, TcBtUint8 *pdata);
extern int TC_BT_CM_Bluetest_Tx(TcBtUint8 HoppingMode, TcBtUint8 Freq, TcBtUint8 ModultionType, TcBtUint8 LogicalCh, 
						TcBtUint8 BBPacketType, TcBtUint16 BBPacketLen, TcBtUint8 TXPowerLevel, TcBtInt8 TransmitPwoerdBM, 
						TcBtUint8 TransmitPowerTableIdx);
extern int TC_BT_CM_Bluetest_Rx(TcBtBdAddr bdAddr, TcBtUint16 ReporPeriod, TcBtUint8 Freq, TcBtUint8 ModultionType, TcBtUint8 LogicalCh, 
						TcBtUint8 BBPacketType, TcBtUint16 BBPacketLen);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_CM_CMD_H__ */
