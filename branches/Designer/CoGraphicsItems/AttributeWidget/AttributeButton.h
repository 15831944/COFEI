#ifndef ATTRIBUTEBUTTON_H
#define ATTRIBUTEBUTTON_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QTabWidget>
#include <QWidget>
#include "AttriBase.h"
#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"

class AttributeButton : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttributeButton(QWidget *parent = 0);
    ~AttributeButton();

private slots:
    void TabChanged(int);
    void IsPressModeChanged(int state);
    void TitleEditedFinished(const QString &text);
    void FontSizeChanged(const QString& text);
    void FontColorChanged(QColor color);
    void AlignVerChanged(int id);
    void AlignHorChanged(int id);
    void BGColorChanged(QColor color);
    void BGImagePathChanged();
    void BGIsSelectImage(bool state);
    void CheckStateChanged(int state);
    void SelectVar();

    void JsTacticChanged(const QString &text);
    void ScreenChanged(const QString &text);
    void SystemOpStateChanged(int);
    void VarChanged();
    void EnableExpreChanged(const QString &text);
    void EnableStateChanged(int id);

    void IsStateVar(const QString &var);
    void FreqChanged();
    void StateVarChanged(const QString &var);
    void StateBGColorChanged(QColor color);
    void StateBGIsSelectImage(bool state);
    void StateBGPixChanged(const QString);
    void StateFColorChanged(QColor color);
    void StateTextChanged(const QString &text);

private:
    virtual void SetItem(CoItemEx*);

    QWidget *CreateBaseAttri();
    QWidget *CreateOperateAttri();
    QWidget *CreateStateAttri();


private:
    QTabWidget      *m_pTabWidget;
    QString         m_strVar;
    QCheckBox       *m_pCheckBoxIsPressMode;

    TextAttri       *m_pTextAttri;
    FontAttri       *m_pFontAttri;
    BackgroundAttri *m_pLiftedBackGroundAttri;
    BackgroundAttri *m_pPressedBackGroundAttri;
    EnableAttri     *m_pEnableAttri;

    //操作属性
    JsTacticAttri   *m_pLiftedJSAttri;
    JsTacticAttri   *m_pPressedJSAttri;
    QCheckBox       *m_pChkIsSwitchScreen;
    QCheckBox       *m_pChkIsUseOpFunc;
    QCheckBox       *m_pChkIsUseVar;

    QComboBox       *m_pCmbScreen;
    QComboBox       *m_pCmbSystemOpState;
    QComboBox       *m_pCmbVarState;
    QLineEdit       *m_pLineEditVarName;
    QPushButton     *m_pBtnSelectVar;

    //状态属性
    TextAttri       *m_pZeroTextAttri;
    TextAttri       *m_pOneTextAttri;
    QTabWidget      *m_pTabState;
    VarSelectedAttri *m_pStateVar;
    BackgroundAttri *m_pZeroStateBGAttri;
    BackgroundAttri *m_pOneStateBGAttri;
    ColorSelect     *m_pZeroFontColor;
    ColorSelect     *m_pOneFontColor;
    QLineEdit       *m_pLineEditFreq;
};
#endif // ATTRIBUTEBUTTON_H
