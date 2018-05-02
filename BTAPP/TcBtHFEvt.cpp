/****************************************************************************
 *   FileName    : TcBtHFEvt.h
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
*    15/01/06     0.1            created                     Kwon Sunhong
*
*******************************************************************************/
#include <QMessageBox>

#include "TC_BT.h"
#include "BT_CFG.h"
#include "TcBtPhone.h"
#include "TcBtHFEvt.h"
#include "BtSys.h"
#include "TC_BT/inc/BtUtil.h"
#ifndef HF_LOG
#ifdef printf
#undef printf
#endif
#define printf(...)
#endif

#ifdef BT_HANDSFREE_UNIT

#define MAX_DATA_LENGTH(length) (((length) < 510) ? (length) : 510)

void TcBtHFEvt_MessageSend(unsigned char type, unsigned short id, const unsigned char *data, unsigned short length)
{
#if 1   /* Telechips Linux HF remark */
    TcBtHFEvt	*pTcBtHFEvt = NULL;
    unsigned char *uiMessage;
    unsigned int messageLength, alignSize;
    unsigned char *pBuff = NULL;
    unsigned int tick=0;

    pTcBtHFEvt = (TcBtHFEvt *)TC_BT_Pmalloc(sizeof(TcBtHFEvt));
    pBuff = (unsigned char *)TC_BT_Pmalloc(MAX_HF_MESSAGE_LENGTH);

    if(pTcBtHFEvt != NULL && pBuff != NULL)
    {
        messageLength = COM_SUBHEADER_MSG_LENGTH + length;
        alignSize = ((messageLength % 4) == 0) ? 0 : (4 - (messageLength % 4));
        messageLength += alignSize;

        uiMessage = pBuff;
        memset(uiMessage, 0x00, (unsigned long)MAX_HF_MESSAGE_LENGTH);

        pTcBtHFEvt->mAddr = (void*)uiMessage;
        pTcBtHFEvt->mSize = messageLength;

        /* Put Message structure */
        //uiMessage[0] = TCC_DEMON_TASK;
        //uiMessage[1] = TCC_UI_TASK;
        //uiMessage[2] = src;
        uiMessage[0] = type;
        uiMessage[1] = (unsigned char)(tick & 0xff);
        uiMessage[2] = (unsigned char)((tick >> 8) & 0xff);
        uiMessage[3] = (unsigned char)((tick >> 16) & 0xff);
        uiMessage[4] = (unsigned char)((tick >> 24) & 0xff);
        uiMessage[5] = (unsigned char)(id & 0xff);
        uiMessage[6] = (unsigned char)((id >> 8) & 0xff);
        uiMessage[7] = (unsigned char)((length + alignSize) & 0xff);
        uiMessage[8] = (unsigned char)(((length + alignSize) >> 8) & 0xff);

        /* Put ID & Data */
        uiMessage += COM_SUBHEADER_MSG_LENGTH;
        if (data != NULL)
        {
            memcpy(uiMessage, data, length);
        }
        else
        {
            length = 0;
        }

        uiMessage += length;
        memset(uiMessage, 0x00, (unsigned long)alignSize);

        //printf("TcBtHFEvt_MessageSend id = %d\n", id);
        emit g_TcBtPhone->m_DBSignal->HF_Event(type, (void *)pTcBtHFEvt);
    }
    else
    {
        printf("Error : TcBtHFEvt_MessageSend id = %d\n", id);
    }
#endif
}

void TcBtHFEvt_EvtBtDirectFourDataMessageSend(unsigned short id, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4)
{
#if 1	/* Telechips Linux HF remark */
    unsigned char cmd[128];
    unsigned int cmd_size;

    cmd_size=0;
    cmd[cmd_size] = sizeof(G_BtSys->gBtAppSysCurStatus);
    cmd_size++;
    memcpy(&cmd[cmd_size], G_BtSys->gBtAppSysCurStatus, sizeof(G_BtSys->gBtAppSysCurStatus));
    cmd_size += sizeof(G_BtSys->gBtAppSysCurStatus);
    TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, BT_HF_EVT_UPDATE_STATUS/*BT_SYS_EVT_UPDATE_STATUS*/, cmd, cmd_size);

    cmd_size=0;
    //cmd[cmd_size] = id
    cmd[cmd_size] = 16;
    cmd_size++;
    memcpy(&cmd[cmd_size], &data1, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data2, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data3, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data4, 4);
    cmd_size += 4;
    TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, id, cmd, cmd_size);
    //TcBtHFEvt_MessageSend(&UI_MBOX, pEvt->src, MSG_EVENT_TYPE, (void *)Cmd, cmd_size);	//direct msg
#endif
}

void TcBtHFEvt_EvtBtFourDataMessageSend(unsigned short id, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4)
{
#if 1	/* Telechips Linux HF remark */
    unsigned char cmd[128];
    unsigned int cmd_size;

    cmd_size=0;
    cmd[cmd_size] = sizeof(G_BtSys->gBtAppSysCurStatus);
    cmd_size++;
    memcpy(&cmd[cmd_size], G_BtSys->gBtAppSysCurStatus, sizeof(G_BtSys->gBtAppSysCurStatus));
    cmd_size += sizeof(G_BtSys->gBtAppSysCurStatus);
    TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, BT_HF_EVT_UPDATE_STATUS/*BT_SYS_EVT_UPDATE_STATUS*/, cmd, cmd_size);

    cmd_size=0;
    //cmd[cmd_size] = id
    cmd[cmd_size] = 16;
    cmd_size++;
    memcpy(&cmd[cmd_size], &data1, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data2, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data3, 4);
    cmd_size += 4;
    memcpy(&cmd[cmd_size], &data4, 4);
    cmd_size += 4;
    TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, id, cmd, cmd_size);
#endif
}

void TcBtHFEvt_EvtBTInfoMessageSend(unsigned short id, unsigned char *data, unsigned int size)
{
#if 1
#if 1
    unsigned char cmd[MAX_STRING_LENGTH + 4];
    unsigned short realSize;
    unsigned int cmd_size;

    cmd_size=0;
    cmd[cmd_size] = sizeof(G_BtSys->gBtAppSysCurStatus);
    cmd_size++;
    memcpy(&cmd[cmd_size], G_BtSys->gBtAppSysCurStatus, sizeof(G_BtSys->gBtAppSysCurStatus));
    cmd_size+=sizeof(G_BtSys->gBtAppSysCurStatus);
    TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, BT_HF_EVT_UPDATE_STATUS/*BT_SYS_EVT_UPDATE_STATUS*/, cmd, cmd_size);

    cmd_size=0;
    realSize = GET_MAX_STRING_LENGTH(size);

    if (realSize > 253)
    {
        cmd[cmd_size] = 0xFF;// [2011-11-28 thshin] extended length flag
        cmd_size++;
        cmd[cmd_size] = (unsigned char)((realSize + 1) & 0xFF);
        cmd_size++;
        cmd[cmd_size] = (unsigned char)(((realSize + 1) >> 8) & 0xFF);
        cmd_size++;
        //cmd[cmd_size] = NULL;// [2011-11-28 thshin] flfn
        //cmd_size++;
        memcpy(&cmd[cmd_size], data, (unsigned long)realSize);

        TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, id, cmd, realSize + 3);
    }
    else
    {
        cmd[cmd_size] = (unsigned char)realSize;
        cmd_size++;
        memcpy(&cmd[cmd_size], data, (unsigned long)realSize);

        TcBtHFEvt_MessageSend(MSG_EVENT_TYPE, id, cmd, realSize + 1);
    }
#else
    //unsigned char cmd[256];
    unsigned char cmd[512];
    unsigned char realSize;
    unsigned int cmd_size;

    cmd_size=0;
    //cmd_size[cmd_size] = BT_SYS_EVT_UPDATE_STATUS;
    //cmd_size++;
    cmd[cmd_size] = sizeof(gBtAppSysCurStatus);
    cmd_size++;
    memcpy(&cmd[cmd_size], gBtAppSysCurStatus, sizeof(gBtAppSysCurStatus));
    cmd_size+=sizeof(gBtAppSysCurStatus);
    DMP_MessageSend(&UI_MBOX, source, MSG_EVENT_TYPE, BT_SYS_EVT_UPDATE_STATUS, cmd, cmd_size);

    cmd_size=0;
    //cmd[cmd_size] = id;
    //cmd_size++;
    //realSize = GET_MAX_STRING_LENGTH(size);
    realSize = MAX_DATA_LENGTH(size);
    cmd[cmd_size] = realSize;
    cmd_size++;
    memcpy(&cmd[cmd_size], data, (unsigned long)realSize);
    //cmd_size += realSize;
    DMP_MessageSend(&UI_MBOX, source, MSG_EVENT_TYPE, id, (void *)cmd, realSize + 1);
#endif
#endif
}
#endif /*BT_HANDSFREE_UNIT*/
