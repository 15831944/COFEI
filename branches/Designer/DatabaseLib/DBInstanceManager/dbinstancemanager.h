#ifndef DBINSTANCEMANAGER_H
#define DBINSTANCEMANAGER_H

#include "dbinstancemanager_global.h"
#include <DatabaseLib/variable.h>
#include <DatabaseLib/DataBaseBatchManager/databasebatchmanager.h>
#include <QMutex>
#include <Comm.h>

class DBINSTANCEMANAGERSHARED_EXPORT DBInstanceManager
{
private:
    DBInstanceManager();
    ~DBInstanceManager();
    static DBInstanceManager *pDBManager;
    class CGarbo //析构定义的指针垃圾站
    {
    public:
        ~CGarbo()
        {
            if(DBInstanceManager::pDBManager)
                if(DBInstanceManager::pDBManager->m_pBDataBase)
                {
                    delete  DBInstanceManager::pDBManager->m_pBDataBase;
                }
            delete DBInstanceManager::pDBManager;
        }
    };
    static CGarbo Garbo; //定义一个静态成员，程序结束时，系统会自动调用它的析构函数
public:
    DataBaseBatchManager *m_pBDataBase;
public:
    static DBInstanceManager *getDBInstance();
public:
    bool OpenDb(const QString &DbFilePath){
        return m_pBDataBase->OpenDb(DbFilePath);
    }
    void CloseDb(){
        m_pBDataBase->CloseDb();
    }
    template<typename T>
    void ReadBDataFromDB(QString tablename,QList<T> &BaseStrulist){
        m_pBDataBase->ReadBDataFromDB(tablename,BaseStrulist);
    }
    void ReadVarDataFromDB();
    void ReadProDataFromDB(QString tablename);
    void ReadParamDataFromDB();
    template<typename T>
    bool WriteDataToDB(QString tablename,QMap<QString,T>valuesmap,bool isDes = true,bool isRAM  = true){
        QMap<QString,T> saveMap;
        if(!isDes)
        {
            for(auto it = valuesmap.begin(); it!= valuesmap.end();it++)
            {
                if(it.value().save)
                {
                    saveMap.insert(it.key(),it.value());
                }
            }
        }
        else{
            saveMap = valuesmap;
        }
        bool isok = true;
        isok = m_pBDataBase->ExcMapTBatchReplaceDB(tablename,saveMap);
        if(isok&&isRAM)
        {
            if(saveMap.size() > 0)
            {
                if(tablename == "Variablemanagement")
                {
                    QString type = QString::fromUtf8(saveMap.begin().value().type);
                    if(type == "计数类")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_COUNT_STRU *count =(VARDB_COUNT_STRU*)(buffer);
                            if(DBVarStru.countMap.contains(it.key()))
                            {
                                DBVarStru.countMap[it.key()] = *count;
                            }
                            else{
                                DBVarStru.countMap.insert(it.key(),*count);
                            }
                        }
                    }
                    else if(type =="计时类")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_TIME_STRU *time =(VARDB_TIME_STRU*)(buffer);
                            if(DBVarStru.timeMap.contains(it.key()))
                            {
                                DBVarStru.timeMap[it.key()] = *time;
                            }
                            else{
                                DBVarStru.timeMap.insert(it.key(),*time);
                            }
                        }
                    }
                    else if(type =="字符串型寄存器")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_REGTEXT_STRU *reg =(VARDB_REGTEXT_STRU*)(buffer);
                            if(DBVarStru.regtextMap.contains(it.key()))
                            {
                                DBVarStru.regtextMap[it.key()] = *reg;
                            }
                            else{
                                DBVarStru.regtextMap.insert(it.key(),*reg);
                            }
                        }
                    }
                    else if(type =="开关类")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_SWITCH_STRU *sw =(VARDB_SWITCH_STRU*)(buffer);
                            if(DBVarStru.SwitchMap.contains(it.key()))
                            {
                                DBVarStru.SwitchMap[it.key()] = *sw;
                            }
                            else{
                                DBVarStru.SwitchMap.insert(it.key(),*sw);
                            }
                        }
                    }
                    else if(type =="数值型寄存器")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_REGNUM_STRU *NUM =(VARDB_REGNUM_STRU*)(buffer);
                            if(DBVarStru.regnumMap.contains(it.key()))
                            {
                                DBVarStru.regnumMap[it.key()] = *NUM;
                            }
                            else{
                                DBVarStru.regnumMap.insert(it.key(),*NUM);
                            }
                        }
                    }
                    else if(type =="运动控制PP模式")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_PPMOTCTR_STRU *NUM =(VARDB_PPMOTCTR_STRU*)(buffer);
                            if(DBVarStru.qpMap.contains(it.key()))
                            {
                                DBVarStru.qpMap[it.key()] = *NUM;
                            }
                            else{
                                DBVarStru.qpMap.insert(it.key(),*NUM);
                            }
                        }
                    }
                    else if(type =="气缸类")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_CY_STRU *cy =(VARDB_CY_STRU*)(buffer);
                            if(DBVarStru.cyMap.contains(it.key()))
                            {
                                DBVarStru.cyMap[it.key()] = *cy;
                            }
                            else{
                                DBVarStru.cyMap.insert(it.key(),*cy);
                            }
                        }
                    }
                    else if(type =="运动控制CSP模式")
                    {
                        for(auto it = saveMap.begin();it!= saveMap.end();++it)
                        {
                            int len = sizeof(T);
                            char buffer[len] = {0};
                            memset(buffer, 0, len);
                            memcpy(buffer,&it.value(),len);
                            VARDB_LCSP_STRU *csp =(VARDB_LCSP_STRU*)(buffer);
                            if(DBVarStru.cspMap.contains(it.key()))
                            {
                                DBVarStru.cspMap[it.key()] = *csp;
                            }
                            else{
                                DBVarStru.cspMap.insert(it.key(),*csp);
                            }
                        }
                    }
                }
                else if(tablename == "DevReaTimInputInfo")
                {
                    for(auto it = saveMap.begin();it!= saveMap.end();++it)
                    {
                        int len = sizeof(DEVREATIMINPUTINFODBCONFIG_STRU);
                        char buffer[len] = {0};
                        memset(buffer, 0, len);
                        memcpy(buffer,&it.value(),len);
                        DEVREATIMINPUTINFODBCONFIG_STRU *realvar =(DEVREATIMINPUTINFODBCONFIG_STRU*)(buffer);
                        if(DBVarStru.cspMap.contains(it.key()))
                        {
                            DBReaTimInfoStru.reatiminfomap[it.key()] = *realvar;
                        }
                        else{
                            DBReaTimInfoStru.reatiminfomap.insert(it.key(),*realvar);
                        }
                    }
                }
                else if(tablename == "Productioninformation")
                {
                    for(auto it = saveMap.begin();it!= saveMap.end();++it)
                    {
                        int len = sizeof(PROINFODBCONFIG_STRU);
                        char buffer[len] = {0};
                        memset(buffer, 0, len);
                        memcpy(buffer,&it.value(),len);
                        PROINFODBCONFIG_STRU *prostru =(PROINFODBCONFIG_STRU*)(buffer);
                        if(DBProInfoStru.proinfoMap.contains(it.key()))
                        {
                            DBProInfoStru.proinfoMap[it.key()] = *prostru;
                        }
                        else{
                            DBProInfoStru.proinfoMap.insert(it.key(),*prostru);
                        }
                    }
                }
            }
        }
        return isok;
    }
    bool DelDBDataByPkey(const QString tablename,QString keyname, QList<QVariant> keys);
private:
    void DelVarRAMDataByDB(QStringList listkeys);
    template<typename T>
    void TDELRAMDataByDB(QStringList pkeys,QMap<QString,T>typemap){
        for(int  i = 0; i > pkeys.size();++i)
        {
            if(typemap.contains(pkeys[i]))
            {
                typemap.remove(pkeys[i]);
            }
        }
    }
public:
    /***实时信息表操作函数*************************/
    bool AddWriteRealTimeDataToDB(QVector<QString> axisnames);
    bool UpdateWriteRealTimeDataToDB(QString oldaxisname,QString newaxisname);
    bool DelWriteRealTimeDataToDB(QVector<QString> axisnames);
    void ReadRealTimeDataFromDB();
public:
    /***变量IO新增删除保存数据库的操作*/
    bool AddWriteIOVarToDB(QString ID,int bytenum);
    bool DelWriteIOVarToDB(QString ID,int bytenum);
    bool UpdateWriteIOVarToDB(QString preID,int prebytenum,QString nowID,int nowbytenum);
    /***pp控制模式变量保存数据库操作*/
    bool AddWritePPVarToDB(SWCONFIGXML_QPOSIT_STRU pp);
    bool DelWritePPVarToDB(QList<SWCONFIGXML_QPOSIT_STRU> pplist);
private :
     bool CheckStrInDB(QString str);
public:/*新增用户名与密码对比函数*/
    bool CheckUserNameandPasswordIsMatch(QString username,QString password,int &permiss);
    QStringList GetAllUserNamesFromDB();
public:
    VARCONFIG_STRU DBVarStru;//变量表的结构
    PROINFOCONFIG_STRU DBProInfoStru;//生产信息表结构
    DEVREATIMINPUTINFOCONFIG_STRU DBReaTimInfoStru;//实时信息表
    PARAMINFOCONFIG_STRU  DBParamInfoStru; //参数信息表（输入输出如何定义）
    QMutex m_mutex;//新增数据操作锁
    QMutex m_rmutex;//新增数据读锁
public:
    VARALLDBCONFIG_STRU vardbstru;//旧版本
    VARALLDBCONFIG_STRU GetVarAllDBstru();//旧版本
    // data读从数据库文件读写内容复制到内存中
    void ReadDBVarData(VARALLDBCONFIG_STRU &vardbstru );//旧版本
    // data内存数据写入Database
    bool WriteDBVarData(VARALLDBCONFIG_STRU vardbstru);//旧版本
    QList<QVariant> DelDBVarData(VARALLDBCONFIG_STRU vardbstru);//对比前后数据删除不存在的keys
    void Getnamesandvaluesfromstru(QString &tablename,VARALLDBCONFIG_STRU vardbstru, QStringList &names, QList<QVariantList> &valueslist);
};

#endif // DBINSTANCEMANAGER_H
