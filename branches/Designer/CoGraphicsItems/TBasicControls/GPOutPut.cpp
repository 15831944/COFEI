#include <QDebug>
#include <QtCore>
#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include "AttributeWidget.h"
#include "GPOutPut.h"

TGPOutPut::TGPOutPut(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor)
    : CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::GPOutPut);
    m_Text = tr("GPOutPut");
    m_Pressed = false;
    m_IsSafe = true;

    m_IOName = "";
    m_IOState = -1;
    m_JsTacticVec.reserve(2);
    m_JsTacticVec << ""<< "";
    m_EnableExpre = "";
    m_EnableState = 0;
}

CoItem *TGPOutPut::Copy()
{
    TGPOutPut *gpInput = new TGPOutPut();

    gpInput->Copy(this);

    return gpInput;
}

void TGPOutPut::Copy(TGPOutPut *IoFrom)
{
    if(NULL == IoFrom)
    {
        return;
    }
    CoWidgets::Copy(IoFrom);
    this->SetBackColor(IoFrom->GetBackColor().at(0),0);
    this->SetBackColor(IoFrom->GetBackColor().at(1),1);
    this->SetSelectPix(IoFrom->GetSelectPix().at(0),0);
    this->SetSelectPix(IoFrom->GetSelectPix().at(1),1);
    this->SetPixFile(IoFrom->GetPixFiles().at(0),0);
    this->SetPixFile(IoFrom->GetPixFiles().at(1),1);

    this->SetJsTactic(IoFrom->GetJsTactic()[0],0);
    this->SetJsTactic(IoFrom->GetJsTactic()[1],1);
    this->SetEnableExpre(IoFrom->GetEnableExpre());
    this->SetEnableState(IoFrom->GetEnableState());
    this->SetIOName(QString("%1,%2").arg(IoFrom->GetIOName())
                    .arg(QString::number(IoFrom->GetIOState())));
}

void TGPOutPut::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::JsTactic:
        SetJsTactic(value,uc);
        break;
    case CoAttributeWidget::VarName:
        SetIOName(value);
        break;
    case CoAttributeWidget::EnableExpre:
        SetEnableExpre(value);
        break;
    case CoAttributeWidget::EnableState:
        SetEnableState(value.toInt());
        break;
    default:
        break;
    }
}

void TGPOutPut::SetIOName(const QString name)
{
    QStringList list = name.split(',');
    if(list.size() > 1)
    {
        m_IOName = list[0];
        m_IOState = list[1].toUShort();
    }
}

QString TGPOutPut::GetIOName() const
{
    return m_IOName;
}

char TGPOutPut::GetIOState() const
{
    return m_IOState;
}

void TGPOutPut::SetJsTactic(const QString js, uchar uc)
{
    if(uc > 0)
        m_JsTacticVec[1] = js;
    else
        m_JsTacticVec[0] = js;
}

QVector<QString> TGPOutPut::GetJsTactic() const
{
    return m_JsTacticVec;
}

void TGPOutPut::SetEnableExpre(const QString expre)
{
    m_EnableExpre = expre;
}

QString TGPOutPut::GetEnableExpre() const
{
    return m_EnableExpre;
}

void TGPOutPut::SetEnableState(const int state)
{
    m_EnableState = state;
}

int TGPOutPut::GetEnableState() const
{
    return m_EnableState;
}

void TGPOutPut::Draw(QPainter *painter)
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
    painter->setPen(m_FontColor);
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
    painter->setPen(QPen(QBrush(m_FontColor),2.0));
    painter->setFont(m_font);
    painter->drawText(m_StartX, m_StartY, Width, Height, m_AlignVer | m_AlignHor |Qt::TextWordWrap, m_Text);
}

void TGPOutPut::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
            if(m_IsSafe)
            {
                if(m_SelectPixVec[1])
                    m_CurPixFile = m_PixFileVec[1];
                else
                    m_CurPixFile = "";
                m_BackGroundColor = m_BackColorVec[1];
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
            if(!m_IOName.isEmpty())
            {
                //TODO:IO写入
            }
        }
    }

ExpreError:
    CoItem::mousePressEvent(event);
}

void TGPOutPut::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            if(m_IsSafe)
            {
                if(m_SelectPixVec[0])
                    m_CurPixFile = m_PixFileVec[0];
                else
                    m_CurPixFile = "";
                m_BackGroundColor = m_BackColorVec[0];

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
            }
        }
    }

    CoItem::mouseReleaseEvent(event);
}
