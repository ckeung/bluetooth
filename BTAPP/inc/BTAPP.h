/****************************************************************************
 *   FileName    : BTAPP.h
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
*    13/11/04     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __BTAPP_H__
#define __BTAPP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus


//#include "../inc/BtDataType.h"
#include "TC_BT/inc/BtUtil.h"
//#include"../TC_BT/inc/TC_BT_SD_EVT.h"
#include "inc/BtBase.h"
/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define ILLEGAL_COMMAND		((unsigned short)(0xFFFF))

typedef void (*CMD_HANDLER)(const unsigned char *data, unsigned short length, unsigned char loc);

typedef struct
{
    unsigned short command;
    CMD_HANDLER handler;
} BTAPP_HF_CMD_HDR;

extern BTAPP_HF_CMD_HDR g_BtAppHfCmdHandlers[];


enum
{
    BT_HF_CMD_NO_STATEMENT = 0,

    /* power */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_ACC_ON,
    BT_HF_CMD_ACC_OFF,
    BT_HF_CMD_PWR_ON,
    BT_HF_CMD_PWR_OFF,

    /* activate */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_UNIT_ACTIVATE, 					//5
    BT_HF_CMD_UNIT_DEACTIVATE,

    /* connect */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_UNIT_CONNECT,
    BT_HF_CMD_UNIT_DISCONNECT,

    /* menu */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_OPTIONS_AUTOANSWER,
    BT_HF_CMD_OPTIONS_MICGAIN, 					//10
    BT_HF_CMD_OPTIONS_SMSALARM,
    BT_HF_CMD_OPTIONS_RING,
    BT_HF_CMD_OPTIONS_BUILTIN,
    BT_HF_CMD_OPTIONS_NREC,
    BT_HF_CMD_OPTIONS_CLCC,						//15
    BT_HF_CMD_OPTIONS_BIA,

    /* mode */
    /*-------------------------------------------------------------------*/
    /* incoming */
    BT_HF_CMD_INCOMING_START,
    BT_HF_CMD_INCOMING_INBAND_START,
    BT_HF_CMD_INCOMING_AUDIO_START,
    BT_HF_CMD_INCOMING_END,						//20
    BT_HF_CMD_INCOMING_ANSWER,
    BT_HF_CMD_INCOMING_REJECT,
    BT_HF_CMD_INCOMING_RESPONSEANDHOLD,

    /* outgoing */
    BT_HF_CMD_OUTGOING_START,
    BT_HF_CMD_OUTGOING_END,						//25
    BT_HF_CMD_OUTGOING_DRAW,

    /* call */
    BT_HF_CMD_CALL_START,
    BT_HF_CMD_CALL_END,
    BT_HF_CMD_CALL_TERMINATE,
    BT_HF_CMD_CALL_AUDIOONOFF,					//30
    BT_HF_CMD_CALL_AUDIOACCEPT,
    BT_HF_CMD_CALL_MIC_MUTE,
    BT_HF_CMD_CALL_DTMF,
    //BT_HF_CMD_CALL_ECNR, ==> option setup
    BT_HF_CMD_CALL_RELEASE_ALL_HELD_CALLS,		/* CHLD 0 */
    BT_HF_CMD_CALL_HOLD_AND_ACCEPT,				/* CHLD 2 */
    BT_HF_CMD_CALL_RELEASE_AND_ACCEPT,			/* CHLD 1 */
    BT_HF_CMD_CALL_EXECUTE_ENHANCEDCALL,
    BT_HF_CMD_CALL_LOOPBACK_TEST,

    /* dial */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_DIAL_REDIAL,
    BT_HF_CMD_DIAL_PLACE_BY_NUMBER,				//40

    BT_HF_CMD_VOICE_RECOGNITION,
    BT_HF_CMD_VOICE_RECOGNITION_START,
    BT_HF_CMD_VOICE_RECOGNITION_END,
    BT_HF_CMD_VOICE_RECOGNITION_LEARN,
    BT_HF_CMD_VOICE_RECOGNITION_LEARN_START,	//45
    BT_HF_CMD_VOICE_RECOGNITION_LEARN_END,
    BT_HF_CMD_VOICE_RECOGNITION_TERMINATE,

    /* common */
    /*-------------------------------------------------------------------*/
#ifdef BT_HFP_UPDATE_SUPPORTED_CODEC
    BT_HF_CMD_COMMON_UPDATE_SUPPORTED_CODEC,
#endif

    BT_HF_CMD_COMMON_MIC_UP,
    BT_HF_CMD_COMMON_MIC_DOWN,					//50
    BT_HF_CMD_COMMON_SPEAKER_UP,
    BT_HF_CMD_COMMON_SPEAKER_DOWN,
    BT_HF_CMD_GET_INFORMATION,

    /* util */
    /*-------------------------------------------------------------------*/
    BT_HF_CMD_UTIL_SIMULATION,
    BT_HF_CMD_MAX
};
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

}
#endif /* __cplusplus */

#endif	/* __BTAPP_H__ */

