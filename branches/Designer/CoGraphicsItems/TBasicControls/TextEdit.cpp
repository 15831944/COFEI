#include "TextEdit.h"
#include <QCursor>
#include <QKeyEvent>
#include <QDebug>
#include <QtCore>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <CoGraphics/Scene/Scene.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include "AttributeWidget.h"

TTextEdit::TTextEdit(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor) :
       CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::TextEdit);
    setFlag(ItemIsFocusable);
    m_jsExc = NULL;
    m_TextType = 1;
    m_Text = tr("");
    m_strInputVarName = "";
    m_strOutputVarName = "";
    m_VarType = -1;
    m_strUnit = tr("");
    m_ValueFormat = 0;
    m_IntAccurecy = 0;
    m_DecAccurecy = 3;
    m_EnableExpre = "";
    m_EnableState = 0;
    m_dMaxVal = 1.79e+308;
    m_dMinVal = -1.79e+308;
    m_strInput = tr("");
    m_strJsTactic = tr("");
    m_IsSafe = true;
    m_bPreview = false;
    m_AutoTime = 300;
    isFousOut = true;
    connect(this,&TTextEdit::TextChange,this,&TTextEdit::HandleText);

    m_Timer = new QTimer();
    connect(m_Timer,&QTimer::timeout,this,&TTextEdit::UpdateText);
}

TTextEdit::~TTextEdit()
{

}

CoItem* TTextEdit::Copy()
{
    TTextEdit *TextEdit = new TTextEdit();

    TextEdit->Copy(this);

    return TextEdit;
}

void TTextEdit::Copy(TTextEdit *TextEditFrom)
{
    if(NULL == TextEditFrom)
    {
        return;
    }
    CoWidgets::Copy(TextEditFrom);
    this->SetBackColor(TextEditFrom->GetBackColor().at(0),0);
    this->SetSelectPix(TextEditFrom->GetSelectPix().at(0),0);
    this->SetPixFile(TextEditFrom->GetPixFiles().at(0),0);
    this->SetVarName(QString("%1,%2,%3").arg(TextEditFrom->GetInputVarName())
                     .arg(TextEditFrom->GetOutputVarName())
                     .arg(QString::number(TextEditFrom->GetVarType())));
    this->SetJsTactic(TextEditFrom->GetJsTactic());
    this->SetAutoTime(TextEditFrom->GetAutoTime());
    this->SetUnit(TextEditFrom->GetUnit());
    this->SetValueFormat(TextEditFrom->GetValueFormat());
    this->SetIntAccurecy(TextEditFrom->GetIntAccurecy());
    this->SetDecAccurecy(TextEditFrom->GetDecAccurecy());
    this->SetMaxValue(TextEditFrom->GetMaxValue());
    this->SetMinValue(TextEditFrom->GetMinValue());
    this->SetEnableExpre(TextEditFrom->GetEnableExpre());
    this->SetEnableState(TextEditFrom->GetEnableState());
}

void TTextEdit::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::VarName:
        SetVarName(value);
        break;
    case CoAttributeWidget::Unit:
        SetUnit(value);
        break;
    case CoAttributeWidget::ValueFormat:
        SetValueFormat(value.toShort());
        break;
    case CoAttributeWidget::IntAccurecy:
        SetIntAccurecy(value.toShort());
        break;
    case CoAttributeWidget::DecAccurecy:
        SetDecAccurecy(value.toShort());
        break;
    case CoAttributeWidget::MaxValue:
        SetMaxValue(value.toDouble());
        break;
    case CoAttributeWidget::MinValue:
        SetMinValue(value.toDouble());
        break;
    case CoAttributeWidget::EnableExpre:
        SetEnableExpre(value);
        break;
    case CoAttributeWidget::EnableState:
        SetEnableState(value.toInt());
        break;
    case CoAttributeWidget::JsTactic:
        SetJsTactic(value);
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    default:
        break;
    }
}

void TTextEdit::SetVarName(const QString str)
{
    QStringList list = str.split(',');
    if(list.size() > 2)
    {
        m_strInputVarName = list[0];
        m_strOutputVarName = list[1];
        m_VarType = list[2].toShort();
    }
}

QString TTextEdit::GetInputVarName() const
{
    return m_strInputVarName;
}

QString TTextEdit::GetOutputVarName() const
{
    return m_strOutputVarName;
}

char TTextEdit::GetVarType() const
{
    return m_VarType;
}

void TTextEdit::SetUnit(const QString str)
{
    m_strUnit = str;
}

QString TTextEdit::GetUnit() const
{
    return m_strUnit;
}

void TTextEdit::SetValueFormat(const uchar uc)
{
    m_ValueFormat = uc;
    if(m_bPreview)
        SetTextByFormat(m_ValueFormat);
}

uchar TTextEdit::GetValueFormat() const
{
    return m_ValueFormat;
}

void TTextEdit::SetIntAccurecy(const uchar acc)
{
    m_IntAccurecy = acc;
    if(m_bPreview)
        SetTextByFormat(m_ValueFormat);
}

uchar TTextEdit::GetIntAccurecy() const
{
    return m_IntAccurecy;
}

void TTextEdit::SetDecAccurecy(const uchar acc)
{
    m_DecAccurecy = acc;
    if(m_bPreview)
        SetTextByFormat(m_ValueFormat);
}

uchar TTextEdit::GetDecAccurecy() const
{
    return m_DecAccurecy;
}

void TTextEdit::SetMaxValue(const double d)
{
    m_dMaxVal = d;
}

double TTextEdit::GetMaxValue() const
{
    return m_dMaxVal;
}

void TTextEdit::SetMinValue(const double d)
{
    m_dMinVal = d;
}

double TTextEdit::GetMinValue() const
{
    return m_dMinVal;
}

void TTextEdit::SetEnableExpre(const QString str)
{
    m_EnableExpre = str;
}

QString TTextEdit::GetEnableExpre() const
{
    return m_EnableExpre;
}

void TTextEdit::SetEnableState(const int ch)
{
    m_EnableState = ch;
}

int TTextEdit::GetEnableState() const
{
    return m_EnableState;
}

void TTextEdit::SetJsTactic(const QString str)
{
    m_strJsTactic = str;
}

QString TTextEdit::GetJsTactic() const
{
    return m_strJsTactic;
}

void TTextEdit::SetPreviewMode(bool b)
{
    m_bPreview = b;
}

void TTextEdit::Draw(QPainter *painter)
{
    painter->drawRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
    if(!m_PixFileVec[0].isEmpty())
    {
        QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
        QString path = strUser + m_PixFileVec[0].right(m_PixFileVec[0].length() - 1);

        painter->drawPixmap(m_StartX,m_StartY,m_StopX - m_StartX, m_StopY - m_StartY,QPixmap(path));
    }

    painter->setPen(QPen(QBrush(m_FontColor),2.0));
    painter->setFont(m_font);

    QString str = m_Text;
    if(m_ValueFormat > 5)
        str.replace(QRegExp("."),"*");  
    painter->drawText(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY,
                      m_AlignVer | m_AlignHor ,str + m_strUnit);
    if(!isFousOut)
    {
        QFontMetrics metrics = painter->fontMetrics();
        int textWidth = metrics.width(str + m_strUnit);
        QFont font = m_font;
        font.setPointSize(font.pointSize()+3);
        painter->setFont(font);
        painter->setPen(QPen(QBrush(m_FontColor),3));
        painter->drawText(m_StartX + textWidth, m_StartY, m_StopX - m_StartX-textWidth, m_StopY - m_StartY,
                          m_AlignVer | m_AlignHor ,"|");
    }
}

void TTextEdit::SetTextByFormat(int type)
{
    if(m_strInput.isEmpty() && m_bPreview)
    {
        if(type > 4)
            m_Text = tr("string");
        else
            m_Text = tr("80.345678");
    }

    switch (type)
    {
    case 0: //浮点
    case 1:
    {
        double d = m_Text.toDouble();
        QString temp = QString::number(d,'f',10);
        //截取到小数点后4位
        int index = -1;
        if(m_DecAccurecy > 0)
        {
            index = temp.indexOf('.') + m_DecAccurecy + 1;
        }
        else
        {
            index = temp.indexOf('.');
        }
        m_Text = temp.left(index);
        break;
    }
    case 2: //四舍五入
    {
        double d = m_Text.toDouble();
        int size = m_IntAccurecy + m_DecAccurecy;

        if(m_DecAccurecy > 0)
        {
            ++size;

            QString temp = QString::number(d,'f',10);
            //截取到小数点后4位
            int index = temp.indexOf('.') + m_DecAccurecy + 1;
            temp = temp.left(index);
            d = temp.toDouble();
        }
        m_Text = QString(tr("%1").arg(d,size,'f',m_DecAccurecy,QLatin1Char('0')));
        break;
    }
    case 3: //前导0
    {
        double d = m_Text.toDouble();
        m_Text = QString::number(d,'f',m_DecAccurecy);
        break;
    }
    case 4: //四舍五入+前导0
    {
        double d = m_Text.toDouble();
        int size = m_IntAccurecy + m_DecAccurecy;
        if(m_DecAccurecy > 0)
            ++size;
        m_Text = QString(tr("%1").arg(d,size,'f',m_DecAccurecy,QLatin1Char('0')));
        break;
    }
    case 5: //字符
        break;
    case 6: //密码
        break;
    default:
        break;
    }
}

void TTextEdit::focusInEvent(QFocusEvent *event)
{
    setFocus();
    isFousOut = false;
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            m_IsSafe = true;
            if(!m_EnableExpre.isEmpty())
            {
                StackManager stack;
                if(!stack.AnalysisEvent(m_EnableExpre))
                {
                    m_IsSafe = false;
                    goto Error;
                }
            }

            if(!m_strOutputVarName.isEmpty())
                m_Timer->stop();
        }
    }

    Error:
    QGraphicsItem::focusInEvent(event);
}

void TTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    isFousOut = true;
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            if(!m_strOutputVarName.isEmpty() && !m_Timer->isActive())
                m_Timer->start(m_AutoTime);
        }
    }
}

void TTextEdit::UpdateText()
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        //if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            QString strTemp = m_Text;
            if(m_strOutputVarName.isEmpty())
            {
                m_Timer->stop();
            }
            else
            {
                DBInstanceManager *db =  DBInstanceManager::getDBInstance();
                QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
                if(m_VarType == 0) //字符型
                {
                    m_Text = db->DBVarStru.regtextMap[m_strOutputVarName].textval;
                }
                else if(m_VarType == 1)  //数值型
                {
                    m_Text = QString::number(db->DBVarStru.regnumMap[m_strOutputVarName].Initvalue);
                }
            }
	    if(strTemp.compare(m_Text) != 0)
                update();
        }
    }
}

void TTextEdit::HandleText(const QString &Text)
{
    if(!m_strInputVarName.isEmpty())
    {
        DBInstanceManager *db =  DBInstanceManager::getDBInstance();
        if(m_VarType == 0) //字符型
        {
            QString str = Text;
            QByteArray ba = str.toLatin1();
            QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
            strncpy(db->DBVarStru.regtextMap[m_strInputVarName].textval,ba.data(),64);
        }
        else if(m_VarType == 1)  //数值型
        {
            double d = Text.toDouble();
            if(d > GetMaxValue())
                d = GetMaxValue();
            else if(d < GetMinValue())
                d = GetMinValue();
            m_Text = QString::number(d);

            if(m_jsExc)
                delete m_jsExc;

            m_jsExc = new CoJS();
            if(m_strInputVarName == "BUILT_IN_SPEED")
            {
                m_jsExc->SetSpeed(d);
            }
            else if(m_strInputVarName == "BUILT_IN_ACC")
            {
                m_jsExc->SetAcc(d);
            }
            else if(m_strInputVarName == "BUILT_IN_DEC")
            {
                m_jsExc->SetDcc(d);
            }
            else if(m_strInputVarName == "BUILT_IN_SPEEDRATE")
            {
                m_jsExc->SetRatio(d);
            }
            else
            {
                QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
                db->DBVarStru.regnumMap[m_strInputVarName].Initvalue = d;
            }
        }
    }
}

void TTextEdit::keyReleaseEvent(QKeyEvent *event)
{
    if(m_IsSafe)
    {
        //bool Changed = false;
        if(Qt::Key_Return == event->key() || Qt::Key_Enter == event->key())
        {
            if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
            {
                if(GetPower() >= AttributeInstance::GetInstance()->Power)
                {
                    SetTextByFormat(m_ValueFormat);
                    //m_Text += event->text();
                    emit TextChange(m_Text);

                    if(!m_strOutputVarName.isEmpty() && !m_Timer->isActive())
                        m_Timer->start(m_AutoTime);

                    if(!m_strJsTactic.isEmpty())
                    {
                        QString errorMsg = "";
                        QString strJs = QApplication::applicationDirPath() + tr("/脚本/") + m_strJsTactic;
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
        else if(Qt::Key_Backspace != event->key())
        {
            if(event->text().length() > 0)
            {
                m_Text += event->text();
                m_strInput += event->text();
                //Changed = true;
            }
        }
        else
        {
            if(m_Text.length() > 0)
            {
                m_Text = m_Text.left(m_Text.length() - 1);
                m_strInput = m_strInput.left(m_strInput.length() - 1);
                //Changed = true;
            }
        }
        Update();
    }
    CoWidgets::keyReleaseEvent(event);
}

