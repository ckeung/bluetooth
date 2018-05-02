#ifndef TCBTMUSICCONERTOR_H
#define TCBTMUSICCONERTOR_H

#include <QObject>
class TcBtMusicAdaptor;
class TcBtMusic;
class TcBtMusicConvertor : public QObject
{
    Q_OBJECT
public:
   static TcBtMusicConvertor* getIntance();

signals:
   void avSinkConnected();
   void avSinkDisConnected();
    void getCurrentPostion(int postion);
    void getMetaDataAlbum(const QString &album);
    void getMetaDataArtist(const QString &artist);
    void getMetaDataGenre(const QString &genre);
    void getMetaDataTitle(const QString &title);
    void getPlayStatus(int status);
    void getTotalLength(int length);
    void getTotalTime(int length);
    void changeRepeatMode(int mode);
    void changeShuffleMode(int mode);
public slots:
    void closeBtMusic();
    void requeseBtMusicAudioSource();
    void openBtMusic();
    void playFF();
    void playNext();
    void playPre();
    void playRew();
    void playStart();
    void playStop();
    void SetRepeatMode(int mode);
    void SetShuffleMode(int mode);
private:
    TcBtMusicConvertor();
    ~TcBtMusicConvertor() override;
    TcBtMusicConvertor(const TcBtMusicConvertor&) = delete;
    TcBtMusicConvertor(TcBtMusicConvertor&&) = delete;
    TcBtMusicConvertor& operator= (const TcBtMusicConvertor&) = delete;
    TcBtMusicConvertor& operator= (TcBtMusicConvertor&&) = delete;
    TcBtMusicAdaptor* m_adaptor;
    static TcBtMusicConvertor* m_instance;
};

#endif // TCBTMUSICCONERTOR_H
