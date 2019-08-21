#include <QDebug>
#include <QtCore>
#include <QRegExp>
#include "math.h"
#include "Label.h"
#include "AttributeWidget.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

TLabel::TLabel(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor,
             const QString &Text) :
        CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Label);

    m_FontBlod = false;
    m_FontItalic = false;
    m_FontUnderLine = false;
    m_FontStrikeOut = false;
    m_FontSpacing = 0;

    m_Text = Text;
    m_strTextBackup = Text;
    m_OutputMode = 0;
    m_strVarName = "";
    m_VarType = -1;
    m_strUnit = "";
    m_ValueType = 1;
    m_ValueFormat = 8;
    m_IntAccurecy = 0;
    m_DecAccurecy = 3;
    m_strJsPath = "";
    m_AutoTime = 300;

    m_dPreview = 80.345678;

    m_Timer = new QTimer();
    connect(m_Timer,&QTimer::timeout,this,&TLabel::UpdateText);
}

TLabel::~TLabel()
{}

CoItem *TLabel::Copy()
{
    TLabel *Label = new TLabel();

    Label->Copy(this);

    return Label;
}

void TLabel::Copy(TLabel *LableFrom)
{
    if(NULL == LableFrom)
    {
        return;
    }

    CoWidgets::Copy(LableFrom);
    this->SetOutputMode(LableFrom->GetOutputMode());
    this->SetBackColor(LableFrom->GetBackColor().at(0),0);
    this->SetSelectPix(LableFrom->GetSelectPix().at(0),0);
    this->SetPixFile(LableFrom->GetPixFiles().at(0),0);

//    this->SetFontBlod(LableFrom->GetFontBlod());
//    this->SetFontItalic(LableFrom->GetFontItalic());
//    this->SetFontSpacing(LableFrom->GetFontSpacing());
//    this->SetFontUnderLine(LableFrom->GetFontUnderLine());
    //    this->SetFontStrikeOut(LableFrom->GetFontStrikeOut());


    this->SetText(LableFrom->GetText());
    this->SetUnit(LableFrom->GetUnit());
    this->SetAutoTime(LableFrom->GetAutoTime());
    this->SetValueType(LableFrom->GetValueType());
    this->SetValueFormat(LableFrom->GetValueFormat());
    this->SetIntAccurecy(LableFrom->GetIntAccurecy());
    this->SetDecAccurecy(LableFrom->GetDecAccurecy());
    this->SetVarName(QString("%1,%2").arg(LableFrom->GetVarName())
                     .arg(QString::number(LableFrom->GetVarType())));
}

void TLabel::SetAttributeEvent(const int type, const QString value, uchar uc /*=0*/)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::Text:
        SetText(value);
        break;
    case CoAttributeWidget::OutputMode:
        SetOutputMode(value.toShort());
        break;
    case CoAttributeWidget::VarName:
        SetVarName(value);
        break;
    case CoAttributeWidget::Unit:
        SetUnit(value);
        break;
    case CoAttributeWidget::ValueType:
        SetValueType(value.toShort());
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
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    case CoAttributeWidget::JsTactic:
        SetJsTactic(value);
        break;
    default:
        break;
    }
}

void TLabel::SetOutputMode(const uchar uc)
{
    m_OutputMode = uc;

    if(m_OutputMode > 0)
        m_AutoTime = 300;
    else
        m_AutoTime = 0;

    SetTextByFormat(m_ValueFormat);
}

uchar TLabel::GetOutputMode() const
{
    return m_OutputMode;
}

void TLabel::SetVarName(const QString str)
{
    QStringList list = str.split(',');
    if(list.size() > 1)
    {
        m_strVarName = list[0];
        m_VarType = list[1].toShort();
    }
}

QString TLabel::GetVarName() const
{
    return m_strVarName;
}

char TLabel::GetVarType() const
{
    return m_VarType;
}

void TLabel::SetUnit(const QString str)
{
    m_strUnit = str;
}

QString TLabel::GetUnit() const
{
    return m_strUnit;
}

void TLabel::SetValueType(const uchar uc)
{
    m_ValueType = uc;
    if(m_ValueType > 0)
    {
        m_ValueFormat = 8;
        SetTextByFormat(m_ValueFormat);
    }
    else
    {
        m_ValueFormat = 0;
        SetTextByFormat(m_ValueFormat);
    }
}

uchar TLabel::GetValueType() const
{
    return m_ValueType;
}

void TLabel::SetValueFormat(const uchar uc)
{
    m_ValueFormat = uc;
    SetTextByFormat(m_ValueFormat);
}

uchar TLabel::GetValueFormat() const
{
    return m_ValueFormat;
}

void TLabel::SetIntAccurecy(const uchar acc)
{
    m_IntAccurecy = acc;
    SetTextByFormat(m_ValueFormat);
}

uchar TLabel::GetIntAccurecy() const
{
    return m_IntAccurecy;
}

void TLabel::SetDecAccurecy(const uchar acc)
{
    m_DecAccurecy = acc;
    SetTextByFormat(m_ValueFormat);
}

uchar TLabel::GetDecAccurecy() const
{
    return m_DecAccurecy;
}

void TLabel::SetJsTactic(const QString path)
{
    m_strJsPath = path;
}

QString TLabel::GetJsTactic() const
{
    return m_strJsPath;
}


void TLabel::SetFontBlod(bool Blod)
{
    m_FontBlod = Blod;
}

bool TLabel::GetFontBlod() const
{
    return m_FontBlod;
}

void TLabel::SetFontItalic(bool Italic)
{
    m_FontItalic = Italic;
}

bool TLabel::GetFontItalic() const
{
    return m_FontItalic;
}

void TLabel::SetFontUnderLine(bool UnderLine)
{
    m_FontUnderLine = UnderLine;
}

bool TLabel::GetFontUnderLine() const
{
    return m_FontUnderLine;
}

void TLabel::SetFontStrikeOut(bool StrikeOut)
{
    m_FontStrikeOut = StrikeOut;
}

bool TLabel::GetFontStrikeOut() const
{
    return m_FontStrikeOut;
}

void TLabel::SetFontSpacing(int Spacing)
{
    m_FontSpacing = Spacing;
}

int TLabel::GetFontSpacing() const
{
    return m_FontSpacing;
}

void TLabel::Draw(QPainter *painter)
{
    painter->setPen(QColor(255,255,255,0));
    painter->setBrush(QBrush(m_BackColorVec[0]));
    painter->drawRect(m_StartX,m_StartY,m_StopX - m_StartX, m_StopY - m_StartY);

    if(!m_PixFileVec[0].isEmpty())
    {
        QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
        QString path = strUser + m_PixFileVec[0].right(m_PixFileVec[0].length() - 1);
        painter->drawPixmap(m_StartX,m_StartY,m_StopX - m_StartX, m_StopY - m_StartY,QPixmap(path));
    }

//    m_font.setBold(m_FontBlod);
//    m_font.setItalic(m_FontItalic);
//    m_font.setUnderline(m_FontUnderLine);
//    m_font.setStrikeOut(m_FontStrikeOut);
//    m_font.setLetterSpacing(QFont::AbsoluteSpacing, m_FontSpacing);
//    if(m_OutputMode >= 1)
//    {
//        QTimer * timer = new QTimer;
//        connect(timer,&QTimer::timeout,this,&TLabel::Test);
//        timer.start(300);
//    }
    painter->setPen(QPen(QBrush(m_FontColor),2.0));
    painter->setFont(m_font);
    painter->drawText(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY,
                      m_AlignVer | m_AlignHor | Qt::TextWordWrap, m_Text + m_strUnit);
}

void TLabel::SetTextByFormat(int type)
{
    ValueType euType = ValueType(type);
    switch (euType)
    {
    case Double: //浮点
    {
        QString temp = QString::number(m_dPreview,'f',10);
        //截取到小数点后4位
        int index = temp.indexOf('.') + m_DecAccurecy + 1;
        m_Text = temp.left(index);
        break;
    }
    case Dec: //十进制
    {
        int n = int(m_dPreview);
        m_Text = QString::number(n);
        break;
    }
    case Hex: //十六进制
    {
        int n = int(m_dPreview);
        m_Text = QString(tr("0X%1").arg(n,0,16));
        break;
    }
    case Bin: //二进制
    {
        int n = int(m_dPreview);
        m_Text = QString::number(n,2);
        break;
    }
    case BinAndFrontZero: //二进制+前导0
    {
        int n = int(m_dPreview);
        m_Text = QString(tr("%1").arg(n,m_IntAccurecy,2,QLatin1Char('0')));
        break;
    }
    case Round: //四舍五入
    {
        m_Text = QString::number(m_dPreview,'f',m_DecAccurecy);
        break;
    }
    case FrontZero: //前导0
    {
        double d = m_dPreview;
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
    case RoundAndFrontZero: //四舍五入+前导0
    {
        int size = m_IntAccurecy + m_DecAccurecy;
        if(m_DecAccurecy > 0)
            ++size;
        m_Text = QString(tr("%1").arg(m_dPreview,size,'f',m_DecAccurecy,QLatin1Char('0')));
        break;
    }
    case String: //字符
    {
        m_Text = m_strTextBackup;
        break;
    }
    case Pwd: //密码
    {
        m_Text.replace(QRegExp("."),"*");
        break;
    }
    default:
        break;
    }
}

void TLabel::SetText(const QString Text)
{
    CoWidgets::SetText(Text);
    m_strTextBackup = m_Text;
}

void TLabel::UpdateText()
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        //if(GetPower() >= AttributeInstance::GetInstance()->Power)
        {
            if(m_OutputMode == 1)
            {
                QString strTextBefore = m_Text;
                if(!m_strVarName.isEmpty())
                {
                    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
                    if(m_VarType == 0)  // 字符串型
                        m_Text = DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[m_strVarName].textval;
                    else if(m_VarType == 1) // 数值型
                        m_Text = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[m_strVarName].Initvalue);
                    else if(m_VarType == 3) // 计时类
                        m_Text = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.timeMap[m_strVarName].curvalue);
                    else if(m_VarType == 4) // 计数类
                        m_Text = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.countMap[m_strVarName].curvalue);
                    else
                         m_Text = QString::number(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[m_strVarName].inputvalue);
                }

                if(!m_strJsPath.isEmpty())
                {
                    QString errorMsg = "";
                    QString strJs = QApplication::applicationDirPath() + tr("/脚本/") + m_strJsPath;
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

                if(strTextBefore.compare(m_Text) != 0)
                    update();
            }
        }
    }
}
