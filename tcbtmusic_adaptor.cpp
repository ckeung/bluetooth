/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -i tcbtmusic_adaptor.h -a :tcbtmusic_adaptor.cpp ../dbus_xml_config/Semisky.BlueToothMidware.TcBtMusic.xml
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "tcbtmusic_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class TcBtMusicAdaptor
 */

TcBtMusicAdaptor::TcBtMusicAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

TcBtMusicAdaptor::~TcBtMusicAdaptor()
{
    // destructor
}

void TcBtMusicAdaptor::SetRepeatMode(int mode)
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.SetRepeatMode
    QMetaObject::invokeMethod(parent(), "SetRepeatMode", Q_ARG(int, mode));
}

void TcBtMusicAdaptor::SetShuffleMode(int mode)
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.SetShuffleMode
    QMetaObject::invokeMethod(parent(), "SetShuffleMode", Q_ARG(int, mode));
}

void TcBtMusicAdaptor::closeBtMusic()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.closeBtMusic
    QMetaObject::invokeMethod(parent(), "closeBtMusic");
}

void TcBtMusicAdaptor::playFF()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playFF
    QMetaObject::invokeMethod(parent(), "playFF");
}

void TcBtMusicAdaptor::playNext()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playNext
    QMetaObject::invokeMethod(parent(), "playNext");
}

void TcBtMusicAdaptor::playPre()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playPre
    QMetaObject::invokeMethod(parent(), "playPre");
}

void TcBtMusicAdaptor::playRew()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playRew
    QMetaObject::invokeMethod(parent(), "playRew");
}

void TcBtMusicAdaptor::playStart()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playStart
    QMetaObject::invokeMethod(parent(), "playStart");
}

void TcBtMusicAdaptor::playStop()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.playStop
    QMetaObject::invokeMethod(parent(), "playStop");
}

void TcBtMusicAdaptor::requeseBtMusicAudioSource()
{
    // handle method call smk.tcc8971.BlueToothMidware.TcBtMusic.requeseBtMusicAudioSource
    QMetaObject::invokeMethod(parent(), "requeseBtMusicAudioSource");
}

