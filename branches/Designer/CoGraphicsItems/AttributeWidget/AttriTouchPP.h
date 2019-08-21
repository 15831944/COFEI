#ifndef ATTRITOUCHPP_H
#define ATTRITOUCHPP_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "../AttributeWidget.h"

class AttriTouchPP : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriTouchPP(QWidget *parent = 0);
    ~AttriTouchPP(){}

private:
    virtual void SetItem(CoItemEx*);
};

#endif // ATTRITOUCHPP_H
