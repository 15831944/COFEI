#include "axisinfoui.h"
#include "configform.h"
#include <DycLibBase/StackManager/stackmanager.h>
#include <QDebug>
MainORGThread::MainORGThread()
{
    m_orgLib = new PackLib;
    m_result = 1;
}
void MainORGThread::run()
{
    m_result = m_orgLib->RunAxis();
}
void MainORGThread::SetParam(SWCONFIGXML_ORG_AXIS_STRU orgAxis)
{
    m_orgLib->SetParam(orgAxis);
}

void MainORGThread::Stop(bool isNor)
{
    m_orgLib->Stop(isNor);
}

AxisInfoUI::AxisInfoUI(SWCONFIGXML_SER_MAP swSerMap,QWidget *parent) : QWidget(parent)
{
    m_swSerMap = swSerMap;
    m_ison = true;
    m_isORG = false;
    isInp = false;
    m_Rerun = false;
    m_iCurpos = 0;
    m_strOPName = "";

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
    m_ORGThread = NULL;
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timerUpInputData()));
    m_font.setPointSize(13);
    m_font.setFamily("黑体");
    QWidget *wg = new QWidget(this);
    wg->setStyleSheet(tr("background-color:rgb(35,35,35);color:rgb(166,167,166);border-radius:10px;"));
    for(int i = 0; i < 3;++i)
    {
        if(i<2)
        {
            m_pLable[i]   = new QLabel(wg);
            m_pLable[i]->setFont(m_font);
            m_pLable[i]->setFixedSize(96,32);
        }
        m_pHBox[i]       = new QHBoxLayout();
        m_pHBox[i]->setAlignment(Qt::AlignLeft);
    }
    m_pLable[2]   = new QLabel(wg);
    m_pLable[2]->setFont(m_font);
    m_pLable[2]->setFixedHeight(32);
    m_pHBox[3]       = new QHBoxLayout();
    m_pHBox[3]->setAlignment(Qt::AlignLeft);
    m_pVBox = new QVBoxLayout();
    wg->setLayout(m_pVBox);
    m_pCurPos = new QLineEdit("0",wg);
    m_pCurPos->setStyleSheet(tr("background-color:rgb(40,42,51);"));
    m_pCurPos->setAlignment(Qt::AlignCenter);
    m_pCurPos->setFixedSize(96,32);
    m_pCurPos->setFont(m_font);
    for(int i = 0; i < 9;++i)
    {
        m_pBtn[i] = new PushButton(wg);
        m_pBtn[i]->setFixedSize(96,32);
        m_pBtn[i]->setFont(m_font);
        m_pBtn[i]->setStatusImageNum(2);
    }
    m_pBtn[0]->getIconPath(":/images/ok");
    m_pBtn[0]->setStatusImageNum(1);
    m_pBtn[1]->getIconPath(":/images/ORG");
    m_pBtn[2]->getIconPath(":/images/son");
    m_pBtn[2]->setText("S_ON");
    m_pBtn[3]->getIconPath(":/images/reset");
    m_pBtn[4]->getIconPath(":/images/left");
    m_pBtn[5]->getIconPath(":/images/right");
    m_pBtn[6]->getIconPath(":/images/FixPos");
    m_pBtn[7]->getIconPath(":/images/stop");
    /****/
    m_pBtn[8]->setText("RelMov");//******
    m_pBtn[8]->setTextSpacing(15);
    m_pBtn[8]->getIconPath(":/images/FixPos");

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
    connect(m_pBtn[1],&PushButton::clicked,this,&AxisInfoUI::RunORG);
    connect(m_pBtn[2],&PushButton::clicked,this,&AxisInfoUI::RunSon);
    connect(m_pBtn[3],&PushButton::clicked,this,&AxisInfoUI::RunReSet);
    connect(m_pBtn[4],&PushButton::pressed,this,&AxisInfoUI::RunLeft);
    connect(m_pBtn[4],&PushButton::released,this,&AxisInfoUI::RunStop);
    connect(m_pBtn[5],&PushButton::pressed,this,&AxisInfoUI::RunRight);
    connect(m_pBtn[5],&PushButton::released,this,&AxisInfoUI::RunStop);
    connect(m_pBtn[6],&PushButton::clicked,this,&AxisInfoUI::RunQuickFixPos);
    connect(m_pBtn[7],&PushButton::clicked,this,&AxisInfoUI::RunStop);
    connect(m_pBtn[8],&PushButton::clicked,this,&AxisInfoUI::RunQuickFixPos);
    this->setFixedSize(420,200);//......
    return;
}

AxisInfoUI::~AxisInfoUI()
{
    if(m_pTimer!= NULL)
        m_pTimer->stop();
}
void AxisInfoUI::CreatAsixUI()
{
    m_pHBox[3]->addWidget(m_pLable[0]);
    m_pHBox[3]->addWidget(m_pLable[2]);
    m_pVBox->addLayout(m_pHBox[3]);
    for(int i = 0;i < 4;++i)
    {
        m_pHBox[0]->addWidget(m_pBtn[i]);
    }
    m_pBtn[0]->setText("run ok");
    m_pBtn[0]->setTextSpacing(15);
    m_pLable[1]->setText("速度:");
    QStringList list;
    list << "1" << "5" << "10" << "15" << "20" << "25" << "30" << "40" << "50" << "60" << "70" << "80" << "90" << "100";
    m_pCbb->addItems(list);
    m_pCurPos->setDisabled(true);
    m_pBtn[4]->setFixedSize(48,32);
    m_pBtn[5]->setFixedSize(48,32);
    m_pHBox[1]->addWidget(m_pLable[1]);
    m_pHBox[1]->addWidget(m_pCbb);
    m_pHBox[1]->addWidget(m_pBtn[4]);
    m_pHBox[1]->addWidget(m_pCurPos);
    m_pHBox[1]->addWidget(m_pBtn[5]);

    m_pBtn[6]->setText("FixPos");
    m_pBtn[6]->setTextSpacing(15);
     m_pHBox[2]->addWidget(m_pBtn[8]);//新增修改目标位置删除更改为 相对运动按钮
    m_pHBox[2]->addWidget(m_pDsb);
    m_pHBox[2]->addWidget(m_pBtn[6]);
    m_pHBox[2]->addWidget(m_pBtn[7]);
    m_pHBox[4]       = new QHBoxLayout();
    m_pHBox[4]->setAlignment(Qt::AlignLeft);
    m_pVBox->addLayout(m_pHBox[0]);
    m_pVBox->addLayout(m_pHBox[1]);
    m_pVBox->addLayout(m_pHBox[2]);
}
void AxisInfoUI::SetAxisName(const QString strName)
{
    m_pLable[0]->setText("轴 " + strName);
    m_axisName = strName;
    m_contrlBytePos = AnalysisHwConfig::GetContrlWorldPos(strName,m_contrlBitInfoMap,m_byteInfoMap);
    m_stausBytePos = AnalysisHwConfig::GetStatusWorldPos(strName,m_stausBitInfoMap,m_byteInfoMap);
    axisEventMap = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_ProtectEvent_Map.axisEventMap;
    CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_byteInfoMap["Model"].strType,m_byteInfoMap["Model"].byteNum,m_byteInfoMap["Model"].bytePos,1);
    QThread::msleep(20);
}
void AxisInfoUI::SetAxisRanage(const double dstart,const double dend)
{
    m_pDsb->setRange(dstart,dend);
}

QString AxisInfoUI::GetAxisName()
{
    return m_pLable[0]->text().replace("轴 ","");
}
double AxisInfoUI::GetTraPos()
{
    return m_pCurPos->text().toDouble();
}

uint  AxisInfoUI::GetTraSpeed()
{
    return m_pCbb->currentText().toUInt();
}
void AxisInfoUI::SetTraPos(double d)
{
    m_pCurPos->setText(QString("%1").arg(d));
}

void AxisInfoUI::RunORG()
{
    if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map.contains(m_axisName))
    {
        m_isORG = true;
        if(m_ORGThread == NULL)
        {
            m_ORGThread = new MainORGThread;
            m_ORGThread->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[m_axisName]);
            m_ORGThread->start();
        }
        else
        {
            m_ORGThread->m_result = 1;
            m_ORGThread->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[m_axisName]);
            m_ORGThread->start();
        }
        for(int i(1);i < 9;++i)
        {
            if(i != 7 )
            {
                m_pBtn[i]->setEnabled(false);
            }
        }
        m_pLable[2]->setText("执行回原点...");
    }
}

void AxisInfoUI::RunSon()
{
    if(m_contrlBytePos == -1) return;
    CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_byteInfoMap["Model"].strType,m_byteInfoMap["Model"].byteNum,m_byteInfoMap["Model"].bytePos,1);
    QThread::msleep(20);
    CoreGlobal::BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,m_contrlBytePos);
    QThread::msleep(20);
    if(m_pBtn[2]->text() == "S_ON")
    {
        m_pBtn[2]->setText("S_OFF");
        CoreGlobal::BaseAxisOperate::SetAxisExcite(m_tempOUTSON,m_contrlBytePos);
    }
    else
    {
        m_pBtn[2]->setText("S_ON");
    }
    m_ison = !m_ison;
}

void AxisInfoUI::RunReSet()
{
    if(m_contrlBytePos == -1) return;
    if(m_contrlBitInfoMap.contains("Reset"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisReset(m_contrlBitInfoMap["Reset"].bit,m_contrlBytePos);
    }
}

void AxisInfoUI::RunLeft()
{
    if(!JudgeAsixEvent())
        return;
    for(int i(1);i < 9;++i)
    {
        if(i != 4)
            m_pBtn[i]->setEnabled(false);
    }
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    uint Traspeed = ConfigData::GetInstance()->iSpeed*m_pCbb->currentText().toUInt()/200.0 ;
    uint accel = ConfigData::GetInstance()->iAccel *m_pCbb->currentText().toUInt()/200.0 ;
    uint dccel = ConfigData::GetInstance()->iDccel *m_pCbb->currentText().toUInt()/200.0 ;
    int imodel = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].modelType.toInt();
    int Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart.toDouble()*1000;
    Traspeed *=  ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].lead;
    dccel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    accel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    if(m_byteInfoMap.contains("Model"))
    {
        CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_byteInfoMap["Model"].strType,m_byteInfoMap["Model"].byteNum,m_byteInfoMap["Model"].bytePos,imodel);
    }
    if(m_byteInfoMap.contains("Trapos"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisPos(m_byteInfoMap["Trapos"].strType,m_byteInfoMap["Trapos"].byteNum,m_byteInfoMap["Trapos"].bytePos,Trapos);
    }
    if(m_byteInfoMap.contains("Traspeed"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisSpeed(m_byteInfoMap["Traspeed"].strType,m_byteInfoMap["Traspeed"].byteNum,m_byteInfoMap["Traspeed"].bytePos,Traspeed);
    }
    if(m_byteInfoMap.contains("Accel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisACC(m_byteInfoMap["Accel"].strType,m_byteInfoMap["Accel"].byteNum,m_byteInfoMap["Accel"].bytePos,accel);
    }
    if(m_byteInfoMap.contains("Decel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisDCC(m_byteInfoMap["Decel"].strType,m_byteInfoMap["Decel"].byteNum,m_byteInfoMap["Decel"].bytePos,dccel);
    }
    if(m_contrlBytePos == -1) return;
    m_strOPName = "-";
    if(m_ORGThread != NULL)
        m_ORGThread->m_result = 1;
    if(m_contrlBitInfoMap.contains("Run"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisRun(m_contrlBitInfoMap["Run"].bit,m_contrlBytePos);
    }
    return;
}

void AxisInfoUI::RunRight()
{
    if(!JudgeAsixEvent())
        return;
    for(int i(1);i < 9;++i)
    {
        if(i != 5)
            m_pBtn[i]->setEnabled(false);
    }
    uint Traspeed = m_pCbb->currentText().toUInt()/200.0 *ConfigData::GetInstance()->iSpeed;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    uint accel = ConfigData::GetInstance()->iAccel *m_pCbb->currentText().toUInt() /200.0;
    uint dccel = ConfigData::GetInstance()->iDccel *m_pCbb->currentText().toUInt() /200.0;
    int imodel = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].modelType.toInt();
    int Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd.toDouble()*1000;
    Traspeed *=  ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].lead;
    dccel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    accel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    if(m_byteInfoMap.contains("Model"))
    {
        CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_byteInfoMap["Model"].strType,m_byteInfoMap["Model"].byteNum,m_byteInfoMap["Model"].bytePos,imodel);
    }
    if(m_byteInfoMap.contains("Trapos"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisPos(m_byteInfoMap["Trapos"].strType,m_byteInfoMap["Trapos"].byteNum,m_byteInfoMap["Trapos"].bytePos,Trapos);
    }
    if(m_byteInfoMap.contains("Traspeed"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisSpeed(m_byteInfoMap["Traspeed"].strType,m_byteInfoMap["Traspeed"].byteNum,m_byteInfoMap["Traspeed"].bytePos,Traspeed);
    }
    if(m_byteInfoMap.contains("Accel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisACC(m_byteInfoMap["Accel"].strType,m_byteInfoMap["Accel"].byteNum,m_byteInfoMap["Accel"].bytePos,accel);
    }
    if(m_byteInfoMap.contains("Decel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisDCC(m_byteInfoMap["Decel"].strType,m_byteInfoMap["Decel"].byteNum,m_byteInfoMap["Decel"].bytePos,dccel);
    }
    if(m_contrlBytePos == -1) return;
    m_strOPName = "+";
    if(m_ORGThread != NULL)
        m_ORGThread->m_result = 1;
    if(m_contrlBitInfoMap.contains("Run"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisRun(m_contrlBitInfoMap["Run"].bit,m_contrlBytePos);
    }
    return;
}

void AxisInfoUI::RunQuickFixPos()
{
    /***************************新增相对运动部分*********/
    QPushButton * senderObj = qobject_cast<QPushButton *>(sender());
    if(senderObj->text() == "RelMov")
    {
	if(!m_byteInfoMap.contains("Trapos"))
        {
            return;
        }
        RelativeChange(1);
        m_Rerun = true;
        m_iCurpos = CoreGlobal::BaseAxisOperate::GetAxisCurPos(m_byteInfoMap["Curpos"].strType,m_byteInfoMap["Curpos"].byteNum,m_byteInfoMap["Curpos"].bytePos);
	CoreGlobal::BaseAxisOperate::SetAxisPos(m_byteInfoMap["Trapos"].strType,m_byteInfoMap["Trapos"].byteNum,m_byteInfoMap["Trapos"].bytePos,m_iCurpos);    
    }
    else
    {
        RelativeChange(0);
        isInp = true;
    }
    if(!JudgeAsixEvent())
        return;
    for(int i(1);i < 9;++i)
    {
        if( i != 7)
            m_pBtn[i]->setEnabled(false);
    }
    int Trapos = m_pDsb->value() * 1000;
    if(Trapos > ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd.toDouble()*1000)
        Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitEnd.toDouble()*1000;
    if(Trapos < ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart.toDouble()*1000)
        Trapos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].swLimitStart.toDouble()*1000;
    int iCurpos = 0;
    if(m_byteInfoMap.contains("Curpos"))
    {
        iCurpos = CoreGlobal::BaseAxisOperate::GetAxisCurPos(m_byteInfoMap["Curpos"].strType,m_byteInfoMap["Curpos"].byteNum,m_byteInfoMap["Curpos"].bytePos);
    }
    if(Trapos > iCurpos)
        m_strOPName = "+";
    else
        m_strOPName = "-";
    if(m_ORGThread != NULL)
        m_ORGThread->m_result = 1;
    m_pDsb->setValue(Trapos/1000.0);
    uint Traspeed = m_pCbb->currentText().toUInt()*ConfigData::GetInstance()->iSpeed/200.0;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(m_axisName))
        return;
    uint accel = ConfigData::GetInstance()->iAccel *m_pCbb->currentText().toUInt() /200.0;
    uint dccel = ConfigData::GetInstance()->iDccel *m_pCbb->currentText().toUInt() /200.0;
    int imodel = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].modelType.toInt();
    Traspeed *=  ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].lead;
    dccel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    accel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[m_axisName].adled;
    if(m_byteInfoMap.contains("Model"))
    {
        CoreGlobal::BaseAxisOperate::ChangeAxisModel(m_byteInfoMap["Model"].strType,m_byteInfoMap["Model"].byteNum,m_byteInfoMap["Model"].bytePos,imodel);
    }
    if(m_byteInfoMap.contains("Trapos"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisPos(m_byteInfoMap["Trapos"].strType,m_byteInfoMap["Trapos"].byteNum,m_byteInfoMap["Trapos"].bytePos,Trapos);
    }
    if(m_byteInfoMap.contains("Traspeed"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisSpeed(m_byteInfoMap["Traspeed"].strType,m_byteInfoMap["Traspeed"].byteNum,m_byteInfoMap["Traspeed"].bytePos,Traspeed);
    }
    if(m_byteInfoMap.contains("Accel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisACC(m_byteInfoMap["Accel"].strType,m_byteInfoMap["Accel"].byteNum,m_byteInfoMap["Accel"].bytePos,accel);
    }
    if(m_byteInfoMap.contains("Decel"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisDCC(m_byteInfoMap["Decel"].strType,m_byteInfoMap["Decel"].byteNum,m_byteInfoMap["Decel"].bytePos,dccel);
    }
    if(m_contrlBytePos == -1) return;
    if(m_contrlBitInfoMap.contains("Run"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisRun(m_contrlBitInfoMap["Run"].bit,m_contrlBytePos);
    }
}

void AxisInfoUI::RunStop()
{
    if(!m_pLable[2]->text().contains("回原点失败") || !m_pLable[2]->text().contains("回原点成功"))
        m_pLable[2]->setText("");
    if(m_contrlBytePos == -1) return;
    if(m_contrlBitInfoMap.contains("Stop"))
    {
        CoreGlobal::BaseAxisOperate::SetAxisStop(m_contrlBitInfoMap["Stop"].bit,m_contrlBytePos);
    }
    if(m_ORGThread != NULL)
    {
        m_ORGThread->Stop();
        m_ORGThread->quit();
        m_ORGThread->wait();
        m_ORGThread->m_result = 1;
    }
    for(int i(1);i < 9;++i)
    {
        m_pBtn[i]->setEnabled(true);
    }
    m_isORG = false;
    RelativeChange(0);
    m_Rerun = false;
    isInp = false;
}
void AxisInfoUI::TimerStatus(bool isStop)
{
    if(isStop)
    {
        m_pTimer->stop();
    }
    else
        m_pTimer->start(50);
}

bool AxisInfoUI::JudeBtnisenble()
{
    bool enble = true;
    for(int i(1);i < 9;++i)
    {
        if(!m_pBtn[i]->isEnabled())
        {
            enble = false;
            break;
        }
    }
    return enble;
}
void AxisInfoUI::timerUpInputData()
{
    if(m_ORGThread != NULL &&m_ORGThread->m_result != 1)
    {
        if(!m_pBtn[1]->isEnabled())
        {
            if(m_ORGThread->m_result != 0)
                m_pLable[2]->setText(QString("回原点失败 %1").arg(m_ORGThread->m_result));
            else
                m_pLable[2]->setText("回原点成功");
            m_isORG = false;
        }
        for(int i(1);i <9;++i)
        {
            m_pBtn[i]->setEnabled(true);
        }
    }
    if(InOutPutData::GetInstance()->inPutData==NULL)// 关闭按钮时候
    {
        return;
    }
    if(m_tempINSON.count() > 3)
    {
        if(CoreGlobal::BaseAxisOperate::CheckAxisExcited(m_tempINSON,2,m_stausBytePos))
        {
            m_pBtn[2]->setText("S_OFF");
        }
        else
        {
            m_pBtn[2]->setText("S_ON");
        }
    }
    if(!m_isORG)
    {
        QString strMsg = "";
        if(!JudgeHWLimit(m_axisName,m_strOPName,strMsg))
        {
            m_pLable[2]->setText(strMsg);
            RunStop();
        }
    }
    if(m_stausBitInfoMap.contains("Fault"))
    {
        if(CoreGlobal::BaseAxisOperate::CheckAxisFault(m_stausBitInfoMap["Fault"].bit,2,m_stausBytePos))
        {
            if(m_byteInfoMap.contains("Errorcode"))
            {
                int ierror = CoreGlobal::BaseAxisOperate::GetAxisErrorCode(m_byteInfoMap["Errorcode"].strType,m_byteInfoMap["Errorcode"].byteNum,m_byteInfoMap["Errorcode"].bytePos);
                m_pBtn[0]->setText(QString("0x%1").arg(QString::number(ierror,16)));
                m_pBtn[0]->getIconPath(":/images/error");
            }
        }
        else
        {
            if(m_byteInfoMap.contains("Curpos"))
            {

                int iCurpos = CoreGlobal::BaseAxisOperate::GetAxisCurPos(m_byteInfoMap["Curpos"].strType,m_byteInfoMap["Curpos"].byteNum,m_byteInfoMap["Curpos"].bytePos);
                m_pCurPos->setText(QString("%1").arg(iCurpos/1000.0));
                if(isInp && qAbs(iCurpos/1000.0-m_pDsb->value()) < 0.5)
                {
                    isInp = false;
                    for(int i(1);i < 9;++i)
                    {
                        m_pBtn[i]->setEnabled(true);
                    }
                }
                //新增相对运动按钮需要判断相对位置已经结束需要新增
                if(m_Rerun && BaseAxisOperate::CheckAxisINP(10,2,m_stausBytePos))//判断到位信号
                {
                    m_Rerun = false;
                    for(int i(1);i < 9;++i)
                    {
                        m_pBtn[i]->setEnabled(true);
                    }
                    RelativeChange(0);//bit 6置0
                }
            }
            //todo errorBtn is Ok
            m_pBtn[0]->getIconPath(":/images/ok");
            m_pBtn[0]->setText(QString(""));
        }
    }
}
bool AxisInfoUI::JudgeHWLimit(QString strKey,QString strOPName,QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
    if(!str.isEmpty() && strOPName == "-")
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
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
    if(!str.isEmpty() && strOPName == "+")
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

    return true;
}
void AxisInfoUI::DisableBtn(bool isenable)
{
    for(int i = 1; i < 9 ; ++i)
    {
        if(m_pBtn[i] != NULL)
            m_pBtn[i]->setEnabled(isenable);
    }
}
///
/// \brief AxisInfoUI::RelativeChange
///新增bit位6的改变
void AxisInfoUI::RelativeChange(int bit)
{
    BaseCalcFun::SetBitValue(6,m_contrlBytePos,bit);
    QThread::msleep(50);
}
bool AxisInfoUI::JudgeAsixEvent()
{
    if(axisEventMap.contains(m_axisName))
    {
        StackManager st;
        if(!st.AnalysisEvent(axisEventMap[m_axisName].strEvent))
        {
            m_pLable[2]->setText(axisEventMap[m_axisName].strNote);
            return false;
        }
    }
    return true;
}
