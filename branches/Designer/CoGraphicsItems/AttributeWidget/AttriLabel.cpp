#include <QIntValidator>
#include <QTabWidget>
#include <QDebug>
#include <QScrollArea>
#include "AttriLabel.h"
#include "SelectVar/DialogSelectVar.h"
#include "TBasicControls/Label.h"

AttriLabel::AttriLabel(QWidget *parent) : GraphicAttrWidget(parent)
{
    m_strVar = ",";
    m_pTabWidgetAtrri = new QTabWidget();
    connect(m_pTabWidgetAtrri,&QTabWidget::currentChanged,this,&AttriLabel::TabChanged);
    m_pTabWidgetAtrri->setFixedWidth(260);
    QWidget *pBaseAttri = CreateBaseAttri();
    m_pDisplayAttri = CreateDisplyAttri();

    m_pTabWidgetAtrri->addTab(pBaseAttri,tr("基本属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(m_pTabWidgetAtrri);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

AttriLabel::~AttriLabel()
{

}

void AttriLabel::SetItem(CoItemEx *Item)
{
    GraphicAttrWidget::SetItem(Item);
    TLabel *Lbl = qobject_cast<TLabel*>(Item);

    //基础属性
    bool b = bool(Lbl->GetOutputMode());
    m_pChkOutputMode->setChecked(b);
    if(b)
    {
        m_pTabWidgetAtrri->addTab(m_pDisplayAttri,tr("显示输出"));
        m_pGrpType->setEnabled(true);
        m_pTextAttri->m_pTextEditText->setEnabled(false);
    }
    m_pTabWidgetAtrri->setCurrentIndex(Lbl->GetAttriIndex());

    m_pTextAttri->SetText(Lbl->GetText());
    m_pFontAttri->SetFontSize(QString::number(Lbl->GetFontSize()));
    m_pFontAttri->SetFontColor(Lbl->GetFontColor());
    m_pFontAttri->m_pHorButtonGroup->button(Lbl->GetFontAlignHor())->setChecked(true);
    m_pFontAttri->m_pVerButtonGroup->button(Lbl->GetFontAlignVer())->setChecked(true);
    m_pBGAttri->SetColor(Lbl->GetBackColor().at(0));
    m_pBGAttri->SetImageCheckState(Lbl->GetSelectPix()[0]);
    m_pBGAttri->SetImage(Lbl->GetPixFiles()[0]);

    //显示属性
    m_pLineEditExpre->setText(Lbl->GetVarName());
    m_pLineEditUnit->setText(Lbl->GetUnit());
    m_pValueTypeBtnGroup->button(Lbl->GetValueType())->setChecked(true);
    m_pValueTypeBtnGroup->buttonClicked(Lbl->GetValueType());
    SetWidgetsByEnum(Lbl->GetValueFormat());
    m_pSpnInt->setValue(Lbl->GetIntAccurecy());
    m_pSpnDecimal->setValue(Lbl->GetDecAccurecy());
    //JS
    m_pJSAttri->SetJsTactic(Lbl->GetJsTactic());

    m_pLineEditFreq->setText(QString::number(Lbl->GetAutoTime()));

    connect(m_pTextAttri,&TextAttri::SendTextFinfished,this,&AttriLabel::TitleEditedFinished);
    connect(m_pFontAttri,&FontAttri::FontSizeChanged,this,&AttriLabel::FontSizeChanged);
    connect(m_pFontAttri,&FontAttri::FontColorChanged,this,&AttriLabel::FontColorChanged);
    connect(m_pFontAttri,&FontAttri::AlignHorChanged,this,&AttriLabel::AlignHorChanged);
    connect(m_pFontAttri,&FontAttri::AlignVerChanged,this,&AttriLabel::AlignVerChanged);
    connect(m_pBGAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriLabel::BGColorChanged);
    connect(m_pBGAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriLabel::BGIsSelectImage);
    connect(m_pBGAttri,&BackgroundAttri::ImageChanged,this,&AttriLabel::BGImagePathChanged);

    connect(m_pChkOutputMode,&QCheckBox::stateChanged,this,&AttriLabel::OutputModeChangedExt);
    connect(m_pLineEditExpre,&QLineEdit::editingFinished,this,&AttriLabel::VarChanged);
    connect(m_pLineEditUnit,&QLineEdit::editingFinished,this,&AttriLabel::UnitChanged);
    connect(m_pValueTypeBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ValueTypeChangedExt(int)));
    connect(m_pTypeBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ValueFormatChanged()));
    connect(m_pRuleBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ValueFormatChanged()));
    connect(m_pChkRound,&QCheckBox::stateChanged,this,&AttriLabel::ValueFormatChanged);
    connect(m_pChkFrontZero,&QCheckBox::stateChanged,this,&AttriLabel::ValueFormatChanged);
    connect(m_pChkPwd,&QCheckBox::stateChanged,this,&AttriLabel::ValueFormatChanged);
    connect(m_pSpnInt,SIGNAL(valueChanged(const QString&)),this,SLOT(IntAccurecyChanged(const QString&)));
    connect(m_pSpnDecimal,SIGNAL(valueChanged(const QString&)),this,SLOT(DecAccurecyChanged(const QString&)));
    connect(m_pLineEditFreq,&QLineEdit::editingFinished,this,&AttriLabel::FreqChanged);
     connect(m_pJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriLabel::JsTacticChanged);

    //Label
    connect(this,&AttriLabel::SendTAttribute,Lbl,&TLabel::SetAttributeEvent);
}

QWidget *AttriLabel::CreateBaseAttri()
{
    QWidget *pBaseAttri = new QWidget();

    m_pCmbSelectPower->setEnabled(false);
    QVBoxLayout *pTopLayout = new QVBoxLayout();
    m_pChkOutputMode = TBase::CheckBox(tr("显示输出"),tr(""));
    connect(m_pChkOutputMode,&QCheckBox::stateChanged,this,&AttriLabel::OutputModeChanged);
    pTopLayout->addWidget(m_pChkOutputMode);
    pTopLayout->addSpacing(-8);
    m_pTextAttri = AttriBase::textAttri();
    m_pTextAttri->setContentsMargins(-8,-8,-8,-8);
    pTopLayout->addWidget(m_pTextAttri);

    QGroupBox *pGroupFont = new QGroupBox(tr("字体"));
    QVBoxLayout *pFontLayout = new QVBoxLayout();
    pGroupFont->setLayout(pFontLayout);
    m_pFontAttri = AttriBase::fontAttri();
    m_pFontAttri->setContentsMargins(-9,-8,-9,-8);
    pFontLayout->addWidget(m_pFontAttri);

    //背景
    QGroupBox *pGroupBG = new QGroupBox(tr("背景"));
    pGroupBG->setContentsMargins(5,10,5,10);
    m_pBGAttri = AttriBase::backgroundAttri();
    m_pBGAttri->setContentsMargins(-9,-8,-9,-8);
    QVBoxLayout *pBGLayout = new QVBoxLayout();
    pBGLayout->addWidget(m_pBGAttri);
    pGroupBG->setLayout(pBGLayout);

    QVBoxLayout *pBaseLayout = new QVBoxLayout();
    pBaseLayout->addLayout(m_pGridLayout);
    pBaseLayout->addLayout(pTopLayout);
    pBaseLayout->addWidget(pGroupFont);
    pBaseLayout->addWidget(pGroupBG);
    pBaseLayout->addSpacing(20);
//    pBaseLayout->setAlignment(Qt::AlignTop);

    pBaseAttri->setLayout(pBaseLayout);

    return pBaseAttri;
}

QWidget* AttriLabel::CreateDisplyAttri()
{
    QWidget *pDisplayAttri = new QWidget();
    QVBoxLayout *pDisplayLayout = new QVBoxLayout();
    pDisplayAttri->setLayout(pDisplayLayout);

    //表达式
    QGroupBox *pExpressGroup = new QGroupBox(tr("变量"));
    pDisplayLayout->addWidget(pExpressGroup);
    QVBoxLayout *pExpressLayout = new QVBoxLayout();
    pExpressGroup->setLayout(pExpressLayout);

    QHBoxLayout *pHLayout = new QHBoxLayout();
    pExpressLayout->addLayout(pHLayout);
    m_pLineEditExpre = TBase::LineEdit(tr(""));
    pHLayout->addWidget(m_pLineEditExpre);

    m_pBtnSelectVarExt = TBase::Button(tr("?"),tr(""));
    pHLayout->addWidget(m_pBtnSelectVarExt);
    connect(m_pBtnSelectVarExt,&QPushButton::clicked,this,&AttriLabel::SelectVar);

    QLabel *pLblUnit = TBase::Label(tr("单位"),tr(""));
    pExpressLayout->addWidget(pLblUnit);
    m_pLineEditUnit = TBase::LineEdit(tr(""));
    pExpressLayout->addWidget(m_pLineEditUnit);


    //输出值类型
    m_pValueTypeBtnGroup = new QButtonGroup();
    m_pRdoNumber = new QRadioButton(tr("数值量输出"));
//    m_pRdoNumber->setEnabled(false);
    m_pRdoString = new QRadioButton(tr("字符串输出"));
//    m_pRdoString->setEnabled(false);
    m_pValueTypeBtnGroup->addButton(m_pRdoNumber,0);
    m_pValueTypeBtnGroup->addButton(m_pRdoString,1);
    connect(m_pValueTypeBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ValueTypeChanged(int)));

    m_pGrpValueType = new QGroupBox(tr("输出值类型"));
    m_pGrpValueType->setEnabled(false);
    pDisplayLayout->addWidget(m_pGrpValueType);
    QHBoxLayout *pValueTypeLayout = new QHBoxLayout();
    m_pGrpValueType->setLayout(pValueTypeLayout);
    pValueTypeLayout->addWidget(m_pRdoNumber);
    pValueTypeLayout->addWidget(m_pRdoString);

    //输出格式
    m_pGrpType = new QGroupBox(tr("输出格式"));
    pDisplayLayout->addWidget(m_pGrpType);
    QGridLayout *pTypeLayout = new QGridLayout();
    m_pGrpType->setLayout(pTypeLayout);

    m_pTypeBtnGroup = new QButtonGroup();
    m_pChkDouble = TBase::CheckBox(tr("浮点输出"),tr(""));
    m_pChkNatural = TBase::CheckBox(tr("整型输出"),tr(""));
    m_pTypeBtnGroup->addButton(m_pChkDouble,0);
    m_pTypeBtnGroup->addButton(m_pChkNatural,1);
    connect(m_pTypeBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(TypeChanged(int)));

    m_pRuleBtnGroup = new QButtonGroup();
    m_pRdoDec = new QRadioButton(tr("十进制"));
    m_pRdoHex = new QRadioButton(tr("十六进制"));
    m_pRdoBin = new QRadioButton(tr("二进制"));
    m_pRuleBtnGroup->addButton(m_pRdoDec,0);
    m_pRuleBtnGroup->addButton(m_pRdoHex,1);
    m_pRuleBtnGroup->addButton(m_pRdoBin,2);
    connect(m_pRuleBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(RuleChanged(int)));

    m_pChkRound = TBase::CheckBox(tr("四舍五入"),tr(""));
    m_pChkFrontZero = TBase::CheckBox(tr("前导0"),tr(""));
    connect(m_pChkFrontZero,&QCheckBox::stateChanged,this,&AttriLabel::FrontZeroCheckChanged);
    m_pChkPwd = TBase::CheckBox(tr("密码"),tr(""));

    QLabel *pLblInt = TBase::Label(tr("整数位数"),tr(""));
    m_pSpnInt = new QSpinBox();
    m_pSpnInt->setRange(0,10);
    QLabel *pLblDecimal = TBase::Label(tr("小数位数"),tr(""));
    m_pSpnDecimal = new QSpinBox();
    m_pSpnDecimal->setRange(0,6);
    m_pSpnDecimal->setValue(3);

    pTypeLayout->addWidget(m_pChkDouble,0,0,1,3);
    pTypeLayout->addWidget(m_pChkNatural,0,3,1,3);
    pTypeLayout->addWidget(m_pRdoDec,1,0,1,2);
    pTypeLayout->addWidget(m_pRdoHex,1,2,1,2);
    pTypeLayout->addWidget(m_pRdoBin,1,4,1,2);
    pTypeLayout->addWidget(m_pChkRound,2,0,1,2);
    pTypeLayout->addWidget(m_pChkFrontZero,2,2,1,2);
    pTypeLayout->addWidget(m_pChkPwd,2,4,1,2);
    pTypeLayout->addWidget(pLblInt,3,0,1,3,Qt::AlignLeft);
    pTypeLayout->addWidget(m_pSpnInt,3,2,1,4);
    pTypeLayout->addWidget(pLblDecimal,4,0,1,2,Qt::AlignLeft);
    pTypeLayout->addWidget(m_pSpnDecimal,4,2,1,4);

    m_pLineEditFreq = TBase::LineEdit(tr(""));
    QIntValidator *pIV = new QIntValidator();
    m_pLineEditFreq->setValidator(pIV);

    QGroupBox *pGroupJS = new QGroupBox(tr("脚本策略"));
    m_pJSAttri = AttriBase::jsTacticAttri();
    m_pJSAttri->setContentsMargins(-9,-9,-9,-9);
    m_pJSAttri->setObjectName(tr("JSAttri"));
    QHBoxLayout *pJSLayout = new QHBoxLayout();
    pJSLayout->addWidget(m_pJSAttri);
    pGroupJS->setLayout(pJSLayout);
    pDisplayLayout->addWidget(pGroupJS);

    QHBoxLayout *pFreqLayout = new QHBoxLayout();
    pDisplayLayout->addLayout(pFreqLayout);
    pFreqLayout->addWidget(new QLabel(tr("刷新频率")));
    pFreqLayout->addWidget(m_pLineEditFreq);
    pFreqLayout->addWidget(new QLabel(tr("ms")));

//    pDisplayLayout->addStretch();
    return pDisplayAttri;
}

uchar AttriLabel::GetCurValueFormat()
{
    uchar uc = 0;
    if(m_pChkNatural->isChecked())
    {
        if(m_pRdoDec->isChecked())
            uc = 1;
        else if(m_pRdoHex->isChecked())
            uc = 2;
        else if(m_pRdoBin->isChecked() && (!m_pChkFrontZero->isChecked()))
            uc = 3;
        else if(m_pRdoBin->isChecked() && m_pChkFrontZero->isChecked())
            uc = 4;
    }

    if(m_pChkDouble->isChecked())
    {
        if(m_pChkRound->isChecked() && m_pChkFrontZero->isChecked())
            uc = 7;
        else if(m_pChkRound->isChecked())
            uc = 5;
        else if(m_pChkFrontZero->isChecked())
            uc = 6;
    }

    //8是普通字符串,9是密码字符串
    if(m_pChkPwd->isChecked())
        uc = 9;
    else if(m_pChkPwd->isEnabled() && !m_pChkPwd->isChecked())
        uc = 8;

    return uc;
}

void AttriLabel::SetWidgetsByEnum(int type)
{
    TLabel::ValueType euType = TLabel::ValueType(type);
    switch (euType)
    {
    case TLabel::Double:
    {
        m_pChkDouble->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(0);
        break;
    }
    case TLabel::Dec:
    {
        m_pChkNatural->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(1);
        m_pRdoDec->setChecked(true);
        break;
    }
    case TLabel::Hex:
    {
        m_pChkNatural->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(1);
        m_pRdoHex->setChecked(true);
        break;
    }
    case TLabel::Bin:
    {
        m_pChkNatural->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(1);
        m_pRdoBin->setChecked(true);
        break;
    }
    case TLabel::BinAndFrontZero:
    {
        m_pChkNatural->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(1);
        m_pRdoBin->setChecked(true);
        m_pChkFrontZero->setChecked(true);
        break;
    }
    case TLabel::Round:
    {
        m_pChkDouble->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(0);
        m_pChkRound->setChecked(true);
        break;
    }
    case TLabel::FrontZero:
    {
        m_pChkDouble->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(0);
        m_pChkFrontZero->setChecked(true);
        break;
    }
    case TLabel::RoundAndFrontZero:
    {
        m_pChkDouble->setChecked(true);
        m_pTypeBtnGroup->buttonClicked(0);
        m_pChkRound->setChecked(true);
        m_pChkFrontZero->setChecked(true);
        break;
    }
    case TLabel::String:
        break;
    case TLabel::Pwd:
    {
        m_pChkPwd->setChecked(true);
        break;
    }
    default:
        break;
    }
}

void AttriLabel::OutputModeChanged(int state)
{
    //reset界面,默认字符串输出
    m_pLineEditExpre->clear();
    m_pLineEditExpre->editingFinished();
    m_pLineEditUnit->clear();
    m_pLineEditUnit->editingFinished();
    m_pValueTypeBtnGroup->button(1)->setChecked(true);
    m_pValueTypeBtnGroup->buttonClicked(1);
    m_pGrpType->setEnabled(false);

    if(state >0)
    {
        m_pTabWidgetAtrri->addTab(m_pDisplayAttri,tr("显示输出"));
    }
    else
    {
        m_pTabWidgetAtrri->removeTab(1);
        m_pTextAttri->m_pTextEditText->setEnabled(true);
    }
}

void AttriLabel::SelectVar()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(tr("Num+String+Time+Count"));
    pDSV->SetIsRealtimeVar(true);
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name,const QString type)
    {
        m_strVar = tr("%1,%2").arg(name).arg(type);
        m_pLineEditExpre->setText(name);
        m_pLineEditExpre->editingFinished();
        m_pGrpType->setEnabled(true);
        m_pTextAttri->m_pTextEditText->setEnabled(false);

        int ntype = type.toInt();

        if(ntype > 0)
        {
            m_pValueTypeBtnGroup->button(0)->setChecked(true);
            m_pValueTypeBtnGroup->buttonClicked(0);
        }
        else
        {
            m_pTextAttri->SetText(tr("string"));
            TitleEditedFinished(tr("string"));
            m_pValueTypeBtnGroup->button(1)->setChecked(true);
            m_pValueTypeBtnGroup->buttonClicked(1);
        }
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

void AttriLabel::ValueTypeChanged(int id)
{
    m_pChkDouble->setChecked(true);
    m_pRdoDec->setChecked(true);
    m_pChkRound->setChecked(false);
    m_pChkFrontZero->setChecked(false);
    m_pChkPwd->setChecked(false);
    m_pSpnInt->setValue(0);
    m_pSpnDecimal->setValue(3);

    switch (id)
    {
    case 0:
    {
        m_pChkDouble->setEnabled(true);
        m_pChkNatural->setEnabled(true);
        m_pRdoDec->setEnabled(false);
        m_pRdoHex->setEnabled(false);
        m_pRdoBin->setEnabled(false);
        m_pChkRound->setEnabled(true);
        m_pChkFrontZero->setEnabled(true);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(true);
        m_pChkPwd->setEnabled(false);
        break;
    }
    case 1:
    {
        m_pChkDouble->setEnabled(false);
        m_pChkNatural->setEnabled(false);
        m_pRdoDec->setEnabled(false);
        m_pRdoHex->setEnabled(false);
        m_pRdoBin->setEnabled(false);
        m_pChkRound->setEnabled(false);
        m_pChkFrontZero->setEnabled(false);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);
        m_pChkPwd->setEnabled(true);
        break;
    }
    default:
        break;
    }
}

void AttriLabel::TypeChanged(int id)
{
    m_pRdoDec->setChecked(true);
    m_pChkRound->setChecked(false);
    m_pChkFrontZero->setChecked(false);

    switch (id)
    {
    case 0:
    {
        m_pChkNatural->setEnabled(true);
        m_pRdoDec->setEnabled(false);
        m_pRdoHex->setEnabled(false);
        m_pRdoBin->setEnabled(false);

        m_pChkRound->setEnabled(true);
        m_pChkFrontZero->setEnabled(true);

        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(true);
        m_pSpnDecimal->setValue(3);
        break;
    }
    case 1:
    {
        m_pRdoDec->setEnabled(true);
        m_pRdoHex->setEnabled(true);
        m_pRdoBin->setEnabled(true);

        m_pChkRound->setEnabled(false);
        m_pChkFrontZero->setEnabled(false);

        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);
        m_pSpnDecimal->setValue(0);
        break;
    }
    default:
        break;
    }
}

void AttriLabel::RuleChanged(int id)
{
    m_pChkFrontZero->setChecked(false);

    switch (id)
    {
    case 0:
    {
        m_pChkNatural->setEnabled(true);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);

        m_pChkFrontZero->setEnabled(false);
        m_pSpnInt->setValue(0);
        break;
    }
    case 1:
    {
        m_pChkNatural->setEnabled(false);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);

        m_pChkFrontZero->setEnabled(false);
        m_pSpnInt->setRange(0,10);
        m_pSpnInt->setValue(0);
        break;
    }
    case 2:
    {
        m_pChkNatural->setEnabled(false);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);

        m_pChkFrontZero->setEnabled(true);

        m_pSpnInt->setRange(0,31);
        m_pSpnInt->setValue(16);
        break;
    }
    default:
        break;
    }
}

void AttriLabel::FrontZeroCheckChanged(int state)
{
    if(state > 0)
    {
        if(m_pChkDouble->isChecked())
        {
            m_pSpnInt->setEnabled(true);
            m_pSpnInt->setRange(0,10);
        }
        else if(m_pChkNatural->isChecked())
        {
            m_pSpnInt->setEnabled(true);
            m_pSpnInt->setRange(0,31);
        }
    }
    else
    {
        m_pSpnInt->setEnabled(false);
        m_pSpnInt->setValue(0);
    }
}

void AttriLabel::TitleEditedFinished(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::Text,text,0);
}

void AttriLabel::FontSizeChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::FontSize,text,0);
}

void AttriLabel::FontColorChanged(QColor color)
{
    QString strColor = color.name(QColor::HexArgb);
    emit SendTAttribute(CoAttributeWidget::FontColor,strColor,0);
}

void AttriLabel::AlignVerChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignVertical,QString::number(id),0);
}

void AttriLabel::AlignHorChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignHorizontal,QString::number(id),0);
}

void AttriLabel::BGColorChanged(QColor color)
{
    emit SendTAttribute(CoAttributeWidget::BGColor,color.name(QColor::HexArgb),0);
}

void AttriLabel::BGImagePathChanged()
{
    emit SendTAttribute(CoAttributeWidget::PixMap,m_pBGAttri->GetImage(),0);
}

void AttriLabel::BGIsSelectImage(bool state)
{
    QString strIsChecked = "";
    if(state >0)
    {
        strIsChecked = "1";
    }
    emit SendTAttribute(CoAttributeWidget::SelectPix,strIsChecked,0);
}

void AttriLabel::OutputModeChangedExt(int state)
{
    if(state > 0)
        state = 1;
    emit SendTAttribute(CoAttributeWidget::OutputMode,QString::number(state),0);
}

void AttriLabel::VarChanged()
{
    QStringList list = m_strVar.split(',');
    if(list.size() > 0)
    {
        list[0] = m_pLineEditExpre->text();
    }
    QString str = QString("%1,%2").arg(list[0]).arg(list[1]);

    emit SendTAttribute(CoAttributeWidget::VarName,str,0);
}

void AttriLabel::UnitChanged()
{
    emit SendTAttribute(CoAttributeWidget::Unit,m_pLineEditUnit->text(),0);
}

void AttriLabel::ValueTypeChangedExt(int id)
{
    if(id > 0)
        emit SendTAttribute(CoAttributeWidget::ValueFormat,"8",0);
    emit SendTAttribute(CoAttributeWidget::ValueType,QString::number(id),0);
}

void AttriLabel::ValueFormatChanged()
{
    uchar uc = GetCurValueFormat();
    emit SendTAttribute(CoAttributeWidget::ValueFormat,QString::number(uc),0);
}

void AttriLabel::IntAccurecyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::IntAccurecy,text,0);
}

void AttriLabel::DecAccurecyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::DecAccurecy,text,0);
}

void AttriLabel::FreqChanged()
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,m_pLineEditFreq->text(),0);
}

void AttriLabel::JsTacticChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::JsTactic,text,0);
}

void AttriLabel::TabChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Index,QString::number(index),0);
}



