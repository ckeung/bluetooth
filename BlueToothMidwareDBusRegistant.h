#ifndef BLUETOOTHMIDWAREDBUSREGISTANT_H
#define BLUETOOTHMIDWAREDBUSREGISTANT_H

#include <QObject>
class QDBusConnection;
class TcBtMusicConvertor;
class TcBtPhoneConvertor;
class BlueToothMidwareDBusRegistant : public QObject
{
    Q_OBJECT
public:
    static BlueToothMidwareDBusRegistant *getInstance();
    bool connectDBus();

signals:

public slots:
private:
    BlueToothMidwareDBusRegistant();
    ~BlueToothMidwareDBusRegistant() override;
    BlueToothMidwareDBusRegistant(const BlueToothMidwareDBusRegistant&) = delete;
    BlueToothMidwareDBusRegistant(BlueToothMidwareDBusRegistant&&) = delete;
    BlueToothMidwareDBusRegistant& operator= (const BlueToothMidwareDBusRegistant&) = delete;
    BlueToothMidwareDBusRegistant& operator= (BlueToothMidwareDBusRegistant&&) = delete;
    QDBusConnection getDBusConnection() const;
    TcBtMusicConvertor *m_btmusic;
    TcBtPhoneConvertor *m_btphone;
    static BlueToothMidwareDBusRegistant* m_instance;
};

#endif // BLUETOOTHMIDWAREDBUSREGISTANT_H
