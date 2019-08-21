#include "cbasetabwidget.h"
#include <QHeaderView>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
CBaseTabWidget::CBaseTabWidget(QStringList headList,QWidget *parent)
{
    m_pe = parent;
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    CreateTabHeader(headList);
    QFont font;
    font.setPointSize(11);
    font.setFamily("黑体");
    this->setFont(font);
}

void CBaseTabWidget::CreateTabHeader(const QStringList headList)
{
    this->setFixedWidth(m_pe->width()-40);
    this->setColumnCount(headList.count());
    this->horizontalHeader()->setDefaultSectionSize(m_pe->width()/headList.count());
    this->resizeColumnToContents(headList.count());
    this->setHorizontalHeaderLabels(headList);
    horizontalHeader()->setStyleSheet("QHeaderView::section{border:1px ;border-radius:4px;padding:2px 4px;background-color: rgb(75, 75, 75);font: 13pt '黑体'}");
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setDefaultSectionSize(25);
    this->setFrameShape(QFrame::NoFrame);
    this->setShowGrid(true);
    this->verticalHeader()->setVisible(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);
    for(int i = 0 ;i < headList.size();++i)
    {
        this->horizontalHeader()->resizeSection(i,150);
    }
    this->horizontalHeader()->setFixedHeight(30);
    this->setStyleSheet("selection-background-color:rgb(14, 14, 14);border-radius:15px;");
}
void CBaseTabWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QPoint point = event->pos();
    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL)
    {
        QMenu *menu = CreateContextMenu();
        menu->exec(QCursor::pos());
        event->accept();
    }
    else
    {
        QMenu *menu = CreateContextMenu(true);
        menu->exec(QCursor::pos());
        event->accept();
    }
}
QMenu* CBaseTabWidget::CreateContextMenu(bool isempty)
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建"),this);
    menu->addAction(add);
    connect(add, &QAction::triggered, this, &CBaseTabWidget::AddPoint);
    if(!isempty)
    {
        QAction *del = new QAction(QObject::tr("删除"),this);
        connect(del, &QAction::triggered, this, &CBaseTabWidget::DeletePoint);
        menu->addAction(del);
    }
    QAction *updata = new QAction(QObject::tr("更新"),this);
    connect(updata, &QAction::triggered, this, &CBaseTabWidget::UpdataPoint);
    QAction *save = new QAction(QObject::tr("保存"),this);
    connect(save, &QAction::triggered, this, &CBaseTabWidget::SavePoints);

    menu->addAction(updata);
    menu->addAction(save);
    return menu;
}
bool CBaseTabWidget::CreateMessageBox(QString strTitle,QString note)
{
    QFont font;
    font.setPointSize(13);
    font.setFamily("黑体");
    QDialog box ;
    box.setWindowTitle(strTitle);
    box.setFixedSize(356,200);
    box.setFont(font);

    QLabel *lable = new QLabel(&box);
    lable->setText(note);
    lable->setFont(font);
    lable->setWordWrap(true);
    lable->setFixedSize(350,100);
    QVBoxLayout *vbox = new QVBoxLayout(&box);
    box.setLayout(vbox);

    vbox->addWidget(lable);

    box.setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    QHBoxLayout *hBox = new QHBoxLayout(&box);
    QPushButton *btn = new QPushButton("确认",this);
    connect(btn,&QPushButton::clicked,&box,&QDialog::accept);
    btn->setFont(font);
    QPushButton *btn1 = new QPushButton("取消",this);
    connect(btn1,&QPushButton::clicked,&box,&QDialog::reject);
    btn1->setFont(font);
    hBox->addWidget(btn);
    hBox->addWidget(btn1);
    vbox->addLayout(hBox);
    if(QDialog::Accepted == box.exec())
        return true;

    return false;
}

void CBaseTabWidget::AddPoint()
{
    int irow = this->rowCount();
    this->insertRow(irow);
}

void CBaseTabWidget::DeletePoint()
{
    int curow = this->currentRow();
    this->removeRow(curow);
}

void CBaseTabWidget::UpdataPoint()
{

}

void CBaseTabWidget::SavePoints()
{

}
