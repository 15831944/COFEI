#include "CoGraphicsItems/AttributeInstance.h"
#include "TeachingDevice/share_value.h"
#include "HandMode.h"

HandMode::HandMode(QWidget *parent) : QWidget(parent)
{
    m_strPosName = "";
    m_strAxis = "";
    m_strPos = "";
    m_AxisRangeMap.clear();
    m_TraPosLblList.clear();
    this->setStyleSheet(tr("QWidget{background-color:rgb(35,35,35);color:rgb(166,167,166);border-radius:10px;}"
                           "QLineEdit,QPushButton,QComboBox{background-color:rgb(40,42,51);}"
                           "QPushButton::hover{background-color:rgb(50, 50, 55);}"
                           "QPushButton::pressed{background-color:rgb(20, 20, 25);}"));

    m_font.setPointSize(12);
    m_font.setFamily("黑体");
    QWidget *wg = new QWidget(this);
    wg->setFixedSize(400,190);
    this->setFixedSize(420,200);
    QHBoxLayout *pHbox = new QHBoxLayout();
    m_CbbRatio = new QComboBox();
    m_CbbRatio->setObjectName(tr("ratio"));
    m_CbbRatio->setFont(m_font);
    QStringList list;
    list.clear();
    list << "" << "10" << "50" << "100" << "500" << "1000";
    m_CbbRatio->addItems(list);
    m_CbbAxis = new QComboBox();
    m_CbbAxis->setObjectName(tr("axis"));
    m_CbbAxis->setFont(m_font);
    connect(m_CbbRatio,SIGNAL(currentIndexChanged(QString)),this,SLOT(CbbIndexChanged(QString)));
    connect(m_CbbAxis,SIGNAL(currentIndexChanged(QString)),this,SLOT(CbbIndexChanged(QString)));
    QLabel*lblAxis = new QLabel(tr("轴:"));
    lblAxis->setFont(m_font);
    QLabel*lblRatio = new QLabel(tr("倍率:"));
    lblRatio->setFont(m_font);
    m_BtnReset = new QPushButton(tr("复位"));
    m_BtnReset->setFont(m_font);
    m_BtnReset->setFixedSize(96,32);
    connect(m_BtnReset,&QPushButton::clicked,this,&HandMode::RunReset);
    pHbox->addWidget(lblAxis);
    pHbox->addWidget(m_CbbAxis);
    pHbox->addSpacing(10);
    pHbox->addWidget(lblRatio);
    pHbox->addWidget(m_CbbRatio);
    pHbox->addWidget(m_BtnReset);
    m_Grid = new QGridLayout();
    m_Grid->setColumnStretch(0,1);
    m_Grid->setColumnStretch(1,6);
    m_Grid->setSpacing(10);
    QVBoxLayout *pVbox = new QVBoxLayout(wg);
    pVbox->addLayout(pHbox);
    pVbox->addLayout(m_Grid);
    pVbox->addStretch(1);
    wg->setLayout(pVbox);
    InitValue();
    m_TDRPluge = new TeachDeviceRunLib();

    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&HandMode::CheckPosTimer);
}

HandMode::~HandMode()
{
    m_timer->stop();
}

void HandMode::setPosInfo(const QString posInfo)
{
    QStringList info = posInfo.split(';');
    if(info.size() < 2)
        return;

    m_strPosName = info.at(0);
    m_strAxis = info.at(1);

    //获取轴运动范围
    QStringList list = m_strAxis.split(',');
    QVector<QString> axisVec;
    axisVec.clear();
    for(int i = 0; i < list.size(); i++)
    {
        ConfigData::GetInstance()->m_mutex.lock();
        if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(list[i]))
        {
            ConfigData::GetInstance()->m_mutex.unlock();
            continue ;
        }
        QString start = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[list[i]].swLimitStart;
        QString end = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[list[i]].swLimitEnd;
        ConfigData::GetInstance()->m_mutex.unlock();
        QStringList range;
        range << start << end;
        m_AxisRangeMap.insert(list[i],range);
        axisVec.append(list.at(i));
    }
    //获取轴位置
    if(m_TDRPluge)
    {
        m_TDRPluge->InitJointsBytePosParam(axisVec);
        for(auto it = axisVec.begin(); it != axisVec.end();)
        {
            int curpos = -99999999;
            m_TDRPluge->GetJointCurPos(*it,curpos);
            m_strPos += QString::number(curpos);
            ++it;
            if(it != axisVec.end())
                m_strPos += ",";
        }
    }
    m_timer->start(300);
}

void HandMode::SetPower(int power)
{
    m_power = power;
}

void HandMode::InitValue()
{
    if(m_strAxis.isEmpty() || m_strPos.isEmpty())
        return;

    QStringList axis = m_strAxis.split(',');
    QStringList pos = m_strPos.split(',');

    m_CbbAxis->clear();
    m_CbbAxis->addItems(axis);
    QLayoutItem *Litem;
    while((Litem = m_Grid->takeAt(0)) != 0)
    {
        if(Litem->widget()){
            delete Litem->widget();
        }
        delete Litem;
    }
    //布局，增加轴位置
    for(int i = 0; i < pos.size(); i++)
    {
        QLabel *lblAxis = new QLabel(tr("%1:").arg(axis.at(i)));
        lblAxis->setObjectName(axis.at(i));
        lblAxis->setFont(m_font);
        QLabel *lblPos = new QLabel(tr("%1").arg(pos.at(i)));
        lblPos->setObjectName(tr("pos_%1").arg(axis.at(i)));
        lblPos->setFont(m_font);
        m_Grid->addWidget(lblAxis,i,0);
        m_Grid->addWidget(lblPos,i,1);
    }
}

QString HandMode::GetCurPos() const
{
    QString pos = "";
    for(int i = 0; i < m_CbbAxis->count();i++)
    {
        QString text = "pos_" + m_CbbAxis->itemText(i);
        for(int i = 0; i < m_Grid->count();i++)
        {
            QWidget* wg = m_Grid->itemAt(i)->widget();
            if(wg->objectName() == text)
            {
                QLabel *lbl = qobject_cast<QLabel*>(wg);
                pos = pos + "," + lbl->text();
                break;
            }
        }
    }
    if(pos.left(1) == ",")
        pos = pos.right(pos.length()-1);
    return pos;
}

void HandMode::CheckPosTimer()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    if(m_TDRPluge)
    {
        //判断驱动器故障
        int errorcode = -99999;
        int result = m_TDRPluge->CheckJointFault(m_CbbAxis->currentText(),errorcode);
        if(result == -1)
        {
            //驱动器故障
            emit SendStateEvent(-1,QString::number(errorcode));
        }

        //读取当前轴的位置
        int curPos = -99999;
        m_TDRPluge->GetJointCurPos(m_CbbAxis->currentText(),curPos);

        //判断轴位置是否超出范围
        QStringList range = m_AxisRangeMap.value(m_CbbAxis->currentText());
        double start = range.at(0).toDouble()*1000;
        double end = range.at(1).toDouble()*1000;
        if(curPos < start || curPos > end)
        {
            emit SendStateEvent(0,""); //发送停止信号，轴停止运动

            if((curPos > 0 && ShareClass::GetIstance()->m_cruntValue < 0)
                    ||(curPos < 0 && ShareClass::GetIstance()->m_cruntValue > 0))
                emit SendStateEvent(1,""); //超出极限后手轮反转，轴运行
        }

        //显示当前轴位置
        QString text = "pos_" + m_CbbAxis->currentText();
        for(int i = 0; i < m_Grid->count(); i++)
        {
            QWidget *wg = m_Grid->itemAt(i)->widget();
            if(wg->objectName() == text)
            {
                QLabel *lbl = qobject_cast<QLabel*>(wg);
                lbl->setText(QString::number(((double)curPos)/1000));
                break;
            }
        }
    }
}

void HandMode::CbbIndexChanged(const QString &text)
{
    QComboBox *cbb = qobject_cast<QComboBox*>(sender());
    QString param = "";
    if(cbb->objectName() == tr("ratio"))
        param = tr("%1,%2,%3").arg(m_strPosName).arg(m_CbbAxis->currentText()).arg(text);
    else
        param = tr("%1,%2,%3").arg(m_strPosName).arg(text).arg(m_CbbRatio->currentText());

    emit SendHandParamEvent(param);
}

void HandMode::RunReset()
{
    if(AttributeInstance::GetInstance()->Power > m_power)
        return;
    if(m_TDRPluge)
    {
        m_TDRPluge->SetJointReset(QVector<QString>(1,m_CbbAxis->currentText()));
    }
}
