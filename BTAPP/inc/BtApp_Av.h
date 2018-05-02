/****************************************************************************
 *   FileName    : BTAPP_AV.h
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
*  Description : This file contains functions and variables for a Bluetooth A2DP application
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    09/07/22     0.1            created                     Ji, Seung-Nam.
*******************************************************************************/

#ifndef __BTAPP_AV_H__
#define __BTAPP_AV_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "TC_BT/inc/TC_BT_AVRCP_CMD.h"
/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/

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
#define BTAPP_AV_EVT_SUCCESS			0x00
#define BTAPP_AV_EVT_FAILURE			0x01
#define BTAPP_AV_EVT_ALREADY			0x02
#define BTAPP_AV_EVT_NOT_YET			0x03

#define BTAPP_AV_PCM_BUF_SIZE			50*1024	/* 1024byte = 256frame = 22.7usec*256 = 5.811msec in 44.1Khz */ 
#define BTAPP_AV_PCM_BUF_SEND			 1*1024
#define BTAPP_AV_ERROR_SUCCESS			 0
#define BTAPP_AV_ERROR_OPEN				-1
#define BTAPP_AV_ERROR_OVERFLOW			-2

/* AVRCP Cotroller Key */
#define BTAPP_AV_CT_KEY_ID_PLAY			0x44	/* refer to the AV/C Panel Subunit Specification */
#define BTAPP_AV_CT_KEY_ID_STOP			0x45
#define BTAPP_AV_CT_KEY_ID_PAUSE		0x46
#define BTAPP_AV_CT_KEY_ID_REWIND		0x48
#define BTAPP_AV_CT_KEY_ID_FFORWARD		0x49
#define BTAPP_AV_CT_KEY_ID_FORWARD		0x4B
#define BTAPP_AV_CT_KEY_ID_BACKWARD		0x4C

/* AVRCP Cotroller Player Application Setting */
#define BTAPP_AV_CT_PAS_VALID_MARK		0xFF
#define BTAPP_AV_CT_PAS_TEXT_SIZE		50		/* AVRCP PAS Attribute or Value Text Size */

#define BTAPP_AV_CT_PAS_EQUALIZER_NONE	0x00	/* Not Support */
#define BTAPP_AV_CT_PAS_EQUALIZER_OFF	0x01	/* refer to the Bluetooth Specification */
#define BTAPP_AV_CT_PAS_EQUALIZER_ON	0x02

#define BTAPP_AV_CT_PAS_REPEAT_NONE		0x00	/* Not Support */
#define BTAPP_AV_CT_PAS_REPEAT_OFF		0x01
#define BTAPP_AV_CT_PAS_REPEAT_SINGLE	0x02
#define BTAPP_AV_CT_PAS_REPEAT_ALL		0x03
#define BTAPP_AV_CT_PAS_REPEAT_GROUP	0x04

#define BTAPP_AV_CT_PAS_SHUFFLE_NONE 	0x00	/* Not Support */
#define BTAPP_AV_CT_PAS_SHUFFLE_OFF 	0x01
#define BTAPP_AV_CT_PAS_SHUFFLE_ALL 	0x02
#define BTAPP_AV_CT_PAS_SHUFFLE_GROUP	0x03

#define BTAPP_AV_CT_PAS_SCAN_NONE		0x00	/* Not Support */
#define BTAPP_AV_CT_PAS_SCAN_OFF		0x01
#define BTAPP_AV_CT_PAS_SCAN_ALL		0x02
#define BTAPP_AV_CT_PAS_SCAN_GROUP		0x03

#define BTAPP_AV_CT_PAS_BATT_NORMAL		0x00	/* refer to the Bluetooth Specification */
#define BTAPP_AV_CT_PAS_BATT_WARNING	0x01
#define BTAPP_AV_CT_PAS_BATT_CRITICAL 	0x02
#define BTAPP_AV_CT_PAS_BATT_EXTERNAL 	0x03
#define BTAPP_AV_CT_PAS_BATT_FULL		0x04

/* AVRCP Cotroller Metadata */
#define BTAPP_AV_CT_METADATA_SIZE		50		/* AVRCP Metadata Text Size */
#define BTAPP_AV_CT_METADATA_WAIT		15		/* AVRCP Metadata Response Wait Time  : 3 sec   */
#define BTAPP_AV_CT_METADATA_RETRY		3		/* AVRCP Metadata Request Retry Count : 3 times */

/* AVRCP Cotroller Notification */ /* refer to "TC_BT_AVRCP_CMD.h" */
#define BTAPP_AV_CT_NOTI_FLAG_NONE					0x00000000
#define BTAPP_AV_CT_NOTI_FLAG_PLAYBACK_STATUS		0x00000001
#define BTAPP_AV_CT_NOTI_FLAG_TRACK 				0x00000002
#define BTAPP_AV_CT_NOTI_FLAG_TRACK_END 			0x00000004
#define BTAPP_AV_CT_NOTI_FLAG_TRACK_START			0x00000008
#define BTAPP_AV_CT_NOTI_FLAG_PLAYBACK_POS			0x00000010
#define BTAPP_AV_CT_NOTI_FLAG_BATT_STATUS			0x00000020
#define BTAPP_AV_CT_NOTI_FLAG_SYSTEM_STATUS 		0x00000040
#define BTAPP_AV_CT_NOTI_FLAG_PAS					0x00000080
#define BTAPP_AV_CT_NOTI_FLAG_NOW_PLAYING_CONTENT	0x00000100
#define BTAPP_AV_CT_NOTI_FLAG_AVAILABLE_PLAYERS 	0x00000200
#define BTAPP_AV_CT_NOTI_FLAG_ADDRESSED_PLAYER		0x00000400
#define BTAPP_AV_CT_NOTI_FLAG_UIDS					0x00000800
#define BTAPP_AV_CT_NOTI_FLAG_VOLUME				0x00001000
#define BTAPP_AV_CT_NOTI_FLAG_ALL					0x00001FFF
#define BTAPP_AV_CT_NOTI_FLAG_AVRCP13_ONLY			0x000000FF

/* AVRCP Cotroller Browsing */
#define BTAPP_AV_CT_BROWSING_MP_LIST	0x00	/* refer to the scopes of Bluetooth Specification */
#define BTAPP_AV_CT_BROWSING_MP_FS		0x01
#define BTAPP_AV_CT_BROWSING_SEARCH		0x02
#define BTAPP_AV_CT_BROWSING_NPL		0x03
#define BTAPP_AV_CT_BROWSING_INVALID	0xFF

#define BTAPP_AV_CT_BROWSING_ITEM		10		/* AVRCP Meta Browsing Item Count */
#define BTAPP_AV_CT_BROWSING_TEXT		64		/* AVRCP Meta Browsing Text Size  */

#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
#define BTAPP_AV_STOP			0x00	/* =TC_BT_AVRCP_PLAYBACK_STATUS_STOPPED */
#define BTAPP_AV_PLAY			0x01	/* =TC_BT_AVRCP_PLAYBACK_STATUS_PLAYING */
#define BTAPP_AV_PAUSE			0x02	/* =TC_BT_AVRCP_PLAYBACK_STATUS_PAUSED  */

#define BTAPP_AV_1st_PHASE		1
#define BTAPP_AV_2nd_PHASE		2

#define BTAPP_AV_ANALYZE_TIME	5
#define BTAPP_AV_NOT_MUSIC		0
#define BTAPP_AV_MUSIC			1
#endif

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/* AVRCP Cotroller Player Application Setting */

/* AVRCP Cotroller Metadata */
typedef enum
{
	BTAPP_AV_CT_ATTRIBUTE_UPDATE = 0,
	BTAPP_AV_CT_PLAY_STATUS_UPDATE,
	BTAPP_AV_CT_COMMAND_END
}BTAPP_AV_CT_COMMAND_ENUM;

typedef enum
{
	BTAPP_AV_CT_META_DISCONNECT = 0,	
	BTAPP_AV_CT_META_CONNECT,
	BTAPP_AV_CT_META_CHECK,
	BTAPP_AV_CT_META_VALID,	
	BTAPP_AV_CT_META_INVALID,		
	BTAPP_AV_CT_META_END
}BTAPP_AV_CT_META_VALIDITY_ENUM;

typedef struct
{
/* GET ELEMENT ATTRIBUTE */
	char NameOfTitle[BTAPP_AV_CT_METADATA_SIZE];	/* 0x1 : Title of the media */
	char NameOfArtist[BTAPP_AV_CT_METADATA_SIZE];	/* 0x2 : Name of the artist */
	char NameOfAlbum[BTAPP_AV_CT_METADATA_SIZE];	/* 0x3 : Name of the album */
	char NameOfGenre[BTAPP_AV_CT_METADATA_SIZE];	/* 0x6 : Genre */	
	long NumberOfTrack;								/* 0x4 : Number of the media */
	long NumberOfTotal;								/* 0x5 : Total number of the media */
	long NumberOfTime;								/* 0x7 : Playing time in millisecond */	
	long NumberOfAttribute;							/* Number of valid attribute */
/* GET PLAY STATUS */
	unsigned long SongLength;	/* The total length of the playing song in millisecond */
	unsigned long SongPosition;	/* The current position of the playing in millisecond elapsed*/
	unsigned long PlayStatus;	/* Current Status of playing*/
}BTAPP_AV_CT_METADATA_TYPE;

/* AVRCP Cotroller Browsing */
typedef struct 
{
	unsigned int  Index;
	unsigned int  Type;
	unsigned char UID[8];
}BTAPP_AV_CT_ITEM_TYPE;

typedef struct 
{
	unsigned int  TotalCount;
/*	unsigned char Name[BTAPP_AV_CT_BROWSING_TEXT]; */ 
 
	unsigned char ItemsScope;
	unsigned int  ItemsCount; 
	BTAPP_AV_CT_ITEM_TYPE Item[BTAPP_AV_CT_BROWSING_ITEM];
	int BrowsedPlayerID;
	int AddressedPlayerID;
}BTAPP_AV_CT_BROWSING_TYPE;

#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
typedef struct
{
	int analyzeFlag;			/* TRUE or FALSE */
	int referenceStatus;		/* BTAPP_AV_PLAY or BTAPP_AV_PAUSE */
	int skipUpdateRefStatus;	/* TRUE or FALSE */
	unsigned int streamCounters[BTAPP_AV_ANALYZE_TIME];	/* to count the number of stream */
	int counterIndex;			/* index of stream counter */
	int isHFRelatedMode;		/* TRUE or FALSE */

	int objectiveStatus;				/* BTAPP_AV_PLAY or BTAPP_AV_PAUSE */
	int analysisResult;					/* BTAPP_AV_NOT_MUSIC or BTAPP_AV_MUSIC */
	int isTargetPlayStatusAvailable;	/* TRUE or FALSE */
	int playRetryCnt;
}BTAPP_AV_STREAM_ANALYZER_TYPE;
#endif

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
extern unsigned char					gBtAvMetaValidity;
extern unsigned char					gBtAvMetaFlag;
extern BTAPP_AV_CT_METADATA_TYPE		gstBtAvMetaData;
#endif

#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
extern unsigned char					gBtAvBrowValidity;
extern unsigned int						gBtAvBrowPlayerIdForPts;
extern BTAPP_AV_CT_BROWSING_TYPE		gstBtAvBrowsing;
#endif


#if 0
extern TcBtUint16 BtApp_AV_SNK_CloseStream(void);
#endif
extern void BtApp_AV_SNK_AudioStart(void);
extern void BtApp_AV_SNK_AudioEnd(void);
extern void BtApp_AV_SNK_Activate(void);
extern void BtApp_AV_SNK_Deactivate(void);
extern void BtApp_AV_SNK_StreamCheckStart(unsigned char enable);
#if defined(BT_AUDIO_SINK_AVRCP_PAS)
extern void BtApp_AV_CT_PasValidityCheck(void);
extern void BtApp_AV_CT_PasValidityCheckStart(unsigned char enable);
extern void BtApp_AV_CT_PasGetSeetings(void);
extern void BtApp_AV_CT_PasSetSeetings(unsigned char *setting_list);
#endif
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
extern void BtApp_AV_CT_MetadataInit(void);
extern void BtApp_AV_CT_MetadataCheckStart(unsigned char enable);
#endif
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
extern void BtApp_AV_CT_BrowseSupportEnable(unsigned char enable);
extern void BtApp_AV_CT_GetMPListEnable(unsigned char enable);
extern void BtApp_AV_CT_SetBrowseScope(unsigned int scope);
extern void BtApp_AV_CT_GetFolderItems(unsigned int index, unsigned int items);
extern void BtApp_AV_CT_ChangePath(int index);
#endif
#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
extern void BtApp_AV_SNK_EnterBTAMode(void);
extern void BtApp_AV_SNK_ExitBTAMode(void);
#endif

extern void BtApp_AV_CT_KeySend(unsigned char key, unsigned char release);
extern void BtApp_AV_SNK_AlsaInit(void);
extern void BtApp_AV_SNK_AlsaSetParam(unsigned short sampleRate, unsigned char ch);
extern int BtApp_AV_SNK_AlsaOpen(void);
extern void BtApp_AV_SNK_AlsaClose(void);
extern int BtApp_AV_SNK_AlsaWrite(unsigned char *pBuf, long BufLen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __BTAPP_AV_H__ */
