#include "torque.h"
#include <QApplication>
#include <QMessageBox>
#include <MyGlobal.h>
#include <QDateTime>
#include <QStringList>
using namespace CoreGlobal;
Torque::Torque()
{
     m_moduleName = "G101";
}

void Torque::Initialization()
{
    isTerminate = false;
    m_isInitStatOk = true;
    //TorqueChart::GetInstance()->ClearChart();
    temppath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹" + "/Data/Torque/";
    QDir dir;
    if(!dir.exists(temppath))
    {
        dir.mkdir(temppath);
    }
}

void Torque::SetModuleParam(SWCONFIGXML_STRU ps)
{
    // TorqueChart::GetInstance()->ClearChart();
     _ps = ps.swXml_TQ_Stru;
//     bitstate = _ps.note;
//     QStringList list = QString.split(".");
//     QString IObyte = list.at(0);
//     IObyte = IObyte.mid(1);
//     int Byte = IObyte.toInt();
//     QString bit = list.at(1);
//     int bitvalue = bit.toInt();
}

void Torque::RunModule()
{
    //todo init ，send speed,pos
    int i = 0;
    for(auto it = _ps.torqueMap.begin();it != _ps.torqueMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        //dosomething
        if(!SetAxisInit(it.key()))
        {
            m_isInitStatOk = false;
            return;
        }
        SetAxisParm(it.key(),it.value());//轴 位置速度集合!!
    }
    //run
    i = 0;
    for(auto it = _ps.torqueMap.begin();it != _ps.torqueMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        //dosomething
        RunAxis(it.key());
    }
}
int Torque::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(false ==(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains("TqValue")&&
                DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains("TqStart")))
    {
        strMsg = QString("请检查 寄存器【TqValue】或开关量【TqStart】设置！");
        return iresult;
    }
    ResetStartState(1);
    QString data = "时间   -   扭矩\n";
    QStringList datalist;
    datalist.append(data);
    //file name
    QDateTime datatime = QDateTime::currentDateTime();
    QString filename =QString("%1__%2.txt").arg(datatime.date().toString("dd_MM_yy")).arg(datatime.time().toString("hh_mm_ss"));
    filename = temppath + filename;
    //todo strMsg is log

    //轴初始化失败，直接退出
    if(!m_isInitStatOk)
    {
        strMsg = QString("%1 %2 Init False!").arg(m_moduleName).arg(_ps.id);
        return iresult;
    }
    //uchar value = 0;
    //设置时间进入超时循环
    QElapsedTimer m_xvalue;
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    m_xvalue.start();//KAI SHI JI SHI
    m_axis = _ps.torqueMap.begin().key();
    if(!_ps.note.contains("free"))
    {
        while (true)    //(timeUse < _ps.timeOut)
        {
            if(isTerminate)//主循环发出退出标志
            {
                iresult = -99;
                StopAxis(m_axis);
                strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
                ResetStartState(0);
                return iresult;
            }
            //dosomething
            //解析输入数据获取当前扭矩，故障信号
            int tempResult = 0;
            int curTq = 0;
            int ireturn = GetInputData(m_axis,tempResult,curTq);
            qreal m_x = m_xvalue.elapsed()/1000.0;  //两个参数都必须是double类型的　important!
            qreal m_y = qAbs(curTq /1000.0 * 0.98 * 0.64);  //50W的电机 单位是kgfcm

            DBInstanceManager::getDBInstance()->m_mutex.lock();
            DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["TqValue"].Initvalue = m_y;
            DBInstanceManager::getDBInstance()->m_mutex.unlock();
            //TorqueChart::GetInstance()->AddChartPoint(m_x,m_y);
            data = QString("%1   %2\n").arg(m_x).arg(m_y);
            datalist.append(data);
            if(ireturn == -1)//故障信号
            {
                iresult = -2;
                StopAxis(m_axis);
                strMsg = QString("%1 %2 axis %3 return errorcode %4").arg(m_moduleName).arg(_ps.id).arg(m_axis).arg(tempResult);
                ResetStartState(0);
                return iresult;
            }
            // BaseCalcFun::Analysis_L(_ps.note,value);
            if(ireturn == 1 && m_x >= 0.5)//到位信号
            {
                //当前轴扭到达标志
                iresult = 1;
                strMsg = QString("%1 axis 扭力到达!").arg(_ps.id);
                break;
            }
            gettimeofday(&tpEnd,NULL);
            timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            if(_ps.timeOut != -1)
            {
                if(timeUse >= _ps.timeOut)
                {
                    ireturn = -3;
                    break;
                }
            }
            QThread::msleep(5);
            QApplication::processEvents(QEventLoop::AllEvents,100);
        }
        if(iresult != 1)
        {
            iresult = -11;
            StopAxis(m_axis);
            strMsg = QString("%1 axis run TimeOut").arg(_ps.id);
            ResetStartState(0);
            return iresult;
        }
    }
    else
    {
        iresult = 1;
        strMsg = QString(tr("Calibration start !"));
    }
    //BaseCalcFun::mDelay(_ps.delayMsec);//danbuyanshibunengyongzhegehanshule
    QElapsedTimer timedly;
    timedly.start();
    while(timedly.elapsed() <= _ps.delayMsec)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            StopAxis(m_axis);
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            ResetStartState(0);
            return iresult;
        }
        int tempResult = 0;
        int curTq = 0;
        GetInputData(m_axis,tempResult,curTq);
        qreal m_x = m_xvalue.elapsed()/1000.0;// yong de bu shi dangqian jishi de shijian
        qreal m_y = qAbs(curTq /1000.0 * 0.98 * 0.64);// 两个参数都必须是double类型的　important!

        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["TqValue"].Initvalue = m_y;
        DBInstanceManager::getDBInstance()->m_mutex.unlock();
        //TorqueChart::GetInstance()->AddChartPoint(m_x,m_y);
        QThread::msleep(10);
        QApplication::processEvents(QEventLoop::AllEvents,100);
        data = QString("%1   %2\n").arg(m_x).arg(m_y);
        datalist.append(data);
    }
    if(!_ps.note.contains("free"))
    {
        SavePoint(filename,datalist);
    }
    StopAxis(m_axis);
    ResetStartState(0);
    return iresult;
}

void Torque::StopModule()
{
    isTerminate = false;
    m_isInitStatOk = true;
}

FunModuleInterface *Torque::Copy()
{
    Torque *t = new Torque();
    return t;
}
///
/// \brief RapidPosition::GetInputData
/// \param strKey 轴号
/// \param ireturn 获取errorcode的值
/// \param curPos 获取当前位置
/// \return 返回当前的状态 0未到位且无错 1到位无错 -1有错
///
int Torque::GetInputData(QString strKey,int &ireturn,int &curTq)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int iResult = 0;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey)) //Y1
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;//字节开始位置
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;  //几个字节
        QString strType = it->type;
        if(it->key == "Statusworld")
        {
            if(BaseAxisOperate::CheckAxisFault(3,ibyteNum,ibytePos))//?位?字节?开始
            {
                iResult = -1;
                break;
            }
            if(BaseAxisOperate::CheckAxisINP(10,ibyteNum,ibytePos))
                iResult = 1;
        }
        if(it->key == "Errorcode")
        {
            ireturn = BaseAxisOperate::GetAxisErrorCode(strType,ibyteNum,ibytePos);
        }
        if(it->key == "Curtq")
        {
            curTq = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);//获取实时扭矩
            if(qAbs(curTq  - (_ps.torqueMap[strKey].torque_value* 10)) <= 2)//扭矩到达
            {
                  iResult = 1;
                  break;
            }

        }

        ibytePos += ibyteNum;
    }
    return iResult;
}

int GetPositionActual(QString strKey, int &ireturn, int &Cutpos)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int iResult = 0;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey)) //Y1
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;//字节开始位置
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;  //几个字节
        QString strType = it->type;
        if(it->key == "Statusworld")
        {
            if(BaseAxisOperate::CheckAxisFault(3,ibyteNum,ibytePos))//?位?字节?开始
            {
                iResult = -1;
                break;
            }
            if(BaseAxisOperate::CheckAxisINP(10,ibyteNum,ibytePos))
                iResult = 1;
        }
        if(it->key == "Errorcode")
        {
            ireturn = BaseAxisOperate::GetAxisErrorCode(strType,ibyteNum,ibytePos);
        }
        if(it->key == "Curpos")
        {
            Cutpos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);//获取实时扭矩
        }

        ibytePos += ibyteNum;
    }
    return iResult;
}
///
/// \brief RapidPosition::JudgeHWLimit
/// \param strKey 轴号
/// \param strMsg   信息
/// \return
/// 校验轴是否超出硬极限
bool Torque::JudgeHWLimit(QString strKey,QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
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
     str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
    itype = BaseCalcFun::AnalysisIOString(str,uc);

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

    return true;
}
///
/// \brief RapidPosition::JudgeSWLimit
/// \param strKey 轴号
/// \param CurPos 当前位置
/// \param strMsg 信息
/// \return
/// 校验当前位置 不超出软极限，与设置的位置偏差小于正负20
bool Torque::JudgeSWLimit(QString strKey,int CurPos,QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitStart;
    if(CurPos < str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限起点").arg(CurPos);
        return false;
    }
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitEnd;
    if(CurPos > str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限终点 ").arg(CurPos);
        return false;
    }
    if(qAbs(_ps.torqueMap[strKey].torque_value*1000 - CurPos) >20.0)
    {
        strMsg = QString("%1 与指定位置偏差大于20.0 ").arg(CurPos);
        return false;
    }
    return true;
}
///
/// \brief RapidPosition::GetContrlWorldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴控制字的位置
int  Torque::GetContrlWorldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;//获取运行的类型
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))//然后在类型里面找到控制子的位置
        return false;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        if(item->key == "Contrlworld")
        {
            break;
        }
        int ibyteNum = item->byteNum;
        ibytePos += ibyteNum;
    }

    return ibytePos;
}
///
/// \brief RapidPosition::GetStatusworldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴状态字的位置
int  Torque::GetStatusworldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        if(it->key == "Statusworld")
        {
            break;
        }
        int ibyteNum = it->byteNum;
        ibytePos += ibyteNum;
    }
    return ibytePos;
}
///
/// \brief RapidPosition::SetAxisInit
/// \param strKey 轴的ID号与硬件配置对应
/// \return 初始化成功true，否则false
///设置轴的初始化 流程如下注释
bool Torque::SetAxisInit(QString strKey)
{
    //根据轴ID获取该轴控制字的位置
    int ibytePos = GetContrlWorldPos(strKey);
    //根据轴ID获取该轴状态字的位置
    int iInbytePos = GetStatusworldPos(strKey);
    //REset
    BaseAxisOperate::SetAxisReset(7,ibytePos);
    //获取检查励磁是否成功的参数（bit，vaule）
    QVector<QPair<int,int>> tempInOP;
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        tempInOP.append(pair);
    }
    //检查轴是否已经励磁成功
    if(BaseAxisOperate::CheckAxisExcited(tempInOP,2,iInbytePos))
        return true;

    //获取励磁的参数（bit，vaule）
    QVector<QPair<int,int>> tempOutOP;
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        tempOutOP.append(pair);
    }
    //设置励磁
    BaseAxisOperate::SetAxisExcite(tempOutOP,ibytePos);

    //检查是否有故障
    if(BaseAxisOperate::CheckAxisFault(3,2,iInbytePos))
        return false;
    //检查轴是否励磁成功
    if(BaseAxisOperate::CheckAxisExcited(tempInOP,2,iInbytePos))
        return true;
    return true;
}
///
/// \brief RapidPosition::SetAxisParm
/// \param strKey 轴的ID
/// \param axisInfoStru 轴信息的参数
/// 设置模式，目标速度，目标位置，加减速
void Torque::SetAxisParm(QString strKey,TORQUE_INFO_STRU axisInfoStru)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    //判断该轴ID是否在硬件配置中
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return ;
    //获取该轴在驱动器中所在字节的位置
    int iBytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    //获取模式类型，PP模式还是其他模式
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
    //判断该模式是否在已经存在
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    //循环该模式下的OUTPUT的结构体 找到模式，目标速度，目标位置，加速减的字节位置，大小，类型，并赋值
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        int byteNum = item->byteNum;
        QString strType = item->type;
        if(item->key == "Model")
        {
            BaseAxisOperate::ChangeAxisModel(strType,byteNum,iBytePos,4);//改变位扭矩控制
        }
        else if(item->key == "Tratq")//1111
        {
            BaseAxisOperate::SetAxisPos(strType,byteNum,iBytePos,axisInfoStru.torque_value*10);//1111百分比
        }
        else if(item->key == "Traspeed")
        {
            BaseAxisOperate::SetAxisSpeed(strType,byteNum,iBytePos,axisInfoStru.speed *
//                                          ConfigData::GetInstance()->speedRate /100.0 *
//                                          ConfigData::GetInstance()->iSpeed *
                                          ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].lead);
        }
//        else if(item->key == "Accel")
//        {
//            BaseAxisOperate::SetAxisACC(strType,byteNum,iBytePos,_ps.acc);
//        }
//        else if(item->key == "Decel")
//        {
//            BaseAxisOperate::SetAxisDCC(strType,byteNum,iBytePos,_ps.dec);
//        }

        else if(item->key == "Accel")
        {
            BaseAxisOperate::SetAxisACC(strType,byteNum,iBytePos,_ps.acc *axisInfoStru.speed/100.0*\
                                        ConfigData::GetInstance()->speedRate /100 *\
                                        ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled);
        }
        else if(item->key == "Decel")
        {
            BaseAxisOperate::SetAxisDCC(strType,byteNum,iBytePos,_ps.dec *axisInfoStru.speed/100.0*\
                                        ConfigData::GetInstance()->speedRate /100 *\
                                        ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled);
        }
        iBytePos += byteNum;
    }
}
///
/// \brief RapidPosition::RunAxis
/// \param strKey 轴号
/// 轴跑起来
void Torque::RunAxis(QString strKey)
{
    int ibytePos = GetContrlWorldPos(strKey);
    BaseCalcFun::SetBitValue(2,ibytePos,1);
    QThread::msleep(20);
    BaseAxisOperate::SetAxisRun(4,ibytePos);
}

void Torque::SavePoint(QString filename, QStringList data)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream filestream(&file);
        for(int i = 0; i < data.size();++i)
            filestream << data[i];
    }
    file.close();
}

void Torque::ResetStartState(int val)
{

    DBInstanceManager::getDBInstance()->m_mutex.lock();
    DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["TqStart"].initvalue = val;//开始
    DBInstanceManager::getDBInstance()->m_mutex.unlock();

}
///
/// \brief RapidPosition::StopAxis
/// \param strKey 轴号
/// 轴停止
void Torque::StopAxis(QString strKey)
{
    int ibytePos = GetContrlWorldPos(strKey);
    BaseCalcFun::SetBitValue(2,ibytePos,0);
    QThread::msleep(20);
}


