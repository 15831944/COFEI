#include "addcount.h"
#include <sys/time.h>
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
AddCount::AddCount()
{
    m_moduleName = "CNT";
    Count = "";
}
void AddCount::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    path_ok = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/" + "Data/OK.txt";
    path_ng = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/NG.txt";
}
void AddCount::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    Count = ps.iAddCount;
}
void AddCount::RunModule()
{
}

int  AddCount::GetExcResult(QString &strMsg)//从数据库读出设定的
{
    //变量名字
    int iresult = -2;
    QString ctkey = Count;
    DBInstanceManager::getDBInstance()->m_rmutex.lock();
    if(DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(ctkey))
    {
        QString  name =  DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].note;
        int addcut =  DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].addvalue;
        int maxcut = DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].setvalue;
        QString info =  QString("%1,%2").arg(name).arg( DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].curvalue);
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].curvalue =  DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].curvalue + addcut;
        DBInstanceManager::getDBInstance()->m_mutex.unlock();
        DBInstanceManager::getDBInstance()->m_rmutex.lock();
        SaveInfo(name,info);
        if(maxcut >=  DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].curvalue)  //在范围内
        {
            strMsg = QString("%1使用次数正常,加%2!").arg(name).arg(addcut);
            iresult = 1;
        }
        else
        {
            strMsg = QString("%1使用次数已达上限！请及时更换！").arg(name);
            if(QString(DBInstanceManager::getDBInstance()->DBVarStru.countMap[ctkey].funtype) == "0")
            {
                iresult = -3;
            }
            else
            {
                iresult = 1;
            }
        }
    }
     DBInstanceManager::getDBInstance()->m_rmutex.unlock();
    return iresult;
}
void AddCount::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *AddCount::Copy()
{
    AddCount *ac = new AddCount();

    return ac;
}

void AddCount::SaveInfo(QString info,QString str)
{
    if(info == "OK")
    {
        SaveTxt(path_ok,str);
    }
    else if (info == "NG")
    {
        SaveTxt(path_ng,str);
    }
}

void AddCount::SaveTxt(QString path,QString info)
{

    QFile file(path);
    bool isok = file.open(QIODevice::WriteOnly);
    if(isok)
    {
        QTextStream filestream(&file);
        filestream<<info;
    }
}
