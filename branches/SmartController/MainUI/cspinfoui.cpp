#include "cspinfoui.h"
#include <QPushButton>
#include "csptablewidget.h"
#include"csptritable.h"
#include <QTableWidget>
#include <QContextMenuEvent>
#include <QTableWidgetItem>
#include <QDebug>
#include <sys/mman.h>
#include<time.h>
#include <QApplication>
#include <DycLibBase/StackManager/stackmanager.h>
#include<QMessageBox>
const QString Stylestr = "QPushButton{background-color:rgb(112,112,112);color:black;}"
                         "QPushButton:hover{background-color:rgb(60,60,60);}"
                         "QPushButton:pressed{background-color:rgb(40,40,40);}";
CspInfoUI::CspInfoUI(QVector<QString>str,SWCONFIGXML_LCSP_STRU stru,QWidget *parent) : QWidget(parent)
{
    m_updatecsptablestru = stru;//进行操作之后的数据表格按照标准库转换之后的结果(初始的ＸＹＺ坐标系)
    GetCSPDataInfo(str);//存储特定的信息
    GetSumeleratMap(str);
    m_CoSwlib = new Coordinate();
    MyLCSPThread = NULL;//定义空指针
    m_isStop = false;
    this->setFixedSize(parent->width(),parent->height()-70);
    CreatCspUI();
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timerUpInputData()));
    AddPosData(stru,m_hwcsysid);    //增加一个位置
    if(MyLCSPThread==NULL)
    {
        MyLCSPThread = new CSPThread();
        MyLCSPThread->InitCSPParam(stru);
        MyLCSPThread->m_runiresult=0;
    }
    coordEventMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_ProtectEvent_Map.coordEventMap;
    axisEventMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_ProtectEvent_Map.axisEventMap;
    m_norun=true;
    m_orgrun=false;
    m_axisrunid="";
}

CspInfoUI::~CspInfoUI()
{
    m_hwarmid.clear();
    m_pTimer->stop();
    if(MyLCSPThread!=NULL)
    {
        if(MyLCSPThread->isRunning())
        {
            MyLCSPThread->Stop();
            MyLCSPThread->quit();
            MyLCSPThread->wait();
        }
        delete MyLCSPThread;
        MyLCSPThread = NULL;
    }
}

void CspInfoUI::TimerStatus(bool isStop)
{
    if(isStop)
    {
        m_pTimer->stop();
    }
    else
    {
        m_pTimer->start(200);
    }
}
///
/// \brief CspInfoUI::GetLcspAllPos
/// \returnｘｍｌ所有的数据表格
///
QVector<LCSP_CSYSINFO_STRU> CspInfoUI::GetLcspAllPos()
{
    if(m_updatecsptablestru.LCSPCPos.size()==0)
    {
        QVector<LCSP_CSYSINFO_STRU> LCSPCPosvec;
        QMap<QString ,LCSP_CPOSINFO_STRU> LCSCPosMap;
        LCSCPosMap.clear();
        LCSP_CSYSINFO_STRU posstru;
        for(int j=0;j<m_hwcsysid.size();++j)
        {
            LCSP_CPOSINFO_STRU pos;
            pos.pos=0;
            LCSCPosMap.insert(m_hwcsysid[j],pos);
        }
        posstru.LCSCPosMap = LCSCPosMap;
        LCSPCPosvec.append(posstru);
        m_updatecsptablestru.LCSPCPos=LCSPCPosvec;
    }
    return m_updatecsptablestru.LCSPCPos;
}
///
/// \brief CspInfoUI::GetCSPDataInfo
/// \param str用户传过来的字符串
///得到所有需要的参数
void CspInfoUI::GetCSPDataInfo(QVector<QString>str)
{
    m_RecswMap.clear();
    for(int i=0;i<str.size()&&(i<3);++i)
    {
        QVector<QString> jvec;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.contains(str[i]))
        {
            if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[i]].hwArmInfoMap.size()>=1)
            {
                for(auto it=ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[i]].hwArmInfoMap.begin();\
                    it!=ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[i]].hwArmInfoMap.end();++it)
                {
                    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(it.key()))
                    {
                        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[it.key()].modelType=="8")
                        {
                            if(m_hwarmid.size()<=6)
                            {
                                if(!m_hwarmid.contains(it.key()))
                                {
                                    m_hwarmid.append(it.key());
                                }
                                if(!jvec.contains(it.key()))
                                {
                                    jvec.append(it.key());
                                }
                                if(!m_hwcsysid.contains(str[i]))
                                {
                                    m_hwcsysid.append(str[i]);
                                }
                            }
                        }
                    }
                }
                if(m_hwcsysid.contains(str[i]))
                {
                    m_RecswMap.insert(str[i],jvec);
                }
            }
        }
    }
    //原有的触发信息对得到的信息进行存储
    for(int i=0;i<m_updatecsptablestru.LCSPCPos.size();++i)
    {
        RIGGERTABLESTU tri;
        if(m_updatecsptablestru.LCSPCPos[i].LCSPTrigInfo.size()!=0)
        {
            tri.row=i;
            tri.LCSPTriggerInfo = m_updatecsptablestru.LCSPCPos[i].LCSPTrigInfo;
            m_newtri.append(tri);
        }
    }
    //获取原始数据已经存在的数据进行存储到m_TraPosvec
    for(int i=0;i<m_updatecsptablestru.LCSPCPos.size();++i)
    {
        QMap<QString,double> map;
        map.clear();
        for(auto it=m_updatecsptablestru.LCSPCPos[i].LCSCPosMap.begin();it!=m_updatecsptablestru.LCSPCPos[i].LCSCPosMap.end();++it)
        {
            map.insert(it.key(),it.value().pos);
        }
        m_TraPosvec.append(map);
    }
    m_alltrap.clear();
    for(int i=0;i<m_updatecsptablestru.LCSPCPos.size();++i)
    {
        m_alltrap.append(QString("P%1").arg(QString::number(i)));
    }
    foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
        m_tempINSON.append(pair);
    }
    foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
        m_tempOUTSON.append(pair);
        int offValue = 0;
        if(it.value == 0)
        {
            offValue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(it.bit,offValue);
        m_tempOFFSON.append(pairoff);
    }
    int stausBytePos = -1;
    m_statposbyteMap.clear();
    for(int i=0;i<m_hwarmid.size();++i)
    {
        stausBytePos = GetStatuswordPos(m_hwarmid[i]);
        if(!m_statposbyteMap.contains(m_hwarmid[i]))
        {
            m_statposbyteMap.insert(m_hwarmid[i],stausBytePos);
        }
    }
}

void CspInfoUI::CreatCspUI()
{
    //界面的字体和样式
    m_font.setPointSize(13);
    m_font.setFamily("黑体");
    this->setStyleSheet(tr("background-color:rgb(255,255,43);color:rgb(166,167,166);border-radius:3px;"));
    //设置总的布局    //轴的状态显示
    statinfo=new QLineEdit(this);
    statinfo->setReadOnly(true);
    statinfo->setFixedWidth(this->width());
    statinfo->setFixedHeight(28);
    statinfo->setFont(m_font);
    statinfo->setStyleSheet("background-color:rgb(35,35,35)");
    QVBoxLayout *MainLayout=new QVBoxLayout(this);
    MainLayout->addWidget(statinfo);
    QHBoxLayout *layout=new QHBoxLayout();
    QWidget *cspbutton=new QWidget(this);
    cspbutton->setStyleSheet("background-color:rgb(0,255,255,0)");
    cspbutton->setFixedWidth(this->width()/28);
    QVBoxLayout *btnlayout=new QVBoxLayout();//坐标的按钮布局

    QFont btnfont;
    btnfont.setPointSize(12);
    btnfont.setFamily("黑体");
    //模式切换按钮
    menu=new QMenu;
    modswt=new QAction("关节");
    menu->addAction(modswt);
    modswtbtn=new QPushButton(this);
    btnlayout->addWidget(modswtbtn);
    modswtbtn->setText("坐标");
    modswtbtn->setMenu(menu);
    modswtbtn->setFixedSize(this->width()/40,this->height()/13);
    modswtbtn->setFont(btnfont);
    modswtbtn->setStyleSheet("QPushButton{background-color:rgb(25,25,112);color:black;}"
                             "QPushButton:hover{background-color:rgb(60,60,60);}" );
    connect(modswt,&QAction::triggered,this,&CspInfoUI::changemodel);
    //联动按钮
    for(int i = 0; i < 6;++i)
    {
        m_cspbtn[i] = new QPushButton(cspbutton);
        m_cspbtn[i]->setFixedSize(this->width()/40,this->height()/13);
        m_cspbtn[i]->setFont(btnfont);
        m_cspbtn[i]->setStyleSheet(Stylestr);
        btnlayout->addWidget(m_cspbtn[i]);
    }
    m_cspbtn[0]->setText("Stop");
    m_cspbtn[1]->setText("Run");
    m_cspbtn[2]->setText("S_ON");
    m_cspbtn[3]->setText("Reset");
    m_cspbtn[4]->setText("LS");
    m_cspbtn[5]->setText("RunSP");

    cspbutton->setLayout(btnlayout);
    layout->addWidget(cspbutton);
    m_cspxyzwg = new QWidget(this);
    m_cspxyzwg->setStyleSheet(tr("background-color:rgb(61,64,66);color:rgb(166,167,166);border-radius:3px;"));
    QHBoxLayout *CSPHlayout=new QHBoxLayout();
    MainLayout->addLayout(layout);
    QGridLayout *xyz=new QGridLayout;
    QWidget *xyzbuttonlay=new QWidget(m_cspxyzwg);

    xyzbuttonlay->setLayout(xyz);
    xyzbuttonlay->setStyleSheet(tr("background-color:rgb(30,30,30);color:rgb(166,167,166);border-radius:3px;"));
    m_XYZSave=new QPushButton(xyzbuttonlay);
    m_XYZSave->setStyleSheet("QPushButton{background-color:rgb(60,60,150);}""QPushButton:pressed{background-color:rgb(80,80,170);}");
    m_XYZSave->setFixedSize(this->width()/30,this->height()/15);
    m_XYZSave->setText("保存");
    m_XYZSave->setFont(btnfont);
    m_poseditxyz=new QLineEdit(xyzbuttonlay);
    m_poseditxyz->setStyleSheet(("background-color:rgb(40,40,40)"));
    m_poseditxyz->setFixedHeight(32);
    QLabel *labelxyz=new QLabel(xyzbuttonlay);
    labelxyz->setText("目标p:");
    labelxyz->setFont(btnfont);
    for(int i = 0; i < 6;++i)
    {
        m_RecIdbtn[i] = new  QPushButton(xyzbuttonlay);
        m_RecIdbtn[i]->setFixedSize(45,30);
        m_RecIdbtn[i]->setFont(btnfont);
        m_RecIdbtn[i]->setStyleSheet(Stylestr);
    }
    for(int i=5;i>=m_hwcsysid.size()*2;--i)
    {
        m_RecIdbtn[i]->setEnabled(false);
    }
    for(int i=0;i<m_hwcsysid.size();++i)
    {
        if(i<3)
        {
            QString str=m_hwcsysid[i];
            m_RecIdbtn[i*2]->setText(QString("+%1").arg(str.remove(str.size()-2,2)));
            m_RecIdbtn[i*2+1]->setText(QString("-%1").arg(str));
        }
    }
    m_XYZPOSMap.clear();
    for(int i=0;i<3;++i)
    {
        m_RecidLinepos[i]=new QLineEdit(xyzbuttonlay);
        m_RecidLinepos[i]->setReadOnly(true);//设置为只读
        m_RecidLinepos[i]->setFixedHeight(28);
        m_RecidLinepos[i]->setStyleSheet(("background-color:rgb(35,35,35)"));
        if(i<m_hwcsysid.size())
        {
            if(!m_XYZPOSMap.contains(m_hwcsysid[i]))
            {
                m_XYZPOSMap.insert(m_hwcsysid[i],m_RecidLinepos[i]);
            }
        }
        else
        {
            m_RecidLinepos[i]->setEnabled(false);
        }
    }
    xyz->addWidget(m_RecIdbtn[0],0,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecidLinepos[0],0,1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecIdbtn[1],0,2,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecIdbtn[2],1,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecidLinepos[1],1,1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecIdbtn[3],1,2,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecIdbtn[4],2,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecidLinepos[2],2,1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_RecIdbtn[5],2,2,1,1,Qt::AlignLeft|Qt::AlignVCenter);

    xyz->addWidget(labelxyz,3,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_poseditxyz,3,1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    xyz->addWidget(m_XYZSave,3,2,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    CSPHlayout->addWidget(xyzbuttonlay);
    m_table=new CSPTableWidget(m_cspxyzwg,m_hwcsysid,m_TraPosvec);
    CSPHlayout->addWidget(m_table);
    m_table->setStyleSheet("selection-background-color:rgb(14, 14, 14);border-radius:15px;");
    //信号与槽建立
    connect(m_cspbtn[0],&QPushButton::clicked,this,&CspInfoUI::StopCSP);
    connect(m_cspbtn[1],&QPushButton::clicked,this,&CspInfoUI::RunCSP);
    connect(m_cspbtn[2],&QPushButton::clicked,this,&CspInfoUI::S_onCSP);
    connect(m_cspbtn[3],&QPushButton::clicked,this,&CspInfoUI::ResetCSP);
    connect(m_cspbtn[4],&QPushButton::clicked,this,&CspInfoUI::SwitchSpeedCSP);
    connect(m_cspbtn[5],&QPushButton::clicked,this,&CspInfoUI::RunSinPos);
    //xyz联动按钮
    connect(m_RecIdbtn[0],&QPushButton::pressed,this,&CspInfoUI::IncreaseXCSP);
    connect(m_RecIdbtn[0],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_RecIdbtn[1],&QPushButton::pressed,this,&CspInfoUI::ReduceXCSP);
    connect(m_RecIdbtn[1],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_RecIdbtn[2],&QPushButton::pressed,this,&CspInfoUI::IncreaseYCSP);
    connect(m_RecIdbtn[2],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_RecIdbtn[3],&QPushButton::pressed,this,&CspInfoUI::ReduceYCSP);
    connect(m_RecIdbtn[3],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_RecIdbtn[4],&QPushButton::pressed,this,&CspInfoUI::IncreaseZCSP);
    connect(m_RecIdbtn[4],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_RecIdbtn[5],&QPushButton::pressed,this,&CspInfoUI::ReduceZCSP);
    connect(m_RecIdbtn[5],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    connect(m_XYZSave,&QPushButton::clicked,this,&CspInfoUI::SaveP);
    //目标位置表格按钮
    connect(m_table,&CSPTableWidget::TriInfo,this,&CspInfoUI::ShowTriTable);
    connect(m_table,&CSPTableWidget::ClearTri,this,&CspInfoUI::ClearTriInfo);
    connect(m_table,&CSPTableWidget::updatedata,this,&CspInfoUI::Acceptupdatedata);
    //触发编辑表格
    m_Tritable=new CSPTriTable(m_cspxyzwg,m_hwcsysid,0);
    m_Tritable->setStyleSheet("selection-background-color:rgb(14, 14, 14);border-radius:15px;");
    CSPHlayout->addWidget(m_Tritable);

    m_cspxyzwg->setLayout(CSPHlayout);

    connect(m_Tritable,&CSPTriTable::CloseTw,this,&CspInfoUI::CloseTriTable);
    connect(m_Tritable,&CSPTriTable::SaveTriInfo,this,&CspInfoUI::SaveTri);
    layout->addWidget(m_cspxyzwg);
    m_Tritable->close();

    //对关节创建对于每一组的信号与槽函数都要更新到每一个表格中
    m_cspjwg = new QWidget(this);
    m_cspjwg->setStyleSheet(tr("background-color:rgb(61,64,66);color:rgb(166,167,166);border-radius:3px;"));
    QHBoxLayout *Jlayhwg=new QHBoxLayout;
    QWidget *deb=new QWidget(m_cspjwg);
    deb->setStyleSheet(tr("background-color:rgb(30,30,30);color:rgb(166,167,166);border-radius:3px;"));
    QGridLayout *jdebtlay=new QGridLayout;
    deb->setLayout(jdebtlay);
    m_JntInfoMap.clear();//便于访问
    for(int i=0;i<6;++i)
    {
        m_JPosInfo[i]=new QLineEdit(this);
        m_JPosInfo[i]->setReadOnly(true);
        m_JPosInfo[i]->setFixedHeight(28);
        if(i < m_hwarmid.size())
        {
            m_JPosInfo[i]->setStyleSheet(("background-color:rgb(35,35,35)"));
            m_JntInfoMap.insert(m_hwarmid[i],m_JPosInfo[i]);//

            //****************create ORG button***** create clicked connect************
            QPushButton *btn = new QPushButton(deb);
            btn->setFixedSize(75,30);
            btn->setFont(btnfont);
            btn->setText(QString(m_hwarmid[i]+":ORG"));
            connect(btn,&PushButton::clicked,this,&CspInfoUI::RunORG);
            btn->setStyleSheet(Stylestr);
            m_ORGBtnMap.insert(m_hwarmid[i],btn);
        }
        else
        {
            m_JPosInfo[i]->setStyleSheet(("background-color:rgb(35,35,35,50)"));
        }
    }
    for(int i=0;i<12;++i)
    {
        m_JntBtn[i] = new QPushButton(deb);
        m_JntBtn[i]->setFixedSize(45,30);
        m_JntBtn[i]->setFont(btnfont);
        if(i< m_hwarmid.size()*2)
        {
            m_JntBtn[i]->setStyleSheet(Stylestr);
        }
        else
        {
            m_JntBtn[i]->setStyleSheet("QPushButton{background-color:rgb(112,112,112,50);color:black;}");
            m_JntBtn[i]->setEnabled(false);
        }
    }
    m_Jsave=new QPushButton(deb);
    m_Jsave->setStyleSheet("QPushButton{background-color:rgb(60,60,150);}""QPushButton:pressed{background-color:rgb(80,80,170);}");
    m_Jsave->setFixedSize(96,32);
    m_Jsave->setText("保存");
    m_Jsave->setFont(btnfont);
    m_poseditj=new QLineEdit(deb);
    m_poseditj->setStyleSheet(("background-color:rgb(40,40,40)"));
    m_poseditj->setFixedHeight(32);
    QLabel *label=new QLabel(deb);
    label->setText("目标p:");
    label->setFont(btnfont);
    for(int i=0;i<6;++i)
    {
        for(int j=0;j<2;++j)
        {
            if(j==1)
            {
                jdebtlay->addWidget(m_JPosInfo[i],i,j,1,1,Qt::AlignLeft|Qt::AlignVCenter);
                jdebtlay->addWidget(m_JntBtn[i*2+j],i,j+1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
                if(i<m_hwarmid.size())
                {
                    m_JntBtn[i*2+j]->setText(QString("-"+m_hwarmid[i]));
                    //**************add ORG BTN int layout**************
                    jdebtlay->addWidget(m_ORGBtnMap[m_hwarmid[i]],i,j+2,1,1,Qt::AlignLeft|Qt::AlignVCenter);
                }
                continue;
            }
            jdebtlay->addWidget(m_JntBtn[i*2+j],i,j,1,1,Qt::AlignLeft|Qt::AlignVCenter);
            if(i<m_hwarmid.size())
            {
                m_JntBtn[i*2+j]->setText(QString("+"+m_hwarmid[i]));
            }
        }
    }
    m_PulSw=new QCheckBox("脉冲",deb);//脉冲和关节切换
    jdebtlay->addWidget(label,6,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    jdebtlay->addWidget(m_poseditj,6,1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    jdebtlay->addWidget(m_Jsave,6,2,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    jdebtlay->addWidget(m_PulSw,7,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    Jlayhwg->addWidget(deb);
    //关节模式的信号槽J1－J6
    connect(m_JntBtn[0],&QPushButton::pressed,this,&CspInfoUI::J1IncrRun,Qt::QueuedConnection);
    connect(m_JntBtn[1],&QPushButton::pressed,this,&CspInfoUI::J1RedRun);
    connect(m_JntBtn[2],&QPushButton::pressed,this,&CspInfoUI::J2IncrRun);
    connect(m_JntBtn[3],&QPushButton::pressed,this,&CspInfoUI::J2RedRun);
    connect(m_JntBtn[4],&QPushButton::pressed,this,&CspInfoUI::J3IncrRun);
    connect(m_JntBtn[5],&QPushButton::pressed,this,&CspInfoUI::J3RedRun);
    connect(m_JntBtn[6],&QPushButton::pressed,this,&CspInfoUI::J4IncrRun);
    connect(m_JntBtn[7],&QPushButton::pressed,this,&CspInfoUI::J4RedRun);
    connect(m_JntBtn[8],&QPushButton::pressed,this,&CspInfoUI::J5IncrRun);
    connect(m_JntBtn[9],&QPushButton::pressed,this,&CspInfoUI::J5RedRun);
    connect(m_JntBtn[10],&QPushButton::pressed,this,&CspInfoUI::J6IncrRun);
    connect(m_JntBtn[11],&QPushButton::pressed,this,&CspInfoUI::J6RedRun);
    connect(m_Jsave,&QPushButton::clicked,this,&CspInfoUI::SaveP);
    for(int i=0;i<12;++i)
    {
        connect(m_JntBtn[i],&QPushButton::released,this,&CspInfoUI::XYZCSPStop);
    }
    //m_TraPosvec转换之后的位置信息，转换之后的结构体
    m_detable=new CSPTableWidget(m_cspjwg,m_hwarmid,m_TraPosvec);
    m_detable->setStyleSheet("selection-background-color:rgb(14, 14, 14);border-radius:15px;");
    connect(m_detable,&CSPTableWidget::TriInfo,this,&CspInfoUI::ShowTriTable);
    connect(m_detable,&CSPTableWidget::ClearTri,this,&CspInfoUI::ClearTriInfo);
    connect(m_detable,&CSPTableWidget::updatedata,this,&CspInfoUI::Acceptupdatedata);
    Jlayhwg->addWidget(m_detable);
    m_deTritable=new CSPTriTable(m_cspjwg,m_hwarmid,0);
    m_deTritable->setStyleSheet("selection-background-color:rgb(14, 14, 14);border-radius:15px;");
    Jlayhwg->addWidget(m_deTritable);
    m_cspjwg->setLayout(Jlayhwg);
    layout->addWidget(m_cspjwg);
    m_deTritable->close();
    connect(m_deTritable,&CSPTriTable::CloseTw,this,&CspInfoUI::CloseTriTable);
    connect(m_deTritable,&CSPTriTable::SaveTriInfo,this,&CspInfoUI::SaveTri);
    m_cspjwg->close();
    connect(this,&CspInfoUI::Getlinekpos,m_deTritable,&CSPTriTable::CalTripos);
    connect(this,&CspInfoUI::Getlinekpos,m_Tritable,&CSPTriTable::CalTripos);
}

void CspInfoUI::GetLCSPCurpos(QString strKey, double &curPos)
{
    {
        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
        QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
        auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
        for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
        {
            int ibyteNum = it->byteNum;
            QString strType = it->type;
            if(it->key == "Curpos")
            {
                curPos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);
                return;
            }
            ibytePos += ibyteNum;
        }
    }
}

int CspInfoUI::GetStatuswordPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        if(it->key == "Statusworld")
        {
            break;
        }
        ibytePos += ibyteNum;
    }
    return ibytePos;
}

int CspInfoUI::GetErrorcode(QString strKey)
{
    int ireturn = 0;
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;
        if(it->key == "Errorcode")
        {
            ireturn = BaseAxisOperate::GetAxisErrorCode(strType,ibyteNum,ibytePos);
            break;
        }
        ibytePos += ibyteNum;
    }
    return ireturn;
}

void CspInfoUI::timerUpInputData()
{      
    if ( CheckSON())
    {
        m_cspbtn[2]->setText("S_OFF");
    }
    else
    {
        m_cspbtn[2]->setText("S_ON");
    }
    HandleRunORGiresult();
    HandleRuniresult();
    ShowCurposInfo();
    for (int it=0;it<m_hwarmid.size();++it)
    {
        int Inbytepos = GetStatuswordPos(m_hwarmid[it]);
        if(BaseAxisOperate::CheckAxisFault(3,2,Inbytepos))  //error检查
        {
            if (MyLCSPThread != NULL &&MyLCSPThread->isRunning())
            {
                MyLCSPThread->Stop();
                MyLCSPThread->quit();
                MyLCSPThread->wait();
                SetenbleBtn();
            }
            int ercode = GetErrorcode(m_hwarmid[it]);
            statinfo->setText(QString("axis %1 have error Errorcode:%2!").arg(m_hwarmid[it]).arg(QString::number(ercode,16)));//获取故障代码
        }
    }
    if(m_axisrunid != "")
    {
        QString str = m_axisrunid;
        QStringList  strkeylist = str.split(":");
        str = strkeylist[0];
        str.remove(0,1);
        QVector<QString> idvec;
        if(m_axisrunid.contains("关节"))
        {
            idvec.append(str);
        }
        else if(m_axisrunid.contains("坐标"))
        {
            if(m_RecswMap.contains(QString("%1坐标").arg(str)))
            {
                idvec = m_RecswMap[QString("%1坐标").arg(str)];
            }
        }
        else if(m_axisrunid.contains("Run"))
        {
            idvec = m_hwarmid;
        }
        for (int it = 0;it < idvec.size();++it)
        {
            QString strmsg = "";
            if(!HandleHWLimit(m_axisrunid,idvec[it],strmsg))
            {
                MyLCSPThread->Stop();
                MyLCSPThread->quit();
                MyLCSPThread->wait();
                SetenbleBtn();
                m_norun = true;
                statinfo->setText(QString("axis %1 run fail%2!").arg(idvec[it]).arg(strmsg));
            }
        }
    }
    if(m_isStop)
    {
        m_isStop = false;
        statinfo->setText("Csprun is stopped!");
    }
    QApplication::processEvents();
}

bool CspInfoUI::Checkconfig(SWCONFIGXML_LCSP_STRU congfig)//传入的为关节坐标形式
{
    //检查参数配置是否有误的问题
    if(congfig.LCSPCPos.size()<1)
        return false;
    //软件xml判断１，X/Y轴的J1 J2数量及ＩＤ必须一致
    for(int i=0;i<congfig.LCSPCPos.size()-1;++i)
    {
        if(congfig.LCSPCPos[i].LCSCPosMap.size()!=congfig.LCSPCPos[i+1].LCSCPosMap.size())
        {
            return false;
        }
        int num=congfig.LCSPCPos[i].LCSCPosMap.size();
        if(num > 6|| num < 1)//不能超过６个轴
        {
            return false;
        }
        for(auto it=congfig.LCSPCPos[i].LCSCPosMap.begin();it!=congfig.LCSPCPos[i].LCSCPosMap.end();++it)
        {
            if(!congfig.LCSPCPos[i+1].LCSCPosMap.contains(it.key()))
            {
                return false;
            }
        }
    }
    return true;
}

void CspInfoUI::SetenbleBtn()
{
    for(int i(1);i < 6;++i)
    {
        if(!m_cspbtn[i]->isEnabled())
        { m_cspbtn[i]->setEnabled(true);}
    }
    for(int i=0;i<m_hwcsysid.size()*2;++i)
    {
        if(!m_RecIdbtn[i]->isEnabled())
        {
            m_RecIdbtn[i]->setEnabled(true);
        }
    }
    m_XYZSave->setEnabled(true);
    for(int i=0;i<m_hwarmid.size()*2;++i)
    {
        if(!m_JntBtn[i]->isEnabled())
        { m_JntBtn[i]->setEnabled(true);}
    }
    m_Jsave->setEnabled(true);
    m_PulSw->setEnabled(true);
    for(auto it=m_ORGBtnMap.begin();it!=m_ORGBtnMap.end();++it)
    {
        it.value()->setEnabled(true);
    }
}

void CspInfoUI::StopCSP()
{
    m_axisrunid = "";
    m_orgrun = false;
    if(MyLCSPThread!=NULL)
    {
        MyLCSPThread->Stop();
        MyLCSPThread->quit();
        MyLCSPThread->wait();
    }
    for(auto it = m_ORGThreadMap.begin();it != m_ORGThreadMap.end();++it)
    {
        it.value()->Stop();
        it.value()->quit();
        it.value()->wait();
        m_ORGBtnMap[it.key()]->setEnabled(true);
    }
    m_isStop = true;
    SetenbleBtn();
}

void CspInfoUI::RunCSP()
{
    for(int i=0;i< m_hwcsysid.size();++i)
    {
        if(!JudgeCoordEvent(m_hwcsysid[i]))
        {
            return;
        }
    }
    m_axisrunid = "Run";
    static double ispeed = m_updatecsptablestru.speed;
    double K = ispeed * 0.1*0.5;
    if(m_cspbtn[4]->text()=="MS")
    {
        K = ispeed*0.5*0.5;
    }
    if(m_cspbtn[4]->text()=="HS")
    {
        K = ispeed*1*0.5;
    }
    if(K < 1)
    {
        K = 1;
    }
    SWCONFIGXML_LCSP_STRU ps;
    ps.speed = K;
    ps.acc = m_updatecsptablestru.acc;
    ps.LCSPCPos = m_updatecsptablestru.LCSPCPos;
    ps.id = m_updatecsptablestru.id;
    m_CoSwlib->CovertTo_J(ps);
    if(!Checkconfig(ps))
    {
        statinfo->setText("直线插补参数配置错误(轴的数量及位置确认或者轴ＩＤ确认...请检查");
    }
    else
    {
        if(!CheckSON())
        {
            statinfo->setText("请设置励磁....");
            return;
        }
        m_isStop = false;
        MyLCSPThread->m_runiresult=0;
        if(!MyLCSPThread->isRunning())
        {
            m_norun = false;
            MyLCSPThread->SetParam(ps);
            MyLCSPThread->start();
            SetDisabledBtn("",-1);
            statinfo->setText("插补运行中...");
        }
    }
}

void CspInfoUI::S_onCSP()
{
    if (m_hwarmid.size() >= 1)
    { if(m_cspbtn[2]->text()=="S_ON")//设置励磁
        {
            if(MyLCSPThread->m_csplib->SONAxis()==1)
            {
                m_cspbtn[2]->setText("S_OFF");
                statinfo->setText("");
            }
        }
        else
        {
            MyLCSPThread->m_csplib->SOFFAxis();//解除励磁
            m_cspbtn[2]->setText("S_ON");
        }
    }
}

void CspInfoUI::ResetCSP()
{
    for(int i=0;i<m_hwarmid.size();++i)//复位
    {
        int contrlBytePos = AnalysisHwConfig::GetContrlWorldPos(m_hwarmid[i],m_contrlBitInfoMap,m_byteInfoMap);
        if(contrlBytePos == -1) return;
        if(m_contrlBitInfoMap.contains("Reset"))
        {
            CoreGlobal::BaseAxisOperate::SetAxisReset(m_contrlBitInfoMap["Reset"].bit,contrlBytePos);
            statinfo->setText("");
        }
    }
}

void CspInfoUI::IncreaseXCSP()
{
    RunXYZIncandRed(0);
}

void CspInfoUI::ReduceXCSP()
{
    RunXYZIncandRed(1);
}

void CspInfoUI::IncreaseYCSP()
{
    RunXYZIncandRed(2);
}

void CspInfoUI::ReduceYCSP()
{
    RunXYZIncandRed(3);
}

void CspInfoUI::IncreaseZCSP()
{    
    RunXYZIncandRed(4);
}
void CspInfoUI::ReduceZCSP()
{
    RunXYZIncandRed(5);
}

void CspInfoUI::XYZCSPStop()
{   
    QThread::msleep(20);
    MyLCSPThread->Stop();
    MyLCSPThread->quit();
    MyLCSPThread->wait();
    SetenbleBtn();
    m_axisrunid="";

}

void CspInfoUI::J1IncrRun()
{
    RunArmIncandRed(0);
}

void CspInfoUI::J1RedRun()
{
    RunArmIncandRed(1);
}

void CspInfoUI::J2IncrRun()
{
    RunArmIncandRed(2);
}

void CspInfoUI::J2RedRun()
{
    RunArmIncandRed(3);
}

void CspInfoUI::J3IncrRun()
{
    RunArmIncandRed(4);
}

void CspInfoUI::J3RedRun()
{
    RunArmIncandRed(5);
}

void CspInfoUI::J4IncrRun()
{
    RunArmIncandRed(6);
}

void CspInfoUI::J4RedRun()
{
    RunArmIncandRed(7);
}

void CspInfoUI::J5IncrRun()
{
    RunArmIncandRed(8);
}

void CspInfoUI::J5RedRun()
{
    RunArmIncandRed(9);
}

void CspInfoUI::J6IncrRun()
{
    RunArmIncandRed(10);
}

void CspInfoUI::J6RedRun()
{
    RunArmIncandRed(11);
}

QVector<QMap<QString, double>> CspInfoUI::GetXYZcurpos(QVector<QMap<QString, double>> pulsepos)
{
    SWCONFIGXML_LCSP_STRU cspstru;
    QVector<LCSP_CSYSINFO_STRU> csppos;
    LCSP_CSYSINFO_STRU cyspos;
    for(int i=0;i<pulsepos.size();++i)
    {
        QMap<QString ,LCSP_CPOSINFO_STRU> posmap;
        for(auto it=pulsepos[i].begin();it!=pulsepos[i].end();++it)
        {
            LCSP_CPOSINFO_STRU pos;
            pos.pos=it.value();
            if(!posmap.contains(it.key()))
            {
                posmap.insert(it.key(),pos);
            }
        }
        cyspos.LCSCPosMap=posmap;
        csppos.append(cyspos);
    }
    cspstru.LCSPCPos=csppos;
    m_CoSwlib->CovertTo_X(cspstru,m_RecswMap);
    QVector<QMap<QString, double>> swxyposmapvec;
    swxyposmapvec.clear();
    for(int i=0;i<cspstru.LCSPCPos.size();++i)
    {
        QMap<QString, double> swxyposmap;
        swxyposmap.clear();
        for(auto item =cspstru.LCSPCPos[i].LCSCPosMap.begin();item!=cspstru.LCSPCPos[i].LCSCPosMap.end();++item)
        {
            if(!swxyposmap.contains(item.key()))
            {
                swxyposmap.insert(item.key(),item.value().pos);
            }
        }
        swxyposmapvec.append(swxyposmap);
    }
    return swxyposmapvec;
}

QMap<QString, double> CspInfoUI::GetXYZSinglecurpos(QMap<QString, double> pulsepos)
{
    SWCONFIGXML_LCSP_STRU cspstru;
    QVector<LCSP_CSYSINFO_STRU> csppos;
    LCSP_CSYSINFO_STRU cyspos;
    QMap<QString ,LCSP_CPOSINFO_STRU> posmap;
    for(auto it=pulsepos.begin();it!=pulsepos.end();++it)
    {
        LCSP_CPOSINFO_STRU pos;
        pos.pos=it.value();
        if(!posmap.contains(it.key()))
        {
            posmap.insert(it.key(),pos);
        }
    }
    cyspos.LCSCPosMap=posmap;
    csppos.append(cyspos);
    cspstru.LCSPCPos=csppos;
    m_CoSwlib->CovertTo_X(cspstru,m_RecswMap);
    QMap<QString, double> swxyposmap;
    swxyposmap.clear();
    for(int i=0;i<cspstru.LCSPCPos.size();++i)
    {
        for(auto item =cspstru.LCSPCPos[i].LCSCPosMap.begin();item!=cspstru.LCSPCPos[i].LCSCPosMap.end();++item)
        {
            if(!swxyposmap.contains(item.key()))
            {
                swxyposmap.insert(item.key(),item.value().pos);
            }
        }
    }
    return swxyposmap;
}

QVector<QMap<QString, double> > CspInfoUI::GetJpos(QVector<QMap<QString, double> > XYZpos)
{
    SWCONFIGXML_LCSP_STRU cspstru;
    QVector<LCSP_CSYSINFO_STRU> csppos;
    LCSP_CSYSINFO_STRU cyspos;
    for(int i=0;i<XYZpos.size();++i)
    {
        QMap<QString ,LCSP_CPOSINFO_STRU> posmap;
        for(auto it=XYZpos[i].begin();it!=XYZpos[i].end();++it)
        {
            LCSP_CPOSINFO_STRU pos;
            pos.pos=it.value();
            if(!posmap.contains(it.key()))
            {
                posmap.insert(it.key(),pos);
            }
        }
        cyspos.LCSCPosMap=posmap;
        csppos.append(cyspos);
    }
    cspstru.LCSPCPos=csppos;
    m_CoSwlib->CovertTo_J(cspstru);
    QVector<QMap<QString, double>> swxyposmapvec;
    swxyposmapvec.clear();
    for(int i=0;i<cspstru.LCSPCPos.size();++i)
    {
        QMap<QString, double> swxyposmap;
        swxyposmap.clear();
        for(auto item =cspstru.LCSPCPos[i].LCSCPosMap.begin();item!=cspstru.LCSPCPos[i].LCSCPosMap.end();++item)
        {
            if(!swxyposmap.contains(item.key()))
            {
                swxyposmap.insert(item.key(),item.value().pos/m_eleratMap[item.key()]);
            }
        }
        swxyposmapvec.append(swxyposmap);
    }
    return swxyposmapvec;
}

void CspInfoUI::GetSumeleratMap( QVector<QString>str)
{
    m_eleratMap.clear();
    for(int it=0;it<str.size();++it)
    {
        if(ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.contains(str[it]))
        {
            if(ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[it]].hwArmInfoMap.size()!=0)
            {
                for(auto item=ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[it]].hwArmInfoMap.begin();\
                    item!=ConfigData::GetInstance()->GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[str[it]].hwArmInfoMap.end();++item )
                {
                    if(!m_eleratMap.contains(item.key()))
                    {
                        m_eleratMap.insert(item.key(),item.value().elegearat);
                    }
                }
            }
        }
    }
}

bool CspInfoUI::HandleHWLimit(QString btntext, QString strKey, QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    QString str;
    if(btntext.contains("Run")||btntext.contains("-"))
    {
        if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
            return false;

        str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
        if(!str.isEmpty())
        {
            int itype = BaseCalcFun::AnalysisIOString(str,uc);
            if(itype != 1)
            {
                strMsg = "硬极限起点设置错误  ";

                return false;
            }
            if(uc != 0)
            {
                strMsg = "超出硬极限起点  ";
                QString str;
                for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
                {
                    str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
                }
                strMsg += str;
                return false;
            }
        }
    }
    if(btntext.contains("Run")||btntext.contains("+"))
    {
        str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
        if(!str.isEmpty())
        {
            int itype = BaseCalcFun::AnalysisIOString(str,uc);
            if(itype != 1)
            {
                strMsg = "硬极限终点设置错误  ";
                return false;
            }
            if( uc != 0)
            {
                strMsg = "超出硬极限终点  ";
                QString str;
                for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
                {
                    str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
                }
                strMsg += str;
                return false;
            }
        }
    }
    return true;

}
///
/// \brief CspInfoUI::HandleRunORGiresult
///
void CspInfoUI::HandleRunORGiresult()
{
    if(m_orgrun)//代表回原点状态
    {
        static QString str="";
        bool nenble = true;
        for(auto it = m_ORGThreadMap.begin();it != m_ORGThreadMap.end();++it)
        {
            if(it.value()->m_result != 1)
            {
                if(!m_ORGBtnMap[it.key()]->isEnabled())
                {
                    if(it.value()->m_result != 0)
                    {
                        str+=(QString("%1回原点失败%2 ").arg(it.key()).arg(it.value()->m_result));
                    }
                    else
                    {
                        str+=(QString("%1回原点成功 ").arg(it.key()));
                    }
                    if(str!="")
                    {
                        statinfo->setText(str);
                    }
                }
                m_ORGBtnMap[it.key()]->setEnabled(true);
            }
            if(m_isStop)
            {
                statinfo->setText("ORG is stopped ");
            }
            if(!m_ORGBtnMap[it.key()]->isEnabled())
            {
                nenble = false;
            }
        }
        if(nenble)
        {
            m_orgrun = false;
            str = "";
            SetenbleBtn();
        }
    }
}

void CspInfoUI::HandleRuniresult()
{
    if(!m_norun && MyLCSPThread != NULL)//点击ｒｕｎ或者singleposrun运行状态
    {
        if(MyLCSPThread->m_runiresult != 0)
        {
            if(MyLCSPThread->m_runiresult!=1)
            {
                statinfo->setText(QString("axis run fail"));
            }
            else
            {
                statinfo->setText(QString("axis run 成功！"));
            }
            MyLCSPThread->m_runiresult = 0;
            MyLCSPThread->Stop();
            MyLCSPThread->quit();
            MyLCSPThread->wait();
            SetenbleBtn();
            m_norun=true;
            m_axisrunid = "";
        }
    }
}

void CspInfoUI::ShowCurposInfo()
{
    QMap<QString,int> m_armpulsepos;    //各个位置显示信息
    m_armpulsepos.clear();
    QMap<QString,double> jposmap;
    for(int i=0;i<m_hwarmid.size();++i)
    {
        double curpos=0;
        GetLCSPCurpos(m_hwarmid[i],curpos);//得到的为脉冲值
        jposmap.insert(m_hwarmid[i],curpos);
        if(m_PulSw->isChecked())
        {
            m_JntInfoMap[m_hwarmid[i]]->setText(QString::number(curpos));
        }
        else
        {
            m_JntInfoMap[m_hwarmid[i]]->setText(QString::number(curpos/m_eleratMap[m_hwarmid[i]],10,3));
        }
    }
    QMap<QString,double> xyzmap =  GetXYZSinglecurpos(jposmap);
    for(auto it=xyzmap.begin();it!=xyzmap.end();++it)
    {
        if(m_XYZPOSMap.contains(it.key()))
        {
            m_XYZPOSMap[it.key()]->setText(QString::number(it.value(),10,3));
        }
    }
}

void CspInfoUI::SetDisabledBtn(QString str,int inde)
{
    for (int j = 0;j < m_hwcsysid.size()*2;++j)
    {
        if(str == "坐标")
        {
            if(j != inde)
            {
                if(m_RecIdbtn[j]->isEnabled())
                {
                    m_RecIdbtn[j]->setEnabled(false);
                }
            }
        }
        else
        {
            if(m_RecIdbtn[j]->isEnabled())
            {
                m_RecIdbtn[j]->setEnabled(false);
            }
        }
    }
    for (int i = 0;i < m_hwarmid.size()*2;++i)
    {
        if(str == "关节")
        {
            if(i != inde)
            {
                if(m_JntBtn[i]->isEnabled())
                {
                    m_JntBtn[i]->setEnabled(false);
                }
            }
        }
        else
        {
            if(m_JntBtn[i]->isEnabled())
            {
                m_JntBtn[i]->setEnabled(false);
            }
        }
    }
    for (int i = 1;i < 6;++i) //按钮保护
    {
        if(m_cspbtn[i] != NULL)
        {
            if(m_cspbtn[i]->isEnabled())
            {
                m_cspbtn[i]->setEnabled(false);
            }
        }
    }
    m_XYZSave->setEnabled(false);
    m_Jsave->setEnabled(false);
    m_PulSw->setEnabled(false);
    for (auto it=m_ORGBtnMap.begin();it!=m_ORGBtnMap.end();++it)
    {
        it.value()->setEnabled(false);
    }
}

void CspInfoUI::SwitchSpeedCSP()
{
    if(m_cspbtn[4]->text()=="LS")
    {
        m_cspbtn[4]->setText("HS");
        return;
    }
    if(m_cspbtn[4]->text()=="HS")
    {
        m_cspbtn[4]->setText("MS");
        return;
    }
    if(m_cspbtn[4]->text()=="MS")
    {
        m_cspbtn[4]->setText("LS");
        return;
    }
}

void CspInfoUI::RunSinPos()
{
    for(int i=0 ;i < m_hwcsysid.size();++i)
    {
        if(!JudgeCoordEvent(m_hwcsysid[i]))
        {
            return;
        }
    }
    m_axisrunid = "Run";
    int row = -1;
    for (int i = 0;i < m_alltrap.size();++i)
    {
        if(m_alltrap[i] == m_poseditxyz->text())
        {
            row = i;
            break;
        }
        else if(m_alltrap[i] == m_poseditj->text())
        {  row = i;
            break;
        }
    }
    if(row == -1)
    {
        return;
    }
    SWCONFIGXML_LCSP_STRU sinposxyzstru;
    static  double ispeed = m_updatecsptablestru.speed;
    double  speed = ispeed*0.1*0.5;
    if(m_cspbtn[4]->text()=="HS")
    {
        speed = m_updatecsptablestru.speed*1*0.5;
    }
    else if(m_cspbtn[4]->text()=="MS")
    {
        speed = m_updatecsptablestru.speed*0.25*0.5;
    }
    sinposxyzstru.speed = speed;
    sinposxyzstru.acc = m_updatecsptablestru.acc;
    QVector<LCSP_CSYSINFO_STRU> sinCPos;
    LCSP_CSYSINFO_STRU cysysstru;
    cysysstru.LCSCPosMap = m_updatecsptablestru.LCSPCPos[row].LCSCPosMap;
    sinCPos.append(cysysstru);
    sinposxyzstru.LCSPCPos = sinCPos;
    sinposxyzstru.id = m_updatecsptablestru.id;
    //判断数组是否有误
    m_CoSwlib->CovertTo_J(sinposxyzstru);
    SWCONFIGXML_LCSP_STRU ps = sinposxyzstru;
    QVector<QMap<QString ,QVector<int>>> sinposarry = MyLCSPThread->m_csplib->GetLCSPArray(ps);
    if(!sinposarry.isEmpty())
    {
        if(!MyLCSPThread->isRunning())
        {
            if(!CheckSON())
            {
                statinfo->setText("请设置励磁....");
                return;
            }
            m_norun = false;
            MyLCSPThread->SetParam(ps);
            MyLCSPThread->m_runiresult=0;
            MyLCSPThread->start();
            statinfo->setText("单位置运行中...");
            SetDisabledBtn("",-1);
        }
    }//库调用
    else
    {
        statinfo->setText("插补数组没有值，请检查设置的目标位置");
    }
}
void CspInfoUI::ClearTriInfo(int curow)
{
    for(int i=0;i<m_hwcsysid.size()+3;++i)//最后一行设置为空
    {
        m_Tritable->setItem(0,i,new QTableWidgetItem(""));
    }
    if(m_updatecsptablestru.LCSPCPos.size()!=0)
    {
        if( m_updatecsptablestru.LCSPCPos[curow].LCSPTrigInfo.size()!=0)
        {
            m_updatecsptablestru.LCSPCPos[curow].LCSPTrigInfo.clear();
        }//数据移除清空
    }
    m_Tritable->setRowCount(1);
    m_deTritable->setRowCount(1);
}

void CspInfoUI::CloseTriTable()
{
    m_deTritable->close();
    m_Tritable->close();
}

void CspInfoUI::ShowTriTable(int currow)
{
    m_row = currow;
    //emit
    if(modswtbtn->text()=="坐标")
    {
        /************************************/
        if(m_row==0)
        {
            QMap<QString,LCSP_CPOSINFO_STRU> posMap;
            for(int i=0;i<m_hwcsysid.size();++i)
            {
                LCSP_CPOSINFO_STRU pos;
                pos.pos=m_XYZPOSMap[m_hwcsysid[i]]->text().toDouble();
                posMap.insert(m_hwcsysid[i],pos);
            }
            emit Getlinekpos(posMap,m_updatecsptablestru.LCSPCPos[currow].LCSCPosMap);
        }
        else{
            emit Getlinekpos(m_updatecsptablestru.LCSPCPos[currow-1].LCSCPosMap,m_updatecsptablestru.LCSPCPos[currow].LCSCPosMap);
        }
        for(int i=0;i<m_Tritable->rowCount();++i)
        {
            m_Tritable->removeRow(i);
        }
        m_Tritable->setRowCount(1);
        for(int j=0;j<m_Tritable->columnCount();++j)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFont(QFont("Helvetica"));
            m_Tritable->setItem(0,j,item);
        }
        //更新数据
        for(int i=0;i< m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo.size();++i)//触发信息显示
        {
            m_Tritable->setRowCount(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo.size());
            for(int j=0;j<m_Tritable->columnCount();++j)
            {
                QString str="";
                if(j==(m_Tritable->columnCount()-1))
                {
                    str= QString::number(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo[i].stat);
                }
                else if(j==(m_Tritable->columnCount()-2))
                {
                    str= m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo[i].bitName;
                }
                else{
                    if(j<m_hwcsysid.size())
                    {
                        int trinde=-1;
                        for(int inde=0;inde<m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo[i].LCSPTrigCPos.size();++inde)
                        {
                            if( m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo[i].LCSPTrigCPos[inde].id==m_hwcsysid[j])//
                            {
                                trinde = inde;
                                break;
                            }
                        }
                        if(trinde >= 0)
                        {
                            str = QString::number(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo[i].LCSPTrigCPos[trinde].pos,10,3);
                        }
                    }
                }
                QTableWidgetItem *item = new QTableWidgetItem(str);
                item->setFont(QFont("Helvetica"));
                m_Tritable->setItem(i,j,item);
            }
        }
        if(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo.size()!=0)
        {
            m_Tritable->insertRow(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo.size());
            for(int j=0;j<m_Tritable->columnCount();++j)
            {
                QTableWidgetItem *item = new QTableWidgetItem("");
                item->setFont(QFont("Helvetica"));
                m_Tritable->setItem(m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo.size(),j,item);
            }
        }
        m_Tritable->show(); //表格展示
    }
    else if(modswtbtn->text()=="关节")
    {
        /***********************************************************/
        LCSP_CSYSINFO_STRU cystritru;
        cystritru.LCSPTrigInfo = m_updatecsptablestru.LCSPCPos[currow].LCSPTrigInfo;
        QVector<LCSP_CSYSINFO_STRU> trivec;
        trivec.append(cystritru);
        SWCONFIGXML_LCSP_STRU swtojstru;
        swtojstru.LCSPCPos=trivec;
        m_CoSwlib->CovertTo_J(swtojstru);//先ＸＹＺ转换成脉冲
        for(int j=0;j<swtojstru.LCSPCPos.size();++j)//脉冲转关节
        {
            for(int i=0;i<swtojstru.LCSPCPos[j].LCSPTrigInfo.size();++i)
            {
                for(int n=0;n<swtojstru.LCSPCPos[j].LCSPTrigInfo[i].LCSPTrigCPos.size();++n)
                {
                    swtojstru.LCSPCPos[j].LCSPTrigInfo[i].LCSPTrigCPos[n].pos= \
                            swtojstru.LCSPCPos[j].LCSPTrigInfo[i].LCSPTrigCPos[n].pos/m_eleratMap[swtojstru.LCSPCPos[j].LCSPTrigInfo[i].LCSPTrigCPos[n].id];
                }
            }
        }
        SWCONFIGXML_LCSP_STRU sumxtstru;
        sumxtstru.LCSPCPos= m_updatecsptablestru.LCSPCPos;
        m_CoSwlib->CovertTo_J(sumxtstru);
        for(int i=0;i<sumxtstru.LCSPCPos.size();++i)
        {
            for(auto it=sumxtstru.LCSPCPos[i].LCSCPosMap.begin();it!=sumxtstru.LCSPCPos[i].LCSCPosMap.end();++it)
            {
                if(m_eleratMap.contains(it.key()))
                {
                    it.value().pos=it.value().pos/m_eleratMap[it.key()];
                }
            }
        }
        if(m_row==0)
        {
            QMap<QString,LCSP_CPOSINFO_STRU> posMap;
            for(int i=0;i<m_hwarmid.size();++i)
            {
                LCSP_CPOSINFO_STRU pos;
                if(m_PulSw->isChecked())
                {
                    pos.pos=m_JntInfoMap[m_hwarmid[i]]->text().toDouble()/m_eleratMap[m_hwarmid[i]];
                }
                posMap.insert(m_hwarmid[i],pos);
            }
            //转化成关节坐标****************************************************************更改
            emit Getlinekpos(posMap,sumxtstru.LCSPCPos[currow].LCSCPosMap);
        }
        else{
            emit Getlinekpos(sumxtstru.LCSPCPos[currow-1].LCSCPosMap,sumxtstru.LCSPCPos[currow].LCSCPosMap);
        }
        QVector<LCSP_TRIGGERINFO_STRU> swTrixyztojvec;
        if(swtojstru.LCSPCPos.size()==1)
        {
            swTrixyztojvec=swtojstru.LCSPCPos[0].LCSPTrigInfo;
        }
        for(int i=0;i<m_deTritable->rowCount();++i)
        {
            m_deTritable->removeRow(i);
        }
        m_deTritable->setRowCount(1);
        for(int j=0;j<m_deTritable->columnCount();++j)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFont(QFont("Helvetica"));
            m_deTritable->setItem(0,j,item);
        }
        //更新数据
        for(int i=0;i< swTrixyztojvec.size();++i)//触发信息显示
        {
            m_deTritable->setRowCount(swTrixyztojvec.size());
            for(int j=0;j<m_deTritable->columnCount();++j)
            {
                QString str="";
                if(j==(m_deTritable->columnCount()-1))
                {
                    str= QString::number(swTrixyztojvec[i].stat);
                }
                else if(j==(m_deTritable->columnCount()-2))
                {
                    str= swTrixyztojvec[i].bitName;
                }
                else{
                    int detri=-1;
                    if(j < m_hwarmid.size())
                    {
                        for(int inde=0;inde<swTrixyztojvec[i].LCSPTrigCPos.size();++inde)
                        {
                            if( swTrixyztojvec[i].LCSPTrigCPos[inde].id==m_hwarmid[j])
                            {
                                detri = inde;
                                break;
                            }
                        }
                        if(detri >= 0)
                        {
                            str=QString::number(swTrixyztojvec[i].LCSPTrigCPos[detri].pos,10,3);
                        }
                    }
                }
                QTableWidgetItem *item = new QTableWidgetItem(str);
                item->setFont(QFont("Helvetica"));
                m_deTritable->setItem(i,j,item);
            }
        }
        if(swTrixyztojvec.size()!=0)
        {
            m_deTritable->insertRow(swTrixyztojvec.size());
            for(int j=0;j<m_deTritable->columnCount();++j)
            {
                QTableWidgetItem *item = new QTableWidgetItem("");
                item->setFont(QFont("Helvetica"));
                m_deTritable->setItem(swTrixyztojvec.size(),j,item);
            }
        }
        m_deTritable->show();
    }
}

void CspInfoUI::SaveTri(QVector<LCSP_TRIGGERINFO_STRU> LCSPTrigInfo)
{
    if(modswtbtn->text()=="关节")
    {
        //先转成脉冲进行组合
        for(int i=0;i<LCSPTrigInfo.size();++i)
        {
            for(int j=0;j<LCSPTrigInfo[i].LCSPTrigCPos.size();++j)
            {
                if(m_eleratMap.contains(LCSPTrigInfo[i].LCSPTrigCPos[j].id))
                {
                    LCSPTrigInfo[i].LCSPTrigCPos[j].pos = LCSPTrigInfo[i].LCSPTrigCPos[j].pos*m_eleratMap[LCSPTrigInfo[i].LCSPTrigCPos[j].id];
                }
            }
        }
        LCSP_CSYSINFO_STRU cystritru;
        cystritru.LCSPTrigInfo = LCSPTrigInfo;
        QVector<LCSP_CSYSINFO_STRU> trivec;
        trivec.append(cystritru);
        SWCONFIGXML_LCSP_STRU swtojstru;
        swtojstru.LCSPCPos=trivec;
        m_CoSwlib->CovertTo_X(swtojstru,m_RecswMap);//脉冲转xyz坐标进行存储
        if(swtojstru.LCSPCPos.size()==1) //把配置的好的信息进行存储
        {
            LCSPTrigInfo=swtojstru.LCSPCPos[0].LCSPTrigInfo;
        }
    }
    QVector<LCSP_TRIGGERINFO_STRU> strtri;
    strtri=LCSPTrigInfo;
    bool findrow = false;
    for(int i=0;i<m_newtri.size();++i)//如果存在当前的行
    {
        if(m_newtri[i].row==m_row)
        {
            findrow=true;
            m_newtri[i].row=m_row;
            m_newtri[i].LCSPTriggerInfo = strtri;
            break;
        }
    }
    if(!findrow)
    {
        RIGGERTABLESTU tri;
        tri.row=m_row;
        tri.LCSPTriggerInfo = strtri;
        m_newtri.append(tri);//原来不存在，则插入触发信息
    }
    for(int i=0;i<m_newtri.size();++i)
    {
        if(m_newtri[i].row==m_row)
        {
            if(m_updatecsptablestru.LCSPCPos[m_row].LCSPTrigInfo.size()==0)//更新数据
            {
                m_updatecsptablestru.LCSPCPos[m_row].LCSPTrigInfo.append(m_newtri[i].LCSPTriggerInfo);
                break;
            }
            else
            {
                m_updatecsptablestru.LCSPCPos[m_row].LCSPTrigInfo = m_newtri[i].LCSPTriggerInfo;
                break;
            }
        }
    }
}
///
/// \brief CspInfoUI::Acceptupdatedata
/// \param posvec
/// \param trap
///
void CspInfoUI::Acceptupdatedata(QVector<QMap<QString, double> > posvec,QVector<QString> trap)
{
    m_Tritable->close();//更新数据触发窗口关闭
    m_deTritable->close();
    int size=posvec.size();
    m_alltrap = trap;
    if(modswtbtn->text()=="关节")
    {    //posvec转化之后赋值关节模式下改变test************************************************
        QVector<QMap<QString, double>> swjvec;
        swjvec.clear();
        //关节转化成脉冲值传入
        for(int i=0;i<posvec.size();++i)
        {
            for(auto it=posvec[i].begin();it!=posvec[i].end();++it)
            {
                if(m_eleratMap.contains(it.key()))
                {
                    it.value()=it.value()*m_eleratMap[it.key()];
                }
            }
        }
        swjvec = GetXYZcurpos(posvec);//转换成由J
        posvec = swjvec;//所有的ｐ点
        m_TraPosvec = swjvec;
    }
    else
    {
        m_TraPosvec = posvec;
    }
    if(m_updatecsptablestru.LCSPCPos.size()>posvec.size())//进行删除操作
    {
        m_updatecsptablestru.LCSPCPos.remove(m_updatecsptablestru.LCSPCPos.size()-1);
    }
    for(int i=0;i< m_updatecsptablestru.LCSPCPos.size();++i)//清空原有的数据
    {
        m_updatecsptablestru.LCSPCPos[i].LCSCPosMap.clear();
    }
    for(int i=0;i<posvec.size();++i)//插入新的数据
    {
        QMap<QString ,LCSP_CPOSINFO_STRU> LCSPaxisMap;
        for(auto it=posvec[i].begin();it!=posvec[i].end();++it)
        {
            LCSP_CPOSINFO_STRU pos;
            pos.pos=it.value();
            LCSPaxisMap.insert(it.key(),pos);
        }
        if(i< m_updatecsptablestru.LCSPCPos.size())
        {
            m_updatecsptablestru.LCSPCPos[i].LCSCPosMap=LCSPaxisMap;//线段更新
        }
        else
        {
            LCSP_CSYSINFO_STRU csyssytru;
            csyssytru.LCSCPosMap=LCSPaxisMap;
            m_updatecsptablestru.LCSPCPos.append(csyssytru);
        }
    }
}

void CspInfoUI::SaveP()
{
    if(modswtbtn->text()=="坐标")
    {
        if(m_poseditxyz->text()=="")
        {
            QMessageBox::warning(this, QString("保存当前位置"), QString("目标ｐ不能为空!!!"));
            return;
        }
        bool flag=false;
        for(int i=0;i<m_table->rowCount()-1;++i)
        {
            if(m_table->item(i,0)->text()==m_poseditxyz->text())
            {
                QMessageBox::warning(this, QString("保存当前位置"), QString("确定要替换当前的P点!!!"));
                flag=true;
                //当前行进行替换表格更新
                for(int j=1;j<m_table->columnCount();++j)
                {
                    if(m_XYZPOSMap[m_hwcsysid[j-1]]->text().size()>0)
                    {
                        m_table->item(i,j)->setText(m_XYZPOSMap[m_hwcsysid[j-1]]->text());
                    }
                }
                //根据行数改变ｕｐｄａｔｅ
                QMap<QString,double> map;
                QMap <QString,LCSP_CPOSINFO_STRU> cspposmap;
                cspposmap.clear();
                for(int k(0);k<m_hwcsysid.size();++k)
                {
                    LCSP_CPOSINFO_STRU postru;
                    postru.pos=m_table->item(i,k+1)->text().toDouble();
                    map.insert(m_hwcsysid[k],m_table->item(i,k+1)->text().toDouble());
                    cspposmap.insert(m_hwcsysid[k],postru);
                }
                m_updatecsptablestru.LCSPCPos[i].LCSCPosMap=cspposmap;//具体的结构的变化
                m_TraPosvec[i]=map;//目标赋值替换
                break;
            }
        }
        if(!flag)//新建
        {
            int row=-1;
            for(int i=0;i<m_hwcsysid.size();++i)
            {
                if(m_XYZPOSMap[m_hwcsysid[i]]->text().size()<=0)
                {
                    QMessageBox::warning(this, QString("新建目标位置"), QString("请确保当前行数据存在!!!"));
                    return;
                }
            }
            row = m_table->currentRow();
            int trar=-1;
            if(row==-1||row==m_table->rowCount()-1)
            {
                trar=m_table->rowCount()-1;
            }
            else
            {
                trar=row+1;
            }
            m_table->insertRow(trar);
            for(int i(0);i<m_hwcsysid.size()+1;++i)
            {
                if(i==0)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(m_poseditxyz->text());
                    item->setFont(QFont("Helvetica"));
                    m_table->setItem(trar,i,item);
                }
                else
                {
                    QTableWidgetItem *item = new QTableWidgetItem(m_XYZPOSMap[m_hwcsysid[i-1]]->text());
                    item->setFont(QFont("Helvetica"));
                    m_table->setItem(trar,i,item);
                }
            }
            QMap<QString,double> map;
            QMap <QString,LCSP_CPOSINFO_STRU> cspposmap;
            cspposmap.clear();
            for(int k(0);k<m_hwcsysid.size();++k)
            {
                LCSP_CPOSINFO_STRU postru;
                postru.pos=m_table->item(trar,k+1)->text().toDouble();
                map.insert(m_hwcsysid[k],m_table->item(trar,k+1)->text().toDouble());
                cspposmap.insert(m_hwcsysid[k],postru);
            }
            LCSP_CSYSINFO_STRU cspcysstru;
            cspcysstru.LCSCPosMap = cspposmap;
            m_updatecsptablestru.LCSPCPos.insert(trar,cspcysstru);
            m_TraPosvec.insert(trar,map);
            m_alltrap.insert(trar,m_poseditxyz->text());//在后面插入
        }
    }
    else//关节模式下的保存数据的信息
    {
        if(m_poseditj->text()=="")
        {
            QMessageBox::warning(this, QString("保存当前位置"), QString("目标ｐ不能为空!!!"));
            return;
        }
        bool flag = false;
        for(int i=0;i<m_detable->rowCount()-1;++i)
        {
            if(m_detable->item(i,0)->text() == m_poseditj->text())
            {
                QMessageBox::warning(this, QString("保存当前位置"), QString("确定要替换当前的P点!!!"));
                flag=true;
                //替换更新
                for(int j=1;j<m_detable->columnCount();++j)
                {
                    m_detable->item(i,j)->setText(m_JntInfoMap[m_hwarmid[j-1]]->text());

                }
                QMap <QString,LCSP_CPOSINFO_STRU> cspposmap;
                cspposmap.clear();
                //转换得到ｘｙｚ坐标j1 j2 j3获取关节的各个坐标位置
                QMap<QString,double> mapj;
                for(int j=0;j<m_hwarmid.size();++j)
                {
                    if(m_PulSw->isChecked())
                    {
                        mapj.insert(m_hwarmid[j],m_JntInfoMap[m_hwarmid[0]]->text().toDouble());
                    }
                    else
                    {
                        if(m_eleratMap.contains(m_hwarmid[j]))
                        {
                            mapj.insert(m_hwarmid[j],m_JntInfoMap[m_hwarmid[0]]->text().toDouble()*m_eleratMap[m_hwarmid[j]]);
                        }
                    }
                }

                QMap<QString,double> map = GetXYZSinglecurpos(mapj);//得到大地坐标库调用
                for(int k(0);k<m_hwcsysid.size();++k)
                {
                    LCSP_CPOSINFO_STRU postru;
                    postru.pos=map[m_hwcsysid[k]];
                    cspposmap.insert(m_hwcsysid[k],postru);
                }
                m_updatecsptablestru.LCSPCPos[i].LCSCPosMap=cspposmap;//具体的结构的变化
                m_TraPosvec[i]=map;//目标赋值替换
                break;
            }
        }
        if(!flag)//新建
        {
            int row=-1;
            for(int i=0;i<m_hwarmid.size();++i)
            {
                if(m_JntInfoMap[m_hwarmid[i]]->text()=="")
                {  QMessageBox::warning(this, QString("新建目标位置"), QString("请确保当前行数据存在!!!"));
                    return;
                }
            }
            row = m_detable->currentRow();
            int trar=-1;
            if(row == -1||row == m_detable->rowCount()-1)
            {
                trar = m_detable->rowCount()-1;
            }
            else
            {
                trar = row+1;
            }
            m_detable->insertRow(trar);
            for(int i(0);i<m_hwarmid.size()+1;++i)
            {
                if(i==0)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(m_poseditj->text());
                    item->setFont(QFont("Helvetica"));
                    m_detable->setItem(trar,i,item);
                }
                else
                {
                    QTableWidgetItem *item = new QTableWidgetItem(m_JntInfoMap[m_hwarmid[i-1]]->text());
                    item->setFont(QFont("Helvetica"));
                    m_detable->setItem(trar,i,item);
                }
            }
            QMap<QString,double> map;
            QMap <QString,LCSP_CPOSINFO_STRU> cspposmap;
            cspposmap.clear();
            //test 转换 //转换得到ｘｙｚ坐标j1 j2 j3//test*********************
            QMap<QString,double> mapj;
            mapj.clear();
            map.clear();
            for(int j=0;j<m_hwarmid.size();++j)
            {
                if(m_PulSw->isChecked())
                {
                    mapj.insert(m_hwarmid[j],m_JntInfoMap[m_hwarmid[0]]->text().toDouble());
                }
                else
                {
                    if(m_eleratMap.contains(m_hwarmid[j]))
                    {
                        mapj.insert(m_hwarmid[j],m_JntInfoMap[m_hwarmid[0]]->text().toDouble()*m_eleratMap[m_hwarmid[j]]);
                    }
                }
            }
            map = GetXYZSinglecurpos(mapj);//得到大地坐标库调用
            for(int k(0);k<m_hwcsysid.size();++k)
            {
                LCSP_CPOSINFO_STRU postru;
                postru.pos=map[m_hwcsysid[k]];
                cspposmap.insert(m_hwcsysid[k],postru);
            }
            LCSP_CSYSINFO_STRU cspcysstru;
            cspcysstru.LCSCPosMap=cspposmap;
            m_updatecsptablestru.LCSPCPos.insert(trar,cspcysstru);
            m_TraPosvec.insert(trar,map);
            m_alltrap.insert(trar,m_poseditj->text());//在后面插入
        }
    }
}

void CspInfoUI::RunXYZIncandRed(int inde)
{
    LCSP_CPOSINFO_STRU xyzpos;
    QString strtext = "";
    strtext = m_RecIdbtn[inde]->text();
    if(m_RecIdbtn[inde]->text().contains("+"))
    {
        xyzpos.pos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[QString("%1坐标").arg(strtext.remove(0,1))].endlim;
    }
    else
    {
        xyzpos.pos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap[QString("%1坐标").arg(strtext.remove(0,1))].stalim;
    }
    QString str = QString("%1坐标").arg(strtext);
    if(!JudgeCoordEvent(str))/****事件保护*/
        return;
    static double ispeed = m_updatecsptablestru.speed;
    double acc = m_updatecsptablestru.acc*0.1*0.5;
    double speed = ispeed*0.1*0.5*0.1;
    if(m_cspbtn[4]->text()=="HS")
    {
        speed = m_updatecsptablestru.speed*1*0.5*0.1;
        acc = m_updatecsptablestru.acc*1*0.5;
    }
    else if(m_cspbtn[4]->text()=="MS")
    {
        speed = m_updatecsptablestru.speed*0.25*0.5*0.1;
        acc = m_updatecsptablestru.acc*1*0.25;
    }
    if(speed < 1)
    {
        speed = 1;
    }
    if(acc < 1)
    {
        acc = 1;
    }
    SWCONFIGXML_LCSP_STRU xstru;
    xstru.speed = speed;
    xstru.timeOut = 10000;
    xstru.acc = acc;//加速度减速度赋值
    QMap<QString ,LCSP_CPOSINFO_STRU> xmap;
    xmap.insert(str,xyzpos);
    LCSP_CSYSINFO_STRU xstrup;
    xstrup.LCSCPosMap = xmap;
    QVector<LCSP_CSYSINFO_STRU> xstruvec;
    xstruvec.append(xstrup);
    xstru.LCSPCPos = xstruvec;
    m_CoSwlib->CovertTo_J(xstru);//***********转换
    xstru.id = m_updatecsptablestru.id;
    if(!Checkconfig(xstru))
    {
        statinfo->setText("请检查ｘｍｌ设置");
        return;
    }
    if(!MyLCSPThread->isRunning())
    {
        if(!CheckSON())
        {
            statinfo->setText("请设置励磁....");
            return;
        }
        m_axisrunid = QString("%1:坐标").arg(m_RecIdbtn[inde]->text());
        MyLCSPThread->SetParam(xstru);
        MyLCSPThread->start();
        SetDisabledBtn("坐标",inde);
        statinfo->setText("");
    }
}

void CspInfoUI::RunArmIncandRed(int inde)
{
    double limpos = 0;
    QString strbtntext = "";
    strbtntext = m_JntBtn[inde]->text();
    if(m_JntBtn[inde]->text().contains("+"))
    {
        limpos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strbtntext.remove(0,1)].swLimitEnd.toDouble()*m_eleratMap[strbtntext];}
    else
    {
        limpos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strbtntext.remove(0,1)].swLimitStart.toDouble()*m_eleratMap[strbtntext];
    }
    if(!JudgeAsixEvent(strbtntext))
        return;
    static double ispeed  = m_updatecsptablestru.speed;
    double speed = ispeed*0.1*0.5;
    double acc = m_updatecsptablestru.acc*0.1*0.5;
    if(m_cspbtn[4]->text()=="HS")
    {
        speed = m_updatecsptablestru.speed*1*0.5;
        acc = m_updatecsptablestru.acc*1*0.5;
    }
    else if(m_cspbtn[4]->text()=="MS")
    {
        speed = m_updatecsptablestru.speed*0.25*0.5;
        acc = m_updatecsptablestru.acc*1*0.25;
    }
    if(speed < 1)
    {
        speed = 1;
    }
    if(acc < 1)
    {
        acc = 1;
    }
    SWCONFIGXML_LCSP_STRU xstru;
    xstru.speed = speed;
    xstru.timeOut = 12500*0.1;
    xstru.acc = acc;//加速度减速度赋值
    QMap<QString ,LCSP_CPOSINFO_STRU> xmap;
    LCSP_CPOSINFO_STRU xyzpos;
    xyzpos.pos=limpos;
    xmap.insert(strbtntext,xyzpos);
    LCSP_CSYSINFO_STRU sysystru;
    sysystru.LCSCPosMap = xmap;
    xstru.LCSPCPos.append(sysystru);
    xstru.id = m_updatecsptablestru.id;
    QVector<QString> str;
    str.append(strbtntext);
    if(!MyLCSPThread->isRunning())
    {
        if(!CheckSON())
        {
            statinfo->setText("请设置励磁....");
            return;
        }
        statinfo->setText("");
        m_axisrunid = QString("%1:关节").arg(m_JntBtn[inde]->text());
        MyLCSPThread->SetParam(xstru);//传入的设置的关节的脉
        MyLCSPThread->start();
        SetDisabledBtn("关节",inde);
    }
}

void CspInfoUI::changemodel()
{
    m_PulSw->setChecked(false);
    if(modswtbtn->text()=="关节")
    {
        modswtbtn->setText("坐标");
        modswt->setText("关节");
        m_cspjwg->close();
        m_cspxyzwg->show();
        for(int i=0;i<m_table->rowCount();++i)  //信息填入表格ｘｙｚ表格
        {
            m_table->removeRow(i);
        }
        m_table->setRowCount(m_TraPosvec.size()+1);
        for(int i=0;i<m_table->rowCount();++i)
        {
            if(i!=(m_table->rowCount()-1))
            {
                QTableWidgetItem *item = new QTableWidgetItem(m_alltrap[i]);
                item->setFont(QFont("Helvetica"));
                m_table->setItem(i,0,item);
                for(int j=1;j<m_table->columnCount();++j)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(m_TraPosvec[i][m_hwcsysid[j-1]],10,3));
                    item->setFont(QFont("Helvetica"));
                    m_table->setItem(i,j,item);
                }
            }
            else
            {
                for(int j=0;j<m_table->columnCount();++j)
                {
                    QTableWidgetItem *item = new QTableWidgetItem("");
                    item->setFont(QFont("Helvetica"));
                    m_table->setItem(m_table->rowCount()-1,j,item);
                }
            }
        }
    }
    else
    {
        m_cspxyzwg->close();
        m_cspjwg->show();
        modswtbtn->setText("关节");
        modswt->setText("坐标");//关节坐标信息填入表格Ｊ１Ｊ２表格
        QVector<QMap<QString,double>> ntrapossw;
        ntrapossw.clear();
        //xyz
        ntrapossw=GetJpos(m_TraPosvec);//由XYZ装换成关节坐标***************
        for(int i=0;i<m_detable->rowCount();++i)
        {
            m_detable->removeRow(i);
        }
        m_detable->setRowCount(m_TraPosvec.size()+1);
        if(ntrapossw.size()!=m_TraPosvec.size())
            return;
        for(int i=0;i<m_detable->rowCount();++i)
        {
            if(i!=m_detable->rowCount()-1)
            {
                QTableWidgetItem *item = new QTableWidgetItem(m_alltrap[i]);
                item->setFont(QFont("Helvetica"));
                m_detable->setItem(i,0,item);
                for(int j=1;j<m_detable->columnCount();++j)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(ntrapossw[i][m_hwarmid[j-1]],10,3));
                    item->setFont(QFont("Helvetica"));
                    m_detable->setItem(i,j,item);
                }
            }
            else
            {
                for (int j=0;j<m_detable->columnCount();++j)
                {
                    QTableWidgetItem *item = new QTableWidgetItem("");
                    item->setFont(QFont("Helvetica"));
                    m_detable->setItem(m_detable->rowCount()-1, j, item);
                }
            }
        }
    }
}

void CspInfoUI::RunORG()
{
    m_axisrunid = "";
    QPushButton *qBtn = qobject_cast<QPushButton*>(sender());
    QString text = qBtn->text();
    QStringList list = text.split(":");
    m_orgrun = true;
    if(!m_ORGThreadMap.contains(list[0]))
    {
        MainORGThread * tr = new MainORGThread;
        m_ORGThreadMap.insert(list[0],tr);
        tr->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[list[0]]);
        tr->start();
    }
    else
    {
        m_ORGThreadMap[list[0]]->m_result = 1;
        m_ORGThreadMap[list[0]]->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[list[0]]);
        m_ORGThreadMap[list[0]]->start();
    }
    qBtn->setEnabled(false);
    for(int i = 1;i < 6;++i) //按钮保护
    {
        if(m_cspbtn[i] != NULL)
        {
            if(m_cspbtn[i]->isEnabled())
            {
                m_cspbtn[i]->setEnabled(false);
            }
        }
    }
    for(int i=0;i<m_hwcsysid.size()*2;++i)
    {
        if(m_RecIdbtn[i]->isEnabled())
        {
            m_RecIdbtn[i]->setEnabled(false);
        }
    }
    m_XYZSave->setEnabled(false);
    for(int i = 0;i < m_hwarmid.size()*2;++i)
    {
        if(m_JntBtn[i]->isEnabled())
        {
            m_JntBtn[i]->setEnabled(false);
        }
    }
    m_Jsave->setEnabled(false);
    m_PulSw->setEnabled(false);
    statinfo->setText("ORG运行中...");
    return;
}

void CspInfoUI::DisableBtn(bool enble)
{
    for(int i(0);i < 6;++i)
    {
        if(m_hwarmid.size() == 0)
        {
            m_cspbtn[i]->setEnabled(false);
        }
        else
        {
            m_cspbtn[i]->setEnabled(enble);
        }
    }
    for(int i = 0;i< m_hwcsysid.size()*2;++i)
    {
        if(i < 6)
        {
            m_RecIdbtn[i]->setEnabled(enble);
        }
    }
    m_XYZSave->setEnabled(enble);
    for(int i = 0;i< m_hwarmid.size()*2;++i)
    {
        if(i < 12)
        {
            m_JntBtn[i]->setEnabled(enble);
        }
    }
    m_Jsave->setEnabled(enble);
    m_PulSw->setEnabled(enble);
    modswtbtn->setEnabled(enble);
    for(auto it = m_ORGBtnMap.begin();it != m_ORGBtnMap.end();++it)
    {
        it.value()->setEnabled(enble);
    }
}

CSPThread::CSPThread()
{
    m_csplib = new LCSPLib();
}

void CSPThread::run()
{
    m_runiresult = m_csplib->RunLCSPAxis(m_ps);
}

void CSPThread::SetParam(SWCONFIGXML_LCSP_STRU _ps)
{
    m_ps = _ps;
}

void CSPThread::InitCSPParam(SWCONFIGXML_LCSP_STRU _ps)
{
    m_csplib->InitCspAxisParam(_ps);
}

void CSPThread::Stop()
{
    m_csplib->StopLCSP(m_ps.id);
}
bool CspInfoUI::JudgeAsixEvent(const QString strAsix)
{
    if(axisEventMap.contains(strAsix))
    {
        StackManager st;
        if(!st.AnalysisEvent(axisEventMap[strAsix].strEvent))
        {
            statinfo->setText(axisEventMap[strAsix].strNote);
            return false;
        }
    }
    return true;
}
bool CspInfoUI::JudgeCoordEvent(const QString strCoord)
{
    if(coordEventMap.contains(strCoord))
    {
        StackManager st;
        if(!st.AnalysisEvent(axisEventMap[strCoord].strEvent))
        {
            statinfo->setText(axisEventMap[strCoord].strNote);
            return false;
        }
    }
    return true;
}

bool CspInfoUI::CheckSON()
{
    bool s_onok = true;
    if(m_tempINSON.count() > 3)
    {
        if (m_hwarmid.size() == 0)
        {
            return false;
        }
        for(int  it=0;it < m_hwarmid.size();++it)
        {
            if(!CoreGlobal::BaseAxisOperate::CheckAxisExcited(m_tempINSON,2,m_statposbyteMap[m_hwarmid[it]]))//检测励磁不准
            {
                s_onok = false;
                break;
            }
        }
    }
    else
    {
        s_onok = false;
    }
    return s_onok;
}

void CspInfoUI::AddPosData(SWCONFIGXML_LCSP_STRU &stru, QVector<QString> str)
{
    if(stru.LCSPCPos.size() == 0)
    {
        LCSP_CSYSINFO_STRU posstru;
        QMap<QString ,LCSP_CPOSINFO_STRU> posmap;
        for(int i = 0; i < str.size();++i)
        {
            LCSP_CPOSINFO_STRU  posru;
            posru.pos = 0.0;
            posmap.insert(str[i],posru);
        }
        posstru.LCSCPosMap = posmap;
        QVector<LCSP_CSYSINFO_STRU> vec;
        vec.append(posstru);
        stru.LCSPCPos = vec;
    }
}
