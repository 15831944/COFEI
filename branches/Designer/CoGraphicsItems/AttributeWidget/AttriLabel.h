#ifndef ATTRILABEL_H
#define ATTRILABEL_H

#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"

class AttriLabel : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriLabel(QWidget *parent = 0);
    ~AttriLabel();

private slots:
    void TabChanged(int);
    void OutputModeChanged(int);
    void SelectVar();
    void ValueTypeChanged(int);
    void TypeChanged(int);
    void RuleChanged(int);
    void FrontZeroCheckChanged(int);

    void TitleEditedFinished(const QString &text);
    void FontSizeChanged(const QString& text);
    void FontColorChanged(QColor color);
    void AlignVerChanged(int id);
    void AlignHorChanged(int id);
    void BGColorChanged(QColor color);
    void BGImagePathChanged();
    void BGIsSelectImage(bool state);

    void OutputModeChangedExt(int);
    void VarChanged();
    void UnitChanged();
    void ValueTypeChangedExt(int);
    void ValueFormatChanged();
    void IntAccurecyChanged(const QString&);
    void DecAccurecyChanged(const QString&);
    void FreqChanged();
    void JsTacticChanged(const QString &text);

private:
    virtual void SetItem(CoItemEx*);

    QWidget *CreateBaseAttri();
    QWidget *CreateDisplyAttri();

    uchar GetCurValueFormat();
    void SetWidgetsByEnum(int);

private:
    QString         m_strVar;
    TextAttri       *m_pTextAttri;
    FontAttri       *m_pFontAttri;
    BackgroundAttri *m_pBGAttri;

    QWidget         *m_pDisplayAttri;
    QTabWidget      *m_pTabWidgetAtrri;
    QCheckBox       *m_pChkOutputMode;

    //显示输出属性
    QLineEdit       *m_pLineEditExpre;
    QPushButton     *m_pBtnSelectVarExt;

    QLineEdit       *m_pLineEditUnit;

    QGroupBox       *m_pGrpValueType;
    QButtonGroup    *m_pValueTypeBtnGroup;
    QRadioButton    *m_pRdoNumber;
    QRadioButton    *m_pRdoString;

    QGroupBox       *m_pGrpType;
    QButtonGroup    *m_pTypeBtnGroup;
    QCheckBox       *m_pChkDouble;
    QCheckBox       *m_pChkNatural;

    QButtonGroup    *m_pRuleBtnGroup;
    QRadioButton    *m_pRdoDec;
    QRadioButton    *m_pRdoHex;
    QRadioButton    *m_pRdoBin;

    QCheckBox       *m_pChkRound;
    QCheckBox       *m_pChkFrontZero;
    QCheckBox       *m_pChkPwd;

    QSpinBox        *m_pSpnInt;
    QSpinBox        *m_pSpnDecimal;
    QLineEdit       *m_pLineEditFreq;

    JsTacticAttri   *m_pJSAttri;

};

#endif // ATTRILABEL_H
