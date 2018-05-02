/****************************************************************************
 *   FileName    : TC_BT_AVRCP_CMD.h
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
*  Description : This file contains functions for a Bluetooth AVRCP command
*******************************************************************************
*
*	 yy/mm/dd	  ver			 descriptions				 Author
*	 ---------	  --------		 -----------------			 -----------------
*	 10/06/02	  0.1			 created					 Park, Kwang-Jin
*
*******************************************************************************/

#ifndef __TC_BT_AVRCP_CMD_H__
#define __TC_BT_AVRCP_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "BT_CFG.h"
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
{	/* refer to the Bluetooth Specification (Player Application Setting) */
    BTAPP_AV_CT_PAS_EQUALIZER = 1,	/* Attribute 0x00 should not be used */
    BTAPP_AV_CT_PAS_REPEAT,
    BTAPP_AV_CT_PAS_SHUFFLE,
    BTAPP_AV_CT_PAS_SCAN,
    BTAPP_AV_CT_PAS_ATT_END
}BTAPP_AV_CT_PAS_ATTRIBUTE_ENUM;

typedef enum
{
/**** Common ********/
	TC_BT_AVRCP_CONFIG_REQ,
	TC_BT_AVRCP_ACTIVATE_REQ,
	TC_BT_AVRCP_DEACTIVATE_REQ,
	TC_BT_AVRCP_CONNECT_REQ,
	TC_BT_AVRCP_CANCEL_CONNECT_REQ,
	TC_BT_AVRCP_DISCONNECT_REQ,
	TC_BT_AVRCP_SECURITY_IN_REQ,
	TC_BT_AVRCP_SECURITY_OUT_REQ,
	TC_BT_AVRCP_COMMON_END,

/**** Target ********/
	TC_BT_AVRCP_TARGET_END,

/**** Controller ****/
	TC_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ,
	TC_BT_AVRCP_CT_NOTI_REGISTER_REQ,
	TC_BT_AVRCP_CT_PASS_THROUGH_REQ,
	TC_BT_AVRCP_CT_PAS_ATT_ID_REQ,
	TC_BT_AVRCP_CT_PAS_VAL_ID_REQ,
	TC_BT_AVRCP_CT_PAS_ATT_TXT_REQ,
	TC_BT_AVRCP_CT_PAS_VAL_TXT_REQ,
	TC_BT_AVRCP_CT_PAS_ATT_TXT_RES,
	TC_BT_AVRCP_CT_PAS_VAL_TXT_RES,
	TC_BT_AVRCP_CT_PAS_CURRENT_REQ,
	TC_BT_AVRCP_CT_PAS_SET_REQ,
	TC_BT_AVRCP_CT_GET_PLAY_STATUS_REQ,
	TC_BT_AVRCP_CT_GET_ATTRIBUTES_REQ,
	TC_BT_AVRCP_CT_GET_ATTRIBUTES_RES,
	TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ,
	TC_BT_AVRCP_CT_PLAY_REQ,
	TC_BT_AVRCP_CT_SEARCH_REQ,
	TC_BT_AVRCP_CT_CHANGE_PATH_REQ,
	TC_BT_AVRCP_CT_SET_VOLUME_REQ,
	TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ,
	TC_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ,
	TC_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ,
	TC_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ,
	TC_BT_AVRCP_CT_UNIT_INFO_CMD_REQ,
	TC_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ,
	TC_BT_AVRCP_CONTROLLER_END
} TC_BT_AVRCP_CMD_ENUM;


/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#define TC_BT_RESULT_CODE_AVRCP_SUCCESS 					((TcBtResultCode) (0x8000))
#define TC_BT_RESULT_CODE_AVRCP_NOTHING_TO_CANCEL			((TcBtResultCode) (0x8001))
#define TC_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER		((TcBtResultCode) (0x8002))
#define TC_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER			((TcBtResultCode) (0x8003))
#define TC_BT_RESULT_CODE_AVRCP_COMMAND_DISALLOWED			((TcBtResultCode) (0x8004))
#define TC_BT_RESULT_CODE_AVRCP_ALREADY_CONNECTED			((TcBtResultCode) (0x8005))
#define TC_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED	((TcBtResultCode) (0x8006))
#define TC_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED		((TcBtResultCode) (0x8007))
#define TC_BT_RESULT_CODE_AVRCP_CHANNEL_NOT_CONNECTED		((TcBtResultCode) (0x8008))
#define TC_BT_RESULT_CODE_AVRCP_INVALID_VERSION 			((TcBtResultCode) (0x8009))
#define TC_BT_RESULT_CODE_AVRCP_TIMEOUT 					((TcBtResultCode) (0x800a))
#define TC_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE			((TcBtResultCode) (0x800b))
#define TC_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR			((TcBtResultCode) (0x800c))
#define TC_BT_RESULT_CODE_AVRCP_NOT_IMPLEMENTED 			((TcBtResultCode) (0x800d))
#define TC_BT_RESULT_CODE_AVRCP_EXCESS_CONNECTION			((TcBtResultCode) (0x800e))

typedef TcBtUint32 TcBtAvrcpConfigRoleMask;
#define TC_BT_AVRCP_CONFIG_ROLE_STANDARD					((TcBtAvrcpConfigRoleMask)(0x00000000))
#define TC_BT_AVRCP_CONFIG_ROLE_TG_BATT_SUPPORT 			((TcBtAvrcpConfigRoleMask)(0x00000001))

typedef TcBtUint16 TcBtAvrcpConfigSrVersion;
#define TC_BT_AVRCP_CONFIG_SR_VERSION_INVALID				((TcBtAvrcpConfigSrVersion)(0x0000))
#define TC_BT_AVRCP_CONFIG_SR_VERSION_10					((TcBtAvrcpConfigSrVersion)(0x0100))
#define TC_BT_AVRCP_CONFIG_SR_VERSION_13					((TcBtAvrcpConfigSrVersion)(0x0103))
#define TC_BT_AVRCP_CONFIG_SR_VERSION_14					((TcBtAvrcpConfigSrVersion)(0x0104))
#define TC_BT_AVRCP_CONFIG_SR_VERSION_15					((TcBtAvrcpConfigSrVersion)(0x0105))

typedef TcBtUint16 TcBtAvrcpConfigSrFeatureMask;
#define TC_BT_AVRCP_CONFIG_SR_FEAT_NONE 					((TcBtAvrcpConfigSrFeatureMask)(0x0000))	/* No features */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_CAT1_PLAY_REC			((TcBtAvrcpConfigSrFeatureMask)(0x0001))	/* Player/recorder */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_CAT2_MON_AMP 			((TcBtAvrcpConfigSrFeatureMask)(0x0002))	/* Monitor/amplifier */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_CAT3_TUNER				((TcBtAvrcpConfigSrFeatureMask)(0x0004))	/* Tuner */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_CAT4_MENU				((TcBtAvrcpConfigSrFeatureMask)(0x0008))	/* Menu */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_PAS						((TcBtAvrcpConfigSrFeatureMask)(0x0010))	/* Player Application Settings */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_GROUP_NAV				((TcBtAvrcpConfigSrFeatureMask)(0x0020))	/* Group Navigation */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_BROWSING 				((TcBtAvrcpConfigSrFeatureMask)(0x0040))	/* Browsing */
#define TC_BT_AVRCP_CONFIG_SR_FEAT_MULTIPLE_MP				((TcBtAvrcpConfigSrFeatureMask)(0x0080))	/* Multiple media players */

typedef TcBtUint8 TcBtAvrcpPlaybackStatus;
#define TC_BT_AVRCP_PLAYBACK_STATUS_STOPPED 				((TcBtAvrcpPlaybackStatus)0x00)
#define TC_BT_AVRCP_PLAYBACK_STATUS_PLAYING 				((TcBtAvrcpPlaybackStatus)0x01)
#define TC_BT_AVRCP_PLAYBACK_STATUS_PAUSED					((TcBtAvrcpPlaybackStatus)0x02)
#define TC_BT_AVRCP_PLAYBACK_STATUS_FWD_SEEK				((TcBtAvrcpPlaybackStatus)0x03)
#define TC_BT_AVRCP_PLAYBACK_STATUS_REV_SEEK				((TcBtAvrcpPlaybackStatus)0x04)
#define TC_BT_AVRCP_PLAYBACK_STATUS_ERROR					((TcBtAvrcpPlaybackStatus)0xFF)

typedef TcBtUint32 TcBtAvrcpPlaybackPos;
#define TC_BT_AVRCP_PLAYBACK_POS_START						((TcBtAvrcpPlaybackPos)0x00000000)
#define TC_BT_AVRCP_PLAYBACK_POS_END						((TcBtAvrcpPlaybackPos)0xFFFFFFFF)
#define TC_BT_AVRCP_PLAYBACK_POS_INVALID					((TcBtAvrcpPlaybackPos)0xFFFFFFFF)

typedef TcBtUint8 TcBtAvrcpBatteryStatus;
#define TC_BT_AVRCP_BATTERY_STATUS_NORMAL					((TcBtAvrcpBatteryStatus)0x00)
#define TC_BT_AVRCP_BATTERY_STATUS_WARNING					((TcBtAvrcpBatteryStatus)0x01)
#define TC_BT_AVRCP_BATTERY_STATUS_CRITICAL 				((TcBtAvrcpBatteryStatus)0x02)
#define TC_BT_AVRCP_BATTERY_STATUS_EXTERNAL 				((TcBtAvrcpBatteryStatus)0x03)
#define TC_BT_AVRCP_BATTERY_STATUS_FULL_CHARGE				((TcBtAvrcpBatteryStatus)0x04)

typedef TcBtUint8 TcBtAvrcpSystemStatus;
#define TC_BT_AVRCP_SYSTEM_STATUS_POWER_ON					((TcBtAvrcpSystemStatus)0x00)
#define TC_BT_AVRCP_SYSTEM_STATUS_POWER_OFF 				((TcBtAvrcpSystemStatus)0x01)
#define TC_BT_AVRCP_SYSTEM_STATUS_UNPLUGGED 				((TcBtAvrcpSystemStatus)0x02)

typedef TcBtUint16 TcBtAvrcpCharSet;
#define TC_BT_AVRCP_CHARACTER_SET_UTF_8						((TcBtAvrcpCharSet)0x006A) /* Default - as specified at http://www.iana.org/assignments/character-sets */
#define TC_BT_AVRCP_CHARACTER_SET_SIZE						(2)

typedef TcBtUint32 TcBtAvrcpItemMediaAttributeId;
#define TC_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK				(1)
#define TC_BT_AVRCP_ITEM_ATT_MINIMUM						((TcBtAvrcpItemMediaAttributeId)0x00000001)
#define TC_BT_AVRCP_ITEM_ATT_TITLE							((TcBtAvrcpItemMediaAttributeId)0x00000001) /* Mandatory */
#define TC_BT_AVRCP_ITEM_ATT_ARTIST							((TcBtAvrcpItemMediaAttributeId)0x00000002)
#define TC_BT_AVRCP_ITEM_ATT_ALBUM							((TcBtAvrcpItemMediaAttributeId)0x00000003)
#define TC_BT_AVRCP_ITEM_ATT_MEDIA_NUMBER					((TcBtAvrcpItemMediaAttributeId)0x00000004)
#define TC_BT_AVRCP_ITEM_ATT_TOTAL_NUMBER					((TcBtAvrcpItemMediaAttributeId)0x00000005)
#define TC_BT_AVRCP_ITEM_ATT_GENRE							((TcBtAvrcpItemMediaAttributeId)0x00000006)
#define TC_BT_AVRCP_ITEM_ATT_TIME							((TcBtAvrcpItemMediaAttributeId)0x00000007)
#define TC_BT_AVRCP_ITEM_ATT_COUNT							(7)
#define TC_BT_AVRCP_ITEM_ATT_INVALID						((TcBtAvrcpItemMediaAttributeId)0xFFFFFFFF)

typedef TcBtUint8 TcBtAvrcpPTOpId;
#define TC_BT_AVRCP_PT_OP_ID_MIN							((TcBtAvrcpPTOpId)0x00)
#define TC_BT_AVRCP_PT_OP_ID_SELECT 						((TcBtAvrcpPTOpId)0x00)
#define TC_BT_AVRCP_PT_OP_ID_OP 							((TcBtAvrcpPTOpId)0x01)
#define TC_BT_AVRCP_PT_OP_ID_DOWN							((TcBtAvrcpPTOpId)0x02)
#define TC_BT_AVRCP_PT_OP_ID_LEFT							((TcBtAvrcpPTOpId)0x03)
#define TC_BT_AVRCP_PT_OP_ID_RIGHT							((TcBtAvrcpPTOpId)0x04)
#define TC_BT_AVRCP_PT_OP_ID_RIGHT_UP						((TcBtAvrcpPTOpId)0x05)
#define TC_BT_AVRCP_PT_OP_ID_RIGHT_DOWN 					((TcBtAvrcpPTOpId)0x06)
#define TC_BT_AVRCP_PT_OP_ID_LEFT_UP						((TcBtAvrcpPTOpId)0x07)
#define TC_BT_AVRCP_PT_OP_ID_LEFT_DOWN						((TcBtAvrcpPTOpId)0x08)
#define TC_BT_AVRCP_PT_OP_ID_ROOT_MENU						((TcBtAvrcpPTOpId)0x09)
#define TC_BT_AVRCP_PT_OP_ID_SETUP_MENU 					((TcBtAvrcpPTOpId)0x0A)
#define TC_BT_AVRCP_PT_OP_ID_CONTENTS_MENU					((TcBtAvrcpPTOpId)0x0B)
#define TC_BT_AVRCP_PT_OP_ID_FAVOURITE_MENU 				((TcBtAvrcpPTOpId)0x0C)
#define TC_BT_AVRCP_PT_OP_ID_EXIT							((TcBtAvrcpPTOpId)0x0D)
#define TC_BT_AVRCP_PT_OP_ID_0								((TcBtAvrcpPTOpId)0x20)
#define TC_BT_AVRCP_PT_OP_ID_1								((TcBtAvrcpPTOpId)0x21)
#define TC_BT_AVRCP_PT_OP_ID_2								((TcBtAvrcpPTOpId)0x22)
#define TC_BT_AVRCP_PT_OP_ID_3								((TcBtAvrcpPTOpId)0x23)
#define TC_BT_AVRCP_PT_OP_ID_4								((TcBtAvrcpPTOpId)0x24)
#define TC_BT_AVRCP_PT_OP_ID_5								((TcBtAvrcpPTOpId)0x25)
#define TC_BT_AVRCP_PT_OP_ID_6								((TcBtAvrcpPTOpId)0x26)
#define TC_BT_AVRCP_PT_OP_ID_7								((TcBtAvrcpPTOpId)0x27)
#define TC_BT_AVRCP_PT_OP_ID_8								((TcBtAvrcpPTOpId)0x28)
#define TC_BT_AVRCP_PT_OP_ID_9								((TcBtAvrcpPTOpId)0x29)
#define TC_BT_AVRCP_PT_OP_ID_DOT							((TcBtAvrcpPTOpId)0x2A)
#define TC_BT_AVRCP_PT_OP_ID_ENTER							((TcBtAvrcpPTOpId)0x2B)
#define TC_BT_AVRCP_PT_OP_ID_CLEAR							((TcBtAvrcpPTOpId)0x2C)
#define TC_BT_AVRCP_PT_OP_ID_CHANNEL_UP 					((TcBtAvrcpPTOpId)0x30)
#define TC_BT_AVRCP_PT_OP_ID_CHANNEL_DOWN					((TcBtAvrcpPTOpId)0x31)
#define TC_BT_AVRCP_PT_OP_ID_PREV_CHANNEL					((TcBtAvrcpPTOpId)0x32)
#define TC_BT_AVRCP_PT_OP_ID_SOUND_SEL						((TcBtAvrcpPTOpId)0x33)
#define TC_BT_AVRCP_PT_OP_ID_INPUT_SEL						((TcBtAvrcpPTOpId)0x34)
#define TC_BT_AVRCP_PT_OP_ID_DISP_INFO						((TcBtAvrcpPTOpId)0x35)
#define TC_BT_AVRCP_PT_OP_ID_HELP							((TcBtAvrcpPTOpId)0x36)
#define TC_BT_AVRCP_PT_OP_ID_PAGE_UP						((TcBtAvrcpPTOpId)0x37)
#define TC_BT_AVRCP_PT_OP_ID_PAGE_DOWN						((TcBtAvrcpPTOpId)0x38)
#define TC_BT_AVRCP_PT_OP_ID_POWER							((TcBtAvrcpPTOpId)0x40)
#define TC_BT_AVRCP_PT_OP_ID_VOLUME_UP						((TcBtAvrcpPTOpId)0x41)
#define TC_BT_AVRCP_PT_OP_ID_VOLUME_DOWN					((TcBtAvrcpPTOpId)0x42)
#define TC_BT_AVRCP_PT_OP_ID_MUTE							((TcBtAvrcpPTOpId)0x43)
#define TC_BT_AVRCP_PT_OP_ID_PLAY							((TcBtAvrcpPTOpId)0x44)
#define TC_BT_AVRCP_PT_OP_ID_STOP							((TcBtAvrcpPTOpId)0x45)
#define TC_BT_AVRCP_PT_OP_ID_PAUSE							((TcBtAvrcpPTOpId)0x46)
#define TC_BT_AVRCP_PT_OP_ID_RECORD 						((TcBtAvrcpPTOpId)0x47)
#define TC_BT_AVRCP_PT_OP_ID_REWIND 						((TcBtAvrcpPTOpId)0x48)
#define TC_BT_AVRCP_PT_OP_ID_FFORWARD						((TcBtAvrcpPTOpId)0x49)
#define TC_BT_AVRCP_PT_OP_ID_EJECT							((TcBtAvrcpPTOpId)0x4A)
#define TC_BT_AVRCP_PT_OP_ID_FORWARD						((TcBtAvrcpPTOpId)0x4B)
#define TC_BT_AVRCP_PT_OP_ID_BACKWARD						((TcBtAvrcpPTOpId)0x4C)
#define TC_BT_AVRCP_PT_OP_ID_ANGLE							((TcBtAvrcpPTOpId)0x50)
#define TC_BT_AVRCP_PT_OP_ID_SUBPICTURE 					((TcBtAvrcpPTOpId)0x51)
#define TC_BT_AVRCP_PT_OP_ID_F1 							((TcBtAvrcpPTOpId)0x71)
#define TC_BT_AVRCP_PT_OP_ID_F2 							((TcBtAvrcpPTOpId)0x72)
#define TC_BT_AVRCP_PT_OP_ID_F3 							((TcBtAvrcpPTOpId)0x73)
#define TC_BT_AVRCP_PT_OP_ID_F4 							((TcBtAvrcpPTOpId)0x74)
#define TC_BT_AVRCP_PT_OP_ID_F5 							((TcBtAvrcpPTOpId)0x75)
#define TC_BT_AVRCP_PT_OP_ID_VENDOR_DEP 					((TcBtAvrcpPTOpId)0x7E)
#define TC_BT_AVRCP_PT_OP_ID_MAX							((TcBtAvrcpPTOpId)0x7E)
#define TC_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT 				((TcBtAvrcpPTOpId)0xFE)	/* Used specifically for indicating that a pass-through command is for Group Navigation */
#define TC_BT_AVRCP_PT_OP_ID_GROUP_NAV_PREV					((TcBtAvrcpPTOpId)0xFF)	/* Used specifically for indicating that a pass-through command is for Group Navigation */

typedef TcBtUint8 TcBtAvrcpPTState;
#define TC_BT_AVRCP_PT_STATE_PRESS							((TcBtAvrcpPTState)0x00)
#define TC_BT_AVRCP_PT_STATE_RELEASE						((TcBtAvrcpPTState)0x01)
/* #define TC_BT_AVRCP_PT_STATE_PRESS_RELEASE				 ((TcBtUint8)0x02) */

typedef TcBtUint8 TcBtAvrcpPTStatus;
#define TC_BT_AVRCP_PT_STATUS_ACCEPT						((TcBtAvrcpPTStatus)0x00)
#define TC_BT_AVRCP_PT_STATUS_REJECT						((TcBtAvrcpPTStatus)0x01)
#define TC_BT_AVRCP_PT_STATUS_NOT_IMPL						((TcBtAvrcpPTStatus)0x02)

typedef TcBtUint32 TcBtAvrcpNotiMask;
#define TC_BT_AVRCP_NOTI_FLAG_NONE							((TcBtAvrcpNotiMask)0x00000000)
#define TC_BT_AVRCP_NOTI_FLAG_PLAYBACK_STATUS				((TcBtAvrcpNotiMask)0x00000001)
#define TC_BT_AVRCP_NOTI_FLAG_TRACK 						((TcBtAvrcpNotiMask)0x00000002)
#define TC_BT_AVRCP_NOTI_FLAG_TRACK_END 					((TcBtAvrcpNotiMask)0x00000004)
#define TC_BT_AVRCP_NOTI_FLAG_TRACK_START					((TcBtAvrcpNotiMask)0x00000008)
#define TC_BT_AVRCP_NOTI_FLAG_PLAYBACK_POS					((TcBtAvrcpNotiMask)0x00000010)
#define TC_BT_AVRCP_NOTI_FLAG_BATT_STATUS					((TcBtAvrcpNotiMask)0x00000020)
#define TC_BT_AVRCP_NOTI_FLAG_SYSTEM_STATUS 				((TcBtAvrcpNotiMask)0x00000040)
#define TC_BT_AVRCP_NOTI_FLAG_PAS							((TcBtAvrcpNotiMask)0x00000080)
#define TC_BT_AVRCP_NOTI_FLAG_NOW_PLAYING_CONTENT			((TcBtAvrcpNotiMask)0x00000100)
#define TC_BT_AVRCP_NOTI_FLAG_AVAILABLE_PLAYERS 			((TcBtAvrcpNotiMask)0x00000200)
#define TC_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER				((TcBtAvrcpNotiMask)0x00000400)
#define TC_BT_AVRCP_NOTI_FLAG_UIDS							((TcBtAvrcpNotiMask)0x00000800)
#define TC_BT_AVRCP_NOTI_FLAG_VOLUME						((TcBtAvrcpNotiMask)0x00001000)
#define TC_BT_AVRCP_NOTI_FLAG_ALL							((TcBtAvrcpNotiMask)0x00001FFF)
#define TC_BT_AVRCP_NOTI_FLAG_AVRCP13_ONLY					((TcBtAvrcpNotiMask)0x000000FF)

typedef TcBtUint32 TcBtAvrcpNotiRegConfigMask;
#define TC_BT_AVRCP_NOTI_REG_STANDARD						((TcBtAvrcpNotiRegConfigMask)0x00000000) /* It makes a registraion automatically for new notifications */
#define TC_BT_AVRCP_NOTI_REG_NON_PERSISTENT					((TcBtAvrcpNotiRegConfigMask)0x00000001) /* Registrations for notifications are not persistent */

typedef TcBtUint8 TcBtAvrcpPasAttId;
typedef TcBtUint8 TcBtAvrcpPasValId;
typedef struct
{
	TcBtAvrcpPasAttId	attribId;
	TcBtAvrcpPasValId	valueId;
} TcBtAvrcpPasAttValPair;
#define TC_BT_AVRCP_PAS_EQUALIZER_ATT_ID					((TcBtAvrcpPasAttId)0x01)
#define TC_BT_AVRCP_PAS_EQUALIZER_VAL_OFF					((TcBtAvrcpPasValId)0x01)
#define TC_BT_AVRCP_PAS_EQUALIZER_VAL_ON					((TcBtAvrcpPasValId)0x02)

#define TC_BT_AVRCP_PAS_REPEAT_ATT_ID						((TcBtAvrcpPasAttId)0x02)
#define TC_BT_AVRCP_PAS_REPEAT_VAL_OFF						((TcBtAvrcpPasValId)0x01)
#define TC_BT_AVRCP_PAS_REPEAT_VAL_SINGLE					((TcBtAvrcpPasValId)0x02)
#define TC_BT_AVRCP_PAS_REPEAT_VAL_ALL						((TcBtAvrcpPasValId)0x03)
#define TC_BT_AVRCP_PAS_REPEAT_VAL_GROUP					((TcBtAvrcpPasValId)0x04)

#define TC_BT_AVRCP_PAS_SHUFFLE_ATT_ID						((TcBtAvrcpPasAttId)0x03)
#define TC_BT_AVRCP_PAS_SHUFFLE_VAL_OFF 					((TcBtAvrcpPasValId)0x01)
#define TC_BT_AVRCP_PAS_SHUFFLE_VAL_ALL 					((TcBtAvrcpPasValId)0x02)
#define TC_BT_AVRCP_PAS_SHUFFLE_VAL_GROUP					((TcBtAvrcpPasValId)0x03)

#define TC_BT_AVRCP_PAS_SCAN_ATT_ID 						((TcBtAvrcpPasAttId)0x04)
#define TC_BT_AVRCP_PAS_SCAN_VAL_OFF						((TcBtAvrcpPasValId)0x01)
#define TC_BT_AVRCP_PAS_SCAN_VAL_ALL						((TcBtAvrcpPasValId)0x02)
#define TC_BT_AVRCP_PAS_SCAN_VAL_GROUP						((TcBtAvrcpPasValId)0x03)

#define TC_BT_AVRCP_PAS_SPECIFIED_MAX_COUNT 				(4)

#define TC_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN					((TcBtAvrcpPasAttId)0x80)
#define TC_BT_AVRCP_PAS_EXT_ATT_ID_END						((TcBtAvrcpPasAttId)0xFF)

#define TC_BT_AVRCP_PAS_ATT_ID_INVALID						((TcBtAvrcpPasAttId)0x00)
#define TC_BT_AVRCP_PAS_VALUE_ID_INVALID					((TcBtAvrcpPasValId)0x00)

typedef TcBtUint8 TcBtAvrcpPasAttMask;
#define TC_BT_AVRCP_PAS_FLAG_NONE							((TcBtAvrcpPasAttMask)0x00000000)
#define TC_BT_AVRCP_PAS_FLAG_EQUALIZER						((TcBtAvrcpPasAttMask)0x00000001)
#define TC_BT_AVRCP_PAS_FLAG_REPEAT 						((TcBtAvrcpPasAttMask)0x00000002)
#define TC_BT_AVRCP_PAS_FLAG_SHUFFLE						((TcBtAvrcpPasAttMask)0x00000004)
#define TC_BT_AVRCP_PAS_FLAG_SCAN							((TcBtAvrcpPasAttMask)0x00000008)
/* Remaining flags can be used in later specifications */
#define TC_BT_AVRCP_PAS_FLAG_UNKNOWN						((TcBtAvrcpPasAttMask)0x40000000)
#define TC_BT_AVRCP_PAS_FLAG_EXTENDED						((TcBtAvrcpPasAttMask)0x80000000)

#define TC_BT_AVRCP_FEATURE_MASK_SIZE						(4)
typedef TcBtUint32											TcBtAvrcpMpFeatureMask[TC_BT_AVRCP_FEATURE_MASK_SIZE];
#define TC_BT_AVRCP_FEATURE_MASK_0_SELECT					(0x01000000) 
#define TC_BT_AVRCP_FEATURE_MASK_0_UP						(0x02000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_DOWN						(0x04000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_LEFT						(0x08000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_RIGHT					(0x10000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_RIGHT_UP					(0x20000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_RIGHT_DOWN				(0x40000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_LEFT_UP					(0x80000000)
#define TC_BT_AVRCP_FEATURE_MASK_0_LEFT_DOWN				(0x00010000) 
#define TC_BT_AVRCP_FEATURE_MASK_0_ROOT_MENU				(0x00020000)
#define TC_BT_AVRCP_FEATURE_MASK_0_SETUP_MENU				(0x00040000)
#define TC_BT_AVRCP_FEATURE_MASK_0_CONTENTS_MENU			(0x00080000)
#define TC_BT_AVRCP_FEATURE_MASK_0_FAVORITE_MENU			(0x00100000)
#define TC_BT_AVRCP_FEATURE_MASK_0_EXIT						(0x00200000)
#define TC_BT_AVRCP_FEATURE_MASK_0_0						(0x00400000)
#define TC_BT_AVRCP_FEATURE_MASK_0_1						(0x00800000)
#define TC_BT_AVRCP_FEATURE_MASK_0_2						(0x00000100) 
#define TC_BT_AVRCP_FEATURE_MASK_0_3						(0x00000200)
#define TC_BT_AVRCP_FEATURE_MASK_0_4						(0x00000400)
#define TC_BT_AVRCP_FEATURE_MASK_0_5						(0x00000800)
#define TC_BT_AVRCP_FEATURE_MASK_0_6						(0x00001000)
#define TC_BT_AVRCP_FEATURE_MASK_0_7						(0x00002000)
#define TC_BT_AVRCP_FEATURE_MASK_0_8						(0x00004000)
#define TC_BT_AVRCP_FEATURE_MASK_0_9						(0x00008000) 
#define TC_BT_AVRCP_FEATURE_MASK_0_DOT						(0x00000001)
#define TC_BT_AVRCP_FEATURE_MASK_0_ENTER					(0x00000002)
#define TC_BT_AVRCP_FEATURE_MASK_0_CLEAR					(0x00000004)
#define TC_BT_AVRCP_FEATURE_MASK_0_CH_UP					(0x00000008)
#define TC_BT_AVRCP_FEATURE_MASK_0_CH_DOWN					(0x00000010)
#define TC_BT_AVRCP_FEATURE_MASK_0_CH_PREV					(0x00000020)
#define TC_BT_AVRCP_FEATURE_MASK_0_SOUND_SELECT				(0x00000040)
#define TC_BT_AVRCP_FEATURE_MASK_0_INPUT_SELECT				(0x00000080)

#define TC_BT_AVRCP_FEATURE_MASK_1_DISP_INFO				(0x01000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_HELP						(0x02000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_PAGE_UP					(0x04000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_PAGE_DOWN				(0x08000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_POWER					(0x10000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_VOLUME_UP				(0x20000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_VOLUME_DOWN				(0x40000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_MUTE						(0x80000000)
#define TC_BT_AVRCP_FEATURE_MASK_1_PLAY						(0x00010000)
#define TC_BT_AVRCP_FEATURE_MASK_1_STOP						(0x00020000)
#define TC_BT_AVRCP_FEATURE_MASK_1_PAUSE					(0x00040000)
#define TC_BT_AVRCP_FEATURE_MASK_1_RECORD					(0x00080000)
#define TC_BT_AVRCP_FEATURE_MASK_1_REWIND					(0x00100000)
#define TC_BT_AVRCP_FEATURE_MASK_1_FFORWARD					(0x00200000)
#define TC_BT_AVRCP_FEATURE_MASK_1_EJECT					(0x00400000)
#define TC_BT_AVRCP_FEATURE_MASK_1_FORWARD					(0x00800000)
#define TC_BT_AVRCP_FEATURE_MASK_1_BACKWARD					(0x00000100)
#define TC_BT_AVRCP_FEATURE_MASK_1_ANGLE					(0x00000200)
#define TC_BT_AVRCP_FEATURE_MASK_1_SUBPICTURE				(0x00000400)
#define TC_BT_AVRCP_FEATURE_MASK_1_F1						(0x00000800)
#define TC_BT_AVRCP_FEATURE_MASK_1_F2						(0x00001000)
#define TC_BT_AVRCP_FEATURE_MASK_1_F3						(0x00002000)
#define TC_BT_AVRCP_FEATURE_MASK_1_F4						(0x00004000)
#define TC_BT_AVRCP_FEATURE_MASK_1_F5						(0x00008000)
#define TC_BT_AVRCP_FEATURE_MASK_1_VENDOR_UNIQUE			(0x00000001)
#define TC_BT_AVRCP_FEATURE_MASK_1_BASIC_GROUP				(0x00000002)
#define TC_BT_AVRCP_FEATURE_MASK_1_ADV_CONTROL				(0x00000004)
#define TC_BT_AVRCP_FEATURE_MASK_1_BROWSING					(0x00000008)
#define TC_BT_AVRCP_FEATURE_MASK_1_SEARCHING				(0x00000010)
#define TC_BT_AVRCP_FEATURE_MASK_1_ADD_TO_NP				(0x00000020)
#define TC_BT_AVRCP_FEATURE_MASK_1_UIDS_UNIQUE				(0x00000040)
#define TC_BT_AVRCP_FEATURE_MASK_1_ONLY_BROWSE_WA			(0x00000080)

#define TC_BT_AVRCP_FEATURE_MASK_2_ONLY_SEARCH_WA			(0x01000000)
#define TC_BT_AVRCP_FEATURE_MASK_2_NOW_PLAYING				(0x02000000)
#define TC_BT_AVRCP_FEATURE_MASK_2_UID_PERSISTENCY			(0x04000000)

/* Predefined feature masks */
#define TC_BT_AVRCP_FEATURE_MASK_PRE_NONE					(0x00000000)
#define TC_BT_AVRCP_FEATURE_MASK_PRE_0_NUMBERS				(TC_BT_AVRCP_FEATURE_MASK_0_0 | TC_BT_AVRCP_FEATURE_MASK_0_1 | TC_BT_AVRCP_FEATURE_MASK_0_2 | TC_BT_AVRCP_FEATURE_MASK_0_3 | TC_BT_AVRCP_FEATURE_MASK_0_4 | TC_BT_AVRCP_FEATURE_MASK_0_5 | TC_BT_AVRCP_FEATURE_MASK_0_6 | TC_BT_AVRCP_FEATURE_MASK_0_7 | TC_BT_AVRCP_FEATURE_MASK_0_8 | TC_BT_AVRCP_FEATURE_MASK_0_9)
#define TC_BT_AVRCP_FEATURE_MASK_PRE_0_ARROWS				(TC_BT_AVRCP_FEATURE_MASK_0_UP | TC_BT_AVRCP_FEATURE_MASK_0_DOWN | TC_BT_AVRCP_FEATURE_MASK_0_LEFT | TC_BT_AVRCP_FEATURE_MASK_0_RIGHT)
#define TC_BT_AVRCP_FEATURE_MASK_PRE_1_TYPICAL				(TC_BT_AVRCP_FEATURE_MASK_1_PLAY | TC_BT_AVRCP_FEATURE_MASK_1_STOP | TC_BT_AVRCP_FEATURE_MASK_1_PAUSE | TC_BT_AVRCP_FEATURE_MASK_1_FORWARD | TC_BT_AVRCP_FEATURE_MASK_1_BACKWARD)

typedef TcBtUint8 TcBtAvrcpMpTypeMajor;
#define TC_BT_AVRCP_MP_TYPE_MAJOR_NONE						((TcBtAvrcpMpTypeMajor)0x00)
#define TC_BT_AVRCP_MP_TYPE_MAJOR_AUDIO						((TcBtAvrcpMpTypeMajor)0x01)
#define TC_BT_AVRCP_MP_TYPE_MAJOR_VIDEO						((TcBtAvrcpMpTypeMajor)0x02)
#define TC_BT_AVRCP_MP_TYPE_MAJOR_BROAD_AUDIO				((TcBtAvrcpMpTypeMajor)0x04)
#define TC_BT_AVRCP_MP_TYPE_MAJOR_BROAD_VIDEO				((TcBtAvrcpMpTypeMajor)0x08)

typedef TcBtUint32 TcBtAvrcpMpTypeSub;
#define TC_BT_AVRCP_MP_TYPE_SUB_NONE						((TcBtAvrcpMpTypeSub)0x00000000)
#define TC_BT_AVRCP_MP_TYPE_SUB_AUDIO_BOOK					((TcBtAvrcpMpTypeSub)0x00000001)
#define TC_BT_AVRCP_MP_TYPE_SUB_PODCAST						((TcBtAvrcpMpTypeSub)0x00000002)

typedef TcBtUint32 TcBtAvrcpMpConfigMask;
#define TC_BT_AVRCP_TG_MP_REGISTER_CONFIG_NONE				((TcBtAvrcpMpConfigMask)0x00000000)
#define TC_BT_AVRCP_TG_MP_REGISTER_CONFIG_SET_DEFAULT		((TcBtAvrcpMpConfigMask)0x00000001)	/* Mark as default player */

#define TC_BT_AVRCP_MP_ID_INVALID							(0xFFFF) 

typedef TcBtUint32 TcBtAvrcpItemAttMask;
#define TC_BT_AVRCP_ITEM_ATT_MASK_TITLE						((TcBtAvrcpItemAttMask)0x00000001)	/* Mandatory */
#define TC_BT_AVRCP_ITEM_ATT_MASK_ARTIST					((TcBtAvrcpItemAttMask)0x00000002)
#define TC_BT_AVRCP_ITEM_ATT_MASK_ALBUM						((TcBtAvrcpItemAttMask)0x00000004)
#define TC_BT_AVRCP_ITEM_ATT_MASK_MEDIA_NUMBER				((TcBtAvrcpItemAttMask)0x00000008)
#define TC_BT_AVRCP_ITEM_ATT_MASK_TOTAL_NUMBER				((TcBtAvrcpItemAttMask)0x00000010)
#define TC_BT_AVRCP_ITEM_ATT_MASK_GENRE						((TcBtAvrcpItemAttMask)0x00000020)
#define TC_BT_AVRCP_ITEM_ATT_MASK_TIME						((TcBtAvrcpItemAttMask)0x00000040)
#define TC_BT_AVRCP_ITEM_ATT_MASK_ALL						(0) /* refer to the Bluetooth Specification */ /* Exception Case : LG-E400 */
#define TC_BT_AVRCP_ITEM_ATT_MASK_NONE						((TcBtAvrcpItemAttMask)0xFFFFFFFF)

typedef TcBtUint8 TcBtAvrcpFolderDirection;
#define TC_BT_AVRCP_CHANGE_PATH_UP							((TcBtAvrcpFolderDirection)0x00)
#define TC_BT_AVRCP_CHANGE_PATH_DOWN						((TcBtAvrcpFolderDirection)0x01)

typedef TcBtUint8 TcBtAvrcpItemType;
#define TC_BT_AVRCP_ITEM_TYPE_SIZE							(1)
#define TC_BT_AVRCP_ITEM_TYPE_MEDIA_PLAYER					((TcBtAvrcpItemType)0x01)
#define TC_BT_AVRCP_ITEM_TYPE_FOLDER						((TcBtAvrcpItemType)0x02)
#define TC_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT					((TcBtAvrcpItemType)0x03)
#define TC_BT_AVRCP_ITEM_TYPE_INVALID						((TcBtAvrcpItemType)0xFF)

typedef TcBtUint8 TcBtAvrcpScope;
#define TC_BT_AVRCP_SCOPE_MP_LIST							((TcBtAvrcpScope)0x00)
#define TC_BT_AVRCP_SCOPE_MP_FS 							((TcBtAvrcpScope)0x01)
#define TC_BT_AVRCP_SCOPE_SEARCH							((TcBtAvrcpScope)0x02)
#define TC_BT_AVRCP_SCOPE_NPL								((TcBtAvrcpScope)0x03)
#define TC_BT_AVRCP_SCOPE_PLAYING							((TcBtAvrcpScope)0xFE)
#define TC_BT_AVRCP_SCOPE_INVALID							((TcBtAvrcpScope)0xFF)

typedef TcBtUint8 TcBtAvrcpFolderType;
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_SIZE					(1)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_MIXED					((TcBtAvrcpFolderType)0x00)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_TITLES					((TcBtAvrcpFolderType)0x01)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_ALBUMS					((TcBtAvrcpFolderType)0x02)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_ARTISTS				((TcBtAvrcpFolderType)0x03)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_GENRES					((TcBtAvrcpFolderType)0x04)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_PLAYLISTS				((TcBtAvrcpFolderType)0x05)
#define TC_BT_AVRCP_ITEM_FOLDER_TYPE_YEARS					((TcBtAvrcpFolderType)0x06)

typedef TcBtUint8 TcBtAvrcpFolderPlayableType;
#define TC_BT_AVRCP_ITEM_FOLDER_PLAYABLE_SIZE				(1)
#define TC_BT_AVRCP_ITEM_FOLDER_PLAYABLE_NO					((TcBtAvrcpFolderPlayableType)0x00)
#define TC_BT_AVRCP_ITEM_FOLDER_PLAYABLE_YES				((TcBtAvrcpFolderPlayableType)0x01)

typedef TcBtUint8 TcBtAvrcpMediaType;
#define TC_BT_AVRCP_ITEM_MEDIA_TYPE_SIZE					(1)
#define TC_BT_AVRCP_ITEM_MEDIA_TYPE_AUDIO					((TcBtAvrcpMediaType)0x00)
#define TC_BT_AVRCP_ITEM_MEDIA_TYPE_VIDEO					((TcBtAvrcpMediaType)0x01)

typedef TcBtUint8 TcBtAvrcpUid[8];

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef struct
{
	TcBtCmdType 				type;
} TcBtAvrcpCmd;

typedef TcBtAvrcpCmd TcBtAvrcpDefaultCmd;

/**** Common ****/
typedef TcBtAvrcpCmd TcBtAvrcpConfigCmd;
typedef TcBtAvrcpCmd TcBtAvrcpDeactivateCmd;
typedef TcBtAvrcpCmd TcBtAvrcpDisconnectCmd;
typedef TcBtAvrcpCmd TcBtAvrcpSecurityInCmd;
typedef TcBtAvrcpCmd TcBtAvrcpSecurityOutCmd;

typedef struct
{
	TcBtCmdType 				type;						/* primitive/message identity */
	TcBtUint16					avrcpVersion;
 	TcBtString					*nameOfService;
 	TcBtString					*nameOfProvider;
} TcBtAvrcpActivateCmd;

typedef struct
{
	TcBtCmdType 				type;						/* primitive/message identity */
	TcBtBdAddr					deviceAddr;					/* address of device to connect to */
} TcBtAvrcpConnectCmd;

typedef struct
{
	TcBtCmdType 				type;						/* primitive/message identity */
	TcBtBdAddr					deviceAddr;					/* address of device to cancel connect to */
} TcBtAvrcpCancelConnectCmd;

/**** Target ****/
typedef TcBtAvrcpCmd TcBtAvrcpTgMpRegisterCmd;
typedef TcBtAvrcpCmd TcBtAvrcpTgMpUnregisterCmd;

/**** Controller ****/
typedef TcBtAvrcpCmd TcBtAvrcpCtPasAttIdCmd;
typedef TcBtAvrcpCmd TcBtAvrcpCtGetPlayStatusCmd;

typedef struct
{
    TcBtCmdType                 type;
    TcBtUint8                   charsetCount;               /* Number of different character sets supported by the controller - set to 0 to use defaults (only UTF-8) */
    TcBtAvrcpCharSet            *charset;                   /* Pointer to supported character sets - set to NULL to use defaults (only UTF-8) */
} TcBtAvrcpCtInformDispCharsetCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtAvrcpNotiMask			mask;						/* A bitmask specifying which notifications that should be registered */
	TcBtUint32					playbackInterval;			/* Only valid if #AVRCP_NOTI_FLAG_PLAYBACK_POS is set in notiMask */
    TcBtAvrcpNotiRegConfigMask	config;						/* Special configuration for the request */	
} TcBtAvrcpCtNotiRegisterCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtAvrcpPTOpId				key;
	TcBtAvrcpPTState			state;	
} TcBtAvrcpCtPassThroughCmd;

typedef struct
{
	TcBtCmdType					type;	
	TcBtAvrcpPasAttId			attId;						/* Attribute to retrieve values for */
} TcBtAvrcpCtPasValIdCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint8					attIdCount;					/* Number of attributes to retrieve text for */
	TcBtAvrcpPasAttId			*attId; 					/* Pointer to attribute IDs */
} TcBtAvrcpCtPasAttTxtCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtAvrcpPasAttId			attId;						/* Same attribute ID as specified in corresponding request */
	TcBtUint8					valIdCount;					/* Number of values to retrieve text for */
	TcBtAvrcpPasValId			*valId; 					/* Pointer to value IDs */
} TcBtAvrcpCtPasValTxtCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint8					attIdCount;					/* Number of attributes to retrieve current value for */
	TcBtAvrcpPasAttId			*attId; 					/* Pointer to attribute IDs */
} TcBtAvrcpCtPasCurrentCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint8					attValPairCount;			/* Number of PAS attributes to change change value for */
	TcBtAvrcpPasAttValPair		*attValPair;				/* Pointer to attribute/value pairs */
} TcBtAvrcpCtPasSetCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtAvrcpScope				scope;
	TcBtAvrcpUid				uid;
	TcBtUint16					uidCounter;
	TcBtAvrcpItemAttMask		attributeMask;				/* Mask specifying which attributes to include in the result */
} TcBtAvrcpCtGetAttributesCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtAvrcpScope				scope;
	TcBtUint32					startItem;
	TcBtUint32					endItem;
	TcBtAvrcpItemAttMask		attributeMask;
} TcBtAvrcpCtGetFolderItemsCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtAvrcpScope				scope;
	TcBtUint16					uidCounter;
	TcBtAvrcpUid				uid;
} TcBtAvrcpCtPlayCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtString					*text;
} TcBtAvrcpCtSearchCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtUint16					uidCounter;
	TcBtAvrcpFolderDirection	folderDir;		   /* Direction - up/down, if AVRCP_CHANGE_PATH_UP folderUid will be ignored */
	TcBtAvrcpUid				folderUid;
} TcBtAvrcpCtChangePathCmd;

typedef struct
{
	TcBtCmdType 				type;
	TcBtUint8					volume;
} TcBtAvrcpCtSetVolumeCmd;

typedef struct
{
	TcBtCmdType				  	type;
	TcBtUint32					playerId;
} TcBtAvrcpCtSetAddressedPlayerCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint32					playerId;
} TcBtAvrcpCtSetBrowsedPlayerCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtAvrcpScope				scope;
	TcBtUint16					uidCounter;
	TcBtAvrcpUid				uid;
} TcBtAvrcpCtAddToNowPlayingCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtAvrcpBatteryStatus		batStatus;
} TcBtAvrcpCtInformBatteryStatusCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint16					pDataLen;
	TcBtUint8					*pData;
} TcBtAvrcpCtUnitInfoCmd;

typedef struct
{
	TcBtCmdType					type;
	TcBtUint16					pDataLen;
	TcBtUint8					*pData;
} TcBtAvrcpCtSubUnitInfoCmd;


/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_AVRCP_Activate(unsigned char *service_name, unsigned char *provider_name);
extern int TC_BT_AVRCP_Deactivate(void);
extern int TC_BT_AVRCP_Connect(const TcBtBdAddr *pBdAddr);
extern int TC_BT_AVRCP_Disconnect(void);
extern int TC_BT_AVRCP_CancelConnect(const TcBtBdAddr *pBdAddr);

extern int TC_BT_AVRCP_CtInformDispCharset(TcBtUint8 charsetCount, const TcBtAvrcpCharSet *charset);
extern int TC_BT_AVRCP_CtNotiRegister(TcBtAvrcpNotiMask mask, TcBtUint32 playbackInterval, TcBtAvrcpNotiRegConfigMask config);
extern int TC_BT_AVRCP_CtPassThrough(TcBtAvrcpPTOpId key, TcBtAvrcpPTState state);
extern int TC_BT_AVRCP_CtPasAttId(void);
extern int TC_BT_AVRCP_CtPasValId(TcBtAvrcpPasAttId attId);
extern int TC_BT_AVRCP_CtPasAttTxt(TcBtUint8 attIdCount, const TcBtAvrcpPasAttId *attId);
extern int TC_BT_AVRCP_CtPasValTxt(TcBtAvrcpPasAttId attId, TcBtUint8 valIdCount, const TcBtAvrcpPasValId *valId);
extern int TC_BT_AVRCP_CtPasCurrent(TcBtUint8 attIdCount, const TcBtAvrcpPasAttId *attId);
extern int TC_BT_AVRCP_CtPasSet(TcBtUint8 attValPairCount, const TcBtAvrcpPasAttValPair *attValPair);
extern int TC_BT_AVRCP_CtGetPlayStatus(void);
extern int TC_BT_AVRCP_CtGetAttributes(TcBtAvrcpScope scope, const TcBtUint8 *uid, TcBtUint16 uidCounter, TcBtAvrcpItemAttMask attributeMask);
extern int TC_BT_AVRCP_CtGetFolderItems(TcBtAvrcpScope scope, TcBtUint32 startItem, TcBtUint32 endItem, TcBtAvrcpItemAttMask attributeMask);
extern int TC_BT_AVRCP_CtPlay(TcBtAvrcpScope scope, TcBtUint16 uidCounter, const TcBtUint8 *uid);
extern int TC_BT_AVRCP_CtSearch(const TcBtString *text);
extern int TC_BT_AVRCP_CtChangePath(TcBtUint16 uidCounter, TcBtAvrcpFolderDirection folderDir, const TcBtUint8 *folderUid);
extern int TC_BT_AVRCP_CtSetVolume(TcBtUint8 volume);
extern int TC_BT_AVRCP_CtSetAddressedPlayer(TcBtUint32 playerId);
extern int TC_BT_AVRCP_CtSetBrowsedPlayer(TcBtUint32 playerId);
extern int TC_BT_AVRCP_CtAddToNowPlaying(TcBtAvrcpScope scope, TcBtUint16 uidCounter, const TcBtUint8 *uid);
extern int TC_BT_AVRCP_CtInformBatteryStatus(TcBtAvrcpBatteryStatus batStatus);
extern int TC_BT_AVRCP_CtUnitInfoCmd(TcBtUint16 pDataLen, const TcBtUint8 *pData);
extern int TC_BT_AVRCP_CtSubUnitInfoCmd(TcBtUint16 pDataLen, const TcBtUint8 *pData);

extern unsigned long	gTcBtAvrcpNotification;
extern unsigned char	gTcBtAvrcpPasAttId;
extern unsigned char	gTcBtAvrcpFolderScope;
extern unsigned int		gTcBtAvrcpFolderStart;

extern unsigned int		gTcBtAvrcpUidCounter;
extern unsigned char	gTcBtAvrcpUidCurrent[8];


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_AVRCP_CMD_H__ */
