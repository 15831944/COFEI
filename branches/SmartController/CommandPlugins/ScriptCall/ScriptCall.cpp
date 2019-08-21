#include "ScriptCall.h"
#include <sys/time.h>
#include <QString>
#include <MyGlobal.h>
#include <QFile>
#include <QTextStream>
#include <QDir>
using namespace CoreGlobal;
ScriptCall::ScriptCall()
{
    m_moduleName = "JS";
}
void ScriptCall::Initialization()
{
    //todo Initialization variable
    isTerminate = false;//调用中断标志


}
void ScriptCall::SetModuleParam(SWCONFIGXML_STRU ps)
{
    _ps = ps.swXml_JS_Stru;//动态脚本部分的结构体赋值
}

void ScriptCall::RunModule()
{
    //todo call command for IO


}
///
/// \brief ScriptCall::GetExcResult/总的处理脚本调用的命令函数
/// \param strMsg引用用于输出执行函数信息日志
/// \return　返回值为int不同的结果对应不同的返回值大小
/// １：插件调用成功
/// －１：若字符串为空且也不是第三方机构中的，那么解析失败，检查ＸＭＬ配置！
///－２：用户执行脚本文件中指定的函数名字包含有”Error“字符，请检查用户脚本文件中函数是否正确！
/// －３：函数的返回值中的ｍａｐ为空（ｍａｐ为固定格式返回），那么失败，还是要检查用户函数的字符串的格式问题！
/// －９：执行根据ＸＭＬ中配置的文件名字和路径创建文件，并写入文件的过程失败！一般在软件ＸＭＬ脚本或者快速定位ＩＤ配置错误！
///
int ScriptCall::GetExcResult(QString &strMsg)
{
    int iresult = -1;//函数的返回结果赋值初始化
    int iNum = 0;//对值进行初始化并作为引用获取map的大小
    QString strParm = GetscriExcMapandstr(iNum);//运行获取对应的ｍａｐ并处理成规定的字符串形式
    if(!strParm.isEmpty())//判断对应数据结构map的字符串
    {
        QString str = QString("%1('%2',%3)").arg(_ps.funtionName).arg(strParm).arg(iNum);//把脚本中函数名称和参数整理
        QString GetScrFunReStr = ScriptManager::GetInstance()->RunAlgorithm(str);//运行脚本函数并返回值
        if(GetScrFunReStr.contains("Error"))//?判断返回值是否有有错误信息
        {
            iresult = -2;
            strMsg = QString("%1 %2 %3 用户脚本文件函数返回字符串有错误（“Error”） %4").arg(m_moduleName).arg(_ps.funtionName).arg(_ps.offsetObject.type).arg(GetScrFunReStr);
            return iresult;
        }
        else
        {
            //把字符串进行存储到指定的目标路径中的用户创建的ｔｘｔ文本中
              strMsg="";
            if(!ScriptCall::SaveDataToPath(GetScrFunReStr,strMsg))
            {
                iresult = -9;
                strMsg = QString("%1 %2 函数返回字符串写入文本失败，信息: %3").arg(m_moduleName).arg(_ps.savePathKey).arg(strMsg);
                return iresult;
            }
            if(GetScrFunReValueandChangeConfValue(GetScrFunReStr))//解析字符串并赋值
            {
                iresult = 1;
                strMsg = QString("%1 %2 %3 脚本调用成功").arg(m_moduleName).arg(_ps.funtionName).arg(_ps.offsetObject.type);
                return iresult;
            }
            else
            {
                iresult = -3;
                strMsg = QString("%1 %2 %3 赋值失败").arg(m_moduleName).arg(_ps.funtionName).arg(_ps.offsetObject.type);
                return iresult;
            }
        }
    }
    else
    {
        if(_ps.offsetObject.type.compare("第三方机构") == 0)
        {
            QString str = QString("%1()").arg(_ps.funtionName);//把脚本中函数名称和参数整理
            QString GetScrFunReStr = ScriptManager::GetInstance()->RunAlgorithm(str);//运行脚本函数并返回值
            if(GetScrFunReStr.contains("Error"))//?判断返回值是否有有错误信息
            {
                iresult = -2;
                strMsg = QString("%1 %2 %3 用户返回字符串有错误 %4").arg(m_moduleName).arg(_ps.funtionName).arg(_ps.offsetObject.type).arg(GetScrFunReStr);
                return iresult;
            }
            else
            {
                //把字符串进行存储到指定的目标路径中的用户创建的ｔｘｔ文本中
                strMsg="第三方机构类型数据保存文件信息成功";
                if(!ScriptCall::SaveDataToPath(GetScrFunReStr,strMsg))
                {
                    iresult = -9;
                    strMsg = QString("%1 %2 函数返回字符串写入文本失败，信息: %3").arg(m_moduleName).arg(_ps.savePathKey).arg(strMsg);
                    return iresult;
                }
                iresult = 1;
            }
        }
        else
        {
            iresult = -1;
            strMsg = QString("%1 %2 %3 解析map字符串失败").arg(m_moduleName).arg(_ps.funtionName).arg(_ps.offsetObject.type);
            return iresult;
        }
    }
    return iresult;
}
void ScriptCall::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *ScriptCall::Copy()
{
    ScriptCall *s = new ScriptCall();
    return s;
}
///
/// \brief ScriptCall::GetscriExcMapandstr//获取MAP对应的数据整理成字符串
/// \param iNum/得到ｍａｐ根据ｋｅｙ的item的个数
/// \return　返回获取的字符串
///
QString ScriptCall::GetscriExcMapandstr(int &iNum)
{
    QString scriExcMapiresultstr = "";//定义得到脚本定义ｍａｐ对应的字符串进行初始化为ｎｕｌｌ
    if(_ps.offsetObject.type.compare("快速定位") == 0)
    {
        auto qpMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
        if(qpMap.contains(_ps.offsetObject.id))// 判断ｉｄｋｅｙ
        {
            scriExcMapiresultstr = GetExcScrMapStr(\
                        qpMap[_ps.offsetObject.id].axisMap);//调用处理ｍａｐ成字符串的函数并赋值
            iNum = qpMap[_ps.offsetObject.id].axisMap.count();//得到ｍａｐ根据ｋｅｙ的item的个数

        }
    }
    else if(_ps.offsetObject.type.compare("直线插补") == 0)
    {
        auto qpMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map;
        if(qpMap.contains(_ps.offsetObject.id))// 判断ｉｄｋｅｙ
        {
            auto items = qpMap[_ps.offsetObject.id].LCSPCPos;
            auto item = items[items.size()-1];
            auto it = item.LCSCPosMap;
            scriExcMapiresultstr = GetExcScrMapStr(it);//调用处理ｍａｐ成字符串的函数并赋值
            iNum = it.count();//得到ｍａｐ根据ｋｅｙ的item的个数
        }
    }
    else
    {
        scriExcMapiresultstr = "";
    }

    return scriExcMapiresultstr;
}
///
/// \brief ScriptCall::GetExcScrMapStr
/// \param axis/获取到的对应的数据结构
/// \return　处理字符串的结果
/// 根据得到的数据结构整理成字符串
template<typename T>
QString ScriptCall::GetExcScrMapStr(T axis)//
{
    QString Getsciptconfigstr = NULL;//对存储函数的参数进行初始化
    //获取对象中axis的Key和value遍历axis对象组成特定的字符串
    auto item = axis.cbegin();//定义参数迭代器
    while(item != axis.cend())
    {
        Getsciptconfigstr += item.key()+":"+QString::number(item.value().pos);//按照规定的格式进行处理
        Getsciptconfigstr += ",";//按照规定的格式进行处理规定格式例如"x:5,y:6,U:45"
        if(++item == axis.cend())
            Getsciptconfigstr.chop(1); //删除最后一个字符“；”
    }
    return Getsciptconfigstr;
}
///
/// \brief ScriptCall::GetScrFunReValueandChangeConfValue
/// \param ScrFunReStr　用户运行调用脚本函数运行结果的字符串
/// \return　判断改变原始数据的值是否成功
/////解析运行函数之后并赋值改变
bool ScriptCall::GetScrFunReValueandChangeConfValue(QString ScrFunReStr)
{
    bool changevalueisok=false;//定义返回值并初始化
    QStringList GetScrFunReStrList;//定义字符串分割的ｌｉｓｔ
    GetScrFunReStrList = ScrFunReStr.split(",");//以“；”进行分割
    QMap<QString,double> SciFunReStrmap;//定义成数据结构并把要返回值存储为数据结构中
    SciFunReStrmap.clear();//先对ｍａｐ清空
    for(int i=0;i < GetScrFunReStrList.size();++i)//字符串分割遍历
    {
        QString  st0 = GetScrFunReStrList[i];
        QStringList st0List;
        st0List =  st0.split(":");//以“：”进行分割得到对应ｋｅｙ和ｖａｌｕｅ
        if(st0List.size()== 2)//判断分割之后是否为２
        {
            QString axisnumber=st0List[0];
            QString  axispos=st0List[1];
            SciFunReStrmap.insert(axisnumber,axispos.toDouble());//赋值运算把转换成double类型存储数据结构中
        }
    }

    if(ChangeConfigDouvalue(SciFunReStrmap))//调用函数修改配置的数据
    {
        changevalueisok=true;
    }

    return changevalueisok;
}
///
/// \brief ScriptCall::ChangeConfigDouvalue
/// \param qmap　把函数的返回的字符串存储到qmap
/// \return　判断修改数据是否成功
/////赋值修改原始数据函数
bool ScriptCall::ChangeConfigDouvalue(QMap<QString,double> qmap)
{
    bool changevalueisok=true;//定义函数的返回值并进行初始化
    if(!qmap.isEmpty())//判断存储的数据结构是否为空
    {
        if(_ps.offsetObject.type.compare("快速定位") == 0)
        {
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(_ps.offsetObject.id))// 按照偏移对象的类型找到偏移对象的ｉｄ作为key
            {
                for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.offsetObject.id].axisMap.begin();\
                    it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.offsetObject.id].axisMap.end();\
                    ++it)
                {
                    if(qmap.contains(it.key()))//判断qmap中ｋｅｙ是否与对应原始数据ｋｅｙ
                    {
                        if(it.value().offsetPos == "0")
                        {
                            it.value().offsetPos = QString::number(qmap[it.key()]);//ｑｍａｐ的ｖａｌｕｅ赋值给原始配置数据中偏移位置对象中
                        }
                    }
                }
            }
        }
        else if(_ps.offsetObject.type.compare("直线插补") == 0)
        {  
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(_ps.offsetObject.id))
            {
                for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[_ps.offsetObject.id].LCSPOffsetPosMap.begin();
                    it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[_ps.offsetObject.id].LCSPOffsetPosMap.end();++it)
                {
                    if(qmap.contains(it.key()))//判断qmap中ｋｅｙ是否与对应原始数据ｋｅｙ
                    {
                        it.value() = qmap[it.key()];//ｑｍａｐ的ｖａｌｕｅ赋值给原始配置数据中偏移位置对象中
                    }
                }
            }
        }
    }
    else
    {
        changevalueisok = false;//若数据结构为空那么函数返回赋值失败
    }
    return changevalueisok;
}

bool ScriptCall::SaveDataToPath(QString str,QString &strMsg)
{
    //1.找到用户配置的文件名称
    bool isok=false;
    QString strpath="";
    QString path="";
    for(auto it=ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.begin();
        it!=ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SavePath_Map.savePathVec.end();++it)
    {
        if(it->key ==_ps.savePathKey)
        {
            //２．获取路径创建路径
            isok=true;
           // path = QDir::currentPath();
            path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
            strpath=QString("%1/Data/%2").arg(path).arg(it->address);
            break;
        }

    }
    if(!isok)
    {
        strMsg=QString( "swxml configure error,%1 Key Non-existent!").arg( _ps.savePathKey);
        return false;
    }
    //３．创建目录和文件,追加数据到文当文件中
    QDir dir(path+"/Data/");
    if(!dir.exists())
    {
        dir.mkdir(path+"/Data/");
    }
    QFile file(strpath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream(&file);
    stream<<str<<"\n";
    file.close();
    return true;
}
