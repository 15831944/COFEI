#ifndef AUTOFLOWFRAME_H
#define AUTOFLOWFRAME_H

#include <QObject>
#include "autoflowoperater.h"
#include <QThread>

class AutoFlowFrame :public QObject
{
      Q_OBJECT
public:
    AutoFlowFrame();
    ~AutoFlowFrame();
public:
    void Start();
    void Stop();
private:
    QMap<QString,QPair<AutoFlowOperater*,QThread*>> m_AutoObjectMap;
    bool CreateAutoFlowDispatcher(const QString strKey);
};

#endif // AUTOFLOWFRAME_H
