#ifndef MOVEPROPERTY_H
#define MOVEPROPERTY_H


#include <libproperybase.h>

class MoveProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(int ID READ getID WRITE setID )
private:
    int ID;
public:
    MoveProperty();

//    int getID() ;
//    void setID(int val) ;
};

#endif // MOVEPROPERTY_H
