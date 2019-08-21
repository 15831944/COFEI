#ifndef SERVETCP_H
#define SERVETCP_H
#include <QTcpServer>
#include <QTcpSocket>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
#include <QDir>
#include <QStandardPaths>
#include <DycLibBase/Communication/communication.h>
class  ServeTcp:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)

public:
    ServeTcp();
    ~ServeTcp();
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
    void SaveTxt(QString str);
    QString ReadTxt();
    bool isok;
    void SaveDataForDemper(QString data);
private:
    QString m_moduleName;
    SWCONFIGXML_COMM_DATA_STRU _ps;
    bool connectisok;
    int result_state;
    QString TCP_read_str;
    QString path;
    QString temppath;
    QString m_pathname;
    bool m_isread;

private slots:
    void TCP_read(QByteArray buf);
};

#endif // SERVETCP_H
