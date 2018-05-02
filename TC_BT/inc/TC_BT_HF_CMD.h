/****************************************************************************
 *   FileName    : TC_BT_HF_CMD.h
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
*  Description : This file contains functions for a Bluetooth HF command
*                "Request(Req) -> Cmd"
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/05/31     0.1            created                     Ji, Seung-Nam
*
*******************************************************************************/

#ifndef __TC_BT_HF_CMD_H__
#define __TC_BT_HF_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtBase.h"
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
	TC_BT_HF_ACTIVATE_REQ = 0,
	TC_BT_HF_DEACTIVATE_REQ,
	TC_BT_HF_CONFIG_LOW_POWER_REQ,						// reserved
	TC_BT_HF_CONFIG_AUDIO_REQ,							// reserved
	TC_BT_HF_SERVICE_CONNECT_REQ,						// +BRSF
	TC_BT_HF_CANCEL_CONNECT_REQ,						// connect
	TC_BT_HF_DISCONNECT_REQ,							// connect
	TC_BT_HF_AUDIO_CONNECT_REQ,							// eSCO_link_req
	TC_BT_HF_AUDIO_ACCEPT_CONNECT_RES,					// eSCO_link_req
	TC_BT_HF_AUDIO_DISCONNECT_REQ,						// eSCO_link_req

	TC_BT_HF_GET_ALL_STATUS_INDICATORS_REQ,				// +CIND
	TC_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ,		// +COPS	
	TC_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ,		// +CNUM
	TC_BT_HF_GET_CURRENT_CALL_LIST_REQ,					// +CLCC
	TC_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ,		// +CMEE
	TC_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ,		// +CLIP
	TC_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ,			// +CCWA
	TC_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ,			// +CMER
	TC_BT_HF_SET_ECHO_AND_NOISE_REQ,					// +NREC
	TC_BT_HF_SET_VOICE_RECOGNITION_REQ,					// +BVRA
	TC_BT_HF_BT_INPUT_REQ,								// +BINP
	TC_BT_HF_GENERATE_DTMF_REQ,							// +VTS
	TC_BT_HF_SPEAKER_GAIN_STATUS_REQ,					// +VGS
	TC_BT_HF_MIC_GAIN_STATUS_REQ,						// +VGM

	TC_BT_HF_DIAL_REQ,									// +ATD, +BLDN
	TC_BT_HF_CALL_ANSWER_REQ,							// +ATA
	TC_BT_HF_CALL_END_REQ,								// +CHUP
	TC_BT_HF_CALL_HANDLING_REQ,							// +CHLD, +BTRH
	TC_BT_HF_AT_CMD_REQ,								// other at commands

	TC_BT_HF_SECURITY_IN_REQ,							// reserved
	TC_BT_HF_SECURITY_OUT_REQ,							// reserved
	TC_BT_HF_C2C_SF_REQ,								// reserved
	TC_BT_HF_SET_C2C_AUDIO_CODEC_REQ,					// reserved
	TC_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ,			// reserved
	TC_BT_HF_SET_C2C_SAMPLE_RATE_REQ,					// reserved
	TC_BT_HF_C2C_PWR_REQ,								// reserved
	TC_BT_HF_C2C_BATT_REQ,								// reserved
	TC_BT_HF_C2C_GET_SMS_REQ,							// reserved
	TC_BT_HF_DEREGISTER_TIME_REQ,						// reserved
	TC_BT_HF_INDICATOR_ACTIVATION_REQ,					// +BIA
	TC_BT_HF_UPDATE_SUPPORTED_CODEC_REQ,				// +BAC
	TC_BT_HF_UPDATE_HF_INDICATORS_REQ,					// +BIEV
	TC_BT_HF_CMD_TYPE_MAX
} TcBtHfCmdEnum;

typedef enum
{
	/* HF connection type */
	TC_BT_HF_CONNECTION_UNKNOWN = 0,             /* Profile will auto-detect Gateway type to connect (prioritized in the order HF first (if awailable) then HS) */
	TC_BT_HF_CONNECTION_HF,                  /* Gateway uses the HFP protocol */
	TC_BT_HF_CONNECTION_HS                   /* Gateway uses the old HSP protocol */
} TcBtHfConnectionTypeEnum;

typedef enum
{
	/* HF dial command opcode */
	TC_BT_HF_DIAL_NUMBER = 0,
	TC_BT_HF_DIAL_MEMORY,
	TC_BT_HF_DIAL_REDIAL
} TcBtHfDialEnum;

typedef enum
{
	/* Call handling (AT+CHLD) commands from headset */
	TC_BT_HF_RELEASE_ALL_HELD_CALL = 0,          /* AT+CHLD=0  */
	TC_BT_HF_RELEASE_ACTIVE_ACCEPT,          /* AT+CHLD=1  */
	TC_BT_HF_RELEASE_SPECIFIED_CALL,         /* AT+CHLD=1x */
	TC_BT_HF_HOLD_ACTIVE_ACCEPT,             /* AT+CHLD=2  */
	TC_BT_HF_REQUEST_PRIVATE_WITH_SPECIFIED, /* AT+CHLD=2x */
	TC_BT_HF_ADD_CALL,                       /* AT+CHLD=3  */
	TC_BT_HF_CONNECT_TWO_CALLS,              /* AT+CHLD=4  */

	/* Response/hold (AT+BTRH) commands from headset. These must be a
	  * continuation of the AT+CHLD commands as they are are combined in
	  * the TC_BT_HFG_CALL_HANDLING_IND and TC_BT_HF_CALL_HANDLING_REQ signals */
	TC_BT_HF_BTRH_PUT_ON_HOLD,               /* AT+BTRH=0 */
	TC_BT_HF_BTRH_ACCEPT_INCOMING,           /* AT+BTRH=1 */
	TC_BT_HF_BTRH_REJECT_INCOMING,           /* AT+BTRH=2 */
	TC_BT_HF_BTRH_READ_STATUS                /* AT+BTRH? query status */
} TcBtHfCallHandlingEnum;

typedef TcBtUint32 TcBtHfSupportedFeatures;
typedef TcBtUint32 TcBtHfConfigMask;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* Supported HF features (AT+BRSF) */
#define TC_BT_HF_SUPPORT_EC_NR_FUNCTION						((TcBtHfSupportedFeatures) 0x00000001)
#define TC_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING		((TcBtHfSupportedFeatures) 0x00000002)
#define TC_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY		((TcBtHfSupportedFeatures) 0x00000004)
#define TC_BT_HF_SUPPORT_VOICE_RECOGNITION					((TcBtHfSupportedFeatures) 0x00000008)
#define TC_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL				((TcBtHfSupportedFeatures) 0x00000010)
#define TC_BT_HF_SUPPORT_ENHANCED_CALL_STATUS				((TcBtHfSupportedFeatures) 0x00000020)
#define TC_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL				((TcBtHfSupportedFeatures) 0x00000040)
#define TC_BT_HF_SUPPORT_CODEC_NEGOTIATION					((TcBtHfSupportedFeatures) 0x00000080)
/* TODO: Add your specialized code here */

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
} TcBtHfCmd;

/* General profile handling, link handling and configuration primitives */
/* Activate profile */
typedef struct
{
    TcBtCmdType                 type;
    TcBtUint8                   maxHFConnections;               /* Max number of HF connections accepted (note a value of 0 will disable support for HF connections) */
    TcBtUint8                   maxHSConnections;               /* Max number of HS connections accepted (note a value of 0 will disable support for HS connections) */
    TcBtHfSupportedFeatures     supportedFeatures;              /* HFP specification supported features bitmask used in AT+BRSF and in the HF SDP record */
    TcBtUint8                   AtCmdRespTimeout;               /* AT command response timeout */
} TcBtHfActivateCmd;

/* Deactivate profile */
typedef struct
{
    TcBtCmdType                 type;
} TcBtHfDeactivateCmd;

/* Configure sniff settings for HF connections */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    //TcBtHfConfigLowPowerMask    mask;                           /* low power configuration bitmask */
    //TcBtBool                    sniffValid;                     /* use new sniff parameter settings below? */
    //TcBtSniffSettings           sniff;                          /* sniff settings to use for current/future connections */
    //TcBtBool                    ssrValid;                       /* use new sniff sub rate parameter settings below? */
    //TcBtUint16                  ssrMaxRemoteLatency;            /* sniff sub rate Max remote Latency setting to use for current/future connections */ 
    //TcBtUint16                  ssrMinRmoteTimeout;             /* sniff sub rate Min remote timeout setting to use for current/future connections */ 
    //TcBtUint16                  ssrMinLocalTimeout;             /* sniff sub rate Min local timeout setting to use for current/future connections */ 
    //TcBtUint8                   lowPowerPriority;               /* priority of HF compared to other profiles */
} TcBtHfConfigLowPowerCmd;

/* Configure audio setup request */
typedef TcBtHfCmd TcBtHfConfigAudioCmd;

/* Connection and audio handling primitives */
/* Connect SLC */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;                         /* BT address of device connecting to. */
    TcBtHfConnectionTypeEnum    connectionType;                 /* Used to indicate if connection must be established to AG /HFG or any of them */
} TcBtHfServiceConnectCmd;

/* Cancel pending SLC connect */
typedef TcBtHfCmd TcBtHfCancelConnectCmd;

/* Disconnect connection */
typedef TcBtHfCmd TcBtHfDisconnectCmd;

/* Audio connection request */
typedef TcBtHfCmd TcBtHfAudioConnectCmd;

/* Audio disconnect request */
typedef TcBtHfCmd TcBtHfAudioDisconnectCmd;


/* Query value of all status indicators (AT+CIND?) */
typedef TcBtHfCmd TcBtHfGetAllStatusIndicatorsCmd;

/* Query current operator selection (AT+COPS=3,0) */
typedef TcBtHfCmd TcBtHfGetCurrentOperatorSelectionCmd;

/* Query Subscriber Number Information (AT+CNUM) */
typedef TcBtHfCmd TcBtHfGetSubscriberNumberInformationCmd;

/* Query List of Current Calls (AT+CLCC) */
typedef TcBtHfCmd TcBtHfGetCurrentCallListCmd;

typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtBool                    enable;
} TcBtHfEnDisCmd;

/* Incoming SCO/eSCO connect response */
typedef TcBtHfEnDisCmd TcBtHfAudioAcceptConnectCmd;

/* Enable/Disable Extended Audio Gateway Error result codes (AT+CMEE) */
typedef TcBtHfEnDisCmd TcBtHfSetExtendedAgErrorResultCodeCmd;

/* Enabled/Disabled Calling Line Notification Indications (AT+CLIP) */
typedef TcBtHfEnDisCmd TcBtHfSetCallNotificationIndicationCmd;

/* Enabled/Disabled Call Waiting Notification (AT+CCWA) */
typedef TcBtHfEnDisCmd TcBtHfSetCallWaitingNotificationCmd;

/* Enabled/Disabled standard status indicatior updates (AT+CMER) */
typedef TcBtHfEnDisCmd TcBtHfSetStatusIndicatorUpdateCmd;

/* Request turning off the AG's NR and EC (AT+NREC) */
typedef TcBtHfEnDisCmd TcBtHfSetEchoAndNoiseCmd;

/* Start/Stop Voice recognition input sequense (AT+BVRA=) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtBool                    start;                          /* Start/stop voice recognition input sequence in the AG */
} TcBtHfSetVoiceRecognitionCmd;

/* Bluetooth Input used to request a Phone number from AG (AT+BINP=1) */
typedef TcBtHfCmd TcBtHfBtInputCmd;

/* Generate DTMF (AT+VTS) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   dtmf;                           /* dtmf value to send with AT+VTS */
} TcBtHfGenerateDtmfCmd;

/* Speaker gain status (AT+VGS) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   gain;                           /* Current status of the local speaker gain */
} TcBtHfSpeakerGainStatusCmd;

/* Mic gain status (AT+VGM) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint8                   gain;                           /* Current status of the local microphone gain */
} TcBtHfMicGainStatusCmd;


/* Dial request (ATD and AT+BLDN) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtHfDialEnum              command;                        /* dial, memory-dial or re-dial */
    TcBtCharString              *number;                        /* number or memory index to dial */
} TcBtHfDialCmd;

/* Answer call (ATA) */
typedef TcBtHfCmd TcBtHfCallAnswerCmd;

/* Reject incoming call or end ongoing call (AT+CHUP) */
typedef TcBtHfCmd TcBtHfCallEndCmd;

/* Call hold/response/handling indication (AT+CHLD and AT+BTRH) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtHfCallHandlingEnum      command;                        /* chld/btrh command */
    TcBtUint8                   index;                          /* call index affected (for chld 1x and 2x) */
} TcBtHfCallHandlingCmd;

/* Unknown downstream AT command request (raw) */
typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtCharString              *atCmdString;                   /* the full formatted and null-terminated AT command string to send */
#if 1 /* Telechips' remark */
	TcBtUint32					atCmdIndex;
#endif
} TcBtHfAtCmdCmd;


/* Security control handling primitives */
/* Set incoming security level */
typedef struct
{
    TcBtCmdType                 type;
    TcBtUint16                  secLevel;
} TcBtHfSecurityInCmd;

/* Set outgoing security level */
typedef struct
{
    TcBtCmdType                 type;
    TcBtUint16                  secLevel;
} TcBtHfSecurityOutCmd;

typedef struct
{
    TcBtCmdType                 type;
    TcBtUint8                   waitSeconds;                    /* Number of seconds the profile shall wait before de-registering a service record upon incoming SLC establishment */
} TcBtHfDeregisterTimeCmd;

typedef struct
{
    TcBtCmdType                 type;
    TcBtBdAddr                  bdAddr;
    TcBtUint16                  indicatorBitMask;               /* bit mask value: if bit0 = 0 then indicator index 0 is requested disabled; 
                                                                        bit1 = 0, indicator 1 disables.... 
                                                                        Made room for up to 16 indicators */
} TcBtHfIndicatorActivationCmd;

typedef struct
{
    TcBtCmdType                 type;                           /* primitive/message identity */
    TcBtBdAddr                  bdAddr;
    TcBtUint32                  codecMask;                      /* Codec to add or remove: see mask definitions in bt_hf.h */
    TcBtBool                    enable;                         /* Enable or disable support for the specific codec */            
    TcBtBool                    sendUpdate;                     /* Whether the codec update should be send to the hfg or not, always recommended on codec upgrade */
}TcBtHfUpdateSupportedCodecCmd;

typedef struct
{
	TcBtCmdType 		type;							/* primitive/message identity */
        TcBtBdAddr                  bdAddr;
	TcBtUint16			ind;
}TcBtHfUpdateHFIndicatorsCmd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_HF_CMD_H__ */
