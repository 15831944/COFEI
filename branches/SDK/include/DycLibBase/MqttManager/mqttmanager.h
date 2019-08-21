#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include "mqttmanager_global.h"
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

class MQTTMANAGERSHARED_EXPORT MqttManager:public QObject
{
    Q_OBJECT
public:
    MqttManager();
    ~MqttManager();
    bool Mqtt_Client_Init(const QString ip, const QString port, const QMap<QString,QString> topical_resive_map);
    bool Subscribe();
    void  Client_push(QString topic,QByteArray msg_buff,bool result);
    QMqttClient              *m_pClient;
private:
    QMap<QString,QString> m_topical_map;
    QString                    temppath;
    void brokerDisconnected();
    void Message_resive(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void Save_reive_info(QString path,QString msg);


signals:
    void broker_disconnected();
    void mqtt_send_signal(QString topic,QByteArray msg_buff,bool result);

};

#endif // MQTTMANAGER_H
