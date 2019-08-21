/************************************
* @brief 摘要 自定义指示灯
* @author sunny
* @date 2017/06/01
************************************/
#ifndef QCWINDICATORLAMP_H
#define QCWINDICATORLAMP_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QBitmap>
class QColor;
class QcwIndicatorLamp : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
public:
    explicit QcwIndicatorLamp(QWidget *parent = 0);
    bool IsAlarm() const {return m_alarm;}
signals:
    void Clicked(bool);
    void SendOutputData(QString,QString,int64_t);
public slots:
    void SetAlarm(bool);

private:
    bool m_alarm;
    bool m_isclicked;
    bool m_enableClick;
public:
    QString m_strType;
    QString m_strName;
    QString m_strEvent;
public:
    void SetClickEnable(bool);

};

#endif // QCWINDICATORLAMP_H
