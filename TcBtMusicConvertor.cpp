#include "TcBtMusicConvertor.h"
#include "tcbtmusic_adaptor.h"
#include "TcBtMusic.h"
TcBtMusicConvertor* TcBtMusicConvertor::m_instance = nullptr;
TcBtMusicConvertor* TcBtMusicConvertor::getIntance()
{
    if (!m_instance)
    {
        m_instance = new TcBtMusicConvertor();
    }
    return m_instance;
}

TcBtMusicConvertor::TcBtMusicConvertor() :
    m_adaptor(new TcBtMusicAdaptor(this))
{
    setProperty("interfaceClassName", "TcBtMusic");

    connect(g_TcBtMusic, SIGNAL(btavSinkConnected()), SIGNAL(avSinkConnected()));
    connect(g_TcBtMusic, SIGNAL(btavSinkDisConnected()), SIGNAL(avSinkDisConnected()));
    connect(g_TcBtMusic, SIGNAL(btgetMetaDataAlbum(QString)), SIGNAL(getMetaDataAlbum(QString)));
    connect(g_TcBtMusic, SIGNAL(btgetMetaDataArtist(QString)), SIGNAL(getMetaDataArtist(QString)));
    connect(g_TcBtMusic, SIGNAL(btgetMetaDataGenre(QString)), SIGNAL(getMetaDataGenre(QString)));
    connect(g_TcBtMusic, SIGNAL(btgetMetaDataTitle(QString)), SIGNAL(getMetaDataTitle(QString)));
    connect(g_TcBtMusic, SIGNAL(btgetCurrentPostion(int)), SIGNAL(getCurrentPostion(int)));
    connect(g_TcBtMusic, SIGNAL(btgetPlayStatus(int)), SIGNAL(getPlayStatus(int)));
    connect(g_TcBtMusic, SIGNAL(btgetTotalLength(int)), SIGNAL(getTotalLength(int)));
    connect(g_TcBtMusic, SIGNAL(btgetTotalTime(int)), SIGNAL(getTotalTime(int)));
     connect(g_TcBtMusic, SIGNAL(btchangeRepeatMode(int)), SIGNAL(changeRepeatMode(int)));
      connect(g_TcBtMusic, SIGNAL(btchangeShuffleMode(int)), SIGNAL(changeShuffleMode(int)));
}

TcBtMusicConvertor::~TcBtMusicConvertor()
{

}
void TcBtMusicConvertor::closeBtMusic()
{
    g_TcBtMusic->closeBtMusic();
}
void TcBtMusicConvertor::openBtMusic()
{
    g_TcBtMusic->openBtMusic();
}

void TcBtMusicConvertor::requeseBtMusicAudioSource()
{
    g_TcBtMusic->requeseBtMusicAudioSource();
}
void TcBtMusicConvertor::playFF()
{
    fprintf(stderr, "btapp service playFF\n");
    g_TcBtMusic->cmdMusicFF();
}
void TcBtMusicConvertor::playNext()
{
     fprintf(stderr, "btapp service playNext\n");
    g_TcBtMusic->cmdMusicNext();
}
void TcBtMusicConvertor::playPre()
{
    fprintf(stderr, "btapp service playPre\n");
   g_TcBtMusic->cmdMusicPre();
}
void TcBtMusicConvertor::playRew()
{
    fprintf(stderr, "btapp service playRew\n");
   g_TcBtMusic->cmdMusicREW();
}
void TcBtMusicConvertor::playStart()
{
    fprintf(stderr, "btapp service playStart\n");
   g_TcBtMusic->cmdMusicStart();
}
void TcBtMusicConvertor::playStop()
{
    fprintf(stderr, "btapp service playStop\n");
   g_TcBtMusic->cmdMusicStop();
}
void TcBtMusicConvertor::SetRepeatMode(int mode)
{
    fprintf(stderr, "btapp service SetRepeatMode\n");
   g_TcBtMusic->cmdSetRepeatMode(mode);
}
void TcBtMusicConvertor::SetShuffleMode(int mode)
{
    fprintf(stderr, "btapp service SetShuffleMod\n");
   g_TcBtMusic->cmdSetShuffleMode(mode);
}
