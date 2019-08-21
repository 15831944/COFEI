#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include "TBasicControls/CalButton.h"
#include "AttriCalButton.h"

AttriCalButton::AttriCalButton(QWidget *parent)
    : GraphicAttrWidget(parent)
{
    m_pTabWidget = new QTabWidget();
    connect(m_pTabWidget,&QTabWidget::currentChanged,this,&AttriCalButton::TabChanged);
    m_pTabWidget->setFixedWidth(260);
    QWidget *pBaseWidget = CreateBaseAttri();
    QWidget *pOperateWidget = CreateOperateAttri();
    m_pEnableAttri = AttriBase::enableAttri();
    m_pEnableAttri->SetButtonText(tr("按钮"));
    m_pEnableAttri->SetSelectVarType(tr("Num+Switch+String"));

    m_pTabWidget->addTab(pBaseWidget,tr("基本属性"));
    m_pTabWidget->addTab(pOperateWidget,tr("操作属性"));
    m_pTabWidget->addTab(m_pEnableAttri,tr("可用属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(m_pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

void AttriCalButton::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TCalButton *calbtn = qobject_cast<TCalButton*>(itm);
    m_pTabWidget->setCurrentIndex(calbtn->GetAttriIndex());
    m_pTextAttri->SetText(calbtn->GetText());
    m_pFontAttri->SetFontSize(QString::number(calbtn->GetFontSize()));
    m_pFontAttri->SetFontColor(calbtn->GetFontColor());
    m_pFontAttri->SetAligHor(calbtn->GetFontAlignHor());
    m_pFontAttri->SetAlignVer(calbtn->GetFontAlignVer());
    QVector<QColor> backColorVec;
    backColorVec.clear();
    backColorVec = calbtn->GetBackColor();
    m_pLiftedBackGroundAttri->SetColor(backColorVec[0]);
    m_pPressedBackGroundAttri->SetColor(backColorVec[1]);

    QVector<bool> selectPixVec = calbtn->GetSelectPix();
    m_pLiftedBackGroundAttri->SetImageCheckState(selectPixVec[0]);
    m_pPressedBackGroundAttri->SetImageCheckState(selectPixVec[1]);

    QVector<QString> pixFileVec = calbtn->GetPixFiles();
    m_pLiftedBackGroundAttri->SetImage(pixFileVec[0]);
    m_pPressedBackGroundAttri->SetImage(pixFileVec[1]);

    QStringList list;
    list.clear();
    if(!calbtn->GetCalVarComb1().isEmpty())
    {
        list = calbtn->GetCalVarComb1().split(",");
        if(list.size() > 3)
        {
            m_pInVar1->SetVarName(list.at(0));
            m_pOutVar1->SetVarName(list.at(2));
        }
    }

    if(!calbtn->GetCalVarComb2().isEmpty())
    {
        list = calbtn->GetCalVarComb2().split(",");
        if(list.size() > 3)
        {
            m_pInVar2->SetVarName(list.at(0));
            m_pOutVar2->SetVarName(list.at(2));
        }
    }

    if(!calbtn->GetCalVarComb3().isEmpty())
    {
        list = calbtn->GetCalVarComb3().split(",");
        if(list.size() > 3)
        {
            m_pInVar3->SetVarName(list.at(0));
            m_pOutVar3->SetVarName(list.at(2));
        }
    }
    if(!calbtn->GetCalVarComb4().isEmpty())
    {
        list = calbtn->GetCalVarComb4().split(",");
        if(list.size() > 3)
        {
            m_pInVar4->SetVarName(list.at(0));
            m_pOutVar4->SetVarName(list.at(2));
        }
    }
    if(!calbtn->GetCalVarComb5().isEmpty())
    {
        list = calbtn->GetCalVarComb5().split(",");
        if(list.size() > 3)
        {
            m_pInVar5->SetVarName(list.at(0));
            m_pOutVar5->SetVarName(list.at(2));
        }
    }

    if(!calbtn->GetCalVarComb6().isEmpty())
    {
        list = calbtn->GetCalVarComb6().split(",");
        if(list.size() > 3)
        {
            m_pInVar6->SetVarName(list.at(0));
            m_pOutVar6->SetVarName(list.at(2));
        }
    }

    //JS
    QVector<QString> jsVec = calbtn->GetJsTactic();
    m_pLiftedJSAttri->SetJsTactic(jsVec[0]);
    m_pPressedJSAttri->SetJsTactic(jsVec[1]);

    m_pEnableAttri->SetEnableExpre(calbtn->GetEnableExpre());
    m_pEnableAttri->SetEnableState(calbtn->GetEnableState());

    connect(m_pTextAttri,&TextAttri::SendTextFinfished,this,&AttriCalButton::TitleEditedFinished);
    connect(m_pFontAttri,&FontAttri::FontSizeChanged,this,&AttriCalButton::FontSizeChanged);
    connect(m_pFontAttri,&FontAttri::FontColorChanged,this,&AttriCalButton::FontColorChanged);
    connect(m_pFontAttri,&FontAttri::AlignHorChanged,this,&AttriCalButton::AlignHorChanged);
    connect(m_pFontAttri,&FontAttri::AlignVerChanged,this,&AttriCalButton::AlignVerChanged);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriCalButton::BGColorChanged);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriCalButton::BGIsSelectImage);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttriCalButton::BGImagePathChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriCalButton::BGColorChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriCalButton::BGIsSelectImage);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttriCalButton::BGImagePathChanged);

    connect(m_pInVar1,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pInVar2,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pInVar3,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pInVar4,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pInVar5,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pInVar6,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar1,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar2,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar3,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar4,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar5,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pOutVar6,&VarSelectedAttri::SendTextFinfished,this,&AttriCalButton::CalVarChanged);
    connect(m_pLiftedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriCalButton::JsTacticChanged);
    connect(m_pPressedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriCalButton::JsTacticChanged);
    connect(m_pEnableAttri,&EnableAttri::SendTextFinfished,this,&AttriCalButton::EnableExpreChanged);
    connect(m_pEnableAttri,&EnableAttri::EnableStateChanged,this,&AttriCalButton::EnableStateChanged);

    connect(this,&AttriCalButton::SendTAttribute,calbtn,&TCalButton::SetAttributeEvent);
}

QWidget *AttriCalButton::CreateBaseAttri()
{
    QWidget *pBaseAttriWidget = new QWidget();

    QVBoxLayout *pTopLayout = new QVBoxLayout();
    m_pTextAttri = AttriBase::textAttri();
    m_pTextAttri->setContentsMargins(-8,-8,-8,-8);
    pTopLayout->addWidget(m_pTextAttri);

    QGroupBox *pGroupFontAttri = new QGroupBox(tr("字体"));
    QVBoxLayout *pFontLayout = new QVBoxLayout();
    pGroupFontAttri->setLayout(pFontLayout);
    m_pFontAttri = AttriBase::fontAttri();
    m_pFontAttri->setContentsMargins(-9,-8,-9,-8);
    pFontLayout->addWidget(m_pFontAttri);

    //背景
    QGroupBox *pGroupBackground = new QGroupBox(tr("背景"));
    pGroupBackground->setContentsMargins(5,10,5,10);
    QTabWidget *pTabWidgetBackground = new QTabWidget();
    m_pLiftedBackGroundAttri = AttriBase::backgroundAttri();
    m_pLiftedBackGroundAttri->setObjectName(tr("LiftedBackground"));
    m_pPressedBackGroundAttri = AttriBase::backgroundAttri();
    m_pPressedBackGroundAttri->setObjectName(tr("PressedBackground"));
    pTabWidgetBackground->addTab(m_pLiftedBackGroundAttri,tr("抬起"));
    pTabWidgetBackground->addTab(m_pPressedBackGroundAttri,tr("按下"));

    QVBoxLayout *pBackgroundLayout = new QVBoxLayout();
    pBackgroundLayout->addWidget(pTabWidgetBackground);
    pGroupBackground->setLayout(pBackgroundLayout);

    QVBoxLayout *pBaseAtrriLayout = new QVBoxLayout();
    pBaseAtrriLayout->addLayout(m_pGridLayout);
    pBaseAtrriLayout->addLayout(pTopLayout);
    pBaseAtrriLayout->addWidget(pGroupFontAttri);
    pBaseAtrriLayout->addWidget(pGroupBackground);
    pBaseAtrriLayout->setAlignment(Qt::AlignTop);

    pBaseAttriWidget->setLayout(pBaseAtrriLayout);
    return pBaseAttriWidget;
}

QWidget *AttriCalButton::CreateOperateAttri()
{
    QWidget *pOperateAttriWidget = new QWidget();
    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pOperateAttriWidget->setLayout(pVBoxLayout);

    m_pInVar1 = AttriBase::varSelectedAttri();
    m_pInVar1->SetSelectVarType(tr(""));
    m_pInVar1->SetIsRealtimeVar(true);
    m_pInVar2 = AttriBase::varSelectedAttri();
    m_pInVar2->SetSelectVarType(tr(""));
    m_pInVar2->SetIsRealtimeVar(true);
    m_pInVar3 = AttriBase::varSelectedAttri();
    m_pInVar3->SetSelectVarType(tr(""));
    m_pInVar3->SetIsRealtimeVar(true);
    m_pInVar4 = AttriBase::varSelectedAttri();
    m_pInVar4->SetSelectVarType(tr(""));
    m_pInVar4->SetIsRealtimeVar(true);
    m_pInVar5 = AttriBase::varSelectedAttri();
    m_pInVar5->SetSelectVarType(tr(""));
    m_pInVar5->SetIsRealtimeVar(true);
    m_pInVar6 = AttriBase::varSelectedAttri();
    m_pInVar6->SetSelectVarType(tr(""));
    m_pInVar6->SetIsRealtimeVar(true);
    m_pInVar1->setObjectName(tr("In_1"));
    m_pInVar2->setObjectName(tr("In_2"));
    m_pInVar3->setObjectName(tr("In_3"));
    m_pInVar4->setObjectName(tr("In_4"));
    m_pInVar5->setObjectName(tr("In_5"));
    m_pInVar6->setObjectName(tr("In_6"));

    m_pOutVar1 = AttriBase::varSelectedAttri();
    m_pOutVar1->SetSelectVarType(tr("Num"));
    m_pOutVar2 = AttriBase::varSelectedAttri();
    m_pOutVar2->SetSelectVarType(tr("Num"));
    m_pOutVar3 = AttriBase::varSelectedAttri();
    m_pOutVar3->SetSelectVarType(tr("Num"));
    m_pOutVar4 = AttriBase::varSelectedAttri();
    m_pOutVar4->SetSelectVarType(tr("Num"));
    m_pOutVar5 = AttriBase::varSelectedAttri();
    m_pOutVar5->SetSelectVarType(tr("Num"));
    m_pOutVar6 = AttriBase::varSelectedAttri();
    m_pOutVar6->SetSelectVarType(tr("Num"));
    m_pOutVar1->setObjectName(tr("Out_1"));
    m_pOutVar2->setObjectName(tr("Out_2"));
    m_pOutVar3->setObjectName(tr("Out_3"));
    m_pOutVar4->setObjectName(tr("Out_4"));
    m_pOutVar5->setObjectName(tr("Out_5"));
    m_pOutVar6->setObjectName(tr("Out_6"));

    QGridLayout *pVarLayout = new QGridLayout();
    pVarLayout->setHorizontalSpacing(2);
    pVarLayout->setVerticalSpacing(10);

    pVBoxLayout->addLayout(pVarLayout);
    pVarLayout->addWidget(new QLabel(tr("读")),0,0);
    pVarLayout->addWidget(m_pInVar1,0,1);
    pVarLayout->addWidget(new QLabel(tr("写")),0,2);
    pVarLayout->addWidget(m_pOutVar1,0,3);
    pVarLayout->addWidget(new QLabel(tr("读")),1,0);
    pVarLayout->addWidget(m_pInVar2,1,1);
    pVarLayout->addWidget(new QLabel(tr("写")),1,2);
    pVarLayout->addWidget(m_pOutVar2,1,3);
    pVarLayout->addWidget(new QLabel(tr("读")),2,0);
    pVarLayout->addWidget(m_pInVar3,2,1);
    pVarLayout->addWidget(new QLabel(tr("写")),2,2);
    pVarLayout->addWidget(m_pOutVar3,2,3);
    pVarLayout->addWidget(new QLabel(tr("读")),3,0);
    pVarLayout->addWidget(m_pInVar4,3,1);
    pVarLayout->addWidget(new QLabel(tr("写")),3,2);
    pVarLayout->addWidget(m_pOutVar4,3,3);
    pVarLayout->addWidget(new QLabel(tr("读")),4,0);
    pVarLayout->addWidget(m_pInVar5,4,1);
    pVarLayout->addWidget(new QLabel(tr("写")),4,2);
    pVarLayout->addWidget(m_pOutVar5,4,3);
    pVarLayout->addWidget(new QLabel(tr("读")),5,0);
    pVarLayout->addWidget(m_pInVar6,5,1);
    pVarLayout->addWidget(new QLabel(tr("写")),5,2);
    pVarLayout->addWidget(m_pOutVar6,5,3);

    //操作属性
    QGroupBox *pGroupJS = new QGroupBox(tr("脚本策略"));
    pVBoxLayout->addWidget(pGroupJS);
    QTabWidget *pTabWidgetJS = new QTabWidget();
    pTabWidgetJS->setFixedHeight(110);
    m_pLiftedJSAttri = AttriBase::jsTacticAttri();
    m_pLiftedJSAttri->setObjectName(tr("LiftedJSTactic"));
    m_pPressedJSAttri = AttriBase::jsTacticAttri();
    m_pPressedJSAttri->setObjectName(tr("PressedJSTactic"));
    pTabWidgetJS->addTab(m_pLiftedJSAttri,tr("抬起"));
    pTabWidgetJS->addTab(m_pPressedJSAttri,tr("按下"));

    QHBoxLayout *pJSLayout = new QHBoxLayout();
    pJSLayout->addWidget(pTabWidgetJS);
    pGroupJS->setLayout(pJSLayout);

    pVBoxLayout->addStretch();
    return pOperateAttriWidget;
}

void AttriCalButton::TitleEditedFinished(const QString &text)
{
     emit SendTAttribute(CoAttributeWidget::Text,text,0);
}

void AttriCalButton::FontSizeChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::FontSize,text,0);
}

void AttriCalButton::FontColorChanged(QColor color)
{
    QString strColor = color.name(QColor::HexArgb);
    emit SendTAttribute(CoAttributeWidget::FontColor,strColor,0);
}

void AttriCalButton::AlignVerChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignVertical,QString::number(id),0);
}

void AttriCalButton::AlignHorChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignHorizontal,QString::number(id),0);
}

void AttriCalButton::BGColorChanged(QColor color)
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    QString strColor = color.name(QColor::HexArgb);
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("PressedBackground"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }

    emit SendTAttribute(CoAttributeWidget::BGColor,strColor,uc);
}

void AttriCalButton::BGImagePathChanged()
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("PressedBackground"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::PixMap,pBGWidget->GetImage(),uc);
}

void AttriCalButton::BGIsSelectImage(bool state)
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());

    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("PressedBackground"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    //是否使用图
    QString strSelectPix = "";
    if(state)
    {
        strSelectPix = "1";
    }

    emit SendTAttribute(CoAttributeWidget::SelectPix,strSelectPix,uc);
}

void AttriCalButton::CalVarChanged(const QString &varInfo)
{
    QString strObjName = sender()->objectName();
    QStringList list = strObjName.split("_");
    QString str = "";
    if(list.size() > 1)
    {
        int id = list.at(1).toInt();
        if(list.at(0) == tr("In"))
        {
            VarSelectedAttri *pVSA = GetOutVarAttriById(id);
            if(pVSA->GetVarName().isEmpty())
                return;
            QString outVarInfo = tr("%1,%2").arg(pVSA->GetVarName()).arg(QString::number(pVSA->GetVarType()));
            str = varInfo + "," + outVarInfo;
        }
        else
        {
            VarSelectedAttri *pVSA = GetInVarAttriById(id);
            if(pVSA->GetVarName().isEmpty())
                return;
            QString inVarInfo = tr("%1,%2").arg(pVSA->GetVarName()).arg(QString::number(pVSA->GetVarType()));
            str = inVarInfo + "," + varInfo;
        }
        emit SendTAttribute(CoAttributeWidget::CalVar,str,id);
    }
}

void AttriCalButton::JsTacticChanged(const QString &text)
{
    uchar uc = 0;
    JsTacticAttri *pJS = qobject_cast<JsTacticAttri*>(sender());
    if(pJS->objectName() == "PressedJSTactic")
        uc = 1;
    emit SendTAttribute(CoAttributeWidget::JsTactic,text,uc);
}

void AttriCalButton::EnableExpreChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,text,0);
}

void AttriCalButton::EnableStateChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::EnableState,QString::number(id),0);
}

void AttriCalButton::TabChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Index,QString::number(index),0);
}

VarSelectedAttri *AttriCalButton::GetInVarAttriById(int id)
{
    switch (id) {
    case 1: return m_pInVar1;
        break;
    case 2: return m_pInVar2;
        break;
    case 3: return m_pInVar3;
        break;
    case 4: return m_pInVar4;
        break;
    case 5: return m_pInVar5;
        break;
    case 6: return m_pInVar6;
        break;
    default:
        return 0;
        break;
    }
}

VarSelectedAttri *AttriCalButton::GetOutVarAttriById(int id)
{
    switch (id) {
    case 1: return m_pOutVar1;
        break;
    case 2: return m_pOutVar2;
        break;
    case 3: return m_pOutVar3;
        break;
    case 4: return m_pOutVar4;
        break;
    case 5: return m_pOutVar5;
        break;
    case 6: return m_pOutVar6;
        break;
    default:
        return 0;
        break;
    }
}
