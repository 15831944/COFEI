#include "ItemEx.h"
#include <QTimer>
CoItemEx::CoItemEx(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    CoItem(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_NameId = tr("");
    m_UserDataName1 = tr("");
    m_UserDataName2 = tr("");
    m_UserData1 = 0;
    m_UserData2 = 0;
    m_Timer = NULL;
    m_AutoTime = 0;
    m_HostScene = NULL;
//    m_isfirstSelected = false;
    m_font.setFamily("宋体");
    m_font.setPointSize(10);
}

CoItemEx::~CoItemEx()
{}

void CoItemEx::Copy(CoItemEx *ItemExFrom)
{
    if(NULL == ItemExFrom)
    {
        return;
    }
    this->SetNameId(ItemExFrom->GetNameId());
    this->SetServiceId(ItemExFrom->GetServiceId());
    this->SetHint(ItemExFrom->GetHint());

    this->SetUserDataName1(ItemExFrom->GetUserDataName1());
    this->SetUserData1(ItemExFrom->GetUserData1());
    this->SetUserDataName2(ItemExFrom->GetUserDataName2());
    this->SetUserData2(ItemExFrom->GetUserData2());
    CoItem::Copy(ItemExFrom);
}

void CoItemEx::SetNameId(const QString &NameId)
{
    m_NameId = NameId;
}

QString CoItemEx::GetNameId() const
{
    return m_NameId;
}

void CoItemEx::SetServiceId(int Id)
{
    m_ServiceId = Id;
}

int CoItemEx::GetServiceId() const
{
    return m_ServiceId;
}

void CoItemEx::SetHint(const QString &Hint)
{
    m_Hint = Hint;
}

QString CoItemEx::GetHint() const
{
    return m_Hint;
}

void CoItemEx::SetHostScene(QObject *HostScene)
{
    m_HostScene = HostScene;
}

QObject* CoItemEx::GetHostScene() const
{
    return m_HostScene;
}

QGraphicsScene *CoItemEx::GetCurScene() const
{
    return scene();
}
void CoItemEx::SetAutoTime(int Time)
{
    m_AutoTime = Time;
}

int CoItemEx::GetAutoTime()
{
    return m_AutoTime;
}

void CoItemEx::StartAuto(int itype)
{
    if(NULL == m_Timer || m_AutoTime <= 0)
    {
        return;
    }

    if(m_Timer->isActive())
    {
        if(itype == 0)
            m_Timer->stop();
    }
    else
    {
        if(itype == 1)
            m_Timer->start(m_AutoTime);
    }
}
void CoItemEx::SetUserDataName1(const QString &UserDataName)
{
    m_UserDataName1 = UserDataName;
}

QString CoItemEx::GetUserDataName1() const
{
    return m_UserDataName1;
}

void CoItemEx::SetUserDataName2(const QString &UserDataName)
{
    m_UserDataName2 = UserDataName;
}

QString CoItemEx::GetUserDataName2() const
{
    return m_UserDataName2;
}

void CoItemEx::SetUserData1(const double UserData)
{
    m_UserData1 = UserData;
}

double CoItemEx::GetUserData1() const
{
    return m_UserData1;
}

void CoItemEx::SetUserData2(const double UserData)
{
    m_UserData2 = UserData;
}

double CoItemEx::GetUserData2() const
{
    return m_UserData2;
}

void CoItemEx::SetFont(QFont font)
{
    m_font = font;
}

QFont CoItemEx::GetFont() const
{
    return m_font;
}
