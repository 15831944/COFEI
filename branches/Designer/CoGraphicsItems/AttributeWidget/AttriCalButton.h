#ifndef ATTRICALBUTTON_H
#define ATTRICALBUTTON_H

#include <QTabWidget>
#include <QWidget>
#include "AttriBase.h"
#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"

class AttriCalButton : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriCalButton(QWidget *parent = 0);
    ~AttriCalButton(){}

private slots:
    void TabChanged(int);
    void TitleEditedFinished(const QString &text);
    void FontSizeChanged(const QString& text);
    void FontColorChanged(QColor color);
    void AlignVerChanged(int id);
    void AlignHorChanged(int id);
    void BGColorChanged(QColor color);
    void BGImagePathChanged();
    void BGIsSelectImage(bool state);

    void CalVarChanged(const QString& varInfo);
    void JsTacticChanged(const QString &text);
    void EnableExpreChanged(const QString &text);
    void EnableStateChanged(int id);

private:
    virtual void SetItem(CoItemEx*);

    QWidget *CreateBaseAttri();
    QWidget *CreateOperateAttri();

    VarSelectedAttri *GetInVarAttriById(int id);
    VarSelectedAttri *GetOutVarAttriById(int id);

private:
    QTabWidget      *m_pTabWidget;
    TextAttri       *m_pTextAttri;
    FontAttri       *m_pFontAttri;
    BackgroundAttri *m_pLiftedBackGroundAttri;
    BackgroundAttri *m_pPressedBackGroundAttri;
    EnableAttri     *m_pEnableAttri;

    VarSelectedAttri *m_pInVar1;
    VarSelectedAttri *m_pInVar2;
    VarSelectedAttri *m_pInVar3;
    VarSelectedAttri *m_pInVar4;
    VarSelectedAttri *m_pInVar5;
    VarSelectedAttri *m_pInVar6;

    VarSelectedAttri *m_pOutVar1;
    VarSelectedAttri *m_pOutVar2;
    VarSelectedAttri *m_pOutVar3;
    VarSelectedAttri *m_pOutVar4;
    VarSelectedAttri *m_pOutVar5;
    VarSelectedAttri *m_pOutVar6;
    JsTacticAttri   *m_pLiftedJSAttri;
    JsTacticAttri   *m_pPressedJSAttri;
};

#endif // ATTRICALBUTTON_H
