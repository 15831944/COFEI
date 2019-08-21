#include "calibrationui.h"
#include <QScrollArea>
#include <QGroupBox>
#include <configform.h>
#include <QDebug>
#include <DycLibBase/StackManager/stackmanager.h>
//#include "axisrunpos.h"
//const QString Stylestr = "QPushButton{background-color:rgb(112,112,112);color:black;}"
//                         "QPushButton:hover{background-color:rgb(60,60,60);}"
//                         "QPushButton:pressed{background-color:rgb(40,40,40);}";
CalibrationUI::CalibrationUI(QVector<QString> vec,int iwidth,int iheght,const QString& strType)
{
    m_hwMap = ConfigData::GetInstance()->xmlMap.hwConfigXml;
    m_swSerMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map;
    m_AxisVec = vec;
    m_width = iwidth;
    m_hight = iheght;
    m_strType = strType;
    m_cspInfoUI = NULL;
    //    m_Runpos = NULL;
    //    m_pTimer = NULL;
    this->setFixedSize(m_width,m_hight);
    m_font.setPointSize(15);
    m_font.setFamily("黑体");
    this->setFont(m_font);
    this->setWindowTitle("轴信息校验界面");
    this->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
    // axisEventMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_ProtectEvent_Map.axisEventMap;
    //CreateUI(m_strType);
}

CalibrationUI::~CalibrationUI()
{
    if(m_cspInfoUI!=NULL)
        delete m_cspInfoUI;
    m_cspInfoUI = NULL;
}
void CalibrationUI::CreateUI(const QString &strType ,QString str)
{
    QVBoxLayout *vbox = new  QVBoxLayout(this);
    this->setLayout(vbox);
    QScrollArea *pArea = new QScrollArea(this);
    pArea->setGeometry(10,10,m_width-20,m_hight -70);
    pArea->setFixedWidth(m_width-50);
    QWidget *wgCbt = new QWidget(this);
    pArea->setWidget(wgCbt);
    pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pArea->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    if(strType.compare("快速定位") == 0)
    {
        QGridLayout *gridLayout = new QGridLayout();
        gridLayout->setAlignment(Qt::AlignLeft);
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout->setSpacing(10);
        int irow = 0;
        int icol = 0;
        int iw = (m_width-20)/410;
        for(int i = 0; i < m_AxisVec.count();++i)
        {
            AxisInfoUI  *asixInfoUI = new AxisInfoUI(m_swSerMap,this);
            asixInfoUI->SetAxisName(m_AxisVec[i]);
            asixInfoUI->SetAxisRanage(m_hwMap.hwConfigXmlOutputMap.hwSerOutputMap[m_AxisVec[i]].swLimitStart.toDouble(),\
                    m_hwMap.hwConfigXmlOutputMap.hwSerOutputMap[m_AxisVec[i]].swLimitEnd.toDouble());
            asixInfoUI->TimerStatus(false);
            connect(this,&CalibrationUI::DisableAxisUI,asixInfoUI,&AxisInfoUI::DisableBtn);
            //connect(asixInfoUI,&AxisInfoUI::DisableGobtn,this,&CalibrationUI::DisableGOBtn);// 增加GO按钮保护
            m_axisUIVec.append(asixInfoUI);
            gridLayout->addWidget(asixInfoUI,irow,icol++);
            if(icol >= iw)
            {
                ++irow;
                icol = 0;
            }
        }
        wgCbt->setLayout(gridLayout);
        vbox->addLayout(gridLayout);
        wgCbt->setGeometry(0,0,420*iw,210*(irow+1));

    }
    else if(strType.compare("直线插补") == 0)
    {
        QVBoxLayout *cspvbox = new  QVBoxLayout(this);
        cspvbox->setSizeConstraint(QLayout::SetFixedSize);
        m_cspInfoUI = new CspInfoUI(m_AxisVec,m_LCSPser,this);
        //add connect
        m_cspInfoUI->TimerStatus(false);
        connect(this,&CalibrationUI::DisableAxisUI,m_cspInfoUI,&CspInfoUI::DisableBtn);
        cspvbox->addWidget(m_cspInfoUI);
        wgCbt->setGeometry(0,0,m_width-10,m_hight-10);
        wgCbt->setLayout(cspvbox);
    }
    vbox->addWidget(pArea);
    QWidget *axisinfo = new QWidget(this);
    QHBoxLayout *axisinfolaout= new QHBoxLayout();
    axisinfolaout->setSpacing(0);
    axisinfo->setFixedSize(290,40);
    axisinfo->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));

    axisinfo->setLayout(axisinfolaout);
    tips = new QLabel();
    tips->setFixedSize(260,40);
    tips->setFont(m_font);
    tips->setAlignment(Qt::AlignHCenter);
    tips->setToolTip("修改当前点位的信息");
    axisinfolaout->addWidget(tips);

    QWidget *wgbtn = new QWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(wgbtn);
    hbox->setSpacing(0);
    wgbtn->setFixedSize(m_width-360,40);
    wgbtn->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    wgbtn->setLayout(vbox);
    QPushButton *cancleBtn = new QPushButton(this);
    cancleBtn->setFixedSize(100,30);
    cancleBtn->setFont(m_font);
    cancleBtn->setText("取消");
    connect(cancleBtn,&QPushButton::clicked,this,&QDialog::reject);
    QPushButton *okBtn = new QPushButton(this);
    okBtn->setFixedSize(100,30);
    okBtn->setText("示教");
    okBtn->setFont(m_font);
    connect(okBtn,&QPushButton::clicked,this,&CalibrationUI::SaveData);

    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox1->addWidget(axisinfo);
    hbox1->addWidget(wgbtn);
    hbox->addWidget(cancleBtn);
    hbox->addWidget(okBtn);
    vbox->addLayout(hbox1);
}

void CalibrationUI::GetAxisConfig(SWCONFIGXML_QPOSIT_STRU &swSer, QMap<QString,int> temp)
{
    swSer.acc = ConfigData::GetInstance()->iAccel;
    swSer.dec = ConfigData::GetInstance()->iDccel;
    swSer.axisNum = m_axisUIVec.count();
    swSer.speed = temp["全局"];
    for(int i = 0; i < m_axisUIVec.count();++i)
    {
        AXISINFO_STRU axisinfo;
        axisinfo.pos = m_axisUIVec[i]->GetTraPos();
        if(temp.contains(m_axisUIVec[i]->GetAxisName()))
            axisinfo.speed = temp[m_axisUIVec[i]->GetAxisName()];
        else
            axisinfo.speed = temp["全局"];
        swSer.axisMap.insert(m_axisUIVec[i]->GetAxisName(),axisinfo);
    }
    //
}
void CalibrationUI::GetAxisConfig(SWCONFIGXML_LCSP_STRU &swSer)
{
    swSer.LCSPCPos = m_cspInfoUI->GetLcspAllPos();
}

bool CalibrationUI::Compare2QPStruct(SWCONFIGXML_QPOSIT_STRU swser1, SWCONFIGXML_QPOSIT_STRU swser2)
{
    bool isok = true;
    if(swser1.acc != swser2.acc)
    {
        isok = false;
        return isok;
    }
    if(swser1.axisNum != swser2.axisNum)
    {
        isok = false;
        return isok;
    }
    if(swser1.dec != swser2.dec)
    {
        isok = false;
        return isok;
    }
    if(swser1.delayMsec != swser2.delayMsec)
    {
        isok = false;
        return isok;
    }
    if(swser1.id != swser2.id)
    {
        isok = false;
        return isok;
    }
    if(swser1.note != swser2.note)
    {
        isok = false;
        return isok;
    }
    if(swser1.speed != swser2.speed)
    {
        isok = false;
        return isok;
    }
    if(swser1.timeOut != swser2.timeOut)
    {
        isok = false;
        return isok;
    }
    for(auto it = swser1.axisMap.begin();it != swser1.axisMap.end();++it)
    {
        if(swser2.axisMap.contains(it.key()))
        {
            if(swser2.axisMap[it.key()].offsetPos != it.value().offsetPos)
            {
                isok = false;
                return isok;
            }
            if(swser2.axisMap[it.key()].pos != it.value().pos)
            {
                isok = false;
                return isok;
            }
            if(swser2.axisMap[it.key()].speed != it.value().speed)
            {
                isok = false;
                return isok;
            }
        }
        else
        {
            isok = false;
            return isok;
        }
    }
    return isok;
}

void CalibrationUI::ChangetipsText(const QString str)
{
    tips->setText(str);
}
bool CalibrationUI::CreateMessageBox(QString strTitle,QString note)
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
void CalibrationUI::SaveData()
{
    QString str = "";
    if(m_strType.compare("快速定位") == 0)
    {
        QString ID = m_QPser.id;

        str = QString("   是否保存当前点位信息？ %1 %2 ID :%3 请确认!").arg("\n").arg(m_strType).arg(m_QPser.id);
    }
    else
    {
        str = QString("   是否保存当前点位信息？ %1 %2 ID :%3 请确认!").arg("\n").arg(m_strType).arg(m_LCSPser.id);
    }
    if(!CreateMessageBox("提醒",str))
    {
        return;
    }
    if(m_strType.compare("快速定位") == 0)
    {
        GetAxisConfig(m_QPser,m_checkdlgData);
        //if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(m_strType))
        {
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(m_QPser.id))
            {
                if(Compare2QPStruct(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[m_QPser.id],m_QPser))
                {
                    return;
                }
                else
                    ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[m_QPser.id] = m_QPser;
            }
            else
            {
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.insert(m_QPser.id,m_QPser);
            }
        }
    }
    else if(m_strType.compare("直线插补") == 0)
    {
        GetAxisConfig(m_LCSPser);
        if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(m_LCSPser.id))
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.insert(m_LCSPser.id,m_LCSPser);
        }
        else
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[m_LCSPser.id] = m_LCSPser;
        }
    }
    emit SaveAxisSerConfig(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map,
                           ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map);
}

void CalibrationUI::GetChangeTimerSig(bool isenable)
{
    for(int i(0);i < m_axisUIVec.count();++i)
    {
        m_axisUIVec[i]->TimerStatus(isenable);
    }
    if(m_cspInfoUI != NULL)
    {
        m_cspInfoUI->TimerStatus(isenable);
    }
}

