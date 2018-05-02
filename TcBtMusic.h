#ifndef TCBTMUSIC_H
#define TCBTMUSIC_H
#include "TC_BT/inc/BtUtil.h"
#include "inc/BtBase.h"
#include "inc/BtDataType.h"
#include <QObject>

namespace Ui {
class TcBtMusic;
}
class TcBtMusicSignal : public QObject
{
    Q_OBJECT

public:
    TcBtMusicSignal() {}
    ~TcBtMusicSignal() {}

signals:
    void AV_Event(int type, void *pTcBtEvt);

};
class TcBtMusic : public QObject
{
    Q_OBJECT
public:
        explicit TcBtMusic(QObject *parent = 0);
     ~TcBtMusic();
        static TcBtMusic* getInstance();
#if defined(g_TcBtMusic)
#undef g_TcBtMusic
#endif
#define  g_TcBtMusic  (TcBtMusic::getInstance())
   private:
signals:
    void btavSinkConnected();
    void btavSinkDisConnected();
    void btgetCurrentPostion(int postion);
    void btgetMetaDataAlbum(const QString &album);
    void btgetMetaDataArtist(const QString &artist);
    void btgetMetaDataGenre(const QString &genre);
    void btgetMetaDataTrack(int track);
    void btgetMetaDataTitle(const QString &title);
    void btgetPlayStatus(int status);
    void btgetTotalLength(int time);
    void btgetTotalTime(int len);
    void btchangeRepeatMode(int mode);
    void btchangeShuffleMode(int mode);
    void btRequestBtMusicAudioSource();
    void btReleaseBtMusicAudioSource();
public slots:
        void OnAVHandler(int type, void *pTcBtEvt);
public:
     TcBtMusicSignal *m_BtmusicSignal;
     unsigned char		AvrcpPasCurrValue[5];
     unsigned char		AvrcpPasValidList[5];
     bool openBtMusic(void);
     void requeseBtMusicAudioSource();
     void releaseBtMusicAudioSource();
     bool closeBtMusic(void);
    void cmdMusicStart(void);
    void cmdMusicStop(void);
    void cmdMusicNext(void);
    void cmdMusicPre(void);
    void cmdMusicFF(void);
    void cmdMusicREW(void);
    void cmdSetRepeatMode(int mode);
    void cmdSetShuffleMode(int mode);

};

#endif // TCBTMUSIC_H
