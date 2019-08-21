#ifndef CYLINDERLAMP_H
#define CYLINDERLAMP_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <Comm.h>
#include <QTimer>
class CylinderLamp : public QWidget
{
    Q_OBJECT
public:
    explicit CylinderLamp(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void Clicked(bool);
    void SendCyOutputData(QMap<QString,uchar>);
public slots:
    void SetAlarm(bool);
    void TimeUpCyOutput();
private:
    bool m_alarm;
    bool m_isclicked;
    bool m_enableClick;
    QTimer *m_timer;
    QString m_strEvent;
    QString m_strName;
    QString m_strType;
    SWCONFIGXML_CY_STRU *_ps;
    uchar m_ucInput;
    QMap<QString,uchar> m_soltCompMap;
public:
    void SetClickEnable(bool);
    void SetProtectEvent(const QString&);
    void SetCylampID(const QString&);
    void SetCylampModel(const QString&);
    void SetCylampStru(SWCONFIGXML_CY_STRU *);
    QString GetCylampID();
    void SetInputLamp(const uchar uc);
    bool IsAlarm() const {return m_alarm;}
};

#endif // CYLINDERLAMP_H
