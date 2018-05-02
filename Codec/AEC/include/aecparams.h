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
	AEC_PARAMS_SET_USE_DEQ_FAROUT,
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
	AEC_PARAMS_SET_NBLFEPOWERSUPPRESS,
	AEC_PARAMS_SET_WBLFEPOWERSUPPRESS,
	AEC_PARAMS_SET_SWBLFEPOWERSUPPRESS,
	AEC_PARAMS_SET_FRAMESIZE,
	AEC_PARAMS_SET_SAMPLERARE,
	AEC_PARAMS_SET_FILTERSIZE,
	AEC_PARAMS_SET_LTSEFRAME,
	AEC_PARAMS_SET_HANGOVERFLAG,
	AEC_PARAMS_SET_NNS_GAINTARGET_QVALUE,
	AEC_PARAMS_SET_CONTROLOPTIONS,

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
	
	_uchar_t boDump;
    _uchar_t boPathEnable;
    _uchar_t cIniPath[256];
    _uchar_t cNBDumpPath[256];
    _uchar_t cWBDumpPath[256];
    _uchar_t cSWBDumpPath[256];
	_ulong_t ulDumpOptions;				
	_ulong_t ulDumpSec;
	
	_uchar_t boUseExternalSamplesFarIn;	

    /* Gain Control */
    _uchar_t boUseRCVGainCtrl;
    _int_t in_scale_RCVGainCtrl;

    _uchar_t boUseDEQGainCtrl;
    _int_t in_scale_DEQGainCtrl;

    _uchar_t boUseNESGainCtrl;
    _int_t in_scale_NESGainCtrl;

    _uchar_t boUseRESGainCtrl;
    _int_t in_scale_RESGainCtrl;

    /* Receiving direction */
    _uchar_t boUseAGCFarIn;                     /* Auto Gain Control in Receiving direction */
    _int_t iThrdBAGCFarIn;
    _int_t iAttdBAGCFarIn;
    _int_t iStepSizeAttAGCFarIn;
    _int_t iStepSizeRelAGCFarIn;

    _uchar_t boUseDRCFarIn;                     /* Dynamic Range Control in Receiving direction */
    _int_t iThrDRCFarIn;
    _float_t iRatioDRCFarIn;
    _int_t iKneeDRCFarIn;
    _int_t iAttkDRCFarIn;
    _int_t iRelDRCFarIn;
    _int_t iLimDRCFarIn;

    _uchar_t boUseDEQFarIn;                     /* Dynamic Equalizer in Receiving direction */
	_ulong_t ulDEQFarIn; 
	_int_t iDEQNumFarIn; 

    _uchar_t boUseLPFFarIn;
    _int_t iLPFFarInCenterFreq;
    _int_t iLPFFarInWBSCenterFreq;
    _int_t iLPFFarInSWBSCenterFreq;
    _int_t iLPFFarInSampleRate;

    /* Sending direction */
	_uchar_t boUseDEQFarOut;                    /* Dynamic Equalizer in Sending direction */
	_ulong_t ulDEQFarOut; 
	_int_t iDEQNumFarOut; 

    _uchar_t boUseLPFFarOut;
    _int_t iLPFFarOutCenterFreq;
    _int_t iLPFFarOutWBSCenterFreq;
    _int_t iLPFFarOutSWBSCenterFreq;
    _int_t iLPFFarOutSampleRate;

    /* Echo Delay */
	_ulong_t ulNBEchoDelayMilliSec;
    _ulong_t ulWBEchoDelayMilliSec;
	_ulong_t ulSWBEchoDelayMilliSec;
	_ulong_t ulSRCEchoDelayMilliSec;
    _uchar_t boUseInitEcho;
    _int_t iInitEchoIntvN;
    _int_t iInitEchoIntvP;

	_ulong_t ulDummySoundMilliSec;

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
			
/* NB */    
	_uint_t uiNBDeclineFreqBinHz;		/* Threshold of frequency band to reduce level of RES. Unit(Hz), Maximum is 300Hz */
	_int_t iNBLfePowerSuppress;			/* Q10 scaling, reduce low frequency band level ex. ( (_int_t)(2.0f*1024) ) */

/* WB */
	_uint_t uiWBDeclineFreqBinHz;		/* Threshold of frequency band to reduce level of RES. Unit(Hz), Maximum is 300Hz */
	_int_t iWBLfePowerSuppress;			/* Q10 scaling, reduce low frequency band level ex. ( (_int_t)(2.0f*1024) ) */

/* SWB */
	_uint_t uiSWBDeclineFreqBinHz;		/* Threshold of frequency band to reduce level of RES. Unit(Hz), Maximum is 300Hz */
	_int_t iSWBLfePowerSuppress;		/* Q10 scaling, reduce low frequency band level ex. ( (_int_t)(2.0f*1024) ) */
	
    _int_t iFrameSize;							/* The framesize, 160 samples */
	_int_t iSampleRate;							/* The samplerate, 8000Hz or 16000hz */
	_int_t iFilterSize;							/* The filtersize, 800 or 1200 */

	_int_t iLTSEFrame;							/* LTSE Frame */
	_int_t iHangoverFlag;						/* Hangover Flag */
	_int_t iNNSGainTargetQValue;				/* NNS level : 1~14, smaller value makes noise lower */
	_int_t iCNGLevel;							/* CNG level inside AEC : 0~100 */													   
	_int_t iControlOptions;		

	_int_t iAmpGain;							/* The amplification of the echo free sound after the AEC process */
    _uint_t uiLibOpMode;                        /* Library Operation Mode for test Library */
	_int_t iSpeakerNominalVolumeIndex;			/* The nominal volume index */
	_int_t iSpeakerNominalVolumeNesGainScale;	/* The reduction of the sound to the microphone with the nominal volume */
	_int_t iSpeakerMaximumVolumeIndex;			/* The maximum volume index */
	_int_t iSpeakerMaximumVolumeNesGainScale;	/* The reduction of the sound to the microphone with the maximum volume */

	_int_t iMicrophoneNominalVolumeIndex;		/* The nominal volume index */
	_int_t iMicrophoneMaximumVolumeIndex;		/* The maximum volume index */	

} tAECParams;

/*****************************************************************************
*
* globals
*
*****************************************************************************/
_extern_t tAECParams *gstAECParams;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
_extern_t _void_t AECParams_SetParams(tAECParams *AECParams);
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
_extern_t _void_t AECParams_SetEchoDelayMilliSec(_ulong_t ulMilliSec);
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
_extern_t _void_t AECParams_SetNBLfePowerSuppress(_int_t iFreq,_int_t iScale);
_extern_t _void_t AECParams_SetWBLfePowerSuppress(_int_t iFreq,_int_t iScale);
_extern_t _void_t AECParams_SetSWBLfePowerSuppress(_int_t iFreq,_int_t iScale);
_extern_t _void_t AECParams_SetFilterSize(_int_t iFilterSize);
_extern_t _void_t AECParams_SetSampleRate(_int_t iSampleRate);
_extern_t _void_t AECParams_SetFrameSize(_int_t iFrameSize);
_extern_t _void_t AECParams_SetLTSEFrame(_int_t iLTSEFrame);
_extern_t _void_t AECParams_SetHangoverFlag(_int_t iHangoverFlag);
_extern_t _void_t AECParams_SetNNSGainTargetQvalue(_int_t iQValue);
_extern_t _void_t AECParams_SetCNGLevel(_int_t iQValue);
_extern_t _void_t AECParams_SetControlOptions(_int_t iOptions);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __AEC_PARAMS_H__ */
