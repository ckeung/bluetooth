#include "BlueToothMidwareDBusRegistant.h"

BlueToothMidwareDBusRegistant *BlueToothMidwareDBusRegistant::m_instance = nullptr;
BlueToothMidwareDBusRegistant* BlueToothMidwareDBusRegistant::getInstance()
{
      fprintf(stderr, "bluetooth getInstance\n");
    if (!m_instance)
    {
        m_instance = new BlueToothMidwareDBusRegistant();
        if (!m_instance->connectDBus())
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    return m_instance;
}
#include "TcBtMusicConvertor.h"
#include "TcBtPhoneConvertor.h"
#include <QtDBus>
BlueToothMidwareDBusRegistant::BlueToothMidwareDBusRegistant() :
    QObject(),
    m_btmusic(TcBtMusicConvertor::getIntance()),
    m_btphone(TcBtPhoneConvertor::getInstance())
{
}

BlueToothMidwareDBusRegistant::~BlueToothMidwareDBusRegistant()
{
    m_instance = nullptr;
}
bool BlueToothMidwareDBusRegistant::connectDBus()
{
    auto dbus = getDBusConnection();
    auto registerObject = [&dbus](QObject* convertor) -> bool {
     return dbus.registerObject(DBUS_BLUETOOTH_MIDWARE_OBJECT_PATH + convertor->property("interfaceClassName").toString(), convertor);
    };
    if(!dbus.registerService(DBUS_BLUETOOTH_MIDWARE_BUS_NAME))
    {
        qDebug() << dbus.lastError();
    }
     else if(!registerObject(m_btmusic))
    {
        qDebug() << dbus.lastError();
    }
    else if(!registerObject(m_btphone))
   {
        qDebug() << dbus.lastError();
   }
    else
    {
        return true;
    }
    return false;
}
inline QDBusConnection BlueToothMidwareDBusRegistant::getDBusConnection() const
{
    return QDBusConnection::systemBus();
}
