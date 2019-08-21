#ifndef SREVER_H
#define SREVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <DycLibBase/Communication/communication.h>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
class Srever:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)

public:
    Srever();
    ~Srever();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface* Copy();
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }
    virtual bool ReadEvent(uchar*)
    {
        return true;
    }
private:
    bool CheckIOBit();
    void SaveTxt(const QString str);
    void SaveDataForDemper(QString data);
    QString ReadTxt();
    char ConvertHexChar(char ch);
    void StringToHex(QString str, QByteArray &senddata);
private:
    QString m_moduleName;
    SWCONFIGXML_COMM_DATA_STRU _ps;
    bool isok;
    bool m_bsleteData;
    int result_state;
    //QByteArray Port_read_str;
    char * Port_read_str;
     QString path;
     QString temppath;
     QString m_pathname;
     QTextCodec *codec;
     QMutex m_mutex;
private slots:
    void Port_read(char*);
};

#endif // SREVER_H
