#ifndef ATTRICURVE_H
#define ATTRICURVE_H

#include <QSpinBox>
#include <QRadioButton>
#include <QTabWidget>
#include <QWidget>
#include <QDoubleValidator>
#include "AttriBase.h"
#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"

class AttriCurve : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriCurve(QWidget *parent = 0);
    ~AttriCurve(){}
    enum Grid
    {
        XAxisGrid = 0,
        XAxisSubGrid,
        YAxisGrid,
        YAxisSubGrid
    };
    Q_ENUMS(Grid)

private:
    virtual void SetItem(CoItemEx*);
    int          GetStringToEnumEx(QString str);

    QWidget     *CreateBaseAttri();
    QWidget     *CreateCurveAttri();

    Q_SLOT void ValidatorChanged(int);
    Q_SLOT void TitleChanged();
    Q_SLOT void AxisLblTxtChanged();
    Q_SLOT void XAxisCountChanged(int);
    Q_SLOT void YAxisCountChanged(int);
    Q_SLOT void GridColorChanged(QColor color);
    Q_SLOT void BGColorChanged(QColor color);
    Q_SLOT void GridVisiableStateChanged(int state);
    Q_SLOT void AxisLblColorChanged(QColor color);
    Q_SLOT void TimeFormatChanged(const QString&);
    Q_SLOT void UnitChanged();
    Q_SLOT void XAxisLengthChanged();
    Q_SLOT void DecAccurecyChanged(int);
    Q_SLOT void MaxValueChanged();
    Q_SLOT void MinValueChanged();
    Q_SLOT void FreqChanged();
    Q_SLOT void CtrlVarNameChanged(const QString&varInfo);
    Q_SLOT void VarNameChanged(const QString&varInfo);
    Q_SLOT void CurveColorChanged(QColor color);

private:
    QLineEdit   *m_pLineEditTitle;
    QSpinBox    *m_pSpnXAxisCount;
    ColorSelect *m_pXAxisGridColor;
    ColorSelect *m_pXAxisSubGridColor;

    QSpinBox    *m_pSpnYAxisCount;
    ColorSelect *m_pYAxisGridColor;
    ColorSelect *m_pYAxisSubGridColor;

    ColorSelect *m_pBGColor;
    QCheckBox   *m_pChkGridVisible;

    QLineEdit   *m_pLineEditXAxisLbl;
    ColorSelect *m_pXAxisLblColor;
    QComboBox   *m_pCmbTimeFormat;
    QLineEdit   *m_pLineEditUnit;
    QLineEdit   *m_pLineEditXMax;

    QLineEdit   *m_pLineEditYAxisLbl;
    ColorSelect *m_pYAxisLblColor;
    QSpinBox    *m_pSpnDecimal;
    QLineEdit   *m_pLineEditMax;
    QDoubleValidator *m_pDVMax;
    QLineEdit   *m_pLineEditMin;
    QDoubleValidator *m_pDVMin;

    QLineEdit   *m_pLineEditFreq;
    VarSelectedAttri *m_pControlVar;
    VarSelectedAttri *m_pCurveVar;
    ColorSelect *m_pCurveColor;
};
#endif // ATTRICURVE_H
