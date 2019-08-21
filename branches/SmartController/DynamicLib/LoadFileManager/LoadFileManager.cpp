#include "LoadFileManager.h"
#include <QDateTime>
LoadFileManager::LoadFileManager()
{
    m_errorList.clear();
}
LoadFileManager* LoadFileManager::GetInstance()
{
    static LoadFileManager pInstance;

    return &pInstance;
}
void LoadFileManager::CloseInstance()
{
    m_loadPlugin.UnloadPlugin();
    deleteLater();
}

void LoadFileManager::InitConfig(bool isAnals)
{
    QObject::connect(&m_handleXml, &HandleXml::operateXmlsError, this, &LoadFileManager::LoadFileErrored);
    QObject::connect(&m_loadPlugin, &LoadPlugin::operatePluginsError, this, &LoadFileManager::LoadFileErrored);
    m_handleXml.SetAnalsUIXml(isAnals);
    m_handleXml.AnalysXml();
    m_loadPlugin.PluginManage();
}
void LoadFileManager::LoadFileErrored(const QString str)
{
    QString dtFormat = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz");

    QString strError = QString("[%1] [Error]] {%2} %3\n").arg(dtFormat).arg(defaultLogModule).arg(str);
    m_errorList.append(strError);
}

bool LoadFileManager::GetErrorState(QList<QString> &list)
{
    if(m_errorList.size() > 0)
    {
        list.swap(m_errorList);
        return true;
    }
    return false;
}

