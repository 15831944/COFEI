#ifndef ATTRITEXTEDIT_H
#define ATTRITEXTEDIT_H

#include <QTabWidget>
#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"

class AttriTextEdit : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriTextEdit(QWidget *parent = 0);
    ~AttriTextEdit(){}
    enum ValueType
    {
        Double = 0,
        FixedPoint, //自然小数位:保留固定的4位小数点
        FrontZero,
        Round,
        RoundAndFrontZero,
        String,
        Pwd
    };

private slots:
    void TabChanged(int);
    void PowerChanged(int);
    void FontSizeChanged(const QString& text);
    void FontColorChanged(QColor color);
    void AlignVerChanged(int id);
    void AlignHorChanged(int id);
    void BGColorChanged(QColor color);
    void BGImagePathChanged();
    void BGIsSelectImage(bool state);

    void VarChanged(const QString& varInfo);
    void UnitChanged();
    void ValueFormatChanged();
    void IntAccurecyChanged(const QString&);
    void DecAccurecyChanged(const QString&);

    void MaxValChanged();
    void MinValChanged();

    void EnableExpreChanged(const QString &text);
    void EnableStateChanged(int id);

    void FixedPointCheckChanged(int);
    void FrontZeroCheckChanged(int);
    void SetWidgetsByVar(const QString&);
    void JsTacticChanged(const QString &text);
    void InputVarChanged(const QString&varInfo);
    void OutputVarChanged(const QString&varInfo);
    void FreqChanged();

private:
    virtual void SetItem(CoItemEx*);

    QWidget *CreateBaseAttri();
    QWidget *CreateOperateAttri();
    uchar GetCurValueFormat();
    void SetWidgetsByEnum(int);
    void SetWidgetsByValueType(int);

private:
    FontAttri        *m_pFontAttri;
    BackgroundAttri  *m_pBGAttri;
    EnableAttri      *m_pEnableAttri;
    VarSelectedAttri *m_pInVarSelectAttri;
    VarSelectedAttri *m_pOutVarSelectAttri;
    JsTacticAttri    *m_pJSAttri;

    QTabWidget       *m_pTabWidgetAtrri;
    QLineEdit        *m_pLineEditUnit;
    QCheckBox        *m_pChkFixedPoint;
    QCheckBox        *m_pChkFrontZero;
    QCheckBox        *m_pChkRound;
    QCheckBox        *m_pChkPwd;

    QSpinBox         *m_pSpnInt;
    QSpinBox         *m_pSpnDecimal;

    QLineEdit        *m_pLineEditMax;
    QLineEdit        *m_pLineEditMin;
    QLineEdit        *m_pLineEditFreq;
    char m_VarType;
};

#endif // ATTRITEXTEDIT_H
