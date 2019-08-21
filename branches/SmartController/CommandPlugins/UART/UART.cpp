#include "UART.h"
#include <QApplication>
#include <MyComm_Shared.h>
#include <QDebug>
using namespace CoreGlobal;
Srever::Srever()
{
    m_moduleName = "串口通信";
    m_bsleteData = false;
    Port_read_str = new char[128];
    codec = QTextCodec::codecForName("GBK");
}
Srever::~Srever()
{
    if(Port_read_str != NULL)
        delete Port_read_str;
    Port_read_str = NULL;
}

void Srever::Initialization()
{
    isTerminate = false;
    isok = true;
    m_bsleteData = true;
    memset(Port_read_str,0,128);
    temppath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/";
    QDir dir;
    if(!dir.exists(temppath))
    {
        dir.mkdir(temppath);
    }
}

void Srever::SetModuleParam(SWCONFIGXML_STRU ps)
{
    path = "";
    m_pathname = "";
    _ps = ps.swXml_COMM_data;
    path = QString("%1%2").arg(temppath).arg(_ps.savePathKey);
    m_pathname = _ps.savePathKey;
    if(MyComm_Shared::GetInstance()->m_CommMap.contains(_ps.commId))
    {
       // MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_CheckStr = _ps.Ip;
       // qDebug()<<"plugin check str is :"<<_ps.Ip;
        qDebug()<<"plugin show style is:"<<_ps.style;
        MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_show_style = _ps.style;

       // connect(MyComm_Shared::GetInstance()->m_CommMap[_ps.commId],&Communication::reseive,this,&Srever::Port_read);
    }
    else
    {
        isok = false;
    }
}

void Srever::RunModule()
{

}
///
/// \brief Srever::GetExcResult
/// \param strMsg
/// \return 1 = 读取and对比成功；  0 = 失败；
///
int Srever::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(!isok)
    {
        strMsg = "serial port open failed!";
        return iresult;
    }
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while(isok)
    {
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        if(_ps.opType =="RS_R")
        {
            QMutexLocker locker(&m_mutex);
          // QString strReadTemp = QString(Port_read_str);
            QString read_key = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlComportMap[_ps.commId].portName;
            QString strReadTemp = MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->Serial_Read[read_key];
            if(strReadTemp.size() != 0)
            {
                if(strReadTemp.contains(_ps.data))
                {
                    SaveTxt(strReadTemp);
                    SaveDataForDemper(strReadTemp);
                    strMsg = "serial port read succeed!";
                    iresult = 1;
                    MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_Wmutex.lock();
                    MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->Serial_Read[read_key] = "";
                     MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_Wmutex.unlock();
                    break;
                }
                MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_Wmutex.lock();
                MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->Serial_Read[read_key] = "";
                MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->m_Wmutex.unlock();

            }
        }
        else if((_ps.opType =="RS_W"))
        {
            if(_ps.style == "16")
            {
                QByteArray writebuf;
                StringToHex(ReadTxt(),writebuf);
                MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->WriteDate(writebuf);
            }
            else
            {
                QString strtxt = ReadTxt();
                strtxt.chop(1);
                if(strtxt.contains("\n"))
                {
                    strtxt.replace("\n","\r\n");                   
                }
                MyComm_Shared::GetInstance()->m_CommMap[_ps.commId]->WriteDate(strtxt.toLatin1());
            }
            strMsg = "serial port write succeed!";
            iresult = 1;
            break;
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= 36000000)
            break;
        QThread::msleep(5);
        QApplication::processEvents();
    }
    if(iresult != 1)
    {
        strMsg = QString("%1 %2 TimeOut!").arg(m_moduleName).arg(_ps.id);;
    }
      return iresult;
}

void Srever::StopModule()
{

    isTerminate = false;
    m_bsleteData = false;
}

FunModuleInterface *Srever::Copy()
{
    Srever *s = new Srever();
    return s;
}

void Srever::SaveTxt(const QString str)
{
    QFile file(path);
    bool isok = file.open(QIODevice::WriteOnly);
    if(true == isok)
    {
        QTextStream out(&file);
        out << str;
        file.close();
    }
}

void Srever::SaveDataForDemper(QString data)
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
        file.write(buff);
    }
    file.close();
}

QString Srever::ReadTxt()
{
    QString str = "";
    QFile file(path);
    bool isok =false;
    isok = file.open(QIODevice::ReadOnly);
    if(true == isok)
    {
        QTextStream filestream(&file);
        //filestream.setCodec("");
        str = filestream.readAll();
    }
    file.close();
    return str;
}

char Srever::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    // else return (-1);
    else return ch-ch;//不在0-f范围内的会发送成0
}

void Srever::StringToHex(QString str, QByteArray &senddata)
{
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; )
        {
            //char lstr,
            hstr=str[i].toLatin1();
            if(hstr == ' ')
            {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
}

void Srever::Port_read(char *c)
{
    if(m_bsleteData)
    {
        if(codec != NULL)
        {
            QMutexLocker locker(&m_mutex);
            memcpy(Port_read_str,c,strlen(c));
        }
    }
}
