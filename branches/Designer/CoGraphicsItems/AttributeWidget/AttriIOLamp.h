#ifndef ATTRIIOLAMP_H
#define ATTRIIOLAMP_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"
#include "../AttributeWidget.h"

class AttriIOLamp : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriIOLamp(QWidget *parent = 0);
    ~AttriIOLamp(){}

private slots:
    void VarChanged();
    void SelectVar();
    void RefreshFrequencyChanged(const QString&);
    void ExpreChanged(const QString &text);

private:
    QWidget *CreateAttri();
    virtual void SetItem(CoItemEx*);

private:
    QString         m_strVar;
    QLineEdit       *m_pLineEditVar;
    QPushButton     *m_pBtnSelectVar;
    QComboBox       *m_pCmbFrequency;
    ExpreSelectedAttri *m_pExpreAttri;  //表达式选择
};

#endif // ATTRIIOLAMP_H
