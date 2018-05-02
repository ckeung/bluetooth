/****************************************************************************
 *   FileName    : TC_BT_PAC_EVT.h
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
*  Description : This file contains functions for a Bluetooth PAC event
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*    ---------    --------       -----------------           -----------------
*    10/07/07     0.1            created                        jhd
*
*******************************************************************************/

#ifndef __TC_BT_PAC_EVT_H__
#define __TC_BT_PAC_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "inc/BtDataType.h"
#include "TC_BT_EVT.h"
#include "TC_BT_CMD.h"
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
typedef enum
{
       TC_BT_PAC_ACTIVATE_CFM = TC_BT_PAC_EVT_TYPE_ENUM, /* TC_BT_EVT_TYPE_ENUM_BASE + 0x300 */
       TC_BT_PAC_DEACTIVATE_CFM,
	TC_BT_PAC_CONNECT_CFM,
	TC_BT_PAC_PULL_PB_CFM,
	TC_BT_PAC_PULL_PB_IND,
	TC_BT_PAC_SET_FOLDER_CFM,
	TC_BT_PAC_PULL_VCARDLIST_CFM,
	TC_BT_PAC_PULL_VCARDLIST_IND,
	TC_BT_PAC_PULL_VCARDENTRY_CFM,
	TC_BT_PAC_PULL_VCARDENTRY_IND,
	TC_BT_PAC_ABORT_CFM,
	TC_BT_PAC_DISCONNECT_IND,

	TC_BT_PAC_EVT_TYPE_MAX
} TcBtPacEvtEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef TcBtUint16	TcBtPacPrim;
typedef TcBtUint16  TcBtPacSrcType;
typedef TcBtUint16  TcBtObexResponseCode;

/* Refer to "Rfcomm_prim.h" */
#define TC_BT_RFC_RES_TABLE_DUMMY
#define TC_BT_RFC_RES_TABLE_X(a, b) a b
#define TC_BT_RFC_RES_TABLE_SEP ,

#define TC_BT_BLUESTACK_ERRORCODE_BLOCK   0x0800
#define TC_BT_RFCOMM_ERRORCODE_BASE       (TC_BT_BLUESTACK_ERRORCODE_BLOCK * 2)

typedef enum
{
	TC_BT_RFC_SUCCESS = 0,
	TC_BT_RFC_ACCEPT_SERVER_CONNECTION = TC_BT_RFCOMM_ERRORCODE_BASE,
	TC_BT_RFC_DECLINE_SERVER_CONNECTION,
	TC_BT_RFC_CONNECTION_PENDING,
	TC_BT_RFC_CONNECTION_REJ_SECURITY,
	TC_BT_RFC_NORMAL_DISCONNECT,
	TC_BT_RFC_ABNORMAL_DISCONNECT,
	TC_BT_RFC_REMOTE_REFUSAL,
	TC_BT_RFC_CHANNEL_ALREADY_EXISTS,
	TC_BT_RFC_INVALID_CHANNEL,
	TC_BT_RFC_UNKNOWN_PRIMITIVE,
	TC_BT_RFC_INVALID_PAYLOAD,
	TC_BT_RFC_INCONSISTENT_PARAMETERS,
	TC_BT_RFC_PEER_VIOLATED_FLOW_CONTROL,
	TC_BT_RFC_RES_ACK_TIMEOUT,
	TC_BT_RFC_DATAWRITE_PENDING,

	TC_BT_RFC_EVT_TYPE_MAX
} TcBtRfcommResultCode;

#define TC_BT_OBEX_CONTINUE_RESPONSE_CODE                                         ((TcBtObexResponseCode) (0x10))
#define TC_BT_OBEX_SUCCESS_RESPONSE_CODE                                          ((TcBtObexResponseCode) (0x20))
#define TC_BT_OBEX_CREATED_RESPONSE_CODE                                          ((TcBtObexResponseCode) (0x21))
#define TC_BT_OBEX_ACCEPTED_RESPONSE_CODE                                         ((TcBtObexResponseCode) (0x22))
#define TC_BT_OBEX_NON_AUTHORITATIVE_INFORMATION_RESPONSE_CODE                    ((TcBtObexResponseCode) (0x23))
#define TC_BT_OBEX_NO_CONTENT_RESPONSE_CODE                                       ((TcBtObexResponseCode) (0x24))
#define TC_BT_OBEX_RESET_CONTENT_RESPONSE_CODE                                    ((TcBtObexResponseCode) (0x25))
#define TC_BT_OBEX_PARTIAL_CONTENT_RESPONSE_CODE                                  ((TcBtObexResponseCode) (0x26))
#define TC_BT_OBEX_MULTIPLE_CHOICES_RESPONSE_CODE                                 ((TcBtObexResponseCode) (0x30))
#define TC_BT_OBEX_MOVED_PERMANENTLY_RESPONSE_CODE                                ((TcBtObexResponseCode) (0x31))
#define TC_BT_OBEX_MOVED_TEMPORARILY_RESPONSE_CODE                                ((TcBtObexResponseCode) (0x32))
#define TC_BT_OBEX_NOT_MODIFIED_RESPONSE_CODE                                     ((TcBtObexResponseCode) (0x34))
#define TC_BT_OBEX_USE_PROXY_RESPONSE_CODE                                        ((TcBtObexResponseCode) (0x35))
#define TC_BT_OBEX_BAD_REQUEST_RESPONSE_CODE                                      ((TcBtObexResponseCode) (0x40))
#define TC_BT_OBEX_UNAUTHORIZED_CODE                                              ((TcBtObexResponseCode) (0x41))
#define TC_BT_OBEX_PAYMENT_REQUIRED_RESPONSE_CODE                                 ((TcBtObexResponseCode) (0x42))
#define TC_BT_OBEX_FORBIDDEN_RESPONSE_CODE                                        ((TcBtObexResponseCode) (0x43))
#define TC_BT_OBEX_NOT_FOUND_RESPONSE_CODE                                        ((TcBtObexResponseCode) (0x44))
#define TC_BT_OBEX_METHOD_NOT_ALLOWED_RESPONSE_CODE                               ((TcBtObexResponseCode) (0x45))
#define TC_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE                                   ((TcBtObexResponseCode) (0x46))
#define TC_BT_OBEX_PROXY_AUTHENTICATION_REQUIRED_RESPONSE_CODE                    ((TcBtObexResponseCode) (0x47))
#define TC_BT_OBEX_REQUEST_TIME_OUT_RESPONSE_CODE                                 ((TcBtObexResponseCode) (0x48))
#define TC_BT_OBEX_CONFLICT_RESPONSE_CODE                                         ((TcBtObexResponseCode) (0x49))
#define TC_BT_OBEX_GONE_RESPONSE_CODE                                             ((TcBtObexResponseCode) (0x4A))
#define TC_BT_OBEX_LENGTH_REQUIRED_RESPONSE_CODE                                  ((TcBtObexResponseCode) (0x4B))
#define TC_BT_OBEX_PRECONDITION_FAILED_RESPONSE_CODE                              ((TcBtObexResponseCode) (0x4C))
#define TC_BT_OBEX_TOO_LARGE_RESPONSE_CODE                                        ((TcBtObexResponseCode) (0x4D))
#define TC_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE                                ((TcBtObexResponseCode) (0x4F))
#define TC_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE                            ((TcBtObexResponseCode) (0x50))
#define TC_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE                                  ((TcBtObexResponseCode) (0x51))
#define TC_BT_OBEX_BAD_GATEWAY_RESPONSE_CODE                                      ((TcBtObexResponseCode) (0x52))
#define TC_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE                              ((TcBtObexResponseCode) (0x53))
#define TC_BT_OBEX_GATEWAY_TIMEOUT                                                ((TcBtObexResponseCode) (0x54))
#define TC_BT_OBEX_HTTP_VERSION_NOT_SUPPORTED_RESPONSE_CODE                       ((TcBtObexResponseCode) (0x55))
#define TC_BT_OBEX_DATABASE_FULL_RESPONSE_CODE                                    ((TcBtObexResponseCode) (0x60))
#define TC_BT_OBEX_DATABASE_LOCKED_RESPONSE_CODE                                  ((TcBtObexResponseCode) (0x61))

/*****************************************************************************
*
* structures
*
*****************************************************************************/
/* TODO: Add your specialized code here */
typedef struct
{
    unsigned short int                 type;
    TcBtBdAddr                  bdAddr;
} TcBtPacEvt;

#if 1 /* Telechips' remark */
typedef struct
{
    TcBtCmdType                 type;
    TcBtResultCode     		resultCode;
    TcBtSupplier         	resultSupplier;
} TcBtPacActivateCfm;

typedef struct
{
    TcBtCmdType                 type;
    TcBtResultCode     		resultCode;
    TcBtSupplier         	resultSupplier;
} TcBtPacDeactivateCfm;
#endif

/* General profile handling, link handling and configuration primitives */
/* Connection and audio handling primitives */
typedef struct
{
    TcBtPacPrim          	type;
    TcBtResultCode     		resultCode;
    TcBtSupplier         	resultSupplier;
    TcBtUint16             	obexPeerMaxPacketSize;
    TcBtPacSrcType     		supportedRepositories;
    TcBtUint32				btConnId;               /* Global Bluetooth connection ID */
	TcBtBdAddr				deviceAddr;
} TcBtPacConnectCfm;

typedef struct
{
    TcBtPacPrim        		type;
    TcBtObexResponseCode   	responseCode;
    TcBtUint16              pbSize;
    TcBtUint8               newMissedCall;
    TcBtUint8              	*payload;
    TcBtUint32              payloadLength;

    TcBtUint8				state;
} TcBtPacPullPbCfm;

typedef struct
{
    TcBtPacPrim        		type;
    TcBtUint16              bodyLength;
    TcBtUint16              bodyOffset;
    TcBtUint8               *payload;
    TcBtUint16              payloadLength;

    TcBtUint8				state;
#if 1/* Telechips' remark */
    TcBtUint8				finalIndFlag;
#endif
} TcBtPacPullPbInd;

typedef struct
{
    TcBtPacPrim        		type;
    TcBtObexResponseCode   	responseCode;
    TcBtUint16              pbSize;
    TcBtUint8               newMissedCall;
    TcBtUint8              	*payload;
    TcBtUint32              payloadLength;
} TcBtPacPullVcardListCfm;

typedef struct
{
    TcBtPacPrim            	type;
    TcBtUint16              bodyLength;
    TcBtUint16              bodyOffset;
    TcBtUint8               *payload;
    TcBtUint16              payloadLength;
} TcBtPacPullVcardListInd;

typedef struct
{
    TcBtPacPrim        		type;
    TcBtObexResponseCode   	responseCode;
    TcBtUint16              pbSize;
    TcBtUint8               newMissedCall;
    TcBtUint8              	*payload;
    TcBtUint32              payloadLength;
} TcBtPacPullVcardEntryCfm;

typedef struct
{
    TcBtPacPrim            	type;
    TcBtUint16              bodyLength;
    TcBtUint16              bodyOffset;
    TcBtUint8               *payload;
    TcBtUint16              payloadLength;
} TcBtPacPullVcardEntryInd;

typedef struct
{
    TcBtPacPrim            	type;
} TcBtPacAbortCfm;

/* Disconnect indication */
typedef struct
{
    TcBtPacPrim            	type;
    TcBtReasonCode     		reasonCode;
    TcBtSupplier           	reasonSupplier;
} TcBtPacDisconnectInd;

typedef struct
{
    TcBtPacPrim        		type;
    TcBtObexResponseCode   	responseCode;
} TcBtPacSetFolderCfm;

typedef struct
{
    TcBtUint8*         		pch;
    TcBtUint32          	newbufferSize;
} VcardDataModify;

typedef struct
{
    int  					year;
    TcBtUint8  				month;
    TcBtUint8  				day;
} TcBtIso8601Date;

typedef struct
{
    TcBtUint8  				hour;
    TcBtUint8  				minute;
    TcBtUint8  				second;
    TcBtUint8  				fraction;
    TcBtInt8   				timezoneHour;
    TcBtUint8  				timezoneMinute;
    TcBtUint8  				flags;
} TcBtIso8601Time;

typedef struct
{
    TcBtIso8601Date date;
    TcBtIso8601Time time;
} TcBtIso8601DateTime;

typedef TcBtUint32 TcBtVcardParamType;

typedef struct
{
    TcBtString *                              type;
    TcBtString *                              string;
} TcBtVcardDataUriString;

typedef struct
{
    TcBtVcardParamType                      addressType;
    TcBtString *                              poBox;
    TcBtString *                              extendedAddress;
    TcBtString *                              street;
    TcBtString *                              locality;
    TcBtString *                              region;
    TcBtString *                              postalCode;
    TcBtString *                              countryName;
} TcBtVcardDataAddress;

typedef struct
{
    TcBtVcardParamType                      type;
    TcBtString *                              string;
} TcBtVcardDataLabel, TcBtVcardDataTypedString;

typedef struct
{
    TcBtString *                              binaryType;
    TcBtUint32                                binarySize;
    TcBtUint8 *                               binaryData;
} TcBtVcardDataBinary;

typedef struct
{
    TcBtString *                              familyName;
    TcBtString *                              givenName;
    TcBtString *                              additionalNames;
    TcBtString *                              honorificPrefixes;
    TcBtString *                              honorificSuffixes;
} TcBtVcardDataName;

typedef struct
{
    TcBtVcardParamType                      type;
    TcBtString *                              number;
} TcBtVcardDataTelephone;

typedef struct
{
    TcBtVcardParamType                      type;
    TcBtIso8601Date                         iso8601Date;
} TcBtVcardDataIso8601Date;

typedef struct
{
    TcBtVcardParamType                      type;
    TcBtIso8601DateTime                     iso8601DateTime;
} TcBtVcardDataIso8601DateTime;

typedef struct
{
    TcBtString *                              organizationalName;
    TcBtString *                              organizationalUnit1;
    TcBtString *                              organizationalUnit2;
} TcBtVcardDataOrganization;

typedef struct TcBtVcardDataTypeRef
{
    struct TcBtVcardDataTypeRef *           next;
    TcBtUint8                               dataType;               /* Datatype for each possible entry is specified below */
    union
    {
        TcBtString *                        string;                 /* VCARD_DATA_TYPE_STRING_UTF8 or VCARD_DATA_TYPE_STRING_UCS2 */
        TcBtVcardDataUriString *            uriString;              /* VCARD_DATA_TYPE_URI_UTF8 or VCARD_DATA_TYPE_URI_UCS2 */
        TcBtVcardDataBinary *               binary;                 /* VCARD_DATA_TYPE_BINARY_UTF8 or VCARD_DATA_TYPE_BINARY_UCS2 */
        TcBtVcardDataAddress *              address;                /* VCARD_DATA_TYPE_ADDRESS_UTF8 or VCARD_DATA_TYPE_ADDRESS_UCS2 */
        TcBtVcardDataName *                 name;                   /* VCARD_DATA_TYPE_NAME_UTF8 or VCARD_DATA_TYPE_NAME_UCS2 */
        TcBtVcardDataTelephone *            telephone;              /* VCARD_DATA_TYPE_TELEPHONE_UTF8 or VCARD_DATA_TYPE_TELEPHONE_UCS2 */
        TcBtIso8601Date *                   iso8601Date;            /* VCARD_DATA_TYPE_ISO8601_DATE */
        TcBtIso8601DateTime *               iso8601DateTime;        /* VCARD_DATA_TYPE_ISO8601_DATETIME */
        TcBtVcardDataLabel *                label;                  /* VCARD_DATA_TYPE_LABEL_UTF8 or VCARD_DATA_TYPE_LABEL_UCS2 */
        TcBtVcardDataTypedString *          typedString;            /* VCARD_DATA_TYPE_TYPED_STRING_UTF8 or VCARD_DATA_TYPE_TYPED_STRING_UCS2 */
        TcBtVcardDataIso8601Date *          iso8601DateTyped;       /* VCARD_DATA_TYPE_ISO8601_DATE_TYPED */
        TcBtVcardDataIso8601DateTime *      iso8601DateTimeTyped;   /* VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED */
        TcBtVcardDataOrganization *         organization;           /* VCARD_DATA_TYPE_ORGANIZATION_UTF8 or VCARD_DATA_TYPE_ORGANIZATION_UCS2 */
        void *                              data;                   /* Entry used to optimize free functionality */
    } payload;
} TcBtVcardDataType;

typedef struct TcBtVcardDataRef
{
#ifndef USE_BTBR
    struct TcBtVcardDataRef *               next; 
#endif
    /* Standard vCard parameters */
    TcBtVcardDataType *                     formattedName;
    TcBtVcardDataType *                     vcardName;
    TcBtVcardDataType *                     name;
    TcBtVcardDataType *                     nickNames;
    TcBtVcardDataType *                     photo;
    TcBtVcardDataType *                     birthDate;
    TcBtVcardDataType *                     source;
    TcBtVcardDataType *                     address;
    TcBtVcardDataType *                     label;
    TcBtVcardDataType *                     tel;
    TcBtVcardDataType *                     email;
    TcBtVcardDataType *                     mailer;
    TcBtVcardDataType *                     timezone;
    TcBtVcardDataType *                     globalPosition;
    TcBtVcardDataType *                     jobTitle;
    TcBtVcardDataType *                     jobRole;
    TcBtVcardDataType *                     jobLogo;
    TcBtVcardDataType *                     jobAgent;
    TcBtVcardDataType *                     organization;
    TcBtVcardDataType *                     catagories;
    TcBtVcardDataType *                     note;
    TcBtVcardDataType *                     productIdentifier;
    TcBtVcardDataType *                     revision;
    TcBtVcardDataType *                     sortString;
    TcBtVcardDataType *                     sound;
    TcBtVcardDataType *                     uid;
    TcBtVcardDataType *                     url;
    TcBtVcardDataType *                     classification;
    TcBtVcardDataType *                     key;

    /* Instant messaging parameters */
    TcBtVcardDataType *                     impp;

    /* IRDA vCard parameters */
    TcBtVcardDataType *                     xIrmcCallDatetime;
    TcBtVcardDataType *                     xIrmcLuid;
#if 1/* Telechips' remark */
    TcBtUint32								callHistoryType;
#endif
} TcBtVcardData;

#define TC_BT_VCARD_FLAGS_DEFAULT                                 (0x0000)
#define TC_BT_VCARD_FLAGS_UCS2_STRINGS                            (0x0001)
#define TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES       (0x0002)
#define TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES       (0x0004)
#define TC_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING              (0x0008)
#define TC_BT_VCARD_FLAGS_NO_MANDATORY_TAGS                       (0x0010)
#define TC_BT_VCARD_FLAGS_VERSION_2_1                             (0x0000)
#define TC_BT_VCARD_FLAGS_VERSION_3_0                             (0x0020)
#define TC_BT_VCARD_FLAGS_NO_VERSION_MATCH                        (0x0040)
#define TC_BT_VCARD_FLAGS_VERSION_SCAN                            (0x0080)
#define TC_BT_VCARD_FLAGS_DISABLE_2_1_TYPE_CHECK                  (0x0100)

#define TC_BT_VCARD_ERROR_SUCCESS                                 (0x00000000)
#define TC_BT_VCARD_ERROR_UNKNOWN_PARAMETER_NAME                  (0x00000001)
#define TC_BT_VCARD_ERROR_UNKNOWN_ATTRIBUTE_NAME                  (0x00000002)
#define TC_BT_VCARD_ERROR_UNKNOWN_ENCODING                        (0x00000004)
#define TC_BT_VCARD_ERROR_INCOMPLETE                              (0x00000008)
#define TC_BT_VCARD_ERROR_VERSION_FOUND                           (0x00000010)
#define TC_BT_VCARD_ERROR_SYNTAX                                  (0x80000020)
#define TC_BT_VCARD_ERROR_INTERNAL_ERROR                          (0x80000040)
#define TC_BT_VCARD_ERROR_PARAMETER_NAME                          (0x80000080)
#define TC_BT_VCARD_ERROR_ATTRIBUTE_NAME                          (0x80000100)
#define TC_BT_VCARD_ERROR_INVALID_ENCODING                        (0x80000200)
#define TC_BT_VCARD_ERROR_INVALID_VALUE                           (0x80000400)
#define TC_BT_VCARD_ERROR_MISSING_MANDATORY_TAGS                  (0x80000800)
#define TC_BT_VCARD_ERROR_WRONG_VERSION                           (0x80001000)
#define TC_BT_VCARD_ERROR_MULTIPLE_VERSION_CONSTRUCT              (0x80002000)
#define TC_BT_VCARD_ERROR_INVALID_PARAMETER                       (0x80004000)

#define TC_BT_VCARD_SUCCEEDED(error) ((error & 0x80000000) != 0x80000000)

#define TC_BT_VCARD_PARAM_TYPE_UNKNOWN       ((TcBtUint32) 0x00000000)
#define TC_BT_VCARD_PARAM_TYPE_HOME          ((TcBtUint32) 0x00000001)
#define TC_BT_VCARD_PARAM_TYPE_WORK          ((TcBtUint32) 0x00000002)
#define TC_BT_VCARD_PARAM_TYPE_PREF          ((TcBtUint32) 0x00000004)
#define TC_BT_VCARD_PARAM_TYPE_VOICE         ((TcBtUint32) 0x00000008)
#define TC_BT_VCARD_PARAM_TYPE_FAX           ((TcBtUint32) 0x00000010)
#define TC_BT_VCARD_PARAM_TYPE_MSG           ((TcBtUint32) 0x00000020)
#define TC_BT_VCARD_PARAM_TYPE_CELL          ((TcBtUint32) 0x00000040)
#define TC_BT_VCARD_PARAM_TYPE_PAGER         ((TcBtUint32) 0x00000080)
#define TC_BT_VCARD_PARAM_TYPE_BBS           ((TcBtUint32) 0x00000100)
#define TC_BT_VCARD_PARAM_TYPE_MODEM         ((TcBtUint32) 0x00000200)
#define TC_BT_VCARD_PARAM_TYPE_CAR           ((TcBtUint32) 0x00000400)
#define TC_BT_VCARD_PARAM_TYPE_ISDN          ((TcBtUint32) 0x00000800)
#define TC_BT_VCARD_PARAM_TYPE_VIDEO         ((TcBtUint32) 0x00001000)
#define TC_BT_VCARD_PARAM_TYPE_PCS           ((TcBtUint32) 0x00002000)
#define TC_BT_VCARD_PARAM_TYPE_INTERNET      ((TcBtUint32) 0x00004000)
#define TC_BT_VCARD_PARAM_TYPE_X400          ((TcBtUint32) 0x00008000)
#define TC_BT_VCARD_PARAM_TYPE_X509          ((TcBtUint32) 0x00010000)
#define TC_BT_VCARD_PARAM_TYPE_PGP           ((TcBtUint32) 0x00020000)
#define TC_BT_VCARD_PARAM_TYPE_DOM           ((TcBtUint32) 0x00040000)
#define TC_BT_VCARD_PARAM_TYPE_INTL          ((TcBtUint32) 0x00080000)
#define TC_BT_VCARD_PARAM_TYPE_POSTAL        ((TcBtUint32) 0x00100000)
#define TC_BT_VCARD_PARAM_TYPE_PARCEL        ((TcBtUint32) 0x00200000)
#define TC_BT_VCARD_PARAM_TYPE_X_NAME        ((TcBtUint32) 0x00400000)
#define TC_BT_VCARD_PARAM_TYPE_STR           ((TcBtUint32) 0x00800000)

#define TCC_MAX_VCARD_NUMBER					500
#define TCC_MAX_VCARD_LIST_NUMBER				500
#define TCC_MAX_CALLHISTORY_NUMBER				500
#define TCC_MAX_CALLHISTORY_FOLDER_NUMBER		5

#define TCC_MAX_VCARD_SIZE						0x3E800 /* 256Kbyte */

#if defined(TCC80XX)
#define TCC_MAX_PHONEBOOK_LIST					2 /* for UI */
#elif defined(TCC801X)
#define TCC_MAX_PHONEBOOK_LIST					9 /* for UI */
#else
#define TCC_MAX_PHONEBOOK_LIST					9 /* for UI */
#endif

#define CONNECTED_STATE 						0x06
//#define TC_BT_OBEX_SUCCESS_RESPONSE_CODE 		0x20
#define TCC_OBEX_PBA_SERVER_PROFILE_UUID       	((unsigned short) 0x112F) //for SyncML
#define TCC_OBEX_PBA_CLIENT_PROFILE_UUID       	((unsigned short) 0x112E) //for PBAP
#define TCC_OBEX_OBJECT_PUSH_SERVICE_UUID      	((unsigned short) 0x1105) //for OPP
#define TCC_HF_PROFILE_UUID       			   	((unsigned short) 0x11E)

/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */
extern int TC_BT_PAC_ActivateCfm(void *pTcBtEvt);
extern int TC_BT_PAC_DeactivateCfm(void *pTcBtEvt);
extern int TC_BT_PAC_ConnectCfm(void *pTcBtEvt);
extern int TC_BT_PAC_PullPbCfm(void *pTcBtEvt);
extern int TC_BT_PAC_PullPbInd(void *pTcBtEvt);
extern int TC_BT_PAC_SetFolderCfm(void *pTcBtEvt);
extern int TC_BT_PAC_PullVcardListInd(void *pTcBtEvt);
extern int TC_BT_PAC_PullVcardListCfm(void *pTcBtEvt);
extern int TC_BT_PAC_PullVcardEntryInd(void *pTcBtEvt);
extern int TC_BT_PAC_PullVcardEntryCfm(void *pTcBtEvt);
extern int TC_BT_PAC_AbortCfm(void *pTcBtEvt);
extern int TC_BT_PAC_DisconnectInd(void *pTcBtEvt);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __TC_BT_PAC_EVT_H__ */
