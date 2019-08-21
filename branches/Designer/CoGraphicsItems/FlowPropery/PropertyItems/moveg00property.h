#ifndef MOVEG00PROPERTY_H
#define MOVEG00PROPERTY_H
#include "../libproperybase.h"
#include"FlowPropery/PropertyBrowser/pointset.h"
class MoveG00Property : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(PointSet pointName READ getPointName WRITE setPointName)

public:
    MoveG00Property();
     virtual QString getXmlString() Q_DECL_OVERRIDE;





private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // MOVEG00PROPERTY_H
