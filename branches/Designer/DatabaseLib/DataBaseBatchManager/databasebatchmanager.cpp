#include "databasebatchmanager.h"
#include <QVariant>
#include <QDebug>

DataBaseBatchManager::DataBaseBatchManager()
{
    currentConnName = "__COFEI__";

}

//DataBaseBatchManager *DataBaseBatchManager::GetInstance()
//{
//    static DataBaseBatchManager pInstance;

//    return &pInstance;
//}

bool DataBaseBatchManager::OpenDb(const QString &DbFilePath)//打开文件时执行约束
{
     QString str = "PRAGMA foreign_keys = ON" ;//打开约束
    if(!dbconn.isValid())
    {
        dbconn = QSqlDatabase::addDatabase("QSQLITE",currentConnName);
        dbconn.setDatabaseName(DbFilePath);
        dbconn.setPassword("123456");
    }
    if(dbconn.isOpen())
    {
        QSqlQuery query(dbconn);
        query.exec(str);
        return true;
    }
    else
    {
        if(!dbconn.open())
        {
            return false;
        }
    }
    return true;
}

void DataBaseBatchManager::CreateDBTable(const QString &tableCmd)
{
    QSqlQuery query(dbconn);
    query.exec(tableCmd);
}

QSqlQuery DataBaseBatchManager::ExcQueryDb(const QString &DbQueryCmd)
{
    QSqlQuery query(dbconn);
    query.exec(DbQueryCmd);
    return query;
}

bool DataBaseBatchManager::ExcInsertDb(const QString &table, QStringList &names, QStringList &values)
{
    if (names.size() != values.size())
    {
        return false;
    }
    QSqlQuery query(dbconn);
    QString sql = QString("insert into %1 (").arg(table);
    int i;
    for (i=0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        if(i != names.size() -1)
            sql += QString(",");
    }
    sql +=  QString(") values (");
    for (i = 0; i < values.size(); i++)
    {
        sql +=  QString("'%1'").arg(values.value(i));
        if(i != values.size() -1)
            sql += QString(",");
    }
    sql += QString(")");
    if(!query.exec(sql))
    {
        return false;
    }
    return true;

}

bool DataBaseBatchManager::ExcDeleteDb(const QString &table, QString &expression)
{
    QSqlQuery query(dbconn);
    QString sql = QString("delete from %1 ").arg(table) + QString(" where %1").arg(expression);
    if (!query.exec(sql))
    {
        return false;
    }
    return true;
}

bool DataBaseBatchManager::ExcUpdateDb(const QString &table, QStringList &names, QStringList &values, QString &expression)
{
    if (names.size() != values.size())
    {
        return false;
    }
    QSqlQuery query(dbconn);
    QString sql = QString("update ")+ table + QString(" set ");
    for (int i = 0; i < names.size(); i++)
    {
        sql +=  names.value(i);
        sql +=  QString(" = '");
        sql +=  values.value(i);
        sql +=  QString("'");
        if (i != names.size()-1)
        {
            sql += QString(" ,");
        }
    }
    sql +=  QString(" where %1").arg(expression);
    if (!query.exec(sql))
    {
        return false;
    }
    return true;
}

bool DataBaseBatchManager::Excisexisttable(const QString table)
{
    QSqlQuery query(dbconn);
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));
    if(query.next())
    {
        if(query.value(0).toInt()==0)//判断id
        {
            return false; // 表不存在
        }
        else
        {
            return true; //表存在
        }
    }
    return false;
}

QSqlDatabase DataBaseBatchManager::GetDataBase()
{
    return dbconn;
}

void DataBaseBatchManager::CloseDb()
{
    if(dbconn.isOpen())
    {
        dbconn.close();
    }
    QSqlDatabase::removeDatabase(dbconn.connectionName());
}

bool DataBaseBatchManager::ExcBatchInsertDb(const QString &table, QStringList &names,  QList<QVariantList> &values)
{
    for(int i = 0 ; i < values.size();++i )
    {
        if(names.size() != values[i].size() )
        {
            return false;
        }
    }
     QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString insertsql = QString("insert into %1(").arg(table);//直接使用占位符方式
    for( int j = 0;j< names.size(); ++j)
    {
        insertsql += names[j] +",";
    }
    insertsql.chop(1);
    insertsql += QString(") values(");
    for(int i = 0; i < names.size();++i)
    {
        insertsql +=QString("?")+",";
    }
    insertsql.chop(1);
    insertsql +=")";
    //qDebug()<<"sql:"<<insertsql;
    query.prepare(insertsql);
    QVector<QVariantList> listvec;//
    for(int i = 0 ; i< names.size();++i)
    {
        QVariantList list;
        for(int j=0; j < values.size();++j)
        {          
          list.append(values[j][i]);
        }
        listvec.append(list);
        //list
    }
    for(int i=0;i<listvec.size();++i)
    {
        query.addBindValue(listvec[i]);
    }
    if(!query.execBatch()){
       // qDebug()<<"eeror:"<<query.lastError().text();
    }//进行批处理操作保证顺序一样
    if(!dbconn.commit())
    {
        dbconn.rollback();
        return false;
    }
    return true;
}

bool DataBaseBatchManager::ExcBatchDeleteDb(const QString &table,QString keyname, QList<QVariant> keys)
{
     QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString keyType = GetKeyType(table,keyname);
    for(int i = 0; i < keys.size();++i)
    {
        QString sql;
        if(keyType.contains("VARCHAR"))
        {
            sql = QString("delete from %1 Where %2 = '%3'").arg(table).arg(keyname).arg(keys[i].toString());
        }
        else if(keyType.contains("INT")){

            sql = QString("delete from %1 Where %2 = %3").arg(table).arg(keyname).arg(keys[i].toString());
        }
        query.exec(sql);
    }
    if(!dbconn.commit())
    {
        dbconn.rollback();
        //qDebug()<<"error"<<query.lastError();
        return false;
    }
    return true;
}

bool DataBaseBatchManager::ExcBatchDeltableDb(const QString &table)
{
     QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString sql = QString("delete from %1").arg(table);
    query.exec(sql);
    if(!dbconn.commit())
    {
        dbconn.rollback();
       // qDebug()<<"error"<<query.lastError();
        return false;
    }
    return true;
}

bool DataBaseBatchManager::ExcBatchUpdateDb(const QString &table, QStringList &names, QVector<QVariantList> &valuesvec, QString keyname,QVector<QVariant> keyvalue)
{
     QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString updatesql = QString("update %1 set ").arg(table);//更新整个表格还是某些行
    for(int i=0;i<valuesvec.size();++i)
    {
        if(names.size()!= valuesvec[i].size())
        {
            return false;
        }
    }
    if(valuesvec.size()!= keyvalue.size())
    {
        return false;
    }
    for(int j=0;j<names.size();++j)
    {
        updatesql+=QString("%1=?,").arg(names[j]);
    }
    updatesql.chop(1);
    updatesql += QString(" where %1").arg(keyname);//默认第一个为主键
    QString keytype = GetKeyType(table,keyname);
    for(int i=0;i<valuesvec.size();++i )
    {
        QString keyid;
        if( keytype.contains("VARCHAR"))
        {
            keyid+=QString("='%1'").arg(keyvalue[i].toString());
        }
        else if(keytype.contains("INT"))
        {
            keyid+=QString("=%1").arg(keyvalue[i].toString());
        }
        QString str =QString("%1 %2").arg(updatesql).arg(keyid);
        //qDebug()<<"sql:"<<str;
        query.prepare(str);
        for(int j=0; j< valuesvec[i].size();++j)
        {
            query.bindValue(j,valuesvec[i][j]);
        }
        if(!query.exec())
        {
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

void DataBaseBatchManager::GettablesFromDb()
{
    QStringList tables = dbconn.tables();  //获取数据库中的表
    //qDebug() << QString::fromLocal8Bit("表的个数： %1").arg(tables.count()); //打印表的个数
    QStringListIterator itr(tables);
    while (itr.hasNext())
    {
        QString tableNmae = itr.next().toLocal8Bit();
       // qDebug() << QString::fromLocal8Bit("表名：")+ tableNmae;
    }
}

QString DataBaseBatchManager::GetKeyType(QString tablename, QString name)
{
    GettablesFromDb();
    //获取数据库中所有表信息
    QSqlQuery query(dbconn);
    QString strTableNmae = tablename;
    QString str = "PRAGMA table_info(" + strTableNmae + ")";
    query.prepare(str);
    QString nametype ="";
    if(query.exec())
    {
        while (query.next())
        {
            if(query.value(1).toString() == name)
            {
                nametype = query.value(2).toString();
               // qDebug()<<"本字段类型为："<< nametype;
                //qDebug() << QString(QString::fromLocal8Bit("字段数:%1     字段名:%2     字段类型:%3")).arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
                return nametype;
            }
        }
    }
    else
    {
        //qDebug() << query.lastError();
        //return query.lastError().text();
    }
    return nametype;
}

bool DataBaseBatchManager::ExcBatchReplaceDb(const QString &table, QStringList &names, QList<QVariantList> &values)
{
    for(int i = 0 ; i < values.size();++i )
    {
        if(names.size() != values[i].size() )
        {
            return false;
        }
    }
     QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString insertsql = QString("replace into %1(").arg(table);//直接使用占位符方式
    QString valuesnumstr ;
    for( int j = 0;j < names.size(); ++j)
    {
        insertsql += names[j] +",";
        valuesnumstr += QString("?")+",";
    }
    insertsql.chop(1);
    insertsql += QString(") values(") + valuesnumstr;
    insertsql.chop(1);
    insertsql +=")";
    //qDebug()<<"sql:"<<insertsql;
    query.prepare(insertsql);
    for(int i = 0 ; i < names.size();++i)
    {
        QVariantList list;
        for(int j = 0; j < values.size();++j)
        {
            list.append(values[j][i]);
        }
       query.addBindValue(list);
    }
    if(!query.execBatch()){
        //qDebug()<<"eeror:"<<query.lastError().text();
    }//进行批处理操作保证顺序一样
    if(!dbconn.commit())
    {
        dbconn.rollback();
        return false;
    }
    return true;
}

bool DataBaseBatchManager::GettableNames(QString tablename, QStringList &names){
    QSqlQuery query(dbconn);
    QString str = "PRAGMA table_info(" + tablename + ")";
    query.prepare(str);
    if(query.exec())
    {
        while (query.next())
        {
            names <<query.value(1).toString();
        }
    }
    if(names.size() < 1)
    {
        return false;
    }
    return true;
}

bool DataBaseBatchManager::ExcBatchLikeDelDB(QString tablename, QString pkeyname, QString likestr){
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString sql;
    sql = QString("delete from %1 Where %2 like '%%3%'").arg(tablename).arg(pkeyname).arg(likestr);
    query.exec(sql);
    if(!dbconn.commit())
    {
        dbconn.rollback();
        //qDebug()<<"error"<<query.lastError();
        return false;
    }
    return true;
}

QSqlQuery DataBaseBatchManager::ExcBatchSelectPkeysDB(QString tablename, QStringList pkeys){
    QStringList names;
    GettableNames(tablename,names);
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString selectsql = QString("select * from %1 where %2 in (").arg(tablename).arg(names[0]);
    for(int i = 0; i< pkeys.size();++i)
    {
        selectsql += QString("'%1',").arg(pkeys[i]);
    }
    selectsql.chop(1);
    selectsql +=")";
    query.exec(selectsql);
    dbconn.commit();
    return query;
}

QSqlQuery DataBaseBatchManager::ExcBatchSelectTypeDB(QString tablename, QString Type){
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString selectsql = QString("select * from %1 where Type = '%2'").arg(tablename).arg(Type);
    query.exec(selectsql);
    dbconn.commit();
    return query;
}

QSqlQuery DataBaseBatchManager::ExcBatchSelectDatatimeDB(QString tablename, QString starttime, QString endtime){
   // select * from Variablemanagement where Datetime between '2017-05-07 14:31:29.279' and '2018-05-09 09:50:29.279'
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString selectsql = QString("select * from %1 where Datetime between '%2' and '%3'").arg(tablename).arg(starttime).arg(endtime);
    query.exec(selectsql);
    dbconn.commit();
    return query;
}
///
/// \brief DataBaseManager::ExcBatchSelectDb
/// \param table
/// \param keyname
/// \param name
/// \return
///查找某一行记录//select * from Variablemanagement where name = 555;不需要区分类型待测试
QSqlQuery DataBaseBatchManager::ExcBatchSelectDb(const QString &table, QString keyname,QVariant name)
{
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString kettype = GetKeyType(table,keyname); //key 类型的是字符串类型的
    if("VARCHAR" == kettype)
    {
        QString selectsql = QString("select * from %1 where %2 = '%3'").arg(table).arg(keyname).arg(name.toString());
        query.prepare(selectsql);//
        query.exec();
        //qDebug()<<"SQL"<<selectsql;
    }
    else if("INTEGER" == kettype){
        QString selectsql = QString("select * from %1 where %2 = %3").arg(table).arg(keyname).arg(name.toString());
        query.prepare(selectsql);
        query.exec();
        //qDebug()<<"SQL"<<selectsql;
    }
    else {

    }
    dbconn.commit();
    return query;
}

QSqlQuery DataBaseBatchManager::ExcBatchSelectDb(const QString &table)
{
    QSqlQuery query(dbconn);
    dbconn.transaction();//启动事务
    QString selectsql = QString("select * from %1").arg(table);
    query.prepare(selectsql);//
    query.exec();
    dbconn.commit();
    return query;
}
