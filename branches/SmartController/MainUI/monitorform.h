/************************************
* @brief 摘要  监视界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef MONITORFORM_H
#define MONITORFORM_H

#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <QScrollArea>
#include <mainwindow.h>
#include <QGroupBox>
#include "qcwindicatorlamp.h"
#include <QTableView>
#include <QSqlTableModel>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QAbstractTableModel>
#include <QSqlQueryModel>
#include <my_talbview.h>
#include <databasemanager.h>
#include "prescripttabwidget.h"
#include "pushbutton.h"
class monitorForm : public QWidget
{
    Q_OBJECT
public:
    explicit monitorForm(QVector<UIXML_STRU> vec,QWidget *parent = 0);
    ~monitorForm();
signals:
private:
    QMenu* CreatQmenu();
    void AddData();
    void UpdateData();
    void DeleteData();
    bool CreateMessagebox(QString title, QString msg);
    void Save();
    void DiffCurTimeToDb(const uchar uc);
    int GetRTCTime();
    void UpdateDb(const QString, QStringList);
    typedef struct _DbBurnin
    {
        uint timesec = 0;
        uchar status = 0;
    }DbBurnin;
    QMap<QString,DbBurnin> m_dbBurninMap;
protected:
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void TimerUpInputData();
    void TimerStatus(bool);
    void GetUIEnableSig(bool);
    void TimerUpVData();
    void TimerUpTabData();
    void ChangedTab(int);
    void Rclicking(bool);
public:
    QVector<UIXML_STRU> m_uivec;
    QTimer *m_pTimer;
    //QTimer *m_pVTimer;
    QTimer *m_pTabTimer;
    QSqlTableModel *tabmodel;
    QTableView *tabview;
    int m_irow;
    int m_icolumn;
private:
     QTabWidget  *tabWig;
    QMap<QString,QcwIndicatorLamp*> m_LampIOMap;
    QMap<QString,QMap<QString,QVector<QcwIndicatorLamp*>>> m_LampSERMap;
    static QWidget *pe;
    QVector<PushButton*> m_pBtnVec;
    QFile *file;
    QDataStream *stream;
    QMutex m_mutex;
    typedef struct _MODELSTRU
    {
        bool isInfirst;
        bool isOutfirst;
        QWidget *wgModel;
        QVBoxLayout *vBox;
        QLabel *lable;
        QScrollArea *pAreaIn;
        QWidget *wgIn;
        QGridLayout *baseLayoutIn;

        QScrollArea *pAreaOut;
        QWidget *wgOut;
        QGridLayout *baseLayoutOut;
        _MODELSTRU()
        {
            wgModel = new QWidget();
            wgModel->setGeometry(0,0,pe->width()/100*98,pe->height()/2);
            wgModel->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
            vBox = new QVBoxLayout();
            wgModel->setLayout(vBox);
            lable = new QLabel();
            lable->setFixedSize(200,20);
            vBox->addWidget(lable);

            pAreaOut = new QScrollArea();
            pAreaOut->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            pAreaOut->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
            wgOut = new QWidget();
            wgOut->setGeometry(0,20,pe->width()/100*98-15,pe->height()/4-15);
            baseLayoutOut = new QGridLayout();
            baseLayoutOut->setSizeConstraint(QLayout::SetFixedSize);
            baseLayoutOut->setSpacing(10);
            wgOut->setLayout(baseLayoutOut);

            pAreaOut->setWidget(wgOut);
            vBox->addWidget(pAreaOut);

            pAreaIn = new QScrollArea();
            //pAreaIn->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            pAreaIn->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
            wgIn = new QWidget();
            wgIn->setGeometry(0,20,pe->width()/100*98-15,pe->height()/4-15);
            baseLayoutIn = new QGridLayout();
            baseLayoutIn->setSizeConstraint(QLayout::SetFixedSize);
            baseLayoutIn->setSpacing(10);
            wgIn->setLayout(baseLayoutIn);
            pAreaIn->setWidget(wgIn);
            vBox->addWidget(pAreaIn);
            isInfirst = true;
            isOutfirst = true;
        }
    }MODELSTRU;

    typedef struct _MODELIOSTRU
    {
        QWidget *wgModel;
        QVBoxLayout *vBox;
        QLabel *lable;

        QScrollArea *pAreaOut;
        QWidget *wgOut;
        QGridLayout *baseLayoutOut;
        void reWGsize(int iheight)
        {
            wgModel->setGeometry(0,0,pe->width()/3,iheight);
            wgOut->setGeometry(0,0,pe->width()/3,iheight);
        }

        _MODELIOSTRU()
        {
            wgModel = new QWidget();
            wgModel->setGeometry(0,0,pe->width()/3,pe->height()-20);
            wgModel->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
            vBox = new QVBoxLayout();
            wgModel->setLayout(vBox);
            lable = new QLabel();
            lable->setFixedSize(200,20);
            vBox->addWidget(lable);

            pAreaOut = new QScrollArea();
            pAreaOut->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            pAreaOut->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
            wgOut = new QWidget();
            //wgOut->setGeometry(0,0,pe->width()/3,pe->height()/3);
            wgOut->setGeometry(0,0,pe->width()/3,pe->height()-10);
            baseLayoutOut = new QGridLayout();
            baseLayoutOut->setSizeConstraint(QLayout::SetFixedSize);
            baseLayoutOut->setSpacing(6);
            wgOut->setLayout(baseLayoutOut);

            pAreaOut->setWidget(wgOut);
            vBox->addWidget(pAreaOut);
        }
    }MODELIOSTRU;

private:
    void CreateUI();
   // void CreateTabview();
    int ShowAddressData(QString str,QMap<QString,QVector<uchar>> &bitMap);
    int ShowAddressData(QString str,uchar &iValue)
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
                    else if(strKey.contains("E"))
                    {
                        QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);
                        if(iBytePos < InOutPutData::GetInstance()->inPutNum)
                        {
                            iValue = (InOutPutData::GetInstance()->inPutData[iBytePos] >> bitPos)&1;
                        }
                    }
                    else
                    {
                        iresult = -1;
                    }
                }


            }
        }
        return iresult;
    }
};

#endif // MONITORFORM_H
