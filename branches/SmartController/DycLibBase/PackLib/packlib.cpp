#include "packlib.h"//zhangping
#include <QThread>
#include <QApplication>
#include <sys/time.h>
#include <QDebug>
#define ORGSTARTLIMIT -99999999
#define ORGENDLIMIT 99999999
PackLib::PackLib()
{
    iAcc = ConfigData::GetInstance()->iAccel ;
    iDcc = ConfigData::GetInstance()->iDccel ;
}

void PackLib::SetParam(SWCONFIGXML_ORG_AXIS_STRU ps)
{
    _ps = ps;
    QString strKey = _ps.axisId;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        return ;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        if(it->key == "Statusworld")
        {
            m_StatusworldPos = ibytePos;
            m_StatusworldNum = ibyteNum;

        }
        else if(it->key == "Errorcode")
        {
            m_ErrorNum = ibyteNum;
            m_ErrorPos = ibytePos;
        }
        else if(it->key == "Model")
        {
            m_INputModelPos = ibytePos;
        }
        else if(it->key == "Curpos")
        {
            m_CurPos = ibytePos;
            m_CurNum = ibyteNum;
        }
        ibytePos += ibyteNum;
    }

    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return ;
    int iBytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    QString ModelType1 = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
    swLimitStart = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitStart.toDouble()*1000;
    swLimitEnd   = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitEnd.toDouble()*1000;
    hwLimitStart = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
    hwLimitEnd  = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType1))
        return ;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType1]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType1]["OUTPUT"].byteInfoVec.end();++item)
    {
        int byteNum = item->byteNum;
        if(item->key == "Contrlworld")
        {
            m_ContrlworldPos = iBytePos;
            m_ContrlworldNum = byteNum;
        }
        if(item->key == "Model")
        {
            m_ModelPos = iBytePos;
            m_ModelNum = byteNum;
        }
        else if(item->key == "Trapos")
        {
            m_TraposPos = iBytePos;
            m_TraposNum = byteNum;
        }
        else if(item->key == "Traspeed")
        {
            m_TraspeedPos = iBytePos;
            m_TraspeedNum = byteNum;
        }
        else if(item->key == "Accel")
        {
            m_AccelPos = iBytePos;
            m_AccelNum = byteNum;
        }
        else if(item->key == "Decel")
        {
            m_DecelPos = iBytePos;
            m_DecelNum = byteNum;
        }

        iBytePos += byteNum;
    }
    isTerminate = false;
}
/// \brief PackLib::RunAxis
/// \return Errorcode
/// 0 --->正确
/// -1 --->初始校验有故障
/// -2  --->设置正向参数校验当前位置超出范围
/// -3 --->励磁设置错误
/// -4 --->正向运行有故障
/// -5 --->负向运行超时
/// -6 --->负向运行有故障
/// -7 --->靠近原点正向低速运行超时
/// -8 --->靠近原点正向低速运行有故障
/// -9 --->模式6下校验当前位置错误或者状态位bit12不是1
/// -10---> 回原点模式2中感应到遮光片到走出遮光片结束检测失败
///
int PackLib::RunAxis()
{
    int iResult = 0;
    BaseAxisOperate::SetAxisReset(7,m_ContrlworldPos);
    QThread::msleep(30);
    //校验故障
    if(BaseAxisOperate::CheckAxisFault(3,m_StatusworldNum,m_StatusworldPos) == 1)
    {
        iResult = -1;
        return iResult;
    }
    if(!SetAxisOverParam())
    {
        iResult = -2;
        return iResult;
    }
    if(!SetAxisOp())
    {
        iResult = -3;
        return iResult;
    }
    int ir = 0;
    if(_ps.directTime >= 10)
    {
        SetAxisStart();
        ir = GetResult(_ps.directTime,false,1);
        if(ir < -1)
        {
            if(ir == -2)
                iResult = -4;
            return iResult;
        }
    }
    SetAxisBackParam();
    ir = GetResult(_ps.orgTimeout,true,1);
    if(ir != 0)
    {
        if(ir == -1)
        {
            iResult = -5;
            BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
        }
        else if(ir == -2)
            iResult = -6;
        return iResult;
    }
    if(!ChangeAxisParam())
    {
        iResult = -10;
        return iResult;
    }
    //    if(_ps.orgmode == "2")
    //    {
    //         SetAxisStart();
    //         QThread::msleep(50);
    //        int ir = GetResult(20000,true,0);
    //        if(ir != 0)
    //        {
    //            return false;
    //        }
    //    }
    if(BaseAxisOperate::CheckAxisFault(3,m_StatusworldNum,m_StatusworldPos) == 1)
    {
        iResult = -10;
        return iResult;
    }
    if(_ps.orgmode == "1")
    {
        ir = ApproachORG();
        if(ir != 0)
        {
            if(ir == -1)
            {
                iResult = -7;
                BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
            }
            else if(ir == -2)
                iResult = -8;
            return iResult;
        }
    }
    QThread::msleep(200);
    if(BaseAxisOperate::CheckAxisFault(3,m_StatusworldNum,m_StatusworldPos) == 1)
    {
        iResult = -11;
        return iResult;
    }
    ChangeAsixModel();
    QThread::msleep(150);//20
    if(BaseAxisOperate::CheckAxisFault(3,m_StatusworldNum,m_StatusworldPos) == 1)
    {
        iResult = -12;
        return iResult;
    }
    if(!CheckAxisInfo())
    {
        iResult = -9;
        return iResult;
    }
    BaseAxisOperate::ChangeAxisModel("I8",m_ModelNum,m_ModelPos,1);
    return iResult;
}
bool PackLib::CheckAxisInfo()
{
    bool isok = true;
    //获取当前位置
    int curpos = BaseAxisOperate::GetAxisCurPos("I32",m_CurNum,m_CurPos);
 //   if(curpos > swLimitEnd || curpos < swLimitStart)
      //  return false;
    if(curpos > 200 || curpos < -200)//5
        isok = false;

    return isok;
}

void PackLib::SetAxisStart()//6040 bit4 1 to 0
{
    BaseCalcFun::SetBitValue(4,m_ContrlworldPos,1);
    QThread::msleep(20);
    for(int i = 0;i < 100;++i)
    {
        if(1 == BaseCalcFun::GetBitValue(12,m_StatusworldNum,m_StatusworldPos))
        {
            break;
        }
        else
        {
            BaseCalcFun::SetBitValue(4,m_ContrlworldPos,1);
            QThread::msleep(10);
        }
    }
    BaseCalcFun::SetBitValue(4,m_ContrlworldPos,0);
    QThread::msleep(30);
}
void PackLib::ChangeAsixModel()
{
    //设置模式
    BaseAxisOperate::ChangeAxisModel("I8",m_ModelNum,m_ModelPos,6);
    QThread::msleep(30);
    for(int i = 0;i < 50;++i)
    {
        if(6 == BaseCalcFun::GetByteValue("I8",m_ModelNum,m_INputModelPos))
        {
            break;
        }
        else
        {
            BaseAxisOperate::ChangeAxisModel("I8",m_ModelNum,m_ModelPos,6);
            QThread::msleep(20);
        }
    }

    SetAxisStart();
}

int PackLib::ApproachORG()
{
    uchar ivalue = 0;
    BaseCalcFun::AnalysisIOString(hwLimitStart, ivalue);
    if(_ps.direction == 1)
    {
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGENDLIMIT);
    }
    else
    {
        //BaseCalcFun::AnalysisIOString(hwLimitEnd, ivalue);
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGSTARTLIMIT);
    }
    SetAxisStart();
    int ir = 0;
    if(ivalue == 1)
    {
        ir = GetResult(30000,true,0); //30000
    }
    else
    {
        ir = GetResult(30000,true,1); //30000
        if(ir == 0)
            ir = GetResult(30000,true,0);
    }
    //BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
    BaseCalcFun::SetBitValue(2,m_ContrlworldPos,0);
    QThread::msleep(100);
    for(int i = 0;i < 20;++i)
    {
        if(0 == BaseCalcFun::GetBitValue(5,m_StatusworldNum,m_StatusworldPos))
        {
            break;
        }
        else
        {
            BaseCalcFun::SetBitValue(2,m_ContrlworldPos,0);
            QThread::msleep(20);
        }
    }
    BaseCalcFun::SetBitValue(2,m_ContrlworldPos,1);
    QThread::msleep(50);
    return ir;
}
int PackLib::GetResult(int itimeout,bool isSatrt,uchar uc)
{
    int iResult = -1;
    //设置时间进入超时循环
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < itimeout)
    {
        if(isTerminate)
        {
            BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
            iResult = -99;
            break;
        }
        if(!isSatrt)//logic
        {
            uchar ivalue = 0;
            BaseCalcFun::AnalysisIOString(hwLimitEnd, ivalue);
            if(ivalue == uc)
            {
                iResult = 0;
                break;
            }
        }
        else
        {
            /***新增模式2**************/
            QString  strIO = hwLimitStart;
            if(_ps.orgmode == "1")
            {
                strIO = hwLimitStart;
            }
            else if(_ps.orgmode == "2")
            {
                strIO = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].orgLimit;
            }
            if(strIO != "")
            {
                uchar ivalue = 0;
                BaseCalcFun::AnalysisIOString(strIO, ivalue);
                if(ivalue == uc)
                {
                    iResult = 0;
                    break;
                }
            }
        }
        //        if( _ps.orgmode == "2")//需要判断起点硬极限
        //        {
        //            uchar ivalue = 0;
        //            BaseCalcFun::AnalysisIOString(hwLimitStart, ivalue);
        //            if(ivalue == 1)
        //            {
        //                iResult = -199;
        //                break;
        //            }
        //        }
        //校验故障
        if(BaseAxisOperate::CheckAxisFault(3,m_StatusworldNum,m_StatusworldPos) == 1)
        {
            iResult = -2;
            BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
            break;
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= itimeout)
            break;
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    return iResult;
}
void PackLib::Stop(bool isNor)
{
    isTerminate = true;
    if(isNor)
    {
        BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
    }
}

bool PackLib::SetAxisOp()
{
    QVector<QPair<int,int>> tempIn;
    foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
        tempIn.append(pair);
    }
    if(BaseAxisOperate::CheckAxisExcited(tempIn,m_StatusworldNum,m_StatusworldPos))
    {
        return true;
    }
    QVector<QPair<int,int>> temp;
    QVector<QPair<int,int>> tempoff;
    foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
        temp.append(pair);
        int offValue = 0;
        if(it.value == 0)
        {
            offValue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(it.bit,offValue);
        tempoff.append(pairoff);
    }
    BaseAxisOperate::SetAxisExciteOff(tempoff,m_ContrlworldPos);
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(temp,m_ContrlworldPos);
    QThread::msleep(50);
    if(BaseAxisOperate::CheckAxisExcited(tempIn,m_StatusworldNum,m_StatusworldPos))
        return true;

    for(int i = 0;i < 10;++i)
    {
        if(BaseAxisOperate::CheckAxisExcited(tempIn,m_StatusworldNum,m_StatusworldPos))
        {
            return true;
        }
        QThread::msleep(50);
    }
    return false;
}

bool PackLib::SetAxisBackParam()
{
    BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
    if(_ps.direction == 1)
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGSTARTLIMIT);
    else
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGENDLIMIT);
    QThread::msleep(30);
    SetAxisStart();
    return true;
}
void PackLib::SetAxisChangeimmediately(uchar uc)
{
    //立即变更
    BaseCalcFun::SetBitValue(5,m_ContrlworldPos,uc);
    QThread::msleep(30);
}

bool PackLib::ChangeAxisParam()
{
    //立即变更
    //    uchar *tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    //    BaseCalcFun::MemCopyTempData(tempOutputData);
    //    QVector<QPair<int,int>> tempBytePosNum;

    uint ispeed = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].lead *
            ConfigData::GetInstance()->iSpeed * 0.05 * _ps.S2 / 100.0;
    BaseAxisOperate::SetAxisSpeed("U32",m_TraspeedNum,m_TraspeedPos, ispeed);
    //    BaseAxisOperate::SetAxisSpeed("U32",m_TraspeedNum,m_TraspeedPos, ispeed,tempOutputData);
    //    QPair<int,int> pair = QPair<int,int>(m_TraspeedPos,m_TraspeedNum);
    //    tempBytePosNum.append(pair);
    //    //设置加减速
    //    uint  ia = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].adled *
    //            iAcc * 0.05* _ps.S2 /100;
    //    uint  id = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].adled *
    //            iDcc * 0.05 * _ps.S2 /100;
    //    BaseAxisOperate::SetAxisACC("U32",m_AccelNum,m_AccelPos,ia,tempOutputData);
    //    pair = QPair<int,int>(m_AccelPos,m_AccelNum);
    //    tempBytePosNum.append(pair);
    //    BaseAxisOperate::SetAxisDCC("U32",m_DecelNum,m_DecelPos,id,tempOutputData);
    //    pair = QPair<int,int>(m_DecelPos,m_DecelNum);
    //    tempBytePosNum.append(pair);
    //    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    //    QThread::msleep(20);
    SetAxisChangeimmediately(1);
    SetAxisStart();
    if(_ps.orgmode == "2")
    {
        QThread::msleep(50);
        int ir = GetResult(20000,true,0);
        if(ir != 0)
        {
            return false;
        }
    }
    //加延时
    QThread::msleep(_ps.orgmDelay);
    BaseAxisOperate::SetAxisStop(2,m_ContrlworldPos);
    SetAxisChangeimmediately(0);
    return true;
}

bool PackLib::SetAxisOverParam()
{
    //获取当前位置
    int curpos = BaseAxisOperate::GetAxisCurPos("I32",m_CurNum,m_CurPos);
   // if(curpos > swLimitEnd || curpos < swLimitStart)
       // return false;
    //设置绝对运动
    BaseCalcFun::SetBitValue(6,m_ContrlworldPos,0);
    QThread::msleep(50);
    //设置模式1
    BaseAxisOperate::ChangeAxisModel("I8",m_ModelNum,m_ModelPos,1);
    QThread::msleep(50);
    for(int i = 0;i < 100;++i)
    {
        if(1 == BaseCalcFun::GetByteValue("I8",m_ModelNum,m_INputModelPos))
        {
            break;
        }
        else
        {
            BaseAxisOperate::ChangeAxisModel("I8",m_ModelNum,m_ModelPos,1);
            QThread::msleep(30);
        }
    }
    //设置速度
    uint ispeed = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].lead *
            ConfigData::GetInstance()->iSpeed *0.05 * _ps.S1/ 100.0;
    BaseAxisOperate::SetAxisSpeed("U32",m_TraspeedNum,m_TraspeedPos, ispeed);
    //设置目标位置
    if(_ps.direction == 1)
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGENDLIMIT);
    else
        BaseAxisOperate::SetAxisPos("I32",m_TraposNum,m_TraposPos,ORGSTARTLIMIT);
    //设置加减速
    uint  ia = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].adled *
            iAcc * 0.05 * _ps.S1 /100.0;
    uint  id = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[_ps.axisId].adled *
            iDcc *0.05 * _ps.S1 /100.0;
    BaseAxisOperate::SetAxisACC("U32",m_AccelNum,m_AccelPos,ia);
    BaseAxisOperate::SetAxisDCC("U32",m_DecelNum,m_DecelPos,id);
    qDebug()<<"ia:"<<ia <<"id:"<<id;
    //qDebug() << QString("ORG -> SetAxisOverParam Speed :%1 Acc:%2 Dcc:%3").arg(ispeed).arg(ia).arg(id);
    return true;
}
