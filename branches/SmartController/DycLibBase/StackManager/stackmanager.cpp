#include "stackmanager.h"
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <MyGlobal.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
using namespace CoreGlobal;
StackManager::StackManager()
{
}
//(E0.1=0)$(E0.2=1)$(E0.8=1)
bool StackManager::AnalysisEvent(const QString strEvent)
{
    if(strEvent.isEmpty())
        return true;
    QString strData = "";
    QString str = strEvent;
    m_sign.clear();
    m_data.clear();
    str = str.replace(QRegExp("\t+"),"");// \t为制表符，+表示至少匹配一个
    str = str.replace(QRegExp(" +"),"");//  用一个空格代替多个空格
    for(int i = 0; i < str.size();)
    {
        if(str[i] == '(')
        {
            m_sign.push(str[i]);
        }
        else if(str[i] == ')')
        {
            while(true)// 本循环会计算与)匹配的最近的(地方
            {
                if('(' == m_sign.top()) // 如果当前栈顶是(则弾栈退出
                {
                    m_sign.pop();
                    break;
                }
                else // 否则弾两个Data，一个符号进行运算
                {
                    // 结果入操作数栈
                    if(m_data.count() < 2)
                        return false;
                    QString value = m_data.pop();
                    QString key = m_data.pop();
                    QChar sign = m_sign.pop();
                    m_data.push(HandleData(key, value, sign));
                }
            }
        }
        else if(CompareSign(str[i]))
        {
            while(true)
            {
                if( m_sign.empty()  || '(' == m_sign.top()) // 如果栈不空则遇到(则压入取到的符号
                {
                    // 为空时也要压栈
                    m_sign.push(str[i]);
                    break;
                }
                else
                {
                    // 取两个data和栈顶符号进行运算
                    if(m_data.count() < 2)
                        break;
                    QString value = m_data.pop();
                    QString key = m_data.pop();
                    QChar sign = m_sign.pop();
                    m_data.push(HandleData(key, value, sign));
                    m_sign.push(str[i]);
                }
            }
        }
        else
        {
            do
            {
                strData += str[i];
                i += 1;
                if(i >= str.size())
                {
                    m_data.push(strData);
                    strData = "";
                    i -= 1;
                    break;
                }
                else
                {
                    if(CompareSign(str[i],2))
                    {
                        m_data.push(strData);
                        strData = "";
                        i -= 1;
                        break;
                    }
                }

            }while( i < (str.size()));

        }
        ++i;
    }
    if(m_data.count() < 2)
    {
        if(m_data.count() > 0)
        {
            if(m_data[0]=="true")
                return true;
            else
                return false;
        }
        else
            return false;
    }
    QString value = m_data.pop();
    QString key = m_data.pop();
    QChar sign = m_sign.pop();
    if(HandleData(key, value, sign) != "true")
        return false;

    return true;
}

QString StackManager::HandleData(const QString key, const QString value, const QChar sign)
{
    //qDebug() << "StackManager::HandleData " << key << " " << value << " " << sign.unicode();
    if(sign == '$')
    {
        if(key == "true" && value == "true")
            return "true";
    }
    else if(sign == '|')
    {
        if(key == "true" || value == "true")
            return "true";
    }
    else
    {
        QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(key))
        {
            if(sign == '=')
            {
                int ivalue = value.toInt();
                if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[key].ishwIO)
                {
                    QString IOaddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[key].IOaddr;
                    if(IOaddr.contains("E"))
                    {
                        uchar uc = 0;
                        if(1 == BaseCalcFun::AnalysisIOString(IOaddr,uc))
                        {
                            if(uc == ivalue)
                                return "true";
                        }
                    }else if(IOaddr.contains("Q"))
                    {
                        uchar uc = 0;
                        if(1 == BaseCalcFun::GetOutIOQBitValue(IOaddr,uc))
                        {
                            if(uc == ivalue)
                                return "true";
                        }
                    }
                }
                else
                {
                    if(ivalue == DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[key].initvalue)
                        return "true";
                }
            }
        }
        else if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(key))
        {
            double dest = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[key].Initvalue;
            if(CompareData(dest,value.toDouble(),sign))
            {
                return "true";
            }
        }
        else if(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(key))
        {
            QString dest = DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[key].textval;
            if(CompareData(dest,value,sign))
            {
                return "true";
            }
        }
        else if(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(value)) //字符串变量名字放在后面数据对比
        {
            QString dest = DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[value].textval;
            if(CompareData(key,dest,sign))
            {
                return "true";
            }
        }
        else if(DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(key))
        {
            int  dest = DBInstanceManager::getDBInstance()->DBVarStru.countMap[key].curvalue;
            if(CompareData(dest,value.toInt(),sign))
            {
                return "true";
            }
        }
        else
        {
            if(SlectSaveAddress(key,value,sign))
                return "true";
        }
    }
    return "false";
}

bool StackManager::CompareData(double dest, double source, const QChar sign)
{
    bool isok = true;
    int isign = sign.unicode();
    switch(isign)
    {
    case 60: // <
        if(dest - source >= 0)
            isok = false;
        break;
    case 61: // =
        if(dest - source != 0)
            isok = false;
        break;
    case 62: // >
        if(dest - source <= 0)
            isok = false;
        break;
    case 8804://≤ ox2264
        if(dest - source > 0)
            isok = false;
        break;
    case 8805://≥ ox2265
        if(dest - source < 0)
            isok = false;
        break;
    default:
        isok = false;
        break;
    }
    return isok;
}

bool StackManager::CompareSign(const QChar strSign, int type)
{
    bool isok = false;
    if(strSign == '=' || strSign == '>' || strSign == '$' ||
            strSign == '|' || strSign == 8804 || strSign == 8805 ||strSign ==8776)
    {
        isok  = true;
    }
    else if(type == 2)
    {
        if(strSign == ')')
            isok  = true;
    }
    return isok;
}

bool StackManager::SlectSaveAddress(const QString strkey, const QString strvalue, const QChar sign)
{
    QString souce = "";
    auto items = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec;
    for(int i = 0; i < items.size();++i)
    {
        if(items[i].key == strkey)
        {
            souce = ReadTxt(items[i].address);
            break;
        }
    }
    if(souce.isEmpty()) //判断txt没有key值,常量字符串也可以数据比对
    {
        souce = strkey;
    }
    if(!souce.isEmpty())
    {
        if(sign.unicode()== 61 )
            souce = souce.replace(QRegExp("\n+"),"");
        QString dest = "";
        for(int i = 0; i < items.size();++i)
        {
            if(items[i].key == strvalue)
            {
                dest = ReadTxt(items[i].address);
                if(sign.unicode()== 61 )
                    dest = dest.replace(QRegExp("\n+"),"");
                return CompareData(souce,dest,sign);
                break;
            }
        }
        dest = strvalue;
        return CompareData(souce,dest,sign);
    }

    return false;
}
bool StackManager::CompareData(const QString dest, const QString source, const QChar sign)
{
    bool isok = true;
    int isign = sign.unicode();
    switch(isign)
    {
    case 60: // <
        if(dest.toDouble() - source.toDouble() >= 0)
            isok = false;
        break;
    case 61: // =
        if(dest.compare(source) != 0)
            isok = false;
        break;
    case 62: // >
        if(dest.toDouble() - source.toDouble() <= 0)
            isok = false;
        break;
    case 8804://≤ ox2264
        if(dest.toDouble() - source.toDouble() > 0)
            isok = false;
        break;
    case 8805://≥ ox2265
        if(dest.toDouble() - source.toDouble() < 0)
            isok = false;
        break;
    case 8776://≈ ox2248
        if(!source.contains(dest))
            isok = false;
        break;
    default:
        isok = false;
        break;
    }
    return isok;
}
QString StackManager::ReadTxt(QString path)
{
    QString str ="";
    if(!path.isEmpty())
    {
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/" + "Data/" + path;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream textstream(&file);  
            str = textstream.readAll();
        }
        else
        {
            str = "";// 打开失败
        }
    }
    return str;
}
