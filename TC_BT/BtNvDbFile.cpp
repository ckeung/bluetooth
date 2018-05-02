/****************************************************************************
 *   FileName    : TC_BT_SC_DB_FILE.c
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
*  Description : This file contains functions and variables for a NV_DB application.
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    10/07/22     0.1            created                     Kwon Sunhong
*    11/10/12     0.2            supported TCLinkV4          Hwang, Jin Ho.
*******************************************************************************/

#include "BT_CFG.h"

/*****************************************************************************
*
* Header File Include
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <pwd.h>

#include "TC_BT.h"
#include "BTAPP.h"
#include "BtNvDbFile.h"
#include "TC_BT_SC_CMD.h"
#ifndef GAP_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

#define BT_SC_DB_FILE 

#ifdef BT_SC_DB_FILE

/*****************************************************************************
*
* definitions
*
*****************************************************************************/

/*****************************************************************************
*
* structures
*
*****************************************************************************/ 

/*****************************************************************************
*
* externs
*
*****************************************************************************/

/*****************************************************************************
*
* static functions
*
*****************************************************************************/
static void* TC_BT_SC_DB_AllocMem(void);  
static void TC_BT_SC_DB_FreeMem(void);
static void* TC_BT_SC_DB_GetMem(void);
static unsigned char TC_BT_SC_DB_CheckSupportProfileFn(const TcBtSecDevRecrd *pTheRecord, unsigned short usUUID);

/*****************************************************************************
*
* static variables
*
*****************************************************************************/
static 	void *gpSCDBMem = NULL;

/*****************************************************************************
*
* global variables
*
*****************************************************************************/
/* TODO: Add your specialized code here */
pthread_mutex_t 	gSCDBMutex = PTHREAD_MUTEX_INITIALIZER;

/*****************************************************************************
*
* debug
*
*****************************************************************************/

/* TODO: Add your specialized code here */

/*****************************************************************************
* 
* entry points
*
*****************************************************************************/
static void* TC_BT_SC_DB_AllocMem(void)
{
	if(gpSCDBMem == NULL)
	{
		gpSCDBMem = TC_BT_Pmalloc(sizeof(TcBtSecDB));
	}

	return gpSCDBMem;
}

static void TC_BT_SC_DB_FreeMem(void)
{
	if(gpSCDBMem != NULL) 
	{
		TC_BT_Pfree(gpSCDBMem); 
		gpSCDBMem=NULL;
	}
}

static void* TC_BT_SC_DB_GetMem(void)
{
	return gpSCDBMem;
}

/*************************************************************************************
*  TC_BT_SC_DB_GetDBPath(void)
*-------------------------------------------------------------------------------------
* Description  : Get DB path from system.
* Parameters   : 
* Return	   : TRUE : config folder is created, FALSE : error
**************************************************************************************/
char* TC_BT_SC_DB_GetDBPath(void)
{
#ifdef __arm__
    static char strHomePath[256];
    struct passwd *pw = getpwuid(getuid());

    memset(strHomePath, 0x00, 256);

    if (pw != NULL)
    {
        strcpy(strHomePath, pw->pw_dir);
    }
    else
    {
        char *path;
        path = getenv("HOME");
        if (path != NULL)
        {
            strcpy(strHomePath, path);
        }
        else
        {
            fprintf(stderr, "get user home directory failed.\n set default path\n");
            strcpy(strHomePath, "/home/root");
        }
    }

    strcat(strHomePath, "/.telechips");
    mkdir(strHomePath, S_IRWXU|S_IRGRP|S_IXGRP);
    strcat(strHomePath, "/bt");
    mkdir(strHomePath, S_IRWXU|S_IRGRP|S_IXGRP);

    return strHomePath;
#else
    return NULL;
#endif
}

/*************************************************************************************
*  TC_BT_SC_DB_InitScDb
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_SC_DB_InitScDb(void)
{
	FILE *fp = NULL;
	TcBtSecDB *info;
    char strDBPath[256] = {NULL, };
    char *strGetDBPath = NULL;

	//Init mutex
	pthread_mutex_init(&gSCDBMutex, NULL);

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_AllocMem();

	if(info==NULL)
    {
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}
	
	memset(info, 0x00, sizeof(TcBtSecDB));

    strGetDBPath = TC_BT_SC_DB_GetDBPath();
    if(strGetDBPath != NULL)
    {
        strcpy(strDBPath, strGetDBPath);
        strcat(strDBPath,"/");
    }

    strcat(strDBPath, SC_DB_FILE_NAME);

	fp = fopen(strDBPath, "rb");
    if(fp != NULL)
    {
		int readlen = 0;
		readlen = fread(info, sizeof(unsigned char),sizeof(TcBtSecDB),fp);
		fclose(fp);
	}
	else
    {
	}

	info->Index=0;	
	info->Mod=FALSE;

	pthread_mutex_unlock(&gSCDBMutex);
}

/*************************************************************************************
*  TC_BT_SC_DB_DeInitScDb
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_SC_DB_DeInitScDb(void)
{
	TC_BT_SC_DB_UpdateScDb();

	pthread_mutex_lock(&gSCDBMutex);

	(void)TC_BT_SC_DB_FreeMem();

	pthread_mutex_unlock(&gSCDBMutex);

	//Destory mutex
	pthread_mutex_destroy(&gSCDBMutex); 
}

/*************************************************************************************
*  TC_BT_SC_DB_UpdateScDb
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_SC_DB_UpdateScDb(void)
{
	FILE *fp = NULL;
	TcBtSecDB *info;
    char strDBPath[256] = {NULL, };
    char *strGetDBPath = NULL;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if(info==NULL)
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}
	
	if(info->Mod==FALSE)
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}
	
    strGetDBPath = TC_BT_SC_DB_GetDBPath();
    if(strGetDBPath != NULL)
    {
        strcpy(strDBPath, strGetDBPath);
        strcat(strDBPath,"/");
    }

    strcat(strDBPath, SC_DB_FILE_NAME);

	fp = fopen(strDBPath, "wb");
    if(fp != NULL)	
    {
    	int writelen = 0;
		writelen = fwrite(info, sizeof(unsigned char), sizeof(TcBtSecDB), fp);
		fflush(fp);
		fclose(fp);
	}
	else
	{
	}

	info->Mod=FALSE;

	pthread_mutex_unlock(&gSCDBMutex);
}

#ifdef BT_AUTO_CONNECTION
/*************************************************************************************
*  TC_BT_SC_DB_ReadLastDevAddr
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
TcBtBdAddr* TC_BT_SC_DB_ReadLastDevAddr(void)
{	
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if(info==NULL || info->Version != TC_BT_NV_DB_FILE_VERSION)
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return NULL;
	}

	if(TC_BT_BdAddrEqZero((const TcBtBdAddr *)&(info->LastDevAddr)) == FALSE)
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return (&(info->LastDevAddr));
	}
	else
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return NULL;
	}
}

/*************************************************************************************
*  TC_BT_SC_DB_WriteLastDevAddr
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_SC_DB_WriteLastDevAddr(const TcBtBdAddr* BdAddr)
{
	TcBtBdAddr 					*ConnectedAddr;
	TcBtSecDB 	*info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();
	
	if((info==NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}

	if(TC_BT_BdAddrEq((const TcBtBdAddr *)&info->LastDevAddr, BdAddr))
	{
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}
	else
	{
		info->LastDevAddr = *BdAddr;
	}

	info->Mod=TRUE;

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();
}
#endif	/* BT_AUTO_CONNECTION */

/*************************************************************************************
*  TC_BT_SC_DB_CheckSupportProfileFn
*-------------------------------------------------------------------------------------
* Description  : find given UUID from the device record
* Parameters   : 
* Return	   : TRUE: if UUID is exist, FALSE: if not
**************************************************************************************/
static unsigned char TC_BT_SC_DB_CheckSupportProfileFn(const TcBtSecDevRecrd* pTheRecord, unsigned short usUUID)
{
	if (usUUID <= 0x111F)
	{/* UUID: 0x1100 -> 0x111F */
		if(COAL_BIT_IS_SET(pTheRecord->knownServices11_00_31, usUUID - 0x1100) != 0)
		{
			return TRUE;
		}
	}
	else if (usUUID <= 0x113F)
	{/* UUID: 0x1120 -> 0x113F */
		if(COAL_BIT_IS_SET(pTheRecord->knownServices11_32_63, usUUID - 0x1120) != 0)
		{
			return TRUE;
		}
	}
	else if (usUUID <= 0x121F)
	{/* UUID: 0x1200 -> 0x121F */
		if(COAL_BIT_IS_SET(pTheRecord->knownServices12_00_31, usUUID - 0x1200) != 0)
		{
			return TRUE;
		}
	}
	else if (usUUID <= 0x131F)
	{/* UUID: 0x1300 -> 0x131F */
		if(COAL_BIT_IS_SET(pTheRecord->knownServices13_00_31, usUUID - 0x1300) != 0)
		{
			return TRUE;
		}
	}
	else
	{
	}

	return FALSE;
}

/*************************************************************************************
*  TC_BT_SC_DB_CheckSupportProfile
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned char TC_BT_SC_DB_CheckSupportProfile(const TcBtBdAddr* BdAddr, unsigned short usUUID)
{
	TcBtSecDevRecrd theRecord;
	unsigned char ret = FALSE;

	memset(&theRecord, 0, sizeof(TcBtSecDevRecrd));

	if(TC_BT_NV_DB_Read(BdAddr, &theRecord) != 0)
	{
		ret = TC_BT_SC_DB_CheckSupportProfileFn((const TcBtSecDevRecrd *)&theRecord, usUUID);
	}

	return ret;
}

/*************************************************************************************
*  TC_BT_SC_DB_UpdateSupportProfile
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_SC_DB_UpdateSupportProfile(const TcBtBdAddr* BdAddr, unsigned short usUUID)
{
	int i;
	TcBtSecDB *info;
	TcBtSecDevRecrd *pTheRecoder;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{ /* insufficient memory or SC_DB_FILE_VERSION was changed */
		pthread_mutex_unlock(&gSCDBMutex);
		return;
	}
	
	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
	{
		/* gets the 'i'th record */
		pTheRecoder = &(info->theRecoder[i]);

		/* updated the record */
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(pTheRecoder->deviceAddr), BdAddr) != 0)
		{
			if(TC_BT_SC_DB_CheckSupportProfileFn((const TcBtSecDevRecrd *)pTheRecoder, usUUID) != 0)
			{
				pthread_mutex_unlock(&gSCDBMutex);
				return;
			}
			
			if (usUUID <= 0x111F)
			{/* UUID: 0x1100 -> 0x111F */
				COAL_BIT_SET(pTheRecoder->knownServices11_00_31, usUUID - 0x1100);
				info->Mod = TRUE;
			}
			else if (usUUID <= 0x113F)
			{/* UUID: 0x1120 -> 0x113F */
				COAL_BIT_SET(pTheRecoder->knownServices11_32_63, usUUID - 0x1120);
				info->Mod = TRUE;
			}
			else if (usUUID <= 0x121F)
			{/* UUID: 0x1200 -> 0x121F */
				COAL_BIT_SET(pTheRecoder->knownServices12_00_31, usUUID - 0x1200);
				info->Mod = TRUE;
			}
			else if (usUUID <= 0x131F)
			{/* UUID: 0x1300 -> 0x131F */
				COAL_BIT_SET(pTheRecoder->knownServices13_00_31, usUUID - 0x1300);
				info->Mod = TRUE;
			}
			else
			{
			}
			
			break;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();
}

int TC_BT_SC_DB_IsFull(void)
{
	int ret = -1;
	int numOfRecord;
	TcBtSecDB *info = NULL;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{ /* insufficient memory or SC_DB_FILE_VERSION was changed */
		pthread_mutex_unlock(&gSCDBMutex);
		return ret;
	}
	
	for(numOfRecord = 0; numOfRecord < SC_DB_MAX_NUM_OF_RECORD_INFO; numOfRecord++)
	{
		if((info->theRecoder[numOfRecord].deviceAddr.lap == 0) &&
			(info->theRecoder[numOfRecord].deviceAddr.nap == 0) &&
			(info->theRecoder[numOfRecord].deviceAddr.uap == 0))
		{
			break;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	if(numOfRecord == SC_DB_MAX_NUM_OF_RECORD_INFO)
	{
		ret = TRUE;
	}
	else /* if(numOfRecord < SC_DB_MAX_NUM_OF_RECORD_INFO) */
	{
		ret = FALSE;
	}

	return ret;
}

int TC_BT_SC_DB_ToTheTop(const TcBtBdAddr *pBdAddr)
{
	int ret = -1;
	int i;
	TcBtSecDB *info = NULL;

	pthread_mutex_lock(&gSCDBMutex);

    info =(TcBtSecDB *) TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{ /* insufficient memory or SC_DB_FILE_VERSION was changed */
		pthread_mutex_unlock(&gSCDBMutex);
		return ret;
	}

	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
	{
		if(TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->theRecoder[i].deviceAddr), pBdAddr) != 0)
		{ /* The record that matches with device address is founded */
			if(i == 0)
			{ /* The founded record was already placed at the first */
				;/* do nothing */
			}
			else /* if(i > 0) */
			{ /* The founded record was placed at other than the first */
				TcBtSecDevRecrd tempRecord;
				TcBtSecDevRecrd *pDestination = &(info->theRecoder[1]);
				TcBtSecDevRecrd *pSource = &(info->theRecoder[0]);
				unsigned long byteLength = sizeof(TcBtSecDevRecrd)*i;

				memcpy(&tempRecord, &(info->theRecoder[i]), sizeof(TcBtSecDevRecrd));
				memmove(pDestination, pSource, byteLength);
				memcpy(&(info->theRecoder[0]), &tempRecord, sizeof(TcBtSecDevRecrd));

				info->Mod = TRUE;
			}
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();

	ret = 0;

	return ret;
}

/*************************************************************************************
*  TC_BT_NV_DB_Read
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned char TC_BT_NV_DB_Read(const TcBtBdAddr *theAddr , TcBtSecDevRecrd *theRecord)
{
	unsigned int i;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return FALSE;
	}
	
	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO ; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->theRecoder[i].deviceAddr), theAddr) != 0)
		{
			*theRecord = info->theRecoder[i];

			pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
			return TRUE;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/

	return FALSE;
}

/*************************************************************************************
*  TC_BT_NV_DB_Write
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_NV_DB_Write(const TcBtBdAddr *theAddr, const TcBtSecDevRecrd *theRecord)
{

	unsigned int i;
	unsigned char	boExistFlag = FALSE;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if(info==NULL)
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return;
	}
	
	if (info->Version != TC_BT_NV_DB_FILE_VERSION)
	{
		memset(info, 0, sizeof(TcBtSecDB));
		info->Version = TC_BT_NV_DB_FILE_VERSION;
	}
	
	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->theRecoder[i].deviceAddr), theAddr) != 0)
		{
			if(memcmp(&(info->theRecoder[i]), theRecord, sizeof(TcBtSecDevRecrd)) != 0)
			{
				info->theRecoder[i] = *theRecord;
				info->Mod = TRUE;
			}
			boExistFlag = TRUE;
			break;
		}
	}

	if(boExistFlag!=TRUE)
	{
		for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
		{
			if ((info->theRecoder[i].deviceAddr.lap == 0) && (info->theRecoder[i].deviceAddr.nap == 0) && (info->theRecoder[i].deviceAddr.uap == 0))
			{
				info->theRecoder[i] = *theRecord;	// The first SCDB write
				info->Mod = TRUE;
				break;
			}
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();
}

/*************************************************************************************
*  TC_BT_NV_DB_Remove
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_NV_DB_Remove(const TcBtBdAddr *theAddr)
{
	unsigned int i;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info =(TcBtSecDB *) TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return;
	}
	
	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO ; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->theRecoder[i].deviceAddr), theAddr) != 0)
		{
			TC_BT_BdAddrZero(&(info->theRecoder[i].deviceAddr));
			if(i < (SC_DB_MAX_NUM_OF_RECORD_INFO - 1))
			{ /* Fill the deleted location by following records */
				TcBtSecDevRecrd *pDestination = &(info->theRecoder[i]);
				TcBtSecDevRecrd *pSource = &(info->theRecoder[i+1]);
				unsigned long byteLength = sizeof(TcBtSecDevRecrd)*((SC_DB_MAX_NUM_OF_RECORD_INFO-1)-i);
				
				memmove(pDestination, pSource, byteLength);
				memset(&(info->theRecoder[SC_DB_MAX_NUM_OF_RECORD_INFO-1]), 0x00, sizeof(TcBtSecDevRecrd));
			}
#ifdef BT_AUTO_CONNECTION
			/* Check for Last Device */
			if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->LastDevAddr), theAddr) != 0)
			{
				/* Delete Last Device */
				TC_BT_BdAddrZero(&(info->LastDevAddr));
			}
#endif
			info->Mod = TRUE;
			break;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();
}

/*************************************************************************************
*  TC_BT_NV_DB_RemoveAll
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
void TC_BT_NV_DB_RemoveAll(void)
{
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if(info == NULL)
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return;
	}

	memset(info, 0, sizeof(TcBtSecDB));

	info->Mod = TRUE;

	pthread_mutex_unlock(&gSCDBMutex);

	TC_BT_SC_DB_UpdateScDb();
}

/*************************************************************************************
*  TC_BT_NV_DB_ReadFirst
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned char TC_BT_NV_DB_ReadFirst(TcBtSecDevRecrd *theRecord)
{
	unsigned int i;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return FALSE;
	}
	
	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
	{
		if (! ((info->theRecoder[i].deviceAddr.lap == 0) 
			&& (info->theRecoder[i].deviceAddr.nap == 0) 
			&& (info->theRecoder[i].deviceAddr.uap == 0)))
		{
			*theRecord = info->theRecoder[i];
			info->Index = i+1;
			pthread_mutex_unlock(&gSCDBMutex);
			return TRUE;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	return FALSE;
}

/*************************************************************************************
*  TC_BT_NV_DB_ReadNext
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned char TC_BT_NV_DB_ReadNext(TcBtSecDevRecrd *theRecord)
{
	unsigned int i;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return FALSE;
	}
	
	for(i = info->Index ; i < SC_DB_MAX_NUM_OF_RECORD_INFO ; i++)
	{
		if (! ((info->theRecoder[i].deviceAddr.lap == 0) 
			&& (info->theRecoder[i].deviceAddr.nap == 0) 
			&& (info->theRecoder[i].deviceAddr.uap == 0)))
		{
			*theRecord = info->theRecoder[i];
			info->Index = i+1;
			pthread_mutex_unlock(&gSCDBMutex);
			return TRUE;
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	return FALSE;
}

/*************************************************************************************
*  TC_BT_SC_DB_PrintLinkKey
*-------------------------------------------------------------------------------------
* Description  : 
* Parameters   : 
* Return	   : 
**************************************************************************************/
unsigned char TC_BT_SC_DB_PrintLinkKey(const TcBtBdAddr *pBdAddr)
{
	unsigned int i;
    TcBtSecDB *info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if(info==NULL)
	{
		return FALSE;	/* insufficient memory  */
	}

	if (info->Version != TC_BT_NV_DB_FILE_VERSION)
	{
		return FALSE;
	}

	if(pBdAddr == NULL)
	{
		return FALSE;
	}

	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO ; i++)
	{
		if (TC_BT_BdAddrEq((const TcBtBdAddr *)&(info->theRecoder[i].deviceAddr), pBdAddr) != 0)
		{
			
			TcBtSecDevRecrd *ptheRecord = &(info->theRecoder[i]);

            //TC_BT_SYS_PrintLinkKey(pBdAddr, ptheRecord->remoteName, ptheRecord->Linkkey, ptheRecord->linkkeyType, ptheRecord->linkkeyValid);

			return TRUE;
		}
	}

	return FALSE;
}

TcBtCharString *TC_BT_SC_DB_GetRemoteDeviceName(const TcBtBdAddr *pBdAddr)
{
	unsigned int i;
	TcBtSecDB *info;

	pthread_mutex_lock(&gSCDBMutex);

    info = (TcBtSecDB *)TC_BT_SC_DB_GetMem();

	if((info == NULL) || (info->Version != TC_BT_NV_DB_FILE_VERSION))
	{
		pthread_mutex_unlock(&gSCDBMutex);	/* insufficient memory	*/
		return FALSE;
	}

	for(i = 0; i < SC_DB_MAX_NUM_OF_RECORD_INFO; i++)
	{
		if(TC_BT_BdAddrEq(&(info->theRecoder[i].deviceAddr), pBdAddr) == TRUE)
		{
			pthread_mutex_unlock(&gSCDBMutex);
            return(TcBtCharString*) (info->theRecoder[i].remoteName);
		}
	}

	pthread_mutex_unlock(&gSCDBMutex);

	return NULL;
}

#endif	/* BT_SC_DB_FILE */
