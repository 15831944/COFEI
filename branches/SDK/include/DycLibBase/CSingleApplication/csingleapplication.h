#ifndef CSINGLEAPPLICATION_H
#define CSINGLEAPPLICATION_H
#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>
#include "csingleapplication_global.h"

class CSINGLEAPPLICATIONSHARED_EXPORT CSingleApplication : public QApplication
{
    Q_OBJECT
    public:
        CSingleApplication(int &argc, char *argv[], const QString uniqueKey);
        bool isRunning();
        bool sendMessage(const QString &message);
    public slots:
        void receiveMessage();
    signals:
        void messageAvailable(QString message);
    private:
        bool _isRunning;
        QString _uniqueKey;
        QSharedMemory sharedMemory;
        QLocalServer *localServer;
        static const int timeout = 1000;
};

#endif // CSINGLEAPPLICATION_H
