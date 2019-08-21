#include <QScrollArea>
#include <QGroupBox>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include "../TBasicControls/Curve.h"
#include "AttriCurve.h"

AttriCurve::AttriCurve(QWidget *parent)
    :GraphicAttrWidget(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pBaseWidget = CreateBaseAttri();
    QWidget *pCurveWidget = CreateCurveAttri();

    pTabWidget->addTab(pBaseWidget,tr("基本属性"));
    pTabWidget->addTab(pCurveWidget,tr("曲线属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

void AttriCurve::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TCurve *curve = qobject_cast<TCurve*>(itm);

    m_pLineEditTitle->setText(curve->GetTitle());
    m_pSpnXAxisCount->setValue(curve->GetXAxisCount());
    m_pSpnYAxisCount->setValue(curve->GetYAxisCount());
    QVector<QColor> vec;
    vec.clear();
    vec = curve->GetGridColor();
    m_pXAxisGridColor->SetColor(vec[0]);
    m_pXAxisSubGridColor->SetColor(vec[1]);
    m_pYAxisGridColor->SetColor(vec[2]);
    m_pYAxisSubGridColor->SetColor(vec[3]);
    m_pBGColor->SetColor(curve->GetBackColor().at(0));
    m_pChkGridVisible->setChecked(bool(curve->GetGridVisible()));

    QStringList list = curve->GetAxisLblTxt();
    m_pLineEditXAxisLbl->setText(list[0]);
    m_pLineEditYAxisLbl->setText(list[1]);
    vec.clear();
    vec = curve->GetAxisLblColor();
    m_pXAxisLblColor->SetColor(vec[0]);
    m_pYAxisLblColor->SetColor(vec[1]);
    m_pCmbTimeFormat->setCurrentText(curve->GetTimeFormat());
    m_pLineEditUnit->setText(curve->GetTimeUnit());
    m_pLineEditXMax->setText(QString::number(curve->GetXAxisLength()));
    m_pSpnDecimal->setValue(curve->GetDecAccurecy());
    m_pDVMax->setDecimals(curve->GetDecAccurecy());
    m_pDVMin->setDecimals(curve->GetDecAccurecy());
    m_pLineEditMax->setText(QString::number(curve->GetMax()));
    m_pLineEditMin->setText(QString::number(curve->GetMin()));
    m_pLineEditFreq->setText(QString::number(curve->GetAutoTime()));
    m_pControlVar->SetVarName(curve->GetCtrlVarName());
    m_pCurveVar->SetVarName(curve->GetVarName());
    m_pCurveColor->SetColor(curve->GetCurveColor());

    connect(m_pLineEditTitle,&QLineEdit::editingFinished,this,&AttriCurve::TitleChanged);
    connect(m_pSpnXAxisCount,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,&AttriCurve::XAxisCountChanged);
    connect(m_pSpnYAxisCount,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,&AttriCurve::YAxisCountChanged);
    connect(m_pXAxisGridColor,&ColorSelect::ColorChanged,this,&AttriCurve::GridColorChanged);
    connect(m_pXAxisSubGridColor,&ColorSelect::ColorChanged,this,&AttriCurve::GridColorChanged);
    connect(m_pYAxisGridColor,&ColorSelect::ColorChanged,this,&AttriCurve::GridColorChanged);
    connect(m_pYAxisSubGridColor,&ColorSelect::ColorChanged,this,&AttriCurve::GridColorChanged);
    connect(m_pBGColor,&ColorSelect::ColorChanged,this,&AttriCurve::BGColorChanged);
    connect(m_pChkGridVisible,&QCheckBox::stateChanged,this,&AttriCurve::GridVisiableStateChanged);
    connect(m_pLineEditXAxisLbl,&QLineEdit::editingFinished,this,&AttriCurve::AxisLblTxtChanged);
    connect(m_pLineEditYAxisLbl,&QLineEdit::editingFinished,this,&AttriCurve::AxisLblTxtChanged);
    connect(m_pXAxisLblColor,&ColorSelect::ColorChanged,this,&AttriCurve::AxisLblColorChanged);
    connect(m_pYAxisLblColor,&ColorSelect::ColorChanged,this,&AttriCurve::AxisLblColorChanged);
    connect(m_pCmbTimeFormat,static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
            this,&AttriCurve::TimeFormatChanged);
    connect(m_pLineEditUnit,&QLineEdit::editingFinished,this,&AttriCurve::UnitChanged);
    connect(m_pLineEditXMax,&QLineEdit::editingFinished,this,&AttriCurve::XAxisLengthChanged);
    connect(m_pSpnDecimal,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,&AttriCurve::DecAccurecyChanged);
    connect(m_pLineEditMax,&QLineEdit::editingFinished,this,&AttriCurve::MaxValueChanged);
    connect(m_pLineEditMin,&QLineEdit::editingFinished,this,&AttriCurve::MinValueChanged);
    connect(m_pControlVar,&VarSelectedAttri::SendTextFinfished,this,&AttriCurve::CtrlVarNameChanged);
    connect(m_pCurveVar,&VarSelectedAttri::SendTextFinfished,this,&AttriCurve::VarNameChanged);
    connect(m_pCurveColor,&ColorSelect::ColorChanged,this,&AttriCurve::CurveColorChanged);

    connect(this,&AttriCurve::SendTAttribute,curve,&TCurve::SetAttributeEvent);
}

int AttriCurve::GetStringToEnumEx(QString str)
{
    const QMetaObject metaObject = AttriCurve::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("Grid");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    QByteArray ba = str.toLatin1();
    return en.keysToValue(ba.data());
}

QWidget *AttriCurve::CreateBaseAttri()
{
    QWidget *pBaseAttri = new QWidget();
    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pBaseAttri->setLayout(pVBoxLayout);

    QGroupBox *pGrp = new QGroupBox(tr("背景网格"));
    QGridLayout *pGridLayout = new QGridLayout();
    pGrp->setLayout(pGridLayout);

    m_pSpnXAxisCount  = new QSpinBox();
    m_pSpnXAxisCount->setRange(1,20);

    m_pXAxisGridColor = AttriBase::colorSelect();
    m_pXAxisGridColor->setObjectName(tr("XAxisGrid"));
    m_pXAxisGridColor->m_pLblText->hide();

    m_pXAxisSubGridColor = AttriBase::colorSelect();
    m_pXAxisSubGridColor->setObjectName(tr("XAxisSubGrid"));
    m_pXAxisSubGridColor->m_pLblText->hide();

    m_pSpnYAxisCount  = new QSpinBox();
    m_pSpnYAxisCount->setRange(0,20);

    m_pYAxisGridColor = AttriBase::colorSelect();
    m_pYAxisGridColor->setObjectName(tr("YAxisGrid"));
    m_pYAxisGridColor->m_pLblText->hide();

    m_pYAxisSubGridColor = AttriBase::colorSelect();
    m_pYAxisSubGridColor->setObjectName(tr("YAxisSubGrid"));
    m_pYAxisSubGridColor->m_pLblText->hide();

    m_pBGColor = AttriBase::colorSelect();
    m_pBGColor->m_pLblText->hide();

    m_pChkGridVisible = TBase::CheckBox(tr("显示网格"),tr(""));

    pGridLayout->addWidget(new QLabel(tr("X主划线")),0,0,1,2);
    pGridLayout->addWidget(new QLabel(tr("数目")),0,2);
    pGridLayout->addWidget(m_pSpnXAxisCount,0,3);
    pGridLayout->addWidget(m_pXAxisGridColor,0,4,1,2);
    pGridLayout->addWidget(new QLabel(tr("X次划线")),1,0,1,2);
    pGridLayout->addWidget(m_pXAxisSubGridColor,1,2,1,2);
    pGridLayout->addWidget(new QLabel(tr("Y主划线")),2,0,1,2);
    pGridLayout->addWidget(new QLabel(tr("数目")),2,2);
    pGridLayout->addWidget(m_pSpnYAxisCount,2,3);
    pGridLayout->addWidget(m_pYAxisGridColor,2,4,1,2);
    pGridLayout->addWidget(new QLabel(tr("Y次划线")),3,0,1,2);
    pGridLayout->addWidget(m_pYAxisSubGridColor,3,2,1,2);
    pGridLayout->setRowMinimumHeight(0,30);
    pGridLayout->setRowMinimumHeight(1,30);
    pGridLayout->setRowMinimumHeight(2,30);
    pGridLayout->setRowMinimumHeight(3,30);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->addWidget(new QLabel(tr("背景颜色")));
    pHBoxLayout->addWidget(m_pBGColor);

    QHBoxLayout *pTitleLayout = new QHBoxLayout();
    m_pLineEditTitle = TBase::LineEdit(tr(""));
    pTitleLayout->addWidget(new QLabel(tr("标题")));
    pTitleLayout->addWidget(m_pLineEditTitle);

    pVBoxLayout->addLayout(pTitleLayout);
    pVBoxLayout->addWidget(pGrp);
    pVBoxLayout->addLayout(pHBoxLayout);
    pVBoxLayout->addWidget(m_pChkGridVisible);
    pVBoxLayout->addStretch();

    return pBaseAttri;
}

QWidget *AttriCurve::CreateCurveAttri()
{
    QWidget *pCurveAttri = new QWidget();
    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pCurveAttri->setLayout(pVBoxLayout);

    QTabWidget *pTabAxis = new QTabWidget();
//    pTabAxis->setFixedHeight(110);
    QWidget *pXAxis = new QWidget();
    QWidget *pYAxis = new QWidget();
    pTabAxis->addTab(pXAxis,tr("X轴"));
    pTabAxis->addTab(pYAxis,tr("Y轴"));

    //X轴标注
    m_pLineEditXAxisLbl = TBase::LineEdit(tr("XAxisLbl"));
    m_pXAxisLblColor = AttriBase::colorSelect();
    m_pXAxisLblColor->setObjectName(tr("XAxisLbl"));
    m_pXAxisLblColor->m_pLblText->hide();

    m_pCmbTimeFormat = TBase::ComboBox(tr(""));
    QStringList list;
    list << tr("%s") << tr("%m") << tr("%h")
         << tr("%m:%s") << tr("%h:%m") << tr("%h:%m:%s");
    m_pCmbTimeFormat->addItems(list);
    m_pLineEditUnit = TBase::LineEdit(tr(""));
    m_pLineEditXMax = TBase::LineEdit(tr(""));
    QIntValidator *pIV = new QIntValidator();
    m_pLineEditXMax->setValidator(pIV);

    QGridLayout *pXAxisLblLayout = new QGridLayout();
    pXAxisLblLayout->addWidget(new QLabel(tr("文本")),0,0);
    pXAxisLblLayout->addWidget(m_pLineEditXAxisLbl,0,1);
    pXAxisLblLayout->addWidget(new QLabel(tr("标注颜色")),1,0);
    pXAxisLblLayout->addWidget(m_pXAxisLblColor,1,1);
    pXAxisLblLayout->addWidget(new QLabel(tr("时间格式")),2,0);
    pXAxisLblLayout->addWidget(m_pCmbTimeFormat,2,1);
    pXAxisLblLayout->addWidget(new QLabel(tr("时间单位")),3,0);
    pXAxisLblLayout->addWidget(m_pLineEditUnit,3,1);
    pXAxisLblLayout->addWidget(new QLabel(tr("X轴长度")),4,0);
    pXAxisLblLayout->addWidget(m_pLineEditXMax,4,1);

    pXAxis->setLayout(pXAxisLblLayout);

    //Y轴标注
    m_pLineEditYAxisLbl = TBase::LineEdit(tr("YAxisLbl"));
    m_pYAxisLblColor = AttriBase::colorSelect();
    m_pYAxisLblColor->setObjectName(tr("YAxisLbl"));
    m_pYAxisLblColor->m_pLblText->hide();

    m_pSpnDecimal  = new QSpinBox();
    m_pSpnDecimal->setRange(1,10);
    connect(m_pSpnDecimal,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,&AttriCurve::ValidatorChanged);

    m_pLineEditMax = TBase::LineEdit(tr(""));
    m_pLineEditMin = TBase::LineEdit(tr(""));
    m_pDVMax = new QDoubleValidator();
    m_pDVMax->setDecimals(0);
    m_pLineEditMax->setValidator(m_pDVMax);
    m_pDVMin = new QDoubleValidator();
    m_pDVMin->setDecimals(0);
    m_pLineEditMin->setValidator(m_pDVMin);

    QGridLayout *pYAxisLblLayout = new QGridLayout();
    pYAxisLblLayout->addWidget(new QLabel(tr("文本")),0,0);
    pYAxisLblLayout->addWidget(m_pLineEditYAxisLbl,0,1);
    pYAxisLblLayout->addWidget(new QLabel(tr("标注颜色")),1,0);
    pYAxisLblLayout->addWidget(m_pYAxisLblColor,1,1);
    pYAxisLblLayout->addWidget(new QLabel(tr("小数位数")),2,0);
    pYAxisLblLayout->addWidget(m_pSpnDecimal,2,1);
    pYAxisLblLayout->addWidget(new QLabel(tr("最小值")),3,0);
    pYAxisLblLayout->addWidget(m_pLineEditMin,3,1);
    pYAxisLblLayout->addWidget(new QLabel(tr("最大值")),4,0);
    pYAxisLblLayout->addWidget(m_pLineEditMax,4,1);
    pYAxis->setLayout(pYAxisLblLayout);

    //曲线
    m_pControlVar = AttriBase::varSelectedAttri();
    m_pControlVar->SetSelectVarType(tr("Switch"));
    m_pCurveVar = AttriBase::varSelectedAttri();
    m_pCurveVar->SetSelectVarType(tr("Num"));
    m_pCurveColor = AttriBase::colorSelect();
    m_pCurveColor->m_pLblText->hide();
    m_pLineEditFreq = TBase::LineEdit(tr(""));
    QIntValidator *pIV1 = new QIntValidator();
    m_pLineEditFreq->setValidator(pIV1);

    QGridLayout *pCurveLayout = new QGridLayout();
    pCurveLayout->addWidget(new QLabel(tr("刷新频率")),0,0);
    pCurveLayout->addWidget(m_pLineEditFreq,0,1,1,2);
    pCurveLayout->addWidget(new QLabel(tr("控制量")),1,0);
    pCurveLayout->addWidget(m_pControlVar,1,1,1,2);
    pCurveLayout->addWidget(new QLabel(tr("曲线")),2,0);
    pCurveLayout->addWidget(m_pCurveVar,2,1);
    pCurveLayout->addWidget(m_pCurveColor,2,2);

    pVBoxLayout->addWidget(pTabAxis);
    pVBoxLayout->addLayout(pCurveLayout);
    pVBoxLayout->addStretch();

    return pCurveAttri;
}

void AttriCurve::ValidatorChanged(int value)
{
    double dMax = m_pLineEditMax->text().toDouble();
    double dMin = m_pLineEditMin->text().toDouble();

    m_pDVMax->setDecimals(value);
    m_pDVMin->setDecimals(value);

    m_pLineEditMax->setText(QString::number(dMax));
    m_pLineEditMin->setText(QString::number(dMin));
}

void AttriCurve::TitleChanged()
{
    emit SendTAttribute(CoAttributeWidget::Title,m_pLineEditTitle->text(),0);
}

void AttriCurve::AxisLblTxtChanged()
{
    uchar uc = 0;
    QString str = m_pLineEditXAxisLbl->text();
    if(sender()->objectName() == tr("YAxisLbl"))
    {
        uc = 1;
        str = m_pLineEditYAxisLbl->text();
    }
    emit SendTAttribute(CoAttributeWidget::AxisLblTxt,str,uc);
}

void AttriCurve::XAxisCountChanged(int value)
{
    emit SendTAttribute(CoAttributeWidget::XAxisCount,QString::number(value),0);
}

void AttriCurve::YAxisCountChanged(int value)
{
    emit SendTAttribute(CoAttributeWidget::YAxisCount,QString::number(value),0);
}

void AttriCurve::GridColorChanged(QColor color)
{
    QString strObjectName = sender()->objectName();
    int type = GetStringToEnumEx(strObjectName);
    emit SendTAttribute(CoAttributeWidget::GridColor,color.name(QColor::HexArgb),type);
}

void AttriCurve::BGColorChanged(QColor color)
{
    emit SendTAttribute(CoAttributeWidget::BGColor,color.name(QColor::HexArgb),0);
}

void AttriCurve::GridVisiableStateChanged(int state)
{
    if(state > 0)
        state = 1;
    emit SendTAttribute(CoAttributeWidget::GridVisiableState,QString::number(state),0);
}

void AttriCurve::AxisLblColorChanged(QColor color)
{
    uchar uc = 0;
    if(sender()->objectName() == tr("YAxisLbl"))
        uc = 1;

    emit SendTAttribute(CoAttributeWidget::AxisLblColor,color.name(QColor::HexArgb),uc);
}

void AttriCurve::TimeFormatChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::TimeFormat,text,0);
}

void AttriCurve::UnitChanged()
{
    emit SendTAttribute(CoAttributeWidget::Unit,m_pLineEditUnit->text(),0);
}

void AttriCurve::XAxisLengthChanged()
{
    if(m_pLineEditXMax->text().isEmpty())
        return;
    emit SendTAttribute(CoAttributeWidget::XAxisLength,m_pLineEditXMax->text(),0);
}

void AttriCurve::DecAccurecyChanged(int value)
{
    emit SendTAttribute(CoAttributeWidget::DecAccurecy,QString::number(value),0);
}

void AttriCurve::MaxValueChanged()
{
    double dMax = m_pLineEditMax->text().toDouble();
    double dMin = m_pLineEditMin->text().toDouble();
    if(dMax < dMin)
        m_pLineEditMax->setText(QString::number(++dMin));

    emit SendTAttribute(CoAttributeWidget::MaxValue,m_pLineEditMax->text(),0);
}

void AttriCurve::MinValueChanged()
{
    double dMax = m_pLineEditMax->text().toDouble();
    double dMin = m_pLineEditMin->text().toDouble();
    if(dMin > dMax)
        m_pLineEditMin->setText(QString::number(--dMax));

    emit SendTAttribute(CoAttributeWidget::MinValue,m_pLineEditMin->text(),0);
}

void AttriCurve::FreqChanged()
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,m_pLineEditFreq->text(),0);
}

void AttriCurve::CtrlVarNameChanged(const QString&varInfo)
{
    emit SendTAttribute(CoAttributeWidget::CtrlVarName,varInfo,0);
}

void AttriCurve::VarNameChanged(const QString&varInfo)
{
    emit SendTAttribute(CoAttributeWidget::VarName,varInfo,0);
}

void AttriCurve::CurveColorChanged(QColor color)
{
    emit SendTAttribute(CoAttributeWidget::CurveColor,color.name(QColor::HexArgb),0);
}

