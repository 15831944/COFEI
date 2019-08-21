
#ifndef WEBSOCKET_H
#define WEBSOCKET_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QtWebSockets/QWebSocket>
#include <sys/time.h>
#include <QTime>
#include <QElapsedTimer>
#include <QThread>
#include <QStandardPaths>
class WebSocket:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    WebSocket();
    ~WebSocket();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    QString ReadTxt(QString path); //读取Txt的内容

private:
    void onConnected();
    void Close();
    void onTextMessageReceived(QString message);
    bool SendTextMessage(QString message,QString receive);
    void Write(QString str);//将服务器发送过来的内容写入到txt
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }

private:
    QString m_moduleName;
    SWCONFIGXML_UPLOAD_STRU _ps;
    QJsonObject json; //ＪＳＯＮ数据结构
    QJsonDocument docment;
    QString path;    // 上传路径
   // QMap<QString,WebSocekt_Client*> m_WebMap;//编号，websocket对象
    QWebSocket m_webSocket;
    bool isalive;
    QString ResiveStr;//接受到服务器发送过来的信号
};

#endif // WEBSOCKET_H
