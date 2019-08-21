#ifndef ATTRIGPOUTPUT_H
#define ATTRIGPOUTPUT_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QTabWidget>
#include <QWidget>
#include "AttriBase.h"
#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"

class AttriGPOutPut : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriGPOutPut(QWidget *parent = 0);
    ~AttriGPOutPut(){}

private slots:
    void TitleEditedFinished(const QString &text);
    void FontSizeChanged(const QString& text);
    void FontColorChanged(QColor color);
    void AlignVerChanged(int id);
    void AlignHorChanged(int id);
    void BGColorChanged(QColor color);
    void BGImagePathChanged();
    void BGIsSelectImage(bool state);

    void CheckStateChanged(int state);
    void JsTacticChanged(const QString &text);
    void IOChanged();
    void EnableExpreChanged(const QString &text);
    void EnableStateChanged(int id);

private:
    virtual void SetItem(CoItemEx*);

    QWidget *CreateBaseAttri();
    QWidget *CreateOperateAttri();


private:
    TextAttri       *m_pTextAttri;
    FontAttri       *m_pFontAttri;
    BackgroundAttri *m_pLiftedBackGroundAttri;
    BackgroundAttri *m_pPressedBackGroundAttri;
    EnableAttri     *m_pEnableAttri;

    //操作属性
    JsTacticAttri   *m_pLiftedJSAttri;
    JsTacticAttri   *m_pPressedJSAttri;
    QCheckBox       *m_pChkIsUseIO;

    QComboBox       *m_pCmbIOState;
    QComboBox       *m_pCmbIOName;
};

#endif // ATTRIGPOUTPUT_H
