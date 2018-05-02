/****************************************************************************
 *   FileName    : BTAPP_AV.c
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
*  Description : This file contains functions and variables for a Bluetooth A2DP application.
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    09/07/22     0.1            created                     Ji, Seung-Nam.
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*******************************************************************************/

#include "BT_CFG.h"

#ifdef BT_AUDIO_SINK
/*****************************************************************************
*
* Header File Include
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "BtSys.h"
#include "BtApp_Av.h"
#include "BtApp_HF.h"
#include "BtApp_Gap.h"
#include "BtApp_Av.h"
#include "TcBtEvt.h"
#include "TC_BT/inc/TC_BT_A2DP_CMD.h"
#include "TC_BT/inc/TC_BT_CMD.h"
#include "../TC_BT/inc/BtPlatform.h"
/* TODO: Add your specialized code here */
#ifndef A2DP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif
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

/*****************************************************************************
*
* static variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* global variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
unsigned char					gBtAvMetaValidity	= BTAPP_AV_CT_META_DISCONNECT;
unsigned char					gBtAvMetaFlag;
BTAPP_AV_CT_METADATA_TYPE		gstBtAvMetaData;
#endif

typedef struct
{
    unsigned short 	usSampleRate;	/* Sample Rate Frequency */
    unsigned char 	ucChannel;		/* Channel Number */

    unsigned char	Buffer[BTAPP_AV_PCM_BUF_SIZE];	/* Buffer for A2DP decoder output */
} BTAPP_AV_PCM;
unsigned char	ulReady;		/* Flag to check if audio is ready */

BT_SRBUFFER		stBuffer;						/* Buffer structure for PCM Data  */
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
unsigned char					gBtAvBrowValidity	= 0;
unsigned int					gBtAvBrowPlayerIdForPts	= 0xABCD0000; /* Shall be used only for PTS Test (browsing part) */
BTAPP_AV_CT_BROWSING_TYPE		gstBtAvBrowsing;
#endif

pthread_t						gBtAvSendThread;
unsigned char					gBTAvStopThread;
pthread_mutex_t					gBtAvPcmSync;
BTAPP_AV_PCM					gBtAvPcmInst;

snd_pcm_t						*gBtAvAlsaHandle;
#ifdef __arm__
static char 					*alsa_device = "plug:mainvol";	/* ALSA playback device */
#else
static char 					*alsa_device = "default";		/* ALSA playback device */
#endif
#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
 BTAPP_AV_STREAM_ANALYZER_TYPE	gstBtAvStreamAnalyzer;
#endif
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
* debug
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* entry points
*
*****************************************************************************/
static void *BtApp_AV_SNK_AudioSend(void *);
#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
static void BtApp_AV_SNK_StreamCheckTimer(unsigned char id, unsigned char loc);
#endif
#if defined(BT_AUDIO_SINK_AVRCP_PAS)
static void BtApp_AV_CT_PasValidityCheckTimer(unsigned char id, unsigned char loc);
#endif
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
static void BtApp_AV_CT_MetadataCheckTimer(unsigned char id, unsigned char loc);
#endif
#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
static void BtApp_AV_CT_BrowseSupportTimer(unsigned char id, unsigned char loc);
static void BtApp_AV_CT_GetMPListTimer(unsigned char id, unsigned char loc);
#endif
extern unsigned short					gBtAvRemoteVersion;
extern unsigned char					gBtAvPasValidList[BTAPP_AV_CT_PAS_ATT_END];
extern unsigned char					gBtAvPasCurrValue[BTAPP_AV_CT_PAS_ATT_END];
extern unsigned char					gBtAvPasEventAtt;
extern unsigned char					gBtAvPasEventVal;
extern unsigned char					gBtAvPasEventText[BTAPP_AV_CT_PAS_TEXT_SIZE];
/*************************************************************************************
*  XXX_API_Initialize
*-------------------------------------------------------------------------------------
* Description  :
* Parameters   :
* Return	   :
**************************************************************************************/
/* TODO: Add your specialized code here */
#if 0
TcBtUint16 BtApp_AV_SNK_CloseStream(void)
{
	TcBtUint16 timerCnt = 0;

	/* suspend stream */
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_STREAM) == TRUE)
	{
		(void)TC_BT_A2DP_SuspendStream();
		timerCnt += 10;
	}
	/* stop stream */
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_OPEN) == TRUE)
	{
		if(timerCnt == 0)
		{
			(void)TC_BT_A2DP_StopStream();
		}
		else
		{
			TCCAPI_Timer_SetTime(timerCnt, TIMER_BT_AV_STREAM_STOP_ID, BtApp_GAP_A2DP_StopStreamForTimer);
		}
	}

	return timerCnt;
}
#endif
void BtApp_AV_SNK_AudioStart(void)
{
	unsigned char lBasicMode;

    lBasicMode = (unsigned char)G_BtSys->BtApp_SYS_GetBasicMode();

	switch(lBasicMode)
	{
		case BIDLE:
			break;		
		case B_BT_A2DP_SINK:
			return;	/* duplicate */
#ifdef BT_HANDSFREE_UNIT
	#ifdef BT_HFP_RING_FOR_NO_IN_BAND
		case B_BT_HFP_RING:
			return; /* for SGH-i450, only +BSIR selects SCO/A2DP ring tone */
	#endif
		case B_BT_HFP_CALL:
		case B_BT_HSP_AUDIO:
		{
			unsigned long inbandRing;
			char status;
			inbandRing = BtApp_HF_InbandRing();
            status = G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING));
			if((inbandRing != 0) && (status != FALSE))
			{
			 	/* for incoming call case of Motorola KRZR K1 */
				BtApp_HF_Close_Call();
			}
		}
			break;
#endif
	}
    BtApp_HF_Close_Call();//modify for  close HF audio .
    G_BtSys->BtApp_SYS_SetBasicMode(B_BT_A2DP_SINK);
    BtApp_AV_SNK_AlsaInit();
	if(BtApp_AV_SNK_AlsaOpen() != 0)
	{
		/* Reserved */
	}

    TC_Q_InitBuffer(&stBuffer, BTAPP_AV_PCM_BUF_SIZE, (char *)gBtAvPcmInst.Buffer);
	{	/* Create audio transfer thread */
	    pthread_attr_t thread_attr;

	    pthread_mutex_init(&gBtAvPcmSync, NULL);	/* Create mutex for PCM Data buffer    */	    
	    pthread_attr_init(&thread_attr);			/* Create thread for PCM Data transfer */
		gBTAvStopThread = FALSE;
        if(pthread_create(&gBtAvSendThread, &thread_attr, BtApp_AV_SNK_AudioSend, NULL) < 0 )
	    {
	    }
    }        
printf("come  to here %s   %d\n",__FUNCTION__,__LINE__);
    ulReady = TRUE;
}

void *BtApp_AV_SNK_AudioSend(void *)
{
	unsigned int  remain_size = 0;
	unsigned char send_buffer[BTAPP_AV_PCM_BUF_SEND];
	static   int  send_flag = 0;

	while(gBTAvStopThread != TRUE)
    {

		pthread_mutex_lock(&gBtAvPcmSync);
        remain_size = TC_Q_GetDataSize(&stBuffer);
		if(remain_size > BTAPP_AV_PCM_BUF_SIZE/2)
        {
            TC_Q_GetData(&stBuffer, (char *)send_buffer, BTAPP_AV_PCM_BUF_SEND);
			pthread_mutex_unlock(&gBtAvPcmSync);
			BtApp_AV_SNK_AlsaWrite((unsigned char *)send_buffer, BTAPP_AV_PCM_BUF_SEND);

			if(send_flag == 0)
			{
				send_flag = 1;
			}	
		}
		else if(remain_size < BTAPP_AV_PCM_BUF_SEND)
		{
			pthread_mutex_unlock(&gBtAvPcmSync);
			send_flag = 0;
		}
		else
		{
			if(send_flag)
			{	/* Decreasing Status */
                TC_Q_GetData(&stBuffer, (char *)send_buffer, BTAPP_AV_PCM_BUF_SEND);
				pthread_mutex_unlock(&gBtAvPcmSync);
				BtApp_AV_SNK_AlsaWrite((unsigned char *)send_buffer, BTAPP_AV_PCM_BUF_SEND);				
			}
			else
			{	/* Increasing Status */
				pthread_mutex_unlock(&gBtAvPcmSync);
			}
		}

		/*usleep(1000*5);*/	/* Wait for 5msec for 1024byte */
		usleep(1000*1);		/* Exception Case : TCC8925    */						
	}

	BtApp_AV_SNK_AlsaClose();	/* AlsaClose & Write must be located at same task */
    //return ;
}

void BtApp_AV_SNK_AudioEnd(void)
{
	int dummy;

    ulReady = FALSE;
	gBTAvStopThread      = TRUE;					/* Stop thread for PCM Data transfer */
	pthread_join(gBtAvSendThread, (void **)&dummy);	/* Wait for the thread-stop          */
	if(pthread_mutex_destroy(&gBtAvPcmSync) != 0);	/* Destory mutex for PCM Data buffer */
    {
    }

    G_BtSys->BtApp_SYS_SetBasicMode(BIDLE);
}

void BtApp_AV_SNK_Activate(void)
{ printf("============ %s ============ \n",__FUNCTION__);
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_ACTIVATE) == TRUE)
	{
#if 0
		DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_ACTIVATE, BTAPP_AV_EVT_ALREADY, 0, 0, 0);
#else
	{
		TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

		*pTcBtAVEvt = BTAPP_AV_EVT_ALREADY;
        TcBtEvt_AvEvt(BT_AV_EVT_SNK_ACTIVATE, pTcBtAVEvt);

	}
#endif
	}
	else
	{
        (void)TC_BT_AVRCP_Activate((unsigned char *)"AVRCP CT", (unsigned char *)"TeleChips");
		(void)TC_BT_A2DP_Activate();
	}
}

void BtApp_AV_SNK_Deactivate(void)
{
	TcBtUint16 state;
#if 0 //ndef USE_BTBR
	state = BtApp_GAP_GetAvrcpState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTING)
	{
		TcBtBdAddr bdAddr;
		(void)BtApp_GAP_GetAvrcpBdAddr(&bdAddr);
		if(TC_BT_AVRCP_CancelConnect(&bdAddr) == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			(void)BtApp_GAP_SetAvrcpState(&bdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
            usleep(500000);
		}
	}
	else if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		if(TC_BT_AVRCP_Disconnect() == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			TcBtBdAddr bdAddr;
			(void)BtApp_GAP_GetAvrcpBdAddr(&bdAddr);
			(void)BtApp_GAP_SetAvrcpState(&bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
            usleep(500000);
		}
	}
	else
	{
		/* do nothing */
	}
#endif
#if 0
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_STREAM) == TRUE)
	{
		(void)TC_BT_A2DP_SuspendStream();
        usleep(500000);
	}
	if(BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_OPEN) == TRUE)
	{
		(void)TC_BT_A2DP_StopStream();
        usleep(500000);
	}
#endif
	state = BtApp_GAP_GetA2dpState(NULL);
	if(state == BTAPP_GAP_STATE_CONNECTING)
	{
		TcBtBdAddr bdAddr;
		(void)BtApp_GAP_GetA2dpBdAddr(&bdAddr);
		if(TC_BT_A2DP_CancelConnect(&bdAddr) == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			(void)BtApp_GAP_SetA2dpState(&bdAddr, BTAPP_GAP_STATE_CONNECT_CANCELLING);
            usleep(500000);
		}
	}
	else if(state == BTAPP_GAP_STATE_CONNECTED)
	{
		if(TC_BT_A2DP_Disconnect() == TC_BT_CMD_RESULT_SUCCESS_ENUM)
		{
			TcBtBdAddr bdAddr;
			(void)BtApp_GAP_GetA2dpBdAddr(&bdAddr);
			(void)BtApp_GAP_SetA2dpState(&bdAddr, BTAPP_GAP_STATE_DISCONNECTING);
            usleep(500000);
		}
	}
	else
	{
		/* do nothing */
	}
		
    if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_ACTIVATE) == TRUE)
	{
		(void)TC_BT_A2DP_Deactivate();
		(void)TC_BT_AVRCP_Deactivate();
	}
	else
	{
#if 0
		DMP_EvtBtFourDataMessageSend(MSG_SRC_BT_AV, BT_AV_EVT_SNK_DEACTIVATE, BTAPP_AV_EVT_ALREADY, 0, 0, 0);
#else
		{
			TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

			*pTcBtAVEvt = BTAPP_AV_EVT_ALREADY;
            TcBtEvt_AvEvt(BT_AV_EVT_SNK_DEACTIVATE, pTcBtAVEvt);

        }
#endif
	}
}

#ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER

#endif /* BT_AUDIO_SINK_USE_STREAM_ANALYZER */

#if defined(BT_AUDIO_SINK_AVRCP_PAS)
void BtApp_AV_CT_PasValidityCheck(void)
{
	unsigned char attribute;

	for(attribute = 0; attribute < BTAPP_AV_CT_PAS_ATT_END; attribute++)
	{	/* Request the support values of the valid attribute */
		if(gBtAvPasValidList[attribute] == BTAPP_AV_CT_PAS_VALID_MARK)
		{
			gBtAvPasValidList[attribute] = 0;
			(void)TC_BT_AVRCP_CtPasValId(attribute);
			break;
		}
	}

	if(attribute == BTAPP_AV_CT_PAS_ATT_END)
	{	/* Notice the valid attribute/value list */
#if 0
		DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_SUPPORT);
#else
		{
			TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasValidList));
			
			memcpy(pTcBtAVEvt, gBtAvPasValidList, sizeof(gBtAvPasValidList)); 		
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_SUPPORT, pTcBtAVEvt);

        }
#endif
	}
}

static void BtApp_AV_CT_PasValidityCheckTimer(unsigned char id, unsigned char loc)
{
	static int start = FALSE;

	if(start == TRUE)
	{
		(void)TC_BT_AVRCP_CtPasAttId();
		start = FALSE;
	}
	else
	{
        if((G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_A2DP_SINK) && (gTcBtA2dpStreamState == 1))
		{	/* PAS validity check after a2dp play */
			start = TRUE;
			TCCAPI_Timer_SetTime(20, TIMER_BT_AV_PAS_CHECK_ID, BtApp_AV_CT_PasValidityCheckTimer);
		}
		else
		{
			TCCAPI_Timer_SetTime(5, TIMER_BT_AV_PAS_CHECK_ID, BtApp_AV_CT_PasValidityCheckTimer);
		}
	}
}

void BtApp_AV_CT_PasValidityCheckStart(unsigned char enable)
{
	if(enable == TRUE)
	{
		TCCAPI_Timer_SetTime(5, TIMER_BT_AV_PAS_CHECK_ID, BtApp_AV_CT_PasValidityCheckTimer);
	}
	else
	{
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_PAS_CHECK_ID);
	}
}

void BtApp_AV_CT_PasGetSeetings(void)
{
	TcBtAvrcpPasAttId attrib_list[BTAPP_AV_CT_PAS_ATT_END];
	unsigned char attrib_count, count;
	
	memset(attrib_list, 0x00, sizeof(attrib_list));
	attrib_count = 0;
	for(count = 0; count < BTAPP_AV_CT_PAS_ATT_END; count++)
	{
		if(gBtAvPasValidList[count] > 0)	/* Request the valid attributes */
		{
			attrib_list[attrib_count] = count;
			attrib_count++;
		}
	}
	
	if(attrib_count > 0)
	{
		(void)TC_BT_AVRCP_CtPasCurrent(attrib_count, attrib_list);
	}	
	else
	{
		memset(gBtAvPasCurrValue, 0, sizeof(gBtAvPasCurrValue));
#if 0		
		DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_PAS_GET_SETTINGS);
#else
		{
			TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)TC_BT_Pmalloc(sizeof(gBtAvPasCurrValue));
			
			memcpy(pTcBtAVEvt, gBtAvPasCurrValue, sizeof(gBtAvPasCurrValue)); 		
            TcBtEvt_AvEvt(BT_AV_EVT_CT_PAS_GET_SETTINGS, pTcBtAVEvt);

		}
#endif
	}
}

void BtApp_AV_CT_PasSetSeetings(unsigned char *setting_list)
{
	TcBtAvrcpPasAttValPair pair_list[BTAPP_AV_CT_PAS_ATT_END];
	unsigned char pair_count, count;
	
	memset(pair_list, 0x00, sizeof(pair_list));
	pair_count = 0;
	for(count = 0; count < BTAPP_AV_CT_PAS_ATT_END; count++)
	{
		if((gBtAvPasValidList[count]) && (setting_list[count])) 				/* Request the valid attributes */
		{
			if((gBtAvPasValidList[count] & (0x1 << (setting_list[count]-1))) != 0)	/* Request the valid values */
			{
				pair_list[pair_count].attribId = count;
				pair_list[pair_count].valueId  = setting_list[count];
				pair_count++;
			}
		}
	}
	
	if(pair_count > 0)
	{
		(void)TC_BT_AVRCP_CtPasSet(pair_count, pair_list);
	}
}

#endif /* BT_AUDIO_SINK_AVRCP_PAS */

#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
void BtApp_AV_CT_MetadataInit(void)
{
	memset(&gstBtAvMetaData, 0, sizeof(BTAPP_AV_CT_METADATA_TYPE));
	gstBtAvMetaData.NameOfTitle[0] = 0xff;
	gstBtAvMetaData.NameOfArtist[0] = 0xff;
	gstBtAvMetaData.NameOfAlbum[0] = 0xff;
	gstBtAvMetaData.NameOfGenre[0] = 0xff;
	gstBtAvMetaData.NumberOfTrack = -1;
	gstBtAvMetaData.NumberOfTotal = -1;
	gstBtAvMetaData.NumberOfTime = -1;
}

static void BtApp_AV_CT_MetadataCheckTimer(unsigned char id, unsigned char loc)
{
	static int wait, retry, state, skip;

    if(G_BtSys->BtApp_SYS_GetBasicMode() == B_BT_A2DP_SINK)
	{
		if(gBtAvMetaValidity == BTAPP_AV_CT_META_CONNECT)
		{
			if(gBtAvRemoteVersion < TC_BT_AVRCP_CONFIG_SR_VERSION_13)
			{	/* Exception Case : Unsupported Version */
				gBtAvMetaValidity = BTAPP_AV_CT_META_INVALID;
				
			#if 0
				DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_META_SUPPORT);
			#else
				TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
		
				*pTcBtAVEvt = FALSE;		
                TcBtEvt_AvEvt(BT_AV_EVT_CT_META_SUPPORT, pTcBtAVEvt);

			#endif
				
             //   printf("[%s : %05d] Info : BtApp_AV_CT_MetadataCheckTimer : Unsupported Version \n", __FILE__, __LINE__);
			}
			else
			{	/* 1st Metadata Request */
				gBtAvMetaValidity = BTAPP_AV_CT_META_CHECK;
				gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
				(void)TC_BT_AVRCP_CtGetAttributes(TC_BT_AVRCP_SCOPE_NPL, gTcBtAvrcpUidCurrent, gTcBtAvrcpUidCounter, TC_BT_AVRCP_ITEM_ATT_MASK_ALL);
				state = BTAPP_AV_CT_ATTRIBUTE_UPDATE; wait = 0; retry = 0; skip = 0;
				
              //  printf("[%s : %05d] Info : BtApp_AV_CT_MetadataCheckTimer : 1st Request \n", __FILE__, __LINE__);
			}
		}
		else if(gBtAvMetaValidity == BTAPP_AV_CT_META_CHECK)
		{
			if(gBtAvMetaFlag == TRUE)
			{	/* Check the response by metadata request */
				switch(state)
				{
					case BTAPP_AV_CT_ATTRIBUTE_UPDATE:
						{	/* 2nd Metadata Request */
							gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
							(void)TC_BT_AVRCP_CtGetPlayStatus();
							state = BTAPP_AV_CT_PLAY_STATUS_UPDATE; wait = 0; retry = 0;
							
                     //       printf("[%s : %05d] Info : BtApp_AV_CT_MetadataCheckTimer : 2nd Request \n", __FILE__, __LINE__);
						}
						break;
					case BTAPP_AV_CT_PLAY_STATUS_UPDATE:
						/* Enter BTAPP_AV_CT_META_VALID State */
						state = BTAPP_AV_CT_COMMAND_END;
						break;
					default:
						gBtAvMetaValidity = BTAPP_AV_CT_META_VALID;
					#if 0
						DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_META_SUPPORT);
					#else
						TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
				
						*pTcBtAVEvt = TRUE;
                        TcBtEvt_AvEvt(BT_AV_EVT_CT_META_SUPPORT, pTcBtAVEvt);

					#endif						
						break;
				}
			}
			else
			{	/* Wait for AVRCP_METADATA_VENDOR_FRAME_CFM */
				wait++;	wait %= BTAPP_AV_CT_METADATA_WAIT;
				if(wait == 0)
				{
                //    printf("[%s : %05d] Info : BtApp_AV_CT_MetadataCheckTimer : No Response %d \n", __FILE__, __LINE__,  state);
					switch(state)
					{
						case BTAPP_AV_CT_ATTRIBUTE_UPDATE:
							if(retry >= BTAPP_AV_CT_METADATA_RETRY)
							{	/* Exception Case : Nokia 5800 */
								gBtAvMetaValidity = BTAPP_AV_CT_META_INVALID;
							#if 0	
								DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_META_SUPPORT);
							#else
								TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
						
								*pTcBtAVEvt = FALSE;
                                TcBtEvt_AvEvt(BT_AV_EVT_CT_META_SUPPORT, pTcBtAVEvt);
                            #endif
							}
							else
							{	/* Retried Metadata Request */
								gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
								(void)TC_BT_AVRCP_CtGetAttributes(TC_BT_AVRCP_SCOPE_NPL, gTcBtAvrcpUidCurrent, gTcBtAvrcpUidCounter, TC_BT_AVRCP_ITEM_ATT_MASK_ALL);
								retry++;
							}
							break;
						case BTAPP_AV_CT_PLAY_STATUS_UPDATE:
							if(retry >= BTAPP_AV_CT_METADATA_RETRY)
							{	/* Exception Case : HTC HD2 */
								skip |= (1 << BTAPP_AV_CT_PLAY_STATUS_UPDATE);
								state = BTAPP_AV_CT_COMMAND_END;
								gstBtAvMetaData.SongPosition = 0xFFFFFFFF;
							}
							else
							{	/* Retried Metadata Request */
								gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
								(void)TC_BT_AVRCP_CtGetAttributes(TC_BT_AVRCP_SCOPE_NPL, gTcBtAvrcpUidCurrent, gTcBtAvrcpUidCounter, TC_BT_AVRCP_ITEM_ATT_MASK_ALL);
								retry++;
							}
							break;
						default:
							gBtAvMetaValidity = BTAPP_AV_CT_META_VALID;
						#if 0	
							DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_META_SUPPORT);
						#else
							TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));
					
							*pTcBtAVEvt = TRUE;
                            TcBtEvt_AvEvt(BT_AV_EVT_CT_META_SUPPORT, pTcBtAVEvt);

						#endif
							break;
					}
				}
			}
		}
		else if(gBtAvMetaValidity == BTAPP_AV_CT_META_VALID)
		{
			if(gBtAvMetaFlag == TRUE)
			{	/* Check the response by metadata request */
				if(state == BTAPP_AV_CT_ATTRIBUTE_UPDATE)
				{
					if((skip & (1 << BTAPP_AV_CT_PLAY_STATUS_UPDATE)) == 0)	
					{	/* Don't request if skip bit is set */
						gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
						(void)TC_BT_AVRCP_CtGetPlayStatus();
					}
					state = BTAPP_AV_CT_PLAY_STATUS_UPDATE; wait = 0;
				}
				else
				{
					if((skip & (1 << BTAPP_AV_CT_ATTRIBUTE_UPDATE)) == 0)
					{	/* Don't request if skip bit is set */
						gBtAvMetaFlag = FALSE;	/* Initialize the flag to check the response */
						(void)TC_BT_AVRCP_CtGetAttributes(TC_BT_AVRCP_SCOPE_NPL, gTcBtAvrcpUidCurrent, gTcBtAvrcpUidCounter, TC_BT_AVRCP_ITEM_ATT_MASK_ALL);
					}
					state = BTAPP_AV_CT_ATTRIBUTE_UPDATE; wait = 0;
				}
			}
			else
			{	/* Wait for AVRCP_METADATA_VENDOR_FRAME_CFM */
				wait++;	wait %= BTAPP_AV_CT_METADATA_WAIT;
				if(wait == 0)
				{
					gBtAvMetaFlag = TRUE;
                   // printf("[%s : %05d] Info : BtApp_AV_CT_MetadataCheckTimer : No Response %d \n", __FILE__, __LINE__,  state);
				}
			}
		}
		else
		{
			/* BTAPP_AV_CT_META_INVALID */ /* BTAPP_AV_CT_META_DISCONNECT */
		}
	}

	TCCAPI_Timer_SetTime(5, TIMER_BT_AV_META_CHECK_ID, BtApp_AV_CT_MetadataCheckTimer);		/* Metadata Request Period : About 500 msec */
}

void BtApp_AV_CT_MetadataCheckStart(unsigned char enable)
{
	if(enable == TRUE)
	{
		gBtAvMetaValidity  = BTAPP_AV_CT_META_CONNECT;
		BtApp_AV_CT_MetadataInit();
		TCCAPI_Timer_SetTime(5, TIMER_BT_AV_META_CHECK_ID, BtApp_AV_CT_MetadataCheckTimer);	/* Metadata Request Period : About 500 msec */
	}
	else
	{
		gBtAvMetaValidity  = BTAPP_AV_CT_META_DISCONNECT;
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_META_CHECK_ID);
	}
}
#endif /* BT_AUDIO_SINK_AVRCP_METADATA */

#if defined(BT_AUDIO_SINK_AVRCP_BROWSING)
static void BtApp_AV_CT_BrowseSupportTimer(unsigned char id, unsigned char loc)
{
#if 0
	DMP_MessageEvtUpdate(DISK_DEVICE_BT_AV, BT_AV_EVT_CT_BROWSE_SUPPORT);
#else
	{
		TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)TC_BT_Pmalloc(sizeof(TcBtUint32));

		*pTcBtAVEvt = gBtAvBrowValidity;
        TcBtEvt_AvEvt(BT_AV_EVT_CT_BROWSE_SUPPORT, pTcBtAVEvt);

	}
#endif
}

void BtApp_AV_CT_BrowseSupportEnable(unsigned char enable)
{
	if(enable == TRUE)
	{
		gBtAvBrowValidity = 1;
		/* Exception Case : PTS 5.1.0 (PTS can accept browsing command after 1~2sec) */
		/* Exception Case : iPhone 5  (iPhone didn't respond right after connection) */
		TCCAPI_Timer_SetTime(20, TIMER_BT_AV_BROWSE_SUPPORT_ID, BtApp_AV_CT_BrowseSupportTimer);
	}
	else
	{
		gBtAvBrowValidity = 0;
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_BROWSE_SUPPORT_ID);
	}
}

static void BtApp_AV_CT_GetMPListTimer(unsigned char id, unsigned char loc)
{
	(void)TC_BT_AVRCP_CtGetFolderItems(BTAPP_AV_CT_BROWSING_MP_LIST, 0, 100, TC_BT_AVRCP_ITEM_ATT_MASK_TITLE);
}

void BtApp_AV_CT_GetMPListEnable(unsigned char enable)
{
	if(enable == TRUE)
	{
		/* Exception Case : PTS 5.1.0 (PTS can accept browsing command after 1~2sec) */
		/* Exception Case : iPhone 5  (iPhone didn't respond right after connection) */
		TCCAPI_Timer_SetTime(20, TIMER_BT_AV_GET_MPLIST_ID, BtApp_AV_CT_GetMPListTimer);
	}
	else
	{
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_GET_MPLIST_ID);
	}
}

void BtApp_AV_CT_SetBrowseScope(unsigned int scope)
{
	if(gBtAvBrowValidity == 1)
	{
		if(gstBtAvBrowsing.ItemsScope != (unsigned char)scope)
		{
			gstBtAvBrowsing.ItemsScope = (unsigned char)scope;
		}
	}
}

void BtApp_AV_CT_GetFolderItems(unsigned int index, unsigned int items)
{
	(void)TC_BT_AVRCP_CtGetFolderItems(gstBtAvBrowsing.ItemsScope, index, ((index+items)-1), TC_BT_AVRCP_ITEM_ATT_MASK_TITLE);
}

void BtApp_AV_CT_ChangePath(int index)
{
	if(index == -1)		/* change path up one level */
	{
		unsigned char UID[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		(void)TC_BT_AVRCP_CtChangePath(gTcBtAvrcpUidCounter, TC_BT_AVRCP_CHANGE_PATH_UP, UID);
	}
	else if(index > -1)	/* change path down */
	{
		unsigned int count;
		for(count = 0; count < gstBtAvBrowsing.ItemsCount; count++)
		{
			if((gstBtAvBrowsing.Item[count].Index == index) && (gstBtAvBrowsing.Item[count].Type == 0/*BT_AUDIO_ITEM_TYPE_FOLDER*/))
			{
				(void)TC_BT_AVRCP_CtChangePath(gTcBtAvrcpUidCounter, TC_BT_AVRCP_CHANGE_PATH_DOWN, gstBtAvBrowsing.Item[count].UID);		
				break;
			}
		}

		if(count == gstBtAvBrowsing.ItemsCount)
		{
			printf("[%s : %05d] Info : BtApp_AV_CT_ChangePath : Can't find anything to be matched.\n", __FILE__, __LINE__);
		}
	}
	else
	{
		printf("[%s : %05d] Info : BtApp_AV_CT_ChangePath : Wrong index number.\n", __FILE__, __LINE__);
	}
}
#endif /* BT_AUDIO_SINK_AVRCP_BROWSING */

#if defined(BT_AUDIO_SINK_USE_STREAM_ANALYZER)
static void StreamAnalyzer(unsigned char id, unsigned char loc);
static void PlayProcedure(int phase);
static void PauseProcedure(int phase);
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
static void PlayByPlayStatus(unsigned char id, unsigned char loc);
static void PauseByPlayStatus(unsigned char id, unsigned char loc);
static void GetPlayStatusTimer(unsigned char id, unsigned char loc);
#endif
static void PlayRetryTimer(unsigned char id, unsigned char loc);
static void ReportTheResult(void);
static void UpdateReferenceState(void);

void BtApp_AV_SNK_EnterBTAMode(void)
{
	if(gstBtAvStreamAnalyzer.isHFRelatedMode == TRUE)
	{ /* from HF related mode */
		gstBtAvStreamAnalyzer.isHFRelatedMode = FALSE;
	}
	else
	{ /* from Non-HF related mode */
	#if 1 /* Enable: playback always, Disable: restore prev status */
	#ifndef CODESONAR_INCLUDE /* Redundant Condition & Unreachable */
		gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PLAY;
	#endif
	#endif
	#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
		if(gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable == TRUE)
		{	/* remote device provide play status */
			if(gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PLAY)
			{
				TCCAPI_Timer_SetTime(10, TIMER_BT_AVRCP_CONTROL_BY_PLAY_STATUS_ID, PlayByPlayStatus);
			}
			else if(gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PAUSE)
			{
				TCCAPI_Timer_SetTime(10, TIMER_BT_AVRCP_CONTROL_BY_PLAY_STATUS_ID, PauseByPlayStatus);
			}
			else
			{
				/* do nothing */
			}
		}
		else
	#endif
		{	/* remote device does NOT provide play status */
			if(gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PLAY)
			{
				PlayProcedure(BTAPP_AV_1st_PHASE);
			}
			else if(gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PAUSE)
			{
				PauseProcedure(BTAPP_AV_1st_PHASE);
			}
			else
			{
				/* do nothing */
			}
		}
	}
}

void BtApp_AV_SNK_ExitBTAMode(void)
{
	if(gstBtAvStreamAnalyzer.analyzeFlag == TRUE)
	{
		gstBtAvStreamAnalyzer.analyzeFlag = FALSE;
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_STREAM_ANALYZE_ID);
		TCCAPI_Timer_CancelTime(TIMER_BT_AVRCP_CONTROL_BY_PLAY_STATUS_ID);
		TCCAPI_Timer_CancelTime(TIMER_BT_AVRCP_CONTROL_RETRY_ID);
	}

	if(gstBtAvStreamAnalyzer.skipUpdateRefStatus == FALSE)
	{
		UpdateReferenceState();
	}
	
	/* 1. The gIsHFRelatedMode is set only TRUE from HF related mode(Call, Voice Dial, etc) and other mode can't set this value
	   2. Set gIsHFRelatedMode to FALSE at the end of BtApp_AV_SNK_EnterBTAMode() */
	if(gstBtAvStreamAnalyzer.isHFRelatedMode == TRUE)
	{ /* to HF related mode */
		/* nothing to do */
	}
	else
	{ /* to Non-HF related mode */
#if defined(BT_AUDIO_SINK_AVRCP_METADATA) 
		if(gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable == TRUE)
		{	/* remote device provide play status */
			if((gstBtAvMetaData.PlayStatus == BTAPP_AV_STOP) || (gstBtAvMetaData.PlayStatus == BTAPP_AV_PAUSE))
			{
				/* nothing to do */
			}
			else if(gstBtAvMetaData.PlayStatus == BTAPP_AV_PLAY)
			{
				(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PAUSE, TC_BT_AVRCP_PT_STATE_PRESS);
				/* The phone needed some time to update its play status */
				TCCAPI_Timer_SetTime(10, TIMER_BT_AVRCP_CONTROL_BY_PLAY_STATUS_ID, GetPlayStatusTimer);
			}
			else
			{
				/* never reach */
			}
		}
		else
#endif
		{	/* remote device does NOT provide play status */
			if((gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_STOP) || (gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PAUSE))
			{
				/* nothing to do */
			}
			else if(gstBtAvStreamAnalyzer.referenceStatus == BTAPP_AV_PLAY)
			{
				(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PAUSE, TC_BT_AVRCP_PT_STATE_PRESS);
			}
			else
			{
				/* never reach */
			}
		}
	}
}

void BtApp_AV_SNK_StreamCheckStart(unsigned char enable)
{	/* Automatic Play/Pause Function : Stream & Mode Check Start/Stop */
	if(enable == TRUE)
	{
		gTcBtA2dpStreamState = 0;			/* Initialize state for stream check */
		TCCAPI_Timer_SetTime(5, TIMER_BT_AV_STREAM_CHECK_ID, BtApp_AV_SNK_StreamCheckTimer);
	}
	else
	{
		TCCAPI_Timer_CancelTime(TIMER_BT_AV_STREAM_CHECK_ID);
	}
}

static void BtApp_AV_SNK_StreamCheckTimer(unsigned char id, unsigned char loc)
{	/* Automatic Play/Pause Function : Stream & Mode Check Process */
	static unsigned int state = TRUE;
	static unsigned int counter = 0;

	if(state == TRUE)
	{
		gTcBtA2dpStreamState = 0;			/* Initialize state for stream check */
		state = FALSE;
	}
	else
	{
		if(gTcBtA2dpStreamState == 1)
		{	/* If it's streaming, check the mode */
            if(G_BtSys->BtApp_SYS_GetBasicMode() != B_BT_A2DP_SINK)
			{
				counter++;
				if(counter > 14) /* for 15 seconds */
				{
					counter = 0;
					(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PAUSE, TC_BT_AVRCP_PT_STATE_PRESS);
				}
			}
		}
		else
		{
			counter = 0;
		}
		state = TRUE;
	}

	TCCAPI_Timer_SetTime(5, TIMER_BT_AV_STREAM_CHECK_ID, BtApp_AV_SNK_StreamCheckTimer);
}

static void StreamAnalyzer(unsigned char id, unsigned char loc)
{
	if(gstBtAvStreamAnalyzer.analyzeFlag == FALSE)
	{
		return;
	}
	
	if(gstBtAvStreamAnalyzer.counterIndex == (BTAPP_AV_ANALYZE_TIME - 1))
	{
		if((gstBtAvStreamAnalyzer.streamCounters[gstBtAvStreamAnalyzer.counterIndex-1]>>1) < gstBtAvStreamAnalyzer.streamCounters[gstBtAvStreamAnalyzer.counterIndex])
		{
			gstBtAvStreamAnalyzer.analysisResult = BTAPP_AV_MUSIC;
		}
		else
		{
			gstBtAvStreamAnalyzer.analysisResult = BTAPP_AV_NOT_MUSIC;
		}
		ReportTheResult();
	}
	else if(gstBtAvStreamAnalyzer.counterIndex == 0)
	{
		gstBtAvStreamAnalyzer.counterIndex++;
		TCCAPI_Timer_SetTime(10, TIMER_BT_AV_STREAM_ANALYZE_ID, StreamAnalyzer);
	}
	else if(gstBtAvStreamAnalyzer.counterIndex == 1)
	{
		if((gstBtAvStreamAnalyzer.streamCounters[0]>>1) < gstBtAvStreamAnalyzer.streamCounters[1])
		{ /* keep on */
			if(gstBtAvStreamAnalyzer.streamCounters[0] > (gstBtAvStreamAnalyzer.streamCounters[1]>>1))
			{
				gstBtAvStreamAnalyzer.counterIndex++;
			}
			else
			{ /* reset */
				int loopIndex;
				
				gstBtAvStreamAnalyzer.streamCounters[0] = gstBtAvStreamAnalyzer.streamCounters[1];
				for(loopIndex = 1; loopIndex < BTAPP_AV_ANALYZE_TIME; loopIndex++)
				{
					gstBtAvStreamAnalyzer.streamCounters[loopIndex] = 0;
				}
			}
			TCCAPI_Timer_SetTime(10, TIMER_BT_AV_STREAM_ANALYZE_ID, StreamAnalyzer);
		}
		else
		{ /* report to the proper procedure */
			gstBtAvStreamAnalyzer.analysisResult = BTAPP_AV_NOT_MUSIC;
			ReportTheResult();
		}
	}
	else if(gstBtAvStreamAnalyzer.counterIndex > 1)
	{
		if((gstBtAvStreamAnalyzer.streamCounters[gstBtAvStreamAnalyzer.counterIndex-1]>>1) < gstBtAvStreamAnalyzer.streamCounters[gstBtAvStreamAnalyzer.counterIndex])
		{ /* keep on */
			gstBtAvStreamAnalyzer.counterIndex++;
			TCCAPI_Timer_SetTime(10, TIMER_BT_AV_STREAM_ANALYZE_ID, StreamAnalyzer);
		}
		else
		{ /* report to the proper procedure */
			gstBtAvStreamAnalyzer.analysisResult = BTAPP_AV_NOT_MUSIC;
			ReportTheResult();
		}
	}
	else
	{
		/* never reach */
	}
}

static void PlayProcedure(int phase)
{
	if(phase == BTAPP_AV_1st_PHASE)
	{ /* First Phase : Initialize and Start */
		memset(&gstBtAvStreamAnalyzer.streamCounters, 0x00, sizeof(gstBtAvStreamAnalyzer.streamCounters));
		gstBtAvStreamAnalyzer.counterIndex = 0;
		gstBtAvStreamAnalyzer.analyzeFlag = TRUE;
		gstBtAvStreamAnalyzer.objectiveStatus = BTAPP_AV_PLAY;
		TCCAPI_Timer_SetTime(10, TIMER_BT_AV_STREAM_ANALYZE_ID, StreamAnalyzer);
	}
	else if(phase == BTAPP_AV_2nd_PHASE)
	{ /* Second Phase : Action depending on analysis result */
		if(gstBtAvStreamAnalyzer.analysisResult == BTAPP_AV_MUSIC)
		{
			/* nothing to do */
		}
		else if(gstBtAvStreamAnalyzer.analysisResult == BTAPP_AV_NOT_MUSIC)
		{
			if(TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PLAY, TC_BT_AVRCP_PT_STATE_PRESS) == TC_BT_CMD_RESULT_DISCONNECTED_ENUM)
			{
				gstBtAvStreamAnalyzer.playRetryCnt = 5;
				TCCAPI_Timer_SetTime(10, TIMER_BT_AVRCP_CONTROL_RETRY_ID, PlayRetryTimer);
			}
		}
		else
		{
			/* never reach */
		}
		gstBtAvStreamAnalyzer.analyzeFlag = FALSE;
	}
	else
	{
		/* never reach */
	}
}

static void PauseProcedure(int phase)
{
	if(phase == BTAPP_AV_1st_PHASE)
	{ /* First Phase : Initialize and Start */
        if(G_BtSys->BtApp_SYS_GetStatus(BTAPP_STATUS_AV_SINK_STREAM) == TRUE)
		{ /* analyze the stream if current state is streaming */
			memset(&gstBtAvStreamAnalyzer.streamCounters, 0x00, sizeof(gstBtAvStreamAnalyzer.streamCounters));
			gstBtAvStreamAnalyzer.counterIndex = 0;
			gstBtAvStreamAnalyzer.analyzeFlag = TRUE;
			gstBtAvStreamAnalyzer.objectiveStatus = BTAPP_AV_PAUSE;
			TCCAPI_Timer_SetTime(10, TIMER_BT_AV_STREAM_ANALYZE_ID, StreamAnalyzer);
		}
	}
	else if(phase == BTAPP_AV_2nd_PHASE)
	{ /* Second Phase : Action depending on analysis result */
		if(gstBtAvStreamAnalyzer.analysisResult == BTAPP_AV_MUSIC)
		{
			(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PAUSE, TC_BT_AVRCP_PT_STATE_PRESS);
		}
		else if(gstBtAvStreamAnalyzer.analysisResult == BTAPP_AV_NOT_MUSIC)
		{
			/* nothing to do */
		}
		else
		{
			/* never reach */
		}
		gstBtAvStreamAnalyzer.analyzeFlag = FALSE;
	}
	else
	{
		/* never reach */
	}
}

#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
static void PlayByPlayStatus(unsigned char id, unsigned char loc)
{
	if((gstBtAvMetaData.PlayStatus == BTAPP_AV_STOP) || (gstBtAvMetaData.PlayStatus == BTAPP_AV_PAUSE))
	{
		(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PLAY, TC_BT_AVRCP_PT_STATE_PRESS);
	}
}

static void PauseByPlayStatus(unsigned char id, unsigned char loc)
{
	if(gstBtAvMetaData.PlayStatus == BTAPP_AV_PLAY)
	{
		(void)TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PAUSE, TC_BT_AVRCP_PT_STATE_PRESS);
	}
}

static void GetPlayStatusTimer(unsigned char id, unsigned char loc)
{
	(void)TC_BT_AVRCP_CtGetPlayStatus();
}
#endif

static void PlayRetryTimer(unsigned char id, unsigned char loc)
{
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)
	if((gstBtAvMetaData.PlayStatus == BTAPP_AV_STOP) || (gstBtAvMetaData.PlayStatus == BTAPP_AV_PAUSE))
#endif
	{
		if(TC_BT_AVRCP_CtPassThrough(TC_BT_AVRCP_PT_OP_ID_PLAY, TC_BT_AVRCP_PT_STATE_PRESS) == TC_BT_CMD_RESULT_DISCONNECTED_ENUM)
		{
			if(gstBtAvStreamAnalyzer.playRetryCnt > 0)
			{
				gstBtAvStreamAnalyzer.playRetryCnt--;
				TCCAPI_Timer_SetTime(10, TIMER_BT_AVRCP_CONTROL_RETRY_ID, PlayRetryTimer);
			}
		}
	}
}

static void ReportTheResult(void)
{
	if(gstBtAvStreamAnalyzer.objectiveStatus == BTAPP_AV_PLAY)
	{
		PlayProcedure(BTAPP_AV_2nd_PHASE);
	}
	else if(gstBtAvStreamAnalyzer.objectiveStatus == BTAPP_AV_PAUSE)
	{
		PauseProcedure(BTAPP_AV_2nd_PHASE);
	}
	else
	{
		/* never reach */
	}
}

static void UpdateReferenceState(void)
{
#if defined(BT_AUDIO_SINK_AVRCP_METADATA)	
	if(gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable == TRUE)
	{
		if((gstBtAvMetaData.PlayStatus == BTAPP_AV_STOP) || (gstBtAvMetaData.PlayStatus == BTAPP_AV_PAUSE))
		{
			gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PAUSE;
		}
		else
		{
			gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PLAY;
		}
	}
	else
#endif
	{
        if((G_BtSys->BtApp_SYS_GetTick() - gTcBtA2dpFootprint) > 30) /* 150ms *//* TC_Retrieve_Clock(): 1 equal 5ms */
		{
			gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PAUSE;
		}
		else
		{
			gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PLAY;
		}
	}
}
#endif /* BT_AUDIO_SINK_USE_STREAM_ANALYZER */

void BtApp_AV_CT_KeySend(unsigned char key, unsigned char release)
{	/* press : 0, release : 1 */

#ifdef BT_AUDIO_SINK_USE_STREAM_ANALYZER
	switch(key)
	{
		case BTAPP_AV_CT_KEY_ID_PLAY:
			if(gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable == FALSE)
			{
				gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PLAY;
				gstBtAvStreamAnalyzer.skipUpdateRefStatus = TRUE;
			}
		break;
		case BTAPP_AV_CT_KEY_ID_PAUSE:
			if(gstBtAvStreamAnalyzer.isTargetPlayStatusAvailable == FALSE)
			{
				gstBtAvStreamAnalyzer.referenceStatus = BTAPP_AV_PAUSE;
				gstBtAvStreamAnalyzer.skipUpdateRefStatus = TRUE;
			}
		break;
	}
#endif

	if(release == FALSE)
	{	/* Release-Key will be sent automatically if you send Press-Key. */
		(void)TC_BT_AVRCP_CtPassThrough(key, TC_BT_AVRCP_PT_STATE_PRESS);
	}
	else
	{	/* Only for FF/REW key operation */
		(void)TC_BT_AVRCP_CtPassThrough(key, TC_BT_AVRCP_PT_STATE_RELEASE);
	}
}

void BtApp_AV_SNK_AlsaInit(void)
{
    //memset(&gBtAvPcmInst, 0x00, sizeof(BTAPP_AV_PCM));
    gBtAvAlsaHandle = NULL;

}

void BtApp_AV_SNK_AlsaSetParam(unsigned short sampleRate, unsigned char ch)
{
	gBtAvPcmInst.usSampleRate = (sampleRate == 0)?(44100):(sampleRate);
	gBtAvPcmInst.ucChannel    = (ch == 9)?(2):(ch);
}

int BtApp_AV_SNK_AlsaOpen(void)
{
	int fr = 0;

    /* If ALSA PCM driver was already open => close it */
    if (gBtAvAlsaHandle != NULL)
    {
    	snd_pcm_drop(gBtAvAlsaHandle);
        snd_pcm_close(gBtAvAlsaHandle);
        gBtAvAlsaHandle = NULL;
    }

    /* Open ALSA driver */
    fr = snd_pcm_open(&gBtAvAlsaHandle, alsa_device, SND_PCM_STREAM_PLAYBACK, /* SND_PCM_NONBLOCK */ 0);
    if (fr < 0)
    {

    	fr = -1;
    	goto ErrExit;
    }
    /* Configure ALSA driver with PCM parameters */
    //gBtAvPcmInst.usSampleRate = 44100;
    //gBtAvPcmInst.ucChannel    = 2;
    fr = snd_pcm_set_params(gBtAvAlsaHandle, SND_PCM_FORMAT_S16_LE,
				SND_PCM_ACCESS_RW_INTERLEAVED, gBtAvPcmInst.ucChannel, gBtAvPcmInst.usSampleRate, 1, 500000); /* 0.5sec */
    if (fr < 0)
    {
		fr = -2;
		goto ErrExit;
    }
ErrExit:
	if(fr != 0)
    {
		gBtAvAlsaHandle = NULL;
	}
	
	return fr;
}

void BtApp_AV_SNK_AlsaClose(void)
{
	if(gBtAvAlsaHandle != NULL)
    {printf("come  to here %s   %d\n",__FUNCTION__,__LINE__);
		snd_pcm_drop(gBtAvAlsaHandle);
		snd_pcm_close(gBtAvAlsaHandle);
		gBtAvAlsaHandle = NULL;
	}

}

int BtApp_AV_SNK_AlsaWrite(unsigned char *pBuf, long BufLen)
{
	int fr = 0;
    snd_pcm_sframes_t alsa_frames;
    snd_pcm_sframes_t alsa_frames_to_send;
	if(gBtAvAlsaHandle == NULL)
	{
		fr = -1;
		goto ErrExit;
	}

    /* Compute number of PCM samples (contained in p_msg->len bytes) */
    alsa_frames_to_send = BufLen / gBtAvPcmInst.ucChannel;
	alsa_frames_to_send /= 2;

    alsa_frames = snd_pcm_writei(gBtAvAlsaHandle, pBuf, alsa_frames_to_send);
    if (alsa_frames < 0)
    {
        alsa_frames = snd_pcm_recover(gBtAvAlsaHandle, alsa_frames, 0);
    }
    if (alsa_frames < 0)
    {
/*            printf("app_avk_uipc_cback snd_pcm_writei failed:%s\n", snd_strerror(alsa_frames)); */
		fr = -2;
		goto ErrExit;
    }

    if (alsa_frames > 0 && alsa_frames < alsa_frames_to_send)
    {
        printf("app_avk_uipc_cback Short write (expected %li, wrote %li)\n", (long) alsa_frames_to_send, alsa_frames);
	}

ErrExit:
	return fr;
}

#endif	/* BT_AUDIO_SINK */
/* End Of File */
