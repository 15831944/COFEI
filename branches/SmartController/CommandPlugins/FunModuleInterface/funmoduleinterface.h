#ifndef FUNMODULEINTERFACE_H
#define FUNMODULEINTERFACE_H

#include <QtCore/QtPlugin>
class FunModuleInterface
{
public:
   virtual int add(int,int) = 0 ;
};
QT_BEGIN_NAMESPACE

#define IBasicModule_iid "com.twsz.tc.zhping.FunModuleInterface/1.0"

Q_DECLARE_INTERFACE(FunModuleInterface, IBasicModule_iid)

QT_END_NAMESPACE
#endif // FUNMODULEINTERFACE_H
