#ifndef BTBASE_H
#define BTBASE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "TC_BT_EVT.h"
/*****************************************************************************
*
* miscellaneous
*
*****************************************************************************/
#ifndef  FALSE
#define  FALSE                     0
#endif

#ifndef  TRUE
#define  TRUE                      1
#endif

/*****************************************************************************
*
* data types
*
*****************************************************************************/
/* TODO: Add your specialized code here */
enum BT_AUDIO_MODE
{
    BIDLE=0,
    B_BT_A2DP_SINK,
    B_BT_HFP_RING,
    B_BT_HFP_CALL,
    B_BT_HSP_AUDIO,
    MODE_MAX
};
typedef enum
{
    TC_BT_SYS_HW_CLEAR_WARM_RESET_IND = TC_BT_EVT_TYPE_ENUM_BASE, /* TC_BT_EVT_TYPE_ENUM_BASE */
    TC_BT_SYS_HW_SET_WARM_RESET_IND,
    TC_BT_SYS_BSA_ACTIVATE_FAIL_CFM,
    TC_BT_SYS_BSA_ACTIVATE_SUCCESS_CFM,
    TC_BT_SYS_ACTIVATE_FAIL_CFM,
    TC_BT_SYS_ACTIVATE_SUCCESS_CFM,
    TC_BT_SYS_DEACTIVATE_CFM,
    TC_BT_SYS_NOT_SUPPORT_CMD_CFM,

    TC_BT_SYS_EVT_TYPE_MAX
} TcBtSYSEvtEnum;

/*****************************************************************************
*
* definitions
*
*****************************************************************************/
#define BTAPP_MAX_FRIENDLY_NAME_LEN 128	// TC_BT_MAX_FRIENDLY_NAME_LEN

/* result for connection related command */
#define BTAPP_SYS_RESULT_NONE			((unsigned short int )0x0000)
#define BTAPP_SYS_RESULT_DONE			((unsigned short int )0x0001)
#define BTAPP_SYS_RESULT_BUSY			((unsigned short int )0x0002)
#define BTAPP_SYS_RESULT_ERROR			((unsigned short int )0x0003)
#define BTAPP_SYS_RESULT_TIMEOUT		((unsigned short int )0x0004)

#define TC_BT_SOCKET_PATH "/var/run/"
#define BT_HFP_MAX_CONNECTIONS 1
#define BT_PBAP_MAX_CONNECTIONS 1
/*****************************************************************************
*
* structures
*
*****************************************************************************/

/* status */
typedef enum
{
    BTAPP_STATUS_GROUP_COMMON=0,
    BTAPP_STATUS_GROUP_AVRCP,
    BTAPP_STATUS_GROUP_AV_SRC,
    BTAPP_STATUS_GROUP_AV_SNK,
    BTAPP_STATUS_GROUP_HFx,
    BTAPP_STATUS_GROUP_PHONEBOOK=BTAPP_STATUS_GROUP_HFx+BT_HFP_MAX_CONNECTIONS,
    BTAPP_STATUS_GROUP_ATCMD=BTAPP_STATUS_GROUP_PHONEBOOK+BT_PBAP_MAX_CONNECTIONS,
    BTAPP_STATUS_GROUP_OPS,
    BTAPP_STATUS_GROUP_SMLC,
    BTAPP_STATUS_GROUP_MAPC,
    BTAPP_STATUS_GROUP_PROXC,

    BTAPP_STATUS_GROUP_MAX
}BTAPP_STATUS_GROUP;

typedef enum
{
    TC_BT_SD_SEARCH_REQ = 0,
    TC_BT_SD_CANCEL_SEARCH_REQ,
    TC_BT_SD_READ_AVAILABLE_SERVICES_REQ,
    TC_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ,
    /* Device Identification */
    TC_BT_SD_DI_REGISTER_SERVICE_RECORD_REQ,
    TC_BT_SD_DI_READ_SERVICE_RECORD_REQ,
    TC_BT_SD_DI_CANCEL_READ_SERVICE_RECORD_REQ
} TcBtSDCmdEnum;
/*****************************************************************************
*
* definitions
*
*****************************************************************************/
/* TODO: Add your specialized code here */

typedef struct
{
    unsigned short int				type;
    unsigned short int 			MajorCmdType;
    unsigned short int				MinorCmdType;
} TcBtSysNotSupportFuncEvt;

typedef TcBtUint16 TcBtCmdType;

typedef struct
{
    TcBtCmdType			type;
} TcBtSdCmd;

typedef struct
{
    TcBtCmdType			type;
    TcBtUint32			totalSearchTime;
    TcBtUint32			maxDevices;
    TcBtUint8			update;
} TcBtSdSearchCmd;

typedef struct
{
    TcBtCmdType			type;
    TcBtBdAddr			deviceAddr;
} TcBtSdReadAvailableServicesCmd, TcBtSdDiReadServiceRecordCmd;

typedef struct
{
    TcBtCmdType			type;
    TcBtUint16  		vendorId;
    TcBtUint16  		productId;
    TcBtUint16  		version;
    TcBtBool    		primaryRecord;
    TcBtUint16  		vendorIdSource;
    TcBtUint8  			*pServiceDescription;
    TcBtUint16  		serviceDescriptionLen;
    TcBtUint8  			*pClientExecutableUrl;
    TcBtUint16  		clientExecutableUrlLen;
    TcBtUint8  			*pDocumentationUrl;
    TcBtUint16  		documentationUrlLen;
} TcBtSdDiRegisterServiceRecordCmd;
/***********
*	mask
***********/
/* format - xx : src (1byte) , xx : devno (1byte), xxxx : status (2bytes) */
#define BTAPP_STATUS_MASK(group,devno,status)		((unsigned int)(((group) << 24) 		\
                                                    | ((unsigned int)(devno) << 16) 		\
                                                    | ((unsigned int)(status))))

#define BTAPP_STATUS_MASK_SET_GROUP(mask,group)		((unsigned int)(((mask) & 0x00ffffff) 	\
                                                    | ((unsigned int)(group) << 24)))

#define BTAPP_STATUS_MASK_SET_DEVNO(mask,devno)		((unsigned int)(((mask) & 0xff00ffff) 	\
                                                    | ((unsigned int)(devno) << 16)))

#define BTAPP_STATUS_MASK_SET_STATUS(mask,status)	((unsigned int)(((mask) & 0xffff0000) 	\
                                                    | ((unsigned int)(status))))

#define BTAPP_STATUS_MASK_GET_GROUP(mask)			(unsigned int)(((mask) >> 24) & 0xff)
#define BTAPP_STATUS_MASK_GET_DEVNO(mask)			(unsigned int)(((mask) >> 16) & 0xff)
#define BTAPP_STATUS_MASK_GET_STATUS(mask)			(unsigned int)(((mask) & 0xffff))

/***********
*	enum
***********/
#define	BTAPP_STATUS_HW_AVAILABLE					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0001)
#define	BTAPP_STATUS_TASK_INIT						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0002)
#define	BTAPP_STATUS_AV_SOURCE_ACTIVATE				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0004)
#define	BTAPP_STATUS_AV_SINK_ACTIVATE				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0008)
#define	BTAPP_STATUS_HF_ACTIVATE					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0010)

#define	BTAPP_STATUS_AVRCP_CTRL_CONNECT				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AVRCP,0,0x0001)
#define	BTAPP_STATUS_AVRCP_TARGET_CONNECT			BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AVRCP,0,0x0002)

#define	BTAPP_STATUS_AV_SOURCE_CONNECT				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SRC,0,0x0001)
#define	BTAPP_STATUS_AV_SOURCE_OPEN					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SRC,0,0x0002)
#define	BTAPP_STATUS_AV_SOURCE_STREAM				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SRC,0,0x0004)

#define	BTAPP_STATUS_AV_SINK_CONNECT				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SNK,0,0x0001)
#define	BTAPP_STATUS_AV_SINK_OPEN					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SNK,0,0x0002)
#define	BTAPP_STATUS_AV_SINK_STREAM					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_AV_SNK,0,0x0004)


#define	BTAPP_STATUS_HFx_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0001)
#define	BTAPP_STATUS_HFx_AUDIO						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0002)
#define	BTAPP_STATUS_HFx_RING						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0004)
#define	BTAPP_STATUS_HFx_OUTGOING					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0008)
#define	BTAPP_STATUS_HFx_OUTGOING_BY_HF				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0010)
#define	BTAPP_STATUS_HFx_CALL						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0020)
#define	BTAPP_STATUS_HSx_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0040)
#define	BTAPP_STATUS_HSx_RING						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0080)
#define	BTAPP_STATUS_HSx_AUDIO						BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0100)
#define	BTAPP_STATUS_HFx_CONNECT_NOTIFICATION		BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_HFx,0,0x0200)


#define	BTAPP_STATUS_PB_PAC_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0001)
#define	BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD		BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0002)
#define	BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_LIST	BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0004)
#define	BTAPP_STATUS_PB_PAC_DOWNLOADING_VCARD_ENTRY	BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0008)
#define	BTAPP_STATUS_PB_PAC_PARSING					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0010)
#define	BTAPP_STATUS_PB_PAC_DOWNLOADED				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PHONEBOOK,0,0x0020)

#define	BTAPP_STATUS_PB_OPS_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_OPS,0,0x0001)
#define	BTAPP_STATUS_PB_OPS_DOWNLOADING_VCARD		BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_OPS,0,0x0002)
#define	BTAPP_STATUS_PB_OPS_DOWNLOADED				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_OPS,0,0x0004)

#define	BTAPP_STATUS_PB_ATC_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_ATCMD,0,0x0001)
#define	BTAPP_STATUS_PB_ATC_DOWNLOADING_VCARD		BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_ATCMD,0,0x0002)
#define	BTAPP_STATUS_PB_ATC_DOWNLOADED				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_ATCMD,0,0x0004)

#define	BTAPP_STATUS_PB_SML_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_SMLC,0,0x0001)
#define	BTAPP_STATUS_PB_SML_DOWNLOADING_VCARD		BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_SMLC,0,0x0002)
#define	BTAPP_STATUS_PB_SML_DOWNLOADED				BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_SMLC,0,0x0004)

#define	BTAPP_STATUS_MAPC_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_MAPC,0,0x0001)

#define	BTAPP_STATUS_PROXC_REGISTER					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_COMMON,0,0x0020)
#define	BTAPP_STATUS_PROXC_CONNECT					BTAPP_STATUS_MASK(BTAPP_STATUS_GROUP_PROXC,0,0x0001)

#define	BTAPP_STATUS_ALL							0xffffffff

/***********
*	app
***********/
#define BTAPP_STATUS_SET_HFP(status)				BTAPP_STATUS_MASK_SET_DEVNO(status		\
                                                   ,BTAPP_STATUS_GET_HFP_CUR_DEVNO())
#define BTAPP_STATUS_SET_PBAP(status)				BTAPP_STATUS_MASK_SET_DEVNO(status		\
                                                        ,BtApp_PB_GetDeviceId())

#define BTAPP_STATUS_GET_HFP_CUR_DEVNO()			BtApp_HF_GetCurrDevNum()

/****************************************************************************

Bluetooth LCD UPDATE FLAG - Group ID

*****************************************************************************/
/***********
*	mask
***********/
#define BTAPP_EVT_MASK_SET_DEVNO(mask,devno)		((unsigned int)(((mask) & EVT_UPDATE_MASK) 	\
                                                    | ((unsigned int)((devno)) << 16)))
#define BTAPP_EVT_MASK_GET_EVT(mask)				(unsigned int)((mask) & EVT_UPDATE_MASK)
#define BTAPP_EVT_MASK_GET_DEVNO(mask)				(unsigned int)(((mask) & EVT_VALUE_MASK) >> 16)



/*****************************************************************************
*
* externs
*
*****************************************************************************/
/* TODO: Add your specialized code here */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // BTBASE_H
