#ifndef COUNTPROPERTY_H
#define COUNTPROPERTY_H

#include <QObject>
#include "../libproperybase.h"

class CountProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )
    Q_PROPERTY(QString note READ getNote WRITE setNote)
public:
    CountProperty();
    void sendval(QString val ="");
    virtual QString getID();
    virtual void setID(QString Idval);

    virtual QString getNote();
    virtual void setNote(QString note_1);
};

#endif // COUNTPROPERTY_H
