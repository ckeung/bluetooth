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

#ifndef TCBTHFEVT_H
#define TCBTHFEVT_H

#include "BT_CFG.h"
#include "TcBtHFTypes.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef BT_HANDSFREE_UNIT

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
#define COM_SUBHEADER_MSG_LENGTH        (9)
#define MAX_STRING_LENGTH               (512)
#define MAX_HF_MESSAGE_LENGTH           (MAX_STRING_LENGTH*2)
#define GET_MAX_STRING_LENGTH(length) (((length) < MAX_STRING_LENGTH) ? (length) : MAX_STRING_LENGTH)

typedef struct
{
    unsigned int mSize;
    void 		*mAddr;
} TcBtHFEvt;

enum
{
    MSG_TYPE_NO_STATEMENT = 0,
    MSG_EVENT_TYPE,
    MSG_INFO_TYPE,
    MSG_CMD_TYPE,
    MSG_REQ_INFO_TYPE,
    MSG_TASK_COMMAND,
    MSG_TYPE_MAX
};

/* HFP */
enum
{
    /*	BT_LCDUPDATE_GROUP_ID_HFP_START = BT_LCDUPDATE_GROUP_ID_HFP, */
    BT_HF_EVT_NO_STATEMENT  = 0,

    BT_HF_EVT_UPDATE_STATUS, //= BT_SYS_EVT_UPDATE_STATUS,
    BT_HF_EVT_POWEROFF,
    BT_HF_EVT_NOT_ACTIVATED,
    BT_HF_EVT_ALREADY_ACTIVATED,
    BT_HF_EVT_ACTIVATE_CFM,
    BT_HF_EVT_ACTIVATE_ERR,
    BT_HF_EVT_DEACTIVATE_CFM,
    BT_HF_EVT_DEACTIVATE_ERR,

    BT_HF_EVT_SERVICE_CONNECT_IND,
    BT_HF_EVT_SERVICE_CONNECT_NOTIFICATION_IND = 10,
    BT_HF_EVT_SERVICE_CONNECT_ERR,
    BT_HF_EVT_ALREADY_CONNECTED,
    BT_HF_EVT_NOT_CONNECTED,
    BT_HF_EVT_DISCONNECT_BY_REMOTE,
    BT_HF_EVT_DISCONNECT_IND,
    BT_HF_EVT_DISCONNECT_ERR,
    BT_HF_EVT_DISCONNECT_LINK_LOSS,

    BT_HF_EVT_AUDIO_IND,
    BT_HF_EVT_AUDIO_ACCEPT_IND,

    BT_HF_EVT_UPDATE_SUPPORTED_CODEC_CFM = 20,
    BT_HF_EVT_SELECTED_CODEC_IND,

    BT_HF_EVT_INCOMING_IND,
    BT_HF_EVT_INCOMING_TIMEOUT_IND,
    BT_HF_EVT_INCOMING_END_IND,

    BT_HF_EVT_CIEV_CALL_ZERO_IND,
    BT_HF_EVT_CIEV_CALL_ONE_IND,
    BT_HF_EVT_CIEV_CALLSETUP_IND,
    BT_HF_EVT_CIEV_CALLHELD_IND,
    BT_HF_EVT_CIEV_SERVICE_IND,
    BT_HF_EVT_CIEV_ROAM_IND = 30,
    BT_HF_EVT_CIEV_SIGNAL_IND,
    BT_HF_EVT_CIEV_BATTCHG_IND,
    BT_HF_EVT_CIEV_MESSAGE_IND,

    BT_HF_EVT_ANSWER_REQ,
    BT_HF_EVT_REJECT_REQ,
    BT_HF_EVT_REJECT_ERR,
    BT_HF_EVT_TERMINATE_REQ,
    BT_HF_EVT_PLACE_REQ,
    BT_HF_EVT_CALL_ALREADY_EXIST,

    BT_HF_EVT_CLIP_IND = 40,
    BT_HF_EVT_OUTGOING_NUMBER_IND,

    BT_HF_EVT_OUTGOING_FROM_HFG_IND,
    BT_HF_EVT_OUTGOING_START_IND,
    BT_HF_EVT_OUTGOING_END_IND,
    BT_HF_EVT_OUTGOING_RETRY,
    BT_HF_EVT_OUTGOING_DIAL_INF,

    BT_HF_EVT_CALL_TIMESTAMP_COUNT_ID,
    BT_HF_EVT_CALL_TIMESTAMP_CLEAR_ID,

    BT_HF_EVT_SET_SPEAKER_VOLUME_IND,
    BT_HF_EVT_SET_MIC_GAIN_IND = 50,

    BT_HF_EVT_LOOPBACK_TEST_START_IND,
    BT_HF_EVT_LOOPBACK_TEST_END_IND,

    BT_HF_EVT_SUPPORT_VOICE_RECOGNITION_IND,

    BT_HF_EVT_VOICE_RECOGNITION_START_IND,
    BT_HF_EVT_VOICE_RECOGNITION_END_IND,
    BT_HF_EVT_VOICE_RECOGNITION_LEARN_START_IND,
    BT_HF_EVT_VOICE_RECOGNITION_LEARN_END_IND,

#if 1 /* Telechips' 2008.12 QA - 3.6 (a), (b) */
    BT_HF_EVT_CLCC_IND,
#endif

    BT_HF_EVT_CCWA_IND,

    BT_HF_EVT_CMEE_IND = 60,

    BT_HF_EVT_OUTGOING_BUSY_LINE_IND,

    /* Headset - reserved */
    BT_HS_EVT_SERVICE_CONNECT_IND,
    BT_HS_EVT_SERVICE_CONNECT_ERR,
    BT_HS_EVT_ALREADY_CONNECTED,
    BT_HS_EVT_NOT_CONNECTED,
    BT_HS_EVT_DISCONNECT_BY_REMOTE,
    BT_HS_EVT_DISCONNECT_IND,
    BT_HS_EVT_DISCONNECT_ERR,
    BT_HS_EVT_DISCONNECT_LINK_LOSS,

    BT_HS_EVT_RING_IND = 70,
    BT_HS_EVT_RING_START_IND,
    BT_HS_EVT_RING_TIMEOUT_IND,
    BT_HS_EVT_RING_END_IND,

    BT_HS_EVT_AUDIO_ON_IND,
    BT_HS_EVT_AUDIO_OFF_IND,

    BT_HS_EVT_SET_SPEAKER_VOLUME_IND,
    BT_HS_EVT_SET_MIC_GAIN_IND,

    BT_HF_EVT_DTMF_INF,

    BT_HF_EVT_OPTIONS_AUTOANSWER_INF,
    BT_HF_EVT_OPTIONS_MICGAIN_INF,
    BT_HF_EVT_OPTIONS_SMSALARM_INF,
    BT_HF_EVT_OPTIONS_RING_INF,
    BT_HF_EVT_OPTIONS_BUILTIN_INF,
    BT_HF_EVT_OPTIONS_NREC_INF,
    BT_HF_EVT_OPTIONS_CLCC_INF,
    BT_HF_EVT_OPTIONS_BIA_INF,

    BT_HF_EVT_GET_INFORMATION_INF,
    BT_HF_EVT_SET_MIC_MUTE_INF,
    BT_HF_EVT_MAX
};

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

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

/*************** HFP Event ***************/
extern void TcBtHFEvt_MessageSend(unsigned char type, unsigned short id, const unsigned char *data, unsigned short length);
extern void TcBtHFEvt_EvtBtDirectFourDataMessageSend(unsigned short id, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);
extern void TcBtHFEvt_EvtBtFourDataMessageSend(unsigned short id, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);
extern void TcBtHFEvt_EvtBTInfoMessageSend(unsigned short id, unsigned char *data, unsigned int size);


#endif /*BT_HANDSFREE_UNIT*/
/*****************************************************************************
*
* debug
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // TCBTHFEVT_H
