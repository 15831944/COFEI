#include "AxisOperater.h"
#include <DycLibBase/StackManager/stackmanager.h>
#include <CoGraphicsItems/AttributeInstance.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QDebug>

float timeuse;
struct timeval tpStart,tpEnd;
bool m_press = true;
AxisOperater::AxisOperater(QWidget *parent) : QWidget(parent)
{
    m_ison = true;
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&AxisOperater::CheckSonTimer);
    m_OrgPlugin = ConfigData::GetInstance()->pluginMap.contains("ORG") ? ConfigData::GetInstance()->pluginMap["ORG"]->Copy():NULL;
    m_PPPlugin = ConfigData::GetInstance()->pluginMap.contains("G00") ? ConfigData::GetInstance()->pluginMap["G00"]->Copy():NULL;
    m_RPPlugin = ConfigData::GetInstance()->pluginMap.contains("G02") ? ConfigData::GetInstance()->pluginMap["G02"]->Copy():NULL;
    m_font.setPointSize(12);
    m_font.setFamily("黑体");
    QWidget *wg = new QWidget(this);
    wg->setStyleSheet(tr("background-color:rgb(35,35,35);color:rgb(166,167,166);border-radius:10px;"));
    for(int i = 0; i < 4;++i)
    {
        m_pHBox[i]       = new QHBoxLayout();
        m_pHBox[i]->setAlignment(Qt::AlignLeft);
    }
    m_pLabel[0]   = new QLabel(wg);
    m_pLabel[0]->setFont(m_font);
    m_pLabel[0]->setFixedSize(96,32);
    m_pLabel[1]   = new QLabel(wg);
    m_pLabel[1]->setFont(m_font);
    m_pLabel[1]->setFixedHeight(32);
    m_pVBox = new QVBoxLayout();
    wg->setLayout(m_pVBox);
    m_pCurPos = new QLineEdit("0",wg);
    m_pCurPos->setStyleSheet(tr("background-color:rgb(40,42,51);"));
    m_pCurPos->setAlignment(Qt::AlignCenter);
    m_pCurPos->setFixedSize(96,32);
    m_pCurPos->setFont(m_font);
    for(int i = 0; i < 8;++i)
    {
        m_pBtn[i] = new QPushButton(wg);
        m_pBtn[i]->setFixedSize(96,32);
        m_pBtn[i]->setFont(m_font);
        m_pBtn[i]->setStyleSheet(tr("QPushButton{background-color:rgb(40,42,51);}"
                                    "QPushButton::hover{background-color:rgb(50, 50, 55);}"
                                    "QPushButton::pressed{background-color:rgb(20, 20, 25);}"));
    }
    m_pBtn[1]->setText("回原点");
    m_pBtn[0]->setText("励磁");
    m_pBtn[2]->setText("复位");
    m_pBtn[3]->setText("停止");
    m_pBtn[4]->setText("+");
    m_pBtn[5]->setText("-");
    m_pBtn[6]->setText("绝对运动");
    m_pBtn[7]->setText("相对运动");//******

    m_pCbb = new QComboBox(wg);
    m_pCbb->setFixedSize(96,32);
    m_pCbb->setFont(m_font);
    m_pCbb->setStyleSheet(tr("background-color:rgb(40,42,51);"));
    m_pDsb = new QDoubleSpinBox(wg);
    m_pDsb->setStyleSheet(tr("background-color:rgb(40,42,51);"));
    m_pDsb->setFixedSize(96,32);
    m_pDsb->setFont(m_font);
    m_pDsb->setDecimals(3);
    CreatAsixUI();
    wg->setFixedSize(400,190);//......
    connect(m_pBtn[1],&QPushButton::clicked,this,&AxisOperater::RunORG);
    connect(m_pBtn[0],&QPushButton::clicked,this,&AxisOperater::RunSon);
    connect(m_pBtn[2],&QPushButton::clicked,this,&AxisOperater::RunReSet);
    connect(m_pBtn[4],&QPushButton::pressed,this,&AxisOperater::RunLeft);
    connect(m_pBtn[4],&QPushButton::released,this,&AxisOperater::RunStop);
    connect(m_pBtn[5],&QPushButton::pressed,this,&AxisOperater::RunRight);
    connect(m_pBtn[5],&QPushButton::released,this,&AxisOperater::RunStop);
    connect(m_pBtn[6],&QPushButton::clicked,this,&AxisOperater::RunQuickFixPos);
    connect(m_pBtn[3],&QPushButton::clicked,this,&AxisOperater::RunStop);
    connect(m_pBtn[7],&QPushButton::clicked,this,&AxisOperater::RunQuickFixPos);
    this->setFixedSize(420,200);//......
    return;
}

AxisOperater::~AxisOperater()
{
    m_timer->stop();
}

void AxisOperater::CreatAsixUI()
{
    m_pHBox[3]->addWidget(m_pLabel[0]);
    m_pHBox[3]->addWidget(m_pLabel[1]);
    m_pVBox->addLayout(m_pHBox[3]);
    for(int i = 0;i < 4;++i)
    {
        m_pHBox[0]->addWidget(m_pBtn[i]);
    }
    QLabel *pLabel   = new QLabel("速度:");
    pLabel->setFont(m_font);
    QStringList list;
    list  << "5" << "10" << "15" << "20" << "25" << "30" << "40" << "50" << "60" << "70" << "80" << "90" << "100";
    m_pCbb->addItems(list);
    m_pCurPos->setDisabled(true);
    m_pBtn[4]->setFixedSize(48,32);
    m_pBtn[5]->setFixedSize(48,32);
    m_pHBox[1]->addWidget(pLabel);
    m_pHBox[1]->addWidget(m_pCbb);
    m_pHBox[1]->addWidget(m_pBtn[4]);
    m_pHBox[1]->addWidget(m_pCurPos);
    m_pHBox[1]->addWidget(m_pBtn[5]);

    pLabel   = new QLabel("位置设定:");
    pLabel->setFont(m_font);
    m_pHBox[2]->addWidget(pLabel);
    m_pHBox[2]->addWidget(m_pDsb);
    m_pHBox[2]->addWidget(m_pBtn[6]);
    m_pHBox[2]->addWidget(m_pBtn[7]);
    m_pVBox->addLayout(m_pHBox[0]);
    m_pVBox->addLayout(m_pHBox[1]);
    m_pVBox->addLayout(m_pHBox[2]);
    m_font.setPixelSize(20);
    m_pBtn[4]->setFont(m_font);
    m_pBtn[5]->setFont(m_font);
}

void AxisOperater::SetAxisName(const QString strName,QMap<QString,PROTECT_EVENT_STRU> temp)
{
    m_pLabel[0]->setText("轴 " + strName);
    m_axisName = strName;
    m_contrlBytePos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(strName,m_contrlBitInfoMap,m_OutbyteInfoMap);
    m_stausBytePos = MyHW_Share::GetInstance()->AnalysisInPutBytes(strName,m_stausBitInfoMap,m_InbyteInfoMap);
    m_axisEventMap = temp;
    m_timer->start(300);    
    //得到所有的数据库保存的速度参数
     m_dbspeedparm.clear();
    if(DBInstanceManager::getDBInstance()->DBVarStru.qpMap.contains(m_pointName))
    {
        int acc =  DBInstanceManager::getDBInstance()->DBVarStru.qpMap[m_pointName].acc;
        int dec = DBInstanceManager::getDBInstance()->DBVarStru.qpMap[m_pointName].dec;
        m_dbspeedparm.append(acc);
        m_dbspeedparm.append(dec);
        QString axisinfo = QString(DBInstanceManager::getDBInstance()->DBVarStru.qpMap[m_pointName].axisidinfo);
        QStringList axisinfolist = axisinfo.split(";");
        for(int i = 0; i < axisinfolist.size(); ++i)
        {
            QString name = axisinfolist[i];
            QStringList  namelist = name.split(":");
            if(namelist.size() == 2)
            {
                if(namelist[0] == m_axisName)
                {
                    QString axisSpeed = namelist[1];
                    QStringList axisSpeedlist = axisSpeed.split(",");
                    if(axisSpeedlist.size() >= 2)
                    {
                        QString speed = axisSpeedlist[1];
                        int speedparm = speed.toInt();
                        m_dbspeedparm.append(speedparm);
                        break;
                    }
                }
            }
        }
    }
}

void AxisOperater::SetAxisRanage(const double dstart,const double dend)
{
    m_pDsb->setRange(dstart,dend);
    m_pDsb->setValue(0.0);
}

QString AxisOperater::GetAxisName()
{
    return m_axisName;
}

double AxisOperater::GetTraPos()
{
    return m_pCurPos->text().toDouble();
 //  return m_pDsb->value();
}

uint AxisOperater::GetTraSpeed()
{
    return m_pCbb->currentText().toUInt();
}
void AxisOperater::SetTraPos(double d)
{
    m_pCurPos->setText(QString("%1").arg(d));
}

void AxisOperater::SetPower(int power)
{
    m_power = power;
}

void AxisOperater::RunORG()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    StopPlugins();
    if(m_OrgPlugin)
    {
        m_pLabel[1]->setText("执行回原点...");
        m_OrgPlugin->Initialization();
        SWCONFIGXML_STRU ps;
        SWCONFIGXML_ORG_OP_STRU orgStru;
        orgStru.utimeout = 60000;
        orgStru.swXml_ORG_AxisId_Vec.append(m_axisName);
        ps.swXml_ORG_Stru = orgStru;

        m_OrgPlugin->SetModuleParam(ps);
        m_OrgPlugin->RunModule();
        QString strMsg;
        m_OrgPlugin->GetExcResult(strMsg);
        m_OrgPlugin->StopModule();
        m_pLabel[1]->setText(strMsg);
    }
}

void AxisOperater::RunSon()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_OutbyteInfoMap["Model"].strType,m_OutbyteInfoMap["Model"].byteNum,m_OutbyteInfoMap["Model"].bytePos,1);
    QThread::msleep(20);
    CoreGlobal::BaseAxisOperate::SetAxisExciteOff(MyHW_Share::GetInstance()->m_OFFSON,m_contrlBytePos);
    QThread::msleep(20);
    if(m_pBtn[0]->text() == "励磁")
    {
        m_pBtn[0]->setText("松励磁");
        CoreGlobal::BaseAxisOperate::SetAxisExcite(MyHW_Share::GetInstance()->m_OUTSON,m_contrlBytePos);
    }
    else
    {
        m_pBtn[0]->setText("励磁");
    }
    m_ison = !m_ison;
}

void AxisOperater::RunReSet()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    if(m_contrlBytePos == -1)
        return;
    if(m_contrlBitInfoMap.contains("Reset"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisReset(m_contrlBitInfoMap["Reset"].bit,m_contrlBytePos);
    }
}

void AxisOperater::RunLeft()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    StopPlugins();
    if(!JudgeButtonIsExcEvent())
    {
        return;
    }
    if(!JudgeAsixEvent() ||
            !ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    uint Traspeed = m_pCbb->currentText().toUInt();
    double Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd.toDouble();
    if(m_pCurPos->text().toDouble() > Trapos)
    {
        return;
    }
    SWCONFIGXML_QPOSIT_STRU qpStru;
    qpStru.delayMsec = 5;
    qpStru.timeOut = 60000;
    qpStru.axisNum = 1;
    AXISINFO_STRU asixStru;
     asixStru.pos = Trapos;
    if(m_dbspeedparm.size() == 3)
    {
        ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart =QString::number(-999999);
        qpStru.acc = m_dbspeedparm[0];
        qpStru.dec = m_dbspeedparm[1];
        asixStru.speed =float(m_dbspeedparm[2]*Traspeed)/100;
        qpStru.axisMap[m_axisName] = asixStru;
        RunAbsAsix(qpStru);
    }
    return;
}

void AxisOperater::RunRight()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    StopPlugins();
    if(!JudgeButtonIsExcEvent())
    {
        return;
    }
    if(!JudgeAsixEvent() ||
            !ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    uint Traspeed = m_pCbb->currentText().toUInt();
    double Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart.toDouble();
    if(m_pCurPos->text().toDouble() < Trapos)
    {
        return;
    }
    SWCONFIGXML_QPOSIT_STRU qpStru;
    qpStru.delayMsec = 5;
    qpStru.timeOut = 60000;
    qpStru.axisNum = 1;
    AXISINFO_STRU asixStru;
    asixStru.pos = Trapos;
    if(m_dbspeedparm.size() == 3)
    {
        ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd = QString::number(999999);
        qpStru.acc = m_dbspeedparm[0];
        qpStru.dec = m_dbspeedparm[1];
        asixStru.speed =float(m_dbspeedparm[2]*Traspeed)/100;
        qpStru.axisMap[m_axisName] = asixStru;
        RunAbsAsix(qpStru);
    }
    return;
}

void AxisOperater::RunQuickFixPos()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    StopPlugins();
    if(!JudgeAsixEvent() ||
            !ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    double Trapos = m_pDsb->value();
    uint Traspeed = m_pCbb->currentText().toUInt()/2;
    SWCONFIGXML_QPOSIT_STRU qpStru;
    qpStru.delayMsec = 5;
    qpStru.timeOut = 60000;
    qpStru.axisNum = 1;
    AXISINFO_STRU asixStru;
    asixStru.pos = Trapos;  
    if(m_dbspeedparm.size() == 3)
    {
        qpStru.acc = m_dbspeedparm[0];
        qpStru.dec = m_dbspeedparm[1];
        asixStru.speed =float(m_dbspeedparm[2]*Traspeed)/100;
        qpStru.axisMap[m_axisName] = asixStru;
        QPushButton * senderObj = qobject_cast<QPushButton *>(sender());
        if(senderObj->text() == "相对运动")
        {
            RunRelAsix(qpStru);
        }
        else
        {
            RunAbsAsix(qpStru);
        }
    }
}

void AxisOperater::RunStop()
{
    //运行结束
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    QThread::msleep(20);
    StopPlugins();
    ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd = QString::number( m_pDsb->maximum());
    ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart = QString::number(m_pDsb->minimum());
}

bool AxisOperater::JudgeAsixEvent()
{
    if(m_axisEventMap.contains(m_axisName))
    {
        StackManager st;
        if(!st.AnalysisEvent(m_axisEventMap[m_axisName].strEvent))
        {
            m_pLabel[1]->setText(m_axisEventMap[m_axisName].strNote);
            return false;
        }
    }
    return true;
}

bool AxisOperater::StopPlugins()
{
    if(m_OrgPlugin)
        m_OrgPlugin->isTerminate = true;
    if(m_PPPlugin)
        m_PPPlugin->isTerminate = true;
    if(m_RPPlugin)
        m_RPPlugin->isTerminate = true;
    return true;
}

void AxisOperater::RunAbsAsix(SWCONFIGXML_QPOSIT_STRU _ps)  //绝对定位
{
    if(m_PPPlugin && AttributeInstance::GetInstance()->Power <= m_power)
    {
        m_pLabel[1]->setText("执行PP...");
        m_PPPlugin->Initialization();
        SWCONFIGXML_STRU ps;
        ps.swXml_QP_Stru = _ps;

        m_PPPlugin->SetModuleParam(ps);
        m_PPPlugin-> RunModule();
        QString strMsg;
        m_PPPlugin->GetExcResult(strMsg);
        m_PPPlugin->StopModule();
        m_pLabel[1]->setText(strMsg);
    }
}

void AxisOperater::RunRelAsix(SWCONFIGXML_QPOSIT_STRU _ps)  //相对定位
{
    if(m_RPPlugin && AttributeInstance::GetInstance()->Power <= m_power)
    {
        m_pLabel[1]->setText("执行G02...");
        m_RPPlugin->Initialization();
        SWCONFIGXML_STRU ps;
        ps.swXml_QP_Stru = _ps;

        m_RPPlugin->SetModuleParam(ps);
        m_RPPlugin-> RunModule();
        QString strMsg;
        m_RPPlugin->GetExcResult(strMsg);
        m_RPPlugin->StopModule();
        m_pLabel[1]->setText(strMsg);
    }
}

bool AxisOperater::JudgeButtonIsExcEvent()
{
    bool enabled = false;
    if(m_press)
    {
        gettimeofday(&tpStart,NULL);
        m_press = false;
        enabled = true;
    }
    else {
        gettimeofday(&tpEnd,NULL);
        timeuse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeuse > 600) //设置600ms之后再响应
        {
            enabled = true;
            m_press = true;
            gettimeofday(&tpStart,NULL);
        }
    }
    return enabled;
}

void AxisOperater::CheckSonTimer()
{
    if(AttributeInstance::GetInstance()->Power <= m_power)
    {
        if(CoreGlobal::BaseAxisOperate::CheckAxisExcited(MyHW_Share::GetInstance()->m_INSON,2,m_stausBytePos))
        {
            m_pBtn[0]->setText("松励磁");
        }
        else
        {
            m_pBtn[0]->setText("励磁");
        }
        if(m_InbyteInfoMap.contains("Curpos"))
        {
            QString strCurpos = m_axisName +tr("_Curpos");
            int iCurpos = DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strCurpos].inputvalue;
            m_pCurPos->setText(QString("%1").arg(iCurpos/1000.0));
        }
    }
}
