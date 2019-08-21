#ifndef CSPINFOUI_H
#define CSPINFOUI_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "pushbutton.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <MyGlobal.h>
#include <QTimer>
#include <QTableWidget>
#include"csptablewidget.h"
#include"csptritable.h"
#include "analysishwconfig.h"
#include <DycLibBase/LCSPLib/lcsplib.h>
#include <QCheckBox>
#include <DycLibBase/Coordinate/coordinate.h>
#include "axisinfoui.h"
class CSPThread : public QThread
{
    Q_OBJECT
public:
    CSPThread();
protected:
    void run();
public:
    LCSPLib *m_csplib;
public:
    void SetParam(SWCONFIGXML_LCSP_STRU);
    void InitCSPParam(SWCONFIGXML_LCSP_STRU);
    void Stop();
    int m_runiresult;

private:
    QMutex m_mutex;
    SWCONFIGXML_LCSP_STRU m_ps;
};

class CspInfoUI : public QWidget
{
    Q_OBJECT
public:
    explicit CspInfoUI(QVector<QString>,SWCONFIGXML_LCSP_STRU ,QWidget *parent = 0);

    //add by sylar.liu
    ~CspInfoUI();

    void TimerStatus(bool isStop);
    QVector<LCSP_CSYSINFO_STRU> GetLcspAllPos();
private:
    QTimer *m_pTimer;
    CSPThread *MyLCSPThread;
    Coordinate *m_CoSwlib;
    QMap<QString,double> m_eleratMap;
    QMap<QString,QVector<QString>> m_RecswMap;
    QVector<QString> m_hwcsysid;
    QVector<QString> m_hwarmid;
    QMap<QString ,int> m_statposbyteMap;

    QLineEdit *statinfo;
    QWidget *m_cspxyzwg;
    QWidget *m_cspjwg;
    QVector<QMap<QString,double>> m_vec;
    QHBoxLayout *m_pHBox;
    QFont m_font;
    QPushButton *m_cspbtn[6];
    QPushButton  *m_RecIdbtn[6];
    QLineEdit *m_RecidLinepos[3];

    QMenu *menu;
    QPushButton *modswtbtn;
    QAction *modswt;
    QLineEdit *m_xyzpos[6];

    QMap<QString,QLineEdit*> m_XYZPOSMap;
    QLineEdit *m_poseditxyz;
    QPushButton *m_XYZSave;
    QPushButton *m_JntBtn[12];
    QLineEdit *m_JPosInfo[6];
    QCheckBox *m_PulSw;
    QLineEdit *m_poseditj;
    QPushButton *m_Jsave;

    QMap<QString,QLineEdit*> m_JntInfoMap;

    CSPTableWidget *m_table;
    CSPTableWidget *m_detable;
    CSPTriTable *m_Tritable;
    CSPTriTable *m_deTritable;

    QMap<QString,BITINFO_STRU> m_contrlBitInfoMap;
    QMap<QString,BYTEINFO> m_byteInfoMap;
    int m_runiresult;
    bool m_isStop;
    bool m_InReStop;
    int m_row;
    QVector<QPair<int,int>> m_tempINSON;
    QVector<QPair<int,int>> m_tempOUTSON;
    QVector<QPair<int,int>> m_tempOFFSON;
private:
    QMap<QString,MainORGThread *>m_ORGThreadMap;
    QMap<QString,QPushButton *>m_ORGBtnMap;
private:
    SWCONFIGXML_LCSP_STRU m_updatecsptablestru;
    QVector<QMap<QString,double>> m_TraPosvec;
    QVector<QString> m_alltrap;
    typedef struct _TRIGGERTABLESTU
    {
        int row = -1;
        QVector<LCSP_TRIGGERINFO_STRU> LCSPTriggerInfo;
    }RIGGERTABLESTU;
    QVector<RIGGERTABLESTU> m_newtri;
    bool m_norun;
    bool m_orgrun;
    QString m_axisrunid;
    QMap<QString,PROTECT_EVENT_STRU> axisEventMap;
    QMap<QString,PROTECT_EVENT_STRU> coordEventMap;
private:
    void GetCSPDataInfo(QVector<QString>str);
    void CreatCspUI();
    void GetLCSPCurpos(QString strKey, double &curPos);
    int GetStatuswordPos(QString strKey);
    int GetErrorcode(QString strKey);
    void RunXYZIncandRed(int inde);
    void RunArmIncandRed(int inde);
    bool Checkconfig(SWCONFIGXML_LCSP_STRU);
    void SetenbleBtn();

public slots:
    void timerUpInputData();
    void DisableBtn(bool);
private slots:
    void changemodel();
    void RunORG();
    void StopCSP();
    void RunCSP();
    void S_onCSP();
    void ResetCSP();
    void SwitchSpeedCSP();
    void RunSinPos();
    void ClearTriInfo(int);
    void CloseTriTable();
    void ShowTriTable(int);
    void SaveTri(QVector<LCSP_TRIGGERINFO_STRU> LCSPTriggerInfo);
    void Acceptupdatedata(QVector<QMap<QString,double>>,QVector<QString>);
    void SaveP();
    //坐标模式
    void IncreaseXCSP();
    void ReduceXCSP();
    void IncreaseYCSP();
    void ReduceYCSP();
    void IncreaseZCSP();
    void ReduceZCSP();
    void XYZCSPStop();
    //关节模式
    void J1IncrRun();
    void J1RedRun();
    void J2IncrRun();
    void J2RedRun();
    void J3IncrRun();
    void J3RedRun();
    void J4IncrRun();
    void J4RedRun();
    void J5IncrRun();
    void J5RedRun();
    void J6IncrRun();
    void J6RedRun();
signals:
    void updatecsptabledata(QVector<_TRIGGERTABLESTU>);
    void Getlinekpos(QMap<QString,LCSP_CPOSINFO_STRU>,QMap<QString,LCSP_CPOSINFO_STRU>);

private:
    QVector<QMap<QString,double>> GetXYZcurpos(QVector<QMap<QString,double>> pulsepos);
    QMap<QString,double> GetXYZSinglecurpos(QMap<QString,double> pulsepos);
    QVector<QMap<QString,double>> GetJpos(QVector<QMap<QString,double>> XYZpos);
    void GetSumeleratMap( QVector<QString>);
    bool HandleHWLimit(QString str,QString strKey,QString &strMsg);
    void HandleRunORGiresult();
    void HandleRuniresult();
    void ShowCurposInfo();
    void SetDisabledBtn(QString ,int inde);
    bool JudgeAsixEvent(const QString strAsix);
    bool JudgeCoordEvent(const QString strCoord);
private:
    bool CheckSON();
    void AddPosData(SWCONFIGXML_LCSP_STRU&,QVector<QString> str);//解决新建一个点位置不能运行界面的问题
};

#endif // CSPINFOUI_H
