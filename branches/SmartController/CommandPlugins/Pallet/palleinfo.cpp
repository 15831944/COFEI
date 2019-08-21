#ifndef PALLETINFO_CPP
#define PALLETINFO_CPP

#include "palletinfo.h"
#include <MyGlobal.h>
#include <QDebug>

Pallets::Pallets()
{
    qDebug() << "constructor in .........pallets" << endl;
    Init();
    CalcaulateAllPoints();
    ShowMsg();
}

Pallets::~Pallets()
{

}

//因为需要用到软件xml中的信息，所以需要先加载xml，再加载模块
void Pallets::Init()
{
    qDebug() << "start in init " << endl;

    palletsInfo = new QMap<QString, _PalletInfo>;

    for (auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_PalletInfo.begin();
         item != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_PalletInfo.end(); item++)
    {
        QString palletName = item.key();
        _PalletInfo _pallet;

        //创建保存托盘位置信息的文件
        _pallet.ps = new positionStore(palletName);

        _pallet.row = item.value().palletDimension["row"];
        _pallet.column = item.value().palletDimension["column"];
        _pallet.floor = item.value().palletDimension["floor"];

        _pallet.moveOrientation = item.value().palletMode["moveOrientation"];

        _pallet._axisParams.increaseModeParams.speed = item.value().runParams["Increase"].speed;
        _pallet._axisParams.increaseModeParams.acc = item.value().runParams["Increase"].acc;
        _pallet._axisParams.increaseModeParams.dec = item.value().runParams["Increase"].dec;
        _pallet._axisParams.increaseModeParams.timeOut = item.value().runParams["Increase"].timeout;
        _pallet._axisParams.increaseModeParams.delayMsec = item.value().runParams["Increase"].delay;

        _pallet._axisParams.locateModeParams.speed = item.value().runParams["Locate"].speed;
        _pallet._axisParams.locateModeParams.acc = item.value().runParams["Locate"].acc;
        _pallet._axisParams.locateModeParams.dec = item.value().runParams["Locate"].dec;
        _pallet._axisParams.locateModeParams.timeOut = item.value().runParams["Locate"].timeout;
        _pallet._axisParams.locateModeParams.delayMsec = item.value().runParams["Locate"].delay;

        _pallet.arr = new myArrary;

        //x, y, z方向和轴的对应关系
        _pallet.xyzToAxis = &(item.value().coordinateToAxis);

        for (auto x = item.value().palletCheckpoint.begin(); x != item.value().palletCheckpoint.end(); x++)
            qDebug() << x.key() << ": " << x.value() << endl;

        GetOrientationNumber(_pallet);
        GetNameOfCheckPoint(_pallet, item.value().palletCheckpoint);
        GetValueOfCheckPoint(_pallet);

        //轴信息
        _pallet._ps.axisMap = new QMap<QString, AxisParams>;

        //设置起始位置
        //从备份文件中读取上一次的位置
        if (!getPreviousPosition(_pallet))
        {
            qDebug() << "get position from file fail" << endl;
            _pallet.currentPosition.x = 0;
            _pallet.currentPosition.y = 0;
            _pallet.currentPosition.z = 0;
        }

        //读取托盘结束信号配置
        _pallet.floorIsEnd.bit = item.value().endSignal["thisFloorIsEnd"].bit;
        _pallet.floorIsEnd.value = item.value().endSignal["thisFloorIsEnd"].value;
        _pallet.palletIsEnd.bit = item.value().endSignal["allFloorIsEnd"].bit;
        _pallet.palletIsEnd.value = item.value().endSignal["allFloorIsEnd"].value;

        //读取托盘重置信号配置
        _pallet.Reset.bit = item.value().resetSignal["resetSignal"].bit;
        _pallet.Reset.value = item.value().resetSignal["resetSignal"].value;

        if (!(palletsInfo->contains(palletName)))
        {
            palletsInfo->insert(palletName, _pallet);
        }
        else {
            qDebug() << "重复的托盘名" << endl;
        }
    }

    qDebug() << "end in init" << endl;
}

int Pallets::GetNameOfCheckPoint(_PalletInfo &info, QMap<QString, QString> &m)
{

    info.leftTop.pointName = m["leftTop"];
    info.rightTop.pointName = m["rightTop"];
    info.leftBottom.pointName = m["leftBottom"];
    info.rightBottom.pointName = m["rightBottom"];
    info.last_leftTop.pointName = m["last_leftTop"];
    info.endPosition.pointName = m["endPosition"];

    return 0;
}

int Pallets::GetOrientationNumber(_PalletInfo &info)
{
    info.orientationNum = 0;

    if (info.xyzToAxis->contains("X"))
        info.orientationNum++;
    if (info.xyzToAxis->contains("Y"))
        info.orientationNum++;
    if (info.xyzToAxis->contains("Z"))
        info.orientationNum++;
    qDebug() << "orientation number: " << info.orientationNum << endl;

    return 0;
}

int Pallets::GetValueOfCheckPoint(_PalletInfo &info)
{
    QString x_Axis, y_Axis, z_Axis;
    x_Axis = "";
    if (info.xyzToAxis->contains("X"))
        x_Axis = info.xyzToAxis->value("X");
    y_Axis = "";
    if (info.xyzToAxis->contains("Y"))
        y_Axis = info.xyzToAxis->value("Y");
    z_Axis = "";
    if (info.xyzToAxis->contains("Z"))
        z_Axis = info.xyzToAxis->value("Z");

    qDebug() << x_Axis << "; " << y_Axis << "; " << z_Axis << endl;

//    for (auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.begin();
//         item != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.end(); item ++)
    {
        //从快速定位部分获取校准点信息
        //if ("快速定位" == item.key())
        {
            //auto it = item.value();
            auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
            QString pn[6] = {info.leftTop.pointName, info.rightTop.pointName, info.leftBottom.pointName,
                             info.rightBottom.pointName, info.last_leftTop.pointName, info.endPosition.pointName};
            _checkPoint *p[6] = {&info.leftTop, &info.rightTop, &info.leftBottom, &info.rightBottom, &info.last_leftTop, &info.endPosition};

            for (int i = 0; i < 6; i++)
            {
                if (it.contains(pn[i]))
                {
                    if (x_Axis != "")
                        p[i]->p.x = it[pn[i]].axisMap[x_Axis].pos;
                    if (y_Axis != "")
                        p[i]->p.y = it[pn[i]].axisMap[y_Axis].pos;
                    if (z_Axis != "")
                        p[i]->p.z = it[pn[i]].axisMap[z_Axis].pos;
                }
                qDebug() << p[i]->p.x << "; " << p[i]->p.y << "; " << p[i]->p.z << endl;
            }
        }
    }

    return 0;
}

//计算所有点的坐标位置，并保存在一个三维数组中
void Pallets::CalcaulateAllPoints()
{
    qDebug() << "start in cal" << endl;

    for (auto item = palletsInfo->begin(); item != palletsInfo->end(); item++)
    {
        _PalletInfo p_info = item.value();

        (p_info.arr)->Create(p_info.row, p_info.column, p_info.floor);

        if (p_info.column >= 1)
            if (1 == p_info.column)
                p_info.xStep = p_info.rightTop.p.x - p_info.leftTop.p.x;
            else p_info.xStep = (p_info.rightTop.p.x - p_info.leftTop.p.x) / (p_info.column - 1);
        else qDebug() << "Invalid column" << endl;


        if (p_info.row >= 1)
            if (1 == p_info.row)
                p_info.yStep = p_info.leftBottom.p.y - p_info.leftTop.p.y;
            else p_info.yStep = (p_info.leftBottom.p.y - p_info.leftTop.p.y) / (p_info.row -1);
        else qDebug() << "Invalid row" << endl;

        if (p_info.floor >= 1)
            if (1 == p_info.floor)
                p_info.zStep = p_info.last_leftTop.p.z - p_info.leftTop.p.z;
            else p_info.zStep = (p_info.last_leftTop.p.z - p_info.leftTop.p.z) / (p_info.floor - 1);
        else qDebug() << "Invalid floor" << endl;

        for (int i = 0; i < p_info.row; i++)
            for (int j = 0; j < p_info.column; j++)
                for (int k = 0; k < p_info.floor; k++)
                    if (0 == k)
                    {
                        if (0 == i && 0 == j)
                            p_info.arr->array[i][j][k] = p_info.leftTop.p;
                        else
                            if (0 == j)
                            {
                                p_info.arr->array[i][j][k].x = p_info.arr->array[i-1][j][k].x;
                                p_info.arr->array[i][j][k].y = p_info.arr->array[i-1][j][k].y + p_info.yStep;
                                p_info.arr->array[i][j][k].z = p_info.arr->array[i-1][j][k].z;
                            }
                            else
                                if (0 == i)
                                {
                                    p_info.arr->array[i][j][k].x = p_info.arr->array[i][j-1][k].x + p_info.xStep;
                                    p_info.arr->array[i][j][k].y = p_info.arr->array[i][j-1][k].y;
                                    p_info.arr->array[i][j][k].z = p_info.arr->array[i][j-1][k].z;
                                }
                                else {
                                    p_info.arr->array[i][j][k].x = p_info.arr->array[i-1][j-1][k].x + p_info.xStep;
                                    p_info.arr->array[i][j][k].y = p_info.arr->array[i-1][j-1][k].y + p_info.yStep;
                                    p_info.arr->array[i][j][k].z = p_info.arr->array[i-1][j-1][k].z;
                                }
                    }
                    else {
                        p_info.arr->array[i][j][k].x = p_info.arr->array[i][j][k-1].x;
                        p_info.arr->array[i][j][k].y = p_info.arr->array[i][j][k-1].y;
                        p_info.arr->array[i][j][k].z = p_info.arr->array[i][j][k-1].z + p_info.zStep;
                    }
    }

    qDebug() << "end in cal" << endl;
}

//return -1, 表示没有找到对应的托盘名
//return -2, get position fail
int Pallets::AnalyzeInstruct(QString _palletName, QString _instructParams)
{
    int value = 0;

    if (!palletsInfo->contains(_palletName))
        value = -1;
    else if (_instructParams.contains("(") && _instructParams.contains(")"))    //指令中有圆括号带入的参数，表示是指定位置的模式
        (*palletsInfo)[_palletName].captureMode = CaptureMode_Locate;
    else
        (*palletsInfo)[_palletName].captureMode = CaptureMode_Increase; //指令参数中没有圆括号，表示是递增的模式

    SetAxisParams(_palletName);
    value = GetNextPosition(_palletName, _instructParams);
    ConvertPositionToCoordinate(_palletName);

    return value;
}

int Pallets::GetNextPosition(QString _palletName, QString _instructParams)
{
    _PalletInfo &pal = (*palletsInfo)[_palletName];
    int value = 0;

    //to do better, reset after move to next position
    if (ResetBySignal(pal))
        return value;

    if (CaptureMode_Increase == pal.captureMode)
        value = IncreaseMode_GetNextPosition(_palletName, _instructParams);
    else if(CaptureMode_Locate == pal.captureMode)
        value = LocateMode_GetNextPosition(_palletName, _instructParams);

    return value;
}

int Pallets::IncreaseMode_GetNextPosition(QString _palletName, QString _instructParams)
{
    _PalletInfo &p_info = (*palletsInfo)[_palletName];
    p_info.increaseStep = _instructParams.toInt();

    int _row = p_info.row;
    int _column = p_info.column;
    int _floor = p_info.floor;

    _Position &_nextPosition = p_info.nextPosition;
    _nextPosition = p_info.currentPosition;
    int &_step = p_info.increaseStep;

    if (MoveOrientation_Row == (p_info.moveOrientation))
    {
        if (0 == _nextPosition.y) // at home or not
            _nextPosition.x = _nextPosition.z = 1;

        int y = _nextPosition.y += _step;
        !(y % _column) ? (_nextPosition.y = _column) : (_nextPosition.y %= _column);

        int x = _nextPosition.x;
        if (y > _column) // row increase or not
        {
            x = ++_nextPosition.x;
            x = _nextPosition.x += (y-_column) / (_column+1);
        }

        !(x % _row) ? (_nextPosition.x = _row) : (_nextPosition.x %= _row);

        int z = _nextPosition.z;
        if (x > _row) // floor increase or not
        {
            z++;
            z += (x-_row) / (_row+1);
        }

        while (z != _nextPosition.z)
        {
            if (z > _floor) //
            {
                setThisPalletIsEndSignal(p_info); // 设置托盘结束信号
                reset(p_info); // 定义(0, 0, 0)点为托盘全部结束后的停留点

                break;
            }
            else if (z > _nextPosition.z)
            {
                y = _nextPosition.y = _step;
                !(y % _column) ? (_nextPosition.y = _column) : (_nextPosition.y %= _column);

                x  = _nextPosition.x = 1;
                if (y > _column)
                {
                    x = ++_nextPosition.x;
                    x = _nextPosition.x += (y-_column) / (_column+1);
                }
                !(x % _row) ? (_nextPosition.x = _row) : (_nextPosition.x %= _row);

                z = _nextPosition.z;
                if (x > _row)
                {
                    z++;
                    z += (x-_row) / (_row+1);
                }

                //设置本层结束信号
                setThisFloorIsEndSignal(p_info);
            }
        }
    }
    else if (MoveOrientation_Column == (p_info.moveOrientation))
    {
        if (0 == _nextPosition.x)
            _nextPosition.y = _nextPosition.z = 1;

        int x = _nextPosition.x += _step;
        !(x % _row) ? (_nextPosition.x = _row) : (_nextPosition.x = x%_row);

        int y = _nextPosition.y;
        if (x > _row)
        {
            y = ++_nextPosition.y;
            y = _nextPosition.y += (x-_row) / (_row+1);
        }
        !(y % _column) ? (_nextPosition.y = _column) : (_nextPosition.y = y % _column);

        int z = _nextPosition.z;
        if (y > _column)
        {
            z++;
            z += (y-_column) / (_column+1);
        }

        while (z != _nextPosition.z)
        {
            if (z > _floor)
            {
                //设置托盘结束信号
                setThisPalletIsEndSignal(p_info);

                //定义(0, 0, 0)点为托盘全部结束后的停留点
                reset(p_info);

                break;
            }
            else if (z > _nextPosition.z)
            {
                x = _nextPosition.x = _step;
                !(x % _row) ? (_nextPosition.x = _row) : (_nextPosition.x %= _row);

                y = _nextPosition.y = 1;
                if (x > _row)
                {
                    y = ++_nextPosition.y;
                    y = _nextPosition.y += (x-_row) / (_row+1);
                }
                !(y % _column) ? (_nextPosition.y = _column) : (_nextPosition.y %= _column);

                z = _nextPosition.z;
                if (y > _column)
                {
                    z++;
                    z += (y-_column) / (_column+1);
                }
            }
        }
    }

    //to do better
    //set current position after axis move
    p_info.currentPosition = _nextPosition;
    qDebug() << "x,y,z: " << p_info.currentPosition.x << ", " << p_info .currentPosition.y << ", " << p_info.currentPosition.z << endl;
    qDebug() << "home: " << p_info.endPosition.p.x << ", " << p_info.endPosition.p.y << ", " << p_info.endPosition.p.z << endl;

    //to do better
    //saveCurrentPosition(p_info);

    return 0;
}

//return 0 - ok
//return -2 - false
int Pallets::LocateMode_GetNextPosition(QString _palletName, QString _instructParams)
{
    bool ok;
    int value = 0;

    //to do better
    QStringList strList = _instructParams.split(";");
    QString str1 = strList.at(0);
    str1 = str1.replace("(", "");
    QString str2 = strList.at(1);
    QString str3 = strList.at(2);
    str3 = str3.replace(")", "");

    int x = str1.toInt(&ok, 10);
    if ( !ok )
        value = -2;

    int y = str2.toInt(&ok, 10);
    if ( !ok )
        value = -2;

    int z = str3.toInt(&ok, 10);
    if ( !ok )
        value = -2;

    _PalletInfo &p_info = (*palletsInfo)[_palletName];
    _Position &_nextPosition = p_info.nextPosition;
    _nextPosition.x = x;
    _nextPosition.y = y;
    _nextPosition.z = z;

//    //(0;0;0)表示重置整个托盘，并回到Home位置
//    if ( 0 == x && 0 == y && 0 == z)
//    {
//        p_info.currentPosition.x = p_info.currentPosition.y = p_info.currentPosition.z = 0;
//        saveCurrentPosition(p_info);
//    }
    p_info.currentPosition = _nextPosition;
    return value;
}

//根据运动模式是递增还是指定位置，设置轴的运动参数
void Pallets::SetAxisParams(QString _palletName)
{
    _PalletInfo &p_info = (*palletsInfo)[_palletName];

    if (CaptureMode_Increase == p_info.captureMode)
    {
        p_info._ps.speed = p_info._axisParams.increaseModeParams.speed;
        p_info._ps.acc = p_info._axisParams.increaseModeParams.acc;
        p_info._ps.dec = p_info._axisParams.increaseModeParams.dec;
        p_info._ps.timeOut = p_info._axisParams.increaseModeParams.timeOut;
        p_info._ps.delayMsec = p_info._axisParams.increaseModeParams.delayMsec;
    }
    else if (p_info.captureMode == CaptureMode_Locate)
    {
        p_info._ps.speed = p_info._axisParams.locateModeParams.speed;
        p_info._ps.acc = p_info._axisParams.locateModeParams.acc;
        p_info._ps.dec = p_info._axisParams.locateModeParams.dec;
        p_info._ps.timeOut = p_info._axisParams.locateModeParams.timeOut;
        p_info._ps.delayMsec = p_info._axisParams.locateModeParams.delayMsec;
    }
}

void Pallets::ConvertPositionToCoordinate(QString _palletName)
{
    _PalletInfo &p_info = (*palletsInfo)[_palletName];

    int _x = p_info.nextPosition.x;
    int _y = p_info.nextPosition.y;
    int _z = p_info.nextPosition.z;

    _Coordinate _coordinate;


    qDebug() << "endPosition: " << p_info.endPosition.p.x << ", " << p_info.endPosition.p.y << ", " << p_info.endPosition.p.z << endl;

    if ( 0 == _x && 0 == _y && 0 == _z)
    {
        _coordinate.x = p_info.endPosition.p.x;
        _coordinate.y = p_info.endPosition.p.y;
        _coordinate.z = p_info.endPosition.p.z;
    }
    else
    {
        _coordinate.x = p_info.arr->array[_x-1][_y-1][_z-1].x;
        _coordinate.y = p_info.arr->array[_x-1][_y-1][_z-1].y;
        _coordinate.z = p_info.arr->array[_x-1][_y-1][_z-1].z;
    }

    qDebug() << "(" << _x << ", " << _y << ", " << _z << "): "
             << "(" << _coordinate.x << ", " << _coordinate.y << ", " << _coordinate.z << ")" << endl;

    SetAxisTarget(_palletName, _coordinate);
}

void Pallets::SetAxisTarget(QString _palletName, _Coordinate target)
{
    _PalletInfo p_info = (*palletsInfo)[_palletName];

    QString x_Axis = (*p_info.xyzToAxis)["X"];
    QString y_Axis = (*p_info.xyzToAxis)["Y"];
    QString z_Axis = (*p_info.xyzToAxis)["Z"];
    qDebug() << "x_Axis: " << x_Axis << "; y_Axis: " << y_Axis << "; z_Aixs: " << z_Axis << endl;

    AxisParams z_Axis_pos, x_Axis_pos, y_Axis_pos;

    x_Axis_pos.pos = target.x;
    y_Axis_pos.pos = target.y;
    z_Axis_pos.pos = target.z;

    x_Axis_pos.speed = y_Axis_pos.speed = z_Axis_pos.speed = p_info._ps.speed;

    if (x_Axis != "") {
        if ( !p_info._ps.axisMap->contains(x_Axis) )
            p_info._ps.axisMap->insert(x_Axis, x_Axis_pos);
        else (*p_info._ps.axisMap)[x_Axis] = x_Axis_pos;
    }

    if (y_Axis != "") {
        if ( !p_info._ps.axisMap->contains(y_Axis) )
            p_info._ps.axisMap->insert(y_Axis, y_Axis_pos);
        else (*p_info._ps.axisMap)[y_Axis] = y_Axis_pos;
    }

    if (z_Axis != "") {
        if ( !p_info._ps.axisMap->contains(z_Axis) )
            p_info._ps.axisMap->insert(z_Axis, z_Axis_pos);
        else (*p_info._ps.axisMap)[z_Axis] = z_Axis_pos;
    }
}

//reset to home position
void Pallets::reset(_PalletInfo &info)
{
    info.currentPosition.x = info.currentPosition.y = info.currentPosition.z = 0;

    info.nextPosition.x = info.nextPosition.y = info.nextPosition.z = 0;
}

bool Pallets::saveCurrentPosition(_PalletInfo &info)
{
    return info.ps->SavePosition(info.currentPosition.x, info.currentPosition.y, info.currentPosition.z);
}

bool Pallets::getPreviousPosition(_PalletInfo &info)
{
    int x, y, z;
    if (!(info.ps->GetPosition(x, y, z)))
        return false;

    info.currentPosition.x = x;
    info.currentPosition.y = y;
    info.currentPosition.z = z;
    info.nextPosition = info.currentPosition;
    qDebug() << "current position: "
             << "x: " << info.currentPosition.x
             << "y: " << info.currentPosition.y
             << "z: " << info.currentPosition.z
             << endl;

    return true;
}

#endif //PALLETINFO_CPP
