#ifndef TEACHDEVICERUNLIB_H
#define TEACHDEVICERUNLIB_H
#include <QObject>
#include "teachdevicerunlib_global.h"
#include <MyGlobal.h>
#include <QMap>
using namespace CoreGlobal;
class TEACHDEVICERUNLIBSHARED_EXPORT TeachDeviceRunLib :public QObject
{
    Q_OBJECT
public:
    TeachDeviceRunLib();
    ~TeachDeviceRunLib();
    void SetRunParam(QVector<QString> jointnames,int speedrate){
        m_jointnames = jointnames;
        m_speedrate = speedrate;
    }
    void InitJointsBytePosParam(QVector<QString> jointnames);//初始化得到所有轴的字节位置参数
    int  SetJointS_ON(QVector<QString> jointnames);//设置轴励磁
    void SetJointS_OFF(QVector<QString> jointnames);//设置轴松励磁
    void SetJointReset(QVector<QString> jointnames);//设置轴复位
    void RunJoints();//设置轴运行
    void Stop();
    bool GetJointCurPos(QString jointname,int &curpos);//得到轴的当前位置
    bool SetJointTraPos(QString jointname,int Trapos);//设置轴目标位置参数
    void SetJointModel(QString jointname,QString modeltype);
    void SetJointsModel(QVector<QString> jointnames,QString modeltype);
    int  CheckJointFault(QString jointname,int &errorcode);
private:
    uchar* m_tempdata;
    //励磁
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOffOP;
    //控制字输出
    QMap<QString,int> m_OutConByteMap;
    QMap<QString,int> m_OutTraPosByteMap;
    QMap<QString,int> m_OutModelByteMap;
    QMap<QString,int> m_OutTraSpeedByteMap;
    QMap<QString,int> m_OutAccByteMap;
    QMap<QString,int> m_OutDecByteMap;
    QMap<QString,QMap<QString,BITINFO_STRU>> m_OutConBitInfoMap;
    //状态字输入
    QMap<QString,int> m_InStatByteMap;
    QMap<QString,int> m_InCurPosByteMap;
    QMap<QString,int> m_InCurModelByteMap;
    QMap<QString,int> m_InErrorByteMap;
private:
    QVector<QString> m_jointnames;
    int m_speedrate;
    QString m_EnableIO;
private:
    void GetIOAddress();
    void InitJointS_ONParam();
    void GetJointConTrolwordBytePos(QString jointname);
    void GetJointStatuswordBytePos(QString jointname);
    bool CheckRunStateError();
};

#endif // TEACHDEVICERUNLIB_H
