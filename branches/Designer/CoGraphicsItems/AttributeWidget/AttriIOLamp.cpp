#include <QScrollArea>
#include <QGridLayout>
#include "SelectVar/DialogSelectVar.h"
#include "../TBasicControls/IOLamp.h"
#include "AttriIOLamp.h"

AttriIOLamp::AttriIOLamp(QWidget *parent):
    GraphicAttrWidget(parent)
{
    m_strVar = tr(",");

    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pAttriWidget = CreateAttri();
    pTabWidget->addTab(pAttriWidget,tr("属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

QWidget *AttriIOLamp::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pLineEditVar = TBase::LineEdit(tr(""));

    m_pBtnSelectVar = TBase::Button(tr("?"),tr(""));
    connect(m_pBtnSelectVar,&QPushButton::clicked,this,&AttriIOLamp::SelectVar);

    m_pCmbFrequency = TBase::ComboBox(tr(""));
    QStringList list;
    list << tr("200") << tr("500") << tr("1000");
    m_pCmbFrequency->addItems(list);

    m_pExpreAttri = AttriBase::expreSelectAttri();
    m_pExpreAttri->SetSelectVarType(tr("Num+Switch+String"));

    m_pCmbSelectPower->setEnabled(false);
    pLayout->addLayout(m_pGridLayout,0,0,1,5);
    pLayout->addWidget(new QLabel(tr("刷新频率")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pCmbFrequency,1,1,1,3);
    pLayout->addWidget(new QLabel(tr("ms")),1,4);
    pLayout->addWidget(new QLabel(tr("控制量")),2,0,Qt::AlignLeft);
    pLayout->addWidget(m_pLineEditVar,2,1,1,3);
    pLayout->addWidget(m_pBtnSelectVar,2,4);
    pLayout->addWidget(m_pExpreAttri,3,0,1,5);


    return pAttriWidget;
}

void AttriIOLamp::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TIOLamp *lmp = qobject_cast<TIOLamp*>(itm);

    m_pLineEditVar->setText(lmp->GetVarName());
    m_pCmbFrequency->setCurrentText(QString::number(lmp->GetAutoTime()));
    m_pExpreAttri->SetExpre(lmp->GetExpre());

    connect(m_pLineEditVar,&QLineEdit::editingFinished,this,&AttriIOLamp::VarChanged);
    connect(m_pCmbFrequency,&QComboBox::currentTextChanged,this,&AttriIOLamp::RefreshFrequencyChanged);
    connect(m_pExpreAttri,&ExpreSelectedAttri::SendTextFinfished,this,&AttriIOLamp::ExpreChanged);
    connect(this,&AttriIOLamp::SendTAttribute,lmp,&TIOLamp::SetAttributeEvent);
}

void AttriIOLamp::VarChanged()
{
    QStringList list = m_strVar.split(',');
    if(list.size() > 1)
    {
        list[0] = m_pLineEditVar->text();
    }

    QString str = QString("%1,%2").arg(list[0]).arg(list[1]);
    emit SendTAttribute(CoAttributeWidget::VarName,str,0);
}

void AttriIOLamp::SelectVar()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(tr("Switch"));
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name,const QString type)
    {
        m_strVar = tr("%1,%2").arg(name).arg(type);
        m_pLineEditVar->setText(name);
        m_pLineEditVar->editingFinished();
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

void AttriIOLamp::RefreshFrequencyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,text,0);
}

void AttriIOLamp::ExpreChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,text,0);
}



