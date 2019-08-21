/*******************************************
 * @author ying
 * Project created 2017/6/8 上午9:05
 * @Describe
 *此命令主要是为用户提供脚本调用命令，用户直接使用规定的命令即可实现
 *
 *
 ********************************************/
#ifndef SCRIPTCALL_H
#define SCRIPTCALL_H
#include <QObject>
#include <QtPlugin>
#include <DycLibBase/ScriptManager/scriptmanager.h>
#include <QApplication>
#include <funmoduleinterface.h>
#include <QMap>
#include <QStandardPaths>

class ScriptCall:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    ScriptCall();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface* Copy();
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }

private:
    QString m_moduleName;//模块名称
    SWCONFIGXML_JS_STRU _ps;//脚本配置的结构体
private:
      QString GetscriExcMapandstr(int&);// 得到对用的数据结构
      template<typename T>
      QString GetExcScrMapStr(T axis);//对应的数据结构进行处理
      bool GetScrFunReValueandChangeConfValue(QString ScrFunReStr );//解析得到函数的返回字符串
      bool ChangeConfigDouvalue(QMap<QString,double> qmap);//更改原始的数据运算
      bool SaveDataToPath(QString str,QString &strMsg);

};

#endif // SCRIPTCALL_H
