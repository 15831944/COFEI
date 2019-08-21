/************************************
* @brief 摘要 调试界面的校准界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef CALIBRATIONUI_H
#define CALIBRATIONUI_H

#include <Comm.h>
#include <QDialog>
#include "axisinfoui.h"
#include "cspinfoui.h"
#include <QLabel>

class CalibrationUI : public QDialog
{
    Q_OBJECT
public:
    CalibrationUI(QVector<QString>,int ,int ,const QString&);
    void GetStruData(SWCONFIGXML_BASE_STRU *baseser)
    {
        if(m_strType.compare("快速定位") == 0)
        {
            m_QPser = *(SWCONFIGXML_QPOSIT_STRU*)baseser;
        }
        else
        {
            m_LCSPser = *(SWCONFIGXML_LCSP_STRU*)baseser;
        }
    }
    ~CalibrationUI();

private:
    HWCONFIGXML_MAP m_hwMap;
    SWCONFIGXML_SER_MAP m_swSerMap;
    SWCONFIGXML_QPOSIT_STRU m_QPser;
    SWCONFIGXML_LCSP_STRU  m_LCSPser;
    QVector<QString> m_AxisVec;
    QFont m_font;
    int m_width;
    int m_hight;
    QString m_strType;
    QMap<QString,int> m_checkdlgData;
    QString m_IDname;
public:
    QVector<AxisInfoUI*> m_axisUIVec;
    CspInfoUI * m_cspInfoUI;
    void CreateUI(const QString&,QString);
private:
    void GetAxisConfig(SWCONFIGXML_QPOSIT_STRU &swser, QMap<QString,int>);
    void GetAxisConfig(SWCONFIGXML_LCSP_STRU &swSer);
    bool Compare2QPStruct(SWCONFIGXML_QPOSIT_STRU swser1,SWCONFIGXML_QPOSIT_STRU swser2);
    bool CreateMessageBox(QString strTitle,QString note);

public:
    void ChangetipsText(const QString str);
    void SetAxisUIPlayStatus(bool isstop)
    {
        for(int i(0);i < m_axisUIVec.count();++i)
        {
            m_axisUIVec[i]->DisableBtn(isstop);
        }
        if(m_cspInfoUI != NULL)
            m_cspInfoUI->DisableBtn(isstop);
    }

    void SetAxisUIonGObtn(bool isstop)//on gO按钮信号
    {
        for(int i(0);i < m_axisUIVec.count();++i)
        {
            m_axisUIVec[i]->DisableBtn(isstop);
        }
    }
    void SetCheckDlgData(QMap<QString,int> temp)
    {
        m_checkdlgData = temp;
    }
public:
    QLabel *tips;

signals:
    void SaveAxisSerConfig(QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
    void DisableAxisUI(bool);
private slots:
    void SaveData();

public slots:
    void GetUIEnableSig(bool isenable)
    {
        emit DisableAxisUI(isenable);
    }
    void GetChangeTimerSig(bool isenable);
};

#endif // CALIBRATIONUI_H
