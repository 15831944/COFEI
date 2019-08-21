#include <QScrollArea>
#include <QGridLayout>
#include <QIntValidator>
#include "../TBasicControls/TableView.h"
#include "AttriTableView.h"

AttriTableView::AttriTableView(QWidget *parent)
    : GraphicAttrWidget(parent)
{
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

QWidget *AttriTableView::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pRowSumEdit = TBase::LineEdit(tr(""));
    QIntValidator *iv = new QIntValidator(0,99999);
    m_pRowSumEdit->setValidator(iv);

    pLayout->addLayout(m_pGridLayout,0,0,1,5);
    pLayout->addWidget(new QLabel(tr("行总数   ")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pRowSumEdit,1,1,1,3);

    return pAttriWidget;
}

void AttriTableView::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TTableView *tv = qobject_cast<TTableView*>(itm);
    m_pRowSumEdit->setText(QString::number(tv->GetRowSum()));
    connect(m_pRowSumEdit,&QLineEdit::editingFinished,this,&AttriTableView::RowSumChanged);

    connect(this,&AttriTableView::SendTAttribute,tv,&TTableView::SetAttributeEvent);
}


void AttriTableView::RowSumChanged()
{
    QString value = m_pRowSumEdit->text();
    emit SendTAttribute(CoAttributeWidget::RowSum,value,0);
}
