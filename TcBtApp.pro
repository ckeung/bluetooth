#-------------------------------------------------
#
# Project created by QtCreator 2014-11-26T13:59:50
#
#-------------------------------------------------

QT += gui

greaterThan(QT_MAJOR_VERSION, 4) { 
    QT += widgets
    QT.gui.CONFIG -= opengl
}

TEMPLATE = app
DESTDIR = bin
CONFIG += release

DEFINES -= UNICODE \
    _UNICODE

CONFIG(debug, debug|release) { 
    OBJECTS_DIR = build/debug/obj
    MOC_DIR = build/debug/moc

}
else { 
    OBJECTS_DIR = build/release/obj
    MOC_DIR = build/release/moc

}
        #DEFINES += LINUX_3_4
DEFINES += LINUX_3_18
DBUS_XML_CONFIG_PATH = $(PWD)/../../../../../../dbus_xml_config

!exists($$DBUS_XML_CONFIG_PATH) {
DBUS_XML_CONFIG_PATH = $(BUILDDIR)/dbus_xml_config

!exists($$DBUS_XML_CONFIG_PATH) {
error("Could not found dbus_xml_config path!!");
}}

include($$DBUS_XML_CONFIG_PATH/DBusXMLConfig_BlueToothMidware.pri) {
    DBUS_ADAPTORS += $$DBUS_BLUETOOTH_MIDWARE_XML_FILES
}
else {
    error("Could not load DBusXMLConfig_BlueToothMidware.pri");
}
include($$DBUS_XML_CONFIG_PATH/DBusXMLConfig_AudioManageMidware.pri) {
    DBUS_INTERFACES += $$DBUS_AUDIO_MANAGE_XML_FILES
}
else {
    error("Could not load DBusXMLConfig_AudioManageMidware.pri");
}
TARGET = SmkBluetoothMidware

SOURCES += main.cpp\
     BTAPP/BtSys.cpp \
    BTAPP/BtThread.cpp \
    BTAPP/BtApp_Gap.cpp \
     TC_BT/BtSysCmd.cpp\
    TC_BT/BtPlatform.cpp \
    TC_BT/BtNvDbFile.cpp \
    TC_BT/BtMsgCmd.cpp \
    BTAPP/BtDeviceManage.cpp \
    TC_BT/BtDmCmd.cpp \
    TC_BT/BtCmCmd.cpp \
    TC_BT/BtPacCmd.cpp \
    BTAPP/BtPhoneBookEvt.cpp \
    BTAPP/BtPhoneBook.cpp \
    BTAPP/BtPhoneBookCmd.cpp \
    TC_BT/BtHfDev.cpp \
    TC_BT/BtHfSco.cpp \
    TC_BT/BtHfCmd.cpp\
    BTAPP/BtApp_HF.cpp\
   BTAPP/BtHandFree.cpp \
   BTAPP/BtApp_HfEvt.cpp \
    TC_BT/BtA2dpCmd.cpp \
    TC_BT/BtAvRcpCmd.cpp \
    BTAPP/BtApp_Av.cpp \
    BTAPP/BtA2dpEvent.cpp \
    BTAPP/BtAvRcpEvent.cpp \
    BTAPP/BtHfEvent.cpp \
    BTAPP/TcBtEvt.cpp \
    BTAPP/TcBtHFEvt.cpp \
    TcBtPhone.cpp \
    TcBtMusic.cpp\
    TcBtPhoneConvertor.cpp \
    BlueToothMidwareDBusRegistant.cpp \
    TcBtMusicConvertor.cpp \
    VolumeManagerData.cpp \
    VolumeManager.cpp

HEADERS  += \
    inc/BtBase.h\
    inc/BtDataType.h\
    BTAPP/inc/BT_CFG.h \
    BTAPP/inc/BtThread.h \
    BTAPP/inc/BtSys.h \
    TC_BT/inc/BtBase.h \
    TC_BT/inc/BtSysCmd.h \
    TC_BT/inc/BtPlatform.h\
    TC_BT/inc/TC_BT_DATATYPE.h \
    TC_BT/inc/TC_BT_NV_DB_FILE.h \
    TC_BT/inc/TC_BT_UTIL.h\
    TC_BT/inc/BtUtil.h \
    TC_BT/inc/BtDataType.h \
    TC_BT/inc/BtNvDbFile.h \
    TC_BT/inc/TC_BT_EVT.h \
    TC_BT/inc/TC_BT_SD_EVT.h \
    BTAPP/inc/BTAPP.h \
    BTAPP/inc/BtApp_Gap.h \
    TC_BT/inc/TC_BT_CMD.h \
    BTAPP/inc/BtDeviceManage.h \
    TC_BT/inc/TC_BT_SC_CMD.h \
    TC_BT/inc/BtDmCmd.h \
    TC_BT/inc/TC_BT_SC_EVT.h \
    TC_BT/inc/TC_BT_CM_CMD.h \
    TC_BT/inc/TC_BT_CM_EVT.h \
    TC_BT/inc/TC_BT_SYS_CMD.h \
    TC_BT/inc/TC_BT_PAC_CMD.h \
    TC_BT/inc/BtPacCmd.h \
    BTAPP/inc/BtPhoneBook.h \
    BTAPP/inc/BtPhoneBookEvt.h \
    BTAPP/inc/BtPhoneBookCmd.h \
    TC_BT/inc/TC_BT_PAC_EVT.h \
    TC_BT/inc/TC_BT_HF_CMD.h \
    TC_BT/inc/TC_BT_HF_DEV.h \
    TC_BT/inc/TC_BT_HF_EVT.h \
    TC_BT/inc/TC_BT_HF_UTIL.h \
    TC_BT/inc/TC_BT_HF_SCO.h \
    TC_BT/inc/BtHfCmd.h \
    BTAPP/inc/BtApp_HF.h \
    BTAPP/inc/BtHandFree.h \
    BTAPP/inc/BtApp_HfEvt.h\
    Codec/HFRing/hf_ring1_8khz_mono.h \
    Codec/HFRing/hf_ring2_8khz_mono.h \
    Codec/HFRing/hf_ring3_8khz_mono.h \
    Codec/HFRing/hfring.h \
    Codec/HFRing/hfringtype.h \
    TC_BT/inc/TC_BT_A2DP_CMD.h \
    TC_BT/inc/TC_BT_AVRCP_CMD.h \
    BTAPP/inc/BtApp_Av.h \
    TC_BT/inc/TC_BT_A2DP_EVT.h \
    TC_BT/inc/TC_BT_AVRCP_EVT.h \
    BTAPP/inc/BtA2dpEvent.h \
    BTAPP/inc/BtAvRcpEvent.h \
    BTAPP/inc/BtHfEvent.h \
    BTAPP/inc/TcBtEvt.h \
    BTAPP/inc/TcBtHFEvt.h \
    BTAPP/inc/TcBtHFTypes.h \
    TcBtPhone.h \
    TcBtMusic.h\
    TcBtPhoneConvertor.h \
    BlueToothMidwareDBusRegistant.h \
    TcBtMusicConvertor.h \
    VolumeManagerData.h \
    VolumeManager.h

INCLUDEPATH += ./BTAPP/inc
INCLUDEPATH += ./TC_BT/inc
INCLUDEPATH += ./TC_BT_SUB
INCLUDEPATH += ./TC_BT_DBUS
INCLUDEPATH += ./Codec/AAC/include
INCLUDEPATH += ./Codec/AEC
INCLUDEPATH += ./Codec/AEC/include
INCLUDEPATH += ./Codec/HFRing

LIBS += -L$$PWD/LIB/ -lTcBt
LIBS += -L$$PWD/Codec/HFRing -lhfring
LIBS += -L$$PWD/Codec/AAC/lib/  -lTCC_ARMv7_AACDEC_LINUX_V3.21.09
LIBS += -L$$PWD/Codec/AAC/lib/  -lTCCxxxx_LATMDMX_LINUX_V2.00.03
LIBS += -lasound -lpthread -lrt -ldbus-1 -ltcutils -lglib-2.0

#LIBS += -L$$PWD/../../telechips_sdk/sysroots/cortexa7-vfp-neon-telechips-linux-gnueabi/usr/lib -ltcutils
#INCLUDEPATH += $$PWD/../../telechips_sdk/sysroots/cortexa7-vfp-neon-telechips-linux-gnueabi/usr/include

LIBS += -L$$PWD/Codec/AEC/ -laeclessaudio
DEFINES += BT_AEC_INCLUDE
SOURCES += Codec/AEC/source/aecif_aecless.c
HEADERS  += Codec/AEC/include/aecif_aecless.h


HEADERS  += Codec/AAC/include/common.h \
        Codec/AAC/include/adec.h \
        Codec/AAC/include/tcc_latm_dmx.h


DEFINES += USE_BTBR

target.path = /usr/bin
INSTALLS += target
