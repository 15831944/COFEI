#include "logmanager.h"
#include <QDateTime>
#include <QTextStream>
#include <time.h>
#include <QThread>
#include <QTime>
#include <QWaitCondition>
namespace CoreLog
{
    QWaitCondition g_waitCondition;
    void QLog_Info(const QString &module, const QString &message)
    {
        QLog_Async(module, InfoLevel, message);
    }

    void QLog_Warning(const QString &module, const QString &message)
    {
        QLog_Async(module, WarnLevel, message);
    }

    void QLog_Error(const QString &module, const QString &message)
    {
        QLog_Async(module, ErrorLevel, message);
    }

    void QLog_(const QString &module, LogLevel level, const QString &message)
    {
        QLoggerManager *manager = QLoggerManager::getInstance();
        QMutexLocker locker(&manager->mutex);
        QLogger *logWriter = manager->getLogInstance(module);

        if (logWriter && logWriter->getLevel() <= level)
                logWriter->write(module,level,message);
    }

    void QLog_Async(const QString &module, LogLevel level, const QString &message)
    {
        QMutexLocker locker(&g_Mutex);
        QLoggerManager *manager = QLoggerManager::getInstance();
        LogStru logstru;
        logstru.strModule = module;
        logstru.iLevel = level;
        logstru.strMsg = message;
        manager->m_queue.enqueue(logstru);
        g_waitCondition.wakeOne();
    }

    //QLoggerManager
    QLoggerManager * QLoggerManager::INSTANCE = NULL;

    QLoggerManager::QLoggerManager() : QThread(), mutex()
    {
        m_stopFlag = false;
        m_queue.clear();
        start();
    }

    QLoggerManager * QLoggerManager::getInstance()
    {
        if (!INSTANCE)
            INSTANCE = new QLoggerManager();

        return INSTANCE;
    }

    QString QLoggerManager::levelToText(const LogLevel &level)
    {
        switch (level)
        {
            case InfoLevel: return "Info";
            case WarnLevel: return "Warning";
            case ErrorLevel: return "Error";
            default: return QString();
        }
    }

    bool QLoggerManager::addLogInstance(const QString &fileDest, const QString &module, LogLevel level)
    {
        QLogger *log;

        if (!moduleDest.contains(module))
        {
            log = new QLogger(fileDest,level);
            moduleDest.insert(module, log);
            return true;
        }

        return false;
    }

    bool QLoggerManager::addLogInstance(const QString &fileDest, const QStringList &modules, LogLevel level)
    {
        QLogger *log;
        foreach (QString module, modules)
        {
            if (!moduleDest.contains(module))
            {
                log = new QLogger(fileDest,level);
                moduleDest.insert(module, log);
                return true;
            }
        }
        return false;
    }

    bool QLoggerManager::addLogInstance(const QString &module,LogLevel level)
    {
        QLogger *log;
        if(!moduleDest.contains(module))
        {
            QDir dir(QDir::currentPath());
            if (!dir.exists("logs"))
                dir.mkdir("logs");

            QString fileDestination = QDir::currentPath() + "/logs/"+ module +".log";
            log = new QLogger(fileDestination,level);
            moduleDest.insert(module,log);
            return true;
        }
        return false;
    }

    void QLoggerManager::closeLogger()
    {
        m_stopFlag = true;
        g_waitCondition.wakeAll();
        for(auto it = moduleDest.begin();it!=moduleDest.end();it++)
        {
            delete it.value();
            it.value() = NULL;
        }
        deleteLater();
    }
    void QLoggerManager::run()
    {
        while(true)
        {
            QMutexLocker locker(&g_Mutex);
            if(m_queue.isEmpty())
            {
                g_waitCondition.wait(&g_Mutex);
            }
            if(m_stopFlag)
                break;
            LogStru logstru = m_queue.dequeue();
            locker.unlock();
            QLogger *logWriter = getLogInstance(logstru.strModule);
            if(logWriter &&logWriter->getLevel() <= logstru.iLevel)
                logWriter->write(logstru.strModule,logstru.iLevel,logstru.strMsg);
        }
    }

    QLogger::QLogger(const QString &fileDestination, LogLevel level,QObject *parent)
        :QObject(parent)
    {
        m_fileDestination = fileDestination;
        isFirst = true;
        m_level = level;
    }

    QLogger::QLogger(LogLevel level,QObject *parent)
        :QObject(parent)
    {
         m_level = level;
         isFirst = true;
        QDir dir(QDir::currentPath());
        if (!dir.exists("logs"))
            dir.mkdir("logs");

        m_fileDestination = QDir::currentPath() + "/logs/system.log";

    }
    QLogger::~QLogger()
    {
        if(file.isOpen())
            file.close();
    }

    bool QLogger::write(const QString &module,  const QString &message)
    {
        QString _fileName = m_fileDestination;

        int MAX_SIZE = 1024 * 1024;

        QFile file(_fileName);
        QString toRemove = _fileName.section('.',-1);
        QString fileNameAux = _fileName.left(_fileName.size() - toRemove.size()-1);
        bool renamed = false;
        QString newName = fileNameAux + "_%1__%2.log";

        //Renomenem l'arxiu si està ple
        if (file.size() >= MAX_SIZE)
        {
            //Creem un fixer nou
            QDateTime currentTime = QDateTime::currentDateTime();
            newName = newName.arg(currentTime.date().toString("yy_MM_dd")).arg(currentTime.time().toString("hh_mm_ss"));
            renamed = file.rename(_fileName, newName);

        }

        file.setFileName(_fileName);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            QTextStream out(&file);
            QString dtFormat = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

            if (renamed)
                out << QString("%1 - Previuous log %2\n").arg(dtFormat).arg(newName);

            QString logLevel = QLoggerManager::levelToText(m_level);
            QString text = QString("[%1] [%2] {%3} %4\n").arg(dtFormat).arg(logLevel).arg(module).arg(message);
            out << text;
            file.close();
            return true;
        }
        else
        {
            //Log faile
            return false;
        }
    }

    bool QLogger::write(const QString &i_module, const LogLevel i_level, const QString &i_message)
    {
        static QMutex logMutex;
        QMutexLocker locker(&logMutex);
        QString _fileName = m_fileDestination;
        int MAX_SIZE = 1024 * 1024 *2;
        if(isFirst)
        {
            isFirst = false;
            file.setFileName(_fileName);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
            {
                return false;
            }
        }
        QTextStream out(&file);
        QString dtFormat = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        QString logLevel = QLoggerManager::levelToText(i_level);
        QString text = QString("[%1] [%2] {%3} %4\n").arg(dtFormat).arg(logLevel).arg(i_module).arg(i_message);
        out << text;
        if("Error" == logLevel)
        {
            QString fileError = m_fileDestination;
            fileError = fileError.replace(".log","Error.log");
            QFile _file(fileError);
             if (_file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
             {
                if(_file.size() > MAX_SIZE)
                {
                    _file.resize(0);
                }
                 QTextStream _out(&_file);
                 _out <<text;
             }
             _file.close();
        }
        emit LogUpdateUI(text);

        //Renomenem l'arxiu si està ple
        if (file.size() >= MAX_SIZE)
        {
            file.close();
            QString toRemove = _fileName.section('.',-1);
            QString fileNameAux = _fileName.left(_fileName.size() - toRemove.size()-1);
            //bool renamed = false;
            QString newName = fileNameAux + "_%1__%2.log";
            //Creem un fixer nou
            QDateTime currentTime = QDateTime::currentDateTime();
            newName = newName.arg(currentTime.date().toString("dd_MM_yy")).arg(currentTime.time().toString("hh_mm_ss"));
            /*renamed = */file.rename(_fileName, newName);
            isFirst = true;
        }
        return true;

    }

    bool QLogger::read(QString &o_message)
    {
        o_message="";
        QString _fileName = m_fileDestination;
        QFile file(_fileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
           while(!in.atEnd()){
               o_message += in.readLine() + "\n";
           }

            file.close();
            return true;
        }
        else//read fail
        {
            return false;
        }
    }

    bool QLogger::read(QStringList &o_message)
    {
        o_message.clear();
        QString _fileName = m_fileDestination;
        QFile file(_fileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
           while(!in.atEnd()){
               o_message.append(in.readLine() + "\n");
           }

            file.close();
            return true;
        }
        else//read fail
        {
            return false;
        }
    }
}
