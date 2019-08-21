#include "ThreadObject.h"
#include <QApplication>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
ThreadObject::ThreadObject()
{
    QString str = QCoreApplication::applicationDirPath();
    if(!DBInstanceManager::getDBInstance()->OpenDb(str+"/cofeidatabasev1.1.db"))
        emit ErrorEvent(0,tr("打开数据库(cofeidatabasev1.1.db)失败!"));
}

ThreadObject::~ThreadObject()
{
     DBInstanceManager::getDBInstance()->CloseDb();
}

void ThreadObject::AnalyDB()
{
  //  DBInstanceManager::getDBInstance()->ReadVarDataFromDB();
    DBInstanceManager::getDBInstance()->ReadRealTimeDataFromDB();
}

void ThreadObject::SaveRealTimeDB()
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
                                                      DBInstanceManager::getDBInstance()->DBVarStru.regtextMap,false,false);
    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
                                                      DBInstanceManager::getDBInstance()->DBVarStru.countMap,false,false);
    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
                                                      DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap,false,false);
    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
                                                      DBInstanceManager::getDBInstance()->DBVarStru.regnumMap,false,false);
    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
                                                      DBInstanceManager::getDBInstance()->DBVarStru.timeMap,false,false);
}

void ThreadObject::SaveSetUpDB()
{
//    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
//                                                      DBInstanceManager::getDBInstance()->DBVarStru.qpMap);
//    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
//                                                      DBInstanceManager::getDBInstance()->DBVarStru.cspMap);
}
