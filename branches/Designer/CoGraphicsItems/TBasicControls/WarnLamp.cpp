#include <QDebug>
#include <QBitmap>
#include <QGraphicsSceneMouseEvent>
#include "CoGraphics/Base/Base.h"
#include "AttributeWidget.h"
#include "WarnLamp.h"
TWarnLamp::TWarnLamp(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Warnlamp);
    m_CurrentIndex = -1;
    m_RowSum = 5;
    m_RowLength = 0;
    SetSize();
    m_VarNameVec.reserve(4);
    m_VarNameVec <<"" <<"" <<"" <<"";

}

TWarnLamp::~TWarnLamp()
{
}

CoItem *TWarnLamp::Copy()
{
    TWarnLamp *ioLamp = new TWarnLamp();

    ioLamp->Copy(this);

    return ioLamp;
}

void TWarnLamp::Copy(TWarnLamp *IoFrom)
{
    if(NULL == IoFrom)
    {
        return;
    }

    this->SetVarName(IoFrom->GetVarName()[0],1);
    this->SetVarName(IoFrom->GetVarName()[1],2);
    this->SetVarName(IoFrom->GetVarName()[2],3);
    this->SetVarName(IoFrom->GetVarName()[3],4);

    CoGraphics::Copy(IoFrom);
}

void TWarnLamp::Update()
{
    SetSize();
    update();
}

void TWarnLamp::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    switch (type)
    {
    case CoAttributeWidget::Power:
        SetPower(value.toInt());
        break;
    case CoAttributeWidget::VarName:
        SetVarName(value,uc);
        break;
    default:
        break;
    }
}

void TWarnLamp::SetVarName(const QString name, uchar uc)
{
    switch (uc) {
    case 1:
        m_VarNameVec[0] = name;
        break;
    case 2:
        m_VarNameVec[1] = name;
        break;
    case 3:
        m_VarNameVec[2] = name;
        break;
    case 4:
        m_VarNameVec[3] = name;
        break;
    default:
        break;
    }
}

QVector<QString> TWarnLamp::GetVarName()
{
    return m_VarNameVec;
}

bool TWarnLamp::SetSize()
{
    // 计算行高
    m_RowLength = (GetCurrentStopY() - GetCurrentStartY()) / m_RowSum;

    return true;
}
void TWarnLamp::Draw(QPainter *painter)
{
    QPixmap pix(tr(":images/draw/WarnLamp.png"));
    pix.setMask(pix.mask());
    painter->drawPixmap(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY,
                        pix);
}

void TWarnLamp::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int curIndex = -1;
    if(m_RowLength > 0 && m_RowSum > 0)
    {
        curIndex = (event->pos().y() - m_StartY) / m_RowLength;
    }
    if(curIndex == (m_RowSum -1))
        return;
    if(curIndex == m_CurrentIndex)
    {
        //IO = 0
        m_CurrentIndex = -1;
    }
    else
    {
        //IO = 1
        m_CurrentIndex = curIndex;
    }
    CoItem::mousePressEvent(event);
}
