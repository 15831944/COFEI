#ifndef ATTRIGROUPBOX_H
#define ATTRIGROUPBOX_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "../AttributeWidget.h"

class AttriGroupBox : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriGroupBox(QWidget *parent = 0);
    ~AttriGroupBox(){}

private:
    Q_SLOT void TextChanged();

    QWidget *CreateAttri();
    void SetItem(CoItemEx*);

private:
    QLineEdit *m_pLineEditTxt;

};

#endif // AttriGroupBox_H
