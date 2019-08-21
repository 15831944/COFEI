#include "customtabwidget.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <algorithm>
#include <QtAlgorithms>
#include <QSet>
#include "calibrationui.h"
#include <QDebug>
#include <DycLibBase/StackManager/stackmanager.h>

const QString Stylestr = "QPushButton{background-color:rgb(112,112,112);color:black;}"
                         "QPushButton:hover{background-color:rgb(60,60,60);}"
                         "QPushButton:pressed{background-color:rgb(40,40,40);}";
#define _Region(a,b,c) if(c < a) c = a;\
    if(c > b) c = b;
CustomTabWidget::CustomTabWidget(QStringList vec,QWidget *parent)
{
    m_pe = parent;
    m_HeaderList = vec;
    m_serQPtemp.clear();
    m_serLCSPtemp.clear();
    m_QPtemp.clear();
    m_RunMap.clear();
    m_runbtnMap.clear();
    m_stopbtnMap.clear();
    m_addID = "";
    m_reject = false;
    m_pTimer = NULL;
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    CreateTabHeader();
    QFont font;
    font.setPointSize(11);
    font.setFamily("黑体");
    this->setFont(font);
    axisEventMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_ProtectEvent_Map.axisEventMap;
}

void CustomTabWidget::CreateTabHeader()
{
    this->setFixedWidth(m_pe->width()-20);
    this->setColumnCount(m_HeaderList.count());
    this->horizontalHeader()->setDefaultSectionSize(m_pe->width()/m_HeaderList.count());
    this->resizeColumnToContents(m_HeaderList.count());
    this->setHorizontalHeaderLabels(m_HeaderList);
    horizontalHeader()->setStyleSheet("QHeaderView::section{border:1px ;border-radius:4px;padding:2px 4px;background-color: rgb(75, 75, 75);font: 13pt '黑体'}");
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setDefaultSectionSize(40);
    this->setFrameShape(QFrame::NoFrame);
    this->setShowGrid(true);
    this->verticalHeader()->setVisible(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
   this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);
    this->horizontalHeader()->resizeSection(0,this->width()/8);
    this->horizontalHeader()->resizeSection(1,this->width()/16);
    this->horizontalHeader()->resizeSection(2,this->width()/8);
    this->horizontalHeader()->resizeSection(3,this->width()/16);
    this->horizontalHeader()->resizeSection(4,this->width()/16);
    this->horizontalHeader()->resizeSection(5,this->width()/16);
    this->horizontalHeader()->resizeSection(6,5*this->width()/16);
    this->horizontalHeader()->resizeSection(7,3*this->width()/16);
    this->horizontalHeader()->setFixedHeight(30);
    this->horizontalHeaderItem(7)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setStyleSheet("selection-background-color:rgb(20,50,100);border-radius:12px;");
    m_serQPtemp = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
    m_serLCSPtemp = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map;
    UpdataTabWidGet();
}
void CustomTabWidget::customContextMenu(const QPoint& )
{
    QMenu *menu = CreateContextMenu();
    menu->exec(QCursor::pos());
}

QString CustomTabWidget::GetStrID( QPushButton *senderObj)
{
    QString strId = "";
    for(int i = 0; i < this->rowCount()-1;++i)
    {
        if(this->item(0,0)->text() == "快速定位")
        {
            if(this->cellWidget(i,this->columnCount()-1) == senderObj->parent())
            {
                strId = this->item(i,1)->text();
                break;
            }
        }
    }
    return strId;
}

bool CustomTabWidget::JudgeAsixEvent(QString strId)
{
    SWCONFIGXML_QPOSIT_STRU ps = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId];
    QVector<QString> AxisVec;
    int inum = 0;
    for(auto it = ps.axisMap.begin(); it!= ps.axisMap.end();++it)
    {
        if(inum == ps.axisNum)
            break;
        if(!AxisVec.contains(it.key()))
        {
            AxisVec.append(it.key());
        }
    }
    for(int i = 0;i < AxisVec.size();++i)
    {
        if(axisEventMap.contains(AxisVec[i]))
        {
            StackManager st;
            if(!st.AnalysisEvent(axisEventMap[AxisVec[i]].strEvent))
            {
                return false;
            }
        }
    }
    return true;
}

void CustomTabWidget::DelRunData(QString strId)
{
    if(m_runbtnMap.contains(strId))
    {
        QMap<QString,QPushButton*> mapbtn;
        for(auto it = m_runbtnMap.begin(); it!=m_runbtnMap.end(); ++it )
        {
            if(it.key()!= strId)
            {
                mapbtn.insert(it.key(),it.value());
            }
        }
        m_runbtnMap.clear();
        m_runbtnMap = mapbtn;
    }
    if(m_RunMap.contains(strId))
    {
        QMap<QString,AxisRunPos*> mappos;
        for(auto it = m_RunMap.begin(); it!=m_RunMap.end(); ++it )
        {
            if(it.key()!= strId)
            {
                mappos.insert(it.key(),it.value());
            }
        }
        m_RunMap.clear();
        m_RunMap = mappos;
    }
    if(m_stopbtnMap.contains(strId))
    {
        QMap<QString,QPushButton*> mapbtn;
        for(auto it = m_stopbtnMap.begin(); it!=m_stopbtnMap.end(); ++it )
        {
            if(it.key()!= strId)
            {
                mapbtn.insert(it.key(),it.value());
            }
        }
        m_stopbtnMap.clear();
        m_stopbtnMap = mapbtn;
    }
}

void CustomTabWidget::ModPRunData(QString strId)
{
    if(m_RunMap.contains(strId))  //重新保存之后的结果,重新 增加go按钮之后的结果
    {
        AxisRunPos *ps;
        ps = new AxisRunPos(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId]);
        m_RunMap[strId] = ps;
    }
}

void CustomTabWidget::SetBtnStatus(bool enable)//设置启动之后不可用
{
    for(auto it = m_runbtnMap.begin(); it != m_runbtnMap.end();++it)//启动按钮
    {
        it.value()->setEnabled(enable);
    }
    for(auto it = m_stopbtnMap.begin(); it != m_stopbtnMap.end();++it)//stop按钮
    {
        it.value()->setEnabled(enable);
    }
}

void CustomTabWidget::reject()
{
  Deltablerowstyle();//当对话框关闭的时候不显示了
  m_reject = true;
}

void CustomTabWidget::Modtablerowstyle(int curow)
{
    if(curow != this->rowCount()-1) //先取消高亮的地方
    {
        Deltablerowstyle();
        for(int i = 0; i< this->columnCount();++i) //再设置当前地方为高亮
        {
            this->item(curow,i)->setBackgroundColor(QColor(100,100,10));
        }
        m_addID = QString("%1:%2").arg(this->item(curow,0)->text()).arg(this->item(curow,1)->text());
    }
}

void CustomTabWidget::Deltablerowstyle()
{
    for(int j =0 ; j < this->rowCount() -1; ++j)
    {
        if(this->item(j,0)->backgroundColor() == QColor(100,100,10))
        {
            for(int i = 0; i < this->columnCount();++i)
            {
              this->item(j,i)->setBackgroundColor(QColor(35,35,35));
            }
            break;
        }
    }
}

void CustomTabWidget::Run()
{
    QPushButton * senderObj = qobject_cast<QPushButton *>(sender());
    QString strId = GetStrID(senderObj);
    if(strId =="")
    {
        return;
    }
    if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(strId))
    {
        SWCONFIGXML_QPOSIT_STRU _ps = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId];
        if(CreateMessageBox("运行提示","请确认是否继续执行Run?"))
        {
            if(!JudgeAsixEvent(strId))//轴保护事件
            {
                CreateMessageBox("  运行提示","  轴保护禁止运行！");//弹出禁止运行的提示
                return;
            }
            if(!m_RunMap.contains(strId))//stop与之相关的轴
            {
                AxisRunPos *Runpos = new AxisRunPos(_ps);
                m_RunMap.insert(strId,Runpos);
                Runpos->StopgoPos();
            }
            else
            {
                m_RunMap[strId]->StopgoPos();
            }
            if(!m_RunMap.contains(strId))
            {
                AxisRunPos *Runpos = new AxisRunPos(_ps);
                Runpos->SetsumRunParam();
                m_RunMap.insert(strId,Runpos);//启动运行
            }
            else
            {
                m_RunMap[strId]->SetsumRunParam();
            }
            if(!m_pTimer->isActive())
            {
                m_pTimer->start(200);
            }
            senderObj->setText("Run...");
            senderObj->setStyleSheet("QPushButton{background-color:rgb(180,180,0);color:black;}");
            senderObj->setEnabled(false);
            emit DiableQPBtn(false); //设置调试界面中轴信息按钮不可用
        }
    }
}

void CustomTabWidget::stop()
{
    QPushButton * senderObj = qobject_cast<QPushButton *>(sender());
    QString strId = GetStrID(senderObj);
    if(strId == "")
    {
        return;
    }
    SWCONFIGXML_QPOSIT_STRU _ps = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId];
    if(!m_RunMap.contains(strId))//stop与之相关的轴
    {
        AxisRunPos *Runpos = new AxisRunPos(_ps);
        m_RunMap.insert(strId,Runpos);
        Runpos->StopgoPos();
    }
    else
    {
        m_RunMap[strId]->StopgoPos();
    }
    if(m_runbtnMap.contains(strId)) //设置run按钮可用
    {
        m_runbtnMap[strId]->setEnabled(true);//设置按钮可用
        m_runbtnMap[strId]->setText("Run");
        m_runbtnMap[strId]->setStyleSheet(Stylestr);
    }
}

void CustomTabWidget::timerUpInputData()
{
    //QString irstr = "";
    for(auto it = m_RunMap.begin();it != m_RunMap.end();++it )
    {
        if(m_runbtnMap.contains(it.key()))
        {
            if(!m_runbtnMap[it.key()]->isEnabled())
            {
                QString str = "";
                int iresult = 0;
                iresult = it.value()->RungoPos(str);//针对每一组，要设置
                if( iresult != 0)
                {
                    if(iresult == 1)
                    {
                        m_runbtnMap[it.key()]->setText("Run ok");
                        m_runbtnMap[it.key()]->setStyleSheet("QPushButton{background-color:rgb(0,180,0);color:black;}");
                    }
                    else
                    {
                        m_runbtnMap[it.key()]->setText(QString("error: %1").arg(iresult));
                        m_runbtnMap[it.key()]->setStyleSheet("QPushButton{background-color:rgb(180,0,0);color:white;}");
                    }
                    // irstr += QString("%1 %2!").arg(it.key()).arg(str);
                    m_runbtnMap[it.key()]->setEnabled(true);
                }
            }
        }
    }
    // if (irstr.compare("")!=0)//输入结果
    //{
    //qDebug()<<"iresult"<<irstr;

    //}
    bool enbtn = true;
    for(auto it = m_runbtnMap.begin(); it != m_runbtnMap.end();++it)
    {
        if(!it.value()->isEnabled())
        {
            enbtn = false;
        }
    }
    if(enbtn)//代表所有的已经执行完
    {
        emit DiableQPBtn(true);
        if( m_pTimer != NULL)
        {
            m_pTimer->stop();
        }
    }
}


void CustomTabWidget::InsertData(QVector<QString> vec,QString )
{
    int irow = this->rowCount();
    this->insertRow(irow);
    for( int i(0); i < vec.count();++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(vec[i]);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        this->setItem(irow,i,item);
    }
    if(vec.count() >= 1)
    {
        if(vec[0] =="快速定位")
        {
            if(!m_runbtnMap.contains(vec[1]))
            {
                QTableWidgetItem *item = new QTableWidgetItem("");
                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignCenter);
                this->setItem(irow,this->columnCount()-1,item);
                QWidget *wg = new QWidget;
                wg->setFixedSize(150,40);
                wg->setStyleSheet("background-color:rgb(0,255,0,0)");
                QHBoxLayout *Hbox = new QHBoxLayout(wg);
                Hbox->setAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
                QPushButton *Runbtn = new QPushButton(wg);
                Runbtn->setText("Run");
                Runbtn->setStyleSheet(Stylestr);
                Runbtn->setFixedSize(60,25);
                Hbox->addWidget(Runbtn);
                Hbox->setSpacing(20);
                QPushButton *stopbtn = new QPushButton(wg);
                stopbtn->setText("stop");
                stopbtn->setStyleSheet(Stylestr);
                stopbtn->setFixedSize(60,25);
                Hbox->addWidget(stopbtn);
                this->setCellWidget(irow,this->columnCount()-1,wg);
                connect(Runbtn,&QPushButton::clicked,this,&CustomTabWidget::Run);
                connect(stopbtn,&QPushButton::clicked,this,&CustomTabWidget::stop);
                m_runbtnMap.insert(vec[1],Runbtn);
                m_stopbtnMap.insert(vec[1],stopbtn);
                if( m_pTimer == NULL)
                {
                    m_pTimer = new QTimer(this);
                    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timerUpInputData()));
                }
            }
            else{
                QTableWidgetItem *item = new QTableWidgetItem("");
                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                this->setItem(irow,this->columnCount()-1,item);
                QWidget *wg = new QWidget;
                wg->setFixedSize(150,40);
                QHBoxLayout *Hbox = new QHBoxLayout(wg);
                Hbox->setAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
                Hbox->setSpacing(20);
                Hbox->addWidget(m_runbtnMap[vec[1]]);
                Hbox->addWidget(m_stopbtnMap[vec[1]]);
                this->setCellWidget(irow,this->columnCount()-1,wg);
            }
        }
        else if(vec[0] =="直线插补")
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignCenter);
            this->setItem(irow,this->columnCount()-1,item);
        }
    }
    this->item(irow,0)->setFlags(this->item(irow,0)->flags() & (~Qt::ItemIsEditable));
    this->item(irow,1)->setFlags(this->item(irow,1)->flags() & (~Qt::ItemIsEditable));
    this->item(irow,5)->setFlags(this->item(irow,5)->flags() & (~Qt::ItemIsEditable));
}

void CustomTabWidget::contextMenuEvent(QContextMenuEvent* event)
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
QMenu* CustomTabWidget::CreateContextMenu()
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建"),this);
    connect(add, &QAction::triggered, this, &CustomTabWidget::AddPoint);
    QAction *del = new QAction(QObject::tr("删除"),this);
    connect(del, &QAction::triggered, this, &CustomTabWidget::DeletePoint);
    QAction *updata = new QAction(QObject::tr("修改"),this);
    connect(updata, &QAction::triggered, this, &CustomTabWidget::UpdataPoint);
    QAction *save = new QAction(QObject::tr("保存"),this);
    connect(save, &QAction::triggered, this, &CustomTabWidget::SavePoints);

    menu->addAction(add);
    menu->addAction(del);
    menu->addAction(updata);
    menu->addAction(save);
    return menu;
}
void CustomTabWidget::AddPoint()
{
    if(m_pTimer!=NULL)
    {
        if(m_pTimer->isActive())
            return;
    }
    QString add = "新建点：";
    ConfigData::GetInstance()->m_mutex.lock();
    CheckDlg *cg = new CheckDlg(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap);
    ConfigData::GetInstance()->m_mutex.unlock();
    cg->setGeometry(m_pe->width()/3,m_pe->height()/8,m_pe->width()/3,m_pe->height()/8*6);
    cg->resize(m_pe->width()/3,m_pe->height()/8*6);
    if(QDialog::Accepted == cg->exec())
    {
        QVector<QString> vec;
        cg->GetCheckBox(vec);
        QMap<QString,int> temp;
        QString strtype = cg->GetConfigType();
        if(strtype.compare("快速定位") == 0)
        {
            SWCONFIGXML_QPOSIT_STRU swSer ;
            swSer.id = cg->GetConfigID();
            swSer.note = cg->GetConfigNote();
            swSer.timeOut = cg->GetConfigTimeout().toInt();
            swSer.delayMsec = cg->GetConfigDelay().toInt();
            cg->GetTableWidgetData(temp);
            if(vec.count() > 0)
            {
                emit UpdateCalibrationUI(vec,&swSer,temp,add,strtype);
                m_reject = false;
                m_addID =  QString("快速定位:%1").arg(swSer.id);             
                Deltablerowstyle();   //取消当前显示的
            }
        }
        else
        {
            SWCONFIGXML_LCSP_STRU swSer;
            swSer.id = cg->GetConfigID();
            swSer.note = cg->GetConfigNote();
            swSer.timeOut = cg->GetConfigTimeout().toInt();
            swSer.delayMsec = cg->GetConfigDelay().toInt();
            swSer.speed = cg->GetConfigCspSpeed().toInt();
            swSer.acc = cg->GetConfigCspAcc().toInt();
            if(vec.count() > 0)
            {
                emit UpdateCalibrationUI(vec,&swSer,temp,add,strtype);
                 m_reject = false;
               m_addID =  QString("直线插补:%1").arg(swSer.id);
                Deltablerowstyle(); //取消当前显示的
            }
        }

    }
    return;
}

void CustomTabWidget::DeletePoint()
{
    if(m_pTimer!=NULL)
    {
        if(m_pTimer->isActive())
        {
            return;
        }
    }
    int curow = this->currentRow();
    QString strtype = this->item(curow, 0)->text();
    if(strtype.isEmpty())
        return;
    if(strtype.compare("快速定位") == 0)
    {
        QString strId = this->item(curow, 1)->text();
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(strId))
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.remove(strId);
            DelRunData(strId);
        }
    }
    else if(strtype.compare("直线插补") == 0)
    {
        QString strId = this->item(curow, 1)->text();
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(strId))
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.remove(strId);
        }
    }
    this->removeRow(curow);
}
void CustomTabWidget::UpdateData(QList<QTableWidgetItem*> items,QVector<QString> vec,QString str)
{
    for( int i(0); i < vec.count();++i)
    {
        items[i]->setText(vec[i]);
    }
    items[3]->setToolTip(str);

}
void CustomTabWidget::UpdataPoint()
{
    if(m_pTimer!=NULL)
    {
        if(m_pTimer->isActive())
        {
            return;
        }
    }
    QString update = "修改点：";
    QList<QTableWidgetItem*> items = this->selectedItems();
    QString strtype = items[0]->text();
    QString strId = items[1]->text();
    QString strNote = items[2]->text();
    QStringList list = items[5]->text().split(" ; ");
    //m_sertemp[strtype][strId].
    ConfigData::GetInstance()->m_mutex.lock();
    CheckDlg *cg = new CheckDlg(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap,false);
    ConfigData::GetInstance()->m_mutex.unlock();
    cg->setGeometry(m_pe->width()/3,m_pe->height()/8,m_pe->width()/3,m_pe->height()/8*6);
    cg->resize(m_pe->width()/3,m_pe->height()/8*6);
    cg->SetConfigID(strId);
    cg->SetConfigNote(strNote);
    cg->SetConfigType(strtype);
    cg->SetConfigTimeout(items[3]->text().toInt());
    cg->SetConfigDelay(items[4]->text().toInt());
    if(strtype.compare("快速定位") == 0)
    {
        cg->SetQPStru(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId]);
    }
    else
    {
        auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId];
        cg->SetConfigCspSpeed(item.speed);
        cg->SetConfigCspAcc(item.acc);
    }
    cg->SetCheckBox(list.toVector());
    if(QDialog::Accepted == cg->exec())
    {
        QVector<QString> vec;
        cg->GetCheckBox(vec);
        QMap<QString,int> temp;
        QString strtype = cg->GetConfigType();
        if(strtype.compare("快速定位") == 0)
        {
            SWCONFIGXML_QPOSIT_STRU swSer;
            swSer.id = cg->GetConfigID();
            swSer.note = cg->GetConfigNote();
            swSer.timeOut = cg->GetConfigTimeout().toInt();
            swSer.delayMsec = cg->GetConfigDelay().toInt();
            cg->GetTableWidgetData(temp);
            if(vec.count() > 0)
            {
                emit UpdateCalibrationUI(vec,&swSer,temp,update,strtype);
                m_reject = false;
                ModPRunData(strId);
                int row = this->currentRow();// 新增修改点位样式的
                Modtablerowstyle(row);
            }
        }
        else
        {
            SWCONFIGXML_LCSP_STRU swSer;
            swSer.id = cg->GetConfigID();
            swSer.note = cg->GetConfigNote();
            swSer.timeOut = cg->GetConfigTimeout().toInt();
            swSer.delayMsec = cg->GetConfigDelay().toInt();
            swSer.delayMsec = cg->GetConfigDelay().toInt();
            swSer.speed = cg->GetConfigCspSpeed().toInt();
            swSer.acc = cg->GetConfigCspAcc().toInt();
            if(vec.count() > 0)
            {
                emit UpdateCalibrationUI(vec,&swSer,temp,update,strtype);
                 m_reject = false;
                int row = this->currentRow();// 新增修改点位样式的
                Modtablerowstyle(row);
            }
        }
    }
}

void CustomTabWidget::SavePoints()
{
    if(m_pTimer!=NULL)
    {
        if(m_pTimer->isActive())
        {
            return;
        }
    }
    for(int i = 0; i < this->rowCount()-1;++i)
    {
        QString strtype = this->item(i, 0)->text();
        if(strtype.compare("快速定位") == 0)
        {
            QString strId = this->item(i, 1)->text();
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(strId))
            {
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].note = this->item(i, 2)->text();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].timeOut = this->item(i, 3)->text().toInt();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].delayMsec = this->item(i, 4)->text().toInt();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].acc = ConfigData::GetInstance()->iAccel;
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].dec = ConfigData::GetInstance()->iDccel;
                QString axis = this->item(i, 5)->text();
                QStringList axislist = axis.split(" ; ");
                QString axisInfotemp = this->item(i, 6)->text();
                QStringList axisInfolist = axisInfotemp.split(";  ");
                if(axisInfolist.count() != axislist.count())
                {
                    CreateMessageBox("错误",QString("ID:%1 轴号与轴信息不匹配,请修改后重新保存!").arg(strId));
                    return;
                }
                for(auto it = axisInfolist.begin();it != axisInfolist.end();++it)
                {
                    QString strInfo = *it;
                    QString key = strInfo.mid(0,strInfo.indexOf("->"));
                    if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].axisMap.contains(key))
                    {
                        CreateMessageBox("错误",QString("ID:%1 轴号与轴信息不匹配,请修改后重新保存!").arg(strId));
                        return;
                    }
                    bool isok = true;
                    QStringList list = strInfo.mid(strInfo.indexOf("->")+2).split(",");
                    if(list.count() == 2)
                    {
                        if(!list[0].contains("Speed:"))
                        {
                            isok = false;
                        }
                        if(!list[1].contains("Pos:"))
                        {
                            isok = false;
                        }
                    }
                    else
                    {
                        isok = false;
                    }
                    if(!isok)
                    {
                        CreateMessageBox("错误",QString("ID:%1 轴信息不正确,请修改后重新保存!").arg(strId));
                        return;
                    }
                    else
                    {
                        ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].axisMap[key].pos = list[1].mid(4).toDouble();
                        int ispeed = list[0].mid(6).toInt();
                        _Region(0,100,ispeed)
                                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].axisMap[key].speed = ispeed;
                        ModPRunData(strId);//运行数据更新增加go按钮之后
                    }
                }
            }
        }
        else  if(strtype.compare("直线插补") == 0)
        {
            QString strId = this->item(i, 1)->text();
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(strId))
            {
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].note = this->item(i, 2)->text();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].timeOut = this->item(i, 3)->text().toInt();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].delayMsec = this->item(i, 4)->text().toInt();
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].acc = ConfigData::GetInstance()->icspAccel;
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].LCSPCPos.clear();
                QString axis = this->item(i, 5)->text();
                QStringList axislist = axis.split(" ; ");
                QString axisInfotemp = this->item(i, 6)->text();
                QStringList axisInfolist = axisInfotemp.split("}");
                LCSP_CSYSINFO_STRU lcspStru;
                for(auto it = axisInfolist.begin();it != axisInfolist.end();++it)
                {
                    QString strInfo = *it;
                    if(strInfo.isEmpty())
                        continue;
                    strInfo.remove("{");
                    QStringList Poslist = strInfo.split("]");
                    for(auto itor = Poslist.begin();itor != Poslist.end();++itor)
                    {
                        QString strPosVec = *itor;
                        if(strPosVec.isEmpty())
                            continue;
                        strPosVec.remove("[");
                        if(strPosVec.contains("Pos"))
                        {
                            QStringList list = strPosVec.split(";");
                            for(auto ir = list.begin();ir != list.end();++ir)
                            {
                                QString strPos = *ir;
                                if(strPos.isEmpty())
                                    continue;
                                QStringList keyvaule = strPos.split("->Pos:");
                                if(!axislist.contains(keyvaule[0]) && keyvaule.count() < 2)
                                {
                                    CreateMessageBox("错误",QString("ID:%1 轴号与轴信息不匹配,请修改后重新保存!").arg(strId));
                                    return;
                                }
                                LCSP_CPOSINFO_STRU lcsp;
                                lcsp.pos =keyvaule[1].toDouble();
                                lcspStru.LCSCPosMap.insert(keyvaule[0],lcsp);
                            }
                        }
                        else
                        {
                            QStringList list = strPosVec.split(";");
                            for(auto ir = list.begin();ir != list.end();++ir)
                            {
                                QString strPos = *ir;
                                if(strPos.isEmpty())
                                    continue;
                                QStringList TigPos = strPos.split("->");
                                if(TigPos.count() < 2)
                                {
                                    CreateMessageBox("错误",QString("ID:%1 触发信息的IO与点位不匹配,请修改后重新保存!").arg(strId));
                                    return;
                                }
                                LCSP_TRIGGERINFO_STRU tigStru;
                                QString IOInfo = TigPos[0];
                                tigStru.bitName = IOInfo.mid(0,IOInfo.indexOf(":"));
                                tigStru.stat = IOInfo.mid(IOInfo.indexOf(":")+1).toInt();
                                QString PosInfo = TigPos[1];

                                PosInfo.remove("(").remove(")");
                                QStringList tigposList = PosInfo.split(",");
                                for(auto ii = tigposList.begin();ii != tigposList.end();++ii)
                                {
                                    QString str = *ii;
                                    LCSP_TRIGGER_CPOS_STRU tigPosstru;
                                    tigPosstru.id = str.mid(0,str.indexOf(":"));
                                    tigPosstru.pos = str.mid(str.indexOf(":")+1).toDouble();
                                    tigStru.LCSPTrigCPos.append(tigPosstru);
                                }
                                lcspStru.LCSPTrigInfo.append(tigStru);
                            }
                        }
                    }
                    ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].LCSPCPos.append(lcspStru);
                }
            }
        }
    }
    emit SendAxisSerConfig(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map,
                         ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map);
}
bool CustomTabWidget::CreateMessageBox(QString strTitle,QString note)
{
    QFont font;
    font.setPointSize(11);
    font.setFamily("黑体");
    QDialog box ;
    box.setWindowTitle(strTitle);
    box.setFixedSize(400,150);
    box.setFont(font);

    QLabel *lable = new QLabel(&box);
    lable->setText(note);
    lable->setFont(font);
    lable->setWordWrap(true);
    lable->setGeometry(0,30,400,150);
    lable->setAlignment( Qt::AlignCenter);

    QVBoxLayout *vbox = new QVBoxLayout(&box);
    box.setLayout(vbox);

    vbox->addWidget(lable);
    box.setStyleSheet(tr("background-color:rgb(46,50,57);color:white;border-radius:5px;"));
    QHBoxLayout *hBox = new QHBoxLayout(&box);
    QPushButton *btn = new QPushButton("确认",this);
    connect(btn,&QPushButton::clicked,&box,&QDialog::accept);
    btn->setStyleSheet(tr("background-color:rgb(191,82,33);color:white;border-radius:5px;"));
    btn->setFixedSize(106,32);
    btn->setFont(font);
    QPushButton *btn1 = new QPushButton("取消",this);
    connect(btn1,&QPushButton::clicked,&box,&QDialog::reject);
    btn1->setStyleSheet(tr("background-color:rgb(191,82,33,180);color:white;border-radius:5px;"));
    btn1->setFixedSize(106,32);
    btn1->setFont(font);
    hBox->addWidget(btn);
    hBox->addWidget(btn1);
    vbox->addLayout(hBox);
    if(QDialog::Accepted == box.exec())
        return true;

    return false;
}
void CustomTabWidget::UpdataTabWidGet()
{
    disconnect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onUserClickListener(QModelIndex)));
    this->setRowCount(0);
    this->clearContents();
    //for(auto item = m_serQPtemp.begin();item != m_serQPtemp.end();++item)
    {
        for(auto it = m_serQPtemp.begin();it != m_serQPtemp.end();++it)
        {
            QVector<QString> vec;
            vec.append("快速定位");
            vec.append(it.key());
            SWCONFIGXML_QPOSIT_STRU stru = it.value();
            m_QPtemp.insert(it.key(),stru);
            vec.append(stru.note);
            vec.append(QString("%1").arg(stru.timeOut));
            vec.append(QString("%1").arg(stru.delayMsec));
            QString str = "";
            QString strTip = "";
            int inum = 0;
            for(auto i = stru.axisMap.begin();i != stru.axisMap.end();++i)
            {
                if(inum++ == stru.axisNum)
                    break;
                str += i.key() + " ; ";
                strTip += QString("%1->Speed:%2,Pos:%3;  ").arg(i.key()).arg(i.value().speed).arg(i.value().pos);
            }
            str = str.mid(0,str.count()-3);
            strTip = strTip.mid(0,strTip.count()-3);
            vec.append(str);
            vec.append(strTip);
            InsertData(vec,strTip);
        }
    }
    //for(auto item = m_serLCSPtemp.begin();item != m_serLCSPtemp.end();++item)
    {
        for(auto it = m_serLCSPtemp.begin();it != m_serLCSPtemp.end();++it)
        {
            QVector<QString> vec;
            vec.append("直线插补");
            vec.append(it.key());
            SWCONFIGXML_LCSP_STRU stru = it.value();
            vec.append(stru.note);
            vec.append(QString("%1").arg(stru.timeOut));
            vec.append(QString("%1").arg(stru.delayMsec));
            QString str = "";
            QSet<QString> setKey;
            QString strTip = "";
            for(auto item = stru.LCSPCPos.begin();item != stru.LCSPCPos.end();++item)
            {
                strTip += "{";
                QString Pos = "[";
                for(auto it1 = item->LCSCPosMap.begin();it1 != item->LCSCPosMap.end();++it1)
                {
                    Pos += QString("%1->Pos:%2;").arg(it1.key()).arg(it1.value().pos);
                    setKey.insert(it1.key());
                }
                Pos = Pos.left(Pos.size()-1);
                Pos += "]";
                QString TrgInfo = "";
                if(item->LCSPTrigInfo.size() > 0)
                {
                    TrgInfo = "[";
                    for(auto it2 = item->LCSPTrigInfo.begin(); it2 != item->LCSPTrigInfo.end();++it2)
                    {
                        QString tigPos = "(";
                        for(auto itor = it2->LCSPTrigCPos.begin();itor != it2->LCSPTrigCPos.end();++itor)
                        {
                            tigPos += QString("%1:%2,").arg(itor->id).arg(itor->pos);
                        }
                        tigPos = tigPos.left(tigPos.size()-1);
                        tigPos += ")";
                        TrgInfo += QString("%1:%2->%3;").arg(it2->bitName).arg(it2->stat).arg(tigPos);
                    }
                    TrgInfo = TrgInfo.left(TrgInfo.size()-1);
                    TrgInfo += "]";
                }

                strTip += QString("%1%2").arg(Pos).arg(TrgInfo);
                strTip += "}";
            }
            for(auto it = setKey.begin();it != setKey.end();++it)
            {
                str += QString("%1 ; ").arg(*it);
            }
            str = str.left(str.size()-3);
            vec.append(str);
            vec.append(strTip);
            InsertData(vec,strTip);
        }
    }
    //新建点
    if (m_reject)
    {
        m_addID = "";
    }
    if( m_addID != "")
    {
        QStringList list = m_addID.split(":");
        if(list.size() > 1)
        {
            for(int j =0 ; j < this->rowCount() -1; ++j)
            {
                if(this->item(j,0)->text()==list[0])
                {
                    if(this->item(j,1)->text() == list[1])
                    {
                        Modtablerowstyle(j);
                        break;
                    }
                }
            }

        }
    }
    QVector<QString> vec1(7,"");
    InsertData(vec1,"");
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onUserClickListener(QModelIndex)));
}

void CustomTabWidget::onUserClickListener(QModelIndex)
{
    int curow = this->currentRow();
    m_reject = false;
    Modtablerowstyle(curow);//新增界面行显示高亮以便用户观察
    QString strtype = this->item(curow, 0)->text();
    ConfigData::GetInstance()->m_mutex.lock();
    if(strtype.compare("快速定位") == 0)
    {
        //if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(strtype))
        {
            QString strId = this->item(curow, 1)->text();
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(strId))
            {
                SWCONFIGXML_QPOSIT_STRU swSer;
                swSer.id = strId;
                swSer.note = this->item(curow, 2)->text();
                swSer.acc = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].acc;
                swSer.dec = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].dec;
                swSer.axisNum = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].axisNum;
                swSer.axisMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].axisMap;
                swSer.delayMsec = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].delayMsec;
                swSer.speed = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].speed;
                swSer.timeOut = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[strId].timeOut;
                QVector<QString> vec;
                QMap<QString,int> temp;
                temp["全局"] = swSer.speed;
                for(auto it = swSer.axisMap.begin();it != swSer.axisMap.end();++it)
                {
                    if(vec.count() == swSer.axisNum)
                        break;
                    vec.append(it.key());
                    temp[it.key()] = it.value().speed;
                }
                ConfigData::GetInstance()->m_mutex.unlock();
                if(vec.count() > 0)
                {
                    QString doubleclick = "当前点：";
                    emit UpdateCalibrationUI(vec,&swSer,temp,doubleclick,strtype);
                }
            }
        }

    }
    else
    {
        //if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(strtype))
        {
            QString strId = this->item(curow, 1)->text();
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(strId))
            {
                SWCONFIGXML_LCSP_STRU swSer;
                swSer.id = strId;
                swSer.note = this->item(curow, 2)->text();
                swSer.acc = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].acc;
                swSer.LCSPCPos = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].LCSPCPos;
                swSer.delayMsec = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].delayMsec;
                swSer.speed = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].speed;
                swSer.timeOut = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[strId].timeOut;
                QVector<QString> vec;
                for(auto it = swSer.LCSPCPos.begin()->LCSCPosMap.begin();it != swSer.LCSPCPos.begin()->LCSCPosMap.end();++it)
                {
                    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.contains(it.key()))
                    {
                        vec.clear();
                        break;
                    }
                    vec.append(it.key());
                }
                ConfigData::GetInstance()->m_mutex.unlock();
                if(vec.count() > 0)
                {
                    QMap<QString,int> temp;
                    QString doubleclick = "当前点：";
                    emit UpdateCalibrationUI(vec,&swSer,temp,doubleclick,strtype);
                }
            }
        }
    }
    ConfigData::GetInstance()->m_mutex.unlock();
}

void CustomTabWidget::ResizeClickListener(QTableWidgetItem *item)
{
    int icolumn = item->column();
    if(6 == icolumn)
    {
        int curow = item->row();
        QString strAxisId = this->item(curow, 5)->text();
        QStringList strList = strAxisId.split(" ; ");
        item->setSizeHint(QSize(250,500));
    }
}
