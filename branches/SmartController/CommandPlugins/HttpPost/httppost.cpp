#include "httppost.h"
#include <sys/time.h>
#include <QStandardPaths>
#include<MyConfig_Shared>
#include <QDir>
#include <QThread>
#include <QElapsedTimer>
HttpPost::HttpPost()
{
    m_moduleName = "POST";
    _uploadManager = NULL;
}
void HttpPost::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/" + "/Data/";
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkdir(path);
    }

}
void HttpPost::SetModuleParam(SWCONFIGXML_STRU ps)
{
//    json.insert("Line","222222");
//    json.insert("Station","788");
//    json.insert("SN","value2");
    //todo Save struct
    _ps = ps.swXml_Upload_Stru;
    if(_uploadManager == NULL)
    {
        _uploadManager = new QNetworkAccessManager(this);
         connect(_uploadManager,&QNetworkAccessManager::finished,this,&HttpPost::replyFinished);
    }
    for(int i= 0;i <_ps.dataSetKeyVec.size();i++)//遍历datasetkeyvec里面所有的值，找到多对应的地址
    {
        QString pathkey = _ps.dataSetKeyVec.at(i);  //获取datasetkeyvec(i)
        //遍历硬件配置里面的　编号和对应的地址
        for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.begin();
            it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.end();it ++)
        {
            if(it->key == pathkey)//如果datasetkeyvec里面的值和　硬件配置里面的编号对应　就取地址然后读
            {
                QString lastpath = "";
                lastpath = QString("%1%2").arg(path).arg(it->address);
                json.insert(it->key,ReadTxt(lastpath));// 将读取的值插入到JSON对象里面
            }
        }
    }
    docment.setObject(json);
    QByteArray buf = docment.toJson(QJsonDocument::Compact);
    QUrl url = QString(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlWebMap.hwconfigWebMap[_ps.Id].URL);//http://192.168.193.16:8080/MesAPI/MesAPI"
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    //request.setRawHeader("filename", uploadfilename.section('/', -1, -1).toUtf8() );
    _reply = _uploadManager->post(request,buf);
    QThread::msleep(200);
    m_resiveflag = false;
    // connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));
    // connect(_reply,&QNetworkReply::error,this,&HttpPOST1::upLoadError);
    // connect(_reply,&QNetworkReply::uploadProgress,this,&HttpPOST1::OnUploadProgress);
}
void HttpPost::RunModule()
{
    //todo call command for IO

}

int  HttpPost::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    QElapsedTimer SetTotaltimeoutTimer;//超时计时器
    SetTotaltimeoutTimer.start();//总超时计时器启动
    int TotaltimeoutRemainingTime = 0;//剩余时间
    int SetTotaltimeoutms = 50000;
    while(TotaltimeoutRemainingTime >= 0)
    {
        if(m_resiveflag)
        {
            if(ResiveStr.contains(_ps.dataRecovery))
            {
                iresult = 1;
                strMsg = "Upload successfully!";
            }
            else
            {
                strMsg = QString("Upload failure!--%1").arg(ResiveStr);
            }
            return iresult;
        }
        QThread::msleep(5);//while循环命令提供缓存时间防止界面死掉
        QApplication::processEvents();//while循环命令提供缓存时间防止界面死掉
        TotaltimeoutRemainingTime = SetTotaltimeoutms - SetTotaltimeoutTimer.elapsed();//计算总的流失时间
    }
    return iresult;
}
void HttpPost::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *HttpPost::Copy()
{
    return (new HttpPost());
}

void HttpPost::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
       // QString result(bytes);  //转化为字符串
        ResiveStr = bytes;


    }
    else{
       ResiveStr = reply->errorString();
    }
    m_resiveflag = true;
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

}

QString HttpPost::ReadTxt(QString path)
{
    QString str ="";
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream textstream(&file);
            textstream.setCodec("utf-8");
//            do
//            {
//                str = textstream.readLine(500);
//            }
//            while(!textstream.atEnd());
            str = textstream.readAll();
        }
        else
        {
            str = "errcode: 0";// 打开失败
        }
    }
    return str;

}
