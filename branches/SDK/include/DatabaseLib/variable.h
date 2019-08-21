#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>
#include <QList>
#include <QMap>
/*******************重新定义其结构体 2018/5/5**********************/
typedef struct _BASEDB_STRU//数据库表的基本类型
{
    char pkey[32] ;
    char date[32] ;
    char type[32] ;
    char note[128] ;
    char fkey[32];
    _BASEDB_STRU(){
        memset(pkey,0,32);
        memset(date,0,32);
        memset(type,0,32);
        memset(note,0,128);
        memset(fkey,0,32);
    }
    _BASEDB_STRU& operator=(const _BASEDB_STRU &other)
    {
        strncpy(pkey,other.pkey,32);
        strncpy(date,other.date,32);
        strncpy(fkey,other.fkey,32);
        strncpy(note,other.note,128);
        strncpy(type,other.type,32);
        return *this;
    }
}BASEDB_STRU;
//定义各个变量类型
enum VarType{
    RegTextTy = 0 ,//字符串型寄存器
    RegNumTy ,     //数值型寄存器
    SwitchTy ,    //开关类
    TimeTy ,      //计时类
    CountTy ,     //计数类
    CyTy ,       //气缸类
    QPosTy ,     //运动控制PP模式
    LCSPTy      //运动控制CSP模式
};
typedef struct VARDB_STRU :public BASEDB_STRU
{
    bool save ;
    VARDB_STRU(){
        save = false;
    }
    VARDB_STRU &operator =(const  VARDB_STRU &other){
        *((BASEDB_STRU*)this) = other;
        save = other.save;
        return *this;
    }
}VARDB_STRU;
typedef struct VARDB_REGTEXT_STRU :public VARDB_STRU//寄存器字符串型
{
    char childtype[16];
    char textval[64];
    VARDB_REGTEXT_STRU(){
        memset(childtype,0,16);
        memset(textval,0,64);
    }
    VARDB_REGTEXT_STRU &operator =(const VARDB_REGTEXT_STRU &other){
         *((VARDB_STRU*)this) = other;
        strncpy(childtype,other.childtype,16);
        strncpy(textval,other.textval,64);
        return *this;
    }
}VARDB_REGTEXT_STRU;
///
typedef struct _VARDB_REGNUM_STRU:public VARDB_STRU//寄存器数值型
{
    char childtype[16];
    int pointnum ;
    double Initvalue ;//初值
    double upvalue ;//上限
    double lowvalue ;//下限
    _VARDB_REGNUM_STRU(){
        Initvalue = 0.00;
        upvalue = 0.00;
        lowvalue = 0.00;
        pointnum = -1;
        memset(childtype,0,16);
    }
    _VARDB_REGNUM_STRU &operator =(const _VARDB_REGNUM_STRU &other){
        *((VARDB_STRU*)this) = other;
        strncpy(childtype,other.childtype,16);
        pointnum = other.pointnum;
        Initvalue = other.Initvalue;
        upvalue = other.upvalue;
        lowvalue = other.lowvalue;
        return *this;
    }
}VARDB_REGNUM_STRU;
typedef struct _VARDB_SWITCH_STRU:public VARDB_STRU
{
    int initvalue ;
    bool ishwIO;
    char IOaddr[16];
    bool isBuilt_in;//新增内置类型这一项，预留软IO开关类
    _VARDB_SWITCH_STRU(){
        initvalue = 0;
        ishwIO = false;
        memset(IOaddr,0,16);
        isBuilt_in = false;//新增内置类型判断方式
    }
    _VARDB_SWITCH_STRU& operator =(const _VARDB_SWITCH_STRU &other){
        *((VARDB_STRU*)this) = other;
        initvalue = other.initvalue;
        ishwIO = other.ishwIO;
        strncpy(IOaddr,other.IOaddr,16);
        return *this;
    }
}VARDB_SWITCH_STRU;
typedef struct _VARDB_FUNMOD_STRU:public VARDB_STRU//功能模块
{
    char funtype[8] ;
    char startIO[16] ;
    int initvalue ;
    int setvalue ;
    int curvalue ;
    char endIO[16];
    _VARDB_FUNMOD_STRU(){
        initvalue = 0;
        setvalue = 0;
        curvalue = 0;
        memset(funtype,0,8);
        memset(startIO,0,16);
        memset(endIO,0,16);
    }
    _VARDB_FUNMOD_STRU &operator =(const _VARDB_FUNMOD_STRU &other){
         *((VARDB_STRU*)this) = other;
        strncpy(funtype,other.funtype,8);
        strncpy(startIO,other.startIO,16);
        initvalue = other.initvalue;
        setvalue = other.setvalue;
        curvalue = other.curvalue;
        strncpy(endIO,other.endIO,16);
        return *this;
    }
}VARDB_FUNMOD_STRU;
typedef struct _VARDB_TIME_STRU:public VARDB_FUNMOD_STRU//计时类结构体
{
    _VARDB_TIME_STRU(){

    }
    _VARDB_TIME_STRU &operator =(const  _VARDB_TIME_STRU &other){
         *((VARDB_FUNMOD_STRU*)this) = other;
        return *this;
    }
}VARDB_TIME_STRU;
typedef struct _VARDB_COUNT_STRU:public VARDB_FUNMOD_STRU//计数类结构体
{
    int addvalue ;
    _VARDB_COUNT_STRU(){
        addvalue = 1;
    }
    _VARDB_COUNT_STRU &operator =(const  _VARDB_COUNT_STRU &other){
         *((VARDB_FUNMOD_STRU*)this) = other;
        addvalue = other.addvalue;
        return *this;
    }
}VARDB_COUNT_STRU;
typedef struct _VARDB_CY_STRU:public VARDB_STRU
{
    char soltComp[32] ;//单电磁阀或双电磁阀
    int delay ;
    char basePoint[16] ;
    char movePoint[16] ;
    int timeOut ;
    char MsgMap[64] ;//气缸类信息输入
    _VARDB_CY_STRU(){
        memset(soltComp,0,32);
        memset(basePoint,0,16);
        memset(movePoint,0,16);
        memset(MsgMap,0,64);
        delay = -1;
        timeOut = -1;
    }
    _VARDB_CY_STRU &operator =(const _VARDB_CY_STRU & other){
         *((VARDB_STRU*)this) = other;
        strncpy(soltComp,other.soltComp,32);
        delay = other.delay;
        strncpy(basePoint,other.basePoint,16);
        strncpy(movePoint,other.movePoint,16);
        timeOut = other.timeOut;
        strncpy(MsgMap,other.MsgMap,64);
        return *this;
    }
}VARDB_CY_STRU;
typedef struct _VARDB_PPMOTCTR_STRU:public VARDB_STRU
{
    char childtype[16];
    int acc ;
    int dec ;
    int timeOut ;
    char axisidinfo[256] ;
    int delay ;
    _VARDB_PPMOTCTR_STRU(){
        acc = -1;
        dec = -1;
        timeOut = -1;
        delay = -1;
        memset(childtype,0,16);
        memset(axisidinfo,0,256);
    }
    _VARDB_PPMOTCTR_STRU &operator =(const _VARDB_PPMOTCTR_STRU &other){
         *((VARDB_STRU*)this) = other;
        strncpy(childtype,other.childtype,16);
        acc= other.acc;
        dec = other.dec;
        timeOut = other.timeOut;
        strncpy(axisidinfo,other.axisidinfo,256);
        delay = other.delay;
        return *this;
    }
}VARDB_PPMOTCTR_STRU;
//____________________________________________csp模式
typedef struct _VARDB_LCSP_STRU :public VARDB_PPMOTCTR_STRU
{
    char axisoffsetinfo[256] ="";
    _VARDB_LCSP_STRU(){
        memset(axisoffsetinfo,0,256);
    }
    _VARDB_LCSP_STRU  &operator = (const  _VARDB_LCSP_STRU  &other){
         *((VARDB_STRU*)this) = other;
        strncpy(axisoffsetinfo,other.axisoffsetinfo,256);
        return *this;
    }
}VARDB_LCSP_STRU;

/**************新增结构体方式**************************************************/
typedef struct _VARALLTYPEDBCONFIG_STRU//针对变量存储这样的
{
    QString name = "";
    QString type = "";
    QString childtype = "";
    int pointnum = -1;
    QString Initvalue = "";//初值
    QString upvalue = "0";//上限
    int addvalue = 1;
    QString lowvalue = "0";//下限
    QString note = "";
    QString startIO = "";
    QString setvalue = "";
    QString curvalue = 0;
    QString endIO = "";
    int     delay = -1;
    QString basePoint = "";
    QString movePoint = "";
    int timeOut = -1;
    int speed = -1;
    int acc = 0;
    int dec = 0;
    QString value = "";
    QString soltCompVec = 0;//单电磁阀或双电磁阀
    QString  CyMsgMap="";
    QString axisid ="";
    QString axisinfo="";
    QString remarkstr = "";
    QString formulaid ="type000";
    bool save = true;

}VARALLTYPEDBCONFIG_STRU;
typedef struct _VARALLDBCONFIG_STRU//
{
    QList<VARALLTYPEDBCONFIG_STRU> varlist;
    _VARALLDBCONFIG_STRU(){
        varlist.clear();
    }
}VARALLDBCONFIG_STRU;
typedef struct _VARCONFIG_STRU{
    QMap<QString,VARDB_REGTEXT_STRU>  regtextMap;//寄存器字符串型
    QMap<QString,VARDB_REGNUM_STRU> regnumMap;//寄存器数值型
    QMap<QString,VARDB_SWITCH_STRU> SwitchMap;//开关
    QMap<QString,VARDB_TIME_STRU> timeMap; //计时器
    QMap<QString,VARDB_COUNT_STRU> countMap;//计数器
    QMap<QString,VARDB_CY_STRU> cyMap;//气缸类
    QMap<QString,VARDB_PPMOTCTR_STRU> qpMap;//快速定位类
    QMap<QString,VARDB_LCSP_STRU> cspMap;//csp类
    _VARCONFIG_STRU(){
        regtextMap.clear();
        regnumMap.clear();
        SwitchMap.clear();
        timeMap.clear();
        countMap.clear();
        cyMap.clear();
        qpMap.clear();
        cspMap.clear();
    }
}VARCONFIG_STRU;//表示整张表格显示的数据信息
/*************生产信息表*********************************/
typedef struct _PROINFODBCONFIG_STRU:public VARDB_STRU
{
    int Demqua ;
    int TotalNum ;
}PROINFODBCONFIG_STRU;
typedef struct _PROINFOCONFIG_STRU{
    QMap<QString,PROINFODBCONFIG_STRU> proinfoMap;//以机种编号为主键信息
    _PROINFOCONFIG_STRU(){
        proinfoMap.clear();
    }
}PROINFOCONFIG_STRU;

/*************设备信息表*********************************/
typedef struct _DEVINFODBCONFIG_STRU:public VARDB_STRU
{
    char SoftVer[50]  ="V1.0";//软件版本
    char Recoder[50] = "";  //记录人员信息
}DEVINFODBCONFIG_STRU;
/*************参数信息表*********************************/
typedef struct _PARAMINFODBCONFIG_STRU:public VARDB_STRU
{
    char value[256] = "";  //参数值或者某一范围
    _PARAMINFODBCONFIG_STRU(){
        memset(value,0,256);
    }
    _PARAMINFODBCONFIG_STRU  &operator = (const  _PARAMINFODBCONFIG_STRU  &other){
         *((VARDB_STRU*)this) = other;
        strncpy(value,other.value,256);
        return *this;
    }
}PARAMINFODBCONFIG_STRU;
typedef struct _PARAMINFOCONFIG_STRU
{
   QMap<QString,PARAMINFODBCONFIG_STRU> paramainfoMap;
   _PARAMINFOCONFIG_STRU(){
       paramainfoMap.clear();
   }
}PARAMINFOCONFIG_STRU;
/*************报警信息表*********************************/
typedef struct _ALARMINFODBCONFIG_STRU:public VARDB_STRU
{

}ALARMINFODBCONFIG_STRU;
/*************良率表*********************************/
typedef struct _QUARATEINFODBCONFIG_STRU:public VARDB_STRU
{
    double QuaRate = 1.00;//良率值
}QUARATEINFODBCONFIG_STRU;
/*************实际参数表*********************************/
typedef struct _ACTPARAMINFODBCONFIG_STRU:public VARDB_STRU
{
    char value[256]  = "";//实际参数值
}ACTPARAMINFODBCONFIG_STRU;
/*************BOM信息表*********************************/
typedef struct _BOMINFODBCONFIG_STRU:public VARDB_STRU
{
    char name[50] = "";//名称
    char spec[50] = ""; //规格
    int quantities = 0;//数量
}BOMINFODBCONFIG_STRU;
/*************设备时间信息表*********************************/
typedef struct _DEVTIMEINFODBCONFIG_STRU:public VARDB_STRU
{
    char statim[50] = "";//开机时间
    char shudowtim[50] = ""; //关机时间
}DEVTIMEINFODBCONFIG_STRU;
/*************设备履历表*********************************/
typedef struct _DEVRESUMEINFODBCONFIG_STRU:public VARDB_STRU
{
    char recoder[50] = "";
}DEVRESUMEINFODBCONFIG_STRU;
/************* 设备稼动率表*********************************/
typedef struct _DEVUTILRATEINFODBCONFIG_STRU:public VARDB_STRU
{
    char Utirate[100] = "";
}DEVUTILRATEINFODBCONFIG_STRU;//Utilizatiorate

/*******设备的实时数据表***********************/
typedef struct _DEVREATIMINPUTINFODBCONFIG_STRU:public VARDB_STRU
{
    int  inputvalue = 0;
}DEVREATIMINPUTINFODBCONFIG_STRU;

typedef struct _DEVREATIMINPUTINFOCONFIG_STRU
{
    QMap<QString,DEVREATIMINPUTINFODBCONFIG_STRU> reatiminfomap;
    _DEVREATIMINPUTINFOCONFIG_STRU(){
        reatiminfomap.clear();
    }
}DEVREATIMINPUTINFOCONFIG_STRU;

#endif // VARIABLE_H
