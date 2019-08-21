#include "csptablewidget.h"
#include <QAction>
#include<QHeaderView>
#include "csptritable.h"
#include <QMessageBox>
#include<QDebug>

CSPTableWidget::CSPTableWidget(QWidget *parent, QVector<QString> axisidstr, QVector<QMap<QString,double>> vec):QTableWidget(parent)
{
    // QString id;传入id
    idstr = axisidstr;
    CSPvec = vec;//这线段传入
    //m_updatetabledata = vec;//数据有可能为空
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    CSPIdCreateUI();
}

QMenu *CSPTableWidget::CreateContextMenu()
{
    menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    m_add = new QAction(QObject::tr("新建"),this);
    connect(m_add, &QAction::triggered, this, &CSPTableWidget::AddPoint);
    QAction *del = new QAction(QObject::tr("删除"),this);
    connect(del, &QAction::triggered, this, &CSPTableWidget::DeletePoint);
    QAction *update = new QAction(QObject::tr("更新"),this);
    connect(update, &QAction::triggered, this, &CSPTableWidget::UpdatePoint);
    m_setTr = new QAction(QObject::tr("触发设置"),this);
    connect(m_setTr, &QAction::triggered, this, &CSPTableWidget::SetTrigger);
    QAction *clearTr = new QAction(QObject::tr("清空触发"),this);
    connect(clearTr, &QAction::triggered, this, &CSPTableWidget::ClearTrigger);

    menu->addAction(m_add);
    menu->addAction(del);
    menu->addAction(update);
    menu->addAction(m_setTr);
    menu->addAction(clearTr);
    return menu;
}

void CSPTableWidget::NewCSPIdCreateUI()//针对不存在的ID
{
    //this->setFixedSize(par);
    this->setRowCount(1);
    idstr.insert(idstr.begin(),"目标p");
    this->setColumnCount(idstr.size());
    QStringList str;
    for(int i=0;i<idstr.size();++i)
    {
        str.append(idstr[i]);
    }
    this->setHorizontalHeaderLabels(str);//表头插入数值
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setItem(0,0,new QTableWidgetItem("P0"));
    for(int i=0;i<idstr.size()-1;++i)
    {
        this->setItem(0,i+1,new QTableWidgetItem("0"));
    }
}

void CSPTableWidget::CSPIdCreateUI()//对已有的ＩＤ
{
    this->setRowCount(CSPvec.size()+1);
    idstr.insert(idstr.begin(),"目标p");
    this->setColumnCount(idstr.size());
    QStringList str;
    for(int i=0;i<idstr.size();++i)
    {
        str.append(idstr[i]);
    }
    this->setHorizontalHeaderLabels(str);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setEditTriggers(QAbstractItemView::DoubleClicked);   //双击可以修改
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);  //设置为可以选中单个
    double pos;
    for(int j=0;j<CSPvec.size();++j)
    {
        this->setItem(j,0,new QTableWidgetItem());
        this->item(j,0)->setText(QString("P%1").arg(QString::number(j)));
        for(int i=1;i<idstr.size();++i)
        {
            pos=CSPvec[j][idstr[i]];
            this->setItem(j,i,new QTableWidgetItem());
            this->item(j,i)->setText(QString::number(pos,10,3));
        }
    }
    for(int i=0;i<idstr.size();++i)
    {
        this->setItem(CSPvec.size(),i,new QTableWidgetItem(""));
    }
    connect(this,&QTableWidget::cellDoubleClicked,this,&CSPTableWidget::disable);
}

void CSPTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();
    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL)
    {
        QMenu *menu = CreateContextMenu();
        menu->exec(QCursor::pos());
        event->accept();
    }
}

void CSPTableWidget::AddPoint()
{
    m_Dilog=new Addtabel(idstr);
    connect(m_Dilog,&Addtabel::data,this,&CSPTableWidget::Datastr);//数据更新
    if(m_Dilog->exec()== QDialog::Accepted)
    {
        for(int i=0;i<m_list.size();++i)
        {
            if(m_list[i]=="")
            {  QMessageBox::warning(this, QString("新建目标位置"), QString("请确保当前行数据存在!!!"));
                return;
            }
        }
        for(int j=0;j<this->rowCount()-1;++j)
        {
            if(m_list[0]==this->item(j,0)->text())
            {
                QMessageBox::warning(this, QString("新建目标位置"), QString("ID已经存在!!!"));
                return;
            }
        }
        int row = this->currentRow();
        int trar=-1;
        if(row==this->rowCount()-1)
        {
            trar = row;
        }
        else
        {
            trar=row+1;
        }
        this->insertRow(row+1);//表格数据更新
        for(int i(0);i<m_list.size();++i)
        {
            QTableWidgetItem *item = new QTableWidgetItem(m_list[i]);
            item->setFont(QFont("Helvetica"));
            this->setItem(trar,i,item);
        }
        for(int i(0);i<m_list.size();++i)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFont(QFont("Helvetica"));
            this->setItem(this->rowCount()-1,i,item);
        }
        QMap<QString,double> map;
        for(int i(1);i<m_list.size();++i)
        {
            map.insert(idstr[i],this->item(trar,i)->text().toDouble());
        }
        //传入行号和数据给cspinfoui
        //        if(trar==row)
        //        {

        //        }
        //        m_updatetabledata.clear();
        //        QVector<QString> trap;
        //        trap.clear();
        //        for(int i=0;i<this->rowCount()-1;++i)
        //        {
        //            QMap<QString,double>map;
        //            for(int j=1;j<this->columnCount();++j)
        //            {
        //                map.insert(idstr[j],this->item(i,j)->text().toDouble());
        //            }
        //            trap.append(this->item(i,0)->text());
        //            m_updatetabledata.append(map);
        //        }
        //        emit updatedata(m_updatetabledata,trap);
    }
}

void CSPTableWidget::DeletePoint()
{
    //    m_updatetabledata.clear();
    //    QVector<QString> trap;
    //    trap.clear();
    //    for(int i=0;i<this->rowCount()-1;++i)
    //    {
    //        QMap<QString,double>map;

    //        for(int j=1;j<this->columnCount();++j)
    //        {
    //            map.insert(idstr[j],this->item(i,j)->text().toDouble());

    //        }
    //        m_updatetabledata.append(map);
    //        trap.append(this->item(i,0)->text());
    //    }
    int curow = this->currentRow();
    if(this->rowCount()>1&&curow!=this->rowCount()-1)
    {
        this->removeRow(curow);
        //m_updatetabledata.removeAt(curow);
        // trap.removeAt(curow);
        //emit updatedata(m_updatetabledata,trap);
    }
}

void CSPTableWidget::SetTrigger()
{
    int curow = this->currentRow();
    if(curow==this->rowCount()-1)
        return;
    //所有的表格数据
    QVector<QMap<QString,double>> sum;
    sum.clear();
    for(int i=0;i<this->rowCount()-1;++i)
    {
        QMap<QString,double> map;
        for(int j=0;j<this->columnCount()-1;++j)
        {
            map.insert(idstr[j+1],this->item(i,j+1)->text().toDouble());
        }
        sum.append(map);
    }
    //触发信息结构体
    emit TriInfo(curow);
}

void CSPTableWidget::ClearTrigger()
{
    int curow = this->currentRow();
    if(curow!=this->rowCount()-1)
    {
        emit ClearTri(curow);
    }
}

void CSPTableWidget::Datastr(QStringList data)
{
    m_list = data;
}

void CSPTableWidget::UpdatePoint()
{
    m_updatetabledata.clear();
    QVector<QString> trap;
    trap.clear();
    for(int i = 0;i < this->rowCount()-1;++i)
    {
        QMap<QString,double>map;
        for(int j=1;j<this->columnCount();++j)
        {
           if(this->item(i,j)->text() == "")
           {
               return;//数据不可为空无法保存
           }
            map.insert(idstr[j],this->item(i,j)->text().toDouble());
            qDebug()<<"map"<<idstr[j]<<"  "<<this->item(i,j)->text().toDouble();
        }
        trap.append(this->item(i,0)->text());
        m_updatetabledata.append(map);
    }
    emit updatedata(m_updatetabledata,trap);
}

void CSPTableWidget::disable()
{
    int curow = this->currentRow();
    int col = this->currentColumn();
    if (curow == this->rowCount()-1)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem ();
        this->setItem(curow,col,newItem);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);//如果为最后一行则双击不可编辑
    }
}

