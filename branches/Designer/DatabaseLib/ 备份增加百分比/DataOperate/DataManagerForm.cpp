#include <DatabaseLib/variable.h>
#include "CountAttriWidget.h"
#include "CylinderInfoAttriWidget.h"
#include "NumberRegisterAttriWidget.h"
#include "StringRegisterAttriWidget.h"
#include "TimingAttriWidget.h"
#include "VirtualIOAttriWidget.h"
#include "motionctrattriwidget.h"
#include "DataManagerForm.h"
#include <QTabWidget>
#include <QWidget>
#include <QApplication>
#include "cspmotionctrwidget.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QMetaType>


DataManagerForm::DataManagerForm(QWidget *parent) :
    QWidget(parent)
{
    m_pAttriArea = NULL;
    m_pVariableWidget = NULL;
    m_index = -1;
    //打开数据库
    QString str = QCoreApplication::applicationDirPath();
    bool bResult = DBInstanceManager::getDBInstance()->OpenDb(str+"/cofeidatabasev1.1.db");
    if(!bResult)
    {
        return ;
    }
   this->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:2px;"));
   DBInstanceManager::getDBInstance()->ReadRealTimeDataFromDB();//读实时信息表2018/8/10
   CreateDataUI();
}

DataManagerForm::~DataManagerForm()
{
    //DBInstanceManager::getDBInstance()->CloseDb();
}

QWidget *DataManagerForm::GetAttriArea()
{
    m_pAttriArea = NULL;
    m_pAttriArea = new QScrollArea();
    ChangetableAttrWgslot();
    return qobject_cast<QWidget*>(m_pAttriArea);
}

void DataManagerForm::CreateDataUI()
{
    m_datatabletabwg = new QTabWidget(this);
    connect(m_datatabletabwg,&QTabWidget::currentChanged,this,&DataManagerForm::ChangetableAttrWgslot);
    //Create 各个表的属性栏
    QScrollArea *pAreaatt= new QScrollArea();
    m_datatabletabwg->addTab(pAreaatt,"变量管理表");
    QVBoxLayout *vbox  = new QVBoxLayout();
    m_pVariableWidget = new VariableInforWidget(this);
    connect(m_pVariableWidget,&VariableInforWidget::signalShowAttriWidget,this,&DataManagerForm::slotShowAttriWidget);
    vbox->addWidget(m_pVariableWidget);
    pAreaatt->setLayout(vbox);

    QScrollArea *pscro_1 = new QScrollArea();
    m_datatabletabwg->addTab(pscro_1,"table00");
    QVBoxLayout *vbox_1 = new QVBoxLayout();
    QWidget *device = new QWidget();

    QPushButton *btn = new QPushButton(device);
    btn->setText("test");
    vbox_1->addWidget(device);
    vbox_1->addWidget(btn);
    pscro_1->setLayout(vbox_1);

    QScrollArea *pAreaother= new QScrollArea(m_datatabletabwg);
    m_datatabletabwg->addTab(pAreaother,"table01");
    QVBoxLayout *vxd_2 = new QVBoxLayout();
    pAreaother->setWidget(device);
    vxd_2->addWidget(device);
    pAreaother->setLayout(vxd_2);

    QVBoxLayout *boxV = new QVBoxLayout();
    boxV->addWidget(m_datatabletabwg);

    this->setLayout(boxV);
}

void DataManagerForm::slotShowAttriWidget(QString type, BASEDB_STRU *base)
{
    QString strType = type;
    if(m_pAttriArea == NULL) return;
    if(strType.compare(tr("计数类"),Qt::CaseInsensitive)==0)
    {
        CountAttriWidget *pCAWidget = new CountAttriWidget();
        connect(pCAWidget,&CountAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_COUNT_STRU *device = static_cast<VARDB_COUNT_STRU*>(base);
        pCAWidget->SetData(*device);
        m_pAttriArea->setWidget(pCAWidget);
    }
    else if(strType.compare(tr("字符串型寄存器"),Qt::CaseInsensitive)==0)
    {
        StringRegisterAttriWidget *pSRWidget = new StringRegisterAttriWidget(m_pAttriArea);
        connect(pSRWidget,&StringRegisterAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_REGTEXT_STRU *reg = static_cast<VARDB_REGTEXT_STRU*>(base);
        pSRWidget->SetData(*reg);
        m_pAttriArea->setWidget(pSRWidget);
    }
    else if(strType.compare(tr("数值型寄存器"),Qt::CaseInsensitive)==0)
    {
        NumberRegisterAttriWidget *pNRWidget = new NumberRegisterAttriWidget(m_pAttriArea);
        connect(pNRWidget,&NumberRegisterAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_REGNUM_STRU *reg = static_cast<VARDB_REGNUM_STRU*>(base);
        pNRWidget->SetData(*reg);
        m_pAttriArea->setWidget(pNRWidget);
    }
    else if(strType.compare(tr("开关类"),Qt::CaseInsensitive)==0)
    {
        VirtualIOAttriWidget *pVIOWidget = new VirtualIOAttriWidget(m_pAttriArea);
        connect(pVIOWidget,&VirtualIOAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_SWITCH_STRU *vio = static_cast<VARDB_SWITCH_STRU*>(base);
        pVIOWidget->SetData(*vio);
        m_pAttriArea->setWidget(pVIOWidget);
    }
    else if(strType.compare(tr("计时类"),Qt::CaseInsensitive)==0)
    {
        TimingAttriWidget *pCAWidget = new TimingAttriWidget();
        connect(pCAWidget,&TimingAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_TIME_STRU *device = static_cast<VARDB_TIME_STRU*>(base);
        pCAWidget->SetData(*device);
        m_pAttriArea->setWidget(pCAWidget);
    }
    else if(strType.compare(tr("气缸类"),Qt::CaseInsensitive)==0)
    {
        VARDB_CY_STRU *cy =  static_cast<VARDB_CY_STRU*>(base);
        CylinderInfoAttriWidget *pCIWidget = new CylinderInfoAttriWidget();
        connect(pCIWidget,&CylinderInfoAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        pCIWidget->SetData(*cy);
        m_pAttriArea->setWidget(pCIWidget);
    }
    else if(strType.compare(tr("运动控制PP模式"),Qt::CaseInsensitive)==0)
    {
        MotionCtrAttriWidget *pMotCtrWidget = new MotionCtrAttriWidget();
        connect(pMotCtrWidget,&MotionCtrAttriWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_PPMOTCTR_STRU *mot = static_cast<VARDB_PPMOTCTR_STRU*>(base);
        pMotCtrWidget->SetData(*mot);
        m_pAttriArea->setWidget(pMotCtrWidget);
    }
    else if(strType.compare(tr("运动控制CSP模式"),Qt::CaseInsensitive)==0)
    {
        cspMotionctrWidget *pMotCtrWidget = new cspMotionctrWidget();
        connect(pMotCtrWidget,&cspMotionctrWidget::signalAttriDataChanged,m_pVariableWidget,&VariableInforWidget::slotVarAttriDataChanged);
        VARDB_LCSP_STRU *mot = static_cast<VARDB_LCSP_STRU*>(base);
        pMotCtrWidget->SetData(*mot);
        m_pAttriArea->setWidget(pMotCtrWidget);
    }
}
void DataManagerForm::ChangetableAttrWgslot()
{
    m_index = m_datatabletabwg->currentIndex();
    if(m_pAttriArea == NULL)
    {
        m_pAttriArea = new QScrollArea();
    }
    if (m_index == 1)
    {
        QWidget *attriwg1 = new QWidget(m_pAttriArea);
        QVBoxLayout *lay = new QVBoxLayout;
        QPushButton *btn0 = new QPushButton(attriwg1);
        lay->addWidget(btn0);
        btn0->setText("table00属性");
        attriwg1->setLayout(lay);
        m_pAttriArea->setWidget(attriwg1);
    }
    else if(m_index == 0)
    { if(m_pVariableWidget != NULL)
        {
             int nCurRow = m_pVariableWidget->GetTableView()->currentIndex().row();
            if(nCurRow >= 0)
            {
                QModelIndex modename  =   m_pVariableWidget->GetTableView()->GetTablemodel()->index(nCurRow,0);
                QString name =  modename.data().toString();
                QModelIndex modetype  =   m_pVariableWidget->GetTableView()->GetTablemodel()->index(nCurRow,1);
                QString type =  modetype.data().toString();
                m_pVariableWidget->slotSelectedRowChanged(type,name);
            }
        }
    }
    else if(m_index == 2)
    {
        QWidget *attriwg2 = new QWidget(m_pAttriArea);
        QVBoxLayout *lay = new QVBoxLayout;
        QPushButton *btn0 = new QPushButton(attriwg2);
        lay->addWidget(btn0);
        btn0->setText("table01属性");
        attriwg2->setLayout(lay);
        m_pAttriArea->setWidget(attriwg2);
    }
}

