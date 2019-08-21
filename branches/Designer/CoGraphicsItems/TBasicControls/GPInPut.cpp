#include <QDebug>
#include <QTimer>
#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include "AttributeWidget.h"
#include "GPInPut.h"

TGPInPut::TGPInPut(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor):
    CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::GPInPut);
    m_IsGreen = false;
    m_strIOName = "";
    m_AutoTime = 300;

}

CoItem *TGPInPut::Copy()
{
    TGPInPut *gpOutput = new TGPInPut();

    gpOutput->Copy(this);

    return gpOutput;
}

void TGPInPut::Copy(TGPInPut *IoFrom)
{
    if(NULL == IoFrom)
    {
        return;
    }
    this->SetIOName(IoFrom->GetIOName());
    this->SetAutoTime(IoFrom->GetAutoTime());
    CoGraphics::Copy(IoFrom);
}

void TGPInPut::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    switch (type)
    {
    case CoAttributeWidget::Power:
        SetPower(value.toInt());
        break;
    case CoAttributeWidget::VarName:
        SetIOName(value);
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    default:
        break;
    }

}

void TGPInPut::SetIOName(const QString &name)
{
    m_strIOName = name;
}

QString TGPInPut::GetIOName() const
{
    return m_strIOName;
}

void TGPInPut::AutoRunRepeat()
{
    if(GetPower() >= AttributeInstance::GetInstance()->Power)
    {
        bool isGreen = m_IsGreen;
        if(isGreen != m_IsGreen)
            Update();
    }
}

void TGPInPut::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;        // 绘图区域的宽度
    double Height = m_StopY - m_StartY;       // 绘图区域的高度
    double CenterX = m_StartX + Width / 2;   // 绘图区域的中心X
    double CenterY = m_StartY + Height / 2;  // 绘图区域的中心Y
    double startX = m_StartX;
    double startY = m_StartY;
    double stopX = m_StopX;
    double stopY = m_StopY;

    painter->setPen(Qt::transparent);
    // 最外层边框
    QLinearGradient lg1(startX,startY,stopX,stopY); // 线性渐变
    lg1.setColorAt(0, m_LineColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_LineColor, -90));
    painter->setBrush(lg1);
    painter->drawRect(startX,startY,Width,Height);

    // 中间层边框
    startX = startX + (Width-(Width*0.87))/2;
    startY = startY + (Height-(Height*0.87))/2;
    Width *= 0.87;
    Height *=0.87;
    stopX = startX + Width;
    stopY = startY + Height;

    QLinearGradient lg2(startX,startY,stopX,stopY);
    lg2.setColorAt(0, TBase::GetNewColor(m_LineColor, -100));
    lg2.setColorAt(1, m_LineColor);
    painter->setBrush(lg2);
    painter->drawRect(startX,startY,Width,Height);

    // 灯
    startX = startX + (Width-(Width*0.96))/2;
    startY = startY + (Height-(Height*0.96))/2;
    Width *= 0.96;
    Height *=0.96;

    double Radius = sqrt((Width*Width) + (Height*Height));
    QRadialGradient rg(CenterX,CenterY,Radius/2); // 辐射渐变
    if (m_IsGreen)
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
    painter->setBrush(rg);
    painter->drawRect(startX,startY,Width,Height);
}
