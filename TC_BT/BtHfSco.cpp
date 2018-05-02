/****************************************************************************
 *   FileName    : TC_BT_HF_SCO.c
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
*  Description : This file contains functions for a Bluetooth HF SCO
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/31     0.1            created                     Ji, Seung-Nam
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*
*******************************************************************************/
#include "BT_CFG.h"

#include "TC_BT.h"
#include "TC_BT_HF_SCO.h"
#ifdef BT_HANDSFREE_UNIT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "BtUtil.h"

#ifndef HF_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

static void *pTC_BT_HF_SCO_ENTRY_LIST = NULL;
static TcBtUint8 ucTcBtScoEntryListSize = 0;

/*****************************************************************************
*
* debug
*
*****************************************************************************/

TcBtBdAddr *TC_BT_HF_SCO_GetBdAddr(TcBtUint16 scoHandle)
{
	int i;
    TcBtHfScoEntry *pEntry = (TcBtHfScoEntry *)pTC_BT_HF_SCO_ENTRY_LIST;

	for(i=0; i < ucTcBtScoEntryListSize; i++)
	{
		if (pEntry[i].scoHandle == scoHandle)
		{
			return (&(pEntry[i].bdAddr));
		}
	}

	return NULL;
}

int TC_BT_HF_SCO_Init(TcBtUint8 maxConnections)
{
	int i;
	TcBtHfScoEntry *pEntry;

	(void)TC_BT_HF_SCO_Deinit();
	
   pTC_BT_HF_SCO_ENTRY_LIST = TC_BT_Pmalloc(maxConnections * sizeof(TcBtHfScoEntry));
    pEntry = (TcBtHfScoEntry *)pTC_BT_HF_SCO_ENTRY_LIST;
	if(pEntry!=NULL)
	{
		ucTcBtScoEntryListSize = maxConnections;

		for(i=0; i < ucTcBtScoEntryListSize; i++)
		{
			TC_BT_BdAddrZero(&(pEntry[i].bdAddr));
			pEntry[i].scoHandle = TC_BT_HF_NO_SCO_HANDLE;
		}

		return TC_BT_HF_SCO_RESULT_SUCCESS;
	}
	else
	{
		return TC_BT_HF_SCO_RESULT_FAIL;
	}
}

int TC_BT_HF_SCO_Deinit(void)
{
	if(pTC_BT_HF_SCO_ENTRY_LIST!=NULL)
	{
		TC_BT_Pfree(pTC_BT_HF_SCO_ENTRY_LIST);
		pTC_BT_HF_SCO_ENTRY_LIST = NULL;
		ucTcBtScoEntryListSize = 0;
	}

	return TC_BT_HF_SCO_RESULT_SUCCESS;
}

int TC_BT_HF_SCO_DataCallback(TcBtUint8 *theData)
{
	TcBtUint16 scoHandle;

	scoHandle = ((TcBtUint16)theData[1] << 4) << 8;
	scoHandle |= theData[0];
    return 1;
    //return BtApp_HF_ScoDataCallback(TC_BT_HF_SCO_GetBdAddr(scoHandle), theData);
}

int TC_BT_HF_SCO_RegisterHandle(const TcBtBdAddr *pBdAddr, TcBtUint16 scoHandle)
{
	int i;
    TcBtHfScoEntry *pEntry = (TcBtHfScoEntry *)pTC_BT_HF_SCO_ENTRY_LIST;

	/* update */
	for(i=0; i < ucTcBtScoEntryListSize; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(pEntry[i].bdAddr), (const TcBtBdAddr *)pBdAddr)!=0)
		{
			TC_BT_BdAddrCopyIfExist(&(pEntry[i].bdAddr), (const TcBtBdAddr *)pBdAddr);
			pEntry[i].scoHandle = scoHandle;

            printf("[%s : %05d] Info : TC_BT_HF_SCO_RegisterHandle(TC_BT_BdAddrEq) : addr : %xh,%xh,%xh : scohandle : %d\n",__FILE__,__LINE__,pBdAddr->lap,pBdAddr->uap,pBdAddr->nap,scoHandle);
			return TC_BT_HF_SCO_RESULT_SUCCESS;
		}
	}

	/* new */
	for(i=0; i < ucTcBtScoEntryListSize; i++)
	{
		if (TC_BT_BdAddrEqZero((const TcBtBdAddr *)&(pEntry[i].bdAddr))!=0)
		{
			TC_BT_BdAddrCopyIfExist(&(pEntry[i].bdAddr), (const TcBtBdAddr *)pBdAddr);
			pEntry[i].scoHandle = scoHandle;

            printf("[%s : %05d] Info : TC_BT_HF_SCO_RegisterHandle(TC_BT_BdAddrEqZero) : addr : %xh,%xh,%xh : scohandle : %d\n",__FILE__,__LINE__,pBdAddr->lap,pBdAddr->uap,pBdAddr->nap,scoHandle);
			return TC_BT_HF_SCO_RESULT_SUCCESS;
		}
	}

    printf("[%s : %05d] Info : TC_BT_HF_SCO_RegisterHandle : TC_BT_HF_SCO_RESULT_FAIL",__FILE__,__LINE__);
	return TC_BT_HF_SCO_RESULT_FAIL;
}

int TC_BT_HF_SCO_DeregisterHandle(const TcBtBdAddr *pBdAddr, TcBtUint16 scoHandle)
{
	int i;
    TcBtHfScoEntry *pEntry = (TcBtHfScoEntry*)pTC_BT_HF_SCO_ENTRY_LIST;

	for(i=0; i < ucTcBtScoEntryListSize; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(pEntry[i].bdAddr), (const TcBtBdAddr *)pBdAddr)!=0)
		{
            printf("[%s : %05d] Info : TC_BT_HF_SCO_DeregisterHandle : addr : %xh,%xh,%xh : scohandle : %d\n",__FILE__,__LINE__,pBdAddr->lap,pBdAddr->uap,pBdAddr->nap,scoHandle);
			TC_BT_BdAddrZero(&(pEntry[i].bdAddr));
			pEntry[i].scoHandle = TC_BT_HF_NO_SCO_HANDLE;

			return TC_BT_HF_SCO_RESULT_SUCCESS;
		}
	}

    printf("[%s : %05d] Info : TC_BT_HF_SCO_DeregisterHandle : TC_BT_HF_SCO_RESULT_FAIL\n",__FILE__,__LINE__);
	return TC_BT_HF_SCO_RESULT_FAIL;
}
#else	/* BT_HANDSFREE_UNIT */
/* It's referred from BT library. To fix build error when remove BT_HANDSFREE_UNIT */
int TC_BT_HF_SCO_DataCallback(TcBtUint8 *theData)
{
	return TC_BT_HF_SCO_RESULT_FAIL;
}
#endif	/* BT_HANDSFREE_UNIT */
