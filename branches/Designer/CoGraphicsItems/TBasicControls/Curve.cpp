#include <QDebug>

#include "AttributeWidget.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include "Curve.h"

TCurve::TCurve(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
    TCustomPlot(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_strTitle = tr("曲线图");
    m_VecGridColor.clear();
    m_VecGridColor.reserve(4);
    QColor color = QColor(170, 170, 170);
    m_VecGridColor<<color<<color<< color<<color;
    m_strAxisLblTxtList.clear();
    m_strAxisLblTxtList<<"X"<<"Y";
    m_XAxisCount = 5;
    m_YAxisCount = 4;
    m_BackColorVec[0] = Qt::white;
    m_IsGridVisible = 1;
    m_VecAxisLblColor.clear();
    m_VecAxisLblColor.reserve(2);
    color = Qt::black;
    m_VecAxisLblColor<<color<<color;
    m_strTimeFormat = "%m:%s";
    m_strTimeUnit = "";
    m_nXAxisLength = 100;
    m_nDecAccurecy = 0;
    m_dMax = 10;
    m_dMin = 0;
    m_AutoTime = 30;
    m_strCtrlVarName = "";
    m_CtrlVarType = -1;
    m_strVarName = "";
    m_VarType = -1;
    m_CurveColor = Qt::red;
    m_bRefresh = false;

    this->axisRect()->setupFullAxesBox();
    this->addGraph();
    this->graph(0)->setPen(QPen(m_CurveColor));
    this->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    //标题
    m_pTitleEle = new QCPTextElement(this,m_strTitle,QFont("sans", 10, QFont::Normal));
    this->plotLayout()->insertRow(0);
    this->plotLayout()->addElement(0,0,m_pTitleEle);
    //背景色
    this->setBackground(QBrush(m_BackColorVec[0]));
    //X轴
    this->xAxis->setLabel(m_strAxisLblTxtList[0]);
    this->xAxis->setLabelColor(m_VecAxisLblColor[0]);
    //Y轴
    this->yAxis->setLabel(m_strAxisLblTxtList[1]);
    this->yAxis->setLabelColor(m_VecAxisLblColor[1]);
    //子网格可见
    this->xAxis->grid()->setSubGridVisible(true);
    this->yAxis->grid()->setSubGridVisible(true);
    //网格颜色
    this->xAxis->grid()->setPen(QPen(m_VecGridColor[0]));
    this->xAxis->grid()->setSubGridPen(QPen(m_VecGridColor[1],1,Qt::DotLine));
    this->yAxis->grid()->setPen(QPen(m_VecGridColor[2]));
    this->yAxis->grid()->setSubGridPen(QPen(m_VecGridColor[3],1,Qt::DotLine));
    //刻度
    m_TimeTicker = QSharedPointer<QCPAxisTickerTime>(new QCPAxisTickerTime);
    m_TimeTicker->setTickCount(m_XAxisCount);
    m_TimeTicker->setTimeFormat("%m:%s");
    this->xAxis->setTicker(m_TimeTicker);
    this->xAxis->setRange(0,m_nXAxisLength);
    this->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    this->yAxis->ticker()->setTickCount(m_YAxisCount);
    this->yAxis->setRange(m_dMin,m_dMax);
    this->yAxis->setNumberFormat("f");
    this->yAxis->setNumberPrecision(m_nDecAccurecy);

    m_Timer = new QTimer();
    m_Timer->setInterval(m_AutoTime);
    connect(m_Timer,&QTimer::timeout,this,&TCurve::DrawCurve);
}

CoItem *TCurve::Copy()
{
    TCurve *_curve = new TCurve();
    _curve->Copy(this);

    return _curve;
}

void TCurve::Copy(TCurve *curve)
{
    if(NULL == curve)
        return;

    TCustomPlot::Copy(curve);
}

void TCurve::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::Title:
        SetTitle(value);
        break;
    case CoAttributeWidget::AxisLblTxt:
        SetAxisLblTxt(value,uc);
        break;
    case CoAttributeWidget::GridColor:
        SetGridColor(QColor(value),uc);
        break;
    case CoAttributeWidget::XAxisCount:
        SetXAxisCount(value.toShort());
        break;
    case CoAttributeWidget::YAxisCount:
        SetYAxisCount(value.toShort());
        break;
    case CoAttributeWidget::BGColor:
    {
        SetBackColor(QColor(value),uc);
        SetCurveBGColor(QColor(value));
        break;
    }
    case CoAttributeWidget::GridVisiableState:
        SetGridVisible(value.toShort());
        break;
    case CoAttributeWidget::AxisLblColor:
        SetAxisLblColor(QColor(value),uc);
        break;
    case CoAttributeWidget::TimeFormat:
        SetTimeFormat(value);
        break;
    case CoAttributeWidget::Unit:
        SetTimeUnit(value);
        break;
    case CoAttributeWidget::XAxisLength:
        SetXAxisLength(value.toInt());
        break;
    case CoAttributeWidget::DecAccurecy:
        SetDecAccurecy(value.toInt());
        break;
    case CoAttributeWidget::MaxValue:
        SetMax(value.toDouble());
        break;
    case CoAttributeWidget::MinValue:
        SetMin(value.toDouble());
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    case CoAttributeWidget::CtrlVarName:
        SetCtrlVarName(value);
        break;
    case CoAttributeWidget::VarName:
        SetVarName(value);
        break;
    case CoAttributeWidget::CurveColor:
        SetCurveColor(QColor(value));
        break;
    default:
        break;
    }
}

void TCurve::SetTitle(const QString &str)
{
    m_strTitle = str;
    m_pTitleEle->setText(m_strTitle);
}

QString TCurve::GetTitle() const
{
    return m_strTitle;
}

void TCurve::SetAxisLblTxt(const QString &str,uchar uc)
{
    m_strAxisLblTxtList[uc] = str;
    if(uc > 0)
        this->yAxis->setLabel(m_strAxisLblTxtList[1]);
    else
        this->xAxis->setLabel(m_strAxisLblTxtList[0]);
}

QStringList TCurve::GetAxisLblTxt() const
{
    return m_strAxisLblTxtList;
}

void TCurve::SetGridColor(QColor color, uchar uc)
{
    m_VecGridColor[uc] = color;
    SetGridColorByParam(uc);
}

QVector<QColor> TCurve::GetGridColor() const
{
    return m_VecGridColor;
}

void TCurve::SetXAxisCount(const uchar uc)
{
    m_XAxisCount = uc;
    m_TimeTicker->setTickCount(m_XAxisCount);
}

uchar TCurve::GetXAxisCount() const
{
    return m_XAxisCount;
}

void TCurve::SetYAxisCount(const uchar uc)
{
    m_YAxisCount = uc;
    this->yAxis->ticker()->setTickCount(m_YAxisCount);
}

uchar TCurve::GetYAxisCount() const
{
    return m_YAxisCount;
}

void TCurve::SetGridVisible(const uchar uc)
{
    m_IsGridVisible = uc;
    SetGridVisibleByParam(uc);
}

uchar TCurve::GetGridVisible() const
{
    return m_IsGridVisible;
}

void TCurve::SetAxisLblColor(QColor color, uchar uc)
{
    m_VecAxisLblColor[uc] = color;
    if(uc > 0)
        this->yAxis->setLabelColor(color);
    else
        this->xAxis->setLabelColor(color);
}

QVector<QColor> TCurve::GetAxisLblColor() const
{
    return m_VecAxisLblColor;
}

void TCurve::SetTimeFormat(const QString &str)
{
    m_strTimeFormat = str;
    m_TimeTicker->setTimeFormat(m_strTimeFormat);
}

QString TCurve::GetTimeFormat() const
{
    return m_strTimeFormat;
}

void TCurve::SetTimeUnit(const QString &str)
{
    m_strTimeUnit = str;
}

QString TCurve::GetTimeUnit() const
{
    return m_strTimeUnit;
}

void TCurve::SetXAxisLength(const int n)
{
    m_nXAxisLength = n;
    this->xAxis->setRangeUpper(n);
}

int TCurve::GetXAxisLength() const
{
    return m_nXAxisLength;
}

void TCurve::SetDecAccurecy(const int n)
{
    m_nDecAccurecy = n;
    this->yAxis->setNumberPrecision(m_nDecAccurecy);
}

int TCurve::GetDecAccurecy() const
{
    return m_nDecAccurecy;
}

void TCurve::SetMax(const double d)
{
    m_dMax = d;
    this->yAxis->setRangeUpper(m_dMax);
}

double TCurve::GetMax() const
{
    return m_dMax;
}

void TCurve::SetMin(const double d)
{
    m_dMin = d;
    this->yAxis->setRangeLower(m_dMin);
}

double TCurve::GetMin() const
{
    return m_dMin;
}

void TCurve::SetCtrlVarName(const QString &str)
{
    QStringList list = str.split(',');
    if(list.size() > 1)
    {
        m_strCtrlVarName = list[0];
        m_CtrlVarType = list[1].toShort();
    }
}

QString TCurve::GetCtrlVarName() const
{
    return m_strCtrlVarName;
}

char TCurve::GetCtrlVarType() const
{
    return m_CtrlVarType;
}

void TCurve::SetVarName(const QString &str)
{
    QStringList list = str.split(',');
    if(list.size() > 1)
    {
        m_strVarName = list[0];
        m_VarType = list[1].toShort();
    }
}

QString TCurve::GetVarName() const
{
    return m_strVarName;
}

char TCurve::GetVarType() const
{
    return m_VarType;
}

void TCurve::SetCurveColor(QColor color)
{
    m_CurveColor = color;
    this->graph(0)->setPen(QPen(m_CurveColor));
}

QColor TCurve::GetCurveColor() const
{
    return m_CurveColor;
}

void TCurve::SetCurveBGColor(QColor color)
{
    Q_UNUSED(color)
    this->setBackground(QBrush(m_BackColorVec[0]));
}

void TCurve::SetGridColorByParam(uchar uc)
{
    switch (uc)
    {
    case 0:
        this->xAxis->grid()->setPen(QPen(m_VecGridColor[0]));
        break;
    case 1:
        this->xAxis->grid()->setSubGridPen(QPen(m_VecGridColor[1],1,Qt::DotLine));
        break;
    case 2:
        this->yAxis->grid()->setPen(QPen(m_VecGridColor[2]));
        break;
    case 3:
        this->yAxis->grid()->setSubGridPen(QPen(m_VecGridColor[3],1,Qt::DotLine));
        break;
    default:
        break;
    }
}

void TCurve::SetGridVisibleByParam(uchar uc)
{
    bool b = bool(uc);
    this->xAxis->grid()->setVisible(b);
    this->yAxis->grid()->setVisible(b);
    this->xAxis->grid()->setSubGridVisible(b);
    this->yAxis->grid()->setSubGridVisible(b);
}

void TCurve::DrawCurve()
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(!m_strCtrlVarName.isEmpty())
        {
            int nCtrlValue = DBInstanceManager::getDBInstance()->DBVarStru.
                    SwitchMap[m_strCtrlVarName].initvalue;
            if(nCtrlValue == 1)
            {
                if(!m_bRefresh)
                {
                    m_bRefresh = true;
                    //clear graph
                    QVector<double> keys;
                    QVector<double> values;
                    this->graph(0)->setData(keys, values);
                    this->xAxis->setRange(0, m_XAxisCount, Qt::AlignRight);
                    this->replot();
                }
                if(!m_strVarName.isEmpty())
                {
                    double value = DBInstanceManager::getDBInstance()->DBVarStru.
                            regnumMap[m_strVarName].Initvalue;

                    static QTime time(QTime::currentTime());
                    //key : second interval
                    double key = (double)(time.elapsed() + 0)/1000;
                    this->graph(0)->addData(key,value);
                    this->xAxis->setRange(key, m_XAxisCount, Qt::AlignRight);
                    this->replot();
                }
            }
            else
            {
                m_bRefresh = false;
            }
        }
    }
}
