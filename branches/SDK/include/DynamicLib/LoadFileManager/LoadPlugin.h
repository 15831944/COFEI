#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QObject>
#include <funmoduleinterface.h>
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QMap>
#include <vector>
#include <dlfcn.h>
class LoadPlugin : public QObject
{
    Q_OBJECT
public:
    explicit LoadPlugin(QObject *parent = 0);
public:
    void UnloadPlugin();
signals:
    void operatePluginsError(const QString);
public slots:
    void PluginManage();
};

#endif // WORKERTHREAD_H
