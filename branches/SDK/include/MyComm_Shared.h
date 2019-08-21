#ifndef MY_COMMS_SHARED_H
#define MY_COMMS_SHARED_H

#include <QObject>
#include <QMap>
#include <DycLibBase/Communication/communication.h>
#include <DycLibBase/MqttManager/mqttmanager.h>
class MyComm_Shared
{
private:
    MyComm_Shared()
    {
        m_TcpMap.clear();
        m_CommMap.clear();
        m_MqttMap.clear();
         Tcp_peer_name = 0;
    }

public:
    static MyComm_Shared* GetInstance()
    {
        static MyComm_Shared pInstance;
        return &pInstance;
    }
public:
    QMap<QString,Communication*> m_CommMap;
    QMap<QString,Communication*> m_TcpMap;
    QMap<QString,MqttManager*>   m_MqttMap;
    int Tcp_peer_name;


};

#endif // MY_COMMS_SHARED_H
