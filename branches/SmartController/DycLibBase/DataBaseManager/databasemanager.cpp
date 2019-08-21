#include "databasemanager.h"

DataBaseManager::DataBaseManager()
{
    currentConnName = "__COFEI__";
}

DataBaseManager *DataBaseManager::GetInstance()
{
    static DataBaseManager pInstance;

    return &pInstance;
}

bool DataBaseManager::OpenDb(const QString &DbFilePath)
{
    if(!dbconn.isValid())
    {
        dbconn = QSqlDatabase::addDatabase("QSQLITE",currentConnName);
        dbconn.setDatabaseName(DbFilePath);
    }
    if(dbconn.isOpen())
    {
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

void DataBaseManager::CreateDBTable(const QString &tableCmd)
{
    QSqlQuery query(dbconn);
    query.exec(tableCmd);
}

QSqlQuery DataBaseManager::ExcQueryDb(const QString &DbQueryCmd)
{
    QSqlQuery query(dbconn);
    query.exec(DbQueryCmd);
    return query;
}

bool DataBaseManager::ExcInsertDb(const QString &table, QStringList &names, QStringList &values)
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

bool DataBaseManager::ExcDeleteDb(const QString &table, QString &expression)
{
    QSqlQuery query(dbconn);
    QString sql = QString("delete from %1 ").arg(table) + QString(" where %1").arg(expression);
    if (!query.exec(sql))
    {
        return false;
    }
    return true;
}

bool DataBaseManager::ExcUpdateDb(const QString &table, QStringList &names, QStringList &values, QString &expression)
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

QSqlDatabase DataBaseManager::GetDataBase()
{
    return dbconn;
}

void DataBaseManager::CloseDb()
{
    if(dbconn.isOpen())
    {
        dbconn.close();

    }
    QSqlDatabase::removeDatabase(dbconn.connectionName());
}
