#include <QDebug>
#include <QTimer>
#include "IOLamp.h"
#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include "AttributeWidget.h"
TIOLamp::TIOLamp(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Iolamp);
    m_Pressed = false;
    m_IsGreen = false;
    m_VarName = "";
    m_VarType = -1;
    m_Expre = "";
    m_AutoTime = 300;
}

TIOLamp::~TIOLamp()
{}

CoItem* TIOLamp::Copy()
{
    TIOLamp *ioLamp = new TIOLamp();

    ioLamp->Copy(this);

    return ioLamp;
}

void TIOLamp::Copy(TIOLamp *IoFrom)
{
    if(NULL == IoFrom)
    {
        return;
    }

    this->SetVarName(QString("%1,%2").arg(IoFrom->GetVarName()).arg(QString::number(IoFrom->GetVarType())));
    this->SetAutoTime(IoFrom->GetAutoTime());
    this->SetExpre(IoFrom->GetExpre());
    CoGraphics::Copy(IoFrom);
}

void TIOLamp::SetVarName(const QString &var)
{
    QStringList list = var.split(',');
    if(list.size() > 1)
    {
        m_VarName = list[0];
        m_VarType = list[1].toShort();
    }
}

QString TIOLamp::GetVarName() const
{
    return m_VarName;
}

char TIOLamp::GetVarType() const
{
    return m_VarType;
}

void TIOLamp::SetExpre(const QString expre)
{
    m_Expre = expre;
}

QString TIOLamp::GetExpre() const
{
    return m_Expre;
}

void TIOLamp::SetAttributeEvent(const int type, const QString value, uchar)
{
    switch (type)
    {
    case CoAttributeWidget::Power:
        SetPower(value.toInt());
        break;
    case CoAttributeWidget::VarName:
        SetVarName(value);
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    case CoAttributeWidget::EnableExpre:
        SetExpre(value);
        break;
    default:
        break;
    }
}

void TIOLamp::AutoRunRepeat()
{ 
    //if(GetPower() >= AttributeInstance::GetInstance()->Power)
    {
        bool expreResult = true;
        bool varResult = true;
        if(!m_Expre.isEmpty())
        {
            QString strExpre = m_Expre;
            strExpre.replace("&","$");
            StackManager stack;
            expreResult = stack.AnalysisEvent(strExpre);
        }
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_VarName].ishwIO)
        {
            uchar uc = 0;
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            QString ioaddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_VarName].IOaddr;
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
            if(ioaddr.contains("E"))
            {
                int iresult = BaseCalcFun::AnalysisIOString(ioaddr,uc);
                if(iresult == 1)
                    varResult = IntToBool(uc);
            }
            if(ioaddr.contains("Q"))
            {
                int iresult = BaseCalcFun::GetOutIOQBitValue(ioaddr,uc);
                if(iresult == 1)
                    varResult = IntToBool(uc);
            }
        }
        else
        {
            QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
            varResult = IntToBool(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_VarName].initvalue);
        }

        bool isGreen = m_IsGreen;
        // 判断表达式和变量的综合值
        m_IsGreen = expreResult & varResult;
        if(isGreen != m_IsGreen)
            Update();
    }
}

void TIOLamp::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;        // 绘图区域的宽度
    double Height = m_StopY - m_StartY;       // 绘图区域的高度
    double CenterX = m_StartX + Width / 2;   // 绘图区域的中心X
    double CenterY = m_StartY + Height / 2;  // 绘图区域的中心Y

    painter->translate(CenterX, CenterY);   // 把区域中心作为零点
    painter->setPen(Qt::transparent);

    int Radius = MIN(Width / 2, Height / 2);// 绘制圆形的半径

    // 边框的外圆
    QLinearGradient lg1(0, -Radius, 0, Radius); // 线性渐变
    lg1.setColorAt(0, m_LineColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_LineColor, -90));
    painter->setBrush(lg1);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 边框的内圆
    Radius *= 0.87;
    QLinearGradient lg2(0, -Radius, 0, Radius);
    lg2.setColorAt(0, TBase::GetNewColor(m_LineColor, -100));
    lg2.setColorAt(1, m_LineColor);
    painter->setBrush(lg2);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 灯的圆
    Radius *= 0.96;
    QRadialGradient rg(0, 0, Radius); // 辐射渐变
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
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 高光
    Radius *= 0.97;
    QPainterPath path;
    path.addEllipse(-Radius, -Radius *1.02, Radius << 1, Radius << 1);

    QPainterPath bigEllipse;

    // 减去高光下部的大圆
    Radius *= 2;
    bigEllipse.addEllipse(-Radius, -Radius * 0.45, Radius << 1, Radius << 1);
    path -= bigEllipse;

    QLinearGradient lg3(0, -Radius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter->setBrush(lg3);
    painter->drawPath(path);
}
