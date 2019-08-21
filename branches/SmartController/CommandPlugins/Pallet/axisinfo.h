#ifndef AXISINFO_H
#define AXISINFO_H

#include <QString>

struct Params
{
    int speed = -1;
    int acc = -1;
    int dec = -1;
    int timeOut = -1;
    int delayMsec = -1;
};

struct AxisCommonParams
{
    Params increaseModeParams;
    Params locateModeParams;
};

struct AxisParams
{
    int speed = -1;
    int acc = -1;
    int dec = -1;
    int timeOut = -1;
    int delayMsec = -1;
    double pos  = -1.0;
    double offsetPos = 0.0;
};

//struct AxisInfo
//{
//    AxisCommonParams commonParams;
//    QMap<QString, AxisEachParams> eachParams;
//};

/************************************/

struct Pallet_AxisInfo
{
    //托盘名称
    QString id = "";

    QString note = "";

    int axisNum = -1;

    int speed = -1;
    int acc = -1;
    int dec = -1;
    int timeOut = -1;
    int delayMsec = -1;
    //key是X,Y或Ｚ
    //value是对应的轴信息
    QMap<QString, AxisParams> *axisMap;
    Pallet_AxisInfo()
    {
//        axisMap.clear();
//        axisMap = new QMap<QString, AxisParams>;
    }
};

#endif // AXISINFO_H
