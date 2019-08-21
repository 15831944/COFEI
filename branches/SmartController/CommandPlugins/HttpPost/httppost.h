
#ifndef HTTPPOST_H
#define HTTPPOST_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QUrl>
#include <QApplication>
class HttpPost:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    HttpPost();
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
private:
     void replyFinished(QNetworkReply *reply);
     QString ReadTxt(QString filepath);
private:
    QString m_moduleName;
    SWCONFIGXML_UPLOAD_STRU _ps;
    QJsonObject json; //ＪＳＯＮ数据结构
    QJsonDocument docment;
    QString path;    // 上传路径
    QString ResiveStr;//接受到服务器发送过来的信号
    bool m_resiveflag;
private:
    QNetworkReply *_reply;
    QNetworkAccessManager *_uploadManager;
};

#endif // HTTPPOST_H
