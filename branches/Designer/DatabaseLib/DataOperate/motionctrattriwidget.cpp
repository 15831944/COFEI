#include "motionctrattriwidget.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "ComboBoxDelegate.h"
#include <QValidator>
#include <QDebug>
#include <QMessageBox>
#include <Hardware/HardwareFrom.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

MotionCtrAttriWidget::MotionCtrAttriWidget(QWidget *parent) : QWidget(parent)
{
    createUI();
}

MotionCtrAttriWidget::~MotionCtrAttriWidget()
{

}

void MotionCtrAttriWidget::createUI()
{
    QGroupBox *group = new QGroupBox (tr("运动控制类PP模式"));
    group->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:5px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *gridlay = new QGridLayout();
    QLabel *tmodlabel = new QLabel ("变量名:");
    m_namelabel  = new QLabel ("");
    QLabel *delaylabel = new QLabel ("延时:");
    m_delayline= new QLineEdit (this);
    QLabel *timeout = new QLabel("超时:");
    m_timoutline = new QLineEdit(this);
    QLabel *acclabel = new QLabel("加速度:");
    m_accline = new QLineEdit(this);
    QLabel *declabel = new QLabel("减速度:");
    m_deccline = new QLineEdit(this);
    m_savetoDb = new QCheckBox(tr("是否保存到数据库"));
    QPushButton *savebtn = new QPushButton();//width:36px;
    savebtn->setStyleSheet("QPushButton{height:18;font:11px;border:1px solid white;border-radius:3px;}"\
                           "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                           "QPushButton:pressed{background-color:rgba(255,255,255,150);}");
    savebtn->setText("保存属性");
    QIntValidator *pIntValidator = new QIntValidator(this);
    m_delayline->setValidator(pIntValidator);
    m_timoutline->setValidator(pIntValidator);
    m_accline->setValidator(pIntValidator);
    m_deccline->setValidator(pIntValidator);
    QStringList hlist;
    hlist<<"轴ID"<<"寄存器变量"<<"速度百分率"<<"偏移";
    m_tableWg = new QTableWidget();
    m_tableWg->setShowGrid(true);
    m_tableWg->verticalHeader()->setVisible(false);
    m_tableWg->setSelectionBehavior(QAbstractItemView::SelectRows);//选择一行
    m_tableWg->setColumnCount(4);//速度百分率相关内容
    m_tableWg->setHorizontalHeaderLabels(hlist);
    m_tableWg->setRowCount(6);
    for(int i= 0; i < 6;++i)
    {
        m_tableWg->setRowHeight(i,25);
    }
    for(int i  =0 ; i < 6; ++i)
    {
        for(int j = 0; j < 4;++j)
        {
            QTableWidgetItem *item = new  QTableWidgetItem();
            item->setText("");
            m_tableWg->setItem(i,j,item);
        }
    }
    QStringList axisNames,regnumNames;
    GetAxisandRegNumNames(axisNames,regnumNames);
    m_tableWg->setItemDelegateForColumn(0, new ComboBoxDelegate(axisNames,this));//添加QCombox代理
    m_tableWg->setItemDelegateForColumn(1, new ComboBoxDelegate(regnumNames,this));//添加QCombox代理
    m_tableWg->setItemDelegateForColumn(2,new SpeedRateDelegate(this));
    gridlay->addWidget(tmodlabel,0,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_namelabel,0,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(delaylabel,2,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_delayline,2,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(timeout,3,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_timoutline,3,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(acclabel,4,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_accline,4,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(declabel,5,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_deccline,5,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(m_tableWg,6,0,1,2,Qt::AlignCenter);

    gridlay->addWidget(m_savetoDb,8,0,1,2);
    gridlay->addWidget(savebtn,9,0,1,1);

    //设置布局比例
    gridlay->setColumnStretch(0,1);
    gridlay->setColumnStretch(1,2);
    gridlay->setAlignment(Qt::AlignLeft);
    group->setAlignment(Qt::AlignTop);
    group->setLayout(gridlay);
    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(group);
    this->setLayout(lay);
    connect(savebtn,&QPushButton::clicked,this,&MotionCtrAttriWidget::slotMotionCtrAttriChanged);
    connect(m_tableWg,&QTableWidget::itemChanged,this,&MotionCtrAttriWidget::slotCheckAxisInfo);
}

QString MotionCtrAttriWidget::GetAxisInfoString()
{
    QString axisinfo = "";
    for(int i = 0; i < 6; ++i)
    {
        if((m_tableWg->item(i,0)->text()!= "")&&(m_tableWg->item(i,1)->text()!= "")&&(m_tableWg->item(i,2)->text()!= ""))
        {
              QString offsetname = "0";
            if(m_tableWg->item(i,3)->text() != "")
            {
               offsetname = m_tableWg->item(i,3)->text();
            }
            if(offsetname != "0")//判断偏移设置变量的问题
            {
                if(!DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(offsetname))
                {
                    axisinfo = "";
                    return axisinfo;
                }
            }
            axisinfo +=m_tableWg->item(i,0)->text() + ":" + m_tableWg->item(i,1)->text() + "," + \
                    m_tableWg->item(i,2)->text() + "," + offsetname+";";
        }
        else{
          //  axisinfo = "";
          //  return axisinfo;
        }
    }
    if(axisinfo != "")
    {
        axisinfo.chop(1);
    }
    return axisinfo;
}

void MotionCtrAttriWidget::GetAxisandRegNumNames(QStringList &axisNames, QStringList &regnumNames)
{
    regnumNames.clear();
    axisNames.clear();
    for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.begin();\
        it!=DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.end();++it)
    {
        if(!it.key().contains("BUILT_IN_"))
        {
            regnumNames.append(it.key());
        }
    }
    Hardwarefrom *hdlib = new Hardwarefrom;
    axisNames = hdlib->GetAxisInfo().toList();
}

void MotionCtrAttriWidget::SetAxisInfoTable(QString axisInfo)
{
    QStringList list = axisInfo.split(";");
    for(int i = 0; i < list.size()&&list.size() <= 6;++i)
    {
        QString axisgroup = list[i];
        QStringList axislist = axisgroup.split(",");
        if(axislist.size()==2)
        {
            QString idpos = axislist[0];
            QStringList  idinfo =idpos.split(":") ;
            if(idinfo.size()==2)
            {
                m_tableWg->item(i,0)->setText(idinfo[0]);
                m_tableWg->item(i,1)->setText(idinfo[1]);
            }
            m_tableWg->item(i,2)->setText(axislist[1]);
        }
        else if(axislist.size() == 3){
            QString idpos = axislist[0];
            QStringList  idinfo = idpos.split(":") ;
            if(idinfo.size()==2)
            {
                m_tableWg->item(i,0)->setText(idinfo[0]);
                m_tableWg->item(i,1)->setText(idinfo[1]);
            }
            m_tableWg->item(i,2)->setText(axislist[1]);
            m_tableWg->item(i,3)->setText(axislist[2]);
        }
        else{
            QString idpos =list[i];
            QStringList  idinfo =idpos.split(":") ;
            if(idinfo.size()==2)
            {
                m_tableWg->item(i,0)->setText(idinfo[0]);
                m_tableWg->item(i,1)->setText(idinfo[1]);
                m_tableWg->item(i,2)->setText("100");
            }
        }
    }
}
void MotionCtrAttriWidget::SetData(VARDB_PPMOTCTR_STRU stru)
{
    m_baselist<<QString::fromUtf8(stru.pkey)<<QString::fromUtf8(stru.type)<<\
                QString::fromUtf8(stru.date)<<QString::fromUtf8(stru.fkey)<<QString::fromUtf8(stru.note);
    m_namelabel->setText(QString::fromUtf8(stru.pkey));
    m_accline->setText(QString::number(stru.acc));
    m_deccline->setText(QString::number(stru.dec));
    m_delayline->setText(QString::number(stru.delay));
    m_timoutline->setText(QString::number(stru.timeOut));
    QString Axisinfo = QString::fromUtf8(stru.axisidinfo);
    SetAxisInfoTable(Axisinfo);
    m_savetoDb->setChecked(stru.save);
}


void MotionCtrAttriWidget::slotMotionCtrAttriChanged()
{
    //获取当前widget界面数据的变化,并发送出去
    QString axinfo = GetAxisInfoString();
    if(axinfo != "")
    {
        VARDB_PPMOTCTR_STRU motqpstru;
        strncpy(motqpstru.pkey,m_baselist[0].toStdString().c_str(),32);
        strncpy(motqpstru.type,m_baselist[1].toStdString().c_str(),32);
        strncpy(motqpstru.date,m_baselist[2].toStdString().c_str(),32);
        strncpy(motqpstru.fkey,m_baselist[3].toStdString().c_str(),32);
        strncpy(motqpstru.note,m_baselist[4].toStdString().c_str(),128);
        strncpy(motqpstru.axisidinfo,axinfo.toStdString().c_str(),256);
        motqpstru.acc = m_accline->text().toInt();
        motqpstru.dec = m_deccline->text().toInt();
        motqpstru.delay = m_delayline->text().toInt();
        motqpstru.timeOut = m_timoutline->text().toInt();
        motqpstru.save = m_savetoDb->isChecked();
        emit signalAttriDataChanged("运动控制PP模式",&motqpstru);
    }
    else{
        QMessageBox::information(NULL,tr("警告"),tr("轴信息输入有误！"));
    }
}

void MotionCtrAttriWidget::slotCheckAxisInfo(QTableWidgetItem *current)
{
    int colmun = current->column();
    QString text = current->text();
    int row = current->row();
    switch (colmun) {
    case 0:
        for(int i = 0 ; i < 6; ++i)
        {
            if(i != row)
            {
                QString itemtext = m_tableWg->item(i,0)->text();
                if(itemtext != "")
                {
                    if(itemtext == text)
                    {
                        QMessageBox::information(NULL,tr("警告"),tr("轴ID不可重复，请重新选择！"));
                        m_tableWg->item(row,0)->setText("");
                        return;//轴ＩＤ不可重复
                    }
                }
            }
        }
        break;
    case 1:
    default:
        break;
    }
}

