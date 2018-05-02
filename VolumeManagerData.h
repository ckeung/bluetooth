#ifndef VOLUMEMANAGERDATA_H
#define VOLUMEMANAGERDATA_H

#include <QObject>
#include "VolumeManager.h"
#include "audiopolicy_interface.h"


class VolumeManager::Data : public QObject
{
    Q_OBJECT
public:
    typedef VolumeManager Manager;
    typedef VolumeManager::Service Service;

    Data();
    ~Data() override;
    //Todo:copy from audio manager, and add something
    enum Source : int { Radio = 0, Music, Video, BTMusic, Phone };
    enum Priority : int { LostTemporarily = 0, Mixer, LostForever,Hold };

    bool isRequest() const;
    Source source() const;
    Priority priority() const;

signals:
    void stateChanged(Priority priority, int source);

public :
    void request(int source,const Priority &priority = LostForever);
    void requestsource(int source,const Priority &priority = LostForever);
    void release(int source);
    void setAudioVolume(int source,int volume);
    Source  m_source;
private slots:
    void onGetAudioSourceFailed(int source);
    void onGetAudioSourceSuccess(int source);
    void onLostForeverAudioSource(int source);
    void onLostTemporarilyAudioSource(int source);
    void onMixerAudioSource(int source);
    void onAudioSourcePriorityHolder(int source);
private:
    smk::tcc8971::AudioMidware::AudioPolicy* m_interface;

    bool m_isRequest;
    Priority m_priority;
};

#endif // VOLUMEMANAGERDATA_H
