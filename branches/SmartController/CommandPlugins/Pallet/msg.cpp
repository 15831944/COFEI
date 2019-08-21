#include "pallet.h"
#include <QDebug>

void Pallets::ShowMsg()
{
    qDebug() << "start show msg" << endl;

    for (auto item = palletsInfo->begin(); item != palletsInfo->end(); item++)
    {
        qDebug() << "****************************************************************" << endl;
        qDebug() << "Name: " << item.key() << endl;

        _PalletInfo p_info = item.value();
        qDebug() << "行数: " << p_info.row << "; " << "列数: " << p_info.column << "; " << "层数: " << p_info.floor << "; " << endl;
        qDebug() << "LeftTop: (" << p_info.leftTop.p.x << ", " << p_info.leftTop.p.y << ", " << p_info.leftTop.p.z << ")" << endl
                 << "RightTop: (" << p_info.rightTop.p.x << ", " << p_info.rightTop.p.y << ", " << p_info.rightTop.p.z << ")" << endl
                 << "LeftBottom: (" << p_info.leftBottom.p.x << ", " << p_info.leftBottom.p.y << ", " << p_info.leftBottom.p.z << ")" << endl
                 << "RightBottom: (" << p_info.rightBottom.p.x << ", " << p_info.rightBottom.p.y << ", " << p_info.rightBottom.p.z << ")" << endl
                 << "p5: (" << p_info.last_leftTop.p.x << ", " << p_info.last_leftTop.p.y << ", " << p_info.last_leftTop.p.z << ")" << endl
                 << "home: (" << p_info.endPosition.p.x << ", " << p_info.endPosition.p.y << ", " << p_info.endPosition.p.z << ")" << endl;


        qDebug() << "current position: " << item.value().currentPosition.x << ", "
                 << item.value().currentPosition.y << ", " << item.value().currentPosition.z << endl;

        QString str = "Unknown Mode";
        if (MoveOrientation_Row == p_info.moveOrientation)
            str = "orientation: row";
        else if (MoveOrientation_Column == p_info.moveOrientation)
            str = "orientation: column";
        qDebug() << str << endl;

        qDebug() << "Increase Mode: " << endl
                 << "speed: " << p_info._axisParams.increaseModeParams.speed << "; "
                 << "acc: " << p_info._axisParams.increaseModeParams.acc << "; "
                 << "dec: " << p_info._axisParams.increaseModeParams.dec << "; "
                 << "timeout: " << p_info._axisParams.increaseModeParams.timeOut << "; "
                 << "delay: " << p_info._axisParams.increaseModeParams.delayMsec << endl;

        qDebug() << "Locate Mode: " << endl
                 << "speed: " << p_info._axisParams.locateModeParams.speed << "; "
                 << "acc: " << p_info._axisParams.locateModeParams.acc << "; "
                 << "dec: " << p_info._axisParams.locateModeParams.dec << "; "
                 << "timeout: " << p_info._axisParams.locateModeParams.timeOut << "; "
                 << "delay: " << p_info._axisParams.locateModeParams.delayMsec << endl;


        qDebug() << "All points: " << endl;
        for (int k = 0; k < p_info.floor; k++)
            for (int i = 0; i < p_info.row; i++)
                for (int j = 0; j < p_info.column; j++)
                    qDebug() << "[" << i+1 << ", " << j+1 << ", " << k+1 << "]: ("
                             << p_info.arr->array[i][j][k].x << ", "
                             << p_info.arr->array[i][j][k].y << ", "
                             << p_info.arr->array[i][j][k].z << ")"
                             << endl;

        qDebug() << "End Signal Settings: " << endl
                 << "\tThis floor is end: (" << p_info.floorIsEnd.bit << ", " << p_info.floorIsEnd.value << ")" << endl
                 << "\tThis pallet is end: (" << p_info.palletIsEnd.bit << ", " << p_info.palletIsEnd.value << ")" << endl;

        qDebug() << "Reset Signal Setting: (" << p_info.Reset.bit << ", " << p_info.Reset.value << ")" << endl;

        qDebug() << "****************************************************************" << endl;
    }

    qDebug() << "end show msg" << endl;
}
