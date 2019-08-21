#include "debugform.h"
#include "pushbutton.h"
#include "lampclick.h"
#include <QTextEdit>
#include <QtCore/qmath.h>
#include <QTimer>
#include "customtabwidget.h"
QWidget *debugForm::pe = NULL;
debugForm::debugForm(QVector<UIXML_STRU> vec,QWidget *parent) : QWidget(parent)
{
    pe = parent;
    m_uivec = vec;
    m_cbUI = NULL;
    m_curTabIndex = 0;
    m_LampIOMap.clear();
    m_CyLampVec.clear();
    m_checkBoxIsCheck = false;
    m_playSig = true;
    m_GoSig = true;//ongo按钮
    m_stack = new StackManager;
    CreateUI();

}
void debugForm::CreateUI()
{
     QTabWidget  *tabWig = new QTabWidget(this);
     tabWig->setGeometry(0,0,pe->width(),pe->height() - 32);
     connect(tabWig,&QTabWidget::currentChanged,this,&debugForm::ChangeTab);
    //Create IO
    QScrollArea *pAreaIO= new QScrollArea(this);
    QWidget *wgIO = new QWidget(this);
    QHBoxLayout *boxIO = new QHBoxLayout();
    wgIO->setLayout(boxIO);
    pAreaIO->setWidget(wgIO);//这里设置滚动窗口
    wgIO->setGeometry(0,0,pe->width()-10,pe->height());
    pAreaIO->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWig->addTab(pAreaIO,"IO部分");

    //Create M00
    QScrollArea *pAreaM00= new QScrollArea(this);
    QWidget *wgM00 = new QWidget(this);
    QHBoxLayout *boxM00 = new QHBoxLayout();
    wgM00->setLayout(boxM00);
    pAreaM00->setWidget(wgM00);//这里设置滚动窗口
    wgM00->setGeometry(0,0,pe->width()-10,pe->height());
    pAreaM00->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWig->addTab(pAreaM00,"气缸部分");
    //pAreaM00->setVisible(false);

    //Create Server
    QScrollArea *pAreaSer= new QScrollArea(this);
    QWidget *wgSer = new QWidget(this);
    m_grifLayout = new QGridLayout();
    wgSer->setLayout(m_grifLayout);
    wgSer->setGeometry(0,0,pe->width(),pe->height());
    pAreaSer->setWidget(wgSer);//这里设置滚动窗口
    tabWig->addTab(pAreaSer,"轴信息");
//    QVector<QString> vec;
//    m_cbUI = new CalibrationUI(vec,pe->width(),pe->height()/2);
//    boxSer->addWidget(m_cbUI);
    QStringList headerlist;
    headerlist << "定位类别" << "参数ID" << "注释" << "超时" << "单步延迟" << "轴ID" << "轴信息"<< "Run状态";
    litsview = new CustomTabWidget(headerlist,wgSer);
    litsview->setGeometry(0,0,pe->width(),pe->height()/3);
    connect(litsview,&CustomTabWidget::SendAxisSerConfig,this,&debugForm::GetAxisSerConfig);
    connect(litsview,&CustomTabWidget::UpdateCalibrationUI,this,&debugForm::ShowCalibrationUI);
    connect(this,&debugForm::SetAxisSerConfig,litsview,&CustomTabWidget::UpdataSerTabWidGet);
    connect(this,&debugForm::SendUIEnableSig,litsview,&CustomTabWidget::SetBtnStatus);
    connect(litsview,&CustomTabWidget::DiableQPBtn,this,&debugForm::DisableQPBtn);
    m_grifLayout->addWidget(litsview,1,0);

    QFont font;
    font.setPointSize(11);
    QFont font1;
    font1.setPointSize(13);
    font1.setFamily("黑体");
    tabWig->setFont(font1);
    tempIOMap = QMap<QString,MODELIOSTRU>();
    tempM00Map = QMap<QString,MODELIOSTRU>();
    for(int i(0);i < m_uivec.count();++i)
    {
        if(m_uivec[i].className == "Lamp")
        {
            if(!tempIOMap.contains(m_uivec[i].model)&&(m_uivec[i].signalType == "IO"))
            {
                MODELIOSTRU model;
                model.lable->setFont(font1);
                model.chb->setFont(font1);
                static int imodel = 0;
                if((++imodel) * pe->width()/3 >= pe->width()-20)
                {
                    wgIO->setGeometry(0,0,(imodel) *pe->width()/3,pe->height()-10);
                }
                model.lable->setText(QString("模组:%1").arg(m_uivec[i].model));
                boxIO->addWidget(model.wgModel);

                tempIOMap.insert(m_uivec[i].model,model);
            }
            else if(!tempM00Map.contains(m_uivec[i].model)&&(m_uivec[i].signalType == "M"))
            {
                MODELIOSTRU model;
                model.lable->setFont(font1);
                model.chb->setFont(font1);
                model.chb->setEnabled(false);
                static int imodel = 0;
                if((++imodel) * pe->width()/3 >= pe->width()-20)
                {
                    wgM00->setGeometry(0,0,(imodel) *pe->width()/3,pe->height()-10);
                }
                model.lable->setText(QString("模组:%1").arg(m_uivec[i].model));
                boxM00->addWidget(model.wgModel);

                tempM00Map.insert(m_uivec[i].model,model);
            }

            if(m_uivec[i].signalType == "IO")
            {
                QWidget *w = new QWidget();
                w->setFixedSize(pe->width()/3-10,m_uivec[i].csize.height());
                QcwIndicatorLamp *btn = new QcwIndicatorLamp(w);
                btn->m_strType = m_uivec[i].model;
                btn->m_strEvent = m_uivec[i].strEvent;
                btn->m_strName = m_uivec[i].address;
                m_LampIOMap.insert(m_uivec[i].address,btn);
                connect(btn,&QcwIndicatorLamp::SendOutputData,this,&debugForm::SetOutputData);
                btn->setFixedSize(m_uivec[i].csize);
                QLabel *l = new QLabel(w);
                l->setAlignment(Qt::AlignTop);
                l->move(btn->width()+10,-2);
                l->setFont(font);
                btn->setToolTip(m_uivec[i].strText);
                l->setText(m_uivec[i].strText);
                static int iQ = 0;
                if(m_uivec[i].address.toUpper().indexOf("Q") != -1)
                {
                    static uchar ic = 0;
                    tempIOMap[m_uivec[i].model].baseLayout->addWidget(w,ic++,0);
                    ++iQ;
                }

                int iheight = iQ * (m_uivec[i].csize.height());
                if(iheight > pe->height()-10)
                {
                    tempIOMap[m_uivec[i].model].reWGsize(iheight);
                    wgIO->setMinimumHeight(iheight);
                }
            }
            else if(m_uivec[i].signalType == "M")
            {
                if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_CY_Map.contains(m_uivec[i].address))
                {
                    QWidget *w = new QWidget();
                    w->setFixedSize(pe->width()/3-10,m_uivec[i].csize.height());
                    CylinderLamp *btn = new CylinderLamp(w);
                    m_CyLampVec.append(btn);
                    btn->SetCylampModel(m_uivec[i].model);
                    btn->SetProtectEvent(m_uivec[i].strEvent);
                    btn->SetCylampID(m_uivec[i].address);
                    btn->SetCylampStru(&ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_CY_Map[m_uivec[i].address]);
                    connect(btn,&CylinderLamp::SendCyOutputData,this,&debugForm::SetCyOutputData);
                    btn->setFixedSize(m_uivec[i].csize);
                    QLabel *l = new QLabel(w);
                    l->setAlignment(Qt::AlignTop);
                    l->move(btn->width()+10,-2);
                    l->setFont(font);
                    btn->setToolTip(m_uivec[i].strText);
                    l->setText(m_uivec[i].strText);
                    static int iQ = 0;
                    static uchar ic = 0;
                    tempM00Map[m_uivec[i].model].baseLayout->addWidget(w,ic++,0);
                    ++iQ;

                    int iheight = iQ * (m_uivec[i].csize.height());
                    if(iheight > pe->height()-10)
                    {
                        tempM00Map[m_uivec[i].model].reWGsize(iheight);
                        wgM00->setMinimumHeight(iheight);
                    }
                }
            }
        }
    }
}

void debugForm::ChangeTab(int index)
{
    if(index != 2)
        RefurbishIOStatus();
    m_curTabIndex = index;
    bool istrue = (index == 1);
    for(int i = 0; i < m_CyLampVec.size(); ++i)
    {
        m_CyLampVec[i]->SetClickEnable(istrue);
    }

}

void debugForm::SetOutputData(QString model, QString strName, int64_t value)
{
    uchar ivalue = 0;

    if(tempIOMap.contains(model))
    {
        if(!tempIOMap[model].chb->isChecked())
        {
            ivalue = value;
        }
    }
    else
    {
        ivalue = value;
    }

    BaseCalcFun::AnalysisIOString(strName,ivalue);
}

void debugForm::SetCyOutputData(QMap<QString, uchar> softIoMap)
{
    //CylinderLamp *qBtn = qobject_cast<CylinderLamp*>(sender());
    for(auto it = softIoMap.begin(); it != softIoMap.end();++it)
    {
        uchar ivalue = it.value();
        BaseCalcFun::AnalysisIOString(it.key(),ivalue);
    }
}


void debugForm::ShowCalibrationUI(QVector<QString> vec,SWCONFIGXML_BASE_STRU *baseser,QMap<QString,int> temp,const QString str,const QString &strType)
{
    if(m_cbUI != NULL)
    {
        delete m_cbUI;
        m_cbUI = NULL;
    }
    m_cbUI = new CalibrationUI(vec,pe->width(),pe->height()/2,strType);
    m_cbUI->GetStruData(baseser);
    m_cbUI->CreateUI(strType ,str);
    if(strType.compare("快速定位") == 0)
    {
        SWCONFIGXML_QPOSIT_STRU *ser = (SWCONFIGXML_QPOSIT_STRU*)baseser;
        QString tips = QString("%1%2    ID:%3  ").arg(strType).arg(str).arg(ser->id);
        m_cbUI->ChangetipsText(tips);
        m_cbUI->SetCheckDlgData(temp);
        m_cbUI->SetAxisUIPlayStatus(m_playSig);
        if(m_playSig)
        {
            m_cbUI->SetAxisUIonGObtn(m_GoSig);
        }//增加GO按钮启动之后的状态如果是FALSE   表格上go按钮点击
    }
    else if(strType.compare("直线插补") == 0)
    {

        SWCONFIGXML_LCSP_STRU *ser = (SWCONFIGXML_LCSP_STRU*)baseser;
        QString tips = QString("%1%2    ID:%3  ").arg(strType).arg(str).arg(ser->id);
        m_cbUI->ChangetipsText(tips);
        m_cbUI->SetAxisUIPlayStatus(m_playSig);


    }
    connect(m_cbUI,&CalibrationUI::SaveAxisSerConfig,this,&debugForm::GetAxisSerConfig);
    connect(this,&debugForm::SendUIEnableSig,m_cbUI,&CalibrationUI::GetUIEnableSig);
    connect(this,&debugForm::SendChangeTimerSig,m_cbUI,&CalibrationUI::GetChangeTimerSig);
    connect(m_cbUI,&CalibrationUI::rejected,litsview,&CustomTabWidget::reject);//新增关闭界面的时候的槽，把该样式表取消掉
    m_grifLayout->addWidget(m_cbUI,0,0);
}

void debugForm::RefurbishIOStatus()
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    for(auto it = m_LampIOMap.begin();it != m_LampIOMap.end();++it)
    {
        uchar uc = 0;
        ShowAddressData(it.key(),uc);
        if(uc == 0)
            it.value()->SetAlarm(false);
        else
            it.value()->SetAlarm(true);
    }
    for(int i = 0;i < m_CyLampVec.size();++i)
    {
        auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_CY_Map[m_CyLampVec[i]->GetCylampID()];
        if(it.soltCompVec.size() > 0)
        {
            uchar uc = 0;
            ShowAddressData(it.soltCompVec[0],uc);
            if(uc == 0)
                m_CyLampVec[i]->SetAlarm(false);
            else
                m_CyLampVec[i]->SetAlarm(true);
        }
    }
}

int debugForm::ShowAddressData(QString str, uchar &iValue)
{
    int iresult = 0;
    QStringList strList = str.split(".");
    if(strList.count() > 1)
    {
        QString strKey = strList[0];
        int bitPos = QString(strList[1]).toInt();
        if(ConfigData::GetInstance()->hwIOMap.contains(strKey))
        {
            int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;
            int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;
            if(iByteNum * 8 > bitPos)
            {
                int ibyte = bitPos /8;
                iBytePos += ibyte;
                bitPos = bitPos % 8;
                if(strKey.contains("Q"))
                {
                    uchar *TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
                    BaseCalcFun::MemCopyTempData(TempOutputData);
                    if(iBytePos < InOutPutData::GetInstance()->outPutNum)
                    {
                        iValue = (TempOutputData[iBytePos] >> bitPos)&1;
                    }
                }
                else
                {
                    iresult = -1;
                }
            }
        }
        else
           iresult = -1;
    }
    else
        iresult = -1;

    return iresult;
}

void debugForm::Resize(QWidget * form,int x,int y)
{
      form->setGeometry(0,0,x,y);//要显示的区域大小
}
