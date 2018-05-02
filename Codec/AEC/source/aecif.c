/********************************************************************************************
*
*   FileName    : aecif.c
*   Description :
*
*********************************************************************************************
*
*   TCC Version 1.0
*   Copyright (c) Telechips Inc.
*   All rights reserved
*
*   This source code contains confidential information of Telechips.
*   Any unauthorized use without a written permission of Telechips including not limited
*   to redistribution in source or binary form is strictly prohibited.
*   This source code is provided "AS IS" and nothing contained in this source code shall
*   constitute any express or implied warranty of any kind, including without limitation,
*   any warranty of merchantability, fitness for a particular purpose or non-infringement
*   of any patent, copyright or other third party intellectual property right.
*   No warranty is made, express or implied, regarding the information's accuracy,
*   completeness, or performance.
*   In no event shall Telechips be liable for any claim, damages or other liability arising
*   from, out of or in connection with this source code or the use in the source code.
*   This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
*   between Telechips and Company.
*
********************************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __arm__
#if defined(BT_AEC_INCLUDE)

#include <BT_CFG.h>
#include <aecif.h>
#include <aecparams.h>
#include <aecutil.h>

#define DEQ_MAX_NUM_OF_BAND	12

tAECParams gstAECIfPCToolParams;

/**
 * exterrn 
 */

/**
 * static
 */
_static_t tAECParams gstAECIfParams;

_static_t int AECIf_GetSpeakerNominalVolumeIndex(void);

_static_t int AECIf_GetMicrophoneNominalVolumeIndex(void);
#if defined(AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS)
_static_t _void_t AECIf_SetExternalParameters(tAECParams *pstParams);
_static_t _void_t AECIf_GetProfileString(_char_t *Section,_char_t *Key,_char_t *Default,_char_t *pstrValue);
_static_t _void_t AECIf_GetProfileValue(_char_t *Section,_char_t *Key,_char_t *Default,_int_t *piValue);
_static_t _void_t AECIf_SetProfileValues(tAECParams *pstParams);
#endif

_static_t _void_t AECIf_SetRealParameters(_char_t cMode,tAECParams *pstParams,tAECParamsDEQ *pstDEQFarIn,tAECParamsDEQ *pstDEQFarOut);

_static_t _void_t *gpvAECIfParamsBase=NULL;

/*****************************************************************************
*
* AEC Options
*
*****************************************************************************/
_const_t tAECIfConfig gstAECIfConfig =
{
    4096*1024UL,					/* uiHeapSize */
	
	/* BT_HFP_SRC - 32, 48 */
#ifdef BT_HFP_SRC
	TRUE, 							/* UseSRC */
	BT_HFP_SAMPLERATE,				/* iSRCSampleRate */
#else
	FALSE,							/* UseSRC */
	0,
#endif

	/* AEC Version */
#ifdef AEC_VER_SINGLE_MICROPHONE
	TRUE,							/* AECVerSingleMIC */
	FALSE,							/* AECVerDualMIC */
#else
	FALSE,							/* AECVerSingleMIC */
	TRUE,							/* AECVerDualMIC */
#endif

#ifdef AEC_SCO_PCM_INTERFACE
	TRUE,							/* UsePCMIF */
#else	
	FALSE,							/* UsePCMIF */
#endif	

	/* MIC_IN_ADC */				/* boUseMICInADC */
	FALSE
};

/*****************************************************************************
*
* default parameters
*
*****************************************************************************/
//#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_VOLUME	/* reserved */
_static_t tAECIfVolumeTable
	gstAECIfSpeakerVolumeTable[AEC_SAMPLERATE_MAX][AEC_SPK_VOL_30+1] =
{
	{
        {AEC_SPK_VOL_0,			0},
        {AEC_SPK_VOL_1,			64},
        {AEC_SPK_VOL_2,			68},
        {AEC_SPK_VOL_3,			72},
        {AEC_SPK_VOL_4,			76},
        {AEC_SPK_VOL_5,			81},
        {AEC_SPK_VOL_6,			85},
        {AEC_SPK_VOL_7,			89},
        {AEC_SPK_VOL_8,			93},	/* 30 */
        {AEC_SPK_VOL_9,			97},	/* 34 */
        {AEC_SPK_VOL_10,		102},	/* 37 */
        {AEC_SPK_VOL_11,		106},	/* 41 */
        {AEC_SPK_VOL_12,		110},	/* 45 */
        {AEC_SPK_VOL_13,		114},	/* 49 */
        {AEC_SPK_VOL_14,		118},	/* 53 */
        {AEC_SPK_VOL_15,		123},	/*Up to 0dB*/	/* tca_wm8731_setoutputgain : 56 */

		{AEC_SPK_VOL_16,		1},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_17,		4},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_18,		7},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_19,		10},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_20,		13},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_21,		16},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_22,		19},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_23,		22},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_24,		25},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_25,		28},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_26,		31},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_27,		34},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_28,		37},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_29,		40},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_30,		43}		/* reserved for the volume extenstion */
	},

/* This table used when WBS, NBS needs different volume tables : For Test */
#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS
	{
        {AEC_SPK_VOL_0,			0},
        {AEC_SPK_VOL_1,			85},
        {AEC_SPK_VOL_2,			87},
        {AEC_SPK_VOL_3,			90},
        {AEC_SPK_VOL_4,			93},
        {AEC_SPK_VOL_5,			96},
        {AEC_SPK_VOL_6,			99},
        {AEC_SPK_VOL_7,			101},
        {AEC_SPK_VOL_8,			104},	/* 30 */
        {AEC_SPK_VOL_9,			107},	/* 34 */
        {AEC_SPK_VOL_10,		110},	/* 37 */
        {AEC_SPK_VOL_11,		113},	/* 41 */
        {AEC_SPK_VOL_12,		115},	/* 45 */
        {AEC_SPK_VOL_13,		118},	/* 49 */
        {AEC_SPK_VOL_14,		121},	/* 53 */
        {AEC_SPK_VOL_15,		124},	/*Up to 0dB*/	/* tca_wm8731_setoutputgain : 56 */

		{AEC_SPK_VOL_16,		1},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_17,		4},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_18,		7},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_19,		10},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_20,		13},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_21,		16},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_22,		19},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_23,		22},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_24,		25},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_25,		28},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_26,		31},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_27,		34},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_28,		37},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_29,		40},	/* reserved for the volume extenstion */
		{AEC_SPK_VOL_30,		43}		/* reserved for the volume extenstion */
	}
#endif
};

_static_t tAECIfVolumeTable	
	gstAECIfMicrophoneVolumeTable[AEC_SAMPLERATE_MAX][AEC_MIC_VOL_30 +1] =
{
	{
		{AEC_MIC_VOL_0,			0},
		{AEC_MIC_VOL_1,			1},
		{AEC_MIC_VOL_2,			1},
		{AEC_MIC_VOL_3,			1},
		{AEC_MIC_VOL_4,			1},
		{AEC_MIC_VOL_5,			1},
		{AEC_MIC_VOL_6,			1},
		{AEC_MIC_VOL_7,			1},
		{AEC_MIC_VOL_8,			1},
		{AEC_MIC_VOL_9,			1},
		{AEC_MIC_VOL_10,		1},
		{AEC_MIC_VOL_11,		1},
		{AEC_MIC_VOL_12,		1},
		{AEC_MIC_VOL_13,		1},
		{AEC_MIC_VOL_14,		1},
		{AEC_MIC_VOL_15,		1},
		
		{AEC_MIC_VOL_16,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_17,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_18,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_19,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_20,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_21,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_22,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_23,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_24,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_25,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_26,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_27,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_28,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_29,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_30,		1}	/* reserved for the volume extenstion */
	},

/* This table used when WBS, NBS needs different volume tables : For Test */
#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS
	{
		{AEC_MIC_VOL_0,			0},
		{AEC_MIC_VOL_1,			1},
		{AEC_MIC_VOL_2,			1},
		{AEC_MIC_VOL_3,			1},
		{AEC_MIC_VOL_4,			1},
		{AEC_MIC_VOL_5,			1},
		{AEC_MIC_VOL_6,			1},
		{AEC_MIC_VOL_7,			1},
		{AEC_MIC_VOL_8,			1},
		{AEC_MIC_VOL_9,			1},
		{AEC_MIC_VOL_10,		1},
		{AEC_MIC_VOL_11,		1},
		{AEC_MIC_VOL_12,		1},
		{AEC_MIC_VOL_13,		1},
		{AEC_MIC_VOL_14,		1},
		{AEC_MIC_VOL_15,		1},
		
		{AEC_MIC_VOL_16,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_17,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_18,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_19,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_20,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_21,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_22,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_23,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_24,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_25,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_26,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_27,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_28,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_29,		1},	/* reserved for the volume extenstion */
		{AEC_MIC_VOL_30,		1}	/* reserved for the volume extenstion */
	}
#endif
};
//#endif

_static_t tAECParamsDEQ gstAECIfParamsDEQFarIn[AEC_SAMPLERATE_MAX][DEQ_MAX_NUM_OF_BAND]=
{
	/* NBS - Fc,  Q, fil_type, BandGain */
	{
		{  400, 13,        1,        0},
		{  600,  9,        1,        0},
		{  800,  9,        1,        0},
		{ 1000, 19,        1,        0},
		{ 1600, 19,        1,        0},
		{ 2100, 29,        1,        0},
		{ 2800, 15,        1,        0},
		{ 3000,  9,        1,        0},
		{ 3200, 19,        1,        0},
		{ 3500,  9,        1,        0},
		{ 3700,  9,        1,        0},
		{ 3800,  9,        1,        0}	
	},		

	/* WBS - Fc,  Q, fil_type, BandGain */
	{
		{  200,  9,        1,        0},
		{  500, 29,        1,        0},
		{  600, 19,        1,        0},
		{  800, 29,        1,        0},
		{ 1000, 39,        1,        0},
		{ 2000, 29,        1,        0},
		{ 3000, 19,        1,        0},
		{ 4000, 29,        1,        0},
		{ 5000, 19,        1,        0},
		{ 6000,  9,        1,        0},
		{ 7000,  9,        1,        0},
		{ 7500,  9,        1,        0}
	},

	/* SWBS - Fc,  Q, fil_type, BandGain */
	{
		{  200,  9,        1,        0},
		{  500, 29,        1,        0},
		{  600, 19,        1,        0},
		{  800, 29,        1,        0},
		{ 1000, 39,        1,        0},
		{ 3000, 29,        1,        0},
		{ 5000, 19,        1,        0},
		{ 7000, 29,        1,        0},
		{ 8000, 19,        1,        0},
		{ 9000,  9,        1,        0},
		{11000,  9,        1,        0},
		{11500,  9,        1,        0}
	}		
};

_static_t tAECParamsDEQ gstAECIfParamsDEQFarOut[AEC_SAMPLERATE_MAX][DEQ_MAX_NUM_OF_BAND]=
{
	/* NBS - Fc,  Q, fil_type, BandGain */
	{
		{  200,  9,        1,        0},
		{  500, 29,        1,        0},
		{  600, 19,        1,        0},
		{  800, 29,        1,        0},
		{ 1000, 39,        1,        0},
		{ 1600, 29,        1,        0},
		{ 2000, 19,        1,        0},
		{ 2500, 29,        1,        0},
		{ 2800, 19,        1,        0},
		{ 3000,  9,        1,        0},
		{ 3700,  9,        1,        0},
		{ 3800,  9,        1,        0}
	},		

	/* WBS - Fc,  Q, fil_type, BandGain */
	{
		{  200,  9,        1,        0},
		{  500, 29,        1,        0},
		{  600, 19,        1,        0},
		{  800, 29,        1,        0},
		{ 1000, 39,        1,        0},
		{ 2000, 29,        1,        0},
		{ 3000, 19,        1,        0},
		{ 4000, 29,        1,        0},
		{ 5000, 19,        1,        0},
		{ 6000,  9,        1,        0},
		{ 7000,  9,        1,        0},
		{ 7500,  9,        1,        0}
	},

	/* SWBS - Fc,  Q, fil_type, BandGain */
	{
		{  200,  9,        1,        0},
		{  500, 29,        1,        0},
		{  600, 19,        1,        0},
		{  800, 29,        1,        0},
		{ 1000, 39,        1,        0},
		{ 3000, 29,        1,        0},
		{ 5000, 19,        1,        0},
		{ 7000, 29,        1,        0},
		{ 8000, 19,        1,        0},
		{ 9000,  9,        1,        0},
		{11000,  9,        1,        0},
		{11500,  9,        1,        0}
	}	
};

_static_t const tAECParams gstAECIfParamsDefault=
{
	FALSE,	/* boBypass */
	FALSE,	/* boDump */	
	0,		/* boPathEnable */
	"./aec/AECif_Params.ini", /* cIniPath */
	"./NB",                  /* cNBDumpPath */
	"./WB",                  /* cWBDumpPath */
	"./SWB",                  /* cSWBDumpPath */
	0 /* ulDumpOptions */
	//|AEC_DUMP_ON(AEC_DUMP_RCV)
	//|AEC_DUMP_ON(AEC_DUMP_RAG)
	//|AEC_DUMP_ON(AEC_DUMP_RDR)
	//|AEC_DUMP_ON(AEC_DUMP_RDQ)
	|AEC_DUMP_ON(AEC_DUMP_FES)
	|AEC_DUMP_ON(AEC_DUMP_NES)
	//|AEC_DUMP_ON(AEC_DUMP_MIC)
	//|AEC_DUMP_ON(AEC_DUMP_LPA)
	|AEC_DUMP_ON(AEC_DUMP_RES)
	//|AEC_DUMP_ON(AEC_DUMP_FOS)
	//|AEC_DUMP_ON(AEC_DUMP_NST)
	,
	20,	/* ulDumpSec */
	
	FALSE, /* boUseExternalSamplesFarIn */

	/* Gain control */
	TRUE,
	0,
	TRUE,
	0,
	TRUE,
	0,
	TRUE,
	0,

	/* Receiving direction */
	TRUE,	/* boUseAGCFarIn */
	0,		/* iThrdBAGCFarIn */
	0,		/* iAttdBAGCFarIn */
	2,		/* iStepSizeAttAGCFarIn */
	2,		/* iStepSizeRelAGCFarIn */

	0,		/* boUseDRCFarIn */
	0,		/* iThrDRCFarIn */
	10,		/* iRatioDRCFarIn */
	10,		/* iKneeDRCFarIn */
	10,		/* iAttkDRCFarIn */
	100,	/* iRelDRCFarIn */
	0,		/* iLimDRCFarIn */

	TRUE,	/* boUseDEQFarIn */
	0,		/* ulDEQFarIn */
	DEQ_MAX_NUM_OF_BAND, /* iDEQNumFarIn */

	TRUE,	/* boUseLPFFarIn */
	6000,	/* iLPFFarInCenterFreq */
	12000,	/* iLPFFarInWBSCenterFreq */
	18000,  /* iLPFFarInSWBSCenterFreq */
#if defined(BT_HFP_SRC) && (BT_HFP_SAMPLERATE == 48000)
	AEC_LPF_SAMPLE_RATE_48KHZ, /* iLPFFarInSampleRate */
#else
	AEC_LPF_SAMPLE_RATE_32KHZ, /* iLPFFarInSampleRate */
#endif

	/* Sending direction */
	TRUE,	/* boUseDEQFarOut */
	0,		/* ulDEQFarOut */
	DEQ_MAX_NUM_OF_BAND, /* iDEQNumFarOut */

	TRUE,	/* boUseLPFFarOut */
	6000,	/* iLPFFarOutCenterFreq */
	12000,	/* iLPFFarOutWBSCenterFreq */
	18000,  /* iLPFFarOutSWBSCenterFreq */
#if defined(BT_HFP_SRC) && (BT_HFP_SAMPLERATE == 48000)
	AEC_LPF_SAMPLE_RATE_48KHZ, /* iLPFFarOutSampleRate */
#else
	AEC_LPF_SAMPLE_RATE_32KHZ, /* iLPFFarOutSampleRate */
#endif

#ifdef BT_HFP_SRC
	4,
	4,
	4,
	4,
#else
	20,		/* ulNBEchoDelayMilliSec */
	10,		/* ulWBEchoDelayMilliSec */
	5,		/* ulSWBEchoDelayMilliSec */
	4,		/* ulSRCEchoDelayMilliSec */
#endif

	0,		/* boUseInitEcho */
	100,	/* iInitEchoIntvN */
	100,	/* iInitEchoIntvP */

	200, /* ulDummySoundMilliSec */

	/*psAecHandle->m_sAecParams structure member setting */
	-15,	/* Noise suppression DB */	/* 0 : bypass */
	FALSE,	/* The dynamic noise suppression DB */
	80000,	/* The threshold of the dynamic noise suppression DB */
	-12,	/* The maximum Noise suppression DB of the dynamic noise suppression DB */	/* 0 : bypass */
	-8,		/* The minimum Noise suppression DB of the dynamic noise suppression DB */	/* 0 : bypass */
	5,		/* The repeatcount f the dynamic noise suppression DB */	/* the number of times */

	-50,	/* Echo suppression DB */	/* 0 : bypass */
	-25,	/* Echo suppression Active DB */	/* 0 : bypass */
										   
	0,		/* NES Gain : Q10 scaling, before AEC microphone gain ex. ( (int)(0.5f*32768) ) */
	0,		/* FES Gain : Q10 scaling, SPK gain : reserved!! */
	0,		/* RES Gain : Q10 scaling, restore gain decreased from NES gain : 1/NES gain ==> ex. ( (int)(2.0f*32768) ) */

/* NB */	
	250,	/* Threshold of frequency band to reduce level of RES. Unit(Hz) */ /* ~ 300Hz */
	100,	/* the reduction of the low frequency band level */ /* 20~100%*/
/* WB */
	250,	/* Threshold of frequency band to reduce level of RES. Unit(Hz) */ 
	100,	/* the reduction of the low frequency band level */
/* SWB */
	250,	/* Threshold of frequency band to reduce level of RES. Unit(Hz) */ 
	100,	/* the reduction of the low frequency band level */
	
	160,	/* The framesize, 160 samples */
	8000,	/* The samplerate, 8000Hz or 16000hz */
	800,	/* The filtersize, 800 or 1200 */

	6,		/* LTSE Frame : default 6 */		
	0,		/* Hangover Flag : default 0 */
	11,		/* NNS level : 1~14, default 11, smaller value makes noise lower */
	100,	/* CNG level : 0~100 */

#ifdef AEC_VER_SINGLE_MICROPHONE
	0,
#else
	0,
#endif	/* AEC_VER_SINGLE_MICROPHONE */

	100,				/* iAmpGain */ /* 40~300% */
	0,					/* uiLibOpMode */
	AEC_SPK_VOL_11, 	/* The nominal volume index */
	100,					/* The reduction of the sound to the microphone with the nominal volume */
						/* 40~100% */
	AEC_SPK_VOL_MAX, 	/* The maximum volume index */
	60,					/* The reduction of the sound to the microphone with the maximum volume */
						/* 40~100% */
						
	AEC_MIC_VOL_11,	/* The nominal volume index */				
	AEC_MIC_VOL_MAX,	/* The maximum volume index */				
};

/*************************************************************************************
*  AECIf_AECLock
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
_void_t AECIf_AECLock(void)
{
//	#if defined(TCC801X)
//		DAI_Disable(0);
//	#else
//		if(gstDai0.ucType==DAI_FIRST_PLAYBACK)
//		{
//			DisableDAI0();
//		}
//		else if(gstDai1.ucType==DAI_FIRST_PLAYBACK)
//		{
//			DisableDAI1();
//		}
//	#endif
}

/*************************************************************************************
*  AECIf_AECUnlock
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
_void_t AECIf_AECUnlock(void)
{
//	#if defined(TCC801X)
//		DAI_Enable(0);
//	#else
//		if(gstDai0.ucType==DAI_FIRST_PLAYBACK)
//		{
//			EnableDAI0();
//		}
//		else if(gstDai1.ucType==DAI_FIRST_PLAYBACK)
//		{
//			EnableDAI1();
//		}
//	#endif
}

/*************************************************************************************
*  AECIf_IsValidParameter
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
_uchar_t AECIf_IsValidParameter(void)
{
	if(gpvAECIfParamsBase!=NULL)
		return TRUE;

	return FALSE;
}

/*************************************************************************************
 *	pAEC.c functions end															 *
 *************************************************************************************/

#if 1
#define AEC_DC_OFFSET (506)		// This value is modified up to MIC gain
#define AEC_SCALE_SHIFT (4)
/*************************************************************************************
*  UI_API_HfMicInputControl
*-------------------------------------------------------------------------------------
* Description  : NES input dc offset & scale control(ADC input)
* Parameters   : 
* Return	   : 
**************************************************************************************/
_void_t UI_API_HfMicInputControl(_short_t *psLeftNearIn)
{
	tAECParams *pstParams=gpvAECIfParamsBase;
	_int_t  i;
	_short_t  *ppsNearIn = psLeftNearIn;

	for ( i = 0 ; i < pstParams->iFrameSize ; i ++ )
	{
		*ppsNearIn -= AEC_DC_OFFSET;
		*ppsNearIn <<= AEC_SCALE_SHIFT;
		ppsNearIn++;
	}
}

/*************************************************************************************
*  UI_API_HfMicGainControl
*-------------------------------------------------------------------------------------
* Description  : RES gain control, if ampgain 300 is not enough(ADC input)
* Parameters   : 
* Return	   : 
**************************************************************************************/
_void_t UI_API_HfMicGainControl(_short_t *pTempFarOut)
{
	tAECParams *pstParams=gpvAECIfParamsBase;
	_int_t i, gain;

	if(AECIf_GetMicrophoneCurrentVolumeIndex()/*UI_API_GetCurrUiMicGainLevel()*/ > AEC_MIC_VOL_11)
	{
		gain = AECIf_GetMicrophoneCurrentVolumeIndex()/*UI_API_GetCurrUiMicGainLevel()*/ - AEC_MIC_VOL_11;
		gain += 2;		// increase gain after AEC process
		for(i=0; i<pstParams->iFrameSize; i++)
			pTempFarOut[i] <<= gain;	
	}	
	else
	{
		gain =  AEC_MIC_VOL_11/*AECIF_MIC_VOL_11*/ - AECIf_GetMicrophoneCurrentVolumeIndex()/*UI_API_GetCurrUiMicGainLevel()*/;
		for(i=0; i<pstParams->iFrameSize; i++)
			pTempFarOut[i] >>= gain;
	}
}
#endif

//#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_VOLUME	/* reserved */
_uint_t AECIf_GetSpeakerVolumeGain(_uchar_t ucIndex)
{
	tAECParams *pstParams=gpvAECIfParamsBase;
	_uchar_t ucTmpIndex;
	eAECSampleRate eSampleRate = AEC_SAMPLERATE_8KHZ;
	
	if(pstParams==NULL)
	{
		return 0;
	}
#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS
	/* WBS, NBS use different volume : for Test */
	if(pstParams->iSampleRate == 24000)
	{
		eSampleRate = AEC_SAMPLERATE_24KHZ;
	}
	else if(pstParams->iSampleRate == 16000)
	{
		eSampleRate = AEC_SAMPLERATE_16KHZ;
	}
	else
	{
		eSampleRate = AEC_SAMPLERATE_8KHZ;
	}
#else
	/* WBS, NBS use same volume */
	//eSampleRate = AEC_SAMPLERATE_8KHZ;			  /* Useless Assignment */
#endif	

	if(ucIndex>AECIf_GetSpeakerMaximumVolumeIndex())
	{
		ucIndex=AECIf_GetSpeakerMaximumVolumeIndex();
	}
	
	for(ucTmpIndex=0;ucTmpIndex<AECIf_GetSpeakerMaximumVolumeIndex();ucTmpIndex++)
	{
		if(gstAECIfSpeakerVolumeTable[eSampleRate][ucTmpIndex].ucIndex==ucIndex)
			break;
	}
	
	return gstAECIfSpeakerVolumeTable[eSampleRate][ucTmpIndex].uiGain;
}
//#endif

_static_t int AECIf_GetSpeakerNominalVolumeIndex(void)
{
	tAECParams *pstParams=gpvAECIfParamsBase;

	if(pstParams==NULL)
		return 0;
		
	return pstParams->iSpeakerNominalVolumeIndex;
}

//_static_t int AECIf_GetSpeakerMaximumVolumeIndex(void)
int AECIf_GetSpeakerMaximumVolumeIndex(void)
{
	tAECParams *pstParams=gpvAECIfParamsBase;

	if(pstParams==NULL)
		return 0;
		
	return pstParams->iSpeakerMaximumVolumeIndex;
}

/*_static_t*/ _uchar_t AECIf_GetSpeakerCurrentVolumeIndex(void)
{
	return 0;
//		_uchar_t ucDevNo;
//	
//		ucDevNo=BtApp_HF_GetCurrDevNum();
//		return gBtAppHfSpkVolIndex[ucDevNo];
//	//	return gucAECIfSpeakerCurrentVolumeIndex;
}

_void_t AECIf_SetSpeakerCurrentVolumeIndex(_uchar_t ucIndex)
{
#ifdef __arm__
	tAECParams *pstParams=gpvAECIfParamsBase;

	if(pstParams==NULL)
		return;

	if(ucIndex>AECIf_GetSpeakerMaximumVolumeIndex())
	{
		ucIndex=AECIf_GetSpeakerMaximumVolumeIndex();
	}

	/* NES */
	if(pstParams->iNesGainScale==0)
	{
		AECParams_SetDynamicNesGainScale(ucIndex);
	}
	else
	{
		AECParams_SetNesGainScale(pstParams->iNesGainScale);
	}

	/* RES */
	if(pstParams->iResGainScale==0)
	{
		AECParams_SetDynamicResGainScale(ucIndex);
	}
	else
	{
		AECParams_SetResGainScale(pstParams->iResGainScale);
	}

	/* FES */
	if(pstParams->iFesGainScale==0)
	{
		/* none */
	}
	else
	{
		AECParams_SetFesGainScale(pstParams->iFesGainScale);
	}
#else
	return;
#endif
}

#if 0	/* reserved */
_void_t AECIf_SetSpeakerCurrentVolumeGain(_uint_t uiGain)
{
	_char_t cIndex;

	uiGain= uiGain & 0xffff;
	
	for(cIndex=AECIf_GetSpeakerMaximumVolumeIndex();cIndex>=0;cIndex--)
	{
		if(uiGain>=gstAECIfSpeakerVolumeTable[cIndex].uiGain)
		{
			AECIf_SetSpeakerCurrentVolumeIndex(cIndex);
			break;
		}
	}
}
#endif

//#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_VOLUME	/* reserved */
_uint_t AECIf_GetMicrophoneVolumeGain(_uchar_t ucIndex)
{
	tAECParams *pstParams=gpvAECIfParamsBase;
	_uchar_t ucTmpIndex;
	eAECSampleRate eSampleRate = AEC_SAMPLERATE_8KHZ;

	if(pstParams==NULL)
	{
		return 0;
	}

#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS	
	/* WBS, NBS use different volume : for Test */
	if(pstParams->iSampleRate == 24000)
	{
		eSampleRate = AEC_SAMPLERATE_24KHZ;
	}
	else if(pstParams->iSampleRate == 16000)
	{
		eSampleRate = AEC_SAMPLERATE_16KHZ;
	}
	else
	{
		eSampleRate = AEC_SAMPLERATE_8KHZ;
	}
#else
	/* WBS, NBS use same volume */
	//eSampleRate = AEC_SAMPLERATE_8KHZ;		/* Useless Assignment */
#endif

	if(ucIndex>AECIf_GetMicrophoneMaximumVolumeIndex())
	{
		ucIndex=AECIf_GetMicrophoneMaximumVolumeIndex();
	}

	for(ucTmpIndex=0;ucTmpIndex<AECIf_GetMicrophoneMaximumVolumeIndex();ucTmpIndex++)
	{
		if(gstAECIfMicrophoneVolumeTable[eSampleRate][ucTmpIndex].ucIndex==ucIndex)
			break;
	}

	return gstAECIfMicrophoneVolumeTable[eSampleRate][ucTmpIndex].uiGain;
}
//#endif

_static_t int AECIf_GetMicrophoneNominalVolumeIndex(void)
{
	tAECParams *pstParams=gpvAECIfParamsBase;

	if(pstParams==NULL)
		return 0;
		
	return pstParams->iMicrophoneNominalVolumeIndex;
}

int AECIf_GetMicrophoneMaximumVolumeIndex(void)
//_static_t int AECIf_GetMicrophoneMaximumVolumeIndex(void)
{
	tAECParams *pstParams=gpvAECIfParamsBase;

	if(pstParams==NULL)
		return 0;
		
	return pstParams->iMicrophoneMaximumVolumeIndex;
}

_uchar_t AECIf_GetMicrophoneCurrentVolumeIndex(void)
{
	return 0;
//		_uchar_t ucDevNo;
//	
//		ucDevNo=BtApp_HF_GetCurrDevNum();
//	
//		return gBtAppHfMicGainIndex[ucDevNo];
//	//	return gucAECIfMicrophoneCurrentVolumeIndex;
}

_void_t AECIf_SetMicrophoneCurrentVolumeIndex(_uchar_t ucIndex)
{
	tAECParams *pstParams=gpvAECIfParamsBase;

	//gBtAppHfMicGainIndex[ucDevNo]=ucIndex;
	//gucAECIfMicrophoneCurrentVolumeIndex=ucIndex;

	if(pstParams==NULL)
		return;	

#if 1	/* Unused value */
	if(ucIndex>AECIf_GetMicrophoneMaximumVolumeIndex())
	{
		/*gBtAppHfMicGainIndex[ucDevNo]=*/ucIndex=AECIf_GetMicrophoneMaximumVolumeIndex();
	}
	
	/* reserved */
#endif	
}

#if defined(AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS)
/* DEQ Params */
_static_t _void_t AECIf_GetProfileString(_char_t *Section,_char_t *Key,_char_t *Default,_char_t *pstrValue)
{
	AECUtil_GetPrivateProfileString(Section,Key,Default,pstrValue,256,NULL);	
}

/* Params Except DEQ */
_static_t _void_t AECIf_GetProfileValue(_char_t *Section,_char_t *Key,_char_t *Default,_int_t *piValue)
{
	_char_t strValue[256]={0,};

	AECUtil_GetPrivateProfileString(Section,Key,Default,strValue,256,NULL);
	*piValue=AECUtil_Atoi(strValue);	
}

_static_t _void_t AECIf_SetExternalParameters(tAECParams *pstParams)
{
	/* UFD */
	if(pstParams->boPathEnable == TRUE)
	{
		if(AECUtil_OpenProfile((_char_t *)pstParams->cIniPath, TRUE) == FALSE)
		{
			printf("%s : UFD Read Error", __func__);
			return;
		}
	}
	else
	{
		if(AECUtil_OpenProfile("AECif_Params.ini", FALSE) == FALSE)
		{
			printf("%s : UFD Read Error", __func__);
			return;
		}
	}

	AECIf_SetProfileValues(pstParams);			

	AECUtil_CloseProfile();
}


_static_t _void_t AECIf_SetProfileValues(tAECParams *pstParams)
{
	int iValue = 0;

	/* BYPASS */
	AECIf_GetProfileValue("AEC Options","Enable","1",&iValue);
	pstParams->boBypass= (iValue==0) ? TRUE : FALSE;		/* Bypass the acoustic echo canceller and the noise reduction */

	/* DUMP */
	AECIf_GetProfileValue("Dump Files","Enable","0",&iValue);
	pstParams->boDump=iValue;

	AECIf_GetProfileValue("Dump Files","PathEnable","0",&iValue);
	pstParams->boPathEnable = iValue;
	/* designate path to store dump files */
	AECIf_GetProfileString("Dump Files", "NBPath", "./NB", (_char_t *)pstParams->cNBDumpPath);
	AECIf_GetProfileString("Dump Files", "WBPath", "./WB", (_char_t *)pstParams->cWBDumpPath);
	AECIf_GetProfileString("Dump Files", "SWBPath", "./SWB", (_char_t *)pstParams->cSWBDumpPath);

	pstParams->ulDumpOptions=0;
	AECIf_GetProfileValue("Dump Files","RCV","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_RCV);
	AECIf_GetProfileValue("Dump Files","RAG","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_RAG);
	AECIf_GetProfileValue("Dump Files","RDR","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_RDR);
	AECIf_GetProfileValue("Dump Files","RDQ","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_RDQ);
	
	AECIf_GetProfileValue("Dump Files","FES","1",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_FES);
	
	AECIf_GetProfileValue("Dump Files","MIC","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_MIC);
	AECIf_GetProfileValue("Dump Files","LPA","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_LPA);
	
	AECIf_GetProfileValue("Dump Files","NES","1",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_NES);
	AECIf_GetProfileValue("Dump Files","RES","1",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_RES);
	AECIf_GetProfileValue("Dump Files","FOS","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_FOS);		
	
	AECIf_GetProfileValue("Dump Files","NST","0",&iValue);
	if(iValue==1)
		pstParams->ulDumpOptions|=AEC_DUMP_ON(AEC_DUMP_NST);
	AECIf_GetProfileValue("Dump Files","Time","20",&iValue);

	/* Dump Sec */
	pstParams->ulDumpSec=iValue;

	/* SAMPLE */
	AECIf_GetProfileValue("External Sample File - Receiving Direction","Enable","0",&iValue);
	pstParams->boUseExternalSamplesFarIn=iValue;

	/*RCV Volume Control - Receiving direction */
	AECIf_GetProfileValue("RCV Gain Control Option - Receiving Direction", "Enable", "0", &iValue);
	pstParams->boUseRCVGainCtrl = iValue;
	AECIf_GetProfileValue("RCV Gain Control Option - Receiving Direction", "in_scale", "0", &iValue);
	pstParams->in_scale_RCVGainCtrl = iValue;

	/*DEQ Volume Control - Receiving direction */
	AECIf_GetProfileValue("DEQ Gain Control Option - Receiving Direction", "Enable", "0", &iValue);
	pstParams->boUseDEQGainCtrl = iValue;
	AECIf_GetProfileValue("DEQ Gain Control Option - Receiving Direction", "in_scale", "0", &iValue);
	pstParams->in_scale_DEQGainCtrl = iValue;

	/*NES Volume Control - Receiving direction */
	AECIf_GetProfileValue("NES Gain Control Option - Sending Direction", "Enable", "0", &iValue);
	pstParams->boUseNESGainCtrl = iValue;
	AECIf_GetProfileValue("NES Gain Control Option - Sending Direction", "in_scale", "0", &iValue);
	pstParams->in_scale_NESGainCtrl = iValue;

	/*RES Volume Control - Receiving direction */
	AECIf_GetProfileValue("RES Gain Control Option - Sending Direction", "Enable", "0", &iValue);
	pstParams->boUseRESGainCtrl = iValue;
	AECIf_GetProfileValue("RES Gain Control Option - Sending Direction", "in_scale", "0", &iValue);
	pstParams->in_scale_RESGainCtrl = iValue;

	/* AGC - Receiving direction */
    AECIf_GetProfileValue("AGC Options - Receiving Direction","Enable","0",&iValue);
    pstParams->boUseAGCFarIn = iValue;
    AECIf_GetProfileValue("AGC Options - Receiving Direction","Threshold","0",&iValue);
    pstParams->iThrdBAGCFarIn = iValue;
    AECIf_GetProfileValue("AGC Options - Receiving Direction","Attenuation","0",&iValue);
    pstParams->iAttdBAGCFarIn = iValue;
    AECIf_GetProfileValue("AGC Options - Receiving Direction","StepSizeAtt","2",&iValue);
    pstParams->iStepSizeAttAGCFarIn = iValue;	
    AECIf_GetProfileValue("AGC Options - Receiving Direction","StepSizeRel","2",&iValue);
    pstParams->iStepSizeRelAGCFarIn = iValue;	

	/* DRC - Receiving direction */
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Enable","0",&iValue);
	pstParams->boUseDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Threshold","0",&iValue);
	pstParams->iThrDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Ratio","10",&iValue);
	pstParams->iRatioDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Knee","10",&iValue);
	pstParams->iKneeDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Attack","10",&iValue);
	pstParams->iAttkDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","Release","100",&iValue);
	pstParams->iRelDRCFarIn = iValue;
	AECIf_GetProfileValue("DRC Options - Receiving Direction","LimThrd","100",&iValue);
	pstParams->iLimDRCFarIn = iValue;

	/* DEQ - Receiving direction */
	AECIf_GetProfileValue("DEQ Options - Receiving Direction","Enable","0",&iValue);
	pstParams->boUseDEQFarIn=iValue;

	/* DEQ - Sending Direction */
	AECIf_GetProfileValue("DEQ Options - Sending Direction","Enable","0",&iValue);
	pstParams->boUseDEQFarOut=iValue;

	/* EchoDelay */	
	AECIf_GetProfileValue("Echo Delay","NBDelayTime","0",&iValue);	
	pstParams->ulNBEchoDelayMilliSec=iValue;
	AECIf_GetProfileValue("Echo Delay","WBDelayTime","0",&iValue);	
	pstParams->ulWBEchoDelayMilliSec=iValue;
	AECIf_GetProfileValue("Echo Delay","SWBDelayTime","0",&iValue);	
	pstParams->ulSWBEchoDelayMilliSec=iValue;
	AECIf_GetProfileValue("Echo Delay","SRCDelayTime","0",&iValue);	
	pstParams->ulSRCEchoDelayMilliSec=iValue;
	AECIf_GetProfileValue("Echo Delay", "InitEnable", "0", &iValue);
	pstParams->boUseInitEcho = iValue;
	AECIf_GetProfileValue("Echo Delay", "InitEchoIntvN", "100", &iValue);
	pstParams->iInitEchoIntvN = iValue;
	AECIf_GetProfileValue("Echo Delay", "InitEchoIntvP", "100", &iValue);
	pstParams->iInitEchoIntvP = iValue;

	AECIf_GetProfileValue("Dummy Sound - Receiving Direction","DummyTime","0",&iValue);	
	pstParams->ulDummySoundMilliSec=iValue;

	/*psAecHandle->m_sAecParams structure member setting */
	AECIf_GetProfileValue("AEC Options","NoiseSuppressdB","-12",&iValue);
	pstParams->iNoiseSuppressdB=iValue;
	
	AECIf_GetProfileValue("Dynamic Noise Suppression - Sending Direction","Enable","0",&iValue);
	pstParams->boUseDynamicNoiseSuppressdB=iValue;
	
	AECIf_GetProfileValue("Dynamic Noise Suppression - Sending Direction","Threshold","80000",&iValue);
	pstParams->iDynamicNoiseSuppressdBThreshold=iValue;	
	AECIf_GetProfileValue("AEC Options","EchoSuppressdB","-50",&iValue);
	pstParams->iEchoSuppressdB=iValue;
	AECIf_GetProfileValue("AEC Options","EchoSuppressActivedB","-25",&iValue);
	pstParams->iEchoSuppressActivedB=iValue;
													   
	AECIf_GetProfileValue("AEC Options","NesGainScale","0",&iValue);
	pstParams->iNesGainScale=iValue;				/* Q10 scaling, before AEC microphone gain 
													   ex. ( (int)(0.5f*32768) )
													*/
	AECIf_GetProfileValue("AEC Options","FesGainScale","0",&iValue);
	pstParams->iFesGainScale=iValue;				/* reserved */

	AECIf_GetProfileValue("AEC Options","ResGainScale","0",&iValue);
	pstParams->iResGainScale=iValue;				/* Q10 scaling, after AEC processed result gain 
													   ex. ( (int)(2.0f*32768) )
													*/
	/* NB */
	AECIf_GetProfileValue("AEC Options","NBDeclineFreqBinHz","250",&iValue);
	pstParams->uiNBDeclineFreqBinHz=iValue;		/* Threshold of frequency band to reduce level of RES. Unit(Hz) */
	AECIf_GetProfileValue("AEC Options","NBLfePowerSuppress","20",&iValue);
	pstParams->iNBLfePowerSuppress=iValue;			/* The reduction of the low frequency band level */

	/* WB */
	AECIf_GetProfileValue("AEC Options","WBDeclineFreqBinHz","250",&iValue);
	pstParams->uiWBDeclineFreqBinHz=iValue;		/* Threshold of frequency band to reduce level of RES. Unit(Hz) */
	AECIf_GetProfileValue("AEC Options","WBLfePowerSuppress","20",&iValue);
	pstParams->iWBLfePowerSuppress=iValue;			/* The reduction of the low frequency band level */

	/* SWB */	
	AECIf_GetProfileValue("AEC Options","SWBDeclineFreqBinHz","250",&iValue);
	pstParams->uiSWBDeclineFreqBinHz=iValue;		/* Threshold of frequency band to reduce level of RES. Unit(Hz) */
	AECIf_GetProfileValue("AEC Options","SWBLfePowerSuppress","20",&iValue);
	pstParams->iSWBLfePowerSuppress=iValue;			/* The reduction of the low frequency band level */

	AECIf_GetProfileValue("AEC Options","LTSEFrame","6",&iValue);
	pstParams->iLTSEFrame=iValue;				/* LTSE Frame */
	AECIf_GetProfileValue("AEC Options","HangOverFlag","0",&iValue);
	pstParams->iHangoverFlag=iValue;				/* Hangover Flag */

	AECIf_GetProfileValue("AEC Options","NNSGainTargetQValue","11",&iValue);
	pstParams->iNNSGainTargetQValue=iValue;				/* The NNS gain control, 1~14 */

	AECIf_GetProfileValue("AEC Options","CNGLevel","100",&iValue);
	pstParams->iCNGLevel=iValue;							/* The CNG Level inside AEC, 0~100 */
	
	AECIf_GetProfileValue("AEC Options","ControlOptions","0",&iValue);
	pstParams->iControlOptions=iValue;		

	AECIf_GetProfileValue("AEC Options","iAmpGain","100",&iValue);
	pstParams->iAmpGain=iValue;						/* The amplification of the echo free sound after the AEC process */
	AECIf_GetProfileValue("AEC Options", "OpMode", "0", &iValue);
	pstParams->uiLibOpMode = iValue;	
	AECIf_GetProfileValue("Speaker Volume - Receiving Direction","NorminalIndex","11",&iValue);
	pstParams->iSpeakerNominalVolumeIndex=iValue;			/* The nominal volume index */
	AECIf_GetProfileValue("AEC Options","NorminalVolumeNesGainScale","90",&iValue);
	pstParams->iSpeakerNominalVolumeNesGainScale=iValue;	/* The reduction of the sound to the microphone with the nominal volume */
	AECIf_GetProfileValue("Speaker Volume - Receiving Direction","MaximumIndex","15",&iValue);
	pstParams->iSpeakerMaximumVolumeIndex=iValue;			/* The maximum volume index */
	AECIf_GetProfileValue("AEC Options","MaximumVolumeNesGainScale","30",&iValue);
	pstParams->iSpeakerMaximumVolumeNesGainScale=iValue;	/* The reduction of the sound to the microphone with the maximum volume */	

	AECIf_GetProfileValue("Microphone Volume - Sending Direction","NorminalIndex","11",&iValue);
	pstParams->iMicrophoneNominalVolumeIndex=iValue;		/* The nominal volume index */
	AECIf_GetProfileValue("Microphone Volume - Sending Direction","MaximumIndex","15",&iValue);
	pstParams->iMicrophoneMaximumVolumeIndex=iValue;			/* The maximum volume index */
													
	/* DEQ table */
	{
		int iSampleRateIndex,iIndex=0;
		_char_t strKey[10];
		_char_t strValue[256];
		_char_t strTemp[5];

		/* RECEIVING */
		for(iSampleRateIndex=0; iSampleRateIndex < AEC_SAMPLERATE_MAX; iSampleRateIndex++)
		{
			for(iIndex=0;iIndex<DEQ_MAX_NUM_OF_BAND;iIndex++)
			{
				sprintf(strKey,"Ch%02d",iIndex);

				if(iSampleRateIndex == AEC_SAMPLERATE_24KHZ)
				{
					AECIf_GetProfileString("DEQ SWBS - Receiving Direction",strKey,"1000,09,1,+00",strValue);
				}
				else if(iSampleRateIndex == AEC_SAMPLERATE_16KHZ)
				{
					AECIf_GetProfileString("DEQ WBS - Receiving Direction",strKey,"1000,09,1,+00",strValue);
				}
				else
				{
					AECIf_GetProfileString("DEQ - Receiving Direction",strKey,"1000,09,1,+00",strValue);
				}
				
				strTemp[0]=strValue[0];
				strTemp[1]=strValue[1];
				strTemp[2]=strValue[2];
				strTemp[3]=strValue[3];
				strTemp[4]='\0';
				gstAECIfParamsDEQFarIn[iSampleRateIndex][iIndex].Fc=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[5];
				strTemp[1]=strValue[6];
				strTemp[2]='\0';
				gstAECIfParamsDEQFarIn[iSampleRateIndex][iIndex].Q=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[8];
				strTemp[1]='\0';
				gstAECIfParamsDEQFarIn[iSampleRateIndex][iIndex].fil_type=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[10];
				strTemp[1]=strValue[11];
				strTemp[2]=strValue[12];
				strTemp[3]='\0';			
				gstAECIfParamsDEQFarIn[iSampleRateIndex][iIndex].BandGain=AECUtil_Atoi(strTemp);
			}

			/* SENDING */
			for(iIndex=0;iIndex<DEQ_MAX_NUM_OF_BAND;iIndex++)
			{
				sprintf(strKey,"Ch%02d",iIndex);

				if(iSampleRateIndex == AEC_SAMPLERATE_24KHZ)
				{
					AECIf_GetProfileString("DEQ SWBS - Sending Direction",strKey,"1000,09,1,+00",strValue);
				}
				else if(iSampleRateIndex == AEC_SAMPLERATE_16KHZ)
				{
					AECIf_GetProfileString("DEQ WBS - Sending Direction",strKey,"1000,09,1,+00",strValue);
				}
				else
				{
					AECIf_GetProfileString("DEQ - Sending Direction",strKey,"1000,09,1,+00",strValue);
				}				
				
				strTemp[0]=strValue[0];
				strTemp[1]=strValue[1];
				strTemp[2]=strValue[2];
				strTemp[3]=strValue[3];
				strTemp[4]='\0';
				gstAECIfParamsDEQFarOut[iSampleRateIndex][iIndex].Fc=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[5];
				strTemp[1]=strValue[6];
				strTemp[2]='\0';
				gstAECIfParamsDEQFarOut[iSampleRateIndex][iIndex].Q=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[8];
				strTemp[1]='\0';
				gstAECIfParamsDEQFarOut[iSampleRateIndex][iIndex].fil_type=AECUtil_Atoi(strTemp);
				strTemp[0]=strValue[10];
				strTemp[1]=strValue[11];
				strTemp[2]=strValue[12];
				strTemp[3]='\0';			
				gstAECIfParamsDEQFarOut[iSampleRateIndex][iIndex].BandGain=AECUtil_Atoi(strTemp);
			}
		}		
	}
	
	/* VOLUME table */
#ifdef AEC_VDA_HFT_MEASUREMENT_EXTERNAL_VOLUME	/* reserved */
	{
		int iIndex=0;
		_char_t strKey[10];
		int iMaximumVolumeIndex;
		
		iMaximumVolumeIndex=AECIf_GetSpeakerMaximumVolumeIndex()+1;
		if(iMaximumVolumeIndex>AEC_SPK_VOL_30+1)
			iMaximumVolumeIndex=AEC_SPK_VOL_30+1;

		for(iIndex=0;iIndex<iMaximumVolumeIndex;iIndex++)
		{
			sprintf(strKey,"Gain%02d",iIndex);
			/* NBS */
			AECIf_GetProfileValue("Speaker Volume - Receiving Direction",strKey,"0",&iValue);
			gstAECIfSpeakerVolumeTable[AEC_SAMPLERATE_8KHZ][iIndex].uiGain=iValue;
			/* WBS */
			AECIf_GetProfileValue("Speaker Volume WBS - Receiving Direction",strKey,"0",&iValue);
			gstAECIfSpeakerVolumeTable[AEC_SAMPLERATE_16KHZ][iIndex].uiGain=iValue;			
			/* 24KHZ */
			AECIf_GetProfileValue("Speaker Volume SWBS - Receiving Direction",strKey,"0",&iValue);
			gstAECIfSpeakerVolumeTable[AEC_SAMPLERATE_24KHZ][iIndex].uiGain=iValue;			
		}

		iMaximumVolumeIndex=AECIf_GetMicrophoneMaximumVolumeIndex()+1;
		if(iMaximumVolumeIndex>AEC_MIC_VOL_30+1)
			iMaximumVolumeIndex=AEC_MIC_VOL_30+1;
			
		for(iIndex=0;iIndex<iMaximumVolumeIndex;iIndex++)
		{
			sprintf(strKey,10,"Gain%02d",iIndex);
			/* NBS */
			AECIf_GetProfileValue("Microphone Volume - Sending Direction",strKey,"0",&iValue);
			gstAECIfMicrophoneVolumeTable[AEC_SAMPLERATE_8KHZ][iIndex].uiGain=iValue;			
			/* WBS */
			AECIf_GetProfileValue("Microphone Volume WBS - Sending Direction",strKey,"0",&iValue);
			gstAECIfMicrophoneVolumeTable[AEC_SAMPLERATE_16KHZ][iIndex].uiGain=iValue;			
			/* 24KHZ */
			AECIf_GetProfileValue("Microphone Volume SWBS - Sending Direction",strKey,"0",&iValue);
			gstAECIfMicrophoneVolumeTable[AEC_SAMPLERATE_24KHZ][iIndex].uiGain=iValue;			
		}
	}
#endif
}

#endif	/* AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS */


/* SampleRate, FrameSize, FilterSize must not set in ini params */
_void_t AECIf_SetSampleRate(tAECParams *pstParams,eAECSampleRate eSampleRate)
{
	switch(eSampleRate)
	{
		case AEC_SAMPLERATE_24KHZ :
			pstParams->iSampleRate=8000*3;
			pstParams->iFrameSize=128*3;
			pstParams->iFilterSize=800*3;
			break;
		case AEC_SAMPLERATE_16KHZ :
			pstParams->iSampleRate=8000*2;
			pstParams->iFrameSize=128*2;
			pstParams->iFilterSize=800*2;
			break;
		case AEC_SAMPLERATE_8KHZ :
			pstParams->iSampleRate=8000;
			pstParams->iFrameSize=128;
			pstParams->iFilterSize=800;
			break;
		default : /*AEC_SAMPLERATE_48KHZ*/	/* test */
			pstParams->iSampleRate=8000*6;
			pstParams->iFrameSize=128*6;
			pstParams->iFilterSize=800*6;
			break;
	}
}

_static_t _void_t AECIf_SetRealParameters(_char_t cMode,tAECParams *pstParams,tAECParamsDEQ *pstDEQFarIn,tAECParamsDEQ *pstDEQFarOut)
{
#ifdef __arm__
//		_uchar_t ucDevNo;
//	
//		ucDevNo=BtApp_HF_GetCurrDevNum();

	/* init. */
	gpvAECIfParamsBase=pstParams;
	
	pstParams->ulDEQFarIn=(unsigned long)pstDEQFarIn; 
	pstParams->ulDEQFarOut=(unsigned long)pstDEQFarOut; 

	AECParams_SetParams(pstParams);

	/* params. */
	AECParams_SetBypass((cMode == 1/*voice*/) ? FALSE/*TRUE*/ : pstParams->boBypass);	/* Bypass the acoustic echo canceller and the noise reduction */

	AECParams_SetDump(pstParams->boDump
							,pstParams->ulDumpOptions
							,pstParams->ulDumpSec);

	AECParams_SetUseExternalSamples(pstParams->boUseExternalSamplesFarIn);		/* Output the PCM stream like the sine wave using a file instead of the real sound.*/

	AECParams_SetDummySoundMilliSec((cMode == 1/*voice*/) ? 0 : pstParams->ulDummySoundMilliSec);		/* The number of invalid dummy samples */

	
	AECParams_SetUseDEQFarIn(pstParams->boUseDEQFarIn
							,pstParams->ulDEQFarIn
							,pstParams->iDEQNumFarIn);						/* the enhancement of the sound */
	AECParams_SetUseDEQFarOut(pstParams->boUseDEQFarOut
							,pstParams->ulDEQFarOut
							,pstParams->iDEQNumFarOut);						/* the enhancement of the sound */

	/* AEC lib. parameters */
	AECParams_SetSpeakerVolumeIndex(AECIf_GetSpeakerNominalVolumeIndex()			/* The nominal volume index */
									,AECIf_GetSpeakerMaximumVolumeIndex());			/* The maximum volume index */
	AECParams_SetMicrophoneVolumeIndex(AECIf_GetMicrophoneNominalVolumeIndex()		/* The nominal volume index */
									,AECIf_GetMicrophoneMaximumVolumeIndex());		/* The maximum volume index */	
	AECParams_SetAmpGainScale(pstParams->iAmpGain						/* The amplification of the echo free sound after the AEC process */
							,pstParams->iSpeakerNominalVolumeNesGainScale		/* The reduction of the sound to the microphone on the norminal volume */
							,pstParams->iSpeakerMaximumVolumeNesGainScale);	/* The reduction of the sound to the microphone on the maximum volume */


	AECParams_SetNoiseSuppressDB(pstParams->iNoiseSuppressdB);					/* 0 : bypass */
	AECParams_SetDynamicNoiseSuppressDB(pstParams->boUseDynamicNoiseSuppressdB,pstParams->iDynamicNoiseSuppressdBThreshold
							,pstParams->iDynamicMinimumNoiseSuppressdB,pstParams->iDynamicRepeatCount);
	AECParams_SetEchoSuppressDB(pstParams->iEchoSuppressdB);						/* 0 : bypass */
	AECParams_SetEchoSuppressActiveDB(pstParams->iEchoSuppressActivedB);			/* 0 : bypass */
	
	if(pstParams->iSampleRate == 8000)
		AECParams_SetNBLfePowerSuppress(pstParams->uiNBDeclineFreqBinHz
				,pstParams->iNBLfePowerSuppress);				/* 20% : the reduction of the low frequency band level */
	else if(pstParams->iSampleRate == 16000) 
		AECParams_SetWBLfePowerSuppress(pstParams->uiWBDeclineFreqBinHz
				,pstParams->iWBLfePowerSuppress);				/* 20% : the reduction of the low frequency band level */
	else if(pstParams->iSampleRate == 24000)
		AECParams_SetSWBLfePowerSuppress(pstParams->uiSWBDeclineFreqBinHz
				,pstParams->iSWBLfePowerSuppress);				/* 20% : the reduction of the low frequency band level */
	else
		AECParams_SetNBLfePowerSuppress(pstParams->uiNBDeclineFreqBinHz
				,pstParams->iNBLfePowerSuppress);				/* 20% : the reduction of the low frequency band level */

	AECParams_SetFilterSize(pstParams->iFilterSize);								/* Size of one frame */
	
	AECParams_SetSampleRate(pstParams->iSampleRate);								/* Sampling rates */
	
	AECParams_SetFrameSize(pstParams->iFrameSize);								/* The number of filter taps */

	AECParams_SetLTSEFrame(pstParams->iLTSEFrame);								/* LTSE Frame */
	
	AECParams_SetHangoverFlag(pstParams->iHangoverFlag);							/* Hangover Flag */
	
	AECParams_SetNNSGainTargetQvalue(pstParams->iNNSGainTargetQValue);			/* NNS level */

	AECParams_SetControlOptions(pstParams->iControlOptions);

	AECParams_SetSpeakerVolume(AECIf_GetSpeakerCurrentVolumeIndex());

	AECParams_SetMicrophoneVolume(AECIf_GetMicrophoneCurrentVolumeIndex());
	
	if(pstParams->iNesGainScale==0)
	{
		AECParams_SetDynamicNesGainScale(AECIf_GetSpeakerCurrentVolumeIndex());
	}
	else
	{
		AECParams_SetNesGainScale(pstParams->iNesGainScale);
	}
	
	if(pstParams->iResGainScale==0)
	{
		AECParams_SetDynamicResGainScale(AECIf_GetSpeakerCurrentVolumeIndex());
	}
	else
	{
		AECParams_SetResGainScale(pstParams->iResGainScale);
	}

	/* FES Gain is reserved */
	if(pstParams->iFesGainScale==0)
	{
		/* none */
	}
	else
	{
		AECParams_SetFesGainScale(pstParams->iFesGainScale);
	}
#endif
}

/*
	uiMode : 0 - Call, 1 - Voice Recognition, 2 - ring
*/
_void_t AECIf_SetParameters(_char_t cMode,_int_t iSampleRate)
{	
	/* 1. Load Default Values */
	gstAECIfParams = gstAECIfParamsDefault;

	/* 2. Copy to pointer */
	gpvAECIfParamsBase=&gstAECIfParams;

#if defined(AEC_VDA_HFT_MEASUREMENT_EXTERNAL_PARAMETERS)
	AECIf_SetExternalParameters(gpvAECIfParamsBase);	// UFD
#endif

	iSampleRate=iSampleRate/8000-1;
	if(iSampleRate<0)
	{	
		iSampleRate=0;
	}
	
	AECIf_SetSampleRate(gpvAECIfParamsBase,iSampleRate);

	AECIf_SetRealParameters(cMode,gpvAECIfParamsBase,gstAECIfParamsDEQFarIn[iSampleRate],gstAECIfParamsDEQFarOut[iSampleRate]);
}
#endif	/* BT_AEC_INCLUDE */
#endif /* arm */

/* End Of File */
