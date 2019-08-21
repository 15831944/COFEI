/************************************
* @brief 摘要 调试界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef DEBUGFORM_H
#define DEBUGFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QFormLayout>
#include <QCheckBox>
#include <mainwindow.h>
#include "qcwindicatorlamp.h"
#include "cylinderlamp.h"
#include <funmoduleinterface.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include "calibrationui.h"
#include <QDebug>
#include "customtabwidget.h"
class debugForm : public QWidget
{
    Q_OBJECT
public:
    explicit debugForm(QVector<UIXML_STRU> vec,QWidget *parent = 0);
public:
    void Resize(QWidget * form,int x,int y =500);
private:
    void CreateUI();
signals:
    void SetAxisSerConfig(QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
    void SendUIEnableSig(bool);
    void SendChangeTimerSig(bool);
public slots:
    void ChangeTab(int index);
    void SetOutputData(QString,QString,int64_t);
    void SetCyOutputData(QMap<QString,uchar>);
    void ShowCalibrationUI(QVector<QString>, SWCONFIGXML_BASE_STRU *baseser, QMap<QString,int>, const QString, const QString &strType);
    void GetAxisSerConfig(QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp1,QMap<QString,SWCONFIGXML_LCSP_STRU> temp2)
    {
        emit SetAxisSerConfig(temp1,temp2);
    }
    void GetUIEnableSig(bool isenable)
    {
        for(auto it = m_LampIOMap.begin(); it != m_LampIOMap.end(); ++it)
        {
            it.value()->setEnabled(isenable);
        }
        for(int i = 0; i < m_CyLampVec.size(); ++i)
        {
            if(isenable)
            {
                if(m_curTabIndex == 2)
                    m_CyLampVec[i]->SetClickEnable(isenable);
            }
            else
               m_CyLampVec[i]->SetClickEnable(isenable);

        }
        emit SendUIEnableSig(isenable);
        m_playSig = isenable;
        if(isenable)
        {
            RefurbishIOStatus();
        }
    }
    void ChangeTimerSig(bool isenable)
    {
        emit SendChangeTimerSig(isenable);
        if(!isenable)
            RefurbishIOStatus();
    }
    void DisableQPBtn(bool isenale)
    {
        m_GoSig = isenale;
        if(m_cbUI != NULL)
        {
             m_cbUI->SetAxisUIonGObtn(m_GoSig);
        }
    }

private:
    void RefurbishIOStatus();
    int ShowAddressData(QString str,uchar &iValue);
private:
    bool m_GoSig;
    bool m_checkBoxIsCheck;
    bool m_playSig;
    QMap<QString,bool> stateMap;
    StackManager * m_stack;
    uchar m_curTabIndex;
private:
    QMap<QString,QcwIndicatorLamp*> m_LampIOMap;
    QVector<CylinderLamp *> m_CyLampVec;
    static QWidget *pe;
    QVector<UIXML_STRU> m_uivec;
    CalibrationUI *m_cbUI ;
    QGridLayout *m_grifLayout;
    typedef struct _MODELIOSTRU
    {
        QWidget *wgModel;
        QHBoxLayout *hBox;
        QVBoxLayout *vBox;
        QLabel *lable;
        QCheckBox *chb;
        QScrollArea *pArea;
        QWidget *wg;
        QGridLayout *baseLayout;
        void reWGsize(int iheight)
        {
            wgModel->setGeometry(0,0,pe->width()/3,iheight);
            wg->setGeometry(0,0,pe->width()/3,iheight);
        }

        _MODELIOSTRU()
        {
            wgModel = new QWidget();
            wgModel->setGeometry(0,0,pe->width()/6,pe->height()-20);
            wgModel->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
            vBox = new QVBoxLayout();
            wgModel->setLayout(vBox);

            hBox = new QHBoxLayout();
            lable = new QLabel();
            lable->setFixedSize(200,20);
            chb = new QCheckBox("归零设置");
            //chb->setStyleSheet("QCheckBox::indicator{border: 1px solid white;border-radius:2px;width:15px;height:15px;} ");
            hBox->addWidget(lable);
            hBox->addWidget(chb);
            vBox->addLayout(hBox);
            pArea = new QScrollArea();
            pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            pArea->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
            wg = new QWidget();
            wg->setGeometry(0,0,pe->width()/6,pe->height()-10);
            baseLayout = new QGridLayout();
            baseLayout->setSizeConstraint(QLayout::SetFixedSize);
            baseLayout->setSpacing(10);
            wg->setLayout(baseLayout);

            pArea->setWidget(wg);
            vBox->addWidget(pArea);
        }
    }MODELIOSTRU;
    QMap<QString,MODELIOSTRU> tempIOMap;// = QMap<QString,MODELIOSTRU>();
    QMap<QString,MODELIOSTRU> tempM00Map;
    CustomTabWidget *litsview;
};

#endif // DEBUGFORM_H
