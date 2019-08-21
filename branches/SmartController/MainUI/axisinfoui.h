/************************************
* @brief 摘要  调试界面校准加界面下轴的界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef AXISINFOUI_H
#define AXISINFOUI_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "pushbutton.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "analysishwconfig.h"
#include <QTimer>
#include <DycLibBase/PackLib/packlib.h>
class MainORGThread : public QThread
{
public:
    MainORGThread();
protected:
    void run();
private:
    PackLib * m_orgLib;
public:
    void SetParam(SWCONFIGXML_ORG_AXIS_STRU);
    void Stop(bool isNor = true);
    int m_result;
};

class AxisInfoUI : public QWidget
{
    Q_OBJECT
public:
    explicit AxisInfoUI(SWCONFIGXML_SER_MAP swSerMap,QWidget *parent = 0);
    ~AxisInfoUI();
public:
    void SetAxisName(const QString strName);
    void SetAxisRanage(const double dstart,const double dend);
    QString GetAxisName();
    double GetTraPos();
    void SetTraPos(double d);
    uint  GetTraSpeed();
    void TimerStatus(bool isStop);
    bool JudeBtnisenble();//增加判断轴按钮可用部分
private:
    QFont m_font;
    QLabel *m_pLable[3];
    PushButton *m_pBtn[9];
    bool isInp;
    bool m_Rerun;
     int m_iCurpos ;
    QComboBox *m_pCbb;
    QDoubleSpinBox *m_pDsb;
    QVBoxLayout *m_pVBox;
    QHBoxLayout *m_pHBox[5];
    QLineEdit   *m_pCurPos;
    QMap<QString,BITINFO_STRU> m_contrlBitInfoMap;
    QMap<QString,BITINFO_STRU> m_stausBitInfoMap;
    int m_contrlBytePos;
    int m_stausBytePos;
    QMap<QString,BYTEINFO> m_byteInfoMap;
    QString m_axisName;
    SWCONFIGXML_SER_MAP m_swSerMap;
    QTimer *m_pTimer;
    MainORGThread *m_ORGThread;
    bool m_isORG;
    bool m_ison;
    QString m_strOPName;
    QVector<QPair<int,int>> m_tempINSON;
    QVector<QPair<int,int>> m_tempOUTSON;
    QVector<QPair<int,int>> m_tempOFFSON;
    QMap<QString,PROTECT_EVENT_STRU> axisEventMap;
private:
    void CreatAsixUI();
    bool JudgeHWLimit(QString strKey, QString strOPName, QString &strMsg);
    bool JudgeAsixEvent();

signals:
public slots:
    void RunORG();
    void RunSon();
    void RunReSet();
    void RunLeft();
    void RunRight();
    void RunQuickFixPos();
    void RunStop();
    void timerUpInputData();
    void DisableBtn(bool);
private:
   void RelativeChange(int bit);
};

#endif // AXISINFOUI_H
