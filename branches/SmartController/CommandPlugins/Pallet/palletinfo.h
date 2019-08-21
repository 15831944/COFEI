#ifndef PALLETINFO_H
#define PALLETINFO_H

#define CaptureMode_Increase 1
#define CaptureMode_Locate 2

#define MoveOrientation_Row 1
#define MoveOrientation_Column 2

#define MoveMode_PP 1
#define MoveMode_CSP 2

#include "axisinfo.h"
#include "positionstore.h"

//以行，列，层数表示的位置
struct _Position
{
    int x, y, z;
};

//以绝对坐标表示的位置
struct _Coordinate
{
    double x, y, z;
};

//信号配置
struct _signalConfig
{
    QString bit;
    int value;
};

struct _checkPoint
{
    QString pointName;
    _Coordinate p;
};

//数组，存储所有点的位置
//索引为点的行，列，层数坐标；对应的值为点的绝对位置坐标
class myArrary
{
public:
    int r, c, f;

    _Coordinate ***array;
    bool created;

    //delete
    void Dispose();

    //Return value: 0 - ok, -1 - error
    int Create(int Row, int Column, int Floor);
};

//一个托盘的信息
struct _PalletInfo
{
    //托盘的行，列,层数
    int row, column, floor;

    //5个校准点的位置
    _checkPoint leftTop, rightTop, leftBottom, rightBottom;
    _checkPoint last_leftTop;
    _checkPoint endPosition;

    //x, y和ｚ轴方向的步长
    double xStep, yStep, zStep;

    //抓取模式，递增 or 指定位置
    int captureMode;

    //递增模式下的步长
    int increaseStep;

    //移动方向，按行 or 按列
    int moveOrientation;

    //运动模式，pp or csp
    int moveMode;

    AxisCommonParams _axisParams;

    //有几个方向的运动
    int orientationNum;

    //x, y, z方向和轴的对应关系
    QMap<QString, QString> *xyzToAxis;

    Pallet_AxisInfo _ps;

    //array, store all points
    myArrary *arr;

    _Position currentPosition;
    _Position nextPosition;

    _signalConfig floorIsEnd, palletIsEnd, Reset;

    positionStore *ps;
};

class Pallets
{
private:
    Pallets();
    ~Pallets();

public:
    static Pallets *GetInstance()
    {
        static Pallets p;
        return &p;
    }

    void Init();
    void CalcaulateAllPoints();
    void ShowMsg();

    int GetNameOfCheckPoint(_PalletInfo &info, QMap<QString, QString> &m);
    int GetValueOfCheckPoint(_PalletInfo &info);
    int GetOrientationNumber(_PalletInfo &info);

    int AnalyzeInstruct(QString _palletName, QString _instructParams);
    int GetNextPosition(QString _palletName, QString _instructParams);
    int IncreaseMode_GetNextPosition(QString _palletName, QString _instructParams);
    int LocateMode_GetNextPosition(QString _palletName, QString _instructParams);
    void SetAxisParams(QString _palletName);

    void ConvertPositionToCoordinate(QString _palletName);
    void SetAxisTarget(QString palletName, _Coordinate target);

    bool ResetBySignal(_PalletInfo &info);
    bool getResetSignal(_PalletInfo &info);
    void reset(_PalletInfo &info);

    bool setThisFloorIsEndSignal(_PalletInfo &info);
    bool setThisPalletIsEndSignal(_PalletInfo &info);

    bool saveCurrentPosition(_PalletInfo &info);
    bool getPreviousPosition(_PalletInfo &info);

public:
    //所有托盘的信息
    //key - xml中配置的托盘的名称; value - 该托盘的信息
    QMap<QString, _PalletInfo> *palletsInfo;

    QString current_pallet_id;  // save current pallet id, to do better
};

#endif // PALLETINFO_H
