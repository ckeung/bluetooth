/****************************************************************************
 *   FileName    : aecif.h
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

*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    15/05/08     0.1            created                     Hwang, Jin Ho.
*******************************************************************************/

#ifndef __AEC_IF_H__
#define __AEC_IF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************
*
* Header File Include
*
******************************************************************************/
#include "BT_CFG.h"
#include "aecerr.h"
#include "aectype.h"
/* TODO: Add your specialized code here */

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

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#define AEC_VER_SINGLE_MICROPHONE						/* single mic. */
//#define AEC_VER_DUAL_MICROPHONE						/* dual mic. - reserved */
//#define AEC_SCO_PCM_INTERFACE							/* pcm i/f - reserved */

/*#define AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS*/	/* vda-hft params. - UFD, UART */
/*#define AEC_VDA_HFT_MEASUREMENT_EXTERNAL_VOLUME*/		/* vda-hft params. - UFD, UART */

/* Use SRC config of BT_CFG.h */
/*--------------------------------------------------*/
#if 0
//#define BT_HFP_SRC
//#define BT_HFP_SAMPLERATE 		48000		
#endif

/*
#define BT_HFP_SAMPLERATE 		32000		
#define BT_HFP_LOOPBACK_TEST	
--------------------------------------------------*/

typedef enum
{
	AEC_PACKET_RING=0,
	AEC_PACKET_OUTPUT=0x5a5a,
	AEC_PACKET_INPUT=0xa5a5
} eAECPacketType;

#define AEC_DUMP_ON(dump) (1UL<<(dump))
#define AEC_TUNE_KIT_MAX_TRANS_STRING_SIZE (4000/*4Kbyte*/)

typedef enum
{
	/* Far-End Speech */	/* Receive the sound from BT */
//	AEC_DUMP_FIS,	/* Dump the original sound received from BT */
	AEC_DUMP_SRB = 0,	/* Dump the sound before SRC */
	AEC_DUMP_LPB,	/* Dump the sound before LPF */
	AEC_DUMP_SPK,	/* Dump the sound of the loud speaker */
	
	AEC_DUMP_FES,	/* Dump the sound before AEC */

	/* Near-End Speech */	/* Transmit the sound to BT */
	AEC_DUMP_MIC,	/* Dump the original sound of the microphone */
	AEC_DUMP_MID,	/* Dump the original sound of the sencondary microphone */
	AEC_DUMP_LPA,	/* Dump the sound after LPF */
	AEC_DUMP_LPD,	/* Dump the sencondary sound after LPF */
//	AEC_DUMP_SRA,	/* Dump the sound after SRC */
//	AEC_DUMP_SRD,	/* Dump the sencondary sound after SRC */
	AEC_DUMP_NES,	/* Dump the primary sound before AEC */
	AEC_DUMP_NED,	/* Dump the secondary sound before AEC */

	/* Echo-Free Speech */	/* Transmit the sound to BT */
	AEC_DUMP_RES,	/* Dump the echo free sound after AEC */
	AEC_DUMP_CNG,	/* Dump the sound after CNG */
	AEC_DUMP_AGC,	/* Dump the sound after AGC */
	AEC_DUMP_FOS,	/* Dump the echo free sound transmitted to BT */

	/* Noise-Suppression Threshold */
	AEC_DUMP_NST,	/* Dump the threshold of the dynamic noise suppression */	
	AEC_DUMP_MAX_NUM_OF_FILE
} eAECDump;

typedef enum
{
	AEC_SAMPLERATE_8KHZ = 0,
	AEC_SAMPLERATE_16KHZ,
	AEC_SAMPLERATE_24KHZ,
	AEC_SAMPLERATE_MAX
}eAECSampleRate;

enum
{	
	AEC_LPF_SAMPLE_RATE_8KHZ =	8000,	/* 8khz */	
	AEC_LPF_SAMPLE_RATE_16KHZ =	16000,	/* 16khz */
	AEC_LPF_SAMPLE_RATE_24KHZ =	24000,	/* 24khz */
	AEC_LPF_SAMPLE_RATE_32KHZ =	32000,	/* 32khz */
	AEC_LPF_SAMPLE_RATE_48KHZ =	48000	/* 48khz */
};

/* volume */
enum 
{
	AEC_SPK_VOL_0=0,
	AEC_SPK_VOL_1=1,
	AEC_SPK_VOL_2=2,
	AEC_SPK_VOL_3=3,
	AEC_SPK_VOL_4=4,
	AEC_SPK_VOL_5=5,
	AEC_SPK_VOL_6=6,
	AEC_SPK_VOL_7=7,
	AEC_SPK_VOL_8=8,
	AEC_SPK_VOL_9=9,
	AEC_SPK_VOL_10=10,
	AEC_SPK_VOL_11=11,
	AEC_SPK_VOL_12=12,
	AEC_SPK_VOL_13=13,
	AEC_SPK_VOL_14=14,
	AEC_SPK_VOL_15=15,
	
	AEC_SPK_VOL_MAX=AEC_SPK_VOL_15,
	
	AEC_SPK_VOL_16=AEC_SPK_VOL_MAX+1,
	AEC_SPK_VOL_17=AEC_SPK_VOL_MAX+2,
	AEC_SPK_VOL_18=AEC_SPK_VOL_MAX+3,
	AEC_SPK_VOL_19=AEC_SPK_VOL_MAX+4,
	AEC_SPK_VOL_20=AEC_SPK_VOL_MAX+5,
	AEC_SPK_VOL_21=AEC_SPK_VOL_MAX+6,
	AEC_SPK_VOL_22=AEC_SPK_VOL_MAX+7,
	AEC_SPK_VOL_23=AEC_SPK_VOL_MAX+8,
	AEC_SPK_VOL_24=AEC_SPK_VOL_MAX+9,
	AEC_SPK_VOL_25=AEC_SPK_VOL_MAX+10,
	AEC_SPK_VOL_26=AEC_SPK_VOL_MAX+11,
	AEC_SPK_VOL_27=AEC_SPK_VOL_MAX+12,
	AEC_SPK_VOL_28=AEC_SPK_VOL_MAX+13,
	AEC_SPK_VOL_29=AEC_SPK_VOL_MAX+14,
	AEC_SPK_VOL_30=AEC_SPK_VOL_MAX+15
};

enum 
{
	AEC_MIC_VOL_0=0,
	AEC_MIC_VOL_1=1,
	AEC_MIC_VOL_2=2,
	AEC_MIC_VOL_3=3,
	AEC_MIC_VOL_4=4,
	AEC_MIC_VOL_5=5,
	AEC_MIC_VOL_6=6,
	AEC_MIC_VOL_7=7,
	AEC_MIC_VOL_8=8,
	AEC_MIC_VOL_9=9,
	AEC_MIC_VOL_10=10,
	AEC_MIC_VOL_11=11,
	AEC_MIC_VOL_12=12,
	AEC_MIC_VOL_13=13,
	AEC_MIC_VOL_14=14,
	AEC_MIC_VOL_15=15,

	AEC_MIC_VOL_MAX=AEC_MIC_VOL_15,
	
	AEC_MIC_VOL_16=AEC_MIC_VOL_MAX+1,
	AEC_MIC_VOL_17=AEC_MIC_VOL_MAX+2,
	AEC_MIC_VOL_18=AEC_MIC_VOL_MAX+3,
	AEC_MIC_VOL_19=AEC_MIC_VOL_MAX+4,
	AEC_MIC_VOL_20=AEC_MIC_VOL_MAX+5,
	AEC_MIC_VOL_21=AEC_MIC_VOL_MAX+6,
	AEC_MIC_VOL_22=AEC_MIC_VOL_MAX+7,
	AEC_MIC_VOL_23=AEC_MIC_VOL_MAX+8,
	AEC_MIC_VOL_24=AEC_MIC_VOL_MAX+9,
	AEC_MIC_VOL_25=AEC_MIC_VOL_MAX+10,
	AEC_MIC_VOL_26=AEC_MIC_VOL_MAX+11,
	AEC_MIC_VOL_27=AEC_MIC_VOL_MAX+12,
	AEC_MIC_VOL_28=AEC_MIC_VOL_MAX+13,
	AEC_MIC_VOL_29=AEC_MIC_VOL_MAX+14,
	AEC_MIC_VOL_30=AEC_MIC_VOL_MAX+15
};

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* AEC options */
typedef struct
{
	_uint_t uiHeapSize;
	
	/* BT_HFP_SRC - 32, 48 */
	_uchar_t boUseSRC;
	_int_t iSRCSampleRate;

	/* Reserved */
	_uchar_t boReserved;
	
	/* AEC_VER_SINGLE_MICROPHONE */
	_uchar_t boUseAECVerSingleMIC;
	_uchar_t boUseAECVerDualMIC;

	/* AEC_SCO_PCM_INTERFACE */
	_uchar_t boUseSCOPCMIF;
	
	/* MIC_IN_ADC */
	_uchar_t boUseMICInADC;
	
} tAECIfConfig;

typedef struct 
{
	_uchar_t ucIndex;
	_uint_t uiGain;
} tAECIfVolumeTable;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
_extern_t _const_t tAECIfConfig gstAECIfConfig;

#ifdef BT_HFP_TUNE_KIT
_extern_t tAECParams gstAECIfPCToolParams;
#endif

/* aec functions */
_extern_t _void_t AECIf_AECLock(_void_t);
_extern_t _void_t AECIf_AECUnlock(_void_t);
_extern_t _uchar_t AECIf_IsValidParameter(_void_t);
_extern_t _void_t AECIf_SetPCToolCloseCall(_uchar_t ucClose);
_extern_t _void_t UI_API_HfMicInputControl(_short_t *psLeftNearIn);
_extern_t _void_t UI_API_HfMicGainControl(_short_t *pTempFarOut);

//_extern_t _void_t AECIf_SetPCToolCloseCall(_uchar_t ucClose);

/* aec speaker volume i/f */
_extern_t _uint_t AECIf_GetSpeakerVolumeGain(_uchar_t ucIndex);
_extern_t _uchar_t AECIf_GetSpeakerCurrentVolumeIndex(_void_t);
_extern_t _void_t AECIf_SetSpeakerCurrentVolumeIndex(_uchar_t ucIndex);
_extern_t _int_t AECIf_GetSpeakerMaximumVolumeIndex(_void_t);

/* aec microphone volume i/f */
_extern_t _uint_t AECIf_GetMicrophoneVolumeGain(_uchar_t ucIndex);
_extern_t _uchar_t AECIf_GetMicrophoneCurrentVolumeIndex(_void_t);
_extern_t _void_t AECIf_SetMicrophoneCurrentVolumeIndex(_uchar_t ucIndex);	/* reserved */
_extern_t _int_t AECIf_GetMicrophoneMaximumVolumeIndex(_void_t);

/* aec parameters i/f */
_extern_t _void_t AECIf_SetParameters(_char_t cMode,_int_t uiSampleRate);
#ifdef __TCC_ANDROID__
_extern_t _void_t AECIf_SetParametersCarPlay(_char_t cMode,_int_t iSampleRate,void *pstParams,void *pstDEQFarIn,void *pstDEQFarOut,_int_t usbUSBparam);
#endif//__TCC_ANDROID__

#ifdef __TCC_ANDROID__
_extern_t _uchar_t AECIf_SetEchoDelay(_uint_t iSampleRate,_uint_t iEchoDelay);
_extern_t _uint_t AECIf_GetEchoDelay(_uint_t iSampleRate);
#endif//__TCC_ANDROID__

_extern_t _char_t AECIf_SavePCToolParameters(_const_t _char_t *buffer, _uint_t length);
_extern_t _void_t AECIf_PrintPCToolParameters(_uchar_t boReset);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __AEC_IF_H__ */
