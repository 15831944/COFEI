#include "definevariable.h"
#include <sys/time.h>
#include <databasemanager.h>
#include <QThread>
#include <QApplication>
#include <MyGlobal.h>
using namespace CoreGlobal;
DefineVariable::DefineVariable()
{
    m_moduleName = "TPV";
    m_dbBurninMap.clear();
}
void DefineVariable::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    isInitStatOk = false;
    imodel = 0;
    m_dbBurninMap.clear();
}
void DefineVariable::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    imodel = ps.iAddCount.toInt();
}
void DefineVariable::RunModule()
{
    //todo call command for IO
    QueryDb();
    for(auto item = m_dbBurninMap.begin();item != m_dbBurninMap.end();++item)
    {
        if(item.value() == 0x20 || item.value() == 0x08 || item.value() == 0x02)
        {
            isInitStatOk = true;
            break;
        }
    }
}

int  DefineVariable::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(m_dbBurninMap.isEmpty())
    {
        strMsg = QString("%1 DB BurnIn is empty()!").arg(m_moduleName);
        return iresult;
    }
    if(!isInitStatOk)
    {
        int iresult = 1;
        strMsg = QString("%1 静置柜已不可取!").arg(m_moduleName);
        uchar uc = 1;
        BaseCalcFun::Analysis_L("LQ15.14",uc);
        return iresult;
    }
    while (true)
    {
        if(isTerminate)
        {
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(imodel);
            return iresult;
        }

        if(CheckTPV())
            break;
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
    iresult = 1;
    strMsg = QString("%1 %2 %3 success!").arg(m_moduleName).arg(imodel).arg(ConfigData::GetInstance()->m_tpPos);
    return iresult;
}
void DefineVariable::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *DefineVariable::Copy()
{
    return (new DefineVariable());
}

bool DefineVariable::CheckTPV()
{
    bool isok = false;
    QueryDb();
    if(imodel == 1) //存
    {
        for(auto item = m_dbBurninMap.begin();item != m_dbBurninMap.end();++item)
        {
            if(item.value() == 0x01)
            {
                QStringList list = item.key().split("x");
                ConfigData::GetInstance()->m_tpPos = QString("(%1;%2;1)").arg(list[0].toInt()).arg(list[1].toInt());
                isok = true;
                break;
            }
        }
    }
    if(imodel == 2) //取
    {
        for(auto item = m_dbBurninMap.begin();item != m_dbBurninMap.end();++item)
        {
            if(item.value() == 0x08 )
            {
                QStringList list = item.key().split("x");
                ConfigData::GetInstance()->m_tpPos = QString("(%1;%2;1)").arg(list[0].toInt()).arg(list[1].toInt());
                isok = true;
                return isok;
            }
        }

        for(auto item = m_dbBurninMap.begin();item != m_dbBurninMap.end();++item)
        {
            if(item.value() == 0x20)
            {
                QStringList list = item.key().split("x");
                ConfigData::GetInstance()->m_tpPos = QString("(%1;%2;1)").arg(list[0].toInt()).arg(list[1].toInt());
                isok = true;
                return isok;
            }
        }

    }
    return isok;
}

void DefineVariable::QueryDb()
{
    QSqlQuery query;
    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT ID,Timesec,status FROM BurnIn");

    while(query.next())
    {
        QString strkey = query.value("ID").toString();
        uchar status = query.value("status").toInt();
        if(!m_dbBurninMap.contains(strkey))
            m_dbBurninMap.insert(strkey,status);
        else
            m_dbBurninMap[strkey] = status;
    }
}
