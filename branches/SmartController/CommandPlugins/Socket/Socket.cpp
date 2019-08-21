#include "Socket.h"
#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <MyComm_Shared.h>
#include <QDebug>
#include <QDateTime>
using namespace CoreGlobal;
ServeTcp::ServeTcp()
{
    m_moduleName = "网络通信";

}
ServeTcp::~ServeTcp()
{

}

void ServeTcp::Initialization()
{
    isTerminate = false;
    m_isread = false;
    connectisok = true;
    temppath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/";
    QDir dir;
    if(!dir.exists(temppath))
    {
        dir.mkdir(temppath);
    }
}

void ServeTcp::SetModuleParam(SWCONFIGXML_STRU ps)
{
    _ps = ps.swXml_COMM_data;
    path = "";
    m_pathname="";
//    for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.begin();
//        it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.end();it ++)
//    {
//        if( _ps.savePathKey == it->key)
//        {
//            path = QString("%1%2").arg(temppath).arg(it->address);
//            m_pathname = it->address;
//        }
//    }
      path = QString("%1%2").arg(temppath).arg(_ps.savePathKey);
      m_pathname = _ps.savePathKey;
//    if(MyComm_Shared::GetInstance()->m_TcpMap.contains(_ps.commId))
//    {
//        connect(MyComm_Shared::GetInstance()->m_TcpMap[_ps.commId],&Communication::TCPResive,this,&ServeTcp::TCP_read);
//    }
//    else
//    {
//        connectisok = false;
//    }
}

void ServeTcp::RunModule()
{

}
///
/// \brief ServeTcp::GetExcResult
/// \param strMsg
/// \return 1=读取对比成功，0= 读取对比失败， -1= 写入失败(!=-1 写入成功)，
///
int ServeTcp::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    //    if(!connectisok)   //没有找到key为【_ps.commid】的communication对象
    //    {
    //        iresult = -6;
    //        return iresult;
    //    }
    Communication *com = MyComm_Shared::GetInstance()->m_TcpMap[_ps.commId];//chang zhou2018.01.11
    if(com != NULL)
    {
        struct timeval tpStart,tpEnd;
        float timeUse = 0;
        gettimeofday(&tpStart,NULL);
        while(true)
        {
            if(isTerminate)
            {
                iresult = -99;
                strMsg = QString("Socket %1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
                return iresult;
            }
            if(_ps.opType == "TCP_W")
            {
                if(path == "")
                {
                    iresult = -3;
                     strMsg = "读取路径不存在!";
                     return iresult;
                }
                QByteArray bytearray = ReadTxt().toLatin1();// bytearray is null?
                char *p = bytearray.data();
                if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap[_ps.commId].serType == "服务器")
                {
                    if(MyComm_Shared::GetInstance()->Tcp_peer_name != 0)
                    {
                        iresult = com->SendBuff_Server(p,_ps.Ip);
                        if(iresult != -1)
                        {
                            strMsg = "Socket write succeed!";
                            iresult = 1;
                            break;
                        }
                        else
                        {
                            strMsg = "Socket write fail !";
                            return iresult;
                        }
                    }
                }
                else
                {
                   // iresult = com->SendBuff(p);
                    emit com->SendData(bytearray);
                    iresult = 1;
                   // qDebug() << "return value: " << iresult << endl;
                    if(iresult != -1)
                    {
                        strMsg = "Socket write succeed!";
                        iresult = 1;
                        break;
                    }
                    else
                    {
                        strMsg = "Socket write Fail!";
                        iresult = -2;
                        return iresult;
                    }
                }
            }
            else if(_ps.opType == "TCP_R")
            {
                if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap[_ps.commId].serType == "服务器")
                {
                    m_isread = true;
                    TCP_read_str = com->ReadMassage[_ps.Ip];
                    if(_ps.data == "" &&TCP_read_str != "")
                    {
                        strMsg = "Socket read succeed!";
                        SaveTxt(TCP_read_str);
                        SaveDataForDemper(TCP_read_str);
                        com->ReadMassage[_ps.Ip] = "";
                        iresult = 1;
                        break;
                    }
                    else if(TCP_read_str.contains( _ps.data))
                    {
                        strMsg = "Socket read succeed!";
                        SaveTxt(TCP_read_str);
                        SaveDataForDemper(TCP_read_str);
                        iresult = 1;
                        com->ReadMassage[_ps.Ip] = "";
                        break;
                    }
                }
                else
                {
                    m_isread = true;
                    if(com->socket->state() == QAbstractSocket::UnconnectedState)
                    {
                        iresult = -4;
                        strMsg = "Client is disconnect!";
                        break;
                    }
                    TCP_read_str = com->Client_Read;
                    if(_ps.data == "" &&TCP_read_str != "")
                    {
                        strMsg = "Socket read succeed!";
                        SaveTxt(TCP_read_str);
                         SaveDataForDemper(TCP_read_str);
                        com->Client_Read = "";
                        iresult = 1;
                        break;
                    }
                    else if(TCP_read_str.contains( _ps.data) &&TCP_read_str != "")//"" is contains ""
                    {
                        strMsg = "Socket read succeed!";
                        SaveTxt(TCP_read_str);
                        SaveDataForDemper(TCP_read_str);
                        iresult = 1;
                        com->Client_Read = "";
                        break;
                    }
                }
            }
            gettimeofday(&tpEnd,NULL);
            timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            if(timeUse >= 20000)
                break;
            QThread::msleep(5);
            QApplication::processEvents();
        }
        if(iresult != 1 && iresult != -4)
        {
            iresult = -3;
            strMsg = QString("Socket %1 %2 TimeOut!").arg(m_moduleName).arg(_ps.id);
        }
    }
    else
    {
        strMsg = "Socket Open Fail!";
    }
    return iresult;
}

void ServeTcp::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *ServeTcp::Copy()
{
    ServeTcp *s = new ServeTcp();
    return s;
}

void ServeTcp::TCP_read( QByteArray buf) //Increase judgment not anytime do it!
{
    if(m_isread)  //make sure it's TCP_R
    {
        TCP_read_str = "";
        QTextCodec *ptc = QTextCodec::codecForName("GBK");
        TCP_read_str = ptc->toUnicode(buf);
        SaveTxt(TCP_read_str);//保存到TXT
        m_isread = false;
    }

}
void ServeTcp::SaveTxt(QString str)
{
    QFile file(path);
    bool isok = file.open(QIODevice::WriteOnly);
    if(true == isok)
    {
        QByteArray buf = str.toLatin1();
        file.write(buf);
    }
    file.close();
}

QString ServeTcp::ReadTxt()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    bool isok = false;
    QString str = "";
    QFile file(path);
    isok = file.open(QIODevice::ReadOnly);
    if(true == isok)
    {
        QTextStream filestream(&file);
        str = filestream.readAll();
        QByteArray buff = str.toLatin1();
        int inum = buff.size();
        if(10 == buff[inum-1])
            buff[inum-1] = 13;
        //buff[inum] = 10;
        str = codec->toUnicode(buff);
    }
    return str;

}

void ServeTcp::SaveDataForDemper(QString data)
{
    QDateTime date = QDateTime::currentDateTime();
    QString datastr = date.toString("yyyy-MM-dd");
    QString datatempath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/"+ datastr+ "/";
    QDir dr;
    if(!dr.exists(datatempath))
    {
        dr.mkdir(datatempath);
    }
    QString actpath = datatempath + m_pathname;
    QFile file(actpath);
    bool isok = file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray buff = data.toLatin1();
        buff += '\n';
        file.write(buff);
    }
    file.close();
}
