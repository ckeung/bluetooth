/****************************************************************************
 *   FileName    : aecerr.h
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

#ifndef __AEC_ERR_H__
#define __AEC_ERR_H__

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

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* errors */
/* errors */
typedef enum
{
	AEC_ERR_NOERR = 0,
	AEC_ERR_BUFFER_LACK,
	AEC_ERR_BUFFER_FULL,
	AEC_ERR_FAILED
} eAECErr;

//	typedef enum
//	{
//		AEC_ERR_NOERR,
//		AEC_ERR_BUFFER_NOT_READY,
//		AEC_ERR_BUFFER_LACK,
//		AEC_ERR_BUFFER_FULL,	
//		AEC_ERR_FAILED
//	} eAECErr;

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __XXX_H__ */
