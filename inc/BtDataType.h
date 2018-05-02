/****************************************************************************
 *   FileName    : TC_BT_DATATYPE.h
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
*  Description : This file contains functions for a Telechips Bluetooth
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    13/11/04     0.1            created                     Kwon Sunhong
*
*******************************************************************************/

#ifndef __TC_BT_DATATYPE_H__
#define __TC_BT_DATATYPE_H__

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
typedef char               TcBtInt8;
typedef unsigned char      TcBtUint8;
typedef short int          TcBtInt16;
typedef unsigned short int TcBtUint16;
typedef long int           TcBtInt32;
typedef unsigned long int  TcBtUint32;

typedef TcBtUint32         TcBtUint24;
typedef TcBtUint8          TcBtBool;

/* Telechips' remark */
typedef TcBtInt8		   TcBtCharString;
typedef TcBtUint32         TcBtConnId;

typedef TcBtUint8          TcBtString;

/* UUID types */
typedef TcBtUint32		   TcBtUuid32;
typedef TcBtUint8		   TcBtUuid128[16];

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */
#ifndef  FALSE
#define  FALSE			0
#endif

#ifndef  TRUE
#define  TRUE			1
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef TC_BT_MAX_FRIENDLY_NAME_LEN
#define TC_BT_MAX_FRIENDLY_NAME_LEN    (50)
#endif /* TC_BT_MAX_FRIENDLY_NAME_LEN */

/* Arm env standard font */
#define DEFAULT_FONT_FAMILY		"NanumGothic"

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    TcBtUint24 lap; /* Lower Address Part 00..23 */
    TcBtUint8  uap; /* upper Address Part 24..31 */
    TcBtUint16 nap; /* Non-significant    32..47 */
} TcBtBdAddr;

typedef struct
{
	TcBtUint8	type;
	TcBtBdAddr	addr;
} TcBtTypedAddr;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_DATATYPE_H__ */


