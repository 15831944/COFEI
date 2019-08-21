#ifndef VARMANAGER_H
#define VARMANAGER_H

#include <QObject>
#include "variable.h"
#include <databasebatchmanager.h>
class VarManager
{
public:
    VarManager();
    ~VarManager();
    static VarManager* pVarManage;
public:
    static VarManager* getDBInstance();
public:
    VARALLDBCONFIG_STRU vardbstru;
    VARALLDBCONFIG_STRU GetVarAllDBstru();
    // data读从数据库文件读写内容复制到内存中
    void ReadDBVarData(VARALLDBCONFIG_STRU &vardbstru );
public:
    // data内存数据写入Database
    bool WriteDBVarData(VARALLDBCONFIG_STRU vardbstru);
    QList<QVariant> DelDBVarData(VARALLDBCONFIG_STRU vardbstru);//对比前后数据删除不存在的keys
public:
   void ReadDataFromDB(QString tablename,VARCONFIG_STRU &Varstru);
private:
    void Getnamesandvaluesfromstru(QString &tablename,VARALLDBCONFIG_STRU vardbstru, QStringList &names, QList<QVariantList> &valueslist);
};

#endif // VARMANAGER_H
