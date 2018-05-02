/****************************************************************************
 *   FileName    : aecutil.h
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

#ifndef __AEC_UTIL_H__
#define __AEC_UTIL_H__

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
/* TODO: Add your specialized code here */

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
_extern_t _uchar_t gucAECUtilboUsePCToolParam;
_extern_t _uchar_t gucAECUtilboCloseCallByPCTool;

_extern_t _void_t *AECUtil_Memset(_void_t *pDes, _int_t iValue, _uint_t uiSize);
_extern_t _void_t *AECUtil_Memcpy(_void_t *pDes, _const_t _void_t *pSrc, _uint_t uiSize);
_extern_t _char_t AECUtil_OpenProfile(_char_t *lpFileName, unsigned int PathEnable);
_extern_t _void_t AECUtil_CloseProfile(void);
_extern_t _char_t AECUtil_OpenPCToolProfile(_const_t _char_t *buffer, _uint_t length);
_extern_t _void_t AECUtil_ClosePCToolProfile(void);
_extern_t _int_t AECUtil_GetPrivateProfileString(_char_t *pAppName,_char_t *pKeyName,_char_t *pDefault,_char_t *pReturnedString,_int_t nSize,_char_t *pcFileName);
_extern_t _int_t AECUtil_Atoi(_const_t _char_t *s);

_extern_t _char_t AECUtil_JitterOutputHandler(_char_t cBufferIndex);
_extern_t _char_t AECUtil_JitterInputHandler(_char_t cBufferIndex);
_extern_t _char_t AECUtil_JitterTolerance(_void_t *pvBase);
_extern_t _char_t AECUtil_JitterAdjust(_void_t *pvBase);
_extern_t _uint_t AECUtil_JitterDMASlotBufferSize(void);
_extern_t _uint_t AECUtil_JitterDMAOutputBufferSize(void);
_extern_t _uint_t AECUtil_JitterDMAInputBufferSize(void);
_extern_t _char_t AECUtil_JitterMode(void);
_extern_t _void_t AECUtil_JitterFlush(_void_t *pvBase);
_extern_t _void_t AECUtil_JitterOpen(_void_t *pvBase,_ushort_t usTargetSampleRate,_ushort_t usBaseSampleRate,_ushort_t usSlotLength
								,_ushort_t usEchoSyncLength,_uint_t uiDMAPageSize);
_extern_t _void_t AECUtil_JitterClose(_void_t *pvBase);
_extern_t _void_t AECUtil_JitterSetBuffer(_void_t *pvOutputBuffer,_void_t *pvInputBuffer);
_extern_t _void_t AECUtil_JitterSetCallback(_char_t (*pfIsActive)(_void_t),_void_t (*pfFlush)(_char_t boReset),_char_t boReset
								,_uchar_t **pTxBuf,_uchar_t **pRxBuf
								,_ulong_t *pulTxDaCsar,_ulong_t *pulRxDaCdar);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __AEC_UTIL_H__ */
