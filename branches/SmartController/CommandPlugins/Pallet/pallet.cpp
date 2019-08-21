#include "pallet.h"
#include <sys/time.h>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>

Pallet::Pallet()
{
    m_moduleName = "G03";

    pal = Pallets::GetInstance();
    m_motion = new MotionControl;
}

void Pallet::Initialization()
{
    //Initialization variable

    isTerminate = false;
    m_motion->Init();
}
void Pallet::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //Save struct
    pal->current_pallet_id = ps.palletCmdParam.param1;
    pal->AnalyzeInstruct(ps.palletCmdParam.param1, ps.palletCmdParam.param2);
    m_motion->axisMap = (*(pal->palletsInfo))[ps.palletCmdParam.param1]._ps.axisMap;
}

void Pallet::RunModule()
{
    //call command for IO

    m_motion->Start();
}

int  Pallet::GetExcResult(QString &strMsg)
{
    //todo strMsg is log

    int iresult = -1;
    //轴初始化失败，直接退出
    if(!m_motion->m_isInitStatOk)
    {
        strMsg = QString("%1 %2 Init False! errorCode %3").arg(m_moduleName).arg(_ps.id).arg(m_motion->ierror);
        return iresult;
    }

    //定义轴的标志
    bool isok[_ps.axisNum];
    for(int i = 0; i < _ps.axisNum;i++)
        isok[i] = false;

    //设置时间进入超时循环
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (true)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            m_motion->StopAxis();
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        else
        {
            //to check IO
            bool isToalOk = true;
            int inum = 0;
            for(auto it = (*(_ps.axisMap)).begin();it != (*(_ps.axisMap)).end();it++)
            {
                if(inum == _ps.axisNum)
                    break;

                //解析输入数据获取当前位置，到位信号，故障信号
                int tempResult = 0;
                int curPos = 0;
                int ireturn = m_motion->GetInputData(it.key(),tempResult,curPos);

                QString strTempMsg = "";
                if(!m_motion->JudgeSWLimit(it.key(),curPos,strTempMsg))
                {
                    iresult = -3;
                    m_motion->StopAxis();
                    strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
                    return iresult;
                }

                if(ireturn == 0) //无故障信号
                {
                    if(qAbs(((*_ps.axisMap)[it.key()].pos + (*_ps.axisMap)[it.key()].offsetPos)*1000 - curPos) >m_motion->m_deviation[it.key()])
                    {
                        isok[inum] = false;
                    }
                    else
                        isok[inum] = true;
                }
                else //故障信号
                {
                    iresult = -2;
                    m_motion->StopAxis();
                    strMsg = QString("%1 %2 axis %3 return errorcode %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(tempResult);
                    return iresult;
                }

                //判断硬极限是否触发
                strTempMsg = "";
                if(!m_motion->JudgeHWLimit(it.key(),strTempMsg))
                {
                    iresult = -4;
                    m_motion->StopAxis();
                    strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
                    return iresult;
                }
                if(!isok[inum++])
                    isToalOk = false;
            }
            //所有的轴ok
            if(isToalOk)
            {
                iresult = 1;
                strMsg = QString("%1 axis run ok").arg(_ps.id);
                break;
            }
            gettimeofday(&tpEnd,NULL);
            timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            if(timeUse >= _ps.timeOut)
                break;
        }
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    if(iresult != 1)
    {
        iresult = -11;
        m_motion->StopAxis();
        strMsg = QString("%1 axis run TimeOut").arg(_ps.id);
        return iresult;
    }
    if(_ps.delayMsec > 0)
        QThread::msleep(_ps.delayMsec);
    pal->saveCurrentPosition((*(pal->palletsInfo))[pal->current_pallet_id]);
    return iresult;
}

void Pallet::StopModule()
{
    isTerminate = false;
    m_motion->m_isInitStatOk = true;
    //if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(m_moduleName))
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(_ps.id))
        {
            for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.id].axisMap.begin();\
                it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.id].axisMap.end();\
                ++it)
            {
                it.value().offsetPos = 0.0;
            }
        }
    }
    BaseCalcFun::MemCopyTempData(m_motion->tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = m_motion->m_CWPosVec.begin();it != m_motion->m_CWPosVec.end();++it)
    {
        BaseAxisOperate::ReAxisRunBit(4,*it,m_motion->tempOutputData);
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(m_motion->tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

FunModuleInterface *Pallet::Copy()
{
    return (new Pallet());
}
