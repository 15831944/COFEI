#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <QObject>

class ThreadObject : public QObject
{
    Q_OBJECT
public:
    explicit ThreadObject();
    ~ThreadObject();

signals:
    void ErrorEvent(const ushort,const QString&);
public slots:
    void AnalyDB();
    void SaveRealTimeDB();
    void SaveSetUpDB();
};

#endif // THREADOBJECT_H
