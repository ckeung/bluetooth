#include "VolumeManagerData.h"

VolumeManager::Data::Data() :
    m_interface(new smk::tcc8971::AudioMidware::AudioPolicy(
                    DBUS_AUDIO_MANAGE_MIDWARE_BUS_NAME,
                    DBUS_AUDIO_MANAGE_MIDWARE_OBJECT_PATH "AudioPolicy",
                    #ifdef MULTIMEDIA_MIDWARE_EMBEDDED_PLATFORM
                    QDBusConnection::systemBus(),
                    #else
                    QDBusConnection::systemBus(),
                    #endif
                    this
                    )),
    m_isRequest(false),
    m_priority(LostForever)
{
    connect(m_interface, SIGNAL(getAudioSourceFailed(int)), SLOT(onGetAudioSourceFailed(int)));
    connect(m_interface, SIGNAL(getAudioSourceSuccess(int)), SLOT(onGetAudioSourceSuccess(int)));
    connect(m_interface, SIGNAL(lostForeverAudioSource(int)), SLOT(onLostForeverAudioSource(int)));
    connect(m_interface, SIGNAL(lostTemporarilyAudioSource(int)), SLOT(onLostTemporarilyAudioSource(int)));
    connect(m_interface, SIGNAL(mixerAudioSource(int)), SLOT(onMixerAudioSource(int)));
    connect(m_interface, SIGNAL(audioSourcePriorityHolder(int)), SLOT(onAudioSourcePriorityHolder(int)));

    //Todo: Init?
    //release();
}

VolumeManager::Data::~Data()
{
    m_interface->releaseAudioSource(1);
}

void VolumeManager::Data::onGetAudioSourceFailed(int source) {
    m_priority = LostForever;
    m_isRequest = false;
    emit stateChanged(m_priority, m_isRequest);
}

void VolumeManager::Data::onGetAudioSourceSuccess(int source) {
    m_priority = LostForever;
    m_isRequest = true;
    emit stateChanged(m_priority, source);
}

void VolumeManager::Data::onLostForeverAudioSource(int source) {
    m_priority = LostForever;
    m_isRequest = false;
      fprintf(stderr,"come to here %s   %d   ,\n",__FUNCTION__,__LINE__);
    emit stateChanged(m_priority, source);
}

void VolumeManager::Data::onLostTemporarilyAudioSource(int source) {
    m_priority = LostTemporarily;
    m_isRequest = true;
      fprintf(stderr,"come to here %s   %d   ,\n",__FUNCTION__,__LINE__);
    emit stateChanged(m_priority, source);
}

void VolumeManager::Data::onMixerAudioSource(int source) {
    m_priority = Mixer;
    m_isRequest = true;
      fprintf(stderr,"come to here %s   %d   ,\n",__FUNCTION__,__LINE__);
    emit stateChanged(m_priority, source);
}
void VolumeManager::Data::onAudioSourcePriorityHolder(int source) {
    m_priority = Hold;
    m_isRequest = true;
      fprintf(stderr,"come to here %s   %d   ,\n",__FUNCTION__,__LINE__);
    emit stateChanged(m_priority, source);
}

void VolumeManager::Data::request(int source,const Priority &priority)
{
    m_interface->requestAudioSource(source, priority);
}
void VolumeManager::Data::requestsource(int source,const Priority &priority)
{
    m_interface->requestAudioSource(source, priority);
}
void VolumeManager::Data::release(int source)
{
    m_interface->releaseAudioSource(source);
}
void VolumeManager::Data::setAudioVolume(int source,int volume)
{
    m_interface->setAudioVolume(source,volume);
}
