#ifndef ATTRIWARMLAMP_H
#define ATTRIWARMLAMP_H

#include <QWidget>
#include "AttriBase.h"
#include "CoGraphics/Base/Base.h"
#include "../AttributeWidget.h"

class AttriWarmLamp : public GraphicAttrWidget
{
    Q_OBJECT

public:
    explicit AttriWarmLamp(QWidget *parent = 0);
    ~AttriWarmLamp(){}

private slots:
    void VarChanged(const QString &);

private:
    QWidget *CreateAttri();
    virtual void SetItem(CoItemEx*);

private:
    VarSelectedAttri *m_pRed;
    VarSelectedAttri *m_pYellow;
    VarSelectedAttri *m_pGreen;
    VarSelectedAttri *m_pBuzzer;
};

#endif // ATTRIWARMLAMP_H
