#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QVector>
#include <QCoreApplication>
#include "flywidget.h"
#include "debugform.h"
#include <DynamicLib/MainFrame/mainframe.h>
#include <ec_control.h>
#include <DycLibBase/LogManager/logmanager.h>
#include <QDebug>
#include <QTableView>
#include <userlogin.h>
#include <myglwidget.h>
QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initLayout();
    void initToolBars();
    void initStatusBars();
    void initDockwidget();
    void initFunModelDockWidget();
    void initShowMainDockWidget();
    void initShowDebugDockWidget();
    void initShowMonitorDockWidget();
    void initShowConfigDockWidget();
    void initShowPhotoDockWidget();
    QChart *createBarChart() const;
    QChart *createPieChart() const;
    QChart *createLineChart();
    void initVariable();
    void showFlyWidget();
    void ErgodicLogDir();
    void ErgodicTorqueDir();
    int DateDiff(QDateTime dtStart, QDateTime dtEnd);
    bool DeleteFile(const QString filePath);
    bool CreateCommunicator();
private:
    Ui::MainWindow *ui;
    FlyWidget *m_FlyWidget;
    QDockWidget *showMainDockWidget;
    QDockWidget *showDebugDockWidget;
    QDockWidget *showMonitorDockWidget;
    QDockWidget *showConfigDockWidget;
    QDockWidget *showPhotoDockWidget;
    QLabel *timeLabel;
    QLineEdit *logEdit;
    MainFrame *m_mainFrame;
    Ec_control m_ec;
    QFont m_font;
    bool m_isLogStop;
    bool m_isEmStop;
    bool m_isNorStop;
    bool m_ORGClicked;
    bool m_isReset;
    QCheckBox *SingleCycle;
    QCheckBox *SigDebug;
    QLineEdit *Counter;
    QLineEdit *CounterT;
    QAction *clear;
    QMap<QString,QMap<QString,bool>> m_SafeMap;
private:
    //用于按钮保护的函数和数据结构
    QMap<QString,QAction*> ButtonProtectMap;
    void OpenUISetORGBtnTrue();
    void CheckErrorInRuning();
private:
    int screenWidth ;
    int screenHeight;
    QList<QChartView *> m_charts;
    QChart * m_Splinechart;
    QLineSeries *m_Splineserie;
    QListWidget  *m_listWidget;
    QListWidget  *m_listErrorWidget;
    QSet<QDockWidget *> m_dockWidgets;
    QMap<QString,QVector<UIXML_STRU>> uixmlMap;
    bool m_isPlay;
    MyGLWidget *m_glw;
    bool m_isWritexml;
signals:
     void ChangeMonitorTimerStatus(bool);
     void ChangeDebugTimerStatus(bool);
     void UpdateSwSerxmlEvent(QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
     void SwitchAuthor(const QString&);
     void SendUIEnableEvent(bool);
public slots:
    void ShowMainDockWindow();
    void ShowDebugDockWindow();
    void ShowMonitorDockWindow();
    void ShowConfigDockWindow();
    void ShowPhotoDockWindow();
    void receiveMessage(QString message);
    void ShowLog(QString strmsg);
    void SafeSigs(const QString strKey, const QString strType, const QString info);
private slots:
    void timerUpDate();
    void TimerDeleteFiles();
    void UpdateUi(QMap<QString,QVector<UIXML_STRU>> uiMap);
    void LogUpdate(const QString);
    void CloseUI();
    void Play();
    void Stop();
    void NorStop();
    void ResetEvent();
    void ORGEvent();
    void AuthorEvent();
    void ShowRunTime(double);
    void clearCounter();
    void ThreadPR();
    void ChangeSpeedRate(int);
    void  OnSingleCycleClicked();
    void  OnSingleDebugClicked();
    void WriteSwSerXml(QMap<QString, SWCONFIGXML_QPOSIT_STRU>, QMap<QString, SWCONFIGXML_LCSP_STRU>);
    void ReSetORGRunOK(const QString, uchar , bool);
    void on_actionVersion_Information_triggered();
};

#endif // MAINWINDOW_H
