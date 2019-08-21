#include "rectcoointer.h"
#include <sys/time.h>
#include <MyGlobal.h>
#include <QString>
#include <QVector>
#include <QApplication>
#include <QDebug>
#include <time.h>
#include <stdio.h>
#include <QtAlgorithms>
#include <DycLibBase/Coordinate/coordinate.h>
using namespace CoreGlobal;
RectCooInter::RectCooInter()
{
    m_moduleName = "G01";
    MyLCSPThread= new QThread();
    m_LCSPlib= new LCSPLib();
    m_swtoJ = new Coordinate();
    this->moveToThread(MyLCSPThread);
    //connect(m_LCSPlib,SIGNAL(ExcFinish(int)),this,SLOT(Runiresult(int)));//运行过程确认(待确认）
    connect(MyLCSPThread,SIGNAL(started()),this,SLOT(ThreadRun()));//建立信号与槽机制用于ＩＯ触发
    foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
        m_tempINSON.append(pair);
    }
}
void RectCooInter::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    m_runiresult = 0;
}
void RectCooInter::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_LCSP_Stru;
    m_LCSPlib->InitCspAxisParam(_ps);
    m_isInitOk=true;
    //检查参数配置是否有误的问题
    if(_ps.LCSPCPos.size()<1)
        m_isInitOk=false;
    /*软件配置xml判断*/
    for(int i=0;i<_ps.LCSPCPos.size()-1;++i)
    {
        if(_ps.LCSPCPos[i].LCSCPosMap.size()!=_ps.LCSPCPos[i+1].LCSCPosMap.size())
        {
            m_isInitOk = false;
            break;
        }
        int num=_ps.LCSPCPos[i].LCSCPosMap.size();
        if(num > 3|| num < 1)//不能超过3个坐标系
        {
            m_isInitOk = false;
            break;
        }
        for(auto it=_ps.LCSPCPos[i].LCSCPosMap.begin();it!=_ps.LCSPCPos[i].LCSCPosMap.end();++it)
        {
            if(!_ps.LCSPCPos[i+1].LCSCPosMap.contains(it.key()))
            {
                m_isInitOk = false;
                break;
            }
        }
    }
    m_ps = ps.swXml_LCSP_Stru;//test**************调用库
    //把偏移位置写入位置最后一个坐标map
    auto item=m_ps.LCSPOffsetPosMap.begin();
    if(m_ps.LCSPOffsetPosMap.size()>3)
        return;
    if(m_ps.LCSPCPos[m_ps.LCSPCPos.size()-1].LCSCPosMap.size()>3)
        return;
    for(auto it=m_ps.LCSPCPos[m_ps.LCSPCPos.size()-1].LCSCPosMap.begin();it!=m_ps.LCSPCPos[m_ps.LCSPCPos.size()-1].LCSCPosMap.end();++it)
    {
      it.value().pos = it.value().pos + item.value();
      ++item;
    }
    m_swtoJ->CovertTo_J(m_ps);
    m_arrary.clear();
    m_ps.speed=ConfigData::GetInstance()->speedRate*0.01*m_ps.speed;//速度改变
    m_ps.acc=ConfigData::GetInstance()->speedRate*0.01*m_ps.acc;//加速度改变
    if(m_ps.speed < 1)
    {
        m_ps.speed = 1;
    }
    if(m_ps.acc < 1)
    {
        m_ps.acc = 1;
    }
    m_arrary = m_LCSPlib->GetLCSPArray(m_ps);//获取数组不是空
    if(m_arrary.isEmpty())
    {
        m_isInitOk = false;
    }
}
///
/// \brief RectCooInter::RunModule
///
void RectCooInter::RunModule()
{
    //todo call command for IO
    isTerminate = false;
}
///
/// \brief RectCooInter::GetExcResult
/// \param strMsg输出先关的运行信息的结果
/// \return
///
int  RectCooInter::GetExcResult(QString &strMsg)
{
    int iresult = -1;  //轴初始化失败，直接退出
    if(!m_isInitOk)
    {
        iresult = -2;
        strMsg = QString("%1 %2 Init False!").arg(m_moduleName).arg(_ps.id);
        return iresult;
    }
    m_LCSPlib->SONAxis();

    if(!MyLCSPThread->isRunning())
    {
        m_runiresult = 0;
        MyLCSPThread->start();//启动运行线程启动
    }
    //设置时间进入超时循环
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < _ps.timeOut)
    {
        if(isTerminate)//主循环发出退出标志
        {
            m_LCSPlib->StopLCSP(_ps.id);
            MyLCSPThread->quit();
            MyLCSPThread->wait();
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        if(m_runiresult < 0)//加速度速度设置错误
        {
            m_LCSPlib->StopLCSP(_ps.id);
            MyLCSPThread->quit();
            MyLCSPThread->wait();
            iresult = -3;
            strMsg = QString("%1 %2故障运行代码：%3加速度速度设置异常!").arg(m_moduleName).arg(_ps.id).arg(m_runiresult);
            return iresult;
        }
        if(m_runiresult==1)//数组发完
        {
            iresult=2;
            break;
        }
        //错误检查伺服Error和硬极限判断
        for(auto it=m_arrary.begin()->begin();it!=m_arrary.begin()->end();++it)
        {
            int Inbytepos = GetStatuswordPos(it.key());
            if(BaseAxisOperate::CheckAxisFault(3,2,Inbytepos))
            {
                m_LCSPlib->StopLCSP(_ps.id);
                MyLCSPThread->quit();
                MyLCSPThread->wait();
                iresult = -4;
                strMsg =QString("%1 axis %2 have error Errorcode:%3!").arg(m_moduleName).arg(it.key()).arg(GetErrorcode(it.key()));
                return iresult;        //获取故障代码
            }
            QString str="";
            if(!JudgeHWLimit(it.key(),str))//硬极限判断
            {
                m_LCSPlib->StopLCSP(_ps.id);
                MyLCSPThread->quit();
                MyLCSPThread->wait();
                iresult = -5;
                strMsg =QString("%1 axis %2 %3!").arg(m_moduleName).arg(it.key()).arg(str);
                return iresult;
            }
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= _ps.timeOut)
            break;
        QThread::msleep(10);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    if(iresult < 0)
    {
        iresult = -11;
        strMsg = QString("%1 axis run TimeOut").arg(m_moduleName);
        return iresult;
    }
    if(_ps.delayMsec > 0)
        QThread::msleep(_ps.delayMsec);
    //此结果插补命令运行成功
    iresult = 1;
    strMsg = QString("%1 axis run ok").arg(_ps.id);
    return iresult;
}
///
/// \brief RectCooInter::StopModule
///停止中断函数
void RectCooInter::StopModule()
{
    isTerminate = false;
    m_LCSPlib->StopLCSP(_ps.id);//停止在运行的轴
    MyLCSPThread->quit();//线程停止
    MyLCSPThread->wait();
    //offset置0

    if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(_ps.id))
    {
        for(auto it=ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[_ps.id].LCSPOffsetPosMap.begin();\
            it!=ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[_ps.id].LCSPOffsetPosMap.end();++it)
        {
            it.value()=0.0;
        }
    }

}

FunModuleInterface *RectCooInter::Copy()
{
    return (new RectCooInter());
}

void RectCooInter::stack_prefault()
{
    unsigned char dummy[MAX_SAFE_STACK];

    memset(dummy, 0, MAX_SAFE_STACK);
    return;
}
///
/// \brief RectCooInter::ThreadRun
///运行函数
void RectCooInter::ThreadRun()
{
    m_runiresult = m_LCSPlib->RunLCSPAxis(m_ps);//启动运行
}
///
/// \brief RectCooInter::IOTRiggerSlot
/// \param bitName
/// \param stat
/// \return
///信号触发操作
void RectCooInter::IOTRiggerSlot(QString bitName,int stat)
{
    QMutexLocker locker(&m_mutex);
    uchar ivalue = 0;
    ivalue = stat;
    if(BaseCalcFun::AnalysisIOString(bitName,ivalue) == 0)//设置out值是否设置成功
        QThread::msleep(15);//延时１５ｍｓ
    if(ivalue != 0)
        ivalue = 0;
    else
        ivalue = 1;
    BaseCalcFun::AnalysisIOString(bitName,ivalue);
}

void RectCooInter::Runiresult(const int ir)
{
    m_runiresult = ir;
}

void RectCooInter::MyPriority()
{
    /*设置优先级Declare ourself as a real time task */
    sched_param param;//调度结构
    param.sched_priority = 0;
    if(sched_setscheduler(0,SCHED_FIFO, &param) == -1)
    {
        exit(-1);
    }
    /* Lock memory:锁定内存MCL_CURRENT:表示对所有已经映射到进程地址空间的页上锁 MCL_FUTURE:表对所有将来映射到进程地空间的页都示上锁*/
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
        exit(-2);
    }
    /* Pre-fault our stack */
    stack_prefault();
}

int RectCooInter::GetStatuswordPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        if(it->key == "Statusworld")
        {
            break;
        }
        ibytePos += ibyteNum;
    }
    return ibytePos;
}

int RectCooInter::GetErrorcode(QString strKey)
{
    int ireturn=0;
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;
        if(it->key == "Errorcode")
        {
            ireturn = BaseAxisOperate::GetAxisErrorCode(strType,ibyteNum,ibytePos);
            break;
        }
        ibytePos += ibyteNum;
    }
    return ireturn;
}

void RectCooInter::GetLCSPCurpos(QString strKey, int &curPos)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;
        if(it->key == "Curpos")
        {
            //数据结构赋值存储
            curPos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);
            return;
        }
        ibytePos += ibyteNum;
    }
}
///
/// \brief RectCooInter::JudgeHWLimit
/// \param strKey　轴的ｉｄ
/// \param strMsg　输出细节的信息
/// \return
///判断在轴跑的过程中出发硬极限
bool RectCooInter::JudgeHWLimit(QString strKey, QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
    if(!str.isEmpty())
    {
        int itype = BaseCalcFun::AnalysisIOString(str,uc);
        if(itype != 1)
        {
            strMsg = "硬极限起点设置错误  ";
            return false;
        }
        if(uc != 0)
        {
            strMsg = "超出硬极限起点  ";
            QString str;
            for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
            {
                str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
            }
            strMsg += str;
            return false;
        }
    }
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
    if(!str.isEmpty())
    {
        int itype = BaseCalcFun::AnalysisIOString(str,uc);
        if(itype != 1)
        {
            strMsg = "硬极限终点设置错误  ";
            return false;
        }
        if( uc != 0)
        {
            strMsg = "超出硬极限终点  ";
            QString str;
            for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
            {
                str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
            }
            strMsg += str;
            return false;
        }
    }
    return true;
}
///
/// \brief RectCooInter::JudgeSWLimit
/// \param strKey　轴的ｉｄ
/// \param CurPos　当前位置
/// \param strMsg　输出对应的信息
/// \return
///判断按照给定的位置运行结束后判断有无到位或者超出软极限
bool RectCooInter::JudgeSWLimit(QString strKey, int CurPos, QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(qAbs((_ps.LCSPCPos[m_arrary.size()-1].LCSCPosMap[strKey].pos*1000- CurPos) >200.0))
    {
        strMsg = QString("%1 与指定位置偏差大于200.0").arg(CurPos);
        return false;
    }
    return true;
}

SWCONFIGXML_LCSP_STRU RectCooInter::Getcsysaxisid(SWCONFIGXML_LCSP_STRU _ps)
{
    QString strid;
    strid="";
    for(int i=0;i<_ps.LCSPCPos.size();++i)//先作处理
    {
        if(_ps.LCSPCPos[i].LCSCPosMap.size()==1)
        {
            if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.contains(_ps.LCSPCPos[i].LCSCPosMap.begin().key()))
            {
                if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[_ps.LCSPCPos[i].LCSCPosMap.begin().key()].hwArmInfoMap.size()==1)
                {
                    strid=ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[_ps.LCSPCPos[i].LCSCPosMap.begin().key()].hwArmInfoMap.begin().key();
                    break;
                }
            }
        }
    }
    if(!strid.isEmpty())
    {
        auto items = _ps.LCSPCPos.begin();
        auto item = items->LCSCPosMap;
        QString str = item.begin().key();
        if(str.compare(strid) == 0)
        {
            return _ps;
        }
        items->LCSCPosMap.insert(strid,item.begin().value());
        items->LCSCPosMap.remove(str);
        for(auto it = items->LCSPTrigInfo.begin();it != items->LCSPTrigInfo.end();++it)
        {
            it->LCSPTrigCPos.begin()->id = strid;
        }
    }
    return _ps;
}

