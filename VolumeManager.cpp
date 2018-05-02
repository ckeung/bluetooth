#include "VolumeManager.h"
#include "TcBtPhone.h"
#include "TcBtMusic.h"
#include <QMutex>
#include <QMap>

QMutex VolumeManager::m_mutex;
VolumeManager* VolumeManager::m_instances;

#include <QException>
VolumeManager* VolumeManager::instance( ) {

    if(!m_instances) {
        m_mutex.lock();
        if(!m_instances) {
            m_instances = new VolumeManager();
        }
        m_mutex.unlock();
    }
    return m_instances;
}


#include "VolumeManagerData.h"

VolumeManager::VolumeManager( ) :
    m_data(new Data())
{
    connect(g_TcBtPhone, &TcBtPhone::btRequestBtPhoneAudioSource, [this]() {
         requestBtPhoneAudioSource();
    });
    connect(g_TcBtPhone, &TcBtPhone::btReleaseBtPhoneAudioSource, [this]() {
         releaseBtPhoneAudioSource();
    });
    connect(g_TcBtMusic, &TcBtMusic::btRequestBtMusicAudioSource, [this]() {
         requestBtMusicAudioSource();
    });
    connect(g_TcBtMusic, &TcBtMusic::btReleaseBtMusicAudioSource, [this]() {
         releaseBtMusicAudioSource();
    });
    connect(m_data, &Data::stateChanged, [this](Data::Priority priority, int source) {

    if(source == Source::BTMusic)
    {
           if((priority ==Data::Priority::LostTemporarily) ||(priority ==Data::Priority::LostForever)   )
           {
        g_TcBtMusic->closeBtMusic();
    }
           else if(priority == Data::Priority::Hold)
           {
               g_TcBtMusic->openBtMusic();
               m_data->setAudioVolume(Source::BTMusic,130);
           }
    }
    });
}

VolumeManager::~VolumeManager()
{
    m_data->release(m_source);
    delete m_data;
}
void VolumeManager::releaseBtPhoneAudioSource()
{
   m_data->release(Source::Phone);
}
void VolumeManager::requestBtPhoneAudioSource()
{
    m_source = Source::Phone;
   m_data->request(Source::Phone,m_data->LostTemporarily);
}
void VolumeManager::releaseBtMusicAudioSource()
{
    m_data->release(Source::BTMusic);
}
void VolumeManager::requestBtMusicAudioSource()
{
    m_source = Source::BTMusic;
   m_data->request(Source::BTMusic,m_data->LostForever);
}
