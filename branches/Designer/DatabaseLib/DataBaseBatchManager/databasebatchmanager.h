#ifndef DATABASEBATCHMANAGER_H
#define DATABASEBATCHMANAGER_H

#include "databasebatchmanager_global.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QDebug>
class DATABASEBATCHMANAGERSHARED_EXPORT DataBaseBatchManager
{
public:
    DataBaseBatchManager();
public:
   // static DataBaseBatchManager * GetInstance();
    bool OpenDb(const QString &DbFilePath);
    void CreateDBTable(const QString &tableCmd);
    QSqlQuery ExcQueryDb(const QString &DbQueryCmd);
    bool ExcInsertDb(const QString& table, QStringList& names, QStringList& values);
    bool ExcDeleteDb(const QString &table, QString &expression);
    bool ExcUpdateDb(const QString &table, QStringList &names, QStringList &values, QString &expression);
    bool Excisexisttable(const QString table);
    bool ExcisExistfile(const QString filename);//需要添加的内容暂时//增加访问类型values  Q
    QSqlDatabase GetDataBase();
    void CloseDb();
    /*******批量操作database*************************************/
    bool ExcBatchInsertDb(const QString& table, QStringList& names, QList<QVariantList> &values);//针对多行操作
    bool ExcBatchDeleteDb(const QString &table,QString keyname, QList<QVariant> keys);
    bool ExcBatchDeltableDb(const QString& table);
    bool ExcBatchUpdateDb(const QString &table, QStringList &names, QVector<QVariantList> &valuesvec, QString keyname,QVector<QVariant> keyvalue);
    QSqlQuery ExcBatchSelectDb(const QString &table, QString keyname,QVariant name);//执行查询根据主键查询方式
    QSqlQuery ExcBatchSelectDb(const QString &table);//执行查询所有
    void GettablesFromDb();
    QString GetKeyType(QString tablename,QString name);
    bool ExcBatchReplaceDb(const QString& table, QStringList& names, QList<QVariantList> &values); //新增replace into 语句批量使用可替换插入和更新操作//如果没有其他字段赋值会把其他字段置为默认值
private:
    QSqlDatabase dbconn;
    QString currentConnName;
private:
    bool GettableNames(QString tablename,QStringList &names);
public:
    /****************模板类接口批量操作数据库*****************************************/
    template<typename T>
    bool ExcTListBatchInsertDb(QString tablename,QList<T> &valueslist){
        if( valueslist.size() < 1)
        {
            return false;
        }
        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString insertsql = QString("insert into %1").arg(tablename);//直接使用占位符方式
        insertsql += QString(" values(?, ?, ?, ?, ?, ?)");//对应的顺序是：pkeys,Datetime,type,note,remarkstr,fkeys,T;
        query.prepare(insertsql);
        for(int i = 0 ; i< valueslist.size();++i)
        {
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &valueslist[i],sizeof(T)); //写入多个数
            query.addBindValue(valueslist[i].pkey);
            query.addBindValue(valueslist[i].date);
            query.addBindValue(valueslist[i].type);
            query.addBindValue(valueslist[i].note);
            query.addBindValue(valueslist[i].fkey);
            query.addBindValue(binary_data);
            if(!query.exec())
            {
                dbconn.rollback();
                return false;
            }
        }
        if(!dbconn.commit())
        {
            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    bool ExcTMapBatchInsertDb(QString tablename,QMap<QString,T> &valuesMap){
        if(valuesMap.size() < 1)
        {
            return false;
        }
        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString insertsql = QString("insert into %1").arg(tablename);//直接使用占位符方式
        insertsql += QString(" values(?, ?, ?, ?, ?, ?)");//对应的顺序是：pkeys,Datetime,type,note,remarkstr,fkeys,T;
        query.prepare(insertsql);
        for(auto it = valuesMap.begin() ; it!= valuesMap.end();++it)
        {
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &it.value(),sizeof(T)); //写入多个数
            query.addBindValue(it.value().pkey);
            query.addBindValue(it.value().date);
            query.addBindValue(it.value().type);
            query.addBindValue(it.value().note);
            query.addBindValue(it.value().fkey);
            query.addBindValue(binary_data);
            if(!query.exec())
            {
                dbconn.rollback();
                return false;
            }
        }
        if(!dbconn.commit())
        {
            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    bool ExcTListBatchUpdateDB(QString tablename,QList<T> &valueslist){

        if( valueslist.size() < 1)
        {
            return false;
        }
        QStringList names ;
        if(!GettableNames(tablename,names))
        {
            return false;
        }
        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString updatesql = QString("update %1 set ").arg(tablename);//直接使用占位符方式
        for(int j = 0;j< names.size();++j)
        {
            updatesql+=QString("%1=?,").arg(names[j]);
        }
        updatesql.chop(1);
        updatesql += QString(" where %1").arg(names[0]);//默认第一个为主键
        for(int i=0;i<valueslist.size();++i )
        {
            QString keyid;
            keyid += QString("='%1'").arg(valueslist[i].pkey);
            QString str =QString("%1 %2").arg(updatesql).arg(keyid);
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &valueslist[i],sizeof(T)); //写入多个数
            query.prepare(str);
            query.bindValue(0,valueslist[i].pkey);
            query.bindValue(1,valueslist[i].date);
            query.bindValue(2,valueslist[i].type);
            query.bindValue(3,valueslist[i].note);
            query.bindValue(4,valueslist[i].fkey);
            query.bindValue(5,binary_data);
            if(!query.exec())
            {
                 dbconn.rollback();
                return false;
            }
        }
        if(!dbconn.commit())
        {
            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    bool ExcTMapBatchUpdateDB(QString tablename,QMap<QString,T> &valuesMap){

        if( valuesMap.size() < 1)
        {
            return false;
        }
        QStringList names ;
        if(!GettableNames(tablename,names))
        {
            return false;
        }
        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString updatesql = QString("update %1 set ").arg(tablename);//直接使用占位符方式
        for(int j = 0;j< names.size();++j)
        {
            updatesql+=QString("%1=?,").arg(names[j]);
        }
        updatesql.chop(1);
        updatesql += QString(" where %1").arg(names[0]);//默认第一个为主键
        for(auto it = valuesMap.begin(); it!=valuesMap.end();++it)
        {
            QString keyid;
            keyid += QString("='%1'").arg(it.value().pkey);
            QString str =QString("%1 %2").arg(updatesql).arg(keyid);
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &it.value(),sizeof(T)); //写入多个数
            query.prepare(str);
            query.bindValue(0,it.value().pkey);
            query.bindValue(1,it.value().date);
            query.bindValue(2,it.value().type);
            query.bindValue(3,it.value().note);
            query.bindValue(4,it.value().fkey);
            query.bindValue(5,binary_data);
            if(!query.exec())
            {
                  dbconn.rollback();
                return false;
            }
        }
        if(!dbconn.commit())
        {
            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    bool ExcTListBatchReplaceDB(QString tablename,QList<T> &valueslist){
        if(valueslist.size()<1)
            return false;
        QStringList names ;
        if(!GettableNames(tablename,names))
        {
            return false;
        }
        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString replacesql = QString("replace into %1").arg(tablename);//直接使用占位符方式
        replacesql += QString(" values(?, ?, ?, ?, ?, ?)");//对应的顺序是：pkeys,Datetime,type,note,remarkstr,fkeys,T;
        query.prepare(replacesql);
        for(int i = 0 ; i< valueslist.size();++i)
        {
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &valueslist[i],sizeof(T)); //写入多个数
            query.addBindValue(valueslist[i].pkey);
            query.addBindValue(valueslist[i].date);
            query.addBindValue(valueslist[i].type);
            query.addBindValue(valueslist[i].note);
            query.addBindValue(valueslist[i].fkey);
            query.addBindValue(binary_data);
            if(!query.exec())
            {
             dbconn.rollback();
              return false;
            }
        }
        if(!dbconn.commit())
        {

            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    bool ExcMapTBatchReplaceDB(QString tablename,QMap<QString,T> valuesmap)
    {
        if(valuesmap.size()<1)
            return false;
        QStringList names ;
        if(!GettableNames(tablename,names))
        {
            return false;
        }

        QSqlQuery query(dbconn);
        dbconn.transaction();//启动事务
        QString replacesql = QString("replace into %1").arg(tablename);//直接使用占位符方式
        replacesql += QString(" values(?, ?, ?, ?, ?, ?)");//对应的顺序是：pkeys,Datetime,type,note,remarkstr,fkeys,T;
        query.prepare(replacesql);
        for(auto it = valuesmap.cbegin(); it != valuesmap.cend();++it)
        {
            QByteArray binary_data;
            binary_data.resize(sizeof(T));
            memcpy(binary_data.data(), &it.value(),sizeof(T)); //写入多个数
            query.addBindValue(it.value().pkey);
            query.addBindValue(it.value().date);
            query.addBindValue(it.value().type);
            query.addBindValue(it.value().note);
            query.addBindValue(it.value().fkey);
            query.addBindValue(binary_data);
            if(!query.exec())
            {             
                 dbconn.rollback();
                return false;
            }
        }
        if(!dbconn.commit())
        {
            dbconn.rollback();
            return false;
        }
        return true;
    }
    template<typename T>
    void ReadDataFromDB(QList<T> &BaseStrulist,QString tablename){
        QSqlQuery query = ExcBatchSelectDb(tablename);
        while (query.next()) {
            QByteArray bytearray = query.value("Datastru").toByteArray();
            int len = bytearray.size();
            if(len == sizeof(T))
            {
                char buffer[len] = {0};
                memset(buffer, 0, len);
                char *data = NULL;
                data = bytearray.data();//得到纪录中的BLOB字段
                if(data != NULL)
                {
                    memmove(buffer, data, len);
                    T *reg =(T*)(buffer);
                    if(reg != NULL)
                        BaseStrulist.append(*reg);
                }
            }
        }
    }
    template<typename T>
    void ReadMapDataFromDB(QMap<QString,T> &BaseStruMap,QString tablename){
        QSqlQuery query = ExcBatchSelectDb(tablename);
        while (query.next()) {
            QByteArray bytearray = query.value("Datastru").toByteArray();
            int len = bytearray.size();
            if(len == sizeof(T))
            {
                char buffer[len] = {0};
                memset(buffer, 0, len);
                char *data = NULL;
                data = bytearray.data();//得到纪录中的BLOB字段
                if(data != NULL)
                {
                    memmove(buffer, data, len);
                    T *reg =(T*)(buffer);
                    QString pkey = QString(QLatin1String(reg->pkey));
                    if(reg != NULL)
                        BaseStruMap.insert(pkey,*reg);
                }
            }
        }
    }
    template<typename T>
    void ReadBDataFromDB(QString tablename,QList<T> &BaseStrulist){
        QSqlQuery query = ExcBatchSelectDb(tablename);

        while (query.next()) {
            QByteArray bytearray = query.value("Datastru").toByteArray();
            T *reg = NULL;
            reg =reinterpret_cast<T*>(bytearray.data());//得到纪录中的BLOB字段
            if(reg != NULL)
            {
                BaseStrulist.append(*reg);
            }
        }
    }
    template<typename T>
    void ReadDataFromDB(QString tablename,QHash<QString,T> &BaseStruhash){
        QSqlQuery query = ExcBatchSelectDb(tablename);
        while (query.next()) {
            QByteArray bytearray = query.value("Datastru").toByteArray();
            T *reg = NULL;
            reg =reinterpret_cast<T*>(bytearray.data());//得到纪录中的BLOB字段
            QString pkey = QString(QLatin1String(reg->pkey));
            if(reg != NULL)
            {
                BaseStruhash.insert(pkey,*reg);
            }
        }
    }
    ///模糊匹配主键的方式
    bool ExcBatchLikeDelDB(QString tablename,QString pkeyname,QString likestr);
    ///按照主键查询
    QSqlQuery ExcBatchSelectPkeysDB(QString tablename,QStringList pkeys);
    ///按照类型查询
    QSqlQuery ExcBatchSelectTypeDB(QString tablename,QString Type);
    ///按照日期起始时间查询
    QSqlQuery ExcBatchSelectDatatimeDB(QString tablename,QString starttime,QString endtime);

};



#endif // DATABASEMANAGER_H
