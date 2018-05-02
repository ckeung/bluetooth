/****************************************************************************
 *   FileName    : TC_BT_A2DP_EVT.h
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
*  Description : This file contains functions for a Bluetooth A2DP event
*******************************************************************************
*
*	 yy/mm/dd	  ver			 descriptions				 Author
*	 ---------	  --------		 -----------------			 -----------------
*	 10/07/07	  0.1			 created					 Park, Kwang-Jin
*
*******************************************************************************/

#ifndef __TC_BT_A2DP_EVT_H__
#define __TC_BT_A2DP_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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
	TC_BT_A2DP_DEACTIVATE_CFM = TC_BT_A2DP_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x100 */
	TC_BT_A2DP_REGISTER_STREAM_HANDLE_CFM,
	TC_BT_A2DP_ACTIVATE_CFM,		 
	TC_BT_A2DP_STREAM_DATA_IND, 	 
	TC_BT_A2DP_QOS_IND, 			 
	TC_BT_A2DP_CONNECT_IND, 		 
	TC_BT_A2DP_CONNECT_CFM, 		 
	TC_BT_A2DP_DISCOVER_CFM,		 
	TC_BT_A2DP_DISCOVER_IND,		 
	TC_BT_A2DP_GET_CAPABILITIES_CFM, 
	TC_BT_A2DP_STATUS_IND,			 
	TC_BT_A2DP_GET_ALL_CAPABILITIES_IND,
	TC_BT_A2DP_GET_CAPABILITIES_IND, 
	TC_BT_A2DP_SET_CONFIGURATION_CFM,
	TC_BT_A2DP_SET_CONFIGURATION_IND,
	TC_BT_A2DP_OPEN_CFM,			 
	TC_BT_A2DP_OPEN_IND,			 
	TC_BT_A2DP_START_CFM,			 
	TC_BT_A2DP_START_IND,			 
	TC_BT_A2DP_CLOSE_CFM,			 
	TC_BT_A2DP_CLOSE_IND,			 
	TC_BT_A2DP_SUSPEND_CFM, 		 
	TC_BT_A2DP_SUSPEND_IND, 		 
	TC_BT_A2DP_RECONFIGURE_CFM, 	 
	TC_BT_A2DP_RECONFIGURE_IND, 	 
	TC_BT_A2DP_GET_CONFIGURATION_CFM,
	TC_BT_A2DP_GET_CONFIGURATION_IND,
	TC_BT_A2DP_ABORT_CFM,			 
	TC_BT_A2DP_ABORT_IND,			 
	TC_BT_A2DP_DISCONNECT_IND,		 
	TC_BT_A2DP_STREAM_MTU_SIZE_IND,  
	TC_BT_A2DP_SECURITY_CONTROL_IND, 
	TC_BT_A2DP_SECURITY_CONTROL_CFM, 
	TC_BT_A2DP_DELAY_REPORT_IND,	 
  
	TC_BT_A2DP_EVT_TYPE_MAX
} TcBtA2dpEvtEnum;
  
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#define TC_BT_RESULT_CODE_AV_SUCCESS					((TcBtResultCode) (0x8000))
#define TC_BT_RESULT_CODE_AV_UNACCEPTABLE_PARAMETER		((TcBtResultCode) (0x8001))
#define TC_BT_RESULT_CODE_AV_INVALID_ROLE				((TcBtResultCode) (0x8002))
#define TC_BT_RESULT_CODE_AV_SDS_REGISTER_FAILED		((TcBtResultCode) (0x8003))
#define TC_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT		((TcBtResultCode) (0x8004))
#define TC_BT_RESULT_CODE_AV_MAX_NUM_OF_CONNECTIONS		((TcBtResultCode) (0x8005))
#define TC_BT_RESULT_CODE_AV_NOT_CONNECTED				((TcBtResultCode) (0x8006))
#define TC_BT_RESULT_CODE_AV_SIGNAL_TIMEOUT 			((TcBtResultCode) (0x8007))

/* Refer to "l2cap_prim.h" */
#define TC_BT_L2CA_DISCONNECT_NORMAL					((TcBtReasonCode) (0x0000)) /* Spec ok */
#define TC_BT_L2CA_DISCONNECT_CONFIG_MISMATCH			((TcBtReasonCode) (0xFFF9))
#define TC_BT_L2CA_DISCONNECT_CONFIG_ILLEGAL			((TcBtReasonCode) (0xFFF8))
#define TC_BT_L2CA_DISCONNECT_RTM_OVERFLOW				((TcBtReasonCode) (0xFFF7))
#define TC_BT_L2CA_DISCONNECT_HARDWARE					((TcBtReasonCode) (0xFFF6))
#define TC_BT_L2CA_DISCONNECT_UNSUPPORTED_MODE			((TcBtReasonCode) (0xFFF5))
#define TC_BT_L2CA_DISCONNECT_PEER_REJECT_CID			((TcBtReasonCode) (0xFFF4))
#define TC_BT_L2CA_DISCONNECT_LINK_LOSS 				((TcBtReasonCode) (0xFFF3))
#define TC_BT_L2CA_DISCONNECT_OUT_OF_MEM				((TcBtReasonCode) (0xFFFA))
#define TC_BT_L2CA_DISCONNECT_SAR_VIOLATION 			((TcBtReasonCode) (0xFFF2))
#define TC_BT_L2CA_DISCONNECT_FEC_HEADER				((TcBtReasonCode) (0xFFF1))
#define TC_BT_L2CA_DISCONNECT_MTU_VIOLATION 			((TcBtReasonCode) (0xFFF0))
#define TC_BT_L2CA_DISCONNECT_MPS_VIOLATION 			((TcBtReasonCode) (0xFFEF))
#define TC_BT_L2CA_DISCONNECT_FEC_SEQUENCE				((TcBtReasonCode) (0xFFEE))
#define TC_BT_L2CA_DISCONNECT_TIMEOUT					((TcBtReasonCode) (0xFFFC))

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
	unsigned short int 				type;
	TcBtUint8					avResultFlag;
} TcBtA2dpEvtInd;

typedef struct
{
	unsigned short int 				type;
	TcBtResultCode				avResultCode;
	TcBtSupplier				avResultSupplier; 
} TcBtA2dpEvtCfm;

typedef TcBtA2dpEvtCfm TcBtA2dpDeactivateCfm;
typedef TcBtA2dpEvtInd TcBtA2dpDiscoverInd;
typedef TcBtA2dpEvtInd TcBtA2dpGetCapabilitiesInd;
typedef TcBtA2dpEvtInd TcBtA2dpSetConfigurationInd;
typedef TcBtA2dpEvtCfm TcBtA2dpOpenCfm;
typedef TcBtA2dpEvtInd TcBtA2dpOpenInd;
typedef TcBtA2dpEvtCfm TcBtA2dpStartCfm;
typedef TcBtA2dpEvtInd TcBtA2dpStartInd;
typedef TcBtA2dpEvtCfm TcBtA2dpCloseCfm;
typedef TcBtA2dpEvtInd TcBtA2dpCloseInd;
typedef TcBtA2dpEvtInd TcBtA2dpSuspendInd;
typedef TcBtA2dpEvtInd TcBtA2dpReconfigureInd;
typedef TcBtA2dpEvtInd TcBtA2dpAbortInd;

typedef struct
{
	unsigned short int 				type;
	TcBtUint8					avResultFlag;			
	TcBtUint8					avBypassFlag;
} TcBtA2dpStreamDataInd;

typedef struct
{
	unsigned short int 				type;
} TcBtA2dpAbortCfm;

typedef struct
{
	unsigned short int 				type;
	TcBtResultCode				avResultCode;
	TcBtSupplier				avResultSupplier;
} TcBtA2dpActivateCfm;

typedef struct
{
	unsigned short int 				type;
	TcBtUint8					avResultFlag;			
	TcBtBdAddr					deviceAddr;					/* address of device connected to */
} TcBtA2dpConnectInd;

typedef struct
{
	unsigned short int 				type;
	TcBtResultCode				avResultCode;
	TcBtSupplier				avResultSupplier;	
	TcBtBdAddr					deviceAddr;					/* address of device connected to */	
} TcBtA2dpConnectCfm;

typedef struct
{
	unsigned short int 				type;
	TcBtResultCode				avResultCode;
	TcBtSupplier				avResultSupplier;	
} TcBtA2dpSuspendCfm;

typedef struct
{
	unsigned short int 				type;
	TcBtUint8					avResultFlag;	
	TcBtUint8					localTerminated;
	TcBtReasonCode				reasonCode;
	TcBtSupplier				reasonSupplier;
} TcBtA2dpDisconnectInd;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_A2DP_ReceiveEvent(TcBtEvt *pTcBtEvt);
extern int TC_BT_A2DP_DeactivateCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_ActivateCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_StreamDataInd(void *pTcBtEvt);
extern int TC_BT_A2DP_ConnectInd(void *pTcBtEvt);
extern int TC_BT_A2DP_ConnectCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_DiscoverInd(void *pTcBtEvt);
extern int TC_BT_A2DP_GetCapabilitiesInd(void *pTcBtEvt);
extern int TC_BT_A2DP_SetConfigurationInd(void *pTcBtEvt);
extern int TC_BT_A2DP_OpenCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_OpenInd(void *pTcBtEvt);
extern int TC_BT_A2DP_StartCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_StartInd(void *pTcBtEvt);
extern int TC_BT_A2DP_CloseCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_CloseInd(void *pTcBtEvt);
extern int TC_BT_A2DP_SuspendCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_SuspendInd(void *pTcBtEvt);
extern int TC_BT_A2DP_ReconfigureInd(void *pTcBtEvt);
extern int TC_BT_A2DP_AbortCfm(void *pTcBtEvt);
extern int TC_BT_A2DP_AbortInd(void *pTcBtEvt);
extern int TC_BT_A2DP_DisconnectInd(void *pTcBtEvt);

extern void TC_BT_A2DP_SBC_SetBitpoolCallBack(TcBtUint8 *locMinBitpool, TcBtUint8 *locMaxBitpool);
extern void TC_BT_A2DP_SBC_GetBitpoolCallBack(TcBtUint8 curMinBitpool, TcBtUint8 curMaxBitpool);
#if 1	/* reserved */
extern int TC_BT_A2DP_AAC_InitCallBack(void);
extern int TC_BT_A2DP_AAC_DeinitCallBack(void);
extern int TC_BT_A2DP_AAC_DecodeCallBack(TcBtUint8 *pStreamData, TcBtUint32 streamDataLength, TcBtUint16 *pLeft, TcBtUint16 *pRight);
extern int TC_BT_A2DP_AAC_SetConfigCallBack(TcBtUint32 samplerate, TcBtUint32 channel, TcBtUint32 bitrate, TcBtBool supportVBR);
#endif
extern void TC_BT_A2DP_GetCodecParamCallBack(TcBtUint8 codecType, TcBtUint16 sampleRate, TcBtUint8 ch);
extern int TC_BT_A2DP_GetPCMDataCallBack(TcBtUint16 *pAudioData, TcBtUint32 audioSize);
#if 0	/* reserved */
extern void TC_BT_A2DP_DumpOpenCallBack(TcBtUint8 codecType);
extern void TC_BT_A2DP_DumpWriteCallBack(TcBtUint16 *pAudioDataL, TcBtUint16 *pAudioDataR, TcBtUint32 audioSize, TcBtUint32 allocSize);
extern void TC_BT_A2DP_DumpCloseCallBack(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_A2DP_EVT_H__ */
