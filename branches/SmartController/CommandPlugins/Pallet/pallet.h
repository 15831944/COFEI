#ifndef PALLET_H
#define PALLET_H

#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include "palletinfo.h"
#include "axisinfo.h"
#include "motioncontrol.h"
#include <MyGlobal.h>

using namespace CoreGlobal;

class Pallet : public QObject, public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid)

    Q_INTERFACES(FunModuleInterface)

public:
    Pallet();

public:
    virtual void Initialization();
    virtual void SetModuleParam(SWCONFIGXML_STRU ps);
    virtual void RunModule();
    virtual int  GetExcResult(QString &strMsg);
    virtual void StopModule();
    virtual FunModuleInterface* Copy();

public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }

private:
    QString m_moduleName;
    Pallets *pal;
    Pallet_AxisInfo _ps;
    MotionControl *m_motion;
};

#endif // PALLET_H
