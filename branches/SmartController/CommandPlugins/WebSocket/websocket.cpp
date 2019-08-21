#include "websocket.h"
#include <sys/time.h>
#include<MyConfig_Shared>
WebSocket::WebSocket()
{
    m_moduleName = "上传服务器";
   // m_WebMap.clear();
    isalive = false;  //是否链接上服务器
}

WebSocket::~WebSocket()  //析构
{
//    for(auto it = m_WebMap.begin();it != m_WebMap.end();it++)
//    {
//       it.value()->Close();
//    }
}
void WebSocket::Initialization()
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
void WebSocket::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_Upload_Stru;
    //if(!m_WebMap.contains(_ps.Id))  //判断ｍａｐ里面是否包含ｉｄ所对应的 websocket_client对象
    //新建对象，并插入到map里面
    //WebSocekt_Client* pwebsocket = new WebSocekt_Client(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlWebMap.hwconfigWebMap[_ps.Id].URL);
    //m_WebMap.insert(_ps.Id,pwebsocket);
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocket::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocket::Close);
    m_webSocket.open(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlWebMap.hwconfigWebMap[_ps.Id].URL);
    for(int i= 0;i <_ps.dataSetKeyVec.size();i++)//遍历datasetkeyvec里面所有的值，找到多对应的地址
    {
        QString pathkey = _ps.dataSetKeyVec.at(i);  //获取datasetkeyvec(i)
        //遍历硬件配置里面的　编号和对应的地址
        for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.begin();
            it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.begin();it ++)
        {
            if(it->key == pathkey)//如果datasetkeyvec里面的值和　硬件配置里面的编号对应　就取地址然后读
            {
                QString lastpath = "";
                lastpath = QString("%1%2").arg(path).arg(it->address);
                json.insert("",ReadTxt(lastpath));// 将读取的值插入到JSON对象里面
            }
        }
    }
}
void WebSocket::RunModule()
{
    //todo call command for IO
}
///
/// \brief WebSocket::GetExcResult
/// \param strMsg
/// \return 0 上传失败　-1断开链接　1 上传成功
///
int  WebSocket::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    bool result = false;
    docment.setObject(json);
    QByteArray buf = docment.toJson(QJsonDocument::Compact);
    QString strjson(buf);
    if(isalive)//判断是否链接上
    {
        //result = m_WebMap[_ps.Id]->SendTextMessage(strjson,_ps.dataRecovery);//发送数据并判断返回值是否是_ps.dataRecovery
        result =  m_webSocket.sendTextMessage(strjson);
        if(result)
        {
            iresult = 1;
            strMsg = "上传服务器成功！";
        }
        else
        {
            iresult = 0;
            strMsg = "上传服务器失败！";
        }
    }
    else
    {
        strMsg = "与服务器链接失败！";
        iresult = -1;
    }
    return iresult;
}
void WebSocket::StopModule()
{
    isTerminate = false;
}
///
/// \brief WebSocket::ReadTxt
/// \param path　文件路径
/// \return 返回读取的内容
///
QString WebSocket::ReadTxt(QString path)
{
    QString str ="";
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream textstream(&file);
            textstream.setCodec("utf-8");
            do
            {
                str = textstream.readLine(500);
            }
            while(!textstream.atEnd());
        }
        else
        {
            str = "errcode: 0";// 打开失败
        }
    }
    return str;
}
///
/// \brief WebSocket::onConnected
///链接服务器时候触发
void WebSocket::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocket::onTextMessageReceived);
    isalive = true;

}
///
/// \brief WebSocket::Close
///当链接断开时
void WebSocket::Close()
{
    isalive = false;
}
///
/// \brief WebSocket::onTextMessageReceived
/// \param message
///收到服务器发送过来的信号
void WebSocket::onTextMessageReceived(QString message)
{
    ResiveStr = message;
    Write(message);
}

bool WebSocket::SendTextMessage(QString message, QString receive)
{
    ResiveStr = "";
    m_webSocket.sendTextMessage(message);
    QElapsedTimer timeoutTimer;//超时计时器
    int Setimeoutms = 3000000; //3000000ms
    int timeout = 0;//剩余时间
    timeoutTimer.start();//总超时计时器启动
    while (timeout >= 0)//总超时循环周期
    {
        if(ResiveStr == receive)
        {
            ResiveStr = "";
            timeoutTimer.invalidate();
            return true;
        }
        timeout = Setimeoutms - timeoutTimer.elapsed();
        QThread::msleep(5);
        QApplication::processEvents();
    }
    timeoutTimer.invalidate();
    return false;
}

void WebSocket::Write(QString str)
{
    QString wpath = QString ("%1.websocket.txt").arg(path);
    QFile file(wpath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QTextStream filestream(&file);
        filestream.setCodec("utf-8");
        str = str + "\n";
        filestream<<str;
    }
}
