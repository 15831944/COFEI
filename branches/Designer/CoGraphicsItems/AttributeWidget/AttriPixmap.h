#ifndef ATTRIPIXMAP_H
#define ATTRIPIXMAP_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"
#include "../AttributeWidget.h"

class AttriPixmap : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriPixmap(QWidget *parent = 0);
    ~AttriPixmap(){}

private slots:
    void SelectImg();
    void ImgChanged();
    void FreqChanged();
    void ZPlaceChange(int value);
    void IsStateVar(const QString &var);
    void StateVarChanged(const QString &var);
    void StateExpreChanged(const QString &var);
    void StatePicChanged();

private:
    QWidget *CreateAttri();
    QWidget *CreateStateAttri();
    void SetItem(CoItemEx*);

private:
    //基础属性
    QLineEdit *m_pLineEditImg;
    QLineEdit *m_pLineEditFreq;
    QPushButton *m_pBtnSelectImg;
    TopBottom *m_pTBAttri;

    //状态属性
    VarSelectedAttri *m_pVarAttr;
    ExpreSelectedAttri *m_pExpreAttri;
    QLineEdit *m_pZeroLineEdit;
    QPushButton *m_pZeroPicBtn;
    QLineEdit *m_pOneLineEdit;
    QPushButton *m_pOnePicBtn;
    QGroupBox *m_pPicGrp;
};

#endif // AttriPixmap_H
