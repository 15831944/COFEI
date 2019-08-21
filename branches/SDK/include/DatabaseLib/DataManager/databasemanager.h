#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databasemanager_global.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
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


    bool Excisexisttable(const QString table);
    bool Excisexistfile(const QString filename);//需要添加的内容暂时。。。 //增加访问类型values  Q
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
    //新增replace into 语句批量使用可替换插入和更新操作//如果没有其他字段赋值会把其他字段置为默认值
    bool ExcBatchReplaceDb(const QString& table, QStringList& names, QList<QVariantList> &values);
    //需要新增多表查询信息语句暂时？？？

private:
    QSqlDatabase dbconn;
    QString currentConnName;
};

#endif // DATABASEMANAGER_H
