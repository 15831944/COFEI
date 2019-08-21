#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databasemanager_global.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
class DATABASEMANAGERSHARED_EXPORT DataBaseManager
{
private:
    DataBaseManager();
public:
    static DataBaseManager * GetInstance();
    bool OpenDb(const QString &DbFilePath);
    void CreateDBTable(const QString &tableCmd);
    QSqlQuery ExcQueryDb(const QString &DbQueryCmd);
    bool ExcInsertDb(const QString& table, QStringList& names, QStringList& values);
    bool ExcDeleteDb(const QString &table, QString &expression);
    bool ExcUpdateDb(const QString &table, QStringList &names, QStringList &values, QString &expression);
    QSqlDatabase GetDataBase();
    void CloseDb();
private:
    QSqlDatabase dbconn;
    QString currentConnName;
};

#endif // DATABASEMANAGER_H
