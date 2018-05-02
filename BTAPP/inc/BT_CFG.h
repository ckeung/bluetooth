/****************************************************************************
 *   FileName    : BT_CFG.h
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
*    14/12/11     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef BT_CFG_H
#define BT_CFG_H

/****************************************** COMMON ******************************************/
//#define BT_EACH_RUN

#ifdef __arm__
//#define BT_NO_LACUCHER

//#define LINUX_3_4				/* Don't modify, Use [qmake TcBtApp.pro CPU=arm LINUX_VER=3.4  ] */
//#define LINUX_3_18			/* Don't modify, Use [qmake TcBtApp.pro CPU=arm LINUX_VER=3.18 ] */
#endif

//BT MODULE DEFINE
#ifdef __arm__
#define TCM3800
//#define TCM3900
#endif

#define BT_AUDIO_SINK			/* enable A2DP SINK application */
#define BT_HANDSFREE_UNIT		/* enable Handsfree application */
#define BT_PBAP_CLIENT 			/* enable PBAP application */
//#define BT_SPP
//#define BT_MAP_CLIENT
//#define BT_HIDD
#ifdef __arm__
//#define BT_PAN
#endif

//#define BT_USE_LEGACY_PAIRING_ONLY	/* Activate when you want to use Legacy Pairing only */
#define BT_CHANGE_LSTO				/* Activate if you want to set LSTO as specific value */
#ifdef BT_CHANGE_LSTO
/* 1sec = 1600 */
#define BT_LINK_SUPERVISION_TIMEOUT			8000 /* =5sec */
#endif

#if defined(BT_HANDSFREE_UNIT) /*|| defined(BT_PTS_TEST) */
#define BT_AUTO_CONNECTION
#endif

//#define TC_BT_CHK_MEM_LEAK
#define BT_AUTOMATIC_ON_SUPPORT


/****************************************** HF ******************************************/
#ifdef BT_HANDSFREE_UNIT

#define BT_AUTO_ACTIVATE_HANDSFREE

//#define BT_HFP_PERIODICEVENT			/* send AT-CMDs to AG regularly */

#define BT_HFP_WIDEBAND_SPEECH
#define BT_HFP_SCO_PCM_INTERFACE
//#define BT_HFP_DUAL_MICROPHONE

#ifdef BT_HFP_DUAL_MICROPHONE
#define AEC_VER_SINGLE_MICROPHONE		/* AEC v1.xx */
//#define AEC_VER_DUAL_MICROPHONE		/* Reserved */
#else
#define AEC_VER_SINGLE_MICROPHONE		/* AEC v1.xx */
#endif

//#define BT_HFP_SRC
#ifdef  BT_HFP_SRC
#define BT_HFP_SAMPLERATE 48000
#endif

#define BT_HFP_RING_FOR_NO_IN_BAND
#ifdef BT_HFP_RING_FOR_NO_IN_BAND
#define BT_HFP_MULTI_RING_SUPPORT
#endif
#define BT_HFP_LOOPBACK_TEST
#define BT_PHONEBOOK_UI_INCLUDE
//#define BT_HFP_EXCLUDE_VOICE_RECOGNITION /* For exclude Voice Recognition */
#define BT_HFP_PERIODIC_AT_CLCC	//Yootest

#define BT_HFP_MIC_MUTE
#define BT_HFP_ENHANCED_CALL_CONTROL_UI_INCLUDE

#if !defined(BT_PTS_TEST)
#define BT_HFP_HF_CONNECTIONS	1	
#define BT_HFP_HS_CONNECTIONS	0	/* Reserved */
#define BT_HFP_MAX_CONNECTIONS	(BT_HFP_HF_CONNECTIONS+BT_HFP_HS_CONNECTIONS)   
#define BT_PBAP_MAX_CONNECTIONS	1   /* Test Func */
#endif

#if (BT_HFP_MAX_CONNECTIONS == 2)				/* Reserved */
#define BT_HFP_2xHF_MULTICALL_NOT_SUPPORTED		/* Only 1 active call */
#elif (BT_HFP_MAX_CONNECTIONS > 2)
#error Check BT_HFP_MAX_CONNECTIONS
#endif

//#define BT_HFP_IDLE_AUDIO_PLAY
#ifdef BT_HFP_WIDEBAND_SPEECH
//#define BT_HFP_UPDATE_SUPPORTED_CODEC		/* Test Func */
#endif
#define BT_HFP_AUTO_HOLD_OFF
//#define BT_HFP_AT_SIRI							/* Iphone's siri cmd example */

#endif

/****************************************** A2DP SINK ******************************************/
#ifdef BT_AUDIO_SINK
#define BT_AUTO_ACTIVATE_AUDIOSINK
#define BT_AUDIO_SINK_USE_STREAM_ANALYZER
#define BT_AUDIO_SINK_AVRCP_PAS
#define BT_AUDIO_SINK_AVRCP_METADATA
//#define BT_AUDIO_SINK_AVRCP_BROWSING
#endif/* BT_AUDIO_SINK */

/****************************************** PBAPC ******************************************/
#ifdef BT_PBAP_CLIENT
#define BT_AUTO_ACTIVATE_PAC
//#define BT_PBAP_MULTISERVER
//#define BT_PBAP_CLIENT_PTS_TEST
//#define VCARD_IMAGE_DISPLAY
#endif

/****************************************** MAPC ******************************************/
#ifdef BT_MAP_CLIENT
#define BT_AUTO_ACTIVATE_MAPC
#endif

/****************************************** SPP ******************************************/
#ifdef BT_SPP
#define BT_AUTO_ACTIVATE_SPP
//#define IPOD_BT_AUTO_ACTIVATE
#endif

/****************************************** HIDD ******************************************/
#ifdef BT_HIDD
#define BT_AUTO_ACTIVATE_HIDD
#endif 

/****************************************** PAN ******************************************/
#ifdef BT_PAN
#define BT_AUTO_ACTIVATE_PAN
#endif 


#endif // BT_CFG_H
