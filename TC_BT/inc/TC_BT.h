/****************************************************************************
 *   FileName    : TC_BT.h
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
*    10/05/31     0.1            created                     Ji, Seung-Nam
*    10/07/08     0.2            event msg was added         Ji, Seung-Nam
*    10/07/31     0.3            HF SCO was added            Ji, Seung-Nam
*    10/08/31     0.4            HF DEV was added            Ji, Seung-Nam
*    10/08/31     0.5            HF UTIL was added           Ji, Seung-Nam
*    11/10/12     0.6            supported TCLinkV4          Hwang, Jin Ho.
*    12/04/23     0.7            supported CodecSupport      Park, Young-Soo.
*
*******************************************************************************/

#ifndef __TC_BT_H__
#define __TC_BT_H__

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
/* TODO: Add your specialized code here */
#include "inc/BtDataType.h"


/*****************************************************************************
*
* debug
*
*****************************************************************************/
/* Log enable define */
#define SYS_LOG
#define GAP_LOG
#define HF_LOG
#define A2DP_LOG
#define PBAP_LOG
//#define MAP_LOG
//#define SPP_LOG
//#define HIDD_LOG
//#define PAN_LOG

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_H__ */
