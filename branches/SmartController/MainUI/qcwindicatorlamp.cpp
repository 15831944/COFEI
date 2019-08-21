#include "qcwindicatorlamp.h"
#include <QTimer>
#include "mainwindow.h"
#include <DycLibBase/StackManager/stackmanager.h>
QcwIndicatorLamp::QcwIndicatorLamp(QWidget *parent): QWidget(parent)
{
    m_alarm = false;
    m_isclicked = false;
    m_enableClick = true;
    connect(this,SIGNAL(Clicked(bool)),this,SLOT(SetAlarm(bool)));
}

void QcwIndicatorLamp::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    StackManager *stackm = new StackManager;
    if(!stackm->AnalysisEvent(m_strEvent))
        return;
    m_isclicked = m_alarm;
    if(m_enableClick)
    {
        m_isclicked = !m_isclicked;
        emit Clicked(m_isclicked);
        if(m_isclicked)
            emit SendOutputData(m_strType,m_strName,1);
        else
           emit SendOutputData(m_strType,m_strName,0);
    }
}
void QcwIndicatorLamp::SetClickEnable(bool isTrue)
{
    m_enableClick = isTrue;
}

void QcwIndicatorLamp::paintEvent(QPaintEvent *event)
{
     Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);

    int radius = 100;

    /* 外边框 */
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    /* 内边框 */
    radius -= 13;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    /* 内部指示颜色 */
    radius -= 4;
    QRadialGradient rg(0, 0, radius);
    if (m_alarm)
    {
        rg.setColorAt(0, QColor(0, 245, 0));
        rg.setColorAt(0.6, QColor(0, 210, 0));
        rg.setColorAt(1, QColor(0, 166, 0));
    }
    else
    {
        rg.setColorAt(0, QColor(245, 0, 0));
        rg.setColorAt(0.6, QColor(210, 0, 0));
        rg.setColorAt(1, QColor(166, 0, 0));
    }
    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
}

void QcwIndicatorLamp::SetAlarm(bool alarm)
{
    m_alarm = alarm;
    update();

}

