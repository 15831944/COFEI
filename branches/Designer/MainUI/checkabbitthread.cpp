#include "checkabbitthread.h"
#include <QDebug>
#include <sys/time.h>
#include <MyGlobal.h>
#include <QDebug>
#include <TeachingDevice/share_value.h>
using namespace CoreGlobal;
CheckABbitThread::CheckABbitThread()
{
    m_exit = true;
    m_counter = 0;
    m_rundir = ZER;
    m_AbitIO = "";
    m_BbitIO = "";
    m_motion_value = 0;
    m_check_over = true;
    GetValue_falg = true;
}

CheckABbitThread *CheckABbitThread::GetInstance()
{
    static CheckABbitThread INSTANCE;
    return &INSTANCE;
}

CheckABbitThread::~CheckABbitThread()
{
   Stop();
}

void CheckABbitThread::SetParam(QString AIOstr, QString BIOstr)
{
    m_AbitIO = AIOstr;
    m_BbitIO = BIOstr;
}

void CheckABbitThread::Stop()
{
    m_exit = false;
    m_counter = 0;
    m_rundir = ZER;
    deleteLater();
}

void CheckABbitThread::Reset()
{
    m_counter = 0;
    m_rundir = ZER;
}

char CheckABbitThread::GetBitValue(QString IOstr)
{
    char bitchar = ' ';
    //判断IOVALUE
    //判断IOVALUE
    uchar value = 0;
    // qDebug()<<"IO字符串"<<IOstr;
   int iresult =  BaseCalcFun::AnalysisIOString(IOstr,value);
    if(value == 0)
    {
        bitchar = '0';
    }
    else if(value == 1)
    {
        bitchar ='1';
    }
    if(iresult < 0){
        //qDebug()<<"检测到脉冲IO配置异常-1";
    }
    return bitchar;
}

///
/// \brief CheckABbitThread::Get_check_value 根据方向获取脉冲值
///
void CheckABbitThread::Get_check_value()
{
    m_counter = 0;
    int time_emit = 0;
    float timeuse_checkAbit;
    struct timeval tpStartAbit,tpEndAbit;
    struct timeval st,et;
    ShareClass::GetIstance()->m_getvalue_Stop = true;
    while (ShareClass::GetIstance()->m_getvalue_Stop)
    {
        gettimeofday(&tpStartAbit,NULL);
        if(GetBitValue("E1.00") == '1')
        {
            float timeuse;
            struct timeval tpStart,tpEnd;
            gettimeofday(&tpStart,NULL);
            while (ShareClass::GetIstance()->m_getvalue_Stop)
            {
                if(GetBitValue("E1.00") == '0')
                {

                    if(true == ShareClass::GetIstance()->m_direction && true == ShareClass::GetIstance()->m_ready)
                    {

                        if(ShareClass::GetIstance()->m_cruntValue < 0)
                            ShareClass::GetIstance()->m_cruntValue = 0;
                       ShareClass::GetIstance()->m_cruntValue++;
//                       qDebug()<<"edcode--:"<<ShareClass::GetIstance()->m_cruntValue;
//                                       qDebug()<<"timeuse"<<timeuse;
                    }
                    else if(false == ShareClass::GetIstance()->m_direction&& true == ShareClass::GetIstance()->m_ready)
                    {
                        if(ShareClass::GetIstance()->m_cruntValue > 0)
                            ShareClass::GetIstance()->m_cruntValue = 0;
                        ShareClass::GetIstance()->m_cruntValue--;
//                        qDebug()<<"edcode--:"<<ShareClass::GetIstance()->m_cruntValue;
//                                       qDebug()<<"timeuse"<<timeuse;
                    }
                    break;
                }
                QThread::usleep(50);
                gettimeofday(&tpEnd,NULL);
                timeuse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);


                if(timeuse >= 100)//没有检测到电机正反转那么退出
                {

//                    qDebug()<<"m_rundir"<<m_counter;
                    m_rundir = ZER;//方向复位
                    ShareClass::GetIstance()->m_cruntValue = 0;
                    ShareClass::GetIstance()->m_ready = false;
                    break;
                }
            }
        }
        else
        {
            gettimeofday(&st,NULL);
            while(ShareClass::GetIstance()->m_getvalue_Stop)
            {
                gettimeofday(&et,NULL);
                m_convertime = 1000 *(et.tv_sec - st.tv_sec) + 0.001*(et.tv_usec - st.tv_usec);
                if(GetBitValue("E1.00") == '1')
                    break;
                if( m_convertime >= 300)
                {
                    m_rundir = ZER;//方向复位
                    ShareClass::GetIstance()->m_cruntValue = 0;
                    ShareClass::GetIstance()->m_ready = false;
                    break;
                }
                QThread::usleep(100);
            }
        }
        gettimeofday(&tpEndAbit,NULL);
        timeuse_checkAbit = 1000 *(tpEndAbit.tv_sec - tpStartAbit.tv_sec) + 0.001*(tpEndAbit.tv_usec - tpStartAbit.tv_usec);
//        qDebug()<<"time used"<<timeuse_checkAbit;
        time_emit = time_emit + timeuse_checkAbit;
        if(time_emit > 50) // 收集周期　50 ms
        {
            time_emit = 0;
            InOutPutData::GetInstance()->m_pulseMutex.lock();
            InOutPutData::GetInstance()->Current_pulse = ShareClass::GetIstance()->m_cruntValue;
            InOutPutData::GetInstance()->m_pulseMutex.unlock();
            ShareClass::GetIstance()->m_cruntValue = 0;
//            qDebug()<<"InOutPutData::GetInstance()->Current_pulse=" <<InOutPutData::GetInstance()->Current_pulse;
        }

        if(timeuse_checkAbit >= TIMEOUT )
        {
//            qDebug()<<"m_"<<m_counter;
            m_rundir = ZER;//方向复位
            ShareClass::GetIstance()->m_cruntValue = 0;
            ShareClass::GetIstance()->m_ready = false;
          //  qDebug()<<ShareClass::GetIstance()->m_cruntValue;
            //break;
        }
    }
}

void CheckABbitThread::StopWhile()
{
    GetValue_falg = false;
}

