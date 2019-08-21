#include <QAbstractItemView>
#include <QModelIndex>
#include <QModelIndexList>
#include <QDebug>
#include "VariableInforWidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QCoreApplication>
#include <DatabaseLib/variable.h>
#include <QModelIndex>
#include <QStandardItemModel>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QMutex>

const QString enstr = "QPushButton{width:70px;font:14px;border:1px solid white;border-radius:2px;}"\
                      "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                      "QPushButton:pressed{background-color:rgba(255,255,255,150);}";
const QString denstr = "width:70px;font:14px;border:0px solid black;border-radius:3px;background-color:rgba(144,144,144,110);";
//#include "MulThreadCopyFile/multhreacopyfile.h"
//#include "MulThreadCopyFile/multhreacopyfile_global.h"

VariableInforWidget::VariableInforWidget(QWidget *parent) : QWidget(parent)
{
    m_ModVarcountMap.clear();
    m_ModVartimeMap.clear();
    m_ModVarregtextMap.clear();
    m_ModVarcyMap.clear();
    m_ModVarregnumMap.clear();
    m_ModVarqpMap.clear();
    m_ModvarcspMap.clear();
    m_DelpkeyList.clear();
    m_ModVarswMap.clear();
    QString protablename = "Productioninformation";
    DBInstanceManager::getDBInstance()->ReadProDataFromDB(protablename);//得到机种编号内容
    QStringList strHeaderList;
    strHeaderList<<tr("变量名称")<<tr("变量类型")
                <<tr("变量描述")<<tr("机种编号");
    m_pTableView = new TableView();
    m_pTableView->SetHeaderData(strHeaderList);
    m_pTableView->resize(parent->width(),parent->height());
    connect(m_pTableView,&TableView::signalSelectedRowChange,this,&VariableInforWidget::slotSelectedRowChanged);
    connect(m_pTableView,&TableView::signalDataChanged,this,&VariableInforWidget::slotTableViewDataChanged);
    connect(m_pTableView,&TableView::signalDeleteRows,this,&VariableInforWidget::slotDeleteRows);
    connect(m_pTableView,&TableView::signalTypeAttriChanged,this,&VariableInforWidget::slotAttriTypeChanged);
    connect(this,&VariableInforWidget::signalFindName,m_pTableView,&TableView::slotFindname);

    QPushButton *pAddBtn = new QPushButton();
    pAddBtn->setText(tr("新增"));
    connect(pAddBtn,&QPushButton::clicked,m_pTableView,&TableView::slotAddRow);
    connect(pAddBtn,&QPushButton::clicked,this,&VariableInforWidget::slotAddRow);

    QPushButton *pBatchAddBtn = new QPushButton();
    pBatchAddBtn->setText(tr("快速新建"));
    connect(pBatchAddBtn,&QPushButton::clicked,m_pTableView,&TableView::slotBatchAddRow);
    connect(pBatchAddBtn,&QPushButton::clicked,this,&VariableInforWidget::slotBatchAddRow);
    QPushButton *pDeleteBtn = new QPushButton();
    pDeleteBtn->setText(tr("删除"));
    connect(pDeleteBtn,&QPushButton::clicked,m_pTableView,&TableView::slotRemove);

    QPushButton *pRefrehBtn = new QPushButton();
    pRefrehBtn->setText(tr("刷新"));
    connect(pRefrehBtn,&QPushButton::clicked,this,&VariableInforWidget::slotRefreh);

    m_pCommitBtn = new QPushButton();
    m_pCommitBtn->setText(tr("提交"));
    connect(m_pCommitBtn,&QPushButton::clicked,this,&VariableInforWidget::slotCommit);
    m_pCommitBtn->setEnabled(false);
    //增加查询功能
    m_nameline = new QLineEdit;
    QPushButton *findbtn = new QPushButton();
    findbtn->setText("变量查询");
    connect(m_nameline,&QLineEdit::returnPressed,this,&VariableInforWidget::slotfindname);
    connect(findbtn,&QPushButton::clicked,this,&VariableInforWidget::slotfindname);

    //布局
    QHBoxLayout *pbtnLayout = new QHBoxLayout();
    pbtnLayout->addWidget(pAddBtn);
    pbtnLayout->addWidget(pBatchAddBtn);
    pbtnLayout->addWidget(pDeleteBtn);
    pbtnLayout->addWidget(pRefrehBtn);
    pbtnLayout->addWidget(m_pCommitBtn);
    pbtnLayout->addWidget(m_nameline);
    pbtnLayout->addWidget(findbtn);
    pbtnLayout->addStretch();

    QVBoxLayout *pBodyLayout = new QVBoxLayout();
    pBodyLayout->addLayout(pbtnLayout);
    pBodyLayout->addWidget(m_pTableView);

    QGroupBox *pMainGroup = new QGroupBox();
    pMainGroup->setLayout(pBodyLayout);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainGroup);

    this->setLayout(pMainLayout);
    this->setStyleSheet("QPushButton{width:70px;font:14px;border:1px solid white;border-radius:3px;}"
                        "QPushButton:hover{background-color:rgba(255,255,255,110);}"
                        "QPushButton:pressed{background-color:rgba(255,255,255,150);}"
                        "QHeaderView{height:40px;font:14px;border:none;}"
                        "QTableView{font:14px;selection-background-color:rgba(255,255,255,40);}");
    m_pCommitBtn->setStyleSheet(tr("width:70px;font:14px;border:0px solid black;border-radius:3px;background-color:rgba(144,144,144,110);"));
    //打开数据库,析构的时候,关闭数据库
    GetDataFromDB();
}

VariableInforWidget::~VariableInforWidget()
{
    //文件备份
    //    MulThreaCopyFile a;
    //    QString sfile = "/root/SVN/dbtest/cofeidatabasev1.1.db";
    //    QString dfile = "/root/cop.db";
    //    a.ExcCopy(sfile,dfile);
}

void VariableInforWidget::slotVarAttriDataChanged(QString type, BASEDB_STRU *base)// 属性界面变化引起的修改记录内容
{
    if(base == NULL)return;//这是属性界面text变化引起的
    if(!m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(true);
        m_pCommitBtn->setStyleSheet(enstr);
    }
    QString pkey = QString::fromUtf8(base->pkey);
    if(type == "计数类")
    {
        VARDB_COUNT_STRU *stru = static_cast<VARDB_COUNT_STRU*>(base);
        if(m_DBVarstru.countMap.contains(pkey))//属性框传过来的信息内容
        {
            strncpy(stru->note,m_DBVarstru.countMap[pkey].note,128);
            strncpy(stru->fkey,m_DBVarstru.countMap[pkey].fkey,32);
            m_DBVarstru.countMap.remove(pkey);// 原有的数据进行修改
            m_DBVarstru.countMap.insert(pkey, *stru);
            if(m_ModVarcountMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarcountMap.remove(pkey);
                m_ModVarcountMap.insert(pkey,*stru);
            }
            else {
                m_ModVarcountMap.insert(pkey,*stru);
            }
        }
    }
    else if(type == "计时类")
    {
        VARDB_TIME_STRU *stru = static_cast<VARDB_TIME_STRU*>(base);
        if(m_DBVarstru.timeMap.contains(pkey))//属性框传过来的信息内容
        {
            strncpy(stru->note,m_DBVarstru.timeMap[pkey].note,128);
            strncpy(stru->fkey,m_DBVarstru.timeMap[pkey].fkey,32);
            m_DBVarstru.timeMap.remove(pkey);
            m_DBVarstru.timeMap.insert(pkey,*stru);// 原有的数据进行修改
            if(m_ModVartimeMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVartimeMap.remove(pkey);
                m_ModVartimeMap.insert(pkey,*stru);
            }
            else{
                m_ModVartimeMap.insert(pkey,*stru);
            }
        }
    }
    else if(type == "气缸类")
    {
        VARDB_CY_STRU *stru = static_cast<VARDB_CY_STRU*>(base);
        if(m_DBVarstru.cyMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.cyMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.cyMap[pkey].fkey);
            m_DBVarstru.cyMap.remove(pkey);
            m_DBVarstru.cyMap.insert(pkey,*stru);
            if(m_ModVarcyMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarcyMap.remove(pkey);
                m_ModVarcyMap.insert(pkey,*stru);
            }
            else {
                m_ModVarcyMap.insert(pkey,*stru);
            }
        }
    }
    else if(type == "字符串型寄存器")
    {
        VARDB_REGTEXT_STRU *stru = static_cast<VARDB_REGTEXT_STRU*>(base);
        if(m_DBVarstru.regtextMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.regtextMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.regtextMap[pkey].fkey);
            m_DBVarstru.regtextMap.remove(pkey);
            m_DBVarstru.regtextMap.insert(pkey,*stru);
            if(m_ModVarregtextMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarregtextMap.remove(pkey);
                m_ModVarregtextMap.insert(pkey,*stru);
            }
            else {
                m_ModVarregtextMap.insert(pkey,m_DBVarstru.regtextMap[pkey]);
            }
        }
    }
    else if(type == "数值型寄存器")
    {
        VARDB_REGNUM_STRU *stru = static_cast<VARDB_REGNUM_STRU*>(base);
        if(m_DBVarstru.regnumMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.regnumMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.regnumMap[pkey].fkey);
            m_DBVarstru.regnumMap.remove(pkey);
            m_DBVarstru.regnumMap.insert(pkey,*stru);
            if(m_ModVarregnumMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarregnumMap.remove(pkey);
                m_ModVarregnumMap.insert(pkey,*stru);
            }
            else {
                m_ModVarregnumMap.insert(pkey,m_DBVarstru.regnumMap[pkey]);
            }
        }
    }
    else if(type == "运动控制CSP模式")
    {
        VARDB_LCSP_STRU *stru = static_cast<VARDB_LCSP_STRU*>(base);
        if(m_DBVarstru.cspMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.cspMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.cspMap[pkey].fkey);
            m_DBVarstru.cspMap.remove(pkey);
            m_DBVarstru.cspMap.insert(pkey,*stru);
            if(m_ModvarcspMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModvarcspMap.remove(pkey);
                m_ModvarcspMap.insert(pkey,*stru);
            }
            else {
                m_ModvarcspMap.insert(pkey,m_DBVarstru.cspMap[pkey]);
            }
        }
    }
    else if(type == "运动控制PP模式")
    {
        VARDB_PPMOTCTR_STRU *stru = static_cast<VARDB_PPMOTCTR_STRU*>(base);
        if(m_DBVarstru.qpMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.qpMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.qpMap[pkey].fkey);
            m_DBVarstru.qpMap.remove(pkey);
            m_DBVarstru.qpMap.insert(pkey,*stru);
            if(m_ModVarqpMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarqpMap.remove(pkey);
                m_ModVarqpMap.insert(pkey,*stru);
            }
            else {
                m_ModVarqpMap.insert(pkey,m_DBVarstru.qpMap[pkey]);
            }
        }
    }
    else if(type == "开关类")
    {
        VARDB_SWITCH_STRU *stru = static_cast<VARDB_SWITCH_STRU*>(base);
        if(m_DBVarstru.SwitchMap.contains(pkey))//属性框传过来的信息内容
        {
            strcpy(stru->note,m_DBVarstru.SwitchMap[pkey].note);
            strcpy(stru->fkey,m_DBVarstru.SwitchMap[pkey].fkey);
            m_DBVarstru.SwitchMap.remove(pkey);
            m_DBVarstru.SwitchMap.insert(pkey,*stru);
            if(m_ModVarswMap.contains(pkey))//原来的修改记录包含此内容就替换
            {
                m_ModVarswMap.remove(pkey);
                m_ModVarswMap.insert(pkey,*stru);
            }
            else {
                m_ModVarswMap.insert(pkey,m_DBVarstru.SwitchMap[pkey]);
            }
        }
    }
}

void VariableInforWidget::slotCommit()
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    QString str = QCoreApplication::applicationDirPath();
    DBInstanceManager::getDBInstance()->OpenDb(str+"/cofeidatabasev1.1.db");
    int row = m_pTableView->GetTablemodel()->rowCount();
    if(row > 0)
    {
        QModelIndex fkey = m_pTableView->GetTablemodel()->index(row -1,3);
        QModelIndex type =  m_pTableView->GetTablemodel()->index(row -1,1);
        QModelIndex name =  m_pTableView->GetTablemodel()->index(row -1,0);
        if(fkey.data().toString() == ""|| type.data().toString() == "" || name.data().toString() == "")
        {
            QMessageBox::warning(this, tr("Warning"), tr("机种编号或变量名字或类型不可为空"));
            return;
        }
        for(auto it = m_ModVarqpMap.begin();it!=m_ModVarqpMap.end();++it)
        {
            if(QString::fromUtf8(it.value().axisidinfo) == "")
            {
                QMessageBox::warning(this, tr("Warning"), tr("检查pp模式中是否选择轴的ID"));
                return;
            }
        }
    }
    m_pTableView->SetNoEditRow(row-1);
    if(m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(false);
        m_pCommitBtn->setStyleSheet(denstr);
    }
    QString tablename = "Variablemanagement";
    if(!m_ModVarcountMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarcountMap);
        m_ModVarcountMap.clear();
    }
    if(!m_ModVarregtextMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarregtextMap);
        m_ModVarregtextMap.clear();
    }
    if(!m_ModvarcspMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModvarcspMap );
        m_ModvarcspMap.clear();
    }
    if(!m_ModVarcyMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarcyMap);
        m_ModVarcyMap.clear();
    }
    if(!m_ModVarqpMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarqpMap);
        m_ModVarqpMap.clear();
    }
    if(!m_ModVarregnumMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarregnumMap);
        m_ModVarregnumMap.clear();
    }
    if(!m_ModVartimeMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVartimeMap);
        m_ModVartimeMap.clear();
    }
    if(!m_ModVarswMap.isEmpty())
    {
        DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,m_ModVarswMap);
        m_ModVarswMap.clear();
    }
    if(!m_DelpkeyList.isEmpty())
    {
        DBInstanceManager::getDBInstance()->DelDBDataByPkey(tr("Variablemanagement"),tr("Name"),m_DelpkeyList);
        m_DelpkeyList.clear();
    }
}
void VariableInforWidget::slotAddRow()
{
    if(!m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(true);
        m_pCommitBtn->setStyleSheet(enstr);
    }
}

void VariableInforWidget::slotBatchAddRow()
{

}

void VariableInforWidget::slotDeleteRows(QMap<QString, QList<QString>> typepkeymap)
{
    if(!m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(true);
        m_pCommitBtn->setStyleSheet(enstr);
    }
    for(auto it = typepkeymap.begin(); it!= typepkeymap.end();++it)
    {
        if(it.key() == "计数类")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.countMap.contains(it.value()[i]))
                {
                    m_DBVarstru.countMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarcountMap.contains(it.value()[i]))
                    {
                        m_ModVarcountMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "字符串型寄存器")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.regtextMap.contains(it.value()[i]))
                {
                    m_DBVarstru.regtextMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarregtextMap.contains(it.value()[i]))
                    {
                        m_ModVarregtextMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "计时类")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.timeMap.contains(it.value()[i]))
                {
                    m_DBVarstru.timeMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVartimeMap.contains(it.value()[i]))
                    {
                        m_ModVartimeMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "气缸类")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.cyMap.contains(it.value()[i]))
                {
                    m_DBVarstru.cyMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarcyMap.contains(it.value()[i]))
                    {
                        m_ModVarcyMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "运动控制CSP模式")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.cspMap.contains(it.value()[i]))
                {
                    m_DBVarstru.cspMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModvarcspMap.contains(it.value()[i]))
                    {
                        m_ModvarcspMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "运动控制PP模式")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.qpMap.contains(it.value()[i]))
                {
                    m_DBVarstru.qpMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarqpMap.contains(it.value()[i]))
                    {
                        m_ModVarqpMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "数值型寄存器")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.regnumMap.contains(it.value()[i]))
                {
                    m_DBVarstru.regnumMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarregnumMap.contains(it.value()[i]))
                    {
                        m_ModVarregnumMap.remove(it.value()[i]);
                    }
                }
            }
        }
        else if(it.key() == "开关类")
        {
            for(int i = 0; i< it.value().size();++i)
            {
                if(m_DBVarstru.SwitchMap.contains(it.value()[i]))
                {
                    m_DBVarstru.SwitchMap.remove(it.value()[i]);
                    if(!m_DelpkeyList.contains(it.value()[i]))
                    {
                        m_DelpkeyList.append(it.value()[i]);
                    }
                    if(m_ModVarswMap.contains(it.value()[i]))
                    {
                        m_ModVarswMap.remove(it.value()[i]);
                    }
                }
            }
        }
    }
}
void VariableInforWidget::slotRefreh()// 直接再内存刷新数据
{
    //刷新表格,读数据库
    if(m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(false);
        m_pCommitBtn->setStyleSheet(denstr);
    }
    QList<QStringList> tableList;
    tableList.clear();
    GetTablelist(tableList);
    m_pTableView->setData(tableList);
}

void VariableInforWidget::slotSelectedRowChanged(QString type,QString name)
{
    //结构体内容也可转成父类在实例化子类
    if(type == "计数类")
    {
        if(m_DBVarstru.countMap.contains(name))
        {
            VARDB_COUNT_STRU stru = m_DBVarstru.countMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "字符串型寄存器")
    {
        if(m_DBVarstru.regtextMap.contains(name))
        {
            VARDB_REGTEXT_STRU stru = m_DBVarstru.regtextMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "气缸类")
    {
        if(m_DBVarstru.cyMap.contains(name))
        {
            VARDB_CY_STRU stru = m_DBVarstru.cyMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "运动控制CSP模式")
    {
        if(m_DBVarstru.cspMap.contains(name))
        {
            VARDB_LCSP_STRU stru = m_DBVarstru.cspMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "运动控制PP模式")
    {
        if(m_DBVarstru.qpMap.contains(name))
        {
            VARDB_PPMOTCTR_STRU stru = m_DBVarstru.qpMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "数值型寄存器")
    {
        if(m_DBVarstru.regnumMap.contains(name))
        {
            VARDB_REGNUM_STRU stru = m_DBVarstru.regnumMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "计时类")
    {
        if(m_DBVarstru.timeMap.contains(name))
        {
            VARDB_TIME_STRU stru = m_DBVarstru.timeMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
    else if(type == "开关类")
    {
        if(m_DBVarstru.SwitchMap.contains(name))
        {
            VARDB_SWITCH_STRU stru = m_DBVarstru.SwitchMap[name];
            emit signalShowAttriWidget(type,&stru);
        }
    }
}

void VariableInforWidget::slotAttriTypeChanged(QStringList listvalue, QString strPreText)
{
    ChangeAttriOperateData(listvalue,strPreText);
    slotSelectedRowChanged(listvalue[1],listvalue[0]);
}
void VariableInforWidget::slotTableViewDataChanged(const QModelIndex &index,QStringList listvalue,QString strPreText)
{
    //表格数据改变
    if(listvalue.size() < 4) return;
    // if(listvalue[0] == "")  return;
    if(listvalue[1] == "") return;//属性没有代表没有任何意义的修改
    if(!m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(true);
        m_pCommitBtn->setStyleSheet(enstr);
    }
    int colmu = index.column();
    //设置改名字的时候先判断是否是提交之前还是提交之后
    switch (colmu) {
    case 0:
        if(listvalue[1] == "计数类")
        {
            if(m_DBVarstru.countMap.contains(strPreText))//改掉key值
            {
                VARDB_COUNT_STRU  ctru;
                ctru = m_DBVarstru.countMap[strPreText];
                m_DBVarstru.countMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(ctru.pkey,name.toStdString().c_str());
                m_DBVarstru.countMap.insert(listvalue[0],ctru);
                if(m_ModVarcountMap.contains(strPreText))
                {
                    m_ModVarcountMap.remove(strPreText);
                }
                if(m_ModVarcountMap.contains(listvalue[0]))
                {
                    m_ModVarcountMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarcountMap.insert(listvalue[0],ctru);
                }
                else {
                    m_ModVarcountMap.insert(listvalue[0],ctru);
                }
            }
        }
        else if(listvalue[1] == "计时类")
        {
            if(m_DBVarstru.timeMap.contains(strPreText))//改掉key值
            {
                VARDB_TIME_STRU  ctru;
                ctru = m_DBVarstru.timeMap[strPreText];
                m_DBVarstru.timeMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(ctru.pkey,name.toStdString().c_str());
                m_DBVarstru.timeMap.insert(listvalue[0],ctru);
                if(m_ModVartimeMap.contains(strPreText))
                {
                    m_ModVartimeMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVartimeMap.contains(listvalue[0]))
                {
                    m_ModVartimeMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVartimeMap.insert(listvalue[0],ctru);
                }
                else {
                    m_ModVartimeMap.insert(listvalue[0],ctru);
                }
            }
        }
        else if(listvalue[1] == "字符串型寄存器")
        {
            if(m_DBVarstru.regtextMap.contains(strPreText))
            {
                VARDB_REGTEXT_STRU restru;
                restru = m_DBVarstru.regtextMap[strPreText];
                m_DBVarstru.regtextMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(restru.pkey,name.toStdString().c_str());
                m_DBVarstru.regtextMap.insert(listvalue[0],restru);
                if(m_ModVarregtextMap.contains(strPreText))
                {
                    m_ModVarregtextMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVarregtextMap.contains(listvalue[0]))
                {
                    m_ModVarregtextMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregtextMap.insert(listvalue[0],restru);
                }
                else {
                    m_ModVarregtextMap.insert(listvalue[0],restru);
                }
            }
        }
        else if(listvalue[1] == "气缸类")
        {
            if(m_DBVarstru.cyMap.contains(strPreText))
            {
                VARDB_CY_STRU cystru;
                cystru = m_DBVarstru.cyMap[strPreText];
                m_DBVarstru.cyMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(cystru.pkey,name.toStdString().c_str());
                m_DBVarstru.cyMap.insert(listvalue[0],cystru);
                if(m_ModVarcyMap.contains(strPreText))
                {
                    m_ModVarcyMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVarcyMap.contains(listvalue[0]))
                {
                    m_ModVarcyMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarcyMap.insert(listvalue[0],cystru);
                }
                else {
                    m_ModVarcyMap.insert(listvalue[0],cystru);
                }
            }
        }
        else if(listvalue[1] == "数值型寄存器")
        {
            if(m_DBVarstru.regnumMap.contains(strPreText))
            {
                VARDB_REGNUM_STRU regnstru;
                regnstru = m_DBVarstru.regnumMap[strPreText];
                m_DBVarstru.regnumMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(regnstru.pkey,name.toStdString().c_str());
                m_DBVarstru.regnumMap.insert(listvalue[0],regnstru);
                if(m_ModVarregnumMap.contains(strPreText))
                {
                    m_ModVarregnumMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVarregnumMap.contains(listvalue[0]))
                {
                    m_ModVarregnumMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregnumMap.insert(listvalue[0],regnstru);
                }
                else {
                    m_ModVarregnumMap.insert(listvalue[0],regnstru);
                }
            }
        }
        else if(listvalue[1] == "运动控制CSP模式")
        {
            if(m_DBVarstru.cspMap.contains(strPreText))
            {
                VARDB_LCSP_STRU cspstru;
                cspstru = m_DBVarstru.cspMap[strPreText];
                m_DBVarstru.cspMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(cspstru.pkey,name.toStdString().c_str());
                m_DBVarstru.cspMap.insert(listvalue[0],cspstru);
                if(m_ModvarcspMap.contains(strPreText))
                {
                    m_ModvarcspMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModvarcspMap.contains(listvalue[0]))
                {
                    m_ModvarcspMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModvarcspMap.insert(listvalue[0],cspstru);
                }
                else {
                    m_ModvarcspMap.insert(listvalue[0],cspstru);
                }
            }
        }
        else if(listvalue[1] == "运动控制PP模式")
        {
            if(m_DBVarstru.qpMap.contains(strPreText))
            {
                VARDB_PPMOTCTR_STRU qpstru;
                qpstru = m_DBVarstru.qpMap[strPreText];
                m_DBVarstru.qpMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(qpstru.pkey,name.toStdString().c_str());
                m_DBVarstru.qpMap.remove(strPreText);
                m_DBVarstru.qpMap.insert(listvalue[0],qpstru);

                if(m_ModVarqpMap.contains(strPreText))
                {
                    m_ModVarqpMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVarqpMap.contains(listvalue[0]))
                {
                    m_ModVarqpMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarqpMap.insert(listvalue[0],qpstru);
                }
                else {
                    m_ModVarqpMap.insert(listvalue[0],qpstru);
                }
            }
        }
        else if(listvalue[1] == "开关类")
        {
            if(m_DBVarstru.SwitchMap.contains(strPreText))
            {
                VARDB_SWITCH_STRU vstru;
                vstru = m_DBVarstru.SwitchMap[strPreText];
                m_DBVarstru.SwitchMap.remove(strPreText);
                QString name = listvalue[0];
                strcpy(vstru.pkey,name.toStdString().c_str());
                m_DBVarstru.SwitchMap.insert(listvalue[0],vstru);
                if(m_ModVarswMap.contains(strPreText))
                {
                    m_ModVarswMap.remove(strPreText);//移除旧的插入新的
                }
                if(m_ModVarswMap.contains(listvalue[0]))
                {
                    m_ModVarswMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarswMap.insert(listvalue[0],vstru);
                }
                else {
                    m_ModVarswMap.insert(listvalue[0],vstru);
                }
            }
        }
        break;
    case 1://涉及到属性修改问题
        ChangeAttriOperateData(listvalue,strPreText);
        break;
    case 2://若修改的注释不用判断
        if(listvalue[1] == "计数类")
        {
            if(m_DBVarstru.countMap.contains(listvalue[0]))
            {
                strcpy( m_DBVarstru.countMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
            }
            if(m_ModVarcountMap.contains(listvalue[0]))
            {
                m_ModVarcountMap.remove(listvalue[0]);//移除旧的插入新的
                m_ModVarcountMap.insert(listvalue[0],m_DBVarstru.countMap[listvalue[0]]);
            }
            else {
                m_ModVarcountMap.insert(listvalue[0],m_DBVarstru.countMap[listvalue[0]]);
            }
        }
        else if(listvalue[1] == "计时类")
        {
            if(m_DBVarstru.timeMap.contains(listvalue[0]))//改掉key值
            {
                strcpy(m_DBVarstru.timeMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVartimeMap.contains(listvalue[0]))
                {
                    m_ModVartimeMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVartimeMap.insert(listvalue[0],m_DBVarstru.timeMap[listvalue[0]]);
                }
                else {
                    m_ModVartimeMap.insert(listvalue[0],m_DBVarstru.timeMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "字符串型寄存器")
        {
            if(m_DBVarstru.regtextMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.regtextMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVarregtextMap.contains(listvalue[0]))
                {
                    m_ModVarregtextMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregtextMap.insert(listvalue[0],m_DBVarstru.regtextMap[listvalue[0]]);
                }
                else {
                    m_ModVarregtextMap.insert(listvalue[0],m_DBVarstru.regtextMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "气缸类")
        {
            if(m_DBVarstru.cyMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.cyMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVarcyMap.contains(listvalue[0]))
                {
                    m_ModVarcyMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarcyMap.insert(listvalue[0],m_DBVarstru.cyMap[listvalue[0]]);
                }
                else {
                    m_ModVarcyMap.insert(listvalue[0],m_DBVarstru.cyMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "数值型寄存器")
        {
            if(m_DBVarstru.regnumMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.regnumMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVarregnumMap.contains(listvalue[0]))
                {
                    m_ModVarregnumMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregnumMap.insert(listvalue[0],m_DBVarstru.regnumMap[listvalue[0]]);
                }
                else {
                    m_ModVarregnumMap.insert(listvalue[0],m_DBVarstru.regnumMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "运动控制CSP模式")
        {
            if(m_DBVarstru.cspMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.cspMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModvarcspMap.contains(listvalue[0]))
                {
                    m_ModvarcspMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModvarcspMap.insert(listvalue[0],m_DBVarstru.cspMap[listvalue[0]]);
                }
                else {
                    m_ModvarcspMap.insert(listvalue[0],m_DBVarstru.cspMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "运动控制PP模式")
        {
            if(m_DBVarstru.qpMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.qpMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVarqpMap.contains(listvalue[0]))
                {
                    m_ModVarqpMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarqpMap.insert(listvalue[0],m_DBVarstru.qpMap[listvalue[0]]);
                }
                else {
                    m_ModVarqpMap.insert(listvalue[0],m_DBVarstru.qpMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "开关类")
        {
            if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.SwitchMap[listvalue[0]].note,listvalue[2].toStdString().c_str());
                if(m_ModVarswMap.contains(listvalue[0]))
                {
                    m_ModVarswMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarswMap.insert(listvalue[0],m_DBVarstru.SwitchMap[listvalue[0]]);
                }
                else {
                    m_ModVarswMap.insert(listvalue[0],m_DBVarstru.SwitchMap[listvalue[0]]);
                }
            }
        }
        break;
    case 3:
        if(listvalue[1] == "计数类")
        {
            if(m_DBVarstru.countMap.contains(listvalue[0]))
            {
                strcpy( m_DBVarstru.countMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
            }
            if(m_ModVarcountMap.contains(listvalue[0]))
            {
                m_ModVarcountMap.remove(listvalue[0]);//移除旧的插入新的
                m_ModVarcountMap.insert(listvalue[0],m_DBVarstru.countMap[listvalue[0]]);
            }
            else {
                m_ModVarcountMap.insert(listvalue[0],m_DBVarstru.countMap[listvalue[0]]);
            }
        }
        else if(listvalue[1] == "计时类")
        {
            if(m_DBVarstru.timeMap.contains(listvalue[0]))//改掉key值
            {
                strcpy(m_DBVarstru.timeMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVartimeMap.contains(listvalue[0]))
                {
                    m_ModVartimeMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVartimeMap.insert(listvalue[0],m_DBVarstru.timeMap[listvalue[0]]);
                }
                else {
                    m_ModVartimeMap.insert(listvalue[0],m_DBVarstru.timeMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "字符串型寄存器")
        {
            if(m_DBVarstru.regtextMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.regtextMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVarregtextMap.contains(listvalue[0]))
                {
                    m_ModVarregtextMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregtextMap.insert(listvalue[0],m_DBVarstru.regtextMap[listvalue[0]]);
                }
                else {
                    m_ModVarregtextMap.insert(listvalue[0],m_DBVarstru.regtextMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "气缸类")
        {
            if(m_DBVarstru.cyMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.cyMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVarcyMap.contains(listvalue[0]))
                {
                    m_ModVarcyMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarcyMap.insert(listvalue[0],m_DBVarstru.cyMap[listvalue[0]]);
                }
                else {
                    m_ModVarcyMap.insert(listvalue[0],m_DBVarstru.cyMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "数值型寄存器")
        {
            if(m_DBVarstru.regnumMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.regnumMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVarregnumMap.contains(listvalue[0]))
                {
                    m_ModVarregnumMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarregnumMap.insert(listvalue[0],m_DBVarstru.regnumMap[listvalue[0]]);
                }
                else {
                    m_ModVarregnumMap.insert(listvalue[0],m_DBVarstru.regnumMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "运动控制CSP模式")
        {
            if(m_DBVarstru.cspMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.cspMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModvarcspMap.contains(listvalue[0]))
                {
                    m_ModvarcspMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModvarcspMap.insert(listvalue[0],m_DBVarstru.cspMap[listvalue[0]]);
                }
                else {
                    m_ModvarcspMap.insert(listvalue[0],m_DBVarstru.cspMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "运动控制PP模式")
        {
            if(m_DBVarstru.qpMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.qpMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVarqpMap.contains(listvalue[0]))
                {
                    m_ModVarqpMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarqpMap.insert(listvalue[0],m_DBVarstru.qpMap[listvalue[0]]);
                    //strcpy(m_ModVarqpMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                }
                else {
                    m_ModVarqpMap.insert(listvalue[0],m_DBVarstru.qpMap[listvalue[0]]);
                }
            }
        }
        else if(listvalue[1] == "开关类")
        {
            if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
            {
                strcpy(m_DBVarstru.SwitchMap[listvalue[0]].fkey,listvalue[3].toStdString().c_str());
                if(m_ModVarswMap.contains(listvalue[0]))
                {
                    m_ModVarswMap.remove(listvalue[0]);//移除旧的插入新的
                    m_ModVarswMap.insert(listvalue[0],m_DBVarstru.SwitchMap[listvalue[0]]);
                }
                else {
                    m_ModVarswMap.insert(listvalue[0],m_DBVarstru.SwitchMap[listvalue[0]]);
                }
            }
        }
        break;
    default:
        break;
    }
}

void VariableInforWidget::slotfindname()//设置查询内存信息
{
    QString name = m_nameline->text();
    emit signalFindName(name);
}

void VariableInforWidget::GetDataFromDB()
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    QString protablename = "Productioninformation";
    DBInstanceManager::getDBInstance()->ReadVarDataFromDB();//读取变量表内容
    m_DBVarstru = DBInstanceManager::getDBInstance()->DBVarStru;
    DBInstanceManager::getDBInstance()->ReadProDataFromDB(protablename);//得到机种编号内容
    //从数据库表中提取部分需要显示到表格中的数据到表格list中
    QList<QStringList> tableList;
    tableList.clear();
    GetTablelist(tableList);
    m_pTableView->setData(tableList);
}

void VariableInforWidget::GetTablelist(QList<QStringList> &tableList)
{
    tableList.clear();
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap,tableList);
    QMap<QString,VARDB_SWITCH_STRU> SwitchMap;
    SwitchMap.clear();
    for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.begin(); \
        it !=DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.end();++it)
    {
       //if(!it.value().isBuilt_in)
        {
            SwitchMap.insert(it.key(),it.value());
           it.value().save = false;
            it.value().initvalue = 0;
            it.value().ishwIO =  false;
            it.value().initvalue = 0;


        }
    }
   // QString tablename = "Variablemanagement";
      //  DBInstanceManager::getDBInstance()->WriteDataToDB(tablename,DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap);


    GetVarDataFromRAM(SwitchMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.timeMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.countMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.cyMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.qpMap,tableList);
    GetVarDataFromRAM(DBInstanceManager::getDBInstance()->DBVarStru.cspMap,tableList);
}
///
/// \brief VariableInforWidget::ChangeAttriOperateData
/// \param listvalue 表示表格所有的数据
/// \param strPreText代表原来的属性
///
void VariableInforWidget::ChangeAttriOperateData(QStringList listvalue, QString strPreText)
{
    if(listvalue.size() < 4 )return;// 属性改变触发
    if(listvalue[1] == "") return;
    if(!m_pCommitBtn->isEnabled())
    {
        m_pCommitBtn->setEnabled(true);
        m_pCommitBtn->setStyleSheet(enstr);
    }
    if(listvalue[1] == "计数类")
    {
        VARDB_COUNT_STRU stru;
        strncpy(stru.type,listvalue[1].toStdString().c_str(),32);
        strncpy(stru.pkey,listvalue[0].toStdString().c_str(),32);
        strncpy(stru.note,listvalue[2].toStdString().c_str(),128);
        strncpy(stru.fkey,listvalue[3].toStdString().c_str(),32);
        if(!m_DBVarstru.countMap.contains(listvalue[0]))
        {
            m_DBVarstru.countMap.insert(listvalue[0],stru);
            if(m_ModVarcountMap.contains(listvalue[0]))
            {
                m_ModVarcountMap[listvalue[0]] = stru;//包含就替换
            }
            else
            {
                m_ModVarcountMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "字符串型寄存器")
    {
        //  VARDB_REGTEXT_STRU *Cstru = static_cast<VARDB_REGTEXT_STRU *>(&stru);// 执行替换
        VARDB_REGTEXT_STRU stru;
        strcpy(stru.type,listvalue[1].toStdString().c_str());
        strcpy(stru.pkey,listvalue[0].toStdString().c_str());
        strcpy(stru.note,listvalue[2].toStdString().c_str());
        strcpy(stru.fkey,listvalue[3].toStdString().c_str());
        if(!m_DBVarstru.regtextMap.contains(listvalue[0]))
        {
            m_DBVarstru.regtextMap.insert(listvalue[0],stru);
            if(m_ModVarregtextMap.contains(listvalue[0]))
            {
                m_ModVarregtextMap[listvalue[0]] =stru;//包含就替换
            }
            else
            {
                m_ModVarregtextMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "计时类")
    {
        VARDB_TIME_STRU stru;
        strncpy(stru.type,listvalue[1].toStdString().c_str(),32);
        strncpy(stru.pkey,listvalue[0].toStdString().c_str(),32);
        strncpy(stru.note,listvalue[2].toStdString().c_str(),128);
        strncpy(stru.fkey,listvalue[3].toStdString().c_str(),32);
        if(!m_DBVarstru.timeMap.contains(listvalue[0]))
        {
            m_DBVarstru.timeMap.insert(listvalue[0],stru);
            if(m_ModVartimeMap.contains(listvalue[0]))
            {
                m_ModVartimeMap.remove(listvalue[0]);
            }
            else{
                m_ModVartimeMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "气缸类")
    {
        VARDB_CY_STRU stru;
        strcpy(stru.type,listvalue[1].toStdString().c_str());
        strcpy(stru.pkey,listvalue[0].toStdString().c_str());
        strcpy(stru.note,listvalue[2].toStdString().c_str());
        strcpy(stru.fkey,listvalue[3].toStdString().c_str());
        if(!m_DBVarstru.cyMap.contains(listvalue[0]))
        {
            m_DBVarstru.cyMap.insert(listvalue[0],stru);
            if(m_ModVarcyMap.contains(listvalue[0]))
            {
                m_ModVarcyMap[listvalue[0]] = stru;//包含就替换
            }
            else
            {
                m_ModVarcyMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "运动控制CSP模式")
    {
        VARDB_LCSP_STRU stru;
        strcpy(stru.type,listvalue[1].toStdString().c_str());
        strcpy(stru.pkey,listvalue[0].toStdString().c_str());
        strcpy(stru.note,listvalue[2].toStdString().c_str());
        strcpy(stru.fkey,listvalue[3].toStdString().c_str());
        if(!m_DBVarstru.cspMap.contains(listvalue[0]))
        {
            m_DBVarstru.cspMap.insert(listvalue[0],stru);
            if(m_ModvarcspMap.contains(listvalue[0]))
            {
                m_ModvarcspMap[listvalue[0]] = stru;//包含就替换
            }
            else
            {
                m_ModvarcspMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "数值型寄存器")
    {
        VARDB_REGNUM_STRU stru;
        strcpy(stru.type,listvalue[1].toStdString().c_str());
        strcpy(stru.pkey,listvalue[0].toStdString().c_str());
        strcpy(stru.note,listvalue[2].toStdString().c_str());
        strcpy(stru.fkey,listvalue[3].toStdString().c_str());
        if(!m_DBVarstru.regnumMap.contains(listvalue[0]))
        {
            m_DBVarstru.regnumMap.insert(listvalue[0],stru);
            if(m_ModVarregnumMap.contains(listvalue[0]))
            {
                m_ModVarregnumMap[listvalue[0]] = stru;//包含就替换
            }
            else
            {
                m_ModVarregnumMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }

                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "运动控制PP模式")
    {
        VARDB_PPMOTCTR_STRU stru;
        strncpy(stru.type,listvalue[1].toStdString().c_str(),32);
        strncpy(stru.pkey,listvalue[0].toStdString().c_str(),32);
        strncpy(stru.note,listvalue[2].toStdString().c_str(),128);
        strncpy(stru.fkey,listvalue[3].toStdString().c_str(),32);
        if(!m_DBVarstru.qpMap.contains(listvalue[0]))
        {
            m_DBVarstru.qpMap.insert(listvalue[0],stru);
            if(m_ModVarqpMap.contains(listvalue[0]))
            {
                m_ModVarqpMap[listvalue[0]] = stru;//包含就替换
                strncpy(m_ModVarqpMap[listvalue[0]].axisidinfo,stru.axisidinfo,256);
                strncpy(m_ModVarqpMap[listvalue[0]].childtype,stru.childtype,16);
                strncpy(m_ModVarqpMap[listvalue[0]].date,stru.date,32);
                strncpy(m_ModVarqpMap[listvalue[0]].fkey,stru.fkey,32);
                strncpy(m_ModVarqpMap[listvalue[0]].type,stru.type,32);
                strncpy(m_ModVarqpMap[listvalue[0]].pkey,stru.pkey,32);
                strncpy(m_ModVarqpMap[listvalue[0]].note,stru.note,128);
            }
            else
            {
                m_ModVarqpMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }


                else if(strPreText == "开关类")
                {
                    if(m_DBVarstru.SwitchMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.SwitchMap.remove(listvalue[0]);
                    }
                    if(m_ModVarswMap.contains(listvalue[0]))
                    {
                        m_ModVarswMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
    else if(listvalue[1] == "开关类")
    {
        VARDB_SWITCH_STRU stru;
        strcpy(stru.type,listvalue[1].toStdString().c_str());
        strcpy(stru.pkey,listvalue[0].toStdString().c_str());
        strcpy(stru.note,listvalue[2].toStdString().c_str());
        strcpy(stru.fkey,listvalue[3].toStdString().c_str());
        if(!m_DBVarstru.SwitchMap.contains(listvalue[0]))
        {
            m_DBVarstru.SwitchMap.insert(listvalue[0],stru);
            if(m_ModVarswMap.contains(listvalue[0]))
            {
                m_ModVarswMap[listvalue[0]] = stru;//包含就替换
            }
            else
            {
                m_ModVarswMap.insert(listvalue[0],stru);//否则插入修改行列
            }
            if(strPreText != "")
            {
                if(strPreText == "计数类")
                {
                    if(m_DBVarstru.countMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.countMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcountMap.contains(listvalue[0]))
                    {
                        m_ModVarcountMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "字符串型寄存器")
                {
                    if(m_DBVarstru.regtextMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regtextMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregtextMap.contains(listvalue[0]))
                    {
                        m_ModVarregtextMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "计时类")
                {
                    if(m_DBVarstru.timeMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.timeMap.remove(listvalue[0]);
                    }
                    if(m_ModVartimeMap.contains(listvalue[0]))
                    {
                        m_ModVartimeMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "气缸类")
                {
                    if(m_DBVarstru.cyMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cyMap.remove(listvalue[0]);
                    }
                    if(m_ModVarcyMap.contains(listvalue[0]))
                    {
                        m_ModVarcyMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制CSP模式")
                {
                    if(m_DBVarstru.cspMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.cspMap.remove(listvalue[0]);
                    }
                    if(m_ModvarcspMap.contains(listvalue[0]))
                    {
                        m_ModvarcspMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "数值型寄存器")
                {
                    if(m_DBVarstru.regnumMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.regnumMap.remove(listvalue[0]);
                    }
                    if(m_ModVarregnumMap.contains(listvalue[0]))
                    {
                        m_ModVarregnumMap.remove(listvalue[0]);
                    }
                }
                else if(strPreText == "运动控制PP模式")
                {
                    if(m_DBVarstru.qpMap.contains(listvalue[0]))
                    {
                        m_DBVarstru.qpMap.remove(listvalue[0]);
                    }
                    if(m_ModVarqpMap.contains(listvalue[0]))
                    {
                        m_ModVarqpMap.remove(listvalue[0]);
                    }
                }
            }
        }
    }
}



