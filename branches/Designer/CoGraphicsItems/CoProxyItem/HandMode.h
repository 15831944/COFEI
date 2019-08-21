#ifndef HANDMODE_H
#define HANDMODE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <DycLibBase/PackLib/packlib.h>
#include <MyHW_Share.h>
#include "DycLibBase/TeachDeviceRunLib/teachdevicerunlib.h"
class HandMode : public QWidget
{
    Q_OBJECT
public:
    explicit HandMode(QWidget *parent = 0);
    ~HandMode();
signals:
    void SendHandParamEvent(const QString param); //手轮模式参数
    void SendStateEvent(const char,const QString errorCode); // -1 驱动器故障；0 手轮停止；1 手轮运行

public:
    void setPosInfo(const QString posInfo);
    void SetPower(int power);
    void InitValue();
    QString GetCurPos() const;
private slots:
    void CheckPosTimer();
    void CbbIndexChanged(const QString &text);
    void RunReset();

private:
    QString m_strPosName;
    QString m_strAxis;
    QString m_strPos;
    QMap<QString,QStringList> m_AxisRangeMap;
    QFont m_font;
    int m_power;

    QList<QLabel*> m_TraPosLblList;
    QComboBox *m_CbbRatio;
    QComboBox *m_CbbAxis;
    QPushButton *m_BtnReset;
    QGridLayout *m_Grid;
    QTimer *m_timer;
    TeachDeviceRunLib *m_TDRPluge;
};

#endif // HANDMODE_H
