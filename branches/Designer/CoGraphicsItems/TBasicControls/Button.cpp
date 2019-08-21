#include <QDebug>
#include <QtCore>
#include "Button.h"
#include "CoGraphics/Base/Base.h"
#include "LibBase.h"
#include "AttributeWidget.h"
#include <DycLibBase/StackManager/stackmanager.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <MyGlobal.h>
using namespace CoreGlobal;

TButton::TButton(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Button);
    m_Text = tr("Button");
    m_Pressed = false;
    m_PressMode = 0;

    m_VarName = "";
    m_VarState = 0;
    m_VarType = -1;
    m_JsTacticVec.reserve(2);
    m_JsTacticVec << ""<< "";
    m_ScreenName = "";
    m_euOpFunc = Function_Free;
    m_EnableExpre = "";
    m_EnableState = 0;
    m_IsSafe = true;

    m_stateVarName = "";
    m_stateVarType = -1;
    m_StateBGPixVec.reserve(2);
    m_StateBGPixVec << ""<< "";
    m_StateBGColorVec.reserve(2);
    m_StateBGColorVec << ""<< "";
    m_StateFontColorVec.reserve(2);
    m_StateFontColorVec << QColor(255,255,255) << QColor(255,255,255);
    m_StateSelectPixVec.reserve(2);
    m_StateSelectPixVec << ""<< "";
    m_StateTextVec.reserve(2);
    m_StateTextVec << "Button" << "Button";


    m_AutoTime = 300;
    m_IsZero = false;
    m_Count = 0;
    m_Timer = new QTimer();
    connect(m_Timer,&QTimer::timeout,this,&TButton::UpdateState);
}

TButton::~TButton()
{}

CoItem* TButton::Copy()
{
    TButton *Button = new TButton();

    Button->Copy(this);

    return Button;
}

void TButton::Copy(TButton *ButtonFrom)
{
    if(NULL == ButtonFrom)
    {
        return;
    }

    CoWidgets::Copy(ButtonFrom);
    this->SetPressMode(ButtonFrom->GetPressMode());
    this->SetBackColor(ButtonFrom->GetBackColor().at(0),0);
    this->SetBackColor(ButtonFrom->GetBackColor().at(1),1);
    this->SetSelectPix(ButtonFrom->GetSelectPix().at(0),0);
    this->SetSelectPix(ButtonFrom->GetSelectPix().at(1),1);
    this->SetPixFile(ButtonFrom->GetPixFiles().at(0),0);
    this->SetPixFile(ButtonFrom->GetPixFiles().at(1),1);
    this->SetText(ButtonFrom->GetText());
    this->SetJsTactic(ButtonFrom->GetJsTactic()[0],0);
    this->SetJsTactic(ButtonFrom->GetJsTactic()[1],1);
    this->SetVarName(QString("%1,%2,%3").arg(ButtonFrom->GetVarName())
                     .arg(QString::number(ButtonFrom->GetVarState()))
                     .arg(QString::number(ButtonFrom->GetVarType())));
    this->SetScreen(ButtonFrom->GetScreen());
    this->SetOpFunc(ButtonFrom->GetOpFunc());
    this->SetEnableExpre(ButtonFrom->GetEnableExpre());
    this->SetEnableState(ButtonFrom->GetEnableState());

    this->SetStateVarName(QString("%1,%2").arg(ButtonFrom->GetStateVarName())
                          .arg(QString::number(ButtonFrom->GetStateVarType())));
    this->SetAutoTime(ButtonFrom->GetAutoTime());
    this->SetStateFontColor(ButtonFrom->GetStateFontColor()[0],0);
    this->SetStateFontColor(ButtonFrom->GetStateFontColor()[1],1);
    this->SetStateText(ButtonFrom->GetStateText()[0],0);
    this->SetStateText(ButtonFrom->GetStateText()[1],1);
    this->SetStateBGColor(ButtonFrom->GetStateBGColor()[0],0);
    this->SetStateBGColor(ButtonFrom->GetStateBGColor()[1],1);
    this->SetStateBGPix(ButtonFrom->GetStateBGPix()[0],0);
    this->SetStateBGPix(ButtonFrom->GetStateBGPix()[1],1);
    this->SetStateSelectPix(ButtonFrom->GetStateSelectPix()[0],0);
    this->SetStateSelectPix(ButtonFrom->GetStateSelectPix()[1],1);
}

void TButton::SetPressMode(const uchar uc)
{
    m_PressMode = uc;
}

uchar TButton::GetPressMode() const
{
    return m_PressMode;
}

void TButton::SetVarName(const QString varName)
{
    QStringList list = varName.split(',');
    if(list.size() > 1)
    {
        m_VarName = list[0];
        m_VarState = list[1].toUShort();
        m_VarType = list[2].toShort();
    }
}

QString TButton::GetVarName() const
{
    return m_VarName;
}

uchar TButton::GetVarState() const
{
    return m_VarState;
}

char TButton::GetVarType() const
{
    return m_VarType;
}

void TButton::SetJsTactic(const QString js,uchar uc)
{
    if(uc > 0)
        m_JsTacticVec[1] = js;
    else
        m_JsTacticVec[0] = js;
}

QVector<QString> TButton::GetJsTactic() const
{
    return m_JsTacticVec;
}

void TButton::SetOpFunc(char c)
{
    if(c > 2)
        c++;
    m_euOpFunc = FunctionType(c);
}

char TButton::GetOpFunc() const
{
    char c = m_euOpFunc;
    if(c > 2)
        c--;
    return c;
}

void TButton::SetScreen(const QString screen)
{
    m_ScreenName = screen;
}

QString TButton::GetScreen() const
{
    return m_ScreenName;
}

void TButton::SetEnableExpre(const QString expre)
{
    m_EnableExpre = expre;
}

QString TButton::GetEnableExpre() const
{
    return m_EnableExpre;
}

void TButton::SetEnableState(const int enable)
{
    m_EnableState = enable;
}

int TButton::GetEnableState() const
{
    return m_EnableState;
}

void TButton::SetStateVarName(const QString var)
{
    QStringList list = var.split(',');
    if(list.size() > 1)
    {
        m_stateVarName = list[0];
        m_stateVarType = list[1].toShort();
    }
}

QString TButton::GetStateVarName() const
{
    return m_stateVarName;
}

uchar TButton::GetStateVarType() const
{
    return m_stateVarType;
}

void TButton::SetStateSelectPix(bool isSelect, const uchar uc)
{
    if(m_StateSelectPixVec.size() > uc)
        m_StateSelectPixVec[uc] = isSelect;
}

QVector<bool> TButton::GetStateSelectPix() const
{
    return m_StateSelectPixVec;
}

void TButton::SetStateBGPix(const QString str, const uchar uc)
{
    if(m_StateBGPixVec.size() > uc)
    {
        m_StateBGPixVec[uc] = str;
    }
}

QVector<QString> TButton::GetStateBGPix() const
{
    return m_StateBGPixVec;
}

void TButton::SetStateBGColor(const QColor color, const uchar uc)
{
    if(m_StateBGColorVec.size() > uc)
    {
        m_StateBGColorVec[uc] = color;
    }
}

QVector<QColor> TButton::GetStateBGColor() const
{
    return m_StateBGColorVec;
}

void TButton::SetStateFontColor(const QColor color, const uchar uc)
{
    if(m_StateFontColorVec.size() > uc)
    {
        m_StateFontColorVec[uc] = color;
    }
}

QVector<QColor> TButton::GetStateFontColor() const
{
    return m_StateFontColorVec;
}

void TButton::SetStateText(const QString text, const uchar uc)
{
    if(m_StateTextVec.size() > uc)
    {
        m_StateTextVec[uc] = text;
    }
}

QVector<QString> TButton::GetStateText() const
{
    return m_StateTextVec;
}

void TButton::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::PressMode:
        SetPressMode(value.toUShort());
        break;
    case CoAttributeWidget::JsTactic:
        SetJsTactic(value,uc);
        break;
    case CoAttributeWidget::SwitchScreen:
        SetScreen(value);
        break;
    case CoAttributeWidget::OPFunc:
        SetOpFunc(value.toShort());
        break;
    case CoAttributeWidget::VarName:
        SetVarName(value);
        break;
    case CoAttributeWidget::EnableExpre:
        SetEnableExpre(value);
        break;
    case CoAttributeWidget::EnableState:
        SetEnableState(value.toInt());
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    case CoAttributeWidget::StateVarName:
        SetStateVarName(value);
        break;
    case CoAttributeWidget::StateBGColor:
        SetStateBGColor(QColor(value),uc);
        break;
    case CoAttributeWidget::StateBGPix:
        SetStateBGPix(value,uc);
        break;
    case CoAttributeWidget::StateSelectPix:
        SetStateSelectPix(value.toInt(),uc);
        break;
    case CoAttributeWidget::StateFColor:
        SetStateFontColor(QColor(value),uc);
        break;
    case CoAttributeWidget::StateText:
        SetStateText(value,uc);
        break;
    default:
        break;
    }
}

void TButton::SetPressed(bool b)
{
    m_Pressed = b;
}

void TButton::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;        // 绘图区域的宽度
    double Height = m_StopY - m_StartY;       // 绘图区域的高度
    double CenterX = m_StartX + Width / 2;   // 绘图区域的中心X

    double Roundness = MIN(Width, Height) * 0.1; //圆角
    QLinearGradient lg1(CenterX, m_StartY, CenterX, m_StopY);
    lg1.setColorAt(0, TBase::GetNewColor(m_BackGroundColor,-20));
    lg1.setColorAt(0.4, m_BackGroundColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_BackGroundColor,20));
    painter->setBrush(lg1);
    QPainterPath OutLine;
    OutLine.addRoundedRect(m_StartX, m_StartY, Width, Height, Roundness, Roundness);
    painter->drawPath(OutLine);
    painter->fillPath(OutLine, lg1);

    if(!m_CurPixFile.isEmpty())
    {
        QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
        QString path = strUser + m_CurPixFile.right(m_CurPixFile.length() - 1);
        painter->drawPixmap(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY,QPixmap(path));
    }

    //文字
    painter->setPen(QPen(QBrush(m_CurFontColor),2.0));
    painter->setFont(m_font);
    painter->drawText(m_StartX, m_StartY, Width, Height, m_AlignVer | m_AlignHor |Qt::TextWordWrap, m_Text);
}

void TButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(GetType() == CoItem::Button)
    {
        if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
        {
            if(GetPower() >= AttributeInstance::GetInstance()->Power)
            {
                //判断是否可用
                m_IsSafe = true;
                if(!m_EnableExpre.isEmpty())
                {
                    QString strExpre = m_EnableExpre;
                    strExpre.replace("&","$");
                    StackManager stack;
                    if(!stack.AnalysisEvent(strExpre))
                    {
                        m_IsSafe = false;
                        goto ExpreError;
                    }
                }
                if(!m_stateVarName.isEmpty())
                    if(m_Timer)
                        m_Timer->stop();

                if(m_IsSafe)
                {
                    if(m_PressMode == 0)
                    {
                        if(m_SelectPixVec[1])
                            m_CurPixFile = m_PixFileVec[1];
                        else
                            m_CurPixFile = "";
                        m_BackGroundColor = m_BackColorVec[1];
                        m_CurFontColor = m_FontColor;
                    }
                    else
                    {
                        if(!m_Pressed)
                        {
                            if(m_SelectPixVec[1])
                                m_CurPixFile = m_PixFileVec[1];
                            m_BackGroundColor = m_BackColorVec[1];
                        }
                        else
                        {
                            if(m_SelectPixVec[0])
                                m_CurPixFile = m_PixFileVec[0];
                            m_BackGroundColor = m_BackColorVec[0];
                        }
                        m_Pressed = !m_Pressed;
                    }
                }
                if(!m_JsTacticVec[1].isEmpty())
                {
                    QString errorMsg = "";
                    QString strJs = QApplication::applicationDirPath() + tr("/脚本/") + m_JsTacticVec[1];
                    if(m_Js == NULL)
                    {
                        m_Js = new TJs(this,strJs);
                    }
                    else
                    {
                        m_Js->SetFile(strJs);
                        m_Js->Init();
                    }
                    m_Js->Call(tr("ExcFunc"),errorMsg);
                }

                if(!m_VarName.isEmpty())
                {
                    SetVar(m_VarName,m_VarState,1);
                }
            }
        }
    }
ExpreError:
    CoItem::mousePressEvent(event);
}

void TButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(GetType() == CoItem::Button)
    {
        if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
        {
            if(GetPower() >= AttributeInstance::GetInstance()->Power)
            {
                if(m_PressMode == 0 && m_IsSafe)
                {
                    if(m_SelectPixVec[0])
                        m_CurPixFile = m_PixFileVec[0];
                    else
                        m_CurPixFile = "";
                    m_BackGroundColor = m_BackColorVec[0];
                    m_CurFontColor = m_FontColor;

                    if(!m_JsTacticVec[0].isEmpty())
                    {
                        QString errorMsg = "";
                        QString strJs = QApplication::applicationDirPath() + tr("/脚本/") + m_JsTacticVec[0];
                        if(m_Js == NULL)
                        {
                            m_Js = new TJs(this,strJs);
                        }
                        else
                        {
                            m_Js->SetFile(strJs);
                            m_Js->Init();
                        }
                        m_Js->Call(tr("ExcFunc"),errorMsg);
                    }
                    if(!m_VarName.isEmpty() && m_VarState > 3)
                    {
                        SetVar(m_VarName,m_VarState,0);
                    }

                    if(!m_ScreenName.isEmpty())
                    {
                        emit mSwithScene(m_ScreenName,GetProName());
                    }
                    if(m_euOpFunc > -1)
                    {
                        emit mSendTo(m_euOpFunc,GetOPNameByType(m_euOpFunc));

                        if(m_euOpFunc == Function_Suspend)
                            m_euOpFunc = Function_Resume;
                        else if(m_euOpFunc == Function_Resume)
                            m_euOpFunc = Function_Suspend;
                    }
                }
                if(!m_stateVarName.isEmpty() && m_Timer != NULL)
                    if(!m_Timer->isActive())
                        m_Timer->start(m_AutoTime);
            }
        }
    }
    CoItem::mouseReleaseEvent(event);
}

void TButton::UpdateState()
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
       if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            if(!m_stateVarName.isEmpty())
            {
                bool isZero = false;
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
                            isZero = IntToBool(uc);
                    }
                    if(ioaddr.contains("Q"))
                    {
                        int iresult = BaseCalcFun::GetOutIOQBitValue(ioaddr,uc);
                        if(iresult == 1)
                            isZero = IntToBool(uc);
                    }
                }
                else
                {
                    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
                    isZero = IntToBool(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_VarName].initvalue);
                }

                if(isZero)
                {
                    m_CurFontColor = m_StateFontColorVec.at(1);
                    m_CurPixFile = m_StateBGPixVec.at(1);
                    m_BackGroundColor = m_StateBGColorVec.at(1);
                    m_Text = m_StateTextVec.at(1);
                }
                else
                {
                    m_CurFontColor = m_StateFontColorVec.at(0);
                    m_CurPixFile = m_StateBGPixVec.at(0);
                    m_BackGroundColor = m_StateBGColorVec.at(0);
                    m_Text = m_StateTextVec.at(0);
                }
                if(m_Count == 0)   //保证第一次刷新能刷到。
                {
                    ++m_Count;
                    m_IsZero = isZero;
                    update();
                }
                if(isZero != m_IsZero)
                {
                    m_IsZero = isZero;
                    update();
                }
            }
            else
            {
                m_Timer->stop();
                delete m_Timer;
                m_Timer = NULL;
            }
        }
    }
}

void TButton::SetVar(const QString name, uchar state,uchar uc)
{
    DBInstanceManager *db =  DBInstanceManager::getDBInstance();
    if(m_VarType == 1) //数值型
    {
        double dVal = db->DBVarStru.regnumMap[name].Initvalue;
        dVal = CalculateByState(dVal,state,uc);
        QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
        db->DBVarStru.regnumMap[name].Initvalue = dVal;
    }
    else if(m_VarType == 2)  //开关型
    {
        uchar dVal = db->DBVarStru.SwitchMap[name].initvalue;
        if(3 != state)
            dVal = CalculateByState(dVal,state,uc);
        else
        {
            if( 0 == dVal)
                dVal = 1;
            else
                dVal = 0;
        }
        if(db->DBVarStru.SwitchMap[name].ishwIO)
        {
            uchar uc = dVal;
            BaseCalcFun::AnalysisIOString(db->DBVarStru.SwitchMap[name].IOaddr,uc);
        }
        QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
        db->DBVarStru.SwitchMap[name].initvalue = dVal;
    }
    else if(m_VarType == 4) //计数类
    {
        int val = db->DBVarStru.countMap[name].curvalue;
        val = CalculateByState(val,state,uc);
        QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
        db->DBVarStru.countMap[name].curvalue = val;
    }
}

QString TButton::GetOPNameByType(FunctionType eu)
{
    switch (eu) {
    case Function_Start:
        return tr("启动");
        break;
    case Function_Suspend:
        return tr("暂停");
        break;
    case Function_Stop:
        return tr("停止");
        break;
    case Function_EmrgStop:
        return tr("急停");
        break;
    case Function_Reset:
        return tr("复位");
        break;
    case Function_ORG:
        return tr("回原点");
        break;
    case Function_Close:
        return tr("关闭程序");
        break;
    case Function_PowerOff:
        return tr("关机");
        break;
    case Function_Reboot:
        return tr("重启系统");
        break;
    default:
        break;
    }
    return tr("");
}

double TButton::CalculateByState(double value, uchar state,uchar uc)
{
    switch (state) {
    case 1: //置1
        return 1;
        break;
    case 2: //置0
        return 0;
        break;
    case 3: //取反
    {
        int n = value;
        return ~n;
        break;
    }
    case 4: // 按1松0
    {
        if(uc == 0)
            return 0;
        else
            return 1;
        break;
    }
    case 5: //按0松1
    {
        if(uc == 0)
            return 1;
        else
            return 0;
        break;
    }
    default:
        break;
    }
    return -1;
}
