#include "doublestart.h"
#include <MyGlobal.h>
#include <unistd.h>
#include <QElapsedTimer>
#include  <QDebug>

//使用命名空间
using namespace CoreGlobal;

DoubleStart::DoubleStart()
{
    m_moduleName = "DS";//给予命令字符名字
}

void DoubleStart::Initialization()
{
    isTerminate = false;//中断标志位一般默认位false
}


void DoubleStart::SetModuleParam(SWCONFIGXML_STRU ps)//初始化完成检查是否有按钮被按下，若有返回重新执行
{
    _ps = ps.swXml_DB_Stru.doubleButtonInfoVec;//函数参数进行自定义变量进行初始
}

void DoubleStart::RunModule()
{

}
//此函数实现的功能为
int DoubleStart::GetExcResult(QString &strMsg)
{
    int iresult = -1;//给变量进行初始化
    QElapsedTimer SetDouButStarintervalTimer;//按钮有效的间隔
    int SetDouButStarintervaltimeoutms = 1000;//1000ms
    int DouButStarintervalRemainingTime = 0;//剩余时间
    static bool isfirstStart = true;//按钮间隔定时器触发标志位限制在循环满足条件只启动一次
    isfirstStart = true;
    QElapsedTimer SetTotaltimeoutTimer;//超时计时器
    int SetTotaltimeoutms = 3000000;//3000000ms用户进行设置配置
    int TotaltimeoutRemainingTime = 0;//剩余时间
    SetTotaltimeoutTimer.start();//总超时计时器启动
    bool onlyinitok=true;
    while (TotaltimeoutRemainingTime >= 0)//总超时循环周期
    {
        if(isTerminate)//中断标志位判断
        {
            iresult = -99;
            strMsg = QString("%1  Terminate!").arg(m_moduleName);
            return iresult;//返回中断结果
        }
        if(onlyinitok)
        {
            if(CheckButtonIOStat0()) //初始化判断
                onlyinitok=false;
            else
            {
                strMsg = "启动初始化失败,按钮被按下,请把按钮复位重新执行";
            }
        }
        else
        {
            if(_ps.size()==2)
            {
                bool isCheckOk = true;//定义按钮两个都被按下标志
                bool resetButtonOk = true;//定义在设置按钮间隔时间内失败然后进行按钮复位的标志
                for(int j=0;j<_ps.count();++j)//遍历数据结构对按钮状态判断
                {
                    uchar ivalue = 0;//变量初始化
                    BaseCalcFun::AnalysisIOString(_ps[j].bitName,ivalue);//调用获取按钮状态的函数并把状态情况赋值给ivalue变量
                    if(ivalue != _ps[j].stat)//每次只要有一个按钮被按下，则就会执行定时器
                    {
                        isCheckOk = false;//检测按钮状态没有被按下则会false
                    }
                    else
                    {
                        resetButtonOk = false;//只要有一个没有被复位就不能执行
                        if(isfirstStart)//启动计时器标志
                        {
                            isfirstStart = false;//防止定时器再次启动
                            SetDouButStarintervalTimer.start();
                        }
                    }
                }
                DouButStarintervalRemainingTime = SetDouButStarintervaltimeoutms - SetDouButStarintervalTimer.elapsed();//计算时间流失时间
                if(DouButStarintervalRemainingTime < 0)//判断若没有在规定时间内完成
                {
                    if(resetButtonOk)//判断复位是否成功
                    {
                        //isfirstStart = true;// 重新启动定时器
                        SetDouButStarintervalTimer.restart();
                        DouButStarintervalRemainingTime = 0;// 定时器初始化为0
                    }
                }
                else
                {
                    if(isCheckOk)//在满足规定的时间间隔内判断是否按钮都被按下的状态
                    {
                        iresult = 1;//返回值为1代表成功
                        strMsg = QString("%1 %2 %3 success!").arg(m_moduleName).arg(_ps.at(0).bitName).arg(_ps.at(1).bitName);
                        break;//跳出循环进入下一次的命令插件的调用
                    }
                }
            }
            else
            {
                //单启动模式的判断
                uchar ivalue = 0;
                BaseCalcFun::AnalysisIOString(_ps[0].bitName,ivalue);//调用获取按钮状态的函数并把状态情况赋值给ivalue变量
                if(ivalue == _ps[0].stat)
                {
                    iresult = 1;//返回值为1代表成功
                    strMsg = QString("%1 %2 success!").arg(m_moduleName).arg(_ps.at(0).bitName);
                    return iresult;//跳出循环进入下一次的命令插件的调用
                }
            }
        }
        TotaltimeoutRemainingTime = SetTotaltimeoutms - SetTotaltimeoutTimer.elapsed();//计算总的流失时间
        QThread::msleep(5);//while循环命令提供缓存时间防止界面死掉
        QApplication::processEvents();//while循环命令提供缓存时间防止界面死掉
    }
    if(iresult == -1)//若还是为-1则为总超时跳出循环则返回值进行赋值
    {
        iresult = -3;
        strMsg = "总超时报警，请重新执行";
    }
    return iresult;
}

void DoubleStart::StopModule()//模块停止函数
{
    isTerminate = false;
}

FunModuleInterface *DoubleStart::Copy()
{
    return (new DoubleStart());
}
///
/// \brief DoubleStart::CheckButtonIOStat0
/// \return true 说明到的按钮复位了，可以执行否则需要一直检测知道复位才可以执行
///检测有没有按钮复位的函数，若一直没复位则会一直执行
bool DoubleStart::CheckButtonIOStat0()
{
    bool initOk = true;//判断初始化是否ok的bool量
    bool isok = true;//函数局部判断按钮有没有没被按下的情况
    if((_ps.size()==2)||(_ps.size()==1)) //判断结构体大小是不是2个或者１个，执行双按钮模式情况
    {
        for(int j=0;j<_ps.count();++j)//遍历数据结构的变量并获取当前的状态按钮
        {
            uchar ivalue=0;//对变量进行初始化
            if(BaseCalcFun::AnalysisIOString(_ps[j].bitName,ivalue) != 1)//调用函数把获取按钮的状态，把状态的值赋值给ivalue变量
            {
                initOk = false;//出现此种状态的原因获取值不成功的情况直接初始化失败

            }
            if(ivalue == _ps[j].stat)//判断获取的状态是否为没有被按下与设置值进行比较一个被按下
            {
                isok = false;//此时的状态为设置值与获取的状态的值进行比较，若与设置值相同那么按钮已经被按下所以为false
            }
        }
        if(!isok)

            initOk = false;
    }
    else
    {
        initOk = false;
    }
    return initOk;

}



