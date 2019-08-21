#include <QDoubleValidator>
#include <QScrollArea>
#include <QStringList>
#include "TBasicControls/TextEdit.h"
#include "AttriTextEdit.h"

AttriTextEdit::AttriTextEdit(QWidget *parent) : GraphicAttrWidget(parent)
{
    m_VarType = -1;
    m_pTabWidgetAtrri = new QTabWidget();
    connect(m_pTabWidgetAtrri,&QTabWidget::currentChanged,this,&AttriTextEdit::TabChanged);
    m_pTabWidgetAtrri->setFixedWidth(260);
    QWidget *pBaseAttri = CreateBaseAttri();
    QWidget *pOperateAttri = CreateOperateAttri();

    m_pEnableAttri = AttriBase::enableAttri();
    m_pEnableAttri->SetButtonText(tr("输入框"));
    m_pEnableAttri->SetSelectVarType(tr("Num+Switch+String"));

    m_pTabWidgetAtrri->addTab(pBaseAttri,tr("基本属性"));
    m_pTabWidgetAtrri->addTab(pOperateAttri,tr("操作属性"));
    m_pTabWidgetAtrri->addTab(m_pEnableAttri,tr("可用属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(m_pTabWidgetAtrri);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);

}

void AttriTextEdit::SetItem(CoItemEx *Item)
{
    GraphicAttrWidget::SetItem(Item);
    TTextEdit *txt = qobject_cast<TTextEdit*>(Item);
    m_pTabWidgetAtrri->setCurrentIndex(txt->GetAttriIndex());

    //基础属性
    m_pFontAttri->SetFontSize(QString::number(txt->GetFontSize()));
    m_pFontAttri->SetFontColor(txt->GetFontColor());
    m_pFontAttri->SetAligHor(txt->GetFontAlignHor());
    m_pFontAttri->SetAlignVer(txt->GetFontAlignVer());

    QVector<QColor> backColorVec = txt->GetBackColor();
    m_pBGAttri->SetColor(backColorVec[0]);

    QVector<bool> selectPixVec = txt->GetSelectPix();
    m_pBGAttri->SetImageCheckState(selectPixVec[0]);

    QVector<QString> pixFileVec = txt->GetPixFiles();
    m_pBGAttri->SetImage(pixFileVec[0]);

    //操作属性
    m_pJSAttri->SetJsTactic(txt->GetJsTactic());
    m_VarType = txt->GetVarType();
    m_pInVarSelectAttri->SetVarName(txt->GetInputVarName());
    m_pOutVarSelectAttri->SetVarName(txt->GetOutputVarName());
    m_pInVarSelectAttri->SetVarType(txt->GetVarType());
    m_pOutVarSelectAttri->SetVarType(txt->GetVarType());

    m_pLineEditUnit->setText(txt->GetUnit());
    m_pLineEditFreq->setText(QString::number(txt->GetAutoTime()));

    if(txt->GetValueFormat() > 4)
        SetWidgetsByValueType(0); //字符型
    else
        SetWidgetsByValueType(1); //数值型

    SetWidgetsByEnum(txt->GetValueFormat());
    m_pSpnInt->setValue(txt->GetIntAccurecy());
    m_pSpnDecimal->setValue(txt->GetDecAccurecy());

    m_pLineEditMax->setText(QString::number(txt->GetMaxValue()));
    m_pLineEditMin->setText(QString::number(txt->GetMinValue()));

    //可用性属性
    m_pEnableAttri->SetEnableExpre(txt->GetEnableExpre());
    m_pEnableAttri->SetEnableState(txt->GetEnableState());

    connect(m_pFontAttri,&FontAttri::FontSizeChanged,this,&AttriTextEdit::FontSizeChanged);
    connect(m_pFontAttri,&FontAttri::FontColorChanged,this,&AttriTextEdit::FontColorChanged);
    connect(m_pFontAttri,&FontAttri::AlignHorChanged,this,&AttriTextEdit::AlignHorChanged);
    connect(m_pFontAttri,&FontAttri::AlignVerChanged,this,&AttriTextEdit::AlignVerChanged);
    connect(m_pBGAttri,&BackgroundAttri::BackgroundColorChanged,this,&AttriTextEdit::BGColorChanged);
    connect(m_pBGAttri,&BackgroundAttri::ImageCheckStateChanged,this,&AttriTextEdit::BGIsSelectImage);
    connect(m_pBGAttri,&BackgroundAttri::ImageChanged,this,&AttriTextEdit::BGImagePathChanged);

    connect(m_pInVarSelectAttri,&VarSelectedAttri::SendTextFinfished,this,&AttriTextEdit::VarChanged);
    connect(m_pOutVarSelectAttri,&VarSelectedAttri::SendTextFinfished,this,&AttriTextEdit::VarChanged);
    connect(m_pLineEditUnit,&QLineEdit::editingFinished,this,&AttriTextEdit::UnitChanged);
    connect(m_pChkFixedPoint,&QCheckBox::stateChanged,this,&AttriTextEdit::ValueFormatChanged);
    connect(m_pChkRound,&QCheckBox::stateChanged,this,&AttriTextEdit::ValueFormatChanged);
    connect(m_pChkFrontZero,&QCheckBox::stateChanged,this,&AttriTextEdit::ValueFormatChanged);
    connect(m_pChkPwd,&QCheckBox::stateChanged,this,&AttriTextEdit::ValueFormatChanged);
    connect(m_pSpnInt,SIGNAL(valueChanged(const QString&)),this,SLOT(IntAccurecyChanged(const QString&)));
    connect(m_pSpnDecimal,SIGNAL(valueChanged(const QString&)),this,SLOT(DecAccurecyChanged(const QString&)));
    connect(m_pLineEditMax,&QLineEdit::editingFinished,this,&AttriTextEdit::MaxValChanged);
    connect(m_pLineEditMin,&QLineEdit::editingFinished,this,&AttriTextEdit::MinValChanged);
    connect(m_pJSAttri,&JsTacticAttri::JsTacticChanged,this,&AttriTextEdit::JsTacticChanged);
    connect(m_pLineEditFreq,&QLineEdit::editingFinished,this,&AttriTextEdit::FreqChanged);

    connect(m_pEnableAttri,&EnableAttri::SendTextFinfished,this,&AttriTextEdit::EnableExpreChanged);
    connect(m_pEnableAttri,&EnableAttri::EnableStateChanged,this,&AttriTextEdit::EnableStateChanged);

    //TextEidt
    connect(this,&AttriTextEdit::SendTAttribute,txt,&TTextEdit::SetAttributeEvent);

    txt->SetPreviewMode(true);
}

QWidget *AttriTextEdit::CreateBaseAttri()
{
    QWidget *pBaseAttri = new QWidget();

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
    pBaseLayout->addWidget(pGroupFont);
    pBaseLayout->addWidget(pGroupBG);
    pBaseLayout->setAlignment(Qt::AlignTop);

    pBaseAttri->setLayout(pBaseLayout);

    return pBaseAttri;
}

QWidget *AttriTextEdit::CreateOperateAttri()
{
    QWidget *pDisplayAttri = new QWidget();
    QVBoxLayout *pDisplayLayout = new QVBoxLayout();
    pDisplayAttri->setLayout(pDisplayLayout);

    QGroupBox *pGroupJS = new QGroupBox(tr("脚本策略"));
    pDisplayLayout->addWidget(pGroupJS);
    m_pJSAttri = AttriBase::jsTacticAttri();
    m_pJSAttri->setContentsMargins(-9,-8,-9,-8);
    m_pJSAttri->setObjectName(tr("JSTactic"));

    QHBoxLayout *pJSLayout = new QHBoxLayout();
    pJSLayout->addWidget(m_pJSAttri);
    pGroupJS->setLayout(pJSLayout);

    //表达式
    QGroupBox *pExpressGroup = new QGroupBox(tr("对应数据对象"));
    pDisplayLayout->addWidget(pExpressGroup);
    QGridLayout *pExpressLayout = new QGridLayout();
    pExpressGroup->setLayout(pExpressLayout);

    m_pInVarSelectAttri = AttriBase::varSelectedAttri();
    m_pInVarSelectAttri->setContentsMargins(-9,-8,-9,-8);
    m_pInVarSelectAttri->SetSelectVarType("Num+String");
    connect(m_pInVarSelectAttri,&VarSelectedAttri::SendTextFinfished,this,&AttriTextEdit::SetWidgetsByVar);
    connect(m_pInVarSelectAttri,&VarSelectedAttri::SendTextFinfished,this,&AttriTextEdit::InputVarChanged);

    m_pOutVarSelectAttri = AttriBase::varSelectedAttri();
    m_pOutVarSelectAttri->setContentsMargins(-9,-8,-9,-8);
    m_pOutVarSelectAttri->SetSelectVarType("Num+String");
    connect(m_pOutVarSelectAttri,&VarSelectedAttri::SendTextFinfished,this,&AttriTextEdit::OutputVarChanged);

    m_pLineEditUnit = TBase::LineEdit(tr("Unit"));

    pExpressLayout->addWidget(new QLabel(tr("写")),0,0);
    pExpressLayout->addWidget(m_pInVarSelectAttri,0,1);
    pExpressLayout->addWidget(new QLabel(tr("读")),1,0);
    pExpressLayout->addWidget(m_pOutVarSelectAttri,1,1);
    pExpressLayout->addWidget(new QLabel(tr("单位")),2,0);
    pExpressLayout->addWidget(m_pLineEditUnit,2,1);


    //输出格式
    QGroupBox *pGrpType = new QGroupBox(tr(""));
    pDisplayLayout->addWidget(pGrpType);
    QGridLayout *pFormatLayout = new QGridLayout();
    pGrpType->setLayout(pFormatLayout);

    m_pChkFixedPoint = TBase::CheckBox(tr("自然小数位"),tr("ValueFormat"));
    m_pChkPwd = TBase::CheckBox(tr("密码"),tr("ValueFormat"));
    m_pChkRound = TBase::CheckBox(tr("四舍五入"),tr("ValueFormat"));
    m_pChkFrontZero = TBase::CheckBox(tr("前导0"),tr("ValueFormat"));

    connect(m_pChkFixedPoint,&QCheckBox::stateChanged,this,&AttriTextEdit::FixedPointCheckChanged);
    connect(m_pChkFrontZero,&QCheckBox::stateChanged,this,&AttriTextEdit::FrontZeroCheckChanged);

    QLabel *pLblInt = TBase::Label(tr("整数位数"),tr(""));
    m_pSpnInt = new QSpinBox();
    m_pSpnInt->setObjectName(tr("IntAccurecy"));
    m_pSpnInt->setRange(0,10);
    QLabel *pLblDecimal = TBase::Label(tr("小数位数"),tr(""));
    m_pSpnDecimal = new QSpinBox();
    m_pSpnDecimal->setObjectName(tr("DecAccurecy"));
    m_pSpnDecimal->setRange(0,6);
    m_pSpnDecimal->setValue(3);

    pFormatLayout->addWidget(m_pChkFixedPoint,0,0,1,3);
    pFormatLayout->addWidget(m_pChkPwd,0,3,1,3);
    pFormatLayout->addWidget(m_pChkFrontZero,1,0,1,3);
    pFormatLayout->addWidget(m_pChkRound,1,3,1,3);
    pFormatLayout->addWidget(pLblInt,2,0);
    pFormatLayout->addWidget(m_pSpnInt,2,2,1,4);
    pFormatLayout->addWidget(pLblDecimal,3,0);
    pFormatLayout->addWidget(m_pSpnDecimal,3,2,1,4);

    QGroupBox *pGrpLimit = new QGroupBox(tr(""));
    pDisplayLayout->addWidget(pGrpLimit);
    QGridLayout *pLimitLayout = new QGridLayout();
    pGrpLimit->setLayout(pLimitLayout);
    QLabel *pLblMax = TBase::Label(tr("最大值"),tr(""));
    QLabel *pLblMin = TBase::Label(tr("最小值"),tr(""));
    m_pLineEditMax = TBase::LineEdit(tr(""));
    m_pLineEditMin = TBase::LineEdit(tr(""));
    QDoubleValidator *pDV = new QDoubleValidator();
    pDV->setDecimals(6);
    QDoubleValidator *pDV1 = new QDoubleValidator();
    pDV1->setDecimals(6);
    m_pLineEditMax->setValidator(pDV);
    m_pLineEditMin->setValidator(pDV1);

    pLimitLayout->addWidget(pLblMax,0,0);
    pLimitLayout->addWidget(m_pLineEditMax,0,1);
    pLimitLayout->addWidget(pLblMin,1,0);
    pLimitLayout->addWidget(m_pLineEditMin,1,1);

    m_pLineEditFreq = TBase::LineEdit(tr(""));
    QIntValidator *pIV = new QIntValidator();
    m_pLineEditFreq->setValidator(pIV);

    QHBoxLayout *pFreqLayout = new QHBoxLayout();
    pDisplayLayout->addLayout(pFreqLayout);
    pFreqLayout->addWidget(new QLabel(tr("刷新频率")));
    pFreqLayout->addWidget(m_pLineEditFreq);
    pFreqLayout->addWidget(new QLabel(tr("ms")));

    pDisplayLayout->addStretch();
    return pDisplayAttri;
}

void AttriTextEdit::SetWidgetsByVar(const QString &var)
{
    QStringList list = var.split(",");
    if(list.size() < 1)
        return;
    SetWidgetsByValueType(list[1].toInt());
}

void AttriTextEdit::JsTacticChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::JsTactic,text,0);
}

void AttriTextEdit::InputVarChanged(const QString &varInfo)
{
    QStringList list = varInfo.split(",");
    if(list.size() > 0)
    {
        if(m_VarType != list.at(1).toShort())
            m_pOutVarSelectAttri->SetVarName("");
        m_VarType = list.at(1).toShort();
    }
}

void AttriTextEdit::OutputVarChanged(const QString &varInfo)
{
    if(m_VarType < 0)
    {
        m_pOutVarSelectAttri->SetVarName("");
        return;
    }

    QStringList list = varInfo.split(",");
    if(list.size() > 0)
    {
        if(m_VarType != list.at(1).toShort())
        {
            m_pOutVarSelectAttri->SetVarName("");
        }
    }
}

void AttriTextEdit::FreqChanged()
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,m_pLineEditFreq->text(),0);
}

uchar AttriTextEdit::GetCurValueFormat()
{
    uchar uc = 0;
    if(m_pChkFixedPoint->isChecked())
    {
        uc = 1;
    }

    if(m_pChkRound->isChecked() && m_pChkFrontZero->isChecked())
        uc = 4;
    else if(m_pChkRound->isChecked())
        uc = 3;
    else if(m_pChkFrontZero->isChecked())
        uc = 2;

    //5是普通字符串,6是密码字符串
    if(m_pChkPwd->isChecked())
        uc = 6;
    else if(m_pChkPwd->isEnabled() && !m_pChkPwd->isChecked())
        uc = 5;

    return uc;
}

void AttriTextEdit::SetWidgetsByEnum(int typ)
{
    ValueType euType = ValueType(typ);
    switch (euType)
    {
    case Double:
        break;
    case FixedPoint:
    {
        m_pChkFixedPoint->setChecked(true);
        break;
    }
    case FrontZero:
    {
        m_pChkFrontZero->setChecked(true);
        break;
    }
    case Round:
    {
        m_pChkRound->setChecked(true);
        break;
    }
    case RoundAndFrontZero:
    {
        m_pChkRound->setChecked(true);
        m_pChkFrontZero->setChecked(true);
        break;
    }
    case String:
        break;
    case Pwd:
    {
        m_pChkPwd->setChecked(true);
        break;
    }
    default:
        break;
    }
}

void AttriTextEdit::SetWidgetsByValueType(int type)
{
    if(type > 0)
    {
        //数值型
        m_pChkFixedPoint->setChecked(false);
        m_pChkFrontZero->setChecked(false);
        m_pChkRound->setChecked(false);
        m_pChkPwd->setChecked(false);

        m_pChkFixedPoint->setEnabled(true);
        m_pChkFrontZero->setEnabled(true);
        m_pChkRound->setEnabled(true);
        m_pChkPwd->setEnabled(false);

        m_pSpnInt->setEnabled(false);
        m_pSpnInt->setValue(0);
        m_pSpnDecimal->setEnabled(true);
        m_pSpnDecimal->setValue(3);

        m_pLineEditMax->setEnabled(true);
        m_pLineEditMin->setEnabled(true);
    }
    else
    {
        //字符型
        m_pChkFixedPoint->setChecked(false);
        m_pChkFrontZero->setChecked(false);
        m_pChkRound->setChecked(false);
        m_pChkPwd->setChecked(false);

        m_pChkFixedPoint->setEnabled(false);
        m_pChkFrontZero->setEnabled(false);
        m_pChkRound->setEnabled(false);
        m_pChkPwd->setEnabled(true);

        m_pSpnInt->setEnabled(false);
        m_pSpnInt->setValue(0);
        m_pSpnDecimal->setEnabled(false);
        m_pSpnDecimal->setValue(3);

        m_pLineEditMax->setEnabled(false);
        m_pLineEditMin->setEnabled(false);
    }
}

void AttriTextEdit::FixedPointCheckChanged(int state)
{
    m_pChkFrontZero->setChecked(false);
    m_pChkRound->setChecked(false);

    m_pSpnInt->setValue(0);
    m_pSpnDecimal->setValue(3);

    if(state > 0)
    {
        m_pChkFrontZero->setEnabled(false);
        m_pChkRound->setEnabled(false);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(false);
    }
    else
    {
        m_pChkFrontZero->setEnabled(true);
        m_pChkRound->setEnabled(true);
        m_pSpnInt->setEnabled(false);
        m_pSpnDecimal->setEnabled(true);
    }
}

void AttriTextEdit::FrontZeroCheckChanged(int state)
{
    m_pSpnInt->setValue(0);

    if(state > 0)
        m_pSpnInt->setEnabled(true);
    else
        m_pSpnInt->setEnabled(false);
}

void AttriTextEdit::PowerChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Power,QString::number(index),0);
}

void AttriTextEdit::FontSizeChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::FontSize,text,0);
}

void AttriTextEdit::FontColorChanged(QColor color)
{
    QString strColor = color.name(QColor::HexArgb);
    emit SendTAttribute(CoAttributeWidget::FontColor,strColor,0);
}

void AttriTextEdit::AlignVerChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignVertical,QString::number(id),0);
}

void AttriTextEdit::AlignHorChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::AlignHorizontal,QString::number(id),0);
}

void AttriTextEdit::BGColorChanged(QColor color)
{
    emit SendTAttribute(CoAttributeWidget::BGColor,color.name(QColor::HexArgb),0);
}

void AttriTextEdit::BGImagePathChanged()
{
    emit SendTAttribute(CoAttributeWidget::PixMap,m_pBGAttri->GetImage(),0);
}

void AttriTextEdit::BGIsSelectImage(bool state)
{
    QString strIsChecked = "";
    if(state >0)
    {
        strIsChecked = "1";
    }
    emit SendTAttribute(CoAttributeWidget::SelectPix,strIsChecked,0);
}

void AttriTextEdit::VarChanged(const QString &varInfo)
{
    QStringList list = varInfo.split(",");
    QString str = list[0] + "," + m_pOutVarSelectAttri->GetVarName() + "," + QString::number(m_VarType);
    emit SendTAttribute(CoAttributeWidget::VarName,str,0);
    ValueFormatChanged();
}

void AttriTextEdit::UnitChanged()
{
    emit SendTAttribute(CoAttributeWidget::Unit,m_pLineEditUnit->text(),0);
}

void AttriTextEdit::ValueFormatChanged()
{
    uchar uc = GetCurValueFormat();
    emit SendTAttribute(CoAttributeWidget::ValueFormat,QString::number(uc),0);
}

void AttriTextEdit::IntAccurecyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::IntAccurecy,text,0);
}

void AttriTextEdit::DecAccurecyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::DecAccurecy,text,0);
}

void AttriTextEdit::MaxValChanged()
{
    emit SendTAttribute(CoAttributeWidget::MaxValue,m_pLineEditMax->text(),0);
}

void AttriTextEdit::MinValChanged()
{
    emit SendTAttribute(CoAttributeWidget::MinValue,m_pLineEditMin->text(),0);
}

void AttriTextEdit::EnableExpreChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,text,0);
}

void AttriTextEdit::EnableStateChanged(int id)
{
    emit SendTAttribute(CoAttributeWidget::EnableState,QString::number(id),0);
}

void AttriTextEdit::TabChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Index,QString::number(index),0);
}
