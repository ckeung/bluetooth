/****************************************************************************
 *   FileName    : aecparams.h
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

#ifndef __AEC_PARAMS_H__
#define __AEC_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/
#ifndef  FALSE
#define  FALSE                     0
#endif

#ifndef  TRUE
#define  TRUE                      1
#endif

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
/* parameters */
typedef enum
{
	AEC_PARAMS_SET_BYPASS = 0,
	AEC_PARAMS_SET_DUMP,
	AEC_PARAMS_SET_USE_EXTERNAL_SAMPLES_FARIN,
	AEC_PARAMS_SET_USE_DEQ_FARIN,
	AEC_PARAMS_SET_USE_LPF_FARIN,
	AEC_PARAMS_SET_USE_DEQ_FAROUT,
	AEC_PARAMS_SET_USE_LPF_FAROUT,
	AEC_PARAMS_SET_USE_CNG_FAROUT,
	AEC_PARAMS_SET_USE_AGC_FAROUT,
	AEC_PARAMS_SET_ECHO_DELAY_MILLISEC,
	AEC_PARAMS_SET_DUMMY_SOUND_MILLISEC,
	AEC_PARAMS_SET_SPEAKER_VOLUME_INDEX,
	AEC_PARAMS_SET_MICROPHONE_VOLUME_INDEX,
	AEC_PARAMS_SET_AMP_GAIN_SCALE,
	AEC_PARAMS_SET_NOISESUPPRESSDB,
	AEC_PARAMS_SET_DYNAMICNOISESUPPRESSDB,
	AEC_PARAMS_SET_ECHOSUPPRESSDB,
	AEC_PARAMS_SET_ECHOSUPPRESSACTIVEDB,
	AEC_PARAMS_SET_NES_GAIN_SCALE,
	AEC_PARAMS_SET_FES_GAIN_SCALE,
	AEC_PARAMS_SET_RES_GAIN_SCALE,
	AEC_PARAMS_SET_LFEPOWERSUPPRESS,
	AEC_PARAMS_SET_FRAMESIZE,
	AEC_PARAMS_SET_SAMPLERARE,
	AEC_PARAMS_SET_FILTERSIZE,
	AEC_PARAMS_SET_LTSEFRAME,
	AEC_PARAMS_SET_HANGOVERFLAG,
	AEC_PARAMS_SET_NNS_GAINTARGET_QVALUE,
	AEC_PARAMS_SET_CNG_LEVEL,
	AEC_PARAMS_SET_CONTROLOPTIONS,
	AEC_PARAMS_SET_CALI_DUMMY_SOUND_MILLISEC,

	AEC_PARAMS_GET_SPEAKER_NOMINAL_VOLUME_INDEX,
	AEC_PARAMS_GET_SPEAKER_MAXIMUM_VOLUME_INDEX,
	AEC_PARAMS_GET_MICROPHONE_NOMINAL_VOLUME_INDEX,
	AEC_PARAMS_GET_MICROPHONE_MAXIMUM_VOLUME_INDEX,
	AEC_PARAMS_GET_AMP_GAIN_SCALE,
#ifdef 	__TCC_ANDROID__
    AEC_PARAMS_GET_ECHO_DELAY_MILLISEC,	
#endif//__TCC_ANDROID__
	AEC_PARAMS_GET_SPEAKER_NOMINAL_VOLUME_INPUT_GAIN_SCALE,
	AEC_PARAMS_GET_SPEAKER_MAXIMUM_VOLUME_INPUT_GAIN_SCALE,
	AEC_PARAMS_GET_NOISESUPPRESSDB,
	AEC_PARAMS_GET_ECHOSUPPRESSDB,
	AEC_PARAMS_GET_ECHOSUPPRESSACTIVEDB	
} eAECParams;

/*****************************************************************************
*
* structures
*
*****************************************************************************/
typedef struct
{
	_int_t	Fc;									/* Center Frequency */
	_uint_t Q;									/* Q factor */
	_int_t	fil_type;							/* Filter type */
	_int_t	BandGain;							/* Current Band Gain */
} tAECParamsDEQ;

/* AEC parameters */
typedef struct
{
	/* AEC application */
	_uchar_t boBypass;							/* Bypass the acoustic echo canceller and the noise reduction */
	
	_uchar_t boDump;							/* Dump streams in the last several seconds to files, which are 16bits 8khz mono pcm files.
													   With this option, a user can inspect them using the advanced software application 
													   like Cool Editor Pro or GoldWave.
												*/	
	_ulong_t ulDumpOptions;				
	_ulong_t ulDumpSec;
	
	_uchar_t boUseExternalSamplesFarIn;			/* Output the PCM stream like the sine wave using a file instead of the real sound.
												   	   With this option, a user can measure the time delay between an input frame and an echo frame
												   	   as well as the quality of the sound.
											   	       Refer to the definition, AEC_PCM_ECHO_SYNC_SAMPLES. 
											   	*/
													   
	_uchar_t boUseDEQFarIn;						/* The enhancement of the sound to the loudspeaker using the dynamic EQ filter. */
	_ulong_t ulDEQFarIn; 
	_int_t iDEQNumFarIn; 
		
	_uchar_t boUseLPFFarIn;						/* The enhancement of the sound to the loudspeaker using the low pass filter. */
	_int_t iLPFFarInCenterFreq;					/* The center frequency of the low pass filter */
	_int_t iLPFFarInWBSCenterFreq;
	_int_t iLPFFarInSWBSCenterFreq;
	_int_t iLPFFarInSampleRate;					/* The sample rate of the low pass filter */

	_uchar_t boUseDEQFarOut;					/* The enhancement of the sound to the microphone using the dynamic EQ filter. */
	_ulong_t ulDEQFarOut; 
	_int_t iDEQNumFarOut; 

	_uchar_t boUseLPFFarOut;					/* The enhancement of the sound to the microphone using the low pass filter. */
	_int_t iLPFFarOutCenterFreq;				/* The center frequency of the low pass filter */
	_int_t iLPFFarOutWBSCenterFreq;
	_int_t iLPFFarOutSWBSCenterFreq;
	_int_t iLPFFarOutSampleRate;				/* The sample rate of the low pass filter */
		
	_uchar_t boUseCNGFarOut;					/* The enhancement of the sound to the microphone using the CNG filter. */
	_int_t iCNGFarOutNoiseLevel;				/*	noise_level : 0 ~ 10, 0 is minimum, 10 is maximum, 5 is default */
	_int_t iCNGFarOutThreshold;					/*	energy_threshold : criterion of decision for generate noise in this frame, 50000 is appropriate */
	_int_t iCNGFarOutFilterType;

	_uchar_t boUseAGCFarOut;					/* The enhancement of the sound to the microphone using the AGC filter. */
	_int_t iAGCFarOutNoiseLevel;						
	_int_t iAGCFarOutMaxOutValue;						
	_uchar_t boAGCFarOutVAD;						
	
	_ulong_t ulEchoDelayMilliSec; 				/* The time of the sound delay between the loud speaker and the microphone. 
												   A user may measure the echo delay with the above two parameters
												   ,boDump and boUseExternalSamplesFarIn.
												*/
#ifdef 	__TCC_ANDROID__
    _ulong_t ulEchoDelayMilliSec16K;
	_ulong_t ulEchoDelayMilliSec24K;
#endif

	_ulong_t ulDummySoundMilliSec;				/* Disable the output of the sound to the loudspeaker for a specified period of time 
												   after initializing AEC. 
												   A user can throw away the invalid sound like a pop noise with it.
												   Refer to the definition, AEC_PCM_DUMMY_SAMPLES.
												*/

	/*psAecHandle->m_sAecParams structure member setting */
	_int_t iNoiseSuppressdB;
	_uchar_t boUseDynamicNoiseSuppressdB;
	_int_t iDynamicNoiseSuppressdBThreshold;
	_int_t iDynamicMaximumNoiseSuppressdB;
	_int_t iDynamicMinimumNoiseSuppressdB;
	_int_t iDynamicRepeatCount;
	
	_int_t iEchoSuppressdB;
	_int_t iEchoSuppressActivedB;
													   
	_int_t iNesGainScale;						/* Q10 scaling, before AEC microphone gain ex. ( (_int_t)(0.5f*1024) ) */
	_int_t iFesGainScale;						/* Q10 scaling, SPK gain : reserved! */
	_int_t iResGainScale;						/* Q10 scaling, after AEC processed result gain ex. ( (_int_t)(2.0f*1024) ) */
													
	_uint_t uiDeclineFreqBinHz;					/* Threshold of frequency band to reduce level of RES. Unit(Hz), Maximum is 300Hz */
	_int_t iLfePowerSuppress;					/* Q10 scaling, reduce low frequency band level ex. ( (_int_t)(2.0f*1024) ) */


	_int_t iFrameSize;							/* The framesize, 160 samples */
	_int_t iSampleRate;							/* The samplerate, 8000Hz or 16000hz */
	_int_t iFilterSize;							/* The filtersize, 800 or 1200 */

	_int_t iLTSEFrame;							/* LTSE Frame */
	_int_t iHangoverFlag;						/* Hangover Flag */
	_int_t iNNSGainTargetQValue;				/* NNS level : 1~14, smaller value makes noise lower */
	_int_t iCNGLevel;							/* CNG level inside AEC : 0~100 */													   
	_int_t iControlOptions;		

	_int_t iAmpGain;							/* The amplification of the echo free sound after the AEC process */
	_int_t iSpeakerNominalVolumeIndex;			/* The nominal volume index */
	_int_t iSpeakerNominalVolumeNesGainScale;	/* The reduction of the sound to the microphone with the nominal volume */
	_int_t iSpeakerMaximumVolumeIndex;			/* The maximum volume index */
	_int_t iSpeakerMaximumVolumeNesGainScale;	/* The reduction of the sound to the microphone with the maximum volume */

	_int_t iMicrophoneNominalVolumeIndex;		/* The nominal volume index */
	_int_t iMicrophoneMaximumVolumeIndex;		/* The maximum volume index */	
	_ulong_t ulCaliDummySoundMilliSec;			/* Reserved */
} tAECParams;

/*****************************************************************************
*
* globals
*
*****************************************************************************/
_extern_t tAECParams gstAECParams;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
_extern_t _void_t AECParams_SetSpeakerVolume(_uchar_t ucVol);
_extern_t _void_t AECParams_SetMicrophoneVolume(_uchar_t ucVol);
_extern_t _void_t AECParams_SetBypass(_uchar_t boEnable);
_extern_t _void_t AECParams_SetDump(_uchar_t boEnable,_ulong_t ulOptions,_ulong_t ulSec);
_extern_t _void_t AECParams_SetUseExternalSamples(_uchar_t boEnable);
_extern_t _void_t AECParams_SetUseDEQFarIn(_uchar_t boEnable,_ulong_t ulDEQ,_int_t iNum);
_extern_t _void_t AECParams_SetUseLPFFarIn(_uchar_t boEnable,_int_t iCenterFreq,_int_t iCenterFreqWBS,_int_t iCenterFreqSWBS,_int_t iSampleRate);
_extern_t _void_t AECParams_SetUseDEQFarOut(_uchar_t boEnable,_ulong_t ulDEQ,_int_t iNum);
_extern_t _void_t AECParams_SetUseLPFFarOut(_uchar_t boEnable,_int_t iCenterFreq,_int_t iCenterFreqWBS,_int_t iCenterFreqSWBS,_int_t iSampleRate);
_extern_t _void_t AECParams_SetUseCNGFarOut(_uchar_t boEnable,_int_t iNoiseLevel,_int_t iThreshold,_int_t iFilterType);
_extern_t _void_t AECParams_SetUseAGCFarOut(_uchar_t boEnable,_int_t iNoiseLevel,_int_t iMaxOutValue,_uchar_t boVAD);
#ifdef __TCC_ANDROID__
_extern_t _void_t AECParams_SetEchoDelayMilliSec(_ulong_t ulMilliSec,_ulong_t ulMilliSec16K,_ulong_t ulMilliSec24K);
#else
_extern_t _void_t AECParams_SetEchoDelayMilliSec(_ulong_t ulMilliSec);
#endif//__TCC_ANDROID__
_extern_t _void_t AECParams_SetDummySoundMilliSec(_ulong_t ulMilliSec);
_extern_t _void_t AECParams_SetSpeakerVolumeIndex(_int_t iSpeakerNominalVolumeIndex,_int_t iSpeakerMaximumVolumeIndex);
_extern_t _int_t AECParams_GetSpeakerNominalVolumeIndex(_void_t);
_extern_t _int_t AECParams_GetSpeakerMaximumVolumeIndex(_void_t);
_extern_t _void_t AECParams_SetMicrophoneVolumeIndex(_int_t iMicrophoneNominalVolumeIndex,_int_t iMicrophoneMaximumVolumeIndex);
_extern_t _int_t AECParams_GetMicrophoneNominalVolumeIndex(_void_t);
_extern_t _int_t AECParams_GetMicrophoneMaximumVolumeIndex(_void_t);
_extern_t _void_t AECParams_SetAmpGainScale(_int_t iAmpGain,_int_t iSpeakerNominalVolumeNesGainScale,_int_t iSpeakerMaximumVolumeNesGainScale);
_extern_t _int_t AECParams_GetAmpGainScale(_void_t);
_extern_t _int_t AECParams_GetSpeakerNominalVolumeNesGainScale(_void_t);
_extern_t _int_t AECParams_GetSpeakerMaximumVolumeNesGainScale(_void_t);
_extern_t _void_t AECParams_SetNoiseSuppressDB(_int_t iDB);
_extern_t _void_t AECParams_SetDynamicNoiseSuppressDB(_uchar_t boEnable,_int_t iThreshold,_int_t iMinimumdB,_int_t iRepeatCount);
_extern_t _int_t AECParams_GetNoiseSuppressDB(_void_t);
_extern_t _void_t AECParams_SetEchoSuppressDB(_int_t iDB);
_extern_t _int_t AECParams_GetEchoSuppressDB(_void_t);
_extern_t _int_t AECParams_GetEchoSuppressActiveDB(_void_t);
_extern_t _void_t AECParams_SetEchoSuppressActiveDB(_int_t iDB);
_extern_t _void_t AECParams_SetNesGainScale(_int_t iScale);
_extern_t _void_t AECParams_SetFesGainScale(_int_t iScale);
_extern_t _void_t AECParams_SetResGainScale(_int_t iScale);
_extern_t _void_t AECParams_SetDynamicNesGainScale(_int_t iCurVol);
_extern_t _void_t AECParams_SetDynamicResGainScale(_int_t iCurVol);
_extern_t _int_t AECParams_GetDynamicNesGainScale(_int_t iCurVol);
_extern_t _void_t AECParams_SetLfePowerSuppress(_int_t iFreq,_int_t iScale);
_extern_t _void_t AECParams_SetFilterSize(_int_t iFilterSize);
_extern_t _void_t AECParams_SetSampleRate(_int_t iSampleRate);
_extern_t _void_t AECParams_SetFrameSize(_int_t iFrameSize);
_extern_t _void_t AECParams_SetLTSEFrame(_int_t iLTSEFrame);
_extern_t _void_t AECParams_SetHangoverFlag(_int_t iHangoverFlag);
_extern_t _void_t AECParams_SetNNSGainTargetQvalue(_int_t iQValue);
_extern_t _void_t AECParams_SetCNGLevel(_int_t iQValue);
_extern_t _void_t AECParams_SetControlOptions(_int_t iOptions);
_extern_t _void_t AECParams_SetCaliDummySoundMilliSec(_ulong_t ulMilliSec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __AEC_PARAMS_H__ */
