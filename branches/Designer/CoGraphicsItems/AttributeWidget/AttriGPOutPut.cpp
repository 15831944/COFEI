#include <QGroupBox>
#include <QScrollArea>
#include <QDir>
#include <QFileInfoList>
#include "CoGraphics/Graphics.h"
#include "CoGraphics/Scene/Scene.h"
#include "../TBasicControls/GPOutPut.h"
#include "SelectVar/DialogSelectVar.h"
#include "AttriGPOutPut.h"

AttriGPOutPut::AttriGPOutPut(QWidget *parent):
        GraphicAttrWidget(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pBaseWidget = CreateBaseAttri();
    QWidget *pOperateWidget = CreateOperateAttri();
    m_pEnableAttri = AttriBase::enableAttri();
    m_pEnableAttri->SetButtonText(tr("按钮"));
    m_pEnableAttri->SetSelectVarType(tr("Num+Switch+String"));

    pTabWidget->addTab(pBaseWidget,tr("基本属性"));
    pTabWidget->addTab(pOperateWidget,tr("操作属性"));
    pTabWidget->addTab(m_pEnableAttri,tr("可用属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

QWidget *AttriGPOutPut::CreateBaseAttri()
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

QWidget *AttriGPOutPut::CreateOperateAttri()
{
    QWidget *pOperateAttriWidget = new QWidget();
    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pOperateAttriWidget->setLayout(pVBoxLayout);
    //操作属性
    QGroupBox *pGroupJS = new QGroupBox(tr("脚本策略"));
    QTabWidget *pTabWidgetJS = new QTabWidget();
    pTabWidgetJS->setFixedHeight(110);
    m_pLiftedJSAttri = AttriBase::jsTacticAttri();
    m_pLiftedJSAttri->setObjectName(tr("LiftedJSTactic"));
    m_pPressedJSAttri = AttriBase::jsTacticAttri();
    m_pPressedJSAttri->setObjectName(tr("PressedJSTactic"));
    pTabWidgetJS->addTab(m_pLiftedJSAttri,tr("抬起"));
    pTabWidgetJS->addTab(m_pPressedJSAttri,tr("按下"));

    QVBoxLayout *pJSLayout = new QVBoxLayout();
    pJSLayout->addWidget(pTabWidgetJS);
    pJSLayout->setAlignment(pTabWidgetJS,Qt::AlignTop);
    pGroupJS->setLayout(pJSLayout);

    m_pChkIsUseIO  = TBase::CheckBox(tr("数据对象值操作"),tr("VarName"));

    QHBoxLayout *pVarLayout = new QHBoxLayout();
    m_pCmbIOState = TBase::ComboBox(tr("CmbVarOperate"));
    QStringList list;
    list.clear();
    list<<tr("")<<tr("置1")<<tr("清0");
    m_pCmbIOState->addItems(list);

    m_pCmbIOName = TBase::ComboBox("");
    list.clear();
    list.append("");
    for(int i=0;i<6;i++)
        list.append(tr("MQ%1").arg(i));
    m_pCmbIOName->addItems(list);
    pVarLayout->addWidget(m_pCmbIOState,1);
    pVarLayout->addWidget(m_pCmbIOName,2);

    pVBoxLayout->addWidget(pGroupJS);
    pVBoxLayout->addWidget(m_pChkIsUseIO);
    pVBoxLayout->addLayout(pVarLayout);
    pVBoxLayout->addStretch();

    return pOperateAttriWidget;
}

void AttriGPOutPut::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TGPOutPut *out = qobject_cast<TGPOutPut*>(itm);

    //设置基础属性窗体数据
    m_pTextAttri->SetText(out->GetText());
    m_pFontAttri->SetFontSize(QString::number(out->GetFontSize()));
    m_pFontAttri->SetFontColor(out->GetFontColor());
    m_pFontAttri->SetAligHor(out->GetFontAlignHor());
    m_pFontAttri->SetAlignVer(out->GetFontAlignVer());

    QVector<QColor> backColorVec;
    backColorVec.clear();
    backColorVec = out->GetBackColor();
    m_pLiftedBackGroundAttri->SetColor(backColorVec[0]);
    m_pPressedBackGroundAttri->SetColor(backColorVec[1]);

    QVector<bool> selectPixVec = out->GetSelectPix();
    m_pLiftedBackGroundAttri->SetImageCheckState(selectPixVec[0]);
    m_pPressedBackGroundAttri->SetImageCheckState(selectPixVec[1]);

    QVector<QString> pixFileVec = out->GetPixFiles();
    m_pLiftedBackGroundAttri->SetImage(pixFileVec[0]);
    m_pPressedBackGroundAttri->SetImage(pixFileVec[1]);

    //设置操作属性窗体数据
    //JS
    QVector<QString> jsVec = out->GetJsTactic();
    m_pLiftedJSAttri->SetJsTactic(jsVec[0]);
    m_pPressedJSAttri->SetJsTactic(jsVec[1]);

    //数据对象值操作
    if(!(out->GetIOName().isEmpty()))
    {
        m_pChkIsUseIO->setChecked(true);
    }
    m_pCmbIOState->setEnabled(m_pChkIsUseIO->isChecked());
    m_pCmbIOName->setEnabled(m_pChkIsUseIO->isChecked());

    m_pCmbIOState->setCurrentIndex(out->GetIOState());
    m_pCmbIOName->setCurrentText(out->GetIOName());

    //可用性属性
    m_pEnableAttri->SetEnableExpre(out->GetEnableExpre());
    m_pEnableAttri->SetEnableState(out->GetEnableState());

    //connect widget datechanged,prevent multiple trigger signals
    connect(m_pTextAttri,&TextAttri::SendTextFinfished,this,&AttriGPOutPut::TitleEditedFinished);
    connect(m_pFontAttri,&FontAttri::FontSizeChanged,this,&AttriGPOutPut::FontSizeChanged);
    connect(m_pFontAttri,&FontAttri::FontColorChanged,this,&AttriGPOutPut::FontColorChanged);
    connect(m_pFontAttri,&FontAttri::AlignHorChanged,this,&AttriGPOutPut::AlignHorChanged);
    connect(m_pFontAttri,&FontAttri::AlignVerChanged,this,&AttriGPOutPut::AlignVerChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriGPOutPut::BGColorChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriGPOutPut::BGIsSelectImage);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttriGPOutPut::BGImagePathChanged);

    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriGPOutPut::BGColorChanged);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriGPOutPut::BGIsSelectImage);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttriGPOutPut::BGImagePathChanged);

    connect(m_pLiftedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriGPOutPut::JsTacticChanged);
    connect(m_pPressedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriGPOutPut::JsTacticChanged);

    connect(m_pChkIsUseIO,SIGNAL(stateChanged(int)),this,SLOT(CheckStateChanged(int)));

    connect(m_pCmbIOState,SIGNAL(currentIndexChanged(int)),this,SLOT(IOChanged()));
    connect(m_pCmbIOName,&QComboBox::currentTextChanged,this,&AttriGPOutPut::IOChanged);

    connect(m_pEnableAttri,&EnableAttri::SendTextFinfished,this,&AttriGPOutPut::EnableExpreChanged);
    connect(m_pEnableAttri,&EnableAttri::EnableStateChanged,this,&AttriGPOutPut::EnableStateChanged);

    //connect button Data
    connect(this,&AttriGPOutPut::SendTAttribute,out,&TGPOutPut::SetAttributeEvent);

}

void AttriGPOutPut::TitleEditedFinished(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::Text,text,0);
}

void AttriGPOutPut::FontSizeChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::FontSize,text,0);
}

void AttriGPOutPut::FontColorChanged(QColor color)
{
    QString strColor = color.name(QColor::HexArgb);
    emit SendTAttribute(CoAttributeWidget::FontColor,strColor,0);
}

void AttriGPOutPut::AlignVerChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignVertical,QString::number(id),0);
}

void AttriGPOutPut::AlignHorChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignHorizontal,QString::number(id),0);
}

void AttriGPOutPut::BGColorChanged(QColor color)
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

void AttriGPOutPut::BGImagePathChanged()
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("PressedBackground"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::PixMap,pBGWidget->GetImage(),uc);
}

void AttriGPOutPut::BGIsSelectImage(bool state)
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

void AttriGPOutPut::CheckStateChanged(int state)
{
    Q_UNUSED(state)

    bool bCheckState = m_pChkIsUseIO->isChecked();
    m_pCmbIOName->setEnabled(bCheckState);
    m_pCmbIOState->setEnabled(bCheckState);

    if(!bCheckState)
    {
        m_pCmbIOName->setCurrentIndex(0);
        m_pCmbIOState->setCurrentIndex(0);
    }
}

void AttriGPOutPut::JsTacticChanged(const QString &text)
{
    uchar uc = 0;
    JsTacticAttri *pJS = qobject_cast<JsTacticAttri*>(sender());
    if(pJS->objectName() == "PressedJSTactic")
        uc = 1;
    emit SendTAttribute(CoAttributeWidget::JsTactic,text,uc);
}

void AttriGPOutPut::IOChanged()
{
    QString text = ",";
    if(!(m_pCmbIOName->currentIndex()==0) && !(m_pCmbIOState->currentIndex()==0))
    {
        text = tr("%1,%2").arg(m_pCmbIOName->currentText()).arg(QString::number(m_pCmbIOState->currentIndex()));
    }
    emit SendTAttribute(CoAttributeWidget::VarName,text,0);
}

void AttriGPOutPut::EnableExpreChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,text,0);
}

void AttriGPOutPut::EnableStateChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::EnableState,QString::number(id),0);
}
