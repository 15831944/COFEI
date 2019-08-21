#include "csptritable.h"
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMessageBox>
#include<QRadioButton>
#include<QGroupBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include<QVector>
#include<QLabel>
#include<QHBoxLayout>
#include<QButtonGroup>
const double _PARAMPULSE = 0.01;

CSPTriTable::CSPTriTable(QWidget *parent,QVector<QString>axisid,int row):QTableWidget(parent)
{
    // m_TriConf=LCSPTriggerInfo;
    m_row = row;
    axisid.insert(axisid.end(),"IOname");
    axisid.insert(axisid.end(),"IOstat");
    m_axisid = axisid;
    this->setRowCount(1);
    this->setColumnCount(axisid.size());
    QStringList str;
    for(int i=0;i<axisid.size();++i)
    {
        str.append(axisid[i]);
    }
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setHorizontalHeaderLabels(str);//表头插入数值
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    for(int i=0;i<axisid.size();++i)//最后一行设置为空
    {
        this->setItem(0,i,new QTableWidgetItem(""));
    }
}

QMenu *CSPTriTable::CreateContextMenu()
{
    menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建"),this);
    connect(add, &QAction::triggered, this, &CSPTriTable::AddPoint);
    QAction *del = new QAction(QObject::tr("删除"),this);
    connect(del, &QAction::triggered, this, &CSPTriTable::DeletePoint);
    QAction *save = new QAction(QObject::tr("保存"),this);
    connect(save, &QAction::triggered, this, &CSPTriTable::SaveTri);
    QAction *close = new QAction(QObject::tr("关闭"),this);
    connect(close, &QAction::triggered, this, &CSPTriTable::CloseTri);
    menu->addAction(add);
    menu->addAction(del);
    menu->addAction(save);
    menu->addAction(close);
    return menu;
}

int CSPTriTable::Calposindex(double pos)
{
    int index=0;
    if(m_trik[m_ID]!=0)
    {
        index=(pos-m_frontpos[m_ID])/m_trik[m_ID];
    }
    return index;
}

void CSPTriTable::contextMenuEvent(QContextMenuEvent *event)
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

void CSPTriTable::AddPoint()
{
    m_list.clear();
    if(m_axisid.size()<3)
        return;
    m_ID="";
    m_adilog=new dialog(m_axisid);//需要修改传入值只需一个坐标位置即可
    connect(m_adilog,&dialog::GetData,this,&CSPTriTable::Dataupdate);//新建的数据存入
    if(m_adilog->exec()== QDialog::Accepted)
    {
        for(int i=0;i<m_list.size();++i)
        {
            if(m_list[i]=="")
            {  QMessageBox::warning(this, QString("新建触发位置"), QString("请确保当前行数据存在!!!"));
                return;
            }
        }
        QString str=m_list[0];
        int index=Calposindex(str.toDouble());//
        if(index <= 0||index > m_maxnum)
        {
            return;
        }
        QMap<QString ,double> newpos;//得到新的位置
        newpos.clear();
        newpos.insert(m_ID,m_list[0].toDouble());
        for(int i=0;i<m_axisid.size()-2;++i)
        {
            if(m_axisid[i]!=m_ID)
            {
                newpos.insert(m_axisid[i],m_frontpos[m_axisid[i]]+index*m_trik[m_axisid[i]]);
            }
        }
        m_list.remove(0);
        for(int i=0;i<m_axisid.size()-2;++i)
        {
            for(auto it=newpos.begin();it!=newpos.end();++it)
            {
                if(m_axisid[i]==it.key())
                {
                    m_list.insert(i,QString::number(newpos[it.key()]));
                    break;
                }
            }
        }
        int row = this->currentRow();
        int trar=-1;
        if(row==(this->rowCount()-1))
        {
            trar = row;
        }
        else
        {
            trar=row+1;
        }
        this->insertRow(row+1); //插入新的值
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
    }
}

void CSPTriTable::DeletePoint()
{
    int curow = this->currentRow();
    if(curow!=this->rowCount()-1)
    {
        this->removeRow(curow);
    }
}

void CSPTriTable::SaveTri()
{
    QVector<LCSP_TRIGGERINFO_STRU> LCSPTriggerInfo;
    //收集触发信息
    for(int i=0;i<this->rowCount()-1;++i)
    {
        LCSP_TRIGGERINFO_STRU tristru;
        QVector<LCSP_TRIGGER_CPOS_STRU>  vec;
        tristru.bitName=this->item(i,this->columnCount()-2)->text();
        tristru.stat=this->item(i,this->columnCount()-1)->text().toInt();
        for(int j=0;j<this->columnCount()-2;++j)
        {
            LCSP_TRIGGER_CPOS_STRU strpos;
            strpos.id=m_axisid[j];
            strpos.pos=this->item(i,j)->text().toDouble();
            vec.append(strpos);
        }
        tristru.LCSPTrigCPos = vec;
        LCSPTriggerInfo.append(tristru);
    }
    emit SaveTriInfo(LCSPTriggerInfo);
}

void CSPTriTable::CloseTri()
{
    emit CloseTw();
}

void CSPTriTable::Dataupdate(QString id, QVector<QString> data)
{
    m_ID = id;
    m_list = data;
}
///
/// \brief CSPTriTable::CalTripos
/// \param map0传入的为XYZ坐标第一个位置点
/// \param map1传入的第二个位置点
/// \param str
///
void CSPTriTable::CalTripos(QMap<QString, LCSP_CPOSINFO_STRU> map0, QMap<QString, LCSP_CPOSINFO_STRU> map1)
{
    Calculformula(map0,map1);
}
bool CSPTriTable::Calculformula(QMap<QString, LCSP_CPOSINFO_STRU> map0, QMap<QString, LCSP_CPOSINFO_STRU> map1)
{
    double GetMaxpos = 0.00;
    GetMaxpos = GetMaxPosnum(map0,map1);
    m_maxnum = GetMaxpos;
    m_frontpos.clear();
    for(auto it=map0.begin();it!=map0.end();++it)
    {
        m_frontpos.insert(it.key(),it.value().pos);
    }
    if(GetMaxpos != -1)
    {
        m_trik.clear();
        for(auto it=map1.begin();it!=map1.end();++it)
        {
            if(!m_trik.contains(it.key()))
            {
                if(GetMaxpos >=1)//如果最大位置点为０的时候
                {
                    m_trik.insert(it.key(),(it.value().pos-map0[it.key()].pos)/GetMaxpos);
                }
            }
        }
        if(m_trik.size()!=0)
        {
            return true;
        }
    }
    return false;
}
int CSPTriTable::GetMaxPosnum(QMap<QString, LCSP_CPOSINFO_STRU> map0, QMap<QString, LCSP_CPOSINFO_STRU> map1)
{
    int GetMaxpos = -1;//得到最大的取值点那个
    if(!map0.isEmpty() && !map1.isEmpty())
    {
        int Posrange = 0;
        for(auto it=map1.begin();it!=map1.end();++it)
        {
            Posrange = qAbs((it.value().pos - map0[it.key()].pos)/_PARAMPULSE);//当量0.01mm
            if(GetMaxpos < Posrange)//比较得到最大的点数
            {
                GetMaxpos = Posrange;
            }
        }
    }
    return GetMaxpos;
}

///
/// \brief dialog::dialog类
/// \param listtext
///
dialog::dialog(QVector<QString> listtext)
{
    m_str=listtext;
    this->setWindowFlags(windowFlags()&~ Qt::WindowMinMaxButtonsHint);
    //setWindowFlags(Qt::ToolTip);
    this->setFixedSize(480,190);
    QDesktopWidget *deskWgt = QApplication::desktop();
    QRect screenRect = deskWgt->screenGeometry();
    int screenWidth = screenRect.width() ;
    int screenHeight = screenRect.height();
    move(screenWidth/3,screenHeight/3);
    setStyleSheet(tr("background-color:rgb(40,42,51);color:white;"));
    createUI();
}

void dialog::createUI()
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayoutradio=new QHBoxLayout;
    QFont ft (QFont("黑体",11));
    int size=m_str.size();
    QLabel *textlabel[size];
    for(int i=0;i<m_str.size();++i)
    {
        textlabel[i]= new QLabel();
        textlabel[i]->setText(m_str[i]);
        textlabel[i]->setFixedSize(90,30);
        textlabel[i]->setAlignment(Qt::AlignHCenter);
    }
    m_idgroupbtn=new QButtonGroup();//radibuttonｂｏｘ
    QRadioButton *radbtn[size-2];

    for(int j=0;j<size-2;++j)
    {
        radbtn[j]=new QRadioButton();
        radbtn[j]->setText(m_str[j]);
        m_idgroupbtn->addButton(radbtn[j]);
        hlayoutradio->addWidget(radbtn[j]);
    }
    hlayoutradio->setSpacing(10);
    hlayoutradio->setContentsMargins(60, 10, 10, 10);
    radbtn[0]->setChecked(true);
    vlayout->addLayout(hlayoutradio);
    connect(m_idgroupbtn, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    QHBoxLayout *h_labellayout=new QHBoxLayout();
    m_showid = new QLabel(this);
    m_showid->setFixedSize(90,30);
    m_showid->setText(m_str[0]);
    h_labellayout->addWidget(m_showid);
    h_labellayout->addWidget(textlabel[m_str.size()-2]);
    h_labellayout->addWidget(textlabel[m_str.size()-1]);
    vlayout->addLayout(h_labellayout);
    QHBoxLayout *h_linlay=new QHBoxLayout();
    for(int i=0;i<3;++i)
    {
        m_line[i]=new QLineEdit(this);
        m_line[i]->setFixedSize(90,30);
        h_linlay->addWidget(m_line[i]);
    }
    vlayout->addLayout(h_linlay);
    QHBoxLayout *hbox = new QHBoxLayout();//确认取消按钮
    m_pok = new QPushButton();
    m_pok->setFont(ft);
    m_pok->setText("确定");
    connect(m_pok,&QPushButton::clicked,this,&dialog::sure);
    hbox->addWidget(m_pok);

    m_pcleal = new QPushButton();
    m_pcleal->setFont(ft);
    m_pcleal->setText("取消");
    connect(m_pcleal,&QPushButton::clicked,this,&QDialog::reject);
    hbox->addWidget(m_pcleal);
    vlayout->addLayout(hbox);

    this->setLayout(vlayout);
    m_checkedid=radbtn[0]->text();
}

void dialog::onButtonClicked(QAbstractButton *)
{
    QList<QAbstractButton*> list = m_idgroupbtn->buttons(); //遍历按钮，获取选中状态
    foreach (QAbstractButton *pButton, list)
    {
        if(pButton->isChecked())
        {
            m_checkedid=pButton->text();
            m_showid->setText(m_checkedid);
            break;
        }
    }
}

void dialog::sure()
{
    QVector<QString> listtext;
    for(int i=0;i<3;++i)
    {
        listtext.append(m_line[i]->text());
    }
    emit GetData(m_checkedid,listtext);
    accept();
}
