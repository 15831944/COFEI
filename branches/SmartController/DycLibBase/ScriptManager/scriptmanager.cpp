#include "scriptmanager.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include "converter.h"
#include <QStandardPaths>
ScriptManager::ScriptManager()
{
    QString path;
    path=QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹"+"/脚本/algorithm.js";
    m_loadOk = LoadScriptFile(path);
    Converter *ct = new Converter();
    QScriptValue cofei = this->newQObject(ct);
    this->globalObject().setProperty("COFEI",cofei);
}
ScriptManager * ScriptManager::GetInstance()
{
    static ScriptManager pInstance;

    return &pInstance;
}

bool ScriptManager::LoadScriptFile(const QString &fileName)
{
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    this->evaluate(contents);
    return true;
}

QString ScriptManager::RunAlgorithm(const QString &str)
{   
    QMutexLocker locker(&m_mutex);
    if(!m_loadOk)
    {
        return QString("Error-> LoadFile false");
    }
    QScriptValue result = this->evaluate(str);
    if (this->hasUncaughtException())
    {
        return QString("Error-> ").arg(uncaughtException().toString());
    }
    else
    {
        return result.toString();
    }


}

