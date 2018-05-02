/****************************************************************************
 *   FileName    : TC_BT_HF_EVT.h
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
*  Description : This file contains functions for a Bluetooth HF event
*				 "Confirm(Cfm) & Indication(Ind)"
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/07     0.1            created                     Ji, Seung-Nam
*    10/08/31     0.2            chg for HF DEV / resultcode Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_HF_EVT_H__
#define __TC_BT_HF_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "TC_BT_HF_CMD.h"
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
	TC_BT_HF_ACTIVATE_CFM = TC_BT_HF_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x300 */
	TC_BT_HF_DEACTIVATE_CFM,
	TC_BT_HF_CONFIG_LOW_POWER_CFM,					// reserved	
	TC_BT_HF_CONFIG_AUDIO_IND,						// reserved
	TC_BT_HF_CONFIG_AUDIO_CFM,						// reserved
	TC_BT_HF_STATUS_LOW_POWER_IND,					// reserved
	TC_BT_HF_SERVICE_CONNECT_IND,					// +BRSF
	TC_BT_HF_SERVICE_CONNECT_CFM,					// +BRSF
	TC_BT_HF_DISCONNECT_IND,						// disconnect
	TC_BT_HF_DISCONNECT_CFM,						// disconnect
	TC_BT_HF_AUDIO_CONNECT_IND,						// eSCO_link_req
	TC_BT_HF_AUDIO_CONNECT_CFM,						// eSCO_link_req
	TC_BT_HF_AUDIO_ACCEPT_CONNECT_IND,				// eSCO_link_req
	TC_BT_HF_AUDIO_DISCONNECT_IND,					// eSCO_link_req
	TC_BT_HF_AUDIO_DISCONNECT_CFM,					// eSCO_link_req
	TC_BT_HF_STATUS_INDICATOR_UPDATE_IND,			// +CIEV

	TC_BT_HF_GET_ALL_STATUS_INDICATORS_CFM,			// +CIND
	TC_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM,	// +COPS
	TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND,	// +CNUM
	TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM,	// +CNUM
	TC_BT_HF_GET_CURRENT_CALL_LIST_IND,				// +CLCC
	TC_BT_HF_GET_CURRENT_CALL_LIST_CFM,				// +CLCC
	TC_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM,	// +CMEE
	TC_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM,	// +CLIP
	TC_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM,		// +CCWA
	TC_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM,		// +CIEV
	TC_BT_HF_SET_ECHO_AND_NOISE_CFM,				// +NREC
	TC_BT_HF_SET_VOICE_RECOGNITION_CFM,				// +BVRA
	TC_BT_HF_SET_VOICE_RECOGNITION_IND,				// +BVRA
	TC_BT_HF_BT_INPUT_CFM,							// +BINP
	TC_BT_HF_INBAND_RING_SETTING_CHANGED_IND,		// +BSIR
	TC_BT_HF_GENERATE_DTMF_CFM,						// +VTS
	TC_BT_HF_SPEAKER_GAIN_STATUS_CFM,				// +VGS
	TC_BT_HF_SPEAKER_GAIN_IND,						// +VGS
	TC_BT_HF_MIC_GAIN_STATUS_CFM,					// +VGM
	TC_BT_HF_MIC_GAIN_IND,							// +VGM

	TC_BT_HF_DIAL_CFM,								// +ATD, +BLDN
	TC_BT_HF_CALL_ANSWER_CFM,						// +ATA
	TC_BT_HF_CALL_END_CFM,							// +CHUP
	TC_BT_HF_CALL_HANDLING_IND,						// +CHLD, +BTRH
	TC_BT_HF_CALL_HANDLING_CFM,						// +CHLD, +BTRH
	TC_BT_HF_AT_CMD_IND,							// other at commands
	TC_BT_HF_AT_CMD_CFM,							// other at commands
	TC_BT_HF_CALL_RINGING_IND,						// RING
	TC_BT_HF_CALL_NOTIFICATION_IND,					// +CLIP
	TC_BT_HF_CALL_WAITING_NOTIFICATION_IND,			// +CCWA

	TC_BT_HF_SECURITY_IN_CFM,						// reserved	
	TC_BT_HF_SECURITY_OUT_CFM,						// reserved	
	TC_BT_HF_C2C_SF_IND,							// reserved	
	TC_BT_HF_SET_C2C_AUDIO_CODEC_IND,				// reserved	
	TC_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND,		// reserved	
	TC_BT_HF_SET_C2C_SAMPLE_RATE_IND,				// reserved	
	TC_BT_HF_C2C_PWR_CFM,							// reserved	
	TC_BT_HF_C2C_BATT_CFM,							// reserved		
	TC_BT_HF_C2C_BATT_IND,							// reserved	
	TC_BT_HF_C2C_GET_SMS_CFM,						// reserved	
	TC_BT_HF_C2C_SMS_IND,							// reserved	
	TC_BT_HF_C2C_TXT_IND,							// reserved	
	TC_BT_HF_DEREGISTER_TIME_CFM,					// reserved	
	TC_BT_HF_INDICATOR_ACTIVATION_CFM,				// +BIA

	TC_BT_HF_UPDATE_SUPPORTED_CODEC_CFM,			// +BAC
	TC_BT_HF_SELECTED_CODEC_IND,					// +BCS
#if 1 /* Telechips' remark */
	TC_BT_HF_SERVICE_CONNECT_NOTIFICATION_IND,		// +CHLD
#endif
	
	TC_BT_HF_EVT_TYPE_MAX
} TcBtHfEvtEnum;

/* Call handling (+BTRH) events from AG */
typedef TcBtUint8  TcBtCallHandlingEvent;

/* CME ERRORS */
typedef TcBtUint16 TcBtCmeeResultCode;

/* Support HFG features (+BRSF) */
typedef TcBtUint32 TcBtHfgSupportedFeatures;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
/* Call handling (+BTRH) events from AG */
#define TC_BT_BTRH_INCOMING_ON_HOLD                             ((TcBtCallHandlingEvent) 0) /* +BTRH:0 */
#define TC_BT_BTRH_INCOMING_ACCEPTED                            ((TcBtCallHandlingEvent) 1) /* +BTRH:1 */
#define TC_BT_BTRH_INCOMING_REJECTED                            ((TcBtCallHandlingEvent) 2) /* +BTRH:2 */

/* CME ERRORS */
#define TC_BT_CME_AG_FAILURE                                    ((TcBtCmeeResultCode) 0)
#define TC_BT_CME_NO_CONNECTION_TO_PHONE                        ((TcBtCmeeResultCode) 1)
#define TC_BT_CME_OPERATION_NOT_ALLOWED                         ((TcBtCmeeResultCode) 3)
#define TC_BT_CME_OPERATION_NOT_SUPPORTED                       ((TcBtCmeeResultCode) 4)
#define TC_BT_CME_PH_SIM_PIN_REQUIRED                           ((TcBtCmeeResultCode) 5)
#define TC_BT_CME_PH_FSIM_PIN                                   ((TcBtCmeeResultCode) 6)   /* not defined in spec */
#define TC_BT_CME_PH_FSIM_PUK                                   ((TcBtCmeeResultCode) 7)   /* not defined in spec */
#define TC_BT_CME_SIM_NOT_INSERTED                              ((TcBtCmeeResultCode) 10)
#define TC_BT_CME_SIM_PIN_REQUIRED                              ((TcBtCmeeResultCode) 11)
#define TC_BT_CME_SIM_PUK_REQUIRED                              ((TcBtCmeeResultCode) 12)
#define TC_BT_CME_SIM_FAILURE                                   ((TcBtCmeeResultCode) 13)
#define TC_BT_CME_SIM_BUSY                                      ((TcBtCmeeResultCode) 14)
#define TC_BT_CME_SIM_WRONG                                     ((TcBtCmeeResultCode) 15)  /* not defined in spec */
#define TC_BT_CME_INCORRECT_PASSWORD                            ((TcBtCmeeResultCode) 16)
#define TC_BT_CME_SIM_PIN2_REQUIRED                             ((TcBtCmeeResultCode) 17)
#define TC_BT_CME_PUK2_REQUIRED                                 ((TcBtCmeeResultCode) 18)
#define TC_BT_CME_MEMORY_FULL                                   ((TcBtCmeeResultCode) 20)
#define TC_BT_CME_INVALID_INDEX                                 ((TcBtCmeeResultCode) 21)
#define TC_BT_CME_NOT_FOUND                                     ((TcBtCmeeResultCode) 22)  /* not defined in spec */
#define TC_BT_CME_MEMORY_FAILURE                                ((TcBtCmeeResultCode) 23)
#define TC_BT_CME_TEXT_STRING_TOO_LONG                          ((TcBtCmeeResultCode) 24)
#define TC_BT_CME_INVALID_CHARACTERS_IN_TEXT_STRING             ((TcBtCmeeResultCode) 25)
#define TC_BT_CME_DIAL_STRING_TOO_LONG                          ((TcBtCmeeResultCode) 26)
#define TC_BT_CME_INVALID_CHARACTERS_IN_DIAL_STRING             ((TcBtCmeeResultCode) 27)
#define TC_BT_CME_NO_NETWORK_SERVICE                            ((TcBtCmeeResultCode) 30)
#define TC_BT_CME_NETWORK_TIMEOUT                               ((TcBtCmeeResultCode) 31)  /* not defined in spec */
#define TC_BT_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY      ((TcBtCmeeResultCode) 32)
#define TC_BT_CME_NETWORK_PERSONALIZATION_PIN_REQUIRED          ((TcBtCmeeResultCode) 40)  /* not defined in spec */
#define TC_BT_CME_NETWORK_PERSONALIZATION_PUK_REQUIRED          ((TcBtCmeeResultCode) 41)  /* not defined in spec */
#define TC_BT_CME_NETWORK_SUBSET_PERSONALIZATION_PIN_REQUIRED   ((TcBtCmeeResultCode) 42)  /* not defined in spec */
#define TC_BT_CME_NETWORK_SUBSET_PERSONALIZATION_PUK_REQUIRED   ((TcBtCmeeResultCode) 43)  /* not defined in spec */
#define TC_BT_CME_SERVICE_PROVIDER_PERSONALIZATION_PIN_REQUIRED ((TcBtCmeeResultCode) 44)  /* not defined in spec */
#define TC_BT_CME_SERVICE_PROVIDER_PERSONALIZATION_PUK_REQUIRED ((TcBtCmeeResultCode) 45)  /* not defined in spec */
#define TC_BT_CME_CORPORATE_PERSONALIZATION_PIN_REQUIRED        ((TcBtCmeeResultCode) 46)  /* not defined in spec */
#define TC_BT_CME_CORPORATE_PERSONALIZATION_PUK_REQUIRED        ((TcBtCmeeResultCode) 47)  /* not defined in spec */
#define TC_BT_CME_HIDDEN_KEY_REQUIRED                           ((TcBtCmeeResultCode) 48)  /* not defined in spec */
#define TC_BT_CME_EAP_METHOD_NOT_SUPPORTED                      ((TcBtCmeeResultCode) 49)  /* not defined in spec */
#define TC_BT_CME_INCORRECT_PARAMETERS                          ((TcBtCmeeResultCode) 50)  /* not defined in spec */
#define TC_BT_CME_UNKNOWN                                       ((TcBtCmeeResultCode) 100) /* not defined in spec */
#define TC_BT_CME_BUSY                                          ((TcBtCmeeResultCode) 101) /* not defined in GSM spec */
#define TC_BT_CME_NO_CARRIER                                    ((TcBtCmeeResultCode) 102) /* not defined in GSM spec */
#define TC_BT_CME_NO_ANSWER                                     ((TcBtCmeeResultCode) 103) /* not defined in GSM spec */
#define TC_BT_CME_DELAYED                                       ((TcBtCmeeResultCode) 104) /* not defined in GSM spec */
#define TC_BT_CME_BLACKLISTED                                   ((TcBtCmeeResultCode) 105) /* not defined in GSM spec */
#define TC_BT_CME_RESPONSE_TIMEOUT                              ((TcBtCmeeResultCode) 0xfffe) /* not defined in GSM spec */
#define TC_BT_CME_SUCCESS                                       ((TcBtCmeeResultCode) 0xffff) /* not defined in spec */

/* Support HFG features (+BRSF) */
#define TC_BT_HFG_SUPPORT_THREE_WAY_CALLING                     ((TcBtHfgSupportedFeatures) 0x00000001)
#define TC_BT_HFG_SUPPORT_EC_NR_FUNCTION                        ((TcBtHfgSupportedFeatures) 0x00000002)
#define TC_BT_HFG_SUPPORT_VOICE_RECOGNITION                     ((TcBtHfgSupportedFeatures) 0x00000004)
#define TC_BT_HFG_SUPPORT_INBAND_RINGING                        ((TcBtHfgSupportedFeatures) 0x00000008)
#define TC_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG            ((TcBtHfgSupportedFeatures) 0x00000010)
#define TC_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL                ((TcBtHfgSupportedFeatures) 0x00000020)
#define TC_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS                  ((TcBtHfgSupportedFeatures) 0x00000040)
#define TC_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL                 ((TcBtHfgSupportedFeatures) 0x00000080)
#define TC_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES                  ((TcBtHfgSupportedFeatures) 0x00000100)
#define TC_BT_HFG_SUPPORT_CODEC_NEGOTIATION                     ((TcBtHfgSupportedFeatures) 0x00000200)
#define TC_BT_HFP_SDP_SUPPORT_MASK                              ((TcBtHfgSupportedFeatures) 0x0000001F)
//#define TC_BT_HFP_SDP_CODEC_NEGOTIATION                         (0x20)


#define TC_BT_RESULT_CODE_HF_SUCCESS                                   ((TcBtResultCode) (0x0000))
#define TC_BT_RESULT_CODE_HF_UNACCEPTABLE_PARAMETER                    ((TcBtResultCode) (0x0001))
#define TC_BT_RESULT_CODE_HF_INVALID_PARAMETER                         ((TcBtResultCode) (0x0002))
#define TC_BT_RESULT_CODE_HF_C2C_REJECTED                              ((TcBtResultCode) (0x0003))
#define TC_BT_RESULT_CODE_HF_SYNCHRONOUS_CONNECTION_ALREADY_CONNECTING ((TcBtResultCode) (0x0004))
#define TC_BT_RESULT_CODE_HF_SYNCHRONOUS_CONNECTION_LIMIT_EXCEEDED     ((TcBtResultCode) (0x0005))
#define TC_BT_RESULT_CODE_HF_REJECTED_BY_PROFILE                       ((TcBtResultCode) (0x0006))
#define TC_BT_RESULT_CODE_HF_MAX_NUM_OF_CONNECTIONS                    ((TcBtResultCode) (0x0007))
#define TC_BT_RESULT_CODE_HF_SDC_SEARCH_FAILED                         ((TcBtResultCode) (0x0008))
#define TC_BT_RESULT_CODE_HF_CONNECT_ATTEMPT_FAILED                    ((TcBtResultCode) (0x0009))
#define TC_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT                 ((TcBtResultCode) (0x000A))
#define TC_BT_RESULT_CODE_HF_CONNECTION_TERM_BY_REMOTE_HOST            ((TcBtResultCode) (0x000B))
#define TC_BT_RESULT_CODE_HF_REJ_RESOURCES                             ((TcBtResultCode) (0x000C))
#if 1 /* Telechips' remark */
#define TC_BT_RESULT_CODE_HF_RECOMM_CONNECTION_FAILED                  ((TcBtResultCode) (0x000D))
#endif

/* eSCO defines */
#define TC_BT_FIRST_HFP_NO_ESCO			(0x0096) 
#define TC_BT_LAST_HFP_NO_ESCO			(0x0101) 
#define TC_BT_FIRST_HFP_ESCO			(0x0105) 
#define TC_BT_FIRST_HFP_CODEC_NEG_ESCO	(0x0106)

/* defines for WBS */
#define TC_BT_WBS_INVALID_CODEC                                 ((TcBtUint8) 0x00)
#define TC_BT_WBS_CVSD_CODEC                                    ((TcBtUint8) 0x01)
#define TC_BT_WBS_MSBC_CODEC                                    ((TcBtUint8) 0x02)

#define TC_BT_WBS_CVSD_CODEC_MASK                               ((TcBtUint32) 0x00000001)
#define TC_BT_WBS_MSBC_CODEC_MASK                               ((TcBtUint32) 0x00000002)

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
} TcBtHfEvt;

/* General profile handling, link handling and configuration primitives */
/* Activate confirm */
typedef struct
{
    unsigned short int                 type;
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfActivateCfm;

/* Deactivate confirm */
typedef struct
{
    unsigned short int                 type;
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfDeactivateCfm;

/* Configure Low power confirm */
typedef struct
{
    unsigned short int                 type;
} TcBtHfConfigLowPowerCfm;

/* Link mode changed */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   currentMode;                    /* current low-power mode */
    TcBtUint8                   oldMode;                        /* old LP mode */
    TcBtUint8                   wantedMode;                     /* wanted LP mode */
    TcBtBool                    remoteReq;                      /* mode change was request by remote peer */
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfStatusLowPowerInd;

typedef struct
{
	unsigned short int                type;                           /* Primitive/message identity */
    TcBtBdAddr                 bdAddr;
	TcBtUint8            	   audioType;                      /* data type in pointer */
	TcBtUint16                 audioSettingLen;                /* size of audioSetting pointer times the number of entries of the pointer instance when allocated as an array */
	TcBtUint8                  *audioSetting;                  /* pointer to "dataType" audio parameters */
	TcBtResultCode             resultCode;
	TcBtSupplier               resultSupplier;
} TcBtHfConfigAudioInd;

/* Configure Audio confirm */
typedef struct
{
    unsigned short int                 type;
} TcBtHfConfigAudioCfm;

/* Connection and audio handling primitives */
/* Incoming SLC has been established */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;                         /* Device address for the connection established */
    TcBtHfgSupportedFeatures    supportedFeatures;              /* Features registered at peer side */
    TcBtUint8                   network;                        /* Network registered at peer side */
    TcBtCharString              *serviceName;                   /* name of service at peer side */
    TcBtHfConnectionTypeEnum    connectionType;                 /* Used to indicate if connection is from AG /HFG */
    TcBtCharString              *indicatorSupported;            /* string with indicators supported by the remote device - Only valid for HFP connections */
    TcBtCharString              *indicatorValue;                /* string with value of the indicators supported by the remote - Only for HFP connections */
    TcBtCharString              *chldString;                    /* string that describes the call hold and multiparty services - Only for HFP connections */
    TcBtUint16                  remoteVersion;                  /* remote device's supported version of the spec */
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
    TcBtConnId                  btConnId;                       /* Global Bluetooth connection ID */
} TcBtHfServiceConnectInd;

/* Outgoing SLC has been established */
typedef TcBtHfServiceConnectInd TcBtHfServiceConnectCfm;

/* Disconnect indication */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtResultCode              reasonCode;
    TcBtSupplier                reasonSupplier;
} TcBtHfDisconnectInd;

/* Disconnect confirm */
typedef TcBtHfDisconnectInd TcBtHfDisconnectCfm;

/* Audio indication */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint16                  scoHandle;                      /* sco handle if routed internally */
    TcBtUint8                   pcmSlot;                        /* chosen PCM slot */

    TcBtUint8                   linkType;                       /* Link type used in the established audio connection (0x00 = SCO, 0x02 = eSCO)*/
    TcBtUint8                   txInterval;                     /* Transmission interval in baseband slots (Set to zero for SCO links) */
    TcBtUint8                   weSco;                          /* Retransmission window in baseband slots (Set to zero for SCO links) */
    TcBtUint16                  rxPacketLength;                 /* RX packet length in bytes for eSCO connections (Set to zero for SCO links) */
    TcBtUint16                  txPacketLength;                 /* TX packet length in bytes for eSCO connections (Set to zero for SCO links) */
    TcBtUint8                   airMode;                        /* Air mode (0x00 = my-law, 0x01 = A-law, 0x02 = CVSD, 0x03 = Transparent data) */

    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfAudioConnectInd;

/* Audio confirm */
typedef TcBtHfAudioConnectInd TcBtHfAudioConnectCfm;

/* Incoming SCO/eSCO connect indication */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   linkType;                       /* specifies SCO/eSCO */
} TcBtHfAudioAcceptConnectInd; 

/* Audio disconnect confirm */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint16                  scoHandle;                      /* sco handle if routed internally */
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfAudioDisconnectCfm;

/* Audio disconnect indication */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint16                  scoHandle;                      /* sco handle if routed internally */
    TcBtReasonCode              reasonCode;
    TcBtSupplier                reasonSupplier;
} TcBtHfAudioDisconnectInd;

/* AT Command handling primitives */
/* Status indicator update (+CIEV) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   index;                          /* The index value into the CIND string */
    TcBtUint8                   value;                          /* Value of the status indicator */
    TcBtCharString              *name;                          /* Name of the status indicator (derived on behalf of the index into the CIND string) */
} TcBtHfStatusIndicatorUpdateInd;

/* Query value of all status indicators confirm (+CIND) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *indicatorSupported;            /* string with indicators supported by the remote device (Note this is the same string as that given in the connection establishment) */
    TcBtCharString              *indicatorValue;                /* string updated with the latest value of the indicators supported by the remote */
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfGetAllStatusIndicatorsCfm;

/* Query current operator selection confirm (+COPS) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *copsString;                    /* The string received in the cops response */
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfGetCurrentOperatorSelectionCfm;

/* Subscriber number information (+CNUM:) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *cnumString;                    /* The string received in a cnum response */
} TcBtHfGetSubscriberNumberInformationInd;

/* Confirm to (AT+CNUM) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfGetSubscriberNumberInformationCfm;

/* Call information (+CLCC:) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *clccString;                    /* The string received in a clcc response */
    TcBtUint8                   idx;
    TcBtUint8                   dir;
    TcBtUint8                   status;
    TcBtUint8                   mode;
    TcBtUint8                   mpty;
    TcBtUint32                  dataLength;
    TcBtUint8                   *data;
    TcBtUint32                  bodyLength;
    TcBtUint8                   *body;
} TcBtHfGetCurrentCallListInd;

/* Confirm to (AT+CLCC) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfGetCurrentCallListCfm;


/* Confirm to AT+CMEE */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetExtendedAgErrorResultCodeCfm;

/* Confirm to AT+CLIP */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetCallNotificationIndicationCfm;

/* Confirm to AT+CCWA */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetCallWaitingNotificationCfm;

/* Confirm to AT+CMER */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetStatusIndicatorUpdateCfm;

/* Confirm to AT+NREC */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetEchoAndNoiseCfm;

/* Confirm to AT+BVRA= */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSetVoiceRecognitionCfm;

/* Voice recognition input sequence initiated from AG (+BVRA:) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtBool                    started;                        /* voice recognition input sequence in the AG is started/stoped */
} TcBtHfSetVoiceRecognitionInd;

/* Confirm to AT+BINP */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *dataRespString;                /* The dataResponse string received from the AG */
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfBtInputCfm;

/* Inband ring setting changed in AG (+BSIR:) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtBool                    inbandRingingActivated;         /* New setting of the inband ring tone feature in the AG. If TRUE = AG will provide inband ring tone, if FALSE = AG will not provide inband ring tone */
} TcBtHfInbandRingSettingChangedInd;

/* Confirm to AT+VTS */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfGenerateDtmfCfm;

/* Confirm to AT+VGS */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfSpeakerGainStatusCfm;

/* Speaker gain indication (AT+VGS) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   gain;                           /* New speaker gain */
} TcBtHfSpeakerGainInd;

/* Confirm to AT+VGM */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfMicGainStatusCfm;

/* Mic gain indication (AT+VGM) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   gain;                           /* New michrophone gain */
} TcBtHfMicGainInd;

/* Confirm to ATD and AT+BLDN */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfDialCfm;

/* Incoming call ringing (RING) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   inBand;                           /* Inband Ring */
} TcBtHfCallRingingInd;

/* Incoming call identification (+CLIP) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *clipString;                    /* string received from AG */
} TcBtHfCallNotificationInd;

/* Incoming call waiting during ongoing call (+CCWA) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *ccwaString;                    /* string received from AG */
} TcBtHfCallWaitingNotificationInd;

/* Confirm to ATA */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfCallAnswerCfm;

/* Confirm to AT+CHUP */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfCallEndCfm;

/* The Response and Hold state of an incoming call changed (+BTRH:) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCallHandlingEvent       event;                          /* btrh event */
} TcBtHfCallHandlingInd;

/* Call hold/response/handling confirm (AT+CHLD and AT+BTRH) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
} TcBtHfCallHandlingCfm;

/* Unknown/unexpected upstream AT command indication (raw) */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *atCmdString;                   /* the full formatted and null-terminated AT command string received */
#if 1 /* Telechips' remark */
	TcBtUint32					atCmdIndex;
#endif
} TcBtHfAtCmdInd;

/* Confirm to AT command request */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          cmeeResultCode;                 /* cme error result code of the operation if success (the AG responded OK) the result is CME_SUCCESS any other result code means the operation failed if CMEE results has not been enabled and ERROR response from the AG will be mapped to CME_AG_FAILURE */
#if 1 /* Telechips' remark */
	TcBtUint32					atCmdIndex;
#endif
} TcBtHfAtCmdCfm;

/* Security control handling primitives */
/* Incoming security level confirm */
typedef struct
{
    unsigned short int                 type;
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfSecurityInCfm;

/* Outgoing security level confirm */
typedef struct
{
    unsigned short int                 type;
    TcBtResultCode              resultCode;
    TcBtSupplier                resultSupplier;
} TcBtHfSecurityOutCfm;

typedef struct
{
    unsigned short int                 type;
    TcBtCmeeResultCode          result;                         /* CME_SUCCESS or error */
} TcBtHfDeregisterTimeCfm;

typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCmeeResultCode          result;                         /* CME_SUCCESS or error */
}TcBtHfIndicatorActivationCfm;

typedef struct
{
    unsigned short int                 type;                           /* primitive/message identity */
    TcBtResultCode             resultCode;
    TcBtSupplier               resultSupplier;
}TcBtHfUpdateSupportedCodecCfm;

typedef struct
{
	unsigned short int 				type;							/* Primitive/message identity */
	TcBtBdAddr 					bdAddr;					/* Connection indentifier */
	TcBtUint8					codecToUse; 					/* Selected codec  */
}TcBtHfSelectedCodecInd;

#if 1 /* Telechips' remark */
typedef struct
{
    unsigned short int                 type;                           /* Primitive/message identity */
    TcBtBdAddr                  bdAddr;
    TcBtResultCode				resultCode;
    
    TcBtCharString				*chldString;                    /* string that describes the call hold and multiparty services - Only for HFP connections */
} TcBtHfServiceConnectNotificationInd;
#endif

/*****************************************************************************
*
* externs
*
*****************************************************************************/
//extern char *strstr( const char *str1, const char *str2);

extern int TC_BT_HF_ReceiveEvent(TcBtEvt *pTcBtEvt);

extern int TC_BT_HF_RegisterDevice (void *pTcBtEvt);
extern int TC_BT_HF_DeregisterDevice(void *pTcBtEvt);
extern int TC_BT_HF_SetDevice(void *pTcBtEvt);

extern int TC_BT_HF_ActivateCfm(void *pTcBtEvt);
extern int TC_BT_HF_DeactivateCfm(void *pTcBtEvt);
extern int TC_BT_HF_ConfigLowPowerCfm(void *pTcBtEvt);
extern int TC_BT_HF_ConfigAudioInd(void *pTcBtEvt);
extern int TC_BT_HF_ConfigAudioCfm(void *pTcBtEvt);
extern int TC_BT_HF_ServiceConnectInd(void *pTcBtEvt);
extern int TC_BT_HF_ServiceConnectCfm(void *pTcBtEvt);
extern int TC_BT_HF_DisconnectInd(void *pTcBtEvt);
extern int TC_BT_HF_DisconnectCfm(void *pTcBtEvt);
extern int TC_BT_HF_AudioConnectInd(void *pTcBtEvt);
extern int TC_BT_HF_AudioConnectCfm(void *pTcBtEvt);
extern int TC_BT_HF_AudioAcceptConnectInd(void *pTcBtEvt);
extern int TC_BT_HF_AudioDisconnectInd(void *pTcBtEvt);
extern int TC_BT_HF_AudioDisconnectCfm(void *pTcBtEvt);
extern int TC_BT_HF_StatusIndicatorUpdateInd(void *pTcBtEvt);

extern int TC_BT_HF_GetAllStatusIndicatorsCfm(void *pTcBtEvt);
extern int TC_BT_HF_GetCurrentOperatorSelectionCfm(void *pTcBtEvt);
extern int TC_BT_HF_GetSubscriberNumberInformationInd(void *pTcBtEvt);
extern int TC_BT_HF_GetSubscriberNumberInformationCfm(void *pTcBtEvt);
extern int TC_BT_HF_GetCurrentCallListInd(void *pTcBtEvt);
extern int TC_BT_HF_GetCurrentCallListCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetExtendedAgErrorResultCodeCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetCallNotificationIndicationCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetCallWaitingNotificationCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetStatusIndicatorUpdateCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetEchoAndNoiseCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetVoiceRecognitionCfm(void *pTcBtEvt);
extern int TC_BT_HF_SetVoiceRecognitionInd(void *pTcBtEvt);
extern int TC_BT_HF_BtInputCfm(void *pTcBtEvt);
extern int TC_BT_HF_InbandRingSettingChangedInd(void *pTcBtEvt);
extern int TC_BT_HF_GenerateDtmfCfm(void *pTcBtEvt);
extern int TC_BT_HF_SpeakerGainStatusCfm(void *pTcBtEvt);
extern int TC_BT_HF_SpeakerGainInd(void *pTcBtEvt);
extern int TC_BT_HF_MicGainStatusCfm(void *pTcBtEvt);
extern int TC_BT_HF_MicGainInd(void *pTcBtEvt);

extern int TC_BT_HF_DialCfm(void *pTcBtEvt);
extern int TC_BT_HF_CallAnswerCfm(void *pTcBtEvt);
extern int TC_BT_HF_CallEndCfm(void *pTcBtEvt);
extern int TC_BT_HF_CallHandlingInd(void *pTcBtEvt);
extern int TC_BT_HF_CallHandlingCfm(void *pTcBtEvt);
extern int TC_BT_HF_AtCmdInd(void *pTcBtEvt);
extern int TC_BT_HF_AtCmdCfm(void *pTcBtEvt);
extern int TC_BT_HF_CallRingingInd(void *pTcBtEvt);
extern int TC_BT_HF_CallNotificationInd(void *pTcBtEvt);
extern int TC_BT_HF_CallWaitingNotificationInd(void *pTcBtEvt);

extern int TC_BT_HF_SecurityInCfm(void *pTcBtEvt);
extern int TC_BT_HF_SecurityOutCfm(void *pTcBtEvt);
extern int TC_BT_HF_DeregisterTimeCfm(void *pTcBtEvt);
extern int TC_BT_HF_IndicatorActivationCfm(void *pTcBtEvt);
extern int TC_BT_HF_UpdateSupportedCodecCfm(void *pTcBtEvt);
extern int TC_BT_HF_SelectedCodecInd(void *pTcBtEvt);
#if 1 /* Telechips' remark */
extern int TC_BT_HF_ServiceConnectNotificationInd(void *pTcBtEvt);
#endif

/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_HF_EVT_H__ */
