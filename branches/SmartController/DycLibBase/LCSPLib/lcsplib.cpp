#include "lcsplib.h"
#include <QDebug>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>
#define NSEC_PER_SEC    (1000000000)
#define _CHANGE //０．０１ＭＭ (10)
const double _PARAMPULSE = 0.01;
LCSPLib::LCSPLib()
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this,&LCSPLib::IOTRigger,this,&LCSPLib::IOTri);
    GetlcspaxisinitParam();
    m_value = -1;
    m_bitname = "";
}
///
/// \brief LCSPLib::HandleArray
/// \return 处理的一个map数组方式
///若参数配置有错误或者获取的结构体是空那么这个ｍａｐ为空
/// 触发信息的判断计算也在此函数实现
QVector<QMap<QString,QVector<int>>> LCSPLib::HandleArray(const SWCONFIGXML_LCSP_STRU _ps)
{
    //调用转换库生成的结构　J1 J2 J3模式
    //SWCONFIGXML_LCSP_STRU a1;
    //_ps= a1;//进行赋值把坐标系更改为J1 J2 J3模式
    m_summap.clear();
    QVector<QMap<QString,QVector<int>>> sumlcspvec;
    QMap<QString,int> CurPos;
    if(_ps.LCSPCPos.size() > 0)
    {
        for(auto it = _ps.LCSPCPos[0].LCSCPosMap.begin();it != _ps.LCSPCPos[0].LCSCPosMap.end();++it)
        {
            int curPos=0;
            if(GetLCSPCurpos(it.key(),curPos))//获取轴的当前位置
            {
                CurPos.insert(it.key(),curPos);
            }
        }
    }
    for(int i=0; i<_ps.LCSPCPos.size();++i)//遍历vector
    {
        QMap<QString,int >axispos;
        for(auto it=_ps.LCSPCPos[i].LCSCPosMap.begin();it!=_ps.LCSPCPos[i].LCSCPosMap.end();++it)
        {
            axispos.insert(it.key(),it.value().pos);//每个线段中轴ＩＤ和位置处理
        }
        m_summap.append(axispos);//所有线段map集合
    }
    if(!CurPos.isEmpty())
        m_summap.insert(m_summap.begin(),CurPos);
    if(!handlesummap(m_summap,sumlcspvec))
    {
        return sumlcspvec;
    }
    return sumlcspvec;
}
void LCSPLib::SetLCSPSUMTraPosandModel()
{
    for(auto item = m_TraPosByteMap.begin();item != m_TraPosByteMap.end();++item)
    {
        int curPos = 0;
        GetLCSPCurpos(item.key(),curPos);
        BaseAxisOperate::SetAxisPos("I32",4,item.value(),curPos);
        QThread::msleep(20);
    }
    for(auto item = m_TraModelByteMap.begin();item != m_TraModelByteMap.end();++item)
    {
        for(int i = 0;i < 100;++i)
        {
            if(8 == BaseCalcFun::GetByteValue("I8",1,m_CurModelByteMap[item.key()]))//记住修改点模式检测的map
            {
                break;
            }
            else
            {
                BaseAxisOperate::ChangeAxisModel("I8",1,item.value(),8);
                QThread::msleep(20);
            }
        }
    }
}

///\接口输出给用户的用的运行函数
///存在多轴的几种情况运行不同的函数
/// 目标位置赋值，改变模式，励磁过程
/// 最后根据轴的数量运行不同的算法
int LCSPLib::RunLCSPAxis(const SWCONFIGXML_LCSP_STRU _ps)
{
    if(ConfigData::GetInstance()->m_CspStopFalg.contains(_ps.id))
    {
        ConfigData::GetInstance()->m_CspStopFalg[_ps.id] = 102;
    }
    else
    {
        ushort istate = 102;
        ConfigData::GetInstance()->m_CspStopFalg.insert(_ps.id,istate);
    }
    int iresult = 1;
    QVector<QMap<QString, QVector<int>>> numArrayVec = HandleArray(_ps);
    QMap<QString, QVector<int> > temp;
    for(int i = 0; i < numArrayVec.count(); ++i)
    {
        for(auto item = numArrayVec[i].begin();item != numArrayVec[i].end();++item)
        {
            if(!temp.contains(item.key()))
            {
                temp.insert(item.key(),item.value());
            }
            else
            {
                for(int j=0;j<item.value().size();++j)
                {
                    temp[item.key()].insert( temp[item.key()].size(),item.value()[j]);
                }
            }
        }
    }
     SetLCSPSUMTraPosandModel();
    auto item = _ps.LCSPCPos;//*********************test
    m_trimap.clear();
    for(int i = 0;i < item.count();++i)
    {
        GetLCSPTriggerInfo(item[i].LCSPTrigInfo,i,numArrayVec);
    }
    int size_map = temp.begin().value().size();
    if(!Getaccandctrspeed(_ps,size_map))
    {
        iresult = -1;
        return iresult;
    }
    RunLCSPSinAxis(_ps.id,temp);
    return iresult;
}

void LCSPLib::SOFFAxis()
{
    uchar *tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    BaseCalcFun::MemCopyTempData(tempOutputData);
    BaseAxisOperate::SetAxisExciteOff(m_tempOffOP,m_CWPosVec,tempOutputData);
    QThread::msleep(30);
}

void LCSPLib::StopLCSP(const QString strId)
{
    if(ConfigData::GetInstance()->m_CspStopFalg.contains(strId))
        ConfigData::GetInstance()->m_CspStopFalg[strId] = 101;
}
///
/// \brief LCSPLib::GetLCSPCurpos
/// \param strKey
/// \param curPos
/// \return
///用于得到轴的目标位置的函数
bool LCSPLib::GetLCSPCurpos(QString strKey, int &curPos)
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
        int ibyteNum = it->byteNum;
        QString strType = it->type;

        if(it->key == "Curpos")
        {
            //数据结构赋值存储
            curPos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);
            return true;
        }
        ibytePos += ibyteNum;
    }
    return false;
}

bool LCSPLib::GetlcspaxisinitParam()//用于检查驱动器获取轴初始化的参数
{
    m_tempInOP.clear();
    m_tempOutOP.clear();
    m_tempOffOP.clear();
    //获取检查励磁是否成功的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    //获取励磁的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
        int offvalue = 0;
        if(item.value == 0)
        {
            offvalue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offvalue);
        m_tempOffOP.append(pairoff);
    }
    return true;
}

bool LCSPLib::CheckLCSPAxisInit()
{
    for(auto it=m_SWByteMap.begin();it!=m_SWByteMap.end();++it)
    {
        //检查是否有故障
        if(BaseAxisOperate::CheckAxisFault(3,2,it.value()))
            return false;
        //检查轴是否励磁成功
        if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,it.value()))
            return false;
    }
    return true;
}

bool LCSPLib::InitCspAxisParam(const SWCONFIGXML_LCSP_STRU ps)
{
    m_eleratMap.clear();
    if(ps.LCSPCPos.size() > 0)
    {
        for(auto item = ps.LCSPCPos[0].LCSCPosMap.begin();
            item != ps.LCSPCPos[0].LCSCPosMap.end();++item)
        {
            if(ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.contains(item.key()))
            {
                for(auto itor = ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[item.key()].hwArmInfoMap.begin();
                    itor != ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[item.key()].hwArmInfoMap.end();
                    ++itor)
                {
                    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(itor.key()))
                        return false;
                    m_AxisIdVec.append(itor.key());
                    GetContrlWordPos(itor.key());
                    GetStatuswordPos(itor.key());
                    if(!m_eleratMap.contains(itor.key()))
                    {
                        m_eleratMap.insert(itor.key(),itor.value().elegearat);
                    }
                }
            }
        }
    }
    return true;
}

void LCSPLib::IOTri(QString bitName, int stat)
{
    QMutexLocker locker(&m_mutex);
    uchar ivalue = 0;
    ivalue = stat;
    m_value = ivalue;
    m_bitname = bitName;
    if(BaseCalcFun::AnalysisIOString(bitName,ivalue) == 0)//设置out值是否设置成功
    {
        qDebug()<<"IOTri"<<endl;
        m_timer->start(15);
    }
}
void LCSPLib::update()
{
    if(m_value != 0)
        m_value = 0;
    else
        m_value = 1;
    BaseCalcFun::AnalysisIOString(m_bitname,m_value);
}
QVector<QMap<QString,QVector<int>>> LCSPLib::GetLCSPArray(const SWCONFIGXML_LCSP_STRU _ps)
{
    return HandleArray(_ps);
}

QVector<LCSPLib::LCSPIOTRIGGERINFO_STRU> LCSPLib::GetLCSPTriggerInfoVector(const SWCONFIGXML_LCSP_STRU _ps)
{
    m_trimap.clear();
    QVector<QMap<QString, QVector<int>>> numArrayVec = HandleArray(_ps);

    auto item = _ps.LCSPCPos;//*********************test
    QVector<LCSPIOTRIGGERINFO_STRU> lcsptrgInfoVec;
    for(int i = 0;i < item.count();++i)
    {
        GetLCSPTriggerInfo(item[i].LCSPTrigInfo,i,numArrayVec);
    }
    return lcsptrgInfoVec;
}

int LCSPLib::SONAxis()
{
    for(auto item = m_TraPosByteMap.begin();item != m_TraPosByteMap.end();++item)
    {
        int curPos = 0;
        GetLCSPCurpos(item.key(),curPos);
        BaseAxisOperate::SetAxisPos("I32",4,item.value(),curPos);
        QThread::msleep(20);
    }
    for(auto it = m_AxisIdVec.begin();it != m_AxisIdVec.end();++it)
    {
        if(8 != BaseCalcFun::GetByteValue("I8",1,m_CurModelByteMap[*it]))
        {
            BaseAxisOperate::ChangeAxisModel("I8",1,m_TraModelByteMap[*it],8);
            QThread::msleep(30);
        }
    }
    if(CheckLCSPAxisInit())
        return 1;
    SOFFAxis();
    //设置轴励磁
    uchar *tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    BaseCalcFun::MemCopyTempData(tempOutputData);
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,m_CWPosVec,tempOutputData);
    QThread::msleep(100);
    if(!CheckLCSPAxisInit())
        return -4;
    return 1;
}
void LCSPLib::GetContrlWordPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return ;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        int ibyteNum = item->byteNum;
        if(item->key == "Contrlworld")
        {
            m_CWByteMap.insert(strKey, ibytePos);
            m_CWPosVec.append(ibytePos);
        }
        if(item->key == "Model")
        {
            m_TraModelByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Trapos")
        {
            m_TraPosByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Traspeed")
        {
            m_TraSpeedByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Accel")
        {
            m_TraAccByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Decel")
        {
            m_TraDccByteMap.insert(strKey, ibytePos);
        }
        ibytePos += ibyteNum;
    }
    return ;
}

void LCSPLib::GetStatuswordPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);

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
            m_SWByteMap.insert(strKey,ibytePos);
            m_SWPosVec.append(ibytePos);
        }
        if(it->key == "Model")
        {
            m_CurModelByteMap.insert(strKey, ibytePos);
        }
        if(it->key == "Curpos")
        {
            m_CurPosByteMap.insert(strKey, ibytePos);
        }
        ibytePos += ibyteNum;
    }
    return ;
}

bool LCSPLib::Getaccandctrspeed(const SWCONFIGXML_LCSP_STRU _ps,int axisNum)
{
    //计算最大速度频率和加速度位移
    double s = axisNum * 0.01;//总的行程
    k = 1.0;
    f = 0;
    x = 0;
    if(_ps.speed <= 10000.00&&_ps.speed > 350.00)
    {
        f = (_PARAMPULSE/_ps.speed)*1000000000;//ns频率计算0.01mm指的是脉冲当量不同的速度公式计算不同
    }
    else if(_ps.speed <= 350.00&&_ps.speed > 0.00)
    {
        f =(_PARAMPULSE/_ps.speed)*1000000000;//ns频率计算0.01mm指的是脉冲当量
    }
    else
    {
        return false;//代表设置的参数不对
    }
    if (_ps.acc<=20000.00&&_ps.acc > 0.00)//限制加速度的最大值12500mm/s^2
    {
        double sp0 = _ps.speed;
        x = ( sp0 * sp0)/( 2 * _ps.acc);

        //        if ( x > s/2)
        //        {
        //            return false;//代表没有加减速过程
        //        }
        //        //if(x < _PARAMPULSE*2)
        //        {
        //            return false;//代表设置了不合适的参数
        //        }
    }
    else
        return false;//代表设置的参数不对
    double a  = _ps.speed;
    double t = a / _ps.acc;
    k = ( t  * 1000000000 - x / 0.01 * f ) / ( x / 0.01 * ( x / 0.01 - 1)) * 2;    //计算变化的系数的值  //计算加减速时间这是一个变化量
    if( k < 0)
    {
        k = 0.0;
    }
    return true;
}
///
/// \brief LCSPLib::GetLCSPTriggerInfo
/// \return
///触发信息存储到所有的数据结构中便于信号函数的触发
bool LCSPLib::GetLCSPTriggerInfo(const QVector<LCSP_TRIGGERINFO_STRU> trgInfoVec,int indexLine,const QVector<QMap<QString, QVector<int> > > sumlcspmapvector)
{
    int n=-1;
    for(int j=0;j<trgInfoVec.size();++j)//触发信息vector第一层
    {
        LCSPIOTRIGGERINFO_STRU lcsptriggerstru;
        n=-1;
        int inde=-1;
        for(int k = 0;k < trgInfoVec[j].LCSPTrigCPos.size();++k)//触发信息轴位置信息vector
        {
            if(indexLine < m_PosMaxkey.size())
            {
                if(trgInfoVec[j].LCSPTrigCPos[k].id==m_PosMaxkey[indexLine])
                {
                    inde=k;
                    break;
                }
            }
        }
        if(inde >= 0)
        {
            double tripos = trgInfoVec[j].LCSPTrigCPos[inde].pos;
            if(m_summap[indexLine].contains(trgInfoVec[j].LCSPTrigCPos[inde].id))
            {
                if(m_calTrik[indexLine].contains(trgInfoVec[j].LCSPTrigCPos[inde].id))
                {
                    int ct=-1;//只求最大位置点的哪一个坐标即可
                    ct = qAbs((tripos - m_summap[indexLine][trgInfoVec[j].LCSPTrigCPos[inde].id])/(m_calTrik[indexLine][trgInfoVec[j].LCSPTrigCPos[inde].id]));
                    if(ct < sumlcspmapvector[indexLine][trgInfoVec[j].LCSPTrigCPos[inde].id].size()&& ct > 0)
                    {
                        if(m_eleratMap[trgInfoVec[j].LCSPTrigCPos[inde].id]>0)
                        {
                            if(qAbs(sumlcspmapvector[indexLine][trgInfoVec[j].LCSPTrigCPos[inde].id][ct]-tripos)<=(20*m_eleratMap[trgInfoVec[j].LCSPTrigCPos[inde].id]*_PARAMPULSE))
                            {
                                n = ct;
                                for(int i=0;i<indexLine;++i)
                                {
                                    n += sumlcspmapvector[i].begin().value().size();
                                }
                            }
                        }
                    }
                }
            }
        }
        lcsptriggerstru.ctrn=n;
        lcsptriggerstru.bitName=trgInfoVec[j].bitName;
        lcsptriggerstru.stat=trgInfoVec[j].stat;
        if(lcsptriggerstru.ctrn!=-1)
        {
            if (!m_trimap.contains(n))
            {
                m_trimap.insert(n,lcsptriggerstru);//存储为map
            }
        }//把有效的触发信息存入到位置
    }
    return true;
}
///
/// \brief LCSPLib::handlesummap
/// \param summap
/// \return false:表示数据处理过程中错误发生
///用于处理的到的所有折线段ｍａｐ
bool LCSPLib::handlesummap(QVector<QMap<QString,int >>summap, QVector<QMap<QString, QVector<int> > > & sumlcspvec)
{
    bool iscalok=true;
    if(summap.size() < 2)
        return false;
    m_calTrik.clear();//得到每个轴对应的系数ｋ
    m_PosMaxkey.clear();
    for(int i=0;i<summap.size()-1;++i)//所有线段判断是否符合要求
    {
        if(!Calculformula(summap[i+1],summap[i],sumlcspvec))
        {
            iscalok=false;
            return iscalok;
        }
    }
    return iscalok;
}
///
/// \brief LCSPLib::Calculformula
/// \param axismappos
/// \param axisfrontmappos
/// \return
///把数据存储到ｍａｐ数据中
bool LCSPLib::Calculformula(QMap<QString, int>axismappos, QMap<QString, int>axisfrontmappos, QVector<QMap<QString, QVector<int> > > & sumlcspvec)
{
    int GetMaxpos = 0;
    GetMaxpos = GetMaxPosnum(axismappos,axisfrontmappos);
    QMap<QString,double> trik;
    if(GetMaxpos != -1)
    {
        QMap<QString,QVector<int>> lcsparrmap;
        for(auto it=axismappos.begin();it!=axismappos.end();++it)
        {
            QVector<int> Arrayaxis;
            for(int i=1;i<= GetMaxpos;++i)
            {
                Arrayaxis.append(((it.value()-axisfrontmappos[it.key()])*1.0/GetMaxpos)*i+axisfrontmappos[it.key()]);
            }
            lcsparrmap.insert(it.key(),Arrayaxis);
            if(!trik.contains(it.key()))
            {
                if(GetMaxpos>=1)//如果最大位置点为０的时候
                {
                    trik.insert(it.key(),((it.value()-axisfrontmappos[it.key()])*1.0/GetMaxpos));
                }
            }
        }
        if(!lcsparrmap.isEmpty())
        {
            sumlcspvec.append(lcsparrmap);//所有这线段的集合
            m_calTrik.append(trik);
            return true;
        }
    }
    return false;
}
///
/// \brief LCSPLib::GetMaxPosnum
/// \param axismappos
/// \param ireturn
/// \param axisfrontmappos
/// \return
///
int LCSPLib::GetMaxPosnum(QMap<QString, int> axismappos, QMap<QString, int>axisfrontmappos)
{
    int GetMaxpos = -1;//得到最大的取值点那个
    QString keystr="";
    if(!axismappos.isEmpty() && !axisfrontmappos.isEmpty())
    {
        int Posrange = 0;
        for(auto it=axismappos.begin();it!=axismappos.end();++it)
        {
            if(m_eleratMap.contains(it.key()))//增加齿轮比的问题
            {
                Posrange = qAbs(it.value() - axisfrontmappos[it.key()])/(m_eleratMap[it.key()]*_PARAMPULSE);//0为当量0.01mm
                if(GetMaxpos < Posrange)//比较得到最大的点数
                {
                    GetMaxpos = Posrange;
                    keystr = it.key();
                }
            }
        }
    }
    m_PosMaxkey.append(keystr);
    return GetMaxpos;
}

void LCSPLib::RunLCSPSinAxis(const QString strId,QMap<QString, QVector<int>> numMap)
{
    uint vari = 0;
    uchar* tempdata = new uchar[InOutPutData::GetInstance()->outPutNum];
    QVector<QPair<int,int>> tempBytePosNum;
    auto it = numMap.begin().value();
    int isize=it.size();
    int inum = 0;
    int ix = x*100;
    struct timespec t;
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    clock_gettime(CLOCK_MONOTONIC ,&t);
    for(int ctr=0;(ctr<isize)&&(ConfigData::GetInstance()->m_CspStopFalg[strId] != 101);++ctr)
    {
        if(m_trimap.contains(ctr))
        {
            emit IOTRigger(m_trimap[ctr].bitName,m_trimap[ctr].stat);
        }
        //        k = 1*(ix - ctr)*ConfigData::GetInstance()->icspAccel + ConfigData::GetInstance()->icspSpeed;
        //       // qDebug()<<"k:"<<k;
        if(ctr <= ix)
        {
            vari = f + k*(ix - ctr);
        }
        else if(ctr >= isize - ix)
        {
            vari = f + (ctr - (isize-ix))*k;
        }
        else
        {
            vari = f;
        }
        //if((ctr/5 == inum) || (ctr == isize -1))
        {
            ++inum;
            tempBytePosNum.clear();
            BaseCalcFun::MemCopyTempData(tempdata);
            for(auto ior = numMap.begin();ior != numMap.end();++ior)
            {
                BaseAxisOperate::SetAxisPos("I32",4,m_TraPosByteMap[ior.key()],\
                        ior.value()[ctr],tempdata);
                QPair<int,int> pair = QPair<int,int>(m_TraPosByteMap[ior.key()],4);
                tempBytePosNum.append(pair);
            }
            BaseCalcFun::MemCopyOutputData(tempdata,tempBytePosNum);
        }
        t.tv_nsec += vari;
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
    gettimeofday(&tpEnd,NULL);
    timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
    qDebug()<<"all time:"<<timeUse;
}


