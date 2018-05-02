#ifndef VOLUMEMANAGER_H
#define VOLUMEMANAGER_H

#include <QObject>
class QMutex;

class VolumeManager : public QObject
{
    Q_OBJECT
    typedef VolumeManager Manager;
    class Data;
    struct Service;

public:
    enum Source : int { Radio = 0, Music, Video, BTMusic, Phone };
        Q_ENUM(Source)
    enum Priority : int { LostTemporarily = 0, Mixer, LostForever };
    Q_ENUM(Priority)
    Source m_source;
    static VolumeManager* instance();
    void requestBtPhoneAudioSource();
    void releaseBtPhoneAudioSource();
    void requestBtMusicAudioSource();
    void releaseBtMusicAudioSource();
signals:


private:
    Data* m_data;

private: //Singleton
    VolumeManager( );
    ~VolumeManager() override;
    VolumeManager(const VolumeManager&) = delete;
    VolumeManager(VolumeManager&&) = delete;
    VolumeManager& operator= (const VolumeManager&) = delete;
    VolumeManager& operator= (VolumeManager&&) = delete;

    static QMutex m_mutex;
    static  VolumeManager* m_instances;
};

#endif // VOLUMEMANAGER_H
