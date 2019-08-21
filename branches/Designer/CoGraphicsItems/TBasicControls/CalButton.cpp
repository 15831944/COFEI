#include <QDebug>
#include <QtCore>
#include "AttributeWidget.h"
#include <DycLibBase/StackManager/stackmanager.h>
#include "CalButton.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

TCalButton::TCalButton(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor)
    :CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::CalBtn);
    m_Text = tr("CalButton");
    m_EnableExpre = "";
    m_EnableState = 0;
    m_IsSafe = true;
    m_JsTacticVec.reserve(2);
    m_JsTacticVec << ""<< "";

    m_strCalVarComb1 = tr("");
    m_strCalVarComb2 = tr("");
    m_strCalVarComb3 = tr("");
    m_strCalVarComb4 = tr("");
    m_strCalVarComb5 = tr("");
    m_strCalVarComb6 = tr("");

}

CoItem *TCalButton::Copy()
{
    TCalButton *pCalBtn = new TCalButton();
    pCalBtn->Copy(this);
    return pCalBtn;
}

void TCalButton::Copy(TCalButton *calBtn)
{
    if(NULL == calBtn)
        return;
    CoWidgets::Copy(calBtn);
    this->SetBackColor(calBtn->GetBackColor().at(0),0);
    this->SetBackColor(calBtn->GetBackColor().at(1),1);
    this->SetSelectPix(calBtn->GetSelectPix().at(0),0);
    this->SetSelectPix(calBtn->GetSelectPix().at(1),1);
    this->SetPixFile(calBtn->GetPixFiles().at(0),0);
    this->SetPixFile(calBtn->GetPixFiles().at(1),1);
    this->SetText(calBtn->GetText());
    this->SetJsTactic(calBtn->GetJsTactic()[0],0);
    this->SetJsTactic(calBtn->GetJsTactic()[1],1);
    this->SetCalVar(calBtn->GetCalVarComb1(),1);
    this->SetCalVar(calBtn->GetCalVarComb2(),2);
    this->SetCalVar(calBtn->GetCalVarComb3(),3);
    this->SetCalVar(calBtn->GetCalVarComb4(),4);
    this->SetCalVar(calBtn->GetCalVarComb5(),5);
    this->SetCalVar(calBtn->GetCalVarComb6(),6);
    this->SetEnableExpre(calBtn->GetEnableExpre());
    this->SetEnableState(calBtn->GetEnableState());
}

void TCalButton::SetAttributeEvent(const int type, const QString value, uchar uc/* = 0*/)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type) {
    case CoAttributeWidget::EnableExpre:
        SetEnableExpre(value);
        break;
    case CoAttributeWidget::EnableState:
        SetEnableState(value.toInt());
        break;
    case CoAttributeWidget::CalVar:
        SetCalVar(value,uc);
        break;
    case CoAttributeWidget::JsTactic:
        SetJsTactic(value,uc);
        break;
    default:
        break;
    }
}

void TCalButton::SetCalVar(const QString varInfo, uchar uc)
{
    switch (uc) {
    case 1: m_strCalVarComb1 = varInfo;
        break;
    case 2: m_strCalVarComb2 = varInfo;
        break;
    case 3: m_strCalVarComb3 = varInfo;
        break;
    case 4: m_strCalVarComb4 = varInfo;
        break;
    case 5: m_strCalVarComb5 = varInfo;
        break;
    case 6: m_strCalVarComb6 = varInfo;
        break;
    default:
        break;
    }
}

QString TCalButton::GetCalVarComb1() const
{
    return m_strCalVarComb1;
}

QString TCalButton::GetCalVarComb2() const
{
    return m_strCalVarComb2;
}

QString TCalButton::GetCalVarComb3() const
{
    return m_strCalVarComb3;
}

QString TCalButton::GetCalVarComb4() const
{
    return m_strCalVarComb4;
}

QString TCalButton::GetCalVarComb5() const
{
    return m_strCalVarComb5;
}

QString TCalButton::GetCalVarComb6() const
{
    return m_strCalVarComb6;
}

void TCalButton::SetJsTactic(const QString js, uchar uc)
{
    if(uc > 0)
        m_JsTacticVec[1] = js;
    else
        m_JsTacticVec[0] = js;
}

QVector<QString> TCalButton::GetJsTactic() const
{
    return m_JsTacticVec;
}

void TCalButton::SetEnableExpre(const QString expre)
{
    m_EnableExpre = expre;
}

QString TCalButton::GetEnableExpre() const
{
    return m_EnableExpre;
}

void TCalButton::SetEnableState(const int enable)
{
    m_EnableState = enable;
}

int TCalButton::GetEnableState() const
{
    return m_EnableState;
}

void TCalButton::Draw(QPainter *painter)
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
    painter->setPen(QPen(QBrush(m_FontColor),2.0));
    painter->setFont(m_font);
    painter->drawText(m_StartX, m_StartY, Width, Height, m_AlignVer | m_AlignHor |Qt::TextWordWrap, m_Text);

}

void TCalButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
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

            if(!m_strCalVarComb1.isEmpty())
            {
                ExcCal(m_strCalVarComb1);
            }
            if(!m_strCalVarComb2.isEmpty())
            {
                ExcCal(m_strCalVarComb2);
            }
            if(!m_strCalVarComb3.isEmpty())
            {
                ExcCal(m_strCalVarComb3);
            }
            if(!m_strCalVarComb4.isEmpty())
            {
                ExcCal(m_strCalVarComb4);
            }
            if(!m_strCalVarComb5.isEmpty())
            {
                ExcCal(m_strCalVarComb5);
            }
            if(!m_strCalVarComb6.isEmpty())
            {
                ExcCal(m_strCalVarComb6);
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
        }
    }
ExpreError:
    CoItem::mousePressEvent(event);
}

void TCalButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void TCalButton::ExcCal(QString calVarComb)
{
    QStringList list = calVarComb.split(",");
    if(list.size() > 3)
    {
        DBInstanceManager *db =  DBInstanceManager::getDBInstance();

        QString strIn = list.at(0);
        QString strOut = list.at(2);

        float_t fInVal = db->DBReaTimInfoStru.reatiminfomap[strIn].inputvalue/1000.0;
        QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
        db->DBVarStru.regnumMap[strOut].Initvalue = fInVal;
    }
}
