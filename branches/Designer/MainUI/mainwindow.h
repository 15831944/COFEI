#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <CoGraphics/Viewer/Viewer.h>
#include <CoGraphics/Scene/Scene.h>
#include <DynamicLib/MainFrame/mainframe.h>
#include <ec_control.h>
#include <QMessageBox>
#include <CoGraphicsItems/XML/XmlReader.h>
#include "ThreadObject.h"
#include <DycLibBase/TeachDeviceRunLib/teachdevicerunlib.h>
#include <checkdirection.h>
#include <checkabbitthread.h>
#include <DynamicLib/MainFrame/autoflowframe.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void            SendChartData(int Type , const QString,double date ,int ValueInt , QString Text = tr(""));
    void            Being_check();
    void            Being_get();
private slots:
    void            ShowLog(QString);
    void            SwithScene(const QString&,const QString&);
    void            SetFuntion(int ,QString);
    void            HandleErrorEvent(const ushort,const QString&);
    void            SafeSigs(const QString , const QString  ,const QString );
    void            HandleDB();
    void            HandleEcInputData();
    void            ReSetORGRunOK(const QString strkey,uchar uc, bool result);

    void            TimerDeleteFiles();
private:
    Ui::MainWindow  *ui;
    QGridLayout     *m_GridLayout;
    CoScene         *m_pCurScene; // 当前场景
    CoView          *m_pView;
    ThreadObject    *m_pThreadObj;
    MainFrame       *m_mainFrame;
   // AutoFlowFrame  *autoframe;
    QThread         *m_pThread;
    QTimer          *m_pDBTimer;//the timer to save DB
    QTimer          *m_pEcTimer;
    QLineEdit       *m_logEdit;
    QLabel          *m_comlabel;
    Ec_control      m_ec;
    bool            m_isEmStop;
    bool            m_isPlay;
    bool            m_ORGClicked;
    bool            m_isReset;
    bool            m_isNorStop;
    bool            m_isWritexml;
    int             m_curPower;
    QThread         *get_thread;
    QThread         *check_Thread;
    CheckDirection *check_Direction;
    CheckABbitThread *get_value;
    TeachDeviceRunLib *m_TeachObj;
    QThread          *m_TeachThread;
    bool             m_Teachmodel;
    QString          m_CurTeachPPID;

private:
    void            InitVariable();
    QWidget         *CreateShowArea(QWidget *parent);
    void            InitExcData();
    void            OpenUIXML();
    void            UpdateUI();
    void            SetORGBtnEnabel();
    void            CreateMsgBox(QMessageBox::Icon,const QString,const QString);

    //function
    void            PowerOff();
    void            Reboot();
    void            CloseUI();
    void            Play(const int);
    void            Stop(const int);
    void            EmrgStop(const int);
    void            ResetEvent(const int);
    void            ORGEvent(const int);
    void            SuspendOrResume(const int);
    //Handle
    void            HandleItems(const ushort);
    //Handle Cal-hand-mode 处理示教手轮模式
    void            HandModeChanged(char mode);
    void            HandModeParamChanged(const QString param);
    void            HandModeStateChanged(const char state);

    void            CreatMenu();
    void            Tcp_Connect();
    void            Connect_reset();
    void            Mqtt_Connect();
private:
    typedef struct _PROJECT_STRU
    {
        QMap<QString,CoScene*> sceneMap;
        QMap<QString,FLOW_STRU> flowMap;
        ushort viewWidth;
        ushort viewHeight;
        QString xmlPathFile;
        _PROJECT_STRU()
        {
            sceneMap.clear();
            flowMap.clear();
            viewWidth = 400;
            viewHeight = 300;
            xmlPathFile = "";
        }
        void Clear()
        {
            sceneMap.clear();
            flowMap.clear();
            viewWidth = 400;
            viewHeight = 300;
            xmlPathFile = "";
        }
    }PROJECT_STRU;
    PROJECT_STRU                        m_ProStru;
    QMap<QString,QMap<QString,bool>>    m_SafeMap;
    QMap<int ,QMap<int,CoWidgets*>>     m_FunctionBtnMap;

};

#endif // MAINWINDOW_H
