#include "dbinstancemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QCoreApplication>
DBInstanceManager::CGarbo DBInstanceManager::Garbo;
DBInstanceManager* DBInstanceManager::pDBManager = new DBInstanceManager();
DBInstanceManager::DBInstanceManager()
{
    m_pBDataBase = new DataBaseBatchManager();

}

DBInstanceManager::~DBInstanceManager()
{

}

DBInstanceManager *DBInstanceManager::getDBInstance()
{
    //    if(NULL == pDBManager)
    //    {
    //        pDBManager = new DBInstanceManager();
    //    }
    return pDBManager;
}

void DBInstanceManager::ReadVarDataFromDB()
{
    QString  tablename = "Variablemanagement";
    QSqlQuery query = m_pBDataBase->ExcBatchSelectDb(tablename);
    DBVarStru.regtextMap.clear();
    DBVarStru.regnumMap.clear();
    DBVarStru.SwitchMap.clear();
    DBVarStru.timeMap.clear();
    DBVarStru.cyMap.clear();
    DBVarStru.qpMap.clear();
    DBVarStru.cspMap.clear();
    DBVarStru.countMap.clear();
    while (query.next()) {
        QString type = query.value("Type").toString();
        QByteArray bytearray = query.value("Datastru").toByteArray();
        QString name = query.value("Name").toString();
        if(type == "字符串型寄存器")
        {
            VARDB_REGTEXT_STRU *reg = NULL;
            reg =(VARDB_REGTEXT_STRU*)(bytearray.data());
            if(reg != NULL)
            {
                DBVarStru.regtextMap.insert(name,*reg);
            }
        }
        else if(type == "数值型寄存器")
        {
            VARDB_REGNUM_STRU *num = NULL;
            num =(VARDB_REGNUM_STRU*)(bytearray.data());
            if(num != NULL)
            {
                DBVarStru.regnumMap.insert(name,*num);
            }
        }
        else if(type == "开关类"){
            VARDB_SWITCH_STRU *sw = NULL;
            sw =(VARDB_SWITCH_STRU*)(bytearray.data());         
            if(sw != NULL)
            {
                DBVarStru.SwitchMap.insert(name,*sw);
            }
        }
        else if(type == "计时类"){
            VARDB_TIME_STRU *tim = NULL;
            tim =(VARDB_TIME_STRU*)(bytearray.data());
            if(tim != NULL)
            {
                DBVarStru.timeMap.insert(name,*tim);
            }
        }
        else if(type == "计数类"){
            VARDB_COUNT_STRU *count = NULL;
            count =(VARDB_COUNT_STRU*)(bytearray.data());//得到纪录中的BLOB字段
            if(count != NULL)
            {
                DBVarStru.countMap.insert(name,*count);
            }
        }
        else if(type == "气缸类"){
            VARDB_CY_STRU *cy = NULL;
            cy =(VARDB_CY_STRU*)(bytearray.data());//得到纪录中的BLOB字段
            if(cy != NULL)
            {
                DBVarStru.cyMap.insert(name,*cy);
            }
        }
        else if(type == "运动控制PP模式"){
            VARDB_PPMOTCTR_STRU *qp = NULL;
            qp =(_VARDB_PPMOTCTR_STRU*)(bytearray.data());//得到纪录中的BLOB字段
            if(qp != NULL)
            {
                DBVarStru.qpMap.insert(name,*qp);
            }
        }
        else if(type == "运动控制CSP模式"){
            VARDB_LCSP_STRU *csp = NULL;
            csp =(VARDB_LCSP_STRU*)(bytearray.data());//得到纪录中的BLOB字段
            if(csp != NULL)
            {
                DBVarStru.cspMap.insert(name,*csp);
            }
        }
    }
}

void DBInstanceManager::ReadProDataFromDB(QString tablename)
{
    DBProInfoStru.proinfoMap.clear();
    m_pBDataBase->ReadMapDataFromDB(DBProInfoStru.proinfoMap,tablename);
}

void DBInstanceManager::ReadParamDataFromDB()//获取参数表的信息
{
    QString  tablename = "ParameterInfo";
    DBParamInfoStru.paramainfoMap.clear();
    m_pBDataBase->ReadMapDataFromDB(DBParamInfoStru.paramainfoMap,tablename);
}

bool DBInstanceManager::DelDBDataByPkey(const QString tablename, QString keyname, QList<QVariant> keys){
    bool isok = true;
    isok = m_pBDataBase->ExcBatchDeleteDb(tablename,keyname,keys);
    if(isok)
    {
        QStringList listkeys ;
        listkeys.clear();
        for(int i = 0; i < keys.size();++i)
        {
            listkeys<<keys[i].toString();
        }
        if(tablename == "Variablemanagement")
        {
            DelVarRAMDataByDB(listkeys);
        }
        else if(tablename == "DevReaTimInputInfo")
        {
            TDELRAMDataByDB(listkeys,DBReaTimInfoStru.reatiminfomap);
        }
        else if(tablename == "ParameterInfo") {
            TDELRAMDataByDB(listkeys,DBParamInfoStru.paramainfoMap);
        }
        else if(tablename == "Productioninformation")
        {
            TDELRAMDataByDB(listkeys,DBProInfoStru.proinfoMap);
        }
    }
    return isok;
}

void DBInstanceManager::DelVarRAMDataByDB(QStringList listkeys){
    for(int i = 0; i < listkeys.size();++i)
    {
        if(DBVarStru.regtextMap.contains(listkeys[i]))
        {
            DBVarStru.regtextMap.remove(listkeys[i]);
        }
        else if(DBVarStru.regnumMap.contains(listkeys[i]))
        {
            DBVarStru.regnumMap.remove(listkeys[i]);
        }
        else if(DBVarStru.SwitchMap.contains(listkeys[i]))
        {
            DBVarStru.SwitchMap.remove(listkeys[i]);
        }
        else if(DBVarStru.countMap.contains(listkeys[i]))
        {
            DBVarStru.countMap.remove(listkeys[i]);
        }
        else if(DBVarStru.timeMap.contains(listkeys[i]))
        {
            DBVarStru.timeMap.remove(listkeys[i]);
        }
        else if(DBVarStru.cyMap.contains(listkeys[i]))
        {
            DBVarStru.cyMap.remove(listkeys[i]);
        }
        else if(DBVarStru.qpMap.contains(listkeys[i]))
        {
            DBVarStru.qpMap.remove(listkeys[i]);
        }
        else if(DBVarStru.cspMap.contains(listkeys[i]))
        {
            DBVarStru.cspMap.remove(listkeys[i]);
        }
    }
}

bool DBInstanceManager::AddWriteRealTimeDataToDB(QVector<QString> axisnames)
{
    //NAME-Errorcode;ReadySw;Fault;Curpos;Curtq
    QVector<QString>axisdatastr;
    axisdatastr<<"Errorcode"<<"ReadySw"<<"Fault"<<"Curpos"<<"Curtq";
    QMap<QString,DEVREATIMINPUTINFODBCONFIG_STRU> realinfoMap;
    for(int i = 0; i < axisnames.size();++i)
    {
        for(int j = 0; j < axisdatastr.size();++j)
        {
            DEVREATIMINPUTINFODBCONFIG_STRU realinfostru;
            QString pkeyname = QString("%1_%2").arg(axisnames[i]).arg(axisdatastr[j]);
            strcpy(realinfostru.pkey,pkeyname.toStdString().c_str());
            if(realinfoMap.contains(pkeyname))
            {
                return false;
            }
            else{
                realinfoMap.insert(realinfostru.pkey,realinfostru);
            }
        }
    }
    if(!realinfoMap.isEmpty())
    {
        if(m_pBDataBase->ExcTMapBatchInsertDb("DevReaTimInputInfo",realinfoMap))
        {
            //新增实时信息内存数据操作新增
            for(auto it = realinfoMap.begin(); it != realinfoMap.end();++it)
            {
                if(!DBReaTimInfoStru.reatiminfomap.contains(it.key()))
                {
                    DBReaTimInfoStru.reatiminfomap.insert(it.key(),it.value());
                }
            }
            return true;
        }
    }
    return false;
}

bool DBInstanceManager::UpdateWriteRealTimeDataToDB(QString oldaxisname, QString newaxisname)
{
    QVector<QString> axisnames;
    axisnames.append(oldaxisname);
    if(!DelWriteRealTimeDataToDB(axisnames))
    {
        return false;
    }
    axisnames.clear();
    axisnames.append(newaxisname);
    if(!AddWriteRealTimeDataToDB(axisnames))
    {
        return false;
    }
    return true;
}

bool DBInstanceManager::DelWriteRealTimeDataToDB(QVector<QString> axisnames)
{
    QVector<QString>axisdatastr;
    axisdatastr<<"Errorcode"<<"ReadySw"<<"Fault"<<"Curpos"<<"Curtq";
    QList <QVariant> oldnames;
    //先删除旧的内容在更新
    for(int j = 0; j < axisnames.size();++j)
    {
        for(int i = 0 ; i < axisdatastr.size(); ++i)
        {
            QString pkeyname = QString("%1_%2").arg(axisnames[j]).arg(axisdatastr[i]);
            if(!oldnames.contains(pkeyname))
            {
                oldnames.append(pkeyname);
            }
            else{
                return false;
            }
        }
    }
    if(!m_pBDataBase->ExcBatchDeleteDb("DevReaTimInputInfo","Name",oldnames))
    {
        return false;
    }
    //删除实时信息内存数据
    for(int i = 0 ; i < oldnames.size();++i)
    {
        if(DBReaTimInfoStru.reatiminfomap.contains(oldnames[i].toString()))
        {
          DBReaTimInfoStru.reatiminfomap.remove(oldnames[i].toString());
        }
    }
    return true;
}

void DBInstanceManager::ReadRealTimeDataFromDB()
{
    DBReaTimInfoStru.reatiminfomap.clear();
    m_pBDataBase->ReadMapDataFromDB(DBReaTimInfoStru.reatiminfomap,"DevReaTimInputInfo");
}
///
/// \brief DBInstanceManager::AddWriteIOVarToDB
/// \param ID
/// \param bytenum
/// \return
///

bool DBInstanceManager::AddWriteIOVarToDB(QString ID, int bytenum)
{
    if((!ID.isEmpty())&& (bytenum > 0)&&((ID.contains("E"))||(ID.contains("Q"))))
    {
        QMap<QString,VARDB_SWITCH_STRU> addIOMap;
        for(int i = 0; i < bytenum*8; ++i )
        {
            QString IOname = QString("%1.%2").arg(ID).arg(i);
            VARDB_SWITCH_STRU Iostru;
            strncpy(Iostru.pkey,IOname.toStdString().c_str(),32);
            strncpy(Iostru.fkey,QString("type000").toStdString().c_str(),32);
            strncpy(Iostru.IOaddr,IOname.toStdString().c_str(),16);
            strncpy(Iostru.type,QString("开关类").toStdString().c_str(),32);
            Iostru.ishwIO = true;
            if(!addIOMap.contains(IOname))
            {
                addIOMap.insert(IOname,Iostru);
            }
            else{
                addIOMap[IOname] = Iostru;
            }
        }
        if(addIOMap.size() > 0)
        {
            int  isok = false;
            isok =  WriteDataToDB("Variablemanagement",addIOMap,true,true);
            if(isok)
            {
                return true;
            }
        }
    }
    return false;
}

bool DBInstanceManager::DelWriteIOVarToDB(QString ID, int bytenum)
{
    if((!ID.isEmpty())&&(bytenum > 0)&&((ID.contains("E"))||(ID.contains("Q"))))
    {
        QList<QVariant> IOkeys;
        for(int i = 0; i < bytenum*8; ++i )
        {
            QString IOname = QString("%1.%2").arg(ID).arg(i);

            if(!IOkeys.contains(IOname))
            {
                IOkeys.append(IOname);
            }
        }
        if(IOkeys.size() > 0)
        {
            bool isok = true;
            isok = DelDBDataByPkey("Variablemanagement","Name",IOkeys);
            if(isok)
            {
                return true;
            }
        }
    }
    return false;
}

bool DBInstanceManager::UpdateWriteIOVarToDB(QString preID, int prebytenum, QString nowID, int nowbytenum)
{
   if(!DelWriteIOVarToDB(preID,prebytenum))
   {
       return false;
   }
    if(!AddWriteIOVarToDB(nowID,nowbytenum))
    {
        return false;
    }
    return true;
}

bool DBInstanceManager::AddWritePPVarToDB(SWCONFIGXML_QPOSIT_STRU pp)
{
    if(!CheckStrInDB(pp.id))
    {
        return false;
    }
    QStringList regnamelist;
    QString axisinfo;
    for(auto it = pp.axisMap.begin(); it != pp.axisMap.end();++it )
    {
        QString name = QString("%1_%2").arg(pp.id).arg(it.key());
        if(!CheckStrInDB(name))
        {
            return false;
        }
        regnamelist<<name;
        axisinfo += it.key()+":"+ name+","+QString::number(it.value().speed)+","+ it.value().offsetPos + ";";
    }
    if(!axisinfo.isEmpty())
    {
        axisinfo.chop(1);
    }
    else{
        return false;
    }
     //组合写入数据
     QMap<QString ,VARDB_REGTEXT_STRU> ppposMap;
     QMap<QString,VARDB_PPMOTCTR_STRU> ppMap;
     VARDB_PPMOTCTR_STRU ppstru;
     ppstru.acc = pp.acc;
     ppstru.dec = pp.dec;
     ppstru.delay = pp.delayMsec;
     ppstru.save = true;
     ppstru.timeOut = pp.timeOut;
     memcpy(ppstru.pkey,pp.id.toStdString().c_str(),32);
     memcpy(ppstru.fkey,QString("type000").toStdString().c_str(),32);
     memcpy(ppstru.type,QString("运动控制PP模式").toStdString().c_str(),32);
     memcpy(ppstru.axisidinfo,axisinfo.toStdString().c_str(),256);
     ppMap.insert(pp.id,ppstru);
     for(int i = 0; i < regnamelist.size() ; ++i)
     {
         VARDB_REGTEXT_STRU regstru;
         regstru.save = true;
         memcpy(regstru.type,QString("数值型寄存器").toStdString().c_str(),32);
         memcpy(regstru.fkey,QString("type000").toStdString().c_str(),32);
         memcpy(regstru.pkey,regnamelist[i].toStdString().c_str(),32);
         ppposMap.insert(regnamelist[i],regstru);
     }
     //写入数据库并更新内存
     if(ppposMap.size() > 0 && ppMap.size() > 0)
     {
        if(WriteDataToDB("Variablemanagement",ppposMap,true,true)&&\
         WriteDataToDB("Variablemanagement",ppMap,true,true))
        {
            return true;
        }
     }
     return false;
}

bool DBInstanceManager::DelWritePPVarToDB(QList<SWCONFIGXML_QPOSIT_STRU> pplist)
{
    QList<QVariant> listkeys;
    for(int i = 0; i < pplist.size();++i)
    {
        for(auto it = pplist[i].axisMap.begin(); it != pplist[i].axisMap.end();++it )
        {
            QString name = QString("%1_%2").arg(pplist[i].id).arg(it.key());
            listkeys<<name;
        }
        if(listkeys.size()  > 0)
        {
            listkeys<<pplist[i].id;
        }
        else{
            return false;
        }
    }
    if(listkeys.size()  > 0)
    {
         if(DelDBDataByPkey("Variablemanagement","Name",listkeys))
         {
             return true;
         }
    }
  return false;
}

bool DBInstanceManager::CheckStrInDB(QString str)
{
    QMutexLocker lock(&m_rmutex);
    if((DBVarStru.qpMap.contains(str))||(DBVarStru.countMap.contains(str))||(DBVarStru.cspMap.contains(str))\
           ||(DBVarStru.regnumMap.contains(str))|| (DBVarStru.regtextMap.contains(str)) ||\
            (DBVarStru.SwitchMap.contains(str))|| (DBVarStru.timeMap.contains(str)) ||  (DBVarStru.cyMap.contains(str)))
    {
        return false;
    }
    return true;
}

bool DBInstanceManager::CheckUserNameandPasswordIsMatch(QString username, QString password,int &permiss)
{
    QString tablename = "PermissionsManagement";
    QSqlQuery query =  m_pBDataBase->ExcBatchSelectDb(tablename);
    while (query.next()) {
        QString user = query.value("UserName").toString();
        if(user == username)
        {
            QString passwordstr = query.value("Password").toString();
            if(passwordstr == password)
            {
                permiss = query.value("PermissionLevel").toInt();
                return true;
            }
        }
    }
    return false;
}

QStringList DBInstanceManager::GetAllUserNamesFromDB()
{
    QStringList names;
    names.clear();
    names<<"请选择用户ID";
    QString tablename = "PermissionsManagement";
    QSqlQuery query =  m_pBDataBase->ExcBatchSelectDb(tablename);
    while (query.next()) {
        QString user = query.value("UserName").toString();
        names<<user;
    }
    return names;
}

VARALLDBCONFIG_STRU DBInstanceManager::GetVarAllDBstru()
{
    return vardbstru;
}

void DBInstanceManager::ReadDBVarData(VARALLDBCONFIG_STRU &vardbstru)
{
    QString tablename = "Variablemanagement";//从数据中表格查询
    QSqlQuery query =  m_pBDataBase->ExcBatchSelectDb(tablename);
    while(query.next())
    {
        VARALLTYPEDBCONFIG_STRU allstru;
        allstru.axisid = query.value("axisid").toString();
        allstru.axisinfo = query.value("axisinfo").toString();
        allstru.basePoint = query.value("basePoint").toString();
        allstru.childtype = query.value("childtype").toString();
        allstru.curvalue = query.value("curvalue").toString();
        allstru.delay =query.value("delay").toInt();
        allstru.endIO = query.value("endIO").toString();
        allstru.startIO = query.value("startIO").toString();
        allstru.formulaid = query.value("formulaid").toString();
        allstru.Initvalue = query.value("Initvalue").toString();
        allstru.lowvalue = query.value("lowvalue").toString();
        allstru.movePoint = query.value("movePoint").toString();
        allstru.name = query.value("name").toString();
        allstru.note = query.value("note").toString();
        allstru.pointnum = query.value("pointnum").toInt();
        allstru.lowvalue = query.value("lowvalue").toString();
        allstru.remarkstr = query.value("remarkstr").toString();
        allstru.CyMsgMap = query.value("CyMsgMap").toString();
        allstru.acc = query.value("acc").toInt();
        allstru.dec = query.value("dec").toInt();
        allstru.save = true;
        allstru.setvalue = query.value("setvalue").toInt();
        allstru.type = query.value("type").toString();
        allstru.soltCompVec = query.value("soltCompVec").toString();
        allstru.upvalue =  query.value("upvalue").toString();
        allstru.axisid = query.value("axisid").toString();
        allstru.timeOut = query.value("timeOut").toInt();
        allstru.speed = query.value("speed").toInt();
        allstru.setvalue = query.value("setvalue").toString();
        vardbstru.varlist.append(allstru);
    }
}

bool DBInstanceManager::WriteDBVarData(VARALLDBCONFIG_STRU vardbstru)
{
    //通过是否保存环节保存的挑选仅仅保存名字基本信息内容
    VARALLDBCONFIG_STRU savevardbstru;
    for(int i = 0 ;i < vardbstru.varlist.size();++i)
    {
        if(vardbstru.varlist[i].save)
        {
            savevardbstru.varlist.append(vardbstru.varlist[i]);
        }
        else {
            VARALLTYPEDBCONFIG_STRU stru;
            stru.name = vardbstru.varlist[i].name ;
            stru.type = vardbstru.varlist[i].type;
            stru.formulaid = vardbstru.varlist[i].formulaid;
            stru.note = vardbstru.varlist[i].note;
            stru.remarkstr = vardbstru.varlist[i].remarkstr;
            savevardbstru.varlist.append(stru);
        }
    }
    //先查出不存在的keys
    QList<QVariant>  delkeyname =  DelDBVarData(savevardbstru);
    //替换掉改变的值插入新的值
    QStringList names;
    QString tablename = "";
    QList<QVariantList> valueslist;
    Getnamesandvaluesfromstru(tablename,savevardbstru,names,valueslist);
    if(m_pBDataBase->ExcBatchReplaceDb(tablename,names,valueslist))//先进行替换插入
    {
        //        if(delkeyname.size()!=0)
        //        {
        //            DataBaseBatchManager::GetInstance()->ExcBatchDeleteDb("Variablemanagement","name",delkeyname);
        //        }
    }
    else{
        //qDebug()<<"数据更新失败";
        return false;
    }
    return true;
}

QList<QVariant> DBInstanceManager::DelDBVarData(VARALLDBCONFIG_STRU vardbstru)
{
    VARALLDBCONFIG_STRU Rvardbstru;
    ReadDBVarData(Rvardbstru);  //先删除不存在的keys
    QList<QVariant> keyname;
    QList<QVariant> delkeyname;
    for(int i=0;i<vardbstru.varlist.size();++i)
    {
        keyname.append(vardbstru.varlist[i].name);
    }
    for(int j=0;j<Rvardbstru.varlist.size();++j)
    {
        if(!keyname.contains(Rvardbstru.varlist[j].name))
        {
            delkeyname.append(Rvardbstru.varlist[j].name);
        }
    }
    return delkeyname;
}

void DBInstanceManager::Getnamesandvaluesfromstru(QString &tablename, VARALLDBCONFIG_STRU vardbstru, QStringList &names, QList<QVariantList> &valueslist)
{
    tablename = "Variablemanagement";
    names<<"name"<<"type"<<"childtype"<<"pointnum"<<"Initvalue"<<"addvalue"<<"setvalue"<<"curvalue"\
        <<"upvalue"<<"lowvalue"<<"startIO"<<"endIO"<<"soltCompVec"<<"movePoint"<<"basePoint"<<"CyMsgMap"\
       <<"speed"<<"acc"<<"dec"<<"axisid"<<"axisinfo"<<"delay"<<"timeOut"<<"value"<<"note"<<"remarkstr"<<"formulaid";
    for(int i = 0; i< vardbstru.varlist.size();++i)
    {
        QVariantList values;
        values.append(vardbstru.varlist[i].name);
        values.append(vardbstru.varlist[i].type);
        values.append(vardbstru.varlist[i].childtype);
        values.append(vardbstru.varlist[i].pointnum);
        values.append(vardbstru.varlist[i].Initvalue);
        values.append(vardbstru.varlist[i].addvalue);
        values.append(vardbstru.varlist[i].setvalue);
        values.append(vardbstru.varlist[i].curvalue);
        values.append(vardbstru.varlist[i].upvalue);
        values.append(vardbstru.varlist[i].lowvalue);
        values.append(vardbstru.varlist[i].startIO);
        values.append(vardbstru.varlist[i].endIO);
        values.append(vardbstru.varlist[i].soltCompVec);
        values.append(vardbstru.varlist[i].movePoint);
        values.append(vardbstru.varlist[i].basePoint);
        values.append(vardbstru.varlist[i].CyMsgMap);
        values.append(vardbstru.varlist[i].speed);
        values.append(vardbstru.varlist[i].acc);
        values.append(vardbstru.varlist[i].dec);
        values.append(vardbstru.varlist[i].axisid);
        values.append(vardbstru.varlist[i].axisinfo);
        values.append(vardbstru.varlist[i].delay);
        values.append(vardbstru.varlist[i].timeOut);
        values.append(vardbstru.varlist[i].value);
        values.append(vardbstru.varlist[i].note);
        values.append(vardbstru.varlist[i].remarkstr);
        values.append(vardbstru.varlist[i].formulaid);
        valueslist.append(values);
    }
}
