#include "dataManagerpermiss.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlTableModel>
#include <QScrollArea>
#include <QHeaderView>
#include <QStringList>
#include <QMenu>
#include <QContextMenuEvent>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

databasepermiss::databasepermiss(QWidget *parent ): QWidget(parent)
{
    QString str = QCoreApplication::applicationDirPath();
    path = str+"/cofeidatabasev1.1.db";
    InitPermanagertable();
}

databasepermiss::~databasepermiss()
{ 
  DBInstanceManager::getDBInstance()->CloseDb();
}

void databasepermiss::InitPermanagertable()
{
    //QScrollArea *pAreaCut = new QScrollArea(this);//设置滚动区域
    QVBoxLayout *vuivlay = new QVBoxLayout();
    QHBoxLayout *infohlay = new QHBoxLayout;//第一排需要显示内容
    QLineEdit *tabelinfo = new QLineEdit(this);
    tabelinfo->setReadOnly(true);
    infohlay->addWidget(tabelinfo);
    vuivlay->addLayout(infohlay);
    //第二排需要显示权限设置界面直接从数据库读取内容
    InitDataBase();//初始化数据库内容
    QFont font;
    font.setPointSize(11);
    QFont font1;
    font1.setPointSize(13);
    font1.setFamily("黑体");
    //设置表头
    m_tabmodel->setHeaderData(0,Qt::Horizontal,"ID");
    m_tabmodel->setHeaderData(1,Qt::Horizontal,"用户名字");
    m_tabmodel->setHeaderData(2,Qt::Horizontal,"密码");
    m_tabmodel->setHeaderData(3,Qt::Horizontal,"权限级别");
    m_tabmodel->setHeaderData(4,Qt::Horizontal,"描述");
    m_tabview = new QTableView(this);

    m_tabview->setModel(m_tabmodel);//链接在前 设置属性在后
    m_tabview->setItemDelegateForColumn(3,new SpinBoxDelegate);
    m_tabview->verticalHeader()->setDefaultSectionSize(30);
    m_tabview->setGeometry(0,5,300,500);
    //设置各列的宽度
    m_tabview->setColumnWidth(0,100);
    m_tabview->setColumnWidth(1,100);
    m_tabview->setColumnWidth(2,300);
    m_tabview->setColumnWidth(3,150);
    m_tabview->setColumnWidth(4,150);
    m_tabview->setFont(font);
    m_tabview->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabview->verticalHeader()->hide();//影藏默认的那个列
    vuivlay->addWidget(m_tabview);
    QHBoxLayout *btnhlay = new QHBoxLayout;

    //    QPushButton *addbtn = new QPushButton(this);
    //    addbtn->setText("新增");
    //    btnhlay->addWidget(addbtn);
    QPushButton *clearbtn = new QPushButton(this);
    clearbtn->setText("清空");
    btnhlay->addWidget(clearbtn);
    clearbtn->setFixedWidth(96);
    btnhlay->setAlignment(Qt::AlignLeft);

    //    QPushButton *exportbtn = new QPushButton(this);
    //    exportbtn->setText("导出文件");
    //    btnhlay->addWidget(exportbtn);
    vuivlay->addLayout(btnhlay);
    this->setLayout(vuivlay);
    //m_tabview->setColumnHidden(0,tr);
}

bool databasepermiss::Excisexisttable(QString table , QSqlDatabase database)
{
    QSqlQuery query(database);
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));
    if(query.next())
    {
        if(query.value(0).toInt()==0)//判断id
        {
            return false; // 表不存在
        }
        else
        {
            return true; //表存在
        }
    }
     return false;
}

void databasepermiss::InitDataBase()
{
    if(!DBInstanceManager::getDBInstance()->OpenDb(path))//打开数据库
    {
        QMessageBox::about(this,"Warning","数据库文件不存在");
        return;
    }
    else
    {
    }
    if(!DBInstanceManager::getDBInstance()->m_pBDataBase->Excisexisttable("PermissionsManagement"))
    {
        QMessageBox::about(this,"Warning","确认表格是否存在");
    }
    m_tabmodel = new QSqlTableModel(this,DBInstanceManager::getDBInstance()->m_pBDataBase->GetDataBase());//重写data函数是字体居中
    m_tabmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置tabview可以手动改
    m_tabmodel->setTable("PermissionsManagement");
    m_tabmodel->select();
}

QMenu *databasepermiss::CreatQmenu()
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建"));
    connect(add,&QAction::triggered,this,&databasepermiss::AddData);
    QAction *update = new QAction(QObject::tr("更新"));
    connect(update,&QAction::triggered,this,&databasepermiss::UpdateData);
    QAction *del = new QAction(QObject::tr("删除"));
    connect(del,&QAction::triggered,this,&databasepermiss::DeleteData);
    QAction *save = new QAction(QObject::tr("保存"));
    connect(save,&QAction::triggered,this,&databasepermiss::Save);
    menu->addAction(add);
    menu->addAction(update);
    menu->addAction(del);
    menu->addAction(save);
    return menu;
}

void databasepermiss::AddData()
{
    int curow = m_tabview->currentIndex().row();
    QSqlRecord record = m_tabmodel->record();
    m_tabmodel->insertRecord(curow+1,record);
}

void databasepermiss::UpdateData()
{
    DBInstanceManager::getDBInstance()->OpenDb(path);
    if(m_tabmodel != NULL)
    {
        m_tabmodel->select();
    }
}

void databasepermiss::DeleteData()
{
    QModelIndexList indexlist = m_tabview->selectionModel()->selectedRows();
    switch(QMessageBox::question(this,tr("询问"),tr("删除?"),
                                 QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
       // m_tabmodel->removeRow(rowCount);
        for(int i = 0; i < indexlist.size(); ++i)
        {
             m_tabmodel->removeRow(indexlist[i].row());
        }
        m_tabmodel->submitAll();
        break;
    case QMessageBox::Cancel:
        m_tabmodel->revertAll();
        break;
    default:
        break;
    }
}
void databasepermiss::Save()//查询语句需要接受语句和记录
{
     DBInstanceManager::getDBInstance()->OpenDb(path);
    if(!m_tabmodel->submitAll())
    {
        QMessageBox::about(this,"Warning","请确认数据不可以为空ID不可重复，并设置合适的权限级别 0-4");
    }
}

void databasepermiss::contextMenuEvent(QContextMenuEvent *event)
{
    if(m_tabview->hasFocus())
    {
        QMenu *menu = CreatQmenu();
        menu->exec(QCursor::pos());
        event->accept();
    }
}

SpinBoxDelegate::SpinBoxDelegate(QObject *parent): QItemDelegate(parent)
{

}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *editor = new QSpinBox(parent);
    editor->setRange(0,4);
    editor->installEventFilter(const_cast<SpinBoxDelegate*>(this));
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{    int value = index.model()->data(index, Qt::EditRole).toInt();
     QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
      spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *box = static_cast<QSpinBox*>(editor);
    int value = box->value();
    model->setData(index,value);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect); // option.rect属性中保存了条目的位置，这里将控件设置在刚好占住条目的位置
}
