#include "movevarvalue.h"
#include <sys/time.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QStandardPaths>
MoveVarValue::MoveVarValue()
{
    m_moduleName = "MOV";
}
void MoveVarValue::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    temppath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/";

}
void MoveVarValue::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.move_var;
}
void MoveVarValue::RunModule()
{
    //todo call command for IO

}

int  MoveVarValue::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    QString strValue = QString(_ps.Value);
    //将读到的信息写到变量里面字符串型寄存器
    DBInstanceManager::getDBInstance()->m_rmutex.lock();
    if(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(_ps.Variable))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        if(_ps.Value.contains(".txt"))
        {
           QByteArray buf;
           buf.clear();
           if(ReadTxt(_ps.Value,buf))
           {
               if(buf.size() > 1 && buf.contains("\n")) //去掉\n
               {
                   buf.chop(1);                 
               }
               DBInstanceManager::getDBInstance()->m_mutex.lock();
               memcpy(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[_ps.Variable].textval,buf.data(),64);
               iresult = 1;
               strMsg = "变量赋值成功";
           }
           else
           {
               strMsg = "赋值文件不存在！";
               iresult = -2;
               return iresult;
           }
        }
        else
        {
           DBInstanceManager::getDBInstance()->m_mutex.lock();
            if(strValue == "NULL")
            {
                memset(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[_ps.Variable].textval,0,64);
            }
            else
            {
                memcpy(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[_ps.Variable].textval,strValue.toStdString().c_str(),64);
            }
            iresult = 1;
            strMsg = "变量赋值成功";
        }
        DBInstanceManager::getDBInstance()->m_mutex.unlock();
    }
    else if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(_ps.Variable))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        if(_ps.Value.contains(".txt"))
        {
           QByteArray buf;
           buf.clear();
           if(ReadTxt(_ps.Value,buf))
           {
               if(buf.size() > 1 && buf.contains("\n")) //去掉\n
               {
                   buf.chop(1);
               }
               DBInstanceManager::getDBInstance()->m_mutex.lock();
               DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.Variable].Initvalue = buf.toDouble();
               iresult = 1;
               strMsg = "变量赋值成功";
           }
           else
           {
               strMsg = "赋值文件不存在！";
               iresult = -2;
               return iresult;
           }
        }
        else
        {
           DBInstanceManager::getDBInstance()->m_mutex.lock();
            if(strValue == "NULL")
            {
                 DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.Variable].Initvalue = 0;
            }
            else
            {
                DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.Variable].Initvalue = strValue.toDouble();
            }
            iresult = 1;
            strMsg = "变量赋值成功";
        }

        DBInstanceManager::getDBInstance()->m_mutex.unlock();
    }
    DBInstanceManager::getDBInstance()->m_rmutex.unlock();
    if(iresult != 1)
    {
        strMsg = "变量不存在！";
    }
    return iresult;
}
void MoveVarValue::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *MoveVarValue::Copy()
{
    MoveVarValue *s = new MoveVarValue();
    return s;
}

bool MoveVarValue::ReadTxt(QString path,QByteArray &buff)
{
    path = temppath + path;
    QFile file (path);
    if(file.open(QIODevice::ReadOnly))
    {
        buff = file.readAll();
        file.close();
        return true;
    }
    else
        return false;
}
