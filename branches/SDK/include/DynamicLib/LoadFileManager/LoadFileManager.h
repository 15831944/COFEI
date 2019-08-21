#ifndef LOADFILEMANAGER_H
#define LOADFILEMANAGER_H
#include <QObject>
#include <QList>
#include "loadfilemanager_global.h"
#include "HandleXml.h"
#include "LoadPlugin.h"
class LOADFILEMANAGERSHARED_EXPORT LoadFileManager :public QObject
{
    Q_OBJECT
private:
    LoadFileManager();
public:
    static LoadFileManager *GetInstance();
    void CloseInstance();
    void InitConfig(bool isAnals);
public slots:
    void LoadFileErrored(const QString);
    bool GetErrorState(QList<QString> &list);
private:
    HandleXml m_handleXml;
    LoadPlugin m_loadPlugin;
    QList<QString> m_errorList;
};

#endif // LOADFILEMANAGER_H
