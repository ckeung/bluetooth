/****************************************************************************
 *   FileName    : TC_BT_NV_DB_FILE.h
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
*  Description : This file contains functions and variables for a sc_db application
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    13/12/17     0.1            created                     Kwon Sunhong
*******************************************************************************/

#ifndef __TC_BT_NV_DB_FILE_H__
#define __TC_BT_NV_DB_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtDataType.h"
/*****************************************************************************
*
* miscellaneous 
*
*****************************************************************************/

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#ifndef SIZE_LINK_KEY
#define SIZE_LINK_KEY   				0x10
#endif

#define SC_DB_FILE_NAME 				"NV_DB.NDB"
#define SC_DB_NAME_SIZE					20

#define SC_DB_MAX_NUM_OF_RECORD_INFO	20

#define COAL_BIT_SET(val, bit)      ((val) |= (1UL << (bit)))
#define COAL_BIT_IS_SET(val, bit)   ((unsigned char) ((((val) & (1UL << (bit))) != 0)))

#define TC_BT_SC_DB_RECORD 			TcBtSecDevRecrd
#define TC_BT_SC_DB_READ_FIRST 		TC_BT_NV_DB_ReadFirst
#define TC_BT_SC_DB_READ_NEXT	 	TC_BT_NV_DB_ReadNext
#define TC_BT_SC_DB_REMOVE	 		TC_BT_NV_DB_Remove
#define TC_BT_SC_DB_REMOVE_ALL	 	TC_BT_NV_DB_RemoveAll

/*****************************************************************************
*
* data types
*
*****************************************************************************/
/* TODO: Add your specialized code here */

/*****************************************************************************
*
* structures
*
*****************************************************************************/
 
typedef struct
{
	TcBtBdAddr		deviceAddr; /* BdAddr of remote device */
	TcBtUint32		classOfDevice;
	TcBtUint8		Linkkey[SIZE_LINK_KEY];
	TcBtBool		linkkeyValid;
	TcBtUint8		linkkeyType;
	TcBtUint8		io_cap;
    TcBtUint8 		remoteName[TC_BT_MAX_FRIENDLY_NAME_LEN + 1];
	TcBtUint32		knownServices11_00_31;
	TcBtUint32		knownServices11_32_63;
	TcBtUint32		knownServices12_00_31;
	TcBtUint32		knownServices13_00_31;
} TcBtSecDevRecrd;

#define TC_BT_NV_DB_FILE_VERSION      ((unsigned long) (0xFFFF0001))

typedef struct
{
	unsigned int 			Version;
	TcBtSecDevRecrd			theRecoder[SC_DB_MAX_NUM_OF_RECORD_INFO];
	unsigned int 			Index;
	unsigned char			Mod;
#ifdef BT_AUTO_CONNECTION
	TcBtBdAddr 				LastDevAddr;
#endif
 } TcBtSecDB;

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

extern char* TC_BT_SC_DB_GetDBPath(void);
extern void TC_BT_SC_DB_InitScDb(void);
extern void TC_BT_SC_DB_DeInitScDb(void);
extern void TC_BT_SC_DB_UpdateScDb(void);
extern TcBtBdAddr* TC_BT_SC_DB_ReadLastDevAddr(void);
extern void TC_BT_SC_DB_WriteLastDevAddr(const TcBtBdAddr* BdAddr);
extern unsigned char TC_BT_SC_DB_CheckSupportProfile(const TcBtBdAddr* BdAddr, unsigned short usUUID);
extern void TC_BT_SC_DB_UpdateSupportProfile(const TcBtBdAddr* BdAddr, unsigned short usUUID);
extern int TC_BT_SC_DB_IsFull(void);
extern int TC_BT_SC_DB_ToTheTop(const TcBtBdAddr *pBdAddr);
extern unsigned char TC_BT_NV_DB_Read(const TcBtBdAddr *theAddr , TcBtSecDevRecrd *theRecord);
extern void TC_BT_NV_DB_Write(const TcBtBdAddr *theAddr, const TcBtSecDevRecrd *theRecord);
extern void TC_BT_NV_DB_Remove(const TcBtBdAddr *theAddr);
extern void TC_BT_NV_DB_RemoveAll(void);
extern unsigned char TC_BT_NV_DB_ReadFirst(TcBtSecDevRecrd *theRecord);
extern unsigned char TC_BT_NV_DB_ReadNext(TcBtSecDevRecrd *theRecord);
extern unsigned char TC_BT_SC_DB_PrintLinkKey(const TcBtBdAddr *pBdAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_NV_DB_FILE_H__ */

