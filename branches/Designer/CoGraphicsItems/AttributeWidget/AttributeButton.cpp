#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QScrollArea>
#include <QDir>
#include <QFileInfoList>
#include "AttributeButton.h"
#include "CoGraphics/Graphics.h"
#include "CoGraphics/Scene/Scene.h"
#include "../TBasicControls/Button.h"
#include "SelectVar/DialogSelectVar.h"

AttributeButton::AttributeButton(QWidget *parent):
    GraphicAttrWidget(parent)
{
    m_strVar = tr(",,");
    m_pTabWidget = new QTabWidget();
    connect(m_pTabWidget,&QTabWidget::currentChanged,this,&AttributeButton::TabChanged);
    m_pTabWidget->setFixedWidth(260);
    QWidget *pBaseWidget = CreateBaseAttri();
    QWidget *pOperateWidget = CreateOperateAttri();
    QWidget *pStateWidget = CreateStateAttri();
    m_pEnableAttri = AttriBase::enableAttri();
    m_pEnableAttri->SetButtonText(tr("按钮"));
    m_pEnableAttri->SetSelectVarType(tr("Num+Switch+String"));

    m_pTabWidget->addTab(pBaseWidget,tr("基本属性"));
    m_pTabWidget->addTab(pOperateWidget,tr("操作"));
    m_pTabWidget->addTab(pStateWidget,tr("状态"));
    m_pTabWidget->addTab(m_pEnableAttri,tr("可用"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(m_pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);

}

AttributeButton::~AttributeButton()
{

}

QWidget *AttributeButton::CreateBaseAttri()
{
    QWidget *pBaseAttriWidget = new QWidget();

    QVBoxLayout *pTopLayout = new QVBoxLayout();
    m_pCheckBoxIsPressMode = TBase::CheckBox(tr("按钮保持"),tr("checkBoxIsKeep"));
    pTopLayout->addWidget(m_pCheckBoxIsPressMode);
    pTopLayout->addSpacing(-8);
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

QWidget *AttributeButton::CreateOperateAttri()
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

    QHBoxLayout *pJSLayout = new QHBoxLayout();
    pJSLayout->addWidget(pTabWidgetJS);
    pGroupJS->setLayout(pJSLayout);

    m_pChkIsSwitchScreen  = TBase::CheckBox(tr("打开用户窗口"),tr("SwitchScreen"));
    m_pChkIsUseOpFunc  = TBase::CheckBox(tr("运行功能"),tr("OPFunc"));
    m_pChkIsUseVar  = TBase::CheckBox(tr("数据对象值操作"),tr("VarName"));

    m_pCmbScreen = TBase::ComboBox(tr("CmbScreen"));
    m_pCmbSystemOpState = TBase::ComboBox(tr("CmbSystemOpState"));
    QStringList list;
    list.clear();
    list<<tr("")
        <<tr("启动")<<tr("暂停/恢复")<<tr("停止")
        << tr("急停")<< tr("复位")<< tr("回原点")
        << tr("关闭程序")<< tr("关机")<< tr("重启系统");
    m_pCmbSystemOpState->addItems(list);

    QHBoxLayout *pVarLayout = new QHBoxLayout();
    m_pCmbVarState = TBase::ComboBox(tr("CmbVarOperate"),80,22);
    list.clear();
    list<<tr("")<<tr("置1")<<tr("清0")<<tr("取反")<<tr("按1松0")<<tr("按0松1");
    m_pCmbVarState->addItems(list);

    m_pLineEditVarName = TBase::LineEdit(tr("LineEditVar"));
    m_pBtnSelectVar = TBase::Button(tr("?"),tr("BtnSelectVar"),30,22);
    connect(m_pBtnSelectVar,&QPushButton::clicked,this,&AttributeButton::SelectVar);
    pVarLayout->addWidget(m_pCmbVarState);
    pVarLayout->addWidget(m_pLineEditVarName);
    pVarLayout->addWidget(m_pBtnSelectVar);

    pVBoxLayout->addWidget(pGroupJS);
    pVBoxLayout->addWidget(m_pChkIsSwitchScreen);
    pVBoxLayout->addWidget(m_pCmbScreen);
    pVBoxLayout->addWidget(m_pChkIsUseOpFunc);
    pVBoxLayout->addWidget(m_pCmbSystemOpState);
    pVBoxLayout->addWidget(m_pChkIsUseVar);
    pVBoxLayout->addLayout(pVarLayout);
    pVBoxLayout->addStretch();

    return pOperateAttriWidget;
}

QWidget *AttributeButton::CreateStateAttri()
{
    QWidget *pStateAttriWidget = new QWidget();

    QHBoxLayout *pVarLayout = new QHBoxLayout();
    m_pStateVar = AttriBase::varSelectedAttri();
    m_pStateVar->SetSelectVarType("Switch");
    pVarLayout->addWidget(new QLabel(tr("控制量")));
    pVarLayout->addWidget(m_pStateVar);
    connect(m_pStateVar,&VarSelectedAttri::SendTextFinfished,this,&AttributeButton::IsStateVar);

    m_pLineEditFreq = TBase::LineEdit(tr(""));
    QIntValidator *pIV = new QIntValidator();
    m_pLineEditFreq->setValidator(pIV);
    QHBoxLayout *pFreqLayout = new QHBoxLayout();
    pFreqLayout->addWidget(new QLabel(tr("刷新频率")));
    pFreqLayout->addWidget(m_pLineEditFreq);
    pFreqLayout->addWidget(new QLabel(tr("ms")));

    QWidget *pZeroWg = new QWidget();
    QVBoxLayout *pZeroLayout = new QVBoxLayout();
    QGroupBox *pGrpZeroBG = new QGroupBox(tr("背景"));
    QVBoxLayout *pGZLayout = new QVBoxLayout();
    pGrpZeroBG->setContentsMargins(5,10,5,10);
    pGrpZeroBG->setLayout(pGZLayout);
    m_pZeroStateBGAttri = AttriBase::backgroundAttri();
    m_pZeroStateBGAttri->setObjectName("Zero");
    m_pZeroStateBGAttri->setContentsMargins(-8,-8,-8,-8);
    pGZLayout->addWidget(m_pZeroStateBGAttri);
    QHBoxLayout *pZFCLayout = new QHBoxLayout();
    m_pZeroFontColor = AttriBase::colorSelect();
    m_pZeroFontColor->setObjectName("Zero");
    m_pZeroFontColor->setContentsMargins(-10,-8,-9,-8);
    pZFCLayout->addWidget(new QLabel(tr("字体")));
    pZFCLayout->addWidget(m_pZeroFontColor);
    m_pZeroTextAttri = AttriBase::textAttri();
    m_pZeroTextAttri->setContentsMargins(-8,-8,-8,-8);
    m_pZeroTextAttri->SetText(tr("Button"));
    m_pZeroTextAttri->setObjectName(tr("Zero"));
    pZeroLayout->addLayout(pZFCLayout);
    pZeroLayout->addWidget(m_pZeroTextAttri);
    pZeroLayout->addWidget(pGrpZeroBG);
    pZeroLayout->addStretch(1);
    pZeroWg->setLayout(pZeroLayout);

    QWidget *pOneWg = new QWidget();
    QVBoxLayout *pOneLayout = new QVBoxLayout();
    QGroupBox *pGrpOneBG = new QGroupBox(tr("背景"));
    QVBoxLayout *pGOLayout = new QVBoxLayout();
    pGrpOneBG->setContentsMargins(5,10,5,10);
    pGrpOneBG->setLayout(pGOLayout);
    m_pOneStateBGAttri = AttriBase::backgroundAttri();
    m_pOneStateBGAttri->setObjectName("One");
    m_pOneStateBGAttri->setContentsMargins(-8,-8,-8,-8);
    pGOLayout->addWidget(m_pOneStateBGAttri);
    QHBoxLayout *pOFCLayout = new QHBoxLayout();
    m_pOneFontColor = AttriBase::colorSelect();
    m_pOneFontColor->setObjectName("One");
    m_pOneFontColor->setContentsMargins(-10,-8,-9,-8);
    pOFCLayout->addWidget(new QLabel(tr("字体")));
    pOFCLayout->addWidget(m_pOneFontColor);
    m_pOneTextAttri = AttriBase::textAttri();
    m_pOneTextAttri->SetText(tr("Button"));
    m_pOneTextAttri->setContentsMargins(-8,-8,-8,-8);
    m_pOneTextAttri->setObjectName(tr("One"));
    pOneLayout->addLayout(pOFCLayout);
    pOneLayout->addWidget(m_pOneTextAttri);
    pOneLayout->addWidget(pGrpOneBG);
    pOneLayout->addStretch(1);
    pOneWg->setLayout(pOneLayout);

    m_pTabState = new QTabWidget();
    m_pTabState->addTab(pZeroWg,tr("0"));
    m_pTabState->addTab(pOneWg,tr("1"));
    m_pTabState->setEnabled(false);

    QVBoxLayout *pStateAtrriLayout = new QVBoxLayout();
    pStateAtrriLayout->addLayout(pVarLayout);
    pStateAtrriLayout->addLayout(pFreqLayout);
    pStateAtrriLayout->addWidget(m_pTabState);
//    pStateAtrriLayout->addStretch();
    pStateAttriWidget->setLayout(pStateAtrriLayout);
    return pStateAttriWidget;
}

void AttributeButton::SetItem(CoItemEx *Item)
{
    GraphicAttrWidget::SetItem(Item);
    TButton *btn = qobject_cast<TButton*>(Item);
    m_pTabWidget->setCurrentIndex(btn->GetAttriIndex());

    //设置基础属性窗体数据
    m_pCheckBoxIsPressMode->setChecked(btn->GetPressMode());
    m_pTextAttri->SetText(btn->GetText());

    m_pFontAttri->SetFontSize(QString::number(btn->GetFontSize()));
    m_pFontAttri->SetFontColor(btn->GetFontColor());
    m_pFontAttri->SetAligHor(btn->GetFontAlignHor());
    m_pFontAttri->SetAlignVer(btn->GetFontAlignVer());

    QVector<QColor> backColorVec;
    backColorVec.clear();
    backColorVec = btn->GetBackColor();
    m_pLiftedBackGroundAttri->SetColor(backColorVec[0]);
    m_pPressedBackGroundAttri->SetColor(backColorVec[1]);

    QVector<bool> selectPixVec = btn->GetSelectPix();
    m_pLiftedBackGroundAttri->SetImageCheckState(selectPixVec[0]);
    m_pPressedBackGroundAttri->SetImageCheckState(selectPixVec[1]);

    QVector<QString> pixFileVec = btn->GetPixFiles();
    m_pLiftedBackGroundAttri->SetImage(pixFileVec[0]);
    m_pPressedBackGroundAttri->SetImage(pixFileVec[1]);

    //设置操作属性窗体数据
    //JS
    QVector<QString> jsVec = btn->GetJsTactic();
    m_pLiftedJSAttri->SetJsTactic(jsVec[0]);
    m_pPressedJSAttri->SetJsTactic(jsVec[1]);

    //打开画布
    CoScene *pScene = static_cast<CoScene*>(Item->GetCurScene());
    QStringList list = AttributeInstance::GetInstance()->canvasMap[pScene->GetProjectName()];
    list.insert(0,"");
    m_pCmbScreen->addItems(list);
    QString strScreen = btn->GetScreen();
    if(!strScreen.isEmpty())
    {
        if(list.contains(strScreen))
        {
            m_pChkIsSwitchScreen->setChecked(true);
            m_pCmbScreen->setCurrentText(btn->GetScreen());
        }
        else
        {
            m_pChkIsSwitchScreen->setChecked(false);
            btn->SetScreen(tr(""));
        }
    }
    m_pCmbScreen->setEnabled(m_pChkIsSwitchScreen->isChecked());

    char state =btn->GetOpFunc();
    if(state > 0)
    {
        m_pCmbSystemOpState->setCurrentIndex(state);
        if(state > 1 && state < 5)
        {
            m_pCmbSelectPower->addItem("4");
            m_pCmbSelectPower->setCurrentText("4");
            m_pCmbSelectPower->setEnabled(false);
        }

        m_pChkIsUseOpFunc->setChecked(true);
    }
    m_pCmbSystemOpState->setEnabled(m_pChkIsUseOpFunc->isChecked());

    //数据对象值操作
    if(!(btn->GetVarName().isEmpty()) && btn->GetVarType() >= 0)
    {
        m_pChkIsUseVar->setChecked(true);
        m_pCmbVarState->setCurrentIndex(btn->GetVarState());
        m_pLineEditVarName->setText(btn->GetVarName());
    }
    m_pCmbVarState->setEnabled(m_pChkIsUseVar->isChecked());
    m_pLineEditVarName->setEnabled(m_pChkIsUseVar->isChecked());
    m_pBtnSelectVar->setEnabled(m_pChkIsUseVar->isChecked());

    QStringList list1;
    list1.reserve(3);
    list1<< btn->GetVarName()
         << QString::number(btn->GetVarState())
         << QString::number(btn->GetVarType());
    m_strVar = tr("%1,%2,%3").arg(list1[0]).arg(list1[1]).arg(list1[2]);

    //可用性属性
    m_pEnableAttri->SetEnableExpre(btn->GetEnableExpre());
    m_pEnableAttri->SetEnableState(btn->GetEnableState());

    //状态属性
    m_pLineEditFreq->setText(QString::number(btn->GetAutoTime()));
    if(!btn->GetStateVarName().isEmpty())
    {
        m_pTabState->setEnabled(true);
        m_pStateVar->SetVarName(btn->GetStateVarName());
        m_pStateVar->SetVarType(btn->GetStateVarType());
        m_pZeroFontColor->SetColor(QColor(btn->GetStateFontColor().at(0)));
        m_pOneFontColor->SetColor(QColor(btn->GetStateFontColor().at(1)));
        m_pZeroTextAttri->SetText(btn->GetStateText().at(0));
        m_pOneTextAttri->SetText(btn->GetStateText().at(1));
        m_pZeroStateBGAttri->SetColor(QColor(btn->GetStateBGColor().at(0)));
        m_pZeroStateBGAttri->SetImageCheckState(btn->GetStateSelectPix().at(0));
        m_pZeroStateBGAttri->SetImage(btn->GetStateBGPix().at(0));
        m_pOneStateBGAttri->SetColor(QColor(btn->GetStateBGColor().at(1)));
        m_pOneStateBGAttri->SetImageCheckState(btn->GetStateSelectPix().at(1));
        m_pOneStateBGAttri->SetImage(btn->GetStateBGPix().at(1));
    }

    //connect widget datechanged,prevent multiple trigger signals
    connect(m_pTextAttri,&TextAttri::SendTextFinfished,this,&AttributeButton::TitleEditedFinished);

    connect(m_pCheckBoxIsPressMode,&QCheckBox::stateChanged,this,&AttributeButton::IsPressModeChanged);
    connect(m_pFontAttri,&FontAttri::FontSizeChanged,this,&AttributeButton::FontSizeChanged);
    connect(m_pFontAttri,&FontAttri::FontColorChanged,this,&AttributeButton::FontColorChanged);
    connect(m_pFontAttri,&FontAttri::AlignHorChanged,this,&AttributeButton::AlignHorChanged);
    connect(m_pFontAttri,&FontAttri::AlignVerChanged,this,&AttributeButton::AlignVerChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttributeButton::BGColorChanged);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttributeButton::BGIsSelectImage);
    connect(m_pPressedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttributeButton::BGImagePathChanged);

    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttributeButton::BGColorChanged);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttributeButton::BGIsSelectImage);
    connect(m_pLiftedBackGroundAttri,&BackgroundAttri::ImageChanged,this,&AttributeButton::BGImagePathChanged);

    connect(m_pLiftedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttributeButton::JsTacticChanged);
    connect(m_pPressedJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttributeButton::JsTacticChanged);

    connect(m_pChkIsSwitchScreen,SIGNAL(stateChanged(int)),this,SLOT(CheckStateChanged(int)));
    connect(m_pChkIsUseOpFunc,SIGNAL(stateChanged(int)),this,SLOT(CheckStateChanged(int)));
    connect(m_pChkIsUseVar,SIGNAL(stateChanged(int)),this,SLOT(CheckStateChanged(int)));

    connect(m_pCmbScreen,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(ScreenChanged(const QString &)));
    connect(m_pCmbSystemOpState,SIGNAL(currentIndexChanged(int)),this,SLOT(SystemOpStateChanged(int)));
    connect(m_pCmbVarState,SIGNAL(currentIndexChanged(int)),this,SLOT(VarChanged()));
    connect(m_pLineEditVarName,&QLineEdit::editingFinished,this,&AttributeButton::VarChanged);

    connect(m_pEnableAttri,&EnableAttri::SendTextFinfished,this,&AttributeButton::EnableExpreChanged);
    connect(m_pEnableAttri,&EnableAttri::EnableStateChanged,this,&AttributeButton::EnableStateChanged);

    connect(m_pStateVar,&VarSelectedAttri::SendTextFinfished,this,&AttributeButton::StateVarChanged);
    connect(m_pLineEditFreq,&QLineEdit::editingFinished,this,&AttributeButton::FreqChanged);
    connect(m_pZeroStateBGAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttributeButton::StateBGColorChanged);
    connect(m_pZeroStateBGAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttributeButton::StateBGIsSelectImage);
    connect(m_pZeroStateBGAttri,&BackgroundAttri::ImageChanged,this,&AttributeButton::StateBGPixChanged);
    connect(m_pOneStateBGAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttributeButton::StateBGColorChanged);
    connect(m_pOneStateBGAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttributeButton::StateBGIsSelectImage);
    connect(m_pOneStateBGAttri,&BackgroundAttri::ImageChanged,this,&AttributeButton::StateBGPixChanged);
    connect(m_pZeroFontColor,&ColorSelect::ColorChanged,this,&AttributeButton::StateFColorChanged);
    connect(m_pOneFontColor,&ColorSelect::ColorChanged,this,&AttributeButton::StateFColorChanged);
    connect(m_pZeroTextAttri,&TextAttri::SendTextFinfished,this,&AttributeButton::StateTextChanged);
    connect(m_pOneTextAttri,&TextAttri::SendTextFinfished,this,&AttributeButton::StateTextChanged);

    //connect button Data
    connect(this,&AttributeButton::SendTAttribute,btn,&TButton::SetAttributeEvent);
}

void AttributeButton::IsPressModeChanged(int state)
{
    QString strState = "0";
    if(state > 0)
    {
        strState = "1";
    }
    emit SendTAttribute(CoAttributeWidget::PressMode,strState,0);
}

void AttributeButton::TitleEditedFinished(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::Text,text,0);
}

void AttributeButton::FontSizeChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::FontSize,text,0);
}

void AttributeButton::FontColorChanged(QColor color)
{
    QString strColor = color.name(QColor::HexArgb);
    emit SendTAttribute(CoAttributeWidget::FontColor,strColor,0);
}

void AttributeButton::AlignVerChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignVertical,QString::number(id),0);
}

void AttributeButton::AlignHorChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignHorizontal,QString::number(id),0);
}

void AttributeButton::BGColorChanged(QColor color)
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

void AttributeButton::BGImagePathChanged()
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("PressedBackground"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::PixMap,pBGWidget->GetImage(),uc);
}

void AttributeButton::BGIsSelectImage(bool state)
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

void AttributeButton::CheckStateChanged(int state)
{
    Q_UNUSED(state)

    QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(sender());
    QString strObjectName = pCheckBox->objectName();
    bool bCheckState = pCheckBox->isChecked();

    if(strObjectName == tr("SwitchScreen"))
    {
        m_pCmbScreen->setEnabled(bCheckState);
    }
    else if(strObjectName == tr("OPFunc"))
    {
        m_pCmbSystemOpState->setEnabled(bCheckState);
    }
    else if(strObjectName == tr("VarName"))
    {
        m_pCmbVarState->setEnabled(bCheckState);
        m_pLineEditVarName->setEnabled(bCheckState);
        m_pBtnSelectVar->setEnabled(bCheckState);
    }

    //未勾选时,combobox中的选项置0,并修改数据
    if(!bCheckState)
    {
        if(strObjectName == tr("SwitchScreen"))
        {
            m_pCmbScreen->setCurrentIndex(0);
        }
        else if(strObjectName == tr("OPFunc"))
        {
            m_pCmbSystemOpState->setCurrentIndex(0);
        }
        else if(strObjectName == tr("VarName"))
        {
            m_pLineEditVarName->clear();
            m_pCmbVarState->setCurrentIndex(0);
            int nEnum = GetStringToEnum(strObjectName);
            emit SendTAttribute(nEnum,",,",0);
        }
    }
}

void AttributeButton::SelectVar()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(tr("Num+Switch+Count"));
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name,const QString type)
    {
        m_strVar = tr("%1,%2,%3").arg(name).arg(m_pCmbVarState->currentIndex()).arg(type);
        m_pLineEditVarName->setText(name);
        m_pLineEditVarName->editingFinished();
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

void AttributeButton::JsTacticChanged(const QString &text)
{
    uchar uc = 0;
    JsTacticAttri *pJS = qobject_cast<JsTacticAttri*>(sender());
    if(pJS->objectName() == "PressedJSTactic")
        uc = 1;
    emit SendTAttribute(CoAttributeWidget::JsTactic,text,uc);
}

void AttributeButton::ScreenChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::SwitchScreen,text,0);
}

void AttributeButton::SystemOpStateChanged(int index)
{
    if(index > 1 && index < 5)
    {
        m_pCmbSelectPower->addItem("4");
        m_pCmbSelectPower->setCurrentText("4");
        m_pCmbSelectPower->setEnabled(false);
    }
    else
    {
        if(m_pCmbSelectPower->currentIndex() == 4)
        {
            m_pCmbSelectPower->removeItem(4);
        }
        m_pCmbSelectPower->setCurrentIndex(3);
        m_pCmbSelectPower->setEnabled(true);
    }
    emit SendTAttribute(CoAttributeWidget::OPFunc,QString::number(index),0);
}

void AttributeButton::VarChanged()
{
    if(m_pLineEditVarName->text().isEmpty())
        return;
    //字符串拼接的形式.
    QStringList list = m_strVar.split(',');
    if(list.size() > 1)
    {
        list[0] = m_pLineEditVarName->text();
        list[1] = QString::number(m_pCmbVarState->currentIndex());
    }

    QString str = QString("%1,%2,%3").arg(list[0]).arg(list[1]).arg(list[2]);
    emit SendTAttribute(CoAttributeWidget::VarName,str,0);
}

void AttributeButton::EnableExpreChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,text,0);
}

void AttributeButton::EnableStateChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::EnableState,QString::number(id),0);
}

void AttributeButton::IsStateVar(const QString &var)
{
    if(var.isEmpty())
        m_pTabState->setEnabled(false);
    else
        m_pTabState->setEnabled(true);
}

void AttributeButton::FreqChanged()
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,m_pLineEditFreq->text(),0);
}

void AttributeButton::StateVarChanged(const QString &var)
{
    emit SendTAttribute(CoAttributeWidget::StateVarName,var,0);
}

void AttributeButton::StateBGColorChanged(QColor color)
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    QString strColor = color.name(QColor::HexArgb);
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }

    emit SendTAttribute(CoAttributeWidget::StateBGColor,strColor,uc);
}

void AttributeButton::StateBGIsSelectImage(bool state)
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());

    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    //是否使用图
    QString strSelectPix = "";
    if(state)
    {
        strSelectPix = "1";
    }

    emit SendTAttribute(CoAttributeWidget::StateSelectPix,strSelectPix,uc);
}

void AttributeButton::StateBGPixChanged(const QString)
{
    BackgroundAttri *pBGWidget = qobject_cast<BackgroundAttri*>(sender());
    uchar uc = 0;
    if(pBGWidget->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::StateBGPix,pBGWidget->GetImage(),uc);
}

void AttributeButton::StateFColorChanged(QColor color)
{
    ColorSelect *pColor = qobject_cast<ColorSelect*>(sender());
    uchar uc = 0;
    if(pColor->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::StateFColor,color.name(QColor::HexArgb),uc);
}

void AttributeButton::StateTextChanged(const QString &text)
{
    TextAttri *pTxt = qobject_cast<TextAttri*>(sender());
    uchar uc = 0;
    if(pTxt->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::StateText,text,uc);
}

void AttributeButton::TabChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Index,QString::number(index),0);
}

