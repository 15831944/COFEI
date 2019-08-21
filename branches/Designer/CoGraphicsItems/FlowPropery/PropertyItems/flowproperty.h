#ifndef FLOWPROPERTY_H
#define FLOWPROPERTY_H
#include "../libproperybase.h"
#include <QString>

class FlowProperty : public LibProperyBase
{
    Q_OBJECT
public:
    FlowProperty();
    void setID(int val) ;
};

#endif // FLOWPROPERTY_H
