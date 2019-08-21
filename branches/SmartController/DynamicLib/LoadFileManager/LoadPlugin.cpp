#include "LoadPlugin.h"
#include <QDebug>
#include <DycLibBase/LogManager/logmanager.h>
#include <MyEC_Shared.h>
#include <MyConfig_Shared>
#include <MyConfig_Shared>
#include <MyComm_Shared.h>
#include <DycLibBase/MqttManager/mqttmanager.h>
LoadPlugin::LoadPlugin(QObject *parent) : QObject(parent)
{

}
void LoadPlugin::UnloadPlugin()
{

}
void LoadPlugin::PluginManage()
{
    QDir plugindir = QDir(QCoreApplication::applicationDirPath()+"/plugins");
    foreach(QString filename,plugindir.entryList(QDir::Files))
    {
        QPluginLoader loader(plugindir.absoluteFilePath(filename));
        if (FunModuleInterface * base_moudle = qobject_cast<FunModuleInterface *>(loader.instance()))
        {
            qDebug() << base_moudle->GetModuleName();
            ConfigData::GetInstance()->pluginMap.insert(base_moudle->GetModuleName(),base_moudle);
        }
    }
   if(ConfigData::GetInstance()->pluginMap.contains("串口通信"))
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlComportMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlComportMap.end();++it)
        {
            Communication *com = new Communication();
            if(!com->OpenPort(it.value().portName,it.value().baudRate.toInt(),8,0,1))
            {
                CoreLog::QLog_Error(defaultLogModule,QString("串口%1打开失败，请确认之后，再重新启动程序！").arg(it.value().portName));
                emit operatePluginsError(QString("串口%1打开失败，请确认之后，再重新启动程序！").arg(it.value().portName));
                return;
            }
            com->m_CheckStr = it.value().headendData;
            MyComm_Shared::GetInstance()->m_CommMap.insert(it.key(),com);
        }
    }
    if(ConfigData::GetInstance()->pluginMap.contains("网络通信"))
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.end();++it)
        {
            Communication *com = new Communication();
            if(it.value().serType == "服务器")
            {
                com->Server(8888,MyComm_Shared::GetInstance()->Tcp_peer_name);
            }
            else if(it.value().serType == "客户端")
            {
                if(!com->Client(it.value().ipAdd,it.value().portName.toInt()))
                {
                    CoreLog::QLog_Error(defaultLogModule,QString("客户端%1打开失败，请确认之后，再重新启动程序！").arg(it.key()));
                    emit operatePluginsError(QString("客户端%1打开失败，请确认之后，再重新启动程序！").arg(it.key()));
//                    return;
                }
            }
            MyComm_Shared::GetInstance()->m_TcpMap.insert(it.key(),com);
        }
    }
    for(auto itm = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigxmlMqttMap.begin();
        itm != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigxmlMqttMap.end();itm++)
    {
        qDebug()<<"88888888";
        MqttManager *mqtt_client = new MqttManager();
        if(!mqtt_client->Mqtt_Client_Init(itm.value().ip,itm.value().port,itm.value().topicalMap))
        {
            qDebug()<<"mqtt_Client init fail";
        }
        MyComm_Shared::GetInstance()->m_MqttMap.insert(itm.key(),mqtt_client);
    }
}
