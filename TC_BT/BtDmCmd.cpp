
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "inc/BtDmCmd.h"
#include "inc/BtBase.h"
#include "inc/BtUtil.h"
#include "inc/TC_BT_CMD.h"
#include "inc/TC_BT_SC_CMD.h"

BtDmCmd::BtDmCmd()
{
}
BtDmCmd::~BtDmCmd()
{
}
//**************************   search device **********************************//

int BtDmCmd::TC_BT_SD_Search(TcBtUint32 totalSearchTime, TcBtUint32 maxDevices, TcBtUint8 update)
{
    TcBtSdSearchCmd *pCmdStr;

        pCmdStr =(TcBtSdSearchCmd *) TC_BT_Pmalloc(sizeof(TcBtSdSearchCmd));
        if(pCmdStr != NULL)
        {
            pCmdStr->type = TC_BT_SD_SEARCH_REQ;
            pCmdStr->totalSearchTime = totalSearchTime;
            pCmdStr->maxDevices = maxDevices;
            pCmdStr->update = update;

            (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

                /* SUCCESS: Send Command */
            return TC_BT_CMD_RESULT_SUCCESS_ENUM;
        }
        else
        {
            /* FAIL: No free Memory */
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
}
int BtDmCmd::TC_BT_SD_CancelSearch()
{
    TcBtSdCmd *pCmdStr;

        pCmdStr = (TcBtSdCmd *)TC_BT_Pmalloc(sizeof(TcBtSdCmd));

        if(pCmdStr != NULL)
        {
            pCmdStr->type = TC_BT_SD_CANCEL_SEARCH_REQ;
            (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

            /* SUCCESS: Send Command */
            return TC_BT_CMD_RESULT_SUCCESS_ENUM;
        }
        else
        {
            /* FAIL: No free Memory */
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
}

//*************************safty connect control ******************************//

int BtDmCmd::TC_BT_SC_Debond(TcBtBdAddr bdAddr)
{
    TcBtScBondCmd 	*pCmdStr = 0;

    pCmdStr = (TcBtScBondCmd *)TC_BT_Pmalloc(sizeof(TcBtScBondCmd));
    if(pCmdStr == 0)
    {
        return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

    pCmdStr->type = TC_BT_SC_DEBOND_REQ;
    TC_BT_BdAddrCopyIfExist(&(pCmdStr->bdAddr), (const TcBtBdAddr *)&bdAddr);

    (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

    return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_LegacyPassKey(const TcBtUint8 *pPassKey, TcBtUint8 PassKeyLen)
{
    TcBtScLegacyPinCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        if(PassKeyLen > 32)
        {
            return -4;
        }

        pCmdStr = (TcBtScLegacyPinCmd *)TC_BT_Pmalloc(sizeof(TcBtScLegacyPinCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }

        memset(pCmdStr, 0x00, sizeof(TcBtScLegacyPinCmd));

        pCmdStr->type = TC_BT_SC_PASSKEY_RES;
        memcpy(pCmdStr->PassKey, pPassKey, PassKeyLen);
        pCmdStr->PassKeyLen = PassKeyLen;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_Authorise(TcBtBdAddr bdAddr, TcBtUint16 bAuth)
{
    TcBtScAuthoirseCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */
        pCmdStr = (TcBtScAuthoirseCmd *)TC_BT_Pmalloc(sizeof(TcBtScAuthoirseCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
        pCmdStr->type = TC_BT_SC_AUTHORISE_RES;
        pCmdStr->bdAddr = bdAddr;
        pCmdStr->bAccept = bAuth;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_SSP_PasskeyNotification(TcBtUint16 bAccept)
{
    TcBtScPairCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        pCmdStr = (TcBtScPairCmd *)TC_BT_Pmalloc(sizeof(TcBtScPairCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }

        pCmdStr->type = TC_BT_SC_SSP_NOTIFICATION_RES;
        pCmdStr->bAccept = bAccept;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_SSP_NumericCompare(TcBtUint16 bAccept)
{
    TcBtScPairCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        pCmdStr = (TcBtScPairCmd *)TC_BT_Pmalloc(sizeof(TcBtScPairCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }

        pCmdStr->type = TC_BT_SC_SSP_COMPARE_RES;
        pCmdStr->bAccept = bAccept;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_SSP_JustWork(TcBtUint16 bAccept)
{
    TcBtScPairCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        pCmdStr = (TcBtScPairCmd *)TC_BT_Pmalloc(sizeof(TcBtScPairCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }

        pCmdStr->type = TC_BT_SC_SSP_JUSTWORKS_RES;
        pCmdStr->bAccept = bAccept;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);

        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}
int BtDmCmd::TC_BT_SC_DebugModeReqSend(TcBtUint8 enable)
{
    TcBtScDebugModeCmd 	*pCmdStr = 0;

        /* Check for BT ON ??? */

        /* FAIL: No BT ON -1 */

        pCmdStr = (TcBtScDebugModeCmd *)TC_BT_Pmalloc(sizeof(TcBtScDebugModeCmd));
        if(pCmdStr == 0)
        {
            return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }

        pCmdStr->type = TC_BT_SC_DEBUG_MODE_REQ;
        pCmdStr->enable = enable;

        (void)TC_BT_SendCommand(TC_BT_SC_CMD, pCmdStr);


        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_ReadAvailableServices(TcBtBdAddr deviceAddr)
{
    TcBtSdReadAvailableServicesCmd *pCmdStr;

    pCmdStr = ( TcBtSdReadAvailableServicesCmd *)TC_BT_Pmalloc(sizeof(TcBtSdReadAvailableServicesCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_SD_READ_AVAILABLE_SERVICES_REQ;
        pCmdStr->deviceAddr = deviceAddr;

        (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int TC_BT_SD_CancelReadAvailableServices(void)
{
    TcBtSdCmd *pCmdStr;

    pCmdStr = (TcBtSdCmd *)TC_BT_Pmalloc(sizeof(TcBtSdCmd));

    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ;

        (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

#ifdef ______________________________________________________________________________
#endif

/**********************************  Device Identification  **********************************/

int TC_BT_SD_DiRegisterServiceRecord(TcBtUint16 	vendorId,
                                    TcBtUint16 		productId,
                                    TcBtUint16 		version,
                                    TcBtBool 		primaryRecord,
                                    TcBtUint16 		vendorIdSource,
                                    TcBtUint8 		*pServiceDescription,
                                    TcBtUint16 		serviceDescriptionLen,
                                    TcBtUint8 		*pClientExecutableUrl,
                                    TcBtUint16 		clientExecutableUrlLen,
                                    TcBtUint8 		*pDocumentationUrl,
                                    TcBtUint16 		documentationUrlLen)
{
    TcBtSdDiRegisterServiceRecordCmd *pCmdStr;

    pCmdStr = (TcBtSdDiRegisterServiceRecordCmd *)TC_BT_Pmalloc(sizeof(TcBtSdDiRegisterServiceRecordCmd));
    if(!pCmdStr)
    {
        return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }

    (void)memset(pCmdStr, 0x00, sizeof(TcBtSdDiRegisterServiceRecordCmd));

    pCmdStr->type = TC_BT_SD_DI_REGISTER_SERVICE_RECORD_REQ;
    pCmdStr->vendorId = vendorId;
    pCmdStr->productId = productId;
    pCmdStr->version = version;
    pCmdStr->primaryRecord = primaryRecord;
    pCmdStr->vendorIdSource = vendorIdSource;

    if(serviceDescriptionLen > 0)
    {
        pCmdStr->pServiceDescription = ( TcBtUint8  *)TC_BT_Pmalloc(serviceDescriptionLen);
        if(!pCmdStr->pServiceDescription)
        {
            return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
        (void)memcpy(pCmdStr->pServiceDescription, pServiceDescription, serviceDescriptionLen);
        pCmdStr->serviceDescriptionLen = serviceDescriptionLen;
    }

    if(clientExecutableUrlLen > 0)
    {
        pCmdStr->pClientExecutableUrl = (TcBtUint8  	*)TC_BT_Pmalloc(clientExecutableUrlLen);
        if(!pCmdStr->pClientExecutableUrl)
        {
            return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
        (void)memcpy(pCmdStr->pClientExecutableUrl, pClientExecutableUrl, clientExecutableUrlLen);
        pCmdStr->clientExecutableUrlLen = clientExecutableUrlLen;
    }

    if(documentationUrlLen > 0)
    {
        pCmdStr->pDocumentationUrl = (TcBtUint8  *)TC_BT_Pmalloc(documentationUrlLen);
        if(!pCmdStr->pDocumentationUrl)
        {
            return  TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
        }
        (void)memcpy(pCmdStr->pDocumentationUrl, pDocumentationUrl, documentationUrlLen);
        pCmdStr->documentationUrlLen = documentationUrlLen;
    }

    (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);


    return TC_BT_CMD_RESULT_SUCCESS_ENUM;
}

int TC_BT_SD_DiReadServiceRecord(TcBtBdAddr deviceAddr)
{
    TcBtSdDiReadServiceRecordCmd *pCmdStr;

    pCmdStr = (TcBtSdDiReadServiceRecordCmd *)TC_BT_Pmalloc(sizeof(TcBtSdDiReadServiceRecordCmd));
    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_SD_DI_READ_SERVICE_RECORD_REQ;
        pCmdStr->deviceAddr = deviceAddr;

        (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

int TC_BT_SD_DiCancelReadServiceRecord(void)
{
    TcBtSdCmd *pCmdStr;

    pCmdStr = (TcBtSdCmd *)TC_BT_Pmalloc(sizeof(TcBtSdCmd));
    if(pCmdStr != NULL)
    {
        pCmdStr->type = TC_BT_SD_DI_CANCEL_READ_SERVICE_RECORD_REQ;

        (void)TC_BT_SendCommand(TC_BT_SD_CMD, pCmdStr);

        /* SUCCESS: Send Command */
        return TC_BT_CMD_RESULT_SUCCESS_ENUM;
    }
    else
    {
        /* FAIL: No free Memory */
        return TC_BT_CMD_RESULT_HEAP_EXHAUSTED_ENUM;
    }
}

