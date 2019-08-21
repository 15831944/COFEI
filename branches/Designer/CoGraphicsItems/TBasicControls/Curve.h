#ifndef CURVE_H
#define CURVE_H

#include <CoGraphics/Graphics.h>
#include <QApplication>
#include "../TCharts/Chart.h"
class TCurve : public TCustomPlot
{
    Q_OBJECT
public:
    explicit TCurve(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                    const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                    const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    ~TCurve(){}

public:
    Q_SLOT CoItem *Copy();
    Q_SLOT void Copy(TCurve *curve);
    Q_SLOT void SetAttributeEvent(const int,const QString,uchar uc = 0);

    void        SetTitle(const QString&);
    QString     GetTitle() const;

    void        SetAxisLblTxt(const QString &str,uchar uc);
    QStringList GetAxisLblTxt() const;

    // 0 XAxisGrid; 1 XAxisSubGrid; 2 YAxisGrid; 3 YAxisSubGrid
    void        SetGridColor(QColor color,uchar uc);
    QVector<QColor> GetGridColor() const;

    void        SetXAxisCount(const uchar);
    uchar       GetXAxisCount() const;

    void        SetYAxisCount(const uchar);
    uchar       GetYAxisCount() const;

    void        SetGridVisible(const uchar);
    uchar       GetGridVisible() const;

    void        SetAxisLblColor(QColor color,uchar uc);
    QVector<QColor> GetAxisLblColor() const;

    void        SetTimeFormat(const QString&);
    QString     GetTimeFormat() const;

    void        SetTimeUnit(const QString&);
    QString     GetTimeUnit() const;

    void        SetXAxisLength(const int);
    int         GetXAxisLength() const;

    void        SetDecAccurecy(const int);
    int         GetDecAccurecy() const;

    void        SetMax(const double);
    double      GetMax() const;

    void        SetMin(const double);
    double      GetMin() const;

    void        SetCtrlVarName(const QString&);
    QString     GetCtrlVarName() const;
    char        GetCtrlVarType() const;

    void        SetVarName(const QString&);
    QString     GetVarName() const;
    char        GetVarType() const;

    void        SetCurveColor(QColor);
    QColor      GetCurveColor() const;
    void        SetCurveBGColor(QColor color);

protected:
    QString     m_strTitle;
    // 0 XAxisGrid; 1 XAxisSubGrid; 2 YAxisGrid; 3 YAxisSubGrid
    QVector<QColor> m_VecGridColor;
    // 0 XAxis;1 YAxis
    QStringList m_strAxisLblTxtList;
    uchar       m_XAxisCount;
    uchar       m_YAxisCount;
    uchar       m_IsGridVisible;
    // 0 XAxis; 1 YAxis
    QVector<QColor> m_VecAxisLblColor;
    QString     m_strTimeFormat;
    QString     m_strTimeUnit;
    int         m_nXAxisLength;
    int         m_nDecAccurecy;
    double      m_dMax;
    double      m_dMin;

    QString     m_strCtrlVarName;
    char        m_CtrlVarType;

    QString     m_strVarName;
    char        m_VarType;
    QColor      m_CurveColor;
    bool        m_bRefresh;

    QCPTextElement *m_pTitleEle;
    QSharedPointer<QCPAxisTickerTime> m_TimeTicker;

private:
    void SetGridColorByParam(uchar);
    void SetGridVisibleByParam(uchar);
    Q_SLOT void DrawCurve();
};
#endif // CURVE_H

