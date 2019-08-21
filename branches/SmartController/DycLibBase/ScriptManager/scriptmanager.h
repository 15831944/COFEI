#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "scriptmanager_global.h"
#include <QScriptEngine>
#include <QList>
#include <QMutex>
class SCRIPTMANAGERSHARED_EXPORT ScriptManager : public QScriptEngine
{
private:
    ScriptManager();
public:
    static ScriptManager * GetInstance();
    QString RunAlgorithm(const QString &);
private:
    bool m_loadOk;
    bool LoadScriptFile(const QString &);
private:
    QMutex m_mutex;
};

#endif // SCRIPTMANAGER_H
