#ifndef ATTRIGPINPUT_H
#define ATTRIGPINPUT_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "../AttributeWidget.h"

class AttriGPInPut : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriGPInPut(QWidget *parent = 0);
    ~AttriGPInPut(){}

private slots:
    void IONameChanged(const QString);
    void RefreshFrequencyChanged(const QString&);

private:
    QWidget *CreateAttri();
    virtual void SetItem(CoItemEx*);

private:
    QComboBox       *m_pCmbIOName;
    QComboBox       *m_pCmbFrequency;
};

#endif // ATTRIGPINPUT_H
