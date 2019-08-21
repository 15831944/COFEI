#include "checkdirection.h"
#include <QDebug>
#include <sys/time.h>
#include <MyGlobal.h>
#include <QDebug>
#include <TeachingDevice/share_value.h>
using namespace CoreGlobal;
CheckDirection::CheckDirection(QObject *parent) : QObject(parent)
{

}

bool CheckDirection::Direction_judgement()
{
    ShareClass::GetIstance()->m_direction_Stop = true;
    while (ShareClass::GetIstance()->m_direction_Stop) {
        {
            static int checktimeFWD = 0;
            static int chechtimeREV = 0;
//            if(GetBitValue(ShareClass::GetIstance()->A_IO_Address) == '1'&& GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '1')
//            {
                m_check_over = true;
                while(true == m_check_over) //检测方向
                {

                    if(GetBitValue(ShareClass::GetIstance()->A_IO_Address)=='1'&& GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '0')//　正波形
                    {
                        struct timeval st1,et1;
                        gettimeofday(&st1,NULL);
                        while(ShareClass::GetIstance()->m_direction_Stop)//检测一定时间内是否有完整的波形
                        {
                            QThread::usleep(400);
                            gettimeofday(&et1,NULL);

                            if(GetBitValue(ShareClass::GetIstance()->A_IO_Address)=='0'&& GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '0')
                            {
                                struct timeval st,et;
                                gettimeofday(&st,NULL);
                                while(ShareClass::GetIstance()->m_direction_Stop)//检测4/4段波
                                {
                                    QThread::usleep(400);
                                    gettimeofday(&et,NULL);
                                    if(GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '1'&& GetBitValue(ShareClass::GetIstance()->A_IO_Address) == '0')
                                    {
                                        checktimeFWD --;
                                        chechtimeREV ++;
                                        m_check_over = false;
                                        break;
                                    }
                                    int cyc = 1000 *(et.tv_sec - st.tv_sec) + 0.001*(et.tv_usec - st.tv_usec);
                                    if( cyc >= 200)//200ms未检测到4/4段波　跳出
                                    {
                                        break;
                                    }
                                }
                                break;
                            }

                            int cyc1 = 1000 *(et1.tv_sec - st1.tv_sec) + 0.001*(et1.tv_usec - st1.tv_usec);
                            if( cyc1 >= 2)//20ms 没检测到3/4段波　跳出
                            {
                                break;
                            }
                        }

                    }
                    else if(GetBitValue(ShareClass::GetIstance()->A_IO_Address)=='0'&& GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '1')
                    {
                        while(ShareClass::GetIstance()->m_direction_Stop)
                        {
                            if(GetBitValue(ShareClass::GetIstance()->A_IO_Address)=='0'&& GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '0')
                            {
                                struct timeval st,et;
                                gettimeofday(&st,NULL);
                                while(ShareClass::GetIstance()->m_direction_Stop)
                                {
                                    gettimeofday(&et,NULL);
                                    if(GetBitValue(ShareClass::GetIstance()->B_IO_Address) == '0'&& GetBitValue(ShareClass::GetIstance()->A_IO_Address) == '1')
                                    {
                                        checktimeFWD++;
                                        m_check_over = false;
                                        break;
                                    }
                                    int cyc = 1000 *(et.tv_sec - st.tv_sec) + 0.001*(et.tv_usec - st.tv_usec);
                                    if( cyc >= 200)
                                    {
                                        break;
                                    }
                                    QThread::usleep(500);
                                }
                                break;
                            }
                            QThread::usleep(500);
                        }

                    }
                    if(checktimeFWD >=2)
                    {
                        m_check_over = false;
                        checktimeFWD = 0;
                        chechtimeREV = 0;
                        ShareClass::GetIstance()->m_direction = true;
                        ShareClass::GetIstance()->m_ready = true;
                    }
                    if(chechtimeREV >=1)
                    {
                        m_check_over = false;
                        chechtimeREV = 0;
                        checktimeFWD = 0;
                        ShareClass::GetIstance()->m_direction = false;
                        ShareClass::GetIstance()->m_ready = true;
                    }
                    //qDebug()<<ShareClass::GetIstance()->m_direction;
                    QThread::usleep(100);
                }
            //}
            //else
                //m_rundir == ZER;
        }
        QThread::usleep(100);
    }
    return true;
}
char CheckDirection::GetBitValue(QString IOstr)
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
      //  qDebug()<<"检测到脉冲IO配置异常";
    }
    return bitchar;
}
