#include "mqtt_client.h"
#include <sys/time.h>
#include <MyComm_Shared.h>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>
Mqtt_Client::Mqtt_Client()
{
    m_moduleName = "MQTT_W";
}
void Mqtt_Client::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    temppath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/";
    QDir dir;
    if(!dir.exists(temppath))
    {
        dir.mkdir(temppath);
    }

}
void Mqtt_Client::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_MQTT_data;

}
void Mqtt_Client::RunModule()
{
    //todo call command for IO

}

int  Mqtt_Client::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(MyComm_Shared::GetInstance()->m_MqttMap.contains(_ps.clinet_id))
    {
        QByteArray buff = get_send_message(_ps.data_save).toUtf8();
        bool result = false;
        if(MyComm_Shared::GetInstance()->m_MqttMap[_ps.clinet_id]->m_pClient->state() == QMqttClient::Disconnected)
        {
            strMsg = "Mqtt_Client Disconnect from the server";
            return iresult;
        }
        emit MyComm_Shared::GetInstance()->m_MqttMap[_ps.clinet_id]->mqtt_send_signal(_ps.topical,buff,result);
        strMsg += "主题消息发送成功";
        iresult = 1;
    }
    return iresult;
}
void Mqtt_Client::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *Mqtt_Client::Copy()
{
    Mqtt_Client *it = new Mqtt_Client();
    return it;
}

QString Mqtt_Client::get_send_message(QString path)
{
    QFile file(temppath + path);
    if(file.open(QIODevice::ReadOnly))
    {
         QString str = file.readAll();
         file.close();
         return str;
    }
    else
    {
        file.close();
        return "";
    }


}
