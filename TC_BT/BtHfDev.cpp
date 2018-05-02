/****************************************************************************
 *   FileName    : TC_BT_HF_DEV.c
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
*  Description : This file contains functions for a Bluetooth HF Device
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/08/31     0.1            created                     Ji, Seung-Nam
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*
*******************************************************************************/
#include "BT_CFG.h"

#if defined(BT_HANDSFREE_UNIT)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "TC_BT.h"
#include "BtUtil.h"
#include "TC_BT_HF_CMD.h"
#include "TC_BT_HF_DEV.h"
#ifndef HF_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

static void *pTC_BT_HF_DEV_LIST = NULL;
static TcBtUint8 ucTcBtHfDevListSize = 0;
static TcBtUint32 ulTcBtHfDevCurrNum = 0;

/*****************************************************************************
*
* debug
*
*****************************************************************************/

TcBtBdAddr *TC_BT_HF_DEV_GetBdAddr(const TcBtUint32 selectedNum)
{
    TcBtBdAddr *pEntry = (TcBtBdAddr *)pTC_BT_HF_DEV_LIST;

	if(pEntry==NULL)
	{
		return NULL;
	}
	
	if(selectedNum>=BT_HFP_MAX_CONNECTIONS)
	{
		return NULL;
	}
	
	return &(pEntry[selectedNum]);
}

/*************************************************************************************
*  TC_BT_HF_DEV_GetCurrentNum
*-------------------------------------------------------------------------------------
* Description  : Function calls from TC_BT parts
* Parameters   : 
* Return	   : 
**************************************************************************************/
TcBtUint32 TC_BT_HF_DEV_GetCurrentNum(void)
{
	return ulTcBtHfDevCurrNum;
}

void TC_BT_HF_DEV_SetCurrentNum(TcBtUint32 i)
{
	 ulTcBtHfDevCurrNum=i;
}

TcBtInt32 TC_BT_HF_DEV_GetNum(const TcBtBdAddr *pBdAddr)
{
	TcBtUint32 i;	
    TcBtBdAddr *pEntry = (TcBtBdAddr *)pTC_BT_HF_DEV_LIST;
	TcBtInt32 iDevNo=-1;

	if(pEntry==NULL)
	{
		return iDevNo;
	}
	
	for(i=0; i < ucTcBtHfDevListSize; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(pEntry[i]), (const TcBtBdAddr *)pBdAddr)!=0)
		{
			iDevNo=i;
			break;
		}
	}

	return iDevNo;
}

int TC_BT_HF_DEV_SetNum(const TcBtBdAddr *pBdAddr)
{
	TcBtUint32 i;
    TcBtBdAddr *pEntry = (TcBtBdAddr *)pTC_BT_HF_DEV_LIST;

	if(pEntry==NULL)
	{
		return TC_BT_HF_DEV_RESULT_FAIL;
	}
	
	for(i=0; i < ucTcBtHfDevListSize; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(pEntry[i]), (const TcBtBdAddr *)pBdAddr)!=0)
		{
			if(ulTcBtHfDevCurrNum != (i))
			{
				ulTcBtHfDevCurrNum = (i);

				//PRINTF("ulTcBtHfDevCurrNum == %d", ulTcBtHfDevCurrNum);
			}

			return TC_BT_HF_DEV_RESULT_SUCCESS;
		}
	}

	return TC_BT_HF_DEV_RESULT_FAIL;
}

int TC_BT_HF_DEV_Init(TcBtUint8 maxConnections)
{
	int i;
	TcBtBdAddr *pEntry;

	(void)TC_BT_HF_DEV_Deinit();
	
    pTC_BT_HF_DEV_LIST = TC_BT_Pmalloc(maxConnections * sizeof(TcBtBdAddr));
    pEntry =(TcBtBdAddr *)pTC_BT_HF_DEV_LIST;
	if(pEntry!=NULL)
	{
		ucTcBtHfDevListSize = maxConnections;

		for(i=0; i < ucTcBtHfDevListSize; i++)
		{
			TC_BT_BdAddrZero(&(pEntry[i]));
		}

		return TC_BT_HF_DEV_RESULT_SUCCESS;
	}
	else
	{
		return TC_BT_HF_DEV_RESULT_FAIL;
	}
}

int TC_BT_HF_DEV_Deinit(void)
{
	if(pTC_BT_HF_DEV_LIST!=NULL)
	{
		TC_BT_Pfree(pTC_BT_HF_DEV_LIST);
		pTC_BT_HF_DEV_LIST = NULL;
		ucTcBtHfDevListSize = 0;
		ulTcBtHfDevCurrNum = 0;
	}

	return TC_BT_HF_DEV_RESULT_SUCCESS;
}

int TC_BT_HF_DEV_Register(const TcBtBdAddr *pBdAddr)
{
	int i;
    TcBtBdAddr *pEntry = (TcBtBdAddr *)pTC_BT_HF_DEV_LIST;

	if(pEntry==NULL)
	{
		return TC_BT_HF_DEV_RESULT_FAIL;
	}
	
	for(i=0; i < ucTcBtHfDevListSize; i++)
	{
		if (TC_BT_BdAddrEqZero((const TcBtBdAddr *)&(pEntry[i]))!=0)
		{
			TC_BT_BdAddrCopyIfExist(&(pEntry[i]), (const TcBtBdAddr *)pBdAddr);

			return TC_BT_HF_DEV_RESULT_SUCCESS;
		}
	}

	return TC_BT_HF_DEV_RESULT_FAIL;
}

int TC_BT_HF_DEV_Deregister(const TcBtBdAddr *pBdAddr)
{
	int i;
    TcBtBdAddr *pEntry = (TcBtBdAddr *)pTC_BT_HF_DEV_LIST;

	if(pEntry==NULL)
	{
		return TC_BT_HF_DEV_RESULT_FAIL;
	}
	
	for(i=0; i < ucTcBtHfDevListSize; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(pEntry[i]), (const TcBtBdAddr *)pBdAddr)!=0)
		{
			TC_BT_BdAddrZero(&(pEntry[i]));

			return TC_BT_HF_DEV_RESULT_SUCCESS;
		}
	}

	return TC_BT_HF_DEV_RESULT_FAIL;
}

#endif /* #if defined(BT_HANDSFREE_UNIT) */
