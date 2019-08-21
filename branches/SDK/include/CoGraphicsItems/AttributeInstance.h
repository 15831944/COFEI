#ifndef ATTRIBUTE_INSTANCE_H
#define ATTRIBUTE_INSTANCE_H
#include <QStringList>
#include <QMap>
#include <QMutex>

class AttributeInstance
{
private:
    AttributeInstance()
    {
        canvasMap.clear();
        flowMap.clear();
        Power = 3;
    }

public:
    static AttributeInstance *GetInstance()
    {
        static AttributeInstance pInstance;
        return &pInstance;
    }
    QMap<QString,QStringList> canvasMap;
    QMap<QString,QStringList> flowMap;
    int Power;
};

#endif
