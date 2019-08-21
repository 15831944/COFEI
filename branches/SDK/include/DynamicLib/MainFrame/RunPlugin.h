#ifndef RUNPLUGIN_H
#define RUNPLUGIN_H
#include <funmoduleinterface.h>

class RunPlugin
{
public:
    RunPlugin();
public:
    static void Exe(const QString , QString , int &, const FLOWTXT_STRU ,FunModuleInterface *);
};

#endif // RUNPLUGIN_H
