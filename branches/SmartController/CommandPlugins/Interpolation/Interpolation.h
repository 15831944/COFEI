#ifndef ARCINTERPOLATION_1_H
#define ARCINTERPOLATION_1_H

#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include "motion/ServoControl.h"
#include "Comm.h"
#include "trajectories.h"
#include "motion.h"
#include <iostream>
#include <string>
#include <vector>
#include "typedef.h"

using std::string;
using std::vector;

class ArcInterpolation_1 : public QObject, public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)

public:
    ArcInterpolation_1();

    virtual void Initialization();
    virtual void SetModuleParam(SWCONFIGXML_STRU ps);
    virtual void RunModule();
    virtual int  GetExcResult(QString &strMsg);
    virtual void StopModule();
     virtual FunModuleInterface* Copy();

    virtual QString GetModuleName()
    {
        return m_moduleName;
    }

private:
    QString m_moduleName;

    Motion * motion;
    Trajectories * trajectories;

    bool isOk;
    QString msg;
};

#endif // ARCINTERPOLATION_1_H
