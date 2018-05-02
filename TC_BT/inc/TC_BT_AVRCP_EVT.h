/****************************************************************************
 *   FileName    : TC_BT_AVRCP_EVT.h
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
*  Description : This file contains functions for a Bluetooth AVRCP event
*******************************************************************************
*
*	 yy/mm/dd	  ver			 descriptions				 Author
*	 ---------	  --------		 -----------------			 -----------------
*	 10/07/07	  0.1			 created					 Park, Kwang-Jin
*
*******************************************************************************/

#ifndef __TC_BT_AVRCP_EVT_H__
#define __TC_BT_AVRCP_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "TC_BT/inc/TC_BT_AVRCP_CMD.h"
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
/**** Common ********/
	TC_BT_AVRCP_CONFIG_CFM = TC_BT_AVRCP_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x200 */					  
	TC_BT_AVRCP_ACTIVATE_CFM,				   
	TC_BT_AVRCP_DEACTIVATE_CFM, 			   
	TC_BT_AVRCP_CONNECT_IND,				   
	TC_BT_AVRCP_DISCONNECT_IND, 			   
	TC_BT_AVRCP_CONNECT_CFM,				   
	TC_BT_AVRCP_REMOTE_FEATURES_IND,		   
	TC_BT_AVRCP_SECURITY_IN_CFM,			   
    TC_BT_AVRCP_SECURITY_OUT_CFM=520,

/**** Target ********/

/**** Controller ****/
	TC_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM,    
	TC_BT_AVRCP_CT_NOTI_REGISTER_CFM,		   
	TC_BT_AVRCP_CT_GET_PLAY_STATUS_CFM, 	   
	TC_BT_AVRCP_CT_PAS_ATT_ID_CFM,			   
	TC_BT_AVRCP_CT_PAS_VAL_ID_CFM,			   
	TC_BT_AVRCP_CT_PAS_ATT_TXT_CFM, 		   
	TC_BT_AVRCP_CT_PAS_VAL_TXT_CFM, 		   
	TC_BT_AVRCP_CT_PAS_CURRENT_CFM, 		   
	TC_BT_AVRCP_CT_PAS_SET_CFM, 			   
	TC_BT_AVRCP_CT_PASS_THROUGH_CFM,		   
	TC_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM,	   
	TC_BT_AVRCP_CT_PLAY_CFM,				   
	TC_BT_AVRCP_CT_SEARCH_CFM,				   
	TC_BT_AVRCP_CT_GET_ATTRIBUTES_CFM,		   
	TC_BT_AVRCP_CT_CHANGE_PATH_CFM, 		   
	TC_BT_AVRCP_CT_SET_VOLUME_CFM,			   
	TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM,   
	TC_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM,	   
	TC_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM,	   
	TC_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM,  
	TC_BT_AVRCP_CT_UNIT_INFO_CMD_CFM,		   
	TC_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM,	   

	TC_BT_AVRCP_CT_PAS_ATT_TXT_IND, 		   
	TC_BT_AVRCP_CT_PAS_VAL_TXT_IND, 		   
	TC_BT_AVRCP_CT_PAS_SET_IND, 			   
	TC_BT_AVRCP_CT_NOTI_UIDS_IND,			   
	TC_BT_AVRCP_CT_NOTI_VOLUME_IND, 		   
	TC_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND,    
	TC_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND,	   
	TC_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND,	   
	TC_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND,	   
	TC_BT_AVRCP_CT_NOTI_TRACK_END_IND,		   
	TC_BT_AVRCP_CT_NOTI_TRACK_START_IND,	   
	TC_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND,   
	TC_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND, 
	TC_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND,	   
	TC_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND,
	
	TC_BT_AVRCP_EVT_TYPE_MAX
} TcBtAvrcpEvtEnum;

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
typedef struct
{
	TcBtAvrcpItemMediaAttributeId	attribId;
	TcBtAvrcpCharSet				charset;
	TcBtUint16						attLen;
	TcBtUint8						*att;
} TcBtAvrcpItemMediaAttributes;

typedef struct
{
	TcBtUint8						Id; 
	TcBtAvrcpCharSet				charset;
	TcBtUint8						textLen; 
	TcBtUint8						*text;
} TcBtAvrcpPasText;

typedef struct
{
	TcBtUint8						roleSupported;	/* Indicates whether the role is supported (#TRUE) or not (#FALSE) */
	TcBtAvrcpConfigRoleMask 		roleConfig;		/* Local role specific configuration - not valid in #TcBtAvrcpRemoteFeaturesInd */
	TcBtAvrcpConfigSrVersion		srAvrcpVersion;	/* The AVRCP version to announce in the role specific service record */
	TcBtAvrcpConfigSrFeatureMask	srFeatures;		/* The role specific feature mask to include in the service record */
	TcBtUint8						*providerName;	/* NUL-terminated string containing the provider name */
	TcBtUint8						*serviceName;	/* NUL-terminated string containing the service name */
} TcBtAvrcpRoleDetails;

typedef struct
{
	TcBtAvrcpItemType				itemType;
	TcBtAvrcpCharSet				charset;
	TcBtAvrcpUid					uid;
	TcBtUint8						type;			/* TcBtAvrcpFolderType or TcBtAvrcpMediaType */
	TcBtUint16						nameLen;		/* Length of folder/media name */
	TcBtUint8						*name;			/* String of folder/media name */
	TcBtAvrcpFolderPlayableType		playableType;	/* Valid only in TC_BT_AVRCP_ITEM_TYPE_FOLDER */
	TcBtUint8						attributeCount;	/* Valid only in TC_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT */
	TcBtAvrcpItemMediaAttributes	*attributeData;	/* Valid only in TC_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT */
} TcBtAvrcpFolderMedia;

typedef struct
{
	TcBtUint16						playerId;
	TcBtAvrcpCharSet				charset;	
	TcBtAvrcpMpTypeMajor			majorType;		/* Major media player types */
	TcBtAvrcpMpTypeSub				subType;		/* Sub media player types */
	TcBtAvrcpPlaybackStatus			playbackStatus;
	TcBtAvrcpMpFeatureMask			featureMask;	/* Media player supported features */
	TcBtUint16						playerNameLen;	/* Length of media player name */
	TcBtUint8						*playerName;	/* String of media player name */
} TcBtAvrcpPlayer;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpActivateCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpDeactivateCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						flag;			
	TcBtBdAddr						deviceAddr;		/* address of device connected to */
} TcBtAvrcpConnectInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						flag;	
	TcBtUint8						localTerminated;
	TcBtReasonCode					reasonCode;
	TcBtSupplier					reasonSupplier;
} TcBtAvrcpDisconnectInd;

typedef struct
{
	unsigned short int 					type;
	TcBtBdAddr						deviceAddr;		/* address of device connected to */	
	TcBtAvrcpRoleDetails			tgFeatures;		/* Details for the target */
	TcBtAvrcpRoleDetails			ctFeatures;		/* Details for the controller */	
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;	
} TcBtAvrcpConnectCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtBdAddr						deviceAddr;		/* address of device connected to */
	TcBtAvrcpRoleDetails			tgFeatures;		/* Details for the target */
	TcBtAvrcpRoleDetails			ctFeatures;		/* Details for the controller */
} TcBtAvrcpRemoteFeaturesInd;

typedef struct
{
	unsigned short int						type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtInformDispCharsetCfm;	

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpNotiMask				notiMask;			/* A bitmask specifying the notifications that were successfully registered (\ref avrcp_noti_flags) */
	TcBtUint32						playbackInterval;	/* Only valid if #AVRCP_NOTI_FLAG_PLAYBACK_POS is set in notiMask */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;	
} TcBtAvrcpCtNotiRegisterCfm;

typedef struct
{
	unsigned short int						type;
	TcBtUint32						songLength;
	TcBtUint32						songPosition;
	TcBtAvrcpPlaybackStatus			playStatus;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;		
} TcBtAvrcpCtGetPlayStatusCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						attIdCount; 	/* Total number of attributes supported by the target */
	TcBtAvrcpPasAttId				*attId; 		/* List of attribute IDs (size is specified by attIdCount) */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;		
} TcBtAvrcpCtPasAttIdCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						valIdCount; 	/* Number of values provided for the attribute */
	TcBtAvrcpPasValId				*valId; 		/* Pointer to the value IDs */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;		
} TcBtAvrcpCtPasValIdCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						textCount;
	TcBtAvrcpPasText				*textData;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtPasAttTxtCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpPasAttId				attId;			/* Same attribute ID as specified in corresponding request */
	TcBtUint8						textCount;
	TcBtAvrcpPasText				*textData;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtPasValTxtCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						attValPairCount;/* Number of attribute/value pairs */
	TcBtAvrcpPasAttValPair			*attValPair;	/* Pointer to attribute/value pairs */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;	
} TcBtAvrcpCtPasCurrentCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;	
} TcBtAvrcpCtPasSetCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						operationId;	/* Same operation ID that was included in the request */
	TcBtUint8						state;			/* Same state that was included in the request */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;		
} TcBtAvrcpCtPassThroughCfm;

typedef struct
{
	unsigned short int						type;
	TcBtUint16						uidCounter;
	TcBtUint16						itemsCount;
	TcBtUint16						itemsDataLen;
	TcBtUint8						*itemsData;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtGetFolderItemsCfm;

typedef struct
{
	unsigned short int						type;
	TcBtResultCode 					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtPlayCfm;

typedef struct
{
	unsigned short int						type;
	TcBtUint16						uidCounter;
	TcBtUint32						numberOfItems;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtSearchCfm;		

typedef struct
{
	unsigned short int						type;
	TcBtUint8						attributeCount;
	TcBtUint16						attribDataLen;	
	TcBtAvrcpItemMediaAttributes	*attribData;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;	
} TcBtAvrcpCtGetAttributesCfm;	

typedef struct
{
	unsigned short int						type;
	TcBtUint32						itemsCount;		/* Number of items in the new folder */
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtChangePathCfm;

typedef struct
{
	unsigned short int						type;
	TcBtUint8						volume;
	TcBtResultCode 					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtSetVolumeCfm;

typedef struct
{
	unsigned short int						type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtSetAddressedPlayerCfm;	

typedef struct
{
	unsigned short int						type;
	TcBtUint16						uidCounter;
	TcBtUint32						itemsCount;
	TcBtUint8						folderDepth;
	TcBtUint16						folderNamesLen;
	TcBtUint8						*folderNames;	/* The folder path */ 
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtSetBrowsedPlayerCfm;

typedef struct
{
	unsigned short int						type;
	TcBtResultCode 					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtAddToNowPlayingCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
} TcBtAvrcpCtInformBatteryStatusCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						textCount;
	TcBtAvrcpPasText				*textData;
} TcBtAvrcpCtPasAttTxtInd;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpPasAttId				attId;			/* Same attribute ID as specified in corresponding request */
	TcBtUint8						textCount;
	TcBtAvrcpPasText				*textData;
} TcBtAvrcpCtPasValTxtInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						attValPairCount;/* Number of PAS attributes to change change value for */
	TcBtAvrcpPasAttValPair			*attValPair;	/* Pointer to attribute/value pairs */
} TcBtAvrcpCtPasSetInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint16						uidCounter;
} TcBtAvrcpCtNotiUidsInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint8						volume;
} TcBtAvrcpCtNotiVolumeInd;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpBatteryStatus			batteryStatus;
} TcBtAvrcpCtNotiBatteryStatusInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint32						playbackPos;	/* Current playback position in milliseconds */
} TcBtAvrcpCtNotiPlaybackPosInd;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpSystemStatus			systemStatus;	/* Current system status */
} TcBtAvrcpCtNotiSystemStatusInd;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpUid					trackUid;		/* UID of the current track */
} TcBtAvrcpCtNotiTrackChangedInd;

typedef struct
{
	unsigned short int 					type;
} TcBtAvrcpCtNotiTrackEndInd;

typedef struct
{
	unsigned short int 					type;
} TcBtAvrcpCtNotiTrackStartInd;

typedef struct
{
	unsigned short int 					type;
	TcBtAvrcpPlaybackStatus			playbackStatus;	/* Current playback status (\ref avrcp_noti_pb_status) */
} TcBtAvrcpCtNotiPlaybackStatusInd;

typedef struct
{
	unsigned short int 					type;
} TcBtAvrcpCtNotiAvailablePlayersInd;

typedef struct
{
	unsigned short int 					type;
} TcBtAvrcpCtNotiNowPlayingInd;

typedef struct
{
	unsigned short int 					type;
	TcBtUint32						playerId;		/* ID of the new addressed player */
	TcBtUint16						uidCounter;
} TcBtAvrcpCtSetAddressedPlayerInd;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
	TcBtUint16						pDataLen;
	TcBtUint8						*pData;
} TcBtAvrcpCtUnitInfoCmdCfm;

typedef struct
{
	unsigned short int 					type;
	TcBtResultCode					resultCode;
	TcBtSupplier					resultSupplier;
	TcBtUint16						pDataLen;
	TcBtUint8						*pData;
} TcBtAvrcpCtSubUnitInfoCmdCfm;


/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_AVRCP_ReceiveEvent(TcBtEvt *pTcBtEvt);
extern int TC_BT_AVRCP_ActivateCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_DeactivateCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_ConnectInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_DisconnectInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_ConnectCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_RemoteFeaturesInd(void *pTcBtEvt);

extern int TC_BT_AVRCP_CtInformDispCharsetCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiRegisterCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtGetPlayStatusCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtPasAttIdCfm(void *pTcBtEvt);			
extern int TC_BT_AVRCP_CtPasValIdCfm(void *pTcBtEvt);		   
extern int TC_BT_AVRCP_CtPasAttTxtCfm(void *pTcBtEvt);		
extern int TC_BT_AVRCP_CtPasValTxtCfm(void *pTcBtEvt);			
extern int TC_BT_AVRCP_CtPasCurrentCfm(void *pTcBtEvt); 	  
extern int TC_BT_AVRCP_CtPasSetCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtPassThroughCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtGetFolderItemsCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtPlayCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSearchCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtGetAttributesCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtChangePathCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSetVolumeCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSetAddressedPlayerCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSetBrowsedPlayerCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtAddToNowPlayingCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtInformBatteryStatusCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtUnitInfoCmdCfm(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSubUnitInfoCmdCfm(void *pTcBtEvt);

extern int TC_BT_AVRCP_CtPasAttTxtInd(void *pTcBtEvt);	
extern int TC_BT_AVRCP_CtPasValTxtInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtPasSetInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiUidsInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiVolumeInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiBatteryStatusInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiPlaybackPosInd(void *pTcBtEvt);  
extern int TC_BT_AVRCP_CtNotiSystemStatusInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiTrackChangedInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiTrackEndInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiTrackStartInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiPlaybackStatusInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiAvailablePlayersInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtNotiNowPlayingInd(void *pTcBtEvt);
extern int TC_BT_AVRCP_CtSetAddressedPlayerInd(void *pTcBtEvt);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_AVRCP_EVT_H__ */
