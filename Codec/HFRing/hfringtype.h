/****************************************************************************
 *   FileName    : hfringtype.h
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

#ifndef __HF_RING_TYPE_H__
#define __HF_RING_TYPE_H__

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
#define _extern_t		extern
#define _static_t		static
#define _void_t 		void
#define _const_t		const
#define _inline_t		__inline

#define _char_t			char
#define _uchar_t		unsigned char
#define _short_t		short
#define _ushort_t		unsigned short
#define _int_t			int
#define _uint_t			unsigned int
#define _long_t			long
#define _ulong_t		unsigned long

#define _float_t		float
#define _double_t		double

#define _bool_t			signed int

typedef size_t			_size_t;
typedef ssize_t			_ssize_t;

typedef _void_t*		_handle_t;

typedef int				_fhandle_t;

#ifndef NULL
#define NULL			((void*)0)
#endif

#ifndef  FALSE
#define  FALSE          0
#endif

#ifndef  TRUE
#define  TRUE           1
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
* externs
*
*****************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __AEC_TYPE_H__ */
