#ifndef TCBTHFTYPES_H
#define TCBTHFTYPES_H

#include "BT_CFG.h"
#include "BTAPP.h"
#include "BtApp_HF.h"
#ifdef BT_HANDSFREE_UNIT
/* constant */
#define FILE_LFN_LEN    (255 * 2)

/* R: role, C: command, R: result */
#define BT_TEXT_HF_ACTIVATED					"R:Handsfree ", "C:Activate  ", "R:Success   "
#define BT_TEXT_HF_ALREADY_ACTIVATED			"R:Handsfree ", "C:Activate  ", "R:Already   "
#define BT_TEXT_HF_ACTIVATE_FAIL				"R:Handsfree ", "C:Activate  ", "R:Fail      "
#define BT_TEXT_HF_DEACTIVATED					"R:Handsfree ", "C:Deactivate", "R:Success   "
#define BT_TEXT_HF_NOT_ACTIVATED				"R:Handsfree ", "C:Deactivate", "R:Not Active"
#define BT_TEXT_HF_DEACTIVATE_FAIL				"R:Handsfree ", "C:Deactivate", "R:Fail      "

#define BT_TEXT_HF_START_CONNECT				"R:Handsfree ", "C:Connect   ", "R:Start     "
#define BT_TEXT_HF_CONNECTED					"R:Handsfree ", "C:Connect   ", "R:from HFG  "
#define BT_TEXT_HF_CONNECT_FAIL					"R:Handsfree ", "C:Connect   ", "R:Fail(HFG) "
#define BT_TEXT_HF_ALREADY_CONNECTED			"R:Handsfree ", "C:Connect   ", "R:Already   "
#define BT_TEXT_HF_DISCONNECTED					"R:Handsfree ", "C:Disconnect", "R:Success   "
#define BT_TEXT_HF_DISCONNECTED_BY_REMOTE		"R:Handsfree ", "C:Disconnect", "R:from HFG  "
#define BT_TEXT_HF_DISCONNECTED_LINK_LOSS		"R:Handsfree ", "C:Disconnect", "R:Link Loss "
#define BT_TEXT_HF_NOT_CONNECTED				"R:Handsfree ", "C:Disconnect", "R:Not Conn. "
#define BT_TEXT_HF_DISCONNECT_FAIL				"R:Handsfree ", "C:Disconnect", "R:Fail      "

#define BT_TEXT_HF_ANSWER_FROM_HF				"C:Answer    ", "from HF     ", NULL		/* call = 1 after Answer */
#define BT_TEXT_HF_ANSWER_FROM_HFG				"C:Answer    ", "from HFG    ", NULL		/* call = 1 at Ring */

#define BT_TEXT_HF_REJECT_FROM_HF				"C:Reject    ", "from HF     ", NULL		/* callsetup = 0 after Reject */
#define BT_TEXT_HF_REJECT_FROM_HFG				"C:Reject    ", "from HFG    ", NULL		/* callsetup = 0 at Ring */

#define BT_TEXT_HF_TERMINATE_FROM_HF			"C:Terminate ", "from HF     ", NULL	/* call = 0 after Terminate */
#define BT_TEXT_HF_TERMINATE_FROM_HFG			"C:Terminate ", "from HFG    ", NULL	/* call = 0 at Call */

#define BT_TEXT_HF_PLACE_FROM_HF				"C:Place     ", "from HF     ", NULL		/* call = 1 at Dial */
#define BT_TEXT_HF_PLACE_FROM_HFG				"C:Place     ", "from HFG    ", NULL		/* call = 1 at Ring */

#define BT_TEXT_HF_CALL_ALREADY_EXIST			"C:Place     ", "from HFG    ", "R:Already   "		/* Already call */

#define BT_TEXT_HF_VOICE_RECOGNITION_ERR		"HFG does", "not support", "VOICE Recog."

 /* Headset */
#define BT_TEXT_HS_START_CONNECT				"R:Headset   ", "C:Connect   ", "R:Start     "
#define BT_TEXT_HS_CONNECTED					"R:Headset   ", "C:Connect   ", "R:from AG   "
#define BT_TEXT_HS_CONNECT_FAIL					"R:Headset   ", "C:Connect   ", "R:Fail(AG)  "
#define BT_TEXT_HS_ALREADY_CONNECTED			"R:Headset   ", "C:Connect   ", "R:Already   "
#define BT_TEXT_HS_DISCONNECTED					"R:Headset   ", "C:Disconnect", "R:Success   "
#define BT_TEXT_HS_DISCONNECTED_BY_REMOTE		"R:Headset   ", "C:Disconnect", "R:from AG   "
#define BT_TEXT_HS_DISCONNECTED_LINK_LOSS		"R:Headset   ", "C:Disconnect", "R:Link Loss "
#define BT_TEXT_HS_NOT_CONNECTED				"R:Headset   ", "C:Disconnect", "R:Not Conn. "
#define BT_TEXT_HS_DISCONNECT_FAIL				"R:Headset   ", "C:Disconnect", "R:Fail      "

/* Enhanced Call Control*/
#define BT_HF_CIEV_CALL_MAX          2
#define BT_HF_CIEV_CALL_SETUP_MAX    4
#define BT_HF_CIEV_CALL_HELD_MAX     3

#define COM_SUBHEADER_MSG_LENGTH        (9)
#define MAX_STRING_LENGTH               (512)

#if 1	/* Telechips Linux HF remark */
#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ __FILE__
#endif

/* typedef basic type */
typedef	unsigned char		U8;

#ifndef INT8U
    typedef unsigned char		INT8U;
#endif
#ifndef INT8S
    typedef signed	 char		INT8S;
#endif
#ifndef INT16U
    typedef unsigned short		INT16U;
#endif
#ifndef INT16S
    typedef signed	 short		INT16S;
#endif
#ifndef INT32U
    typedef unsigned int		INT32U;
#endif
#ifndef INT32S
    typedef signed	 int		INT32S;
#endif

/* enum */
enum
{
    MSG_SRC_NONE = 0,
    MSG_SRC_BT_HF,
    MSG_SRC_MAX
};

typedef enum _ui_mode
{
    UI_MODE_IDLE,
    UI_MODE_BT_AV,
    UI_MODE_BT_HF,

    UI_MODE_MAX
} UI_MODE;

enum
{
    BT_HF_WINDOW_ACTIVATED,
    BT_HF_WINDOW_NONE
};

enum
{
    BT_HF_UI_MODE_NONE,
    BT_HF_UI_MODE_CONNECTED,
    BT_HF_UI_MODE_ON,
    BT_HF_UI_MODE_MAX
};

typedef enum
{
    WIN_ID_HOME,
    WIN_ID_BTHFAPP,

    WIN_ID_MAX
} WINDOW_ID;

typedef enum {
    /* activate */
    BT_HF_STATUS_DEACTIVATED = 0,
    BT_HF_STATUS_ACTIVATED,

    /* connect */
    BT_HF_STATUS_CONNECT,
    BT_HS_STATUS_CONNECT,

    /* status */
    /* hands-free */
    BT_HF_STATUS_INCOMING,	// ring
    BT_HF_STATUS_OUTGOING,
    BT_HF_STATUS_CALL,
    BT_HF_STATUS_DIAL,
    BT_HF_STATUS_VOICERECOGNITION,

    /* headset */
    BT_HS_STATUS_RING,
    BT_HS_STATUS_AUDIO,

    BT_HF_STATUS_MAX
} BT_HF_STATUS;

typedef enum {
    BT_HF_STATUS_UPDATE_CALLNUMBER = 0,
    BT_HF_STATUS_UPDATE_CALLTIME,
    BT_HF_STATUS_UPDATE_RESPONSEANDHOLD,
    BT_HF_STATUS_UPDATE_ENHANCEDCALL,
    BT_HF_STATUS_UPDATE_VOICERECOGNITION,
    BT_HF_STATUS_UPDATE_VOICERECOGNITION_LEARN,		// [2012-2-29 yspark]
    BT_HF_STATUS_UPDATE_DTMF_NUMBER,
    BT_HF_STATUS_UPDATE_SERVICECONNECTNOTIFICATION
} BT_HF_STATUS_UPDATE;

typedef enum {
    BT_HF_VOICE_VOICERECOGNITION_START = 0,
    BT_HF_VOICE_VOICERECOGNITION_END,
    BT_HF_VOICE_VOICERECOGNITION_LEARN_START,
    BT_HF_VOICE_VOICERECOGNITION_LEARN_END
} BT_HF_VOICE;

typedef enum {
    BT_HF_DIAL_ADD = 0,
    BT_HF_DIAL_DELETE,
    BT_HF_DIAL_DELETE_ALL
} BT_HF_DIAL_STATUS;

/* Enhanced Call Control*/
enum
{
    UI_BT_HF_AT_CMD_NULL = 0,
    UI_BT_HF_AT_CMD_ATA,
    UI_BT_HF_AT_CMD_CHUP,
    UI_BT_HF_AT_CMD_REJECT,
    UI_BT_HF_AT_CMD_AUDIO_TR,
    UI_BT_HF_AT_CMD_CHLD_0,
    UI_BT_HF_AT_CMD_CHLD_1,
    UI_BT_HF_AT_CMD_CHLD_2,
    UI_BT_HF_AT_CMD_MAX
};

enum
{
    UI_BT_HF_CALL_SELECT_ANSWER = 0,
    UI_BT_HF_CALL_SELECT_REJECT,
    UI_BT_HF_CALL_SELECT_MAX
};

/* Main Status */
typedef enum {
    BT_HF_DRAW_STATUS_CALL = 0,
    BT_HF_DRAW_STATUS_INDICATOR_BT_SIGNAL,
    BT_HF_DRAW_STATUS_INDICATOR_BT_MODE,
    BT_HF_DRAW_STATUS_INDICATOR_BATTERY
} BT_HF_DRAW_STATUS;

/* Update(Sub) Status */
typedef enum {
    BT_HF_DRAW_STATUS_UPDATE_CALL_ONHOOK = 0,		/*Idle*/
    BT_HF_DRAW_STATUS_UPDATE_CALL_INCOMING,
    BT_HF_DRAW_STATUS_UPDATE_CALL_OUTGOING,
    BT_HF_DRAW_STATUS_UPDATE_CALL_CALL,
    BT_HF_DRAW_STATUS_UPDATE_CALL_MAX
} BT_HF_DRAW_STATUS_UPDATE_CALL;

/* Indicator must pair with g_stBTHFDispIndicatorInfo registration image count*/
typedef enum {
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_0 = 0,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_1,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_2,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_3,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_4,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_5,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL_MAX
} BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL;

typedef enum {
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_NONE = 0,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_A2DP,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_HF,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_PBAP,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_MAP,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE_MAX
} BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE;

typedef enum {
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_0 = 0,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_1,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_2,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_3,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_4,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_5,
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY_MAX
} BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY;

typedef enum
{
    /* Main Dials */
    BT_HF_MAIN_BTN_DIAL_1 = 0,
    BT_HF_MAIN_BTN_DIAL_2,
    BT_HF_MAIN_BTN_DIAL_3,
    BT_HF_MAIN_BTN_DIAL_4,
    BT_HF_MAIN_BTN_DIAL_5,
    BT_HF_MAIN_BTN_DIAL_6,
    BT_HF_MAIN_BTN_DIAL_7,
    BT_HF_MAIN_BTN_DIAL_8,
    BT_HF_MAIN_BTN_DIAL_9,
    BT_HF_MAIN_BTN_DIAL_EXT1,		/* "*" */
    BT_HF_MAIN_BTN_DIAL_0,							//10
    BT_HF_MAIN_BTN_DIAL_EXT2,		/* "#" */
    BT_HF_MAIN_BTN_DIAL_EXT3,		/* ">" */
    BT_HF_MAIN_BTN_DIAL_A,			/* "+" */
    BT_HF_MAIN_BTN_DIAL_B,			/* "," */
    BT_HF_MAIN_BTN_DIAL_DELETE,	/* delete dial number button */
    BT_HF_MAIN_BTN_DIAL_C,			/* ";" */
    BT_HF_MAIN_BTN_DIAL_D,

    /* Volume */
    BT_HF_MAIN_BTN_VOL_MIC_UP,
    BT_HF_MAIN_BTN_VOL_MIC_DOWN,
    BT_HF_MAIN_BTN_VOL_SPK_UP,
    BT_HF_MAIN_BTN_VOL_SPK_DOWN,

    /* Option */
    BT_HF_MAIN_BTN_OPTION_MENU,
    BT_HF_MAIN_BTN_OPTION_PREV,
    BT_HF_MAIN_BTN_OPTION_SETTING,
    BT_HF_MAIN_BTN_OPTION_HF,
    BT_HF_MAIN_BTN_OPTION_HF2,				/* reserved */

    /* Main Call Functions */
    BT_HF_MAIN_BTN_CALL_FUNC_CALL,
    BT_HF_MAIN_BTN_CALL_FUNC_REJECT,
    BT_HF_MAIN_BTN_CALL_FUNC_END,
    BT_HF_MAIN_BTN_CALL_FUNC_RESERVED_1,
    BT_HF_MAIN_BTN_CALL_FUNC_MUTE,
    BT_HF_MAIN_BTN_CALL_FUNC_ECNS_ONOFF,	/* ECNS of phone */
    BT_HF_MAIN_BTN_CALL_FUNC_AUDIO_ONOFF,
    BT_HF_MAIN_BTN_CALL_FUNC_RESERVED_2,
    BT_HF_MAIN_BTN_CALL_FUNC_CONTACTS,
    BT_HF_MAIN_BTN_CALL_FUNC_MESSAGE,
    BT_HF_MAIN_BTN_CALL_FUNC_VOICE_RECOG,			//37
    BT_HF_MAIN_BTN_CALL_FUNC_VOICE_LEARN,
    //BT_HF_MAIN_BTN_CALL_FUNC_DELETE,		/* removed - moved to dial button */

    /* Add here */
        BT_HF_MAIN_VIRTUAL_BTN_LONG_CALL,
        BT_HF_MAIN_VIRTUAL_BTN_LONG_DIAL_DELETE,
    BT_HF_MAIN_BTN_MAX								//40
} BT_HF_MAIN_BTN_ID;

/* macro*/
#ifndef MAX
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))
#endif

/* structure */
typedef struct
{
    unsigned char group;
    unsigned char type;
    unsigned char src;
    unsigned char id;
} UI_EVENT_TYPE;

typedef struct _TC_BTAPP_DATA
{
    unsigned int mSize;
    void 		*mAddr;
} TC_BTAPP_DATA;

typedef struct BT_HF_MSG_s
{
    /* for Operation */
    unsigned char type;
    unsigned int tickCount;
    unsigned short id;
    unsigned short totalLength;
    const unsigned char *data;
} BT_HF_MSG_t;

typedef struct
{
    INT8U boUpdateSupportedCodec;
} BT_HF_INFO;

/* BT_HF_COMMON_INFO - Clear when disconnected */
typedef struct
{
    INT32U uiAutoAnswerDelay;		/* Unit : second, 0 means disable */
    INT32U uiSpeakerGainIndex;
    INT32U uiMicGainIndex;
    INT32U uiMessageUse;
    INT32U uiInbandRingPriority;	/* 1: Higher than built-in ring, 0: Lower */
#ifdef BT_HFP_MULTI_RING_SUPPORT
    INT32U uiBuiltInRingIdx;
#endif
    INT32U uiEchoOnOffStatus;

    /* Add for Setting Icon of HF Status */
    INT32U uiMicMute;
    INT32U uiAudioOnOff;
    INT32U uiModeStatus;
#ifdef BT_HFP_WIDEBAND_SPEECH
    INT8U uiCodec;
#endif

    INT8U ucSupportThreeWayCalling;
    INT8U ucSupportEnhancedCallStatus;
    INT8U ucSupportEnhancedCallControl;
    INT8U ucSupportChld;
    INT8U ucSupportVoiceRecognitionAbility;
} BT_HF_COMMON_INFO;

typedef struct
{
    INT8U ucServiceSupport, ucServiceIndex;
    INT8U ucRoamSupport, ucRoamIndex;
#if 1 /* Telechips' 2008.12 QA - 3.2 */
    INT8U ucSignalSupport, ucSignalIndex;
    INT8U ucMessageSupport, ucMessageIndex;
    INT8U ucBattchgSupport, ucBattchgIndex;
#endif
    TcBtBdAddr 		stCurrRemoteAddr;
    unsigned long 	ulBTHfRemoteVersion;
    unsigned char	ucBTRemoteFriendlyName[TC_BT_MAX_FRIENDLY_NAME_LEN+1];
    //TcBtBdAddr 		stBTRemoteDevAddr;
} BT_HF_DEVICE_INFO;

typedef struct
{
    //INT8U ucPos;
    INT8U aucDtmfNumber[BTAPP_HF_DTMFNUM_MAX_LEN];
} BT_HF_DTMF_INFO;

typedef struct
{
    INT8U ucPos;
    INT8U ucCallNumberPos;
    INT8U aucCallNumber[BTAPP_HF_CALLNUM_MAX_LEN];
} BT_HF_DIAL_INFO;

/* BT_HF_STATUS_INFO - Clear when Poweroff */
typedef struct
{
    INT8U aucCallNumber[BTAPP_HF_CALLNUM_MAX_LEN];
    INT8U boUnicodeOrNot;			/* 0:ASCII, 1:Unicode */
    INT8U ucCall;
    INT8U ucCallSetup;
    INT8U ucCallPrevSetup;
    INT8U ucCallHeld;
    INT8U ucCallPrevHeld;

    struct
    {
        INT8U ucInbandRing;
        INT8U ucInbandRingType;			/* 1: ACL, 0: SCO */
        INT8U ucRejectAbility;

        INT8U ucResponseAndHoldControlIdx;

        INT8U aucCcwaNumber[BTAPP_HF_CALLNUM_MAX_LEN];
        INT8U boCcwaUnicodeOrNot;		/* 0:ASCII, 1:Unicode */
    } INCOMING;

    struct
    {
        INT32U uiReserved;
    } OUTGOING;

    struct
    {
        INT32U uiCallSec;

        INT8U ucEnhancedCallControlIdx;
        INT8U boService;
    } CALL;

    struct
    {
        /* voice regcognition */
        INT8U ucVoiceRecognition;
        INT8U ucVoiceRecognitionLearning;
    } VOICERECOGNITION;
} BT_HF_STATUS_INFO;

typedef struct
{
    BT_HF_DRAW_STATUS_UPDATE_CALL 					ucUpdateCallStatus;
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_SIGNAL	ucUpdateIndicatorBTSignal;
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BT_MODE	 	ucUpdateIndicatorBTMode;
    BT_HF_DRAW_STATUS_UPDATE_INDICATOR_BATTERY 		ucUpdateIndicatorBattery;
} BT_HF_MAIN_DRAW_INFO;
#endif//BT_HANDSFREE_UNIT
#endif  // TCBTHFTYPES_H
