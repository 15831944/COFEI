/************************************
* @brief 摘要  调试界面校准加界面下轴的界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef AXISOPERATER_H
#define AXISOPERATER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTimer>
#include <QPushButton>
#include <DycLibBase/PackLib/packlib.h>
#include <MyHW_Share.h>

class AxisOperater : public QWidget
{
    Q_OBJECT
public:
    explicit AxisOperater(QWidget *parent = 0);
    ~AxisOperater();
public:
    void        SetAxisName(const QString strName, QMap<QString, PROTECT_EVENT_STRU> temp);
    void        SetAxisRanage(const double dstart,const double dend);
    QString     GetAxisName();
    double      GetTraPos();
    uint        GetTraSpeed();
    void        SetTraPos(double d);
    void        SetPower(int);
    QString     m_pointName;
private:
    void        CreatAsixUI();
    bool        JudgeAsixEvent();
    bool        StopPlugins();
    void        RunAbsAsix(SWCONFIGXML_QPOSIT_STRU);
    void        RunRelAsix(SWCONFIGXML_QPOSIT_STRU);
    //关于按钮间隔多长时间响应的函数
    bool        JudgeButtonIsExcEvent();
private slots:
    void        CheckSonTimer();
public slots:
    void        RunORG();
    void        RunSon();
    void        RunReSet();
    void        RunLeft();
    void        RunRight();
    void        RunQuickFixPos();
    void        RunStop();
private:
    FunModuleInterface  *m_OrgPlugin;
    FunModuleInterface  *m_PPPlugin;
    FunModuleInterface  *m_RPPlugin;
    int                 m_power;
    QTimer              *m_timer;
    QFont               m_font;
    QLabel              *m_pLabel[2];
    QPushButton         *m_pBtn[8];
    QComboBox           *m_pCbb;
    QDoubleSpinBox      *m_pDsb;
    QVBoxLayout         *m_pVBox;
    QHBoxLayout         *m_pHBox[4];
    QLineEdit           *m_pCurPos;
    QString             m_axisName;
    bool                m_ison;
    int                 m_contrlBytePos;
    int                 m_stausBytePos;
    QMap<QString,BITINFO_STRU>  m_contrlBitInfoMap;
    QMap<QString,BITINFO_STRU>  m_stausBitInfoMap;
    QMap<QString,BYTEINFO>      m_OutbyteInfoMap;
    QMap<QString,BYTEINFO>      m_InbyteInfoMap;
    QMap<QString,PROTECT_EVENT_STRU> m_axisEventMap;
    QVector<int> m_dbspeedparm;
};

#endif // AXISOPERATER_H
