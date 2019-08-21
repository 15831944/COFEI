#include "cylinderlamp.h"
#include <DycLibBase/StackManager/stackmanager.h>
#include <QMutexLocker>
#include <MyGlobal.h>
using namespace CoreGlobal;
CylinderLamp::CylinderLamp(QWidget *parent) : QWidget(parent)
{
    m_alarm = false;
    m_isclicked = false;
    m_enableClick = true;
    m_ucInput = 0;
    m_timer = new QTimer(this);
    connect(this,SIGNAL(Clicked(bool)),this,SLOT(SetAlarm(bool)));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(TimeUpCyOutput()));
}

void CylinderLamp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);
    int x = 0;
    if(width() > height())
        x = qAbs(width() - height());
    float_t k = side / 100.0;
    int radius = 100 ;
    x *= k;
    /* 外边框 */
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawRoundRect(-radius - x, -radius, (radius+x) << 1, radius << 1);

    /* 内边框 */
    radius -= 10;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawRoundRect(-radius- x, -radius, (radius +x)<< 1, radius << 1);

    /* 内部指示颜色 */
    radius -= 4;
    QRadialGradient rg(0,0, radius);

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
    painter.drawRoundRect(-radius-x, -radius, (radius+x) << 1, radius << 1);

    if(_ps->soltCompVec.size() > 1)
    {
        QPainterPath path;
        path.addRoundRect(-radius-x, -radius, (radius+x) << 1, radius << 1,25,25);
        QPainterPath bigEllipse;

        radius *= 2;
        bigEllipse.addRoundRect(-radius -180 -x*2 , -radius , (radius+x) << 1, radius << 1,25,25);
        path -= bigEllipse;

        QLinearGradient lg3(0, -radius, 0, radius);
        lg3.setColorAt(0, QColor(254, 0, 0,120));
        lg3.setColorAt(1, QColor(254, 0, 0,80));
        painter.setBrush(lg3);
        painter.drawPath(path);
    }

    if(!_ps->basePoint.isEmpty())
    {
        QPainter painter1(this);
        painter1.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
        painter1.translate(width()*0.333, height()*0.333);	/* 坐标变换为窗体中心 */
        side = qMin(width()/3, height()/3);
        painter1.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
        painter1.setPen(Qt::NoPen);

        radius = 100;
        QLinearGradient lgx(0, -radius, 0, radius);
        lgx.setColorAt(0, QColor(255, 255, 255));
        lgx.setColorAt(1, QColor(166, 166, 166));
        painter1.setBrush(lgx);
        painter1.drawEllipse(-radius, -radius, radius << 1, radius << 1);
        radius -= 13;
        QLinearGradient lgx2(0, -radius, 0, radius);
        lgx2.setColorAt(0, QColor(166, 166, 166));
        lgx2.setColorAt(1, QColor(255, 255, 255));
        painter1.setBrush(lgx2);
        painter1.drawEllipse(-radius, -radius, radius << 1, radius << 1);
        radius -= 6;
        QRadialGradient rg1(0, 0, radius);

        uchar uc = m_ucInput >> 4;
        if (uc == 1)//green
        {
            rg1.setColorAt(0, QColor(0, 245, 0));
            rg1.setColorAt(0.6, QColor(0, 210, 0));
            rg1.setColorAt(1, QColor(0, 166, 0));
        }
        else if(uc == 2)//red
        {
            rg1.setColorAt(0, QColor(245, 0, 0));
            rg1.setColorAt(0.6, QColor(210, 0, 0));
            rg1.setColorAt(1, QColor(166, 0, 0));
        }
        else //bule
        {
            rg1.setColorAt(0, QColor(0, 0, 254));
            rg1.setColorAt(0.6, QColor(0, 0, 210));
            rg1.setColorAt(1, QColor(0, 0, 166));
        }
        painter1.setBrush(rg1);
        painter1.drawEllipse(-radius, -radius, radius << 1, radius << 1);
    }
    if(!_ps->movePoint.isEmpty())
    {
        QPainter painter2(this);
        painter2.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
        painter2.translate(width()*0.666, height()*0.333);	/* 坐标变换为窗体中心 */
        side = qMin(width()/3, height()/3);
        painter2.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
        painter2.setPen(Qt::NoPen);

        radius = 100;
        QLinearGradient lgy(0, -radius, 0, radius);
        lgy.setColorAt(0, QColor(255, 255, 255));
        lgy.setColorAt(1, QColor(166, 166, 166));
        painter2.setBrush(lgy);
        painter2.drawEllipse(-radius, -radius, radius << 1, radius << 1);
        radius -= 13;
        QLinearGradient lgy2(0, -radius, 0, radius);
        lgy2.setColorAt(0, QColor(166, 166, 166));
        lgy2.setColorAt(1, QColor(255, 255, 255));
        painter2.setBrush(lgy2);
        painter2.drawEllipse(-radius, -radius, radius << 1, radius << 1);
        radius -= 6;
        QRadialGradient rg2(0, 0, radius);

        uchar uc = m_ucInput & 0x0f;
        if (uc == 2) //red
        {
            rg2.setColorAt(0, QColor(245, 0, 0));
            rg2.setColorAt(0.6, QColor(210, 0, 0));
            rg2.setColorAt(1, QColor(166, 0, 0));
        }
        else if (uc == 1)//green
        {
            rg2.setColorAt(0, QColor(0, 245, 0));
            rg2.setColorAt(0.6, QColor(0, 210, 0));
            rg2.setColorAt(1, QColor(0, 166, 0));
        }
        else //bule
        {
            rg2.setColorAt(0, QColor(0, 0, 254));
            rg2.setColorAt(0.6, QColor(0, 0, 210));
            rg2.setColorAt(1, QColor(0, 0, 166));
        }
        painter2.setBrush(rg2);
        painter2.drawEllipse(-radius, -radius, radius << 1, radius << 1);
    }
}

void CylinderLamp::mousePressEvent(QMouseEvent *event)
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
        {
            if(m_soltCompMap.size() == 1)
            {
                m_soltCompMap.begin().value() = 1;
            }
            else if(m_soltCompMap.size() == 2)
            {
                m_soltCompMap.begin().value() = 1;
                (++m_soltCompMap.begin()).value() = 0;
            }
            else
                return;
            emit SendCyOutputData(m_soltCompMap);
        }
        else
        {
            if(m_soltCompMap.size() == 1)
            {
                m_soltCompMap.begin().value() = 0;
            }
            else if(m_soltCompMap.size() == 2)
            {
                m_soltCompMap.begin().value() = 0;
                (++m_soltCompMap.begin()).value() = 1;
            }
            else
                return;
            emit SendCyOutputData(m_soltCompMap);
        }
    }
}

void CylinderLamp::SetAlarm(bool alarm)
{
    m_alarm = alarm;
    update();
}

void CylinderLamp::TimeUpCyOutput()
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    m_ucInput = 0;
    if(BaseCalcFun::AnalysisIOString(_ps->basePoint,uc) == 1)
    {
        if(uc == 0) //red
            m_ucInput += 0x20;
        else // green
            m_ucInput += 0x10;
    }

    uc = 0;
    if(BaseCalcFun::AnalysisIOString(_ps->movePoint,uc) == 1)
    {
        if(uc == 0) //red
            m_ucInput += 2;
        else//green
            m_ucInput += 1;
    }


    update();
}

void CylinderLamp::SetClickEnable(bool isTrue)
{
    m_enableClick = isTrue;
    if(m_enableClick)
        m_timer->start(200);
    else
        m_timer->stop();
}

void CylinderLamp::SetProtectEvent(const QString &strEvent)
{
    m_strEvent = strEvent;
}

void CylinderLamp::SetCylampID(const QString &strName)
{
    m_strName = strName;
}
QString CylinderLamp::GetCylampID()
{
    return m_strName;
}
void CylinderLamp::SetCylampModel(const QString &strType)
{
    m_strType = strType;
}

void CylinderLamp::SetCylampStru(SWCONFIGXML_CY_STRU *ps)
{
    _ps = ps;
    for(int i = 0; i < _ps->soltCompVec.size();++i)
        m_soltCompMap.insert(_ps->soltCompVec[i],0);
}

void CylinderLamp::SetInputLamp(const uchar uc)
{
    m_ucInput = uc;
}
