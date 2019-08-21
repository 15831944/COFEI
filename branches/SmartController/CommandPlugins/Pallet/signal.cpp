#include "pallet.h"
#include <QDebug>
#include <MyGlobal.h>

bool Pallets::ResetBySignal(_PalletInfo &info)
{
    bool isReset = false;
    if (getResetSignal(info))
    {
        reset(info);
        isReset = true;
    }

    return isReset;
}

//check reset signal
bool Pallets::getResetSignal(_PalletInfo &info)
{
    QString bitName = info.Reset.bit;
    int value = info.Reset.value;
    uchar v;
    bool isOk = false;

    if (1 == CoreGlobal::BaseCalcFun::Analysis_L(bitName, v) && v == value)
    {
        qDebug() << "get reset signal" << endl;
        isOk = true;
    }

    return isOk;
}

//if this floor is end, send a signal
bool Pallets::setThisFloorIsEndSignal(_PalletInfo &info)
{
    QString bitName = info.floorIsEnd.bit;
    uchar value = info.floorIsEnd.value;
    bool isOk = false;

    if (!CoreGlobal::BaseCalcFun::Analysis_L(bitName, value))
    {
        qDebug() << "set this floor is end signal success." << endl;
        isOk = true;
    }

    return isOk;
}

//if all floors of this pallet is end, send a signal
bool Pallets::setThisPalletIsEndSignal(_PalletInfo &info)
{
    QString bitName = info.palletIsEnd.bit;
    uchar value = info.palletIsEnd.value;
    bool isOk = false;

    if (!CoreGlobal::BaseCalcFun::Analysis_L(bitName, value))
    {
        qDebug() << "set this pallet is end signal success." << endl;
        isOk = true;
    }

    return isOk;
}
