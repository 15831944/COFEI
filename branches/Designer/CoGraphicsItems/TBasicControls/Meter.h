#ifndef TMETER_H
#define TMETER_H

#include <CoGraphics/Graphics.h>

class TMeter : public CoGraphics
{
    Q_OBJECT
public:
    TMeter(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

    virtual ~TMeter();
    enum
    {
        MODE_CLOSCK_WISE,       // 顺时针
        MODE_COUNTER_WISE       // 逆时针
    };

public slots:
    CoItem *Copy();
    void Copy(TMeter *MeterFrom);

    void SetMode(int Mode);
    int GetMode();
    void SetScaleSum(int Sum);
    int GetScaleSum();
    void SetStartAngle(double Angle);
    double GetStartAngle();
    void SetStepAngle(double Angle);
    double GetStepAngle();
    void SetStartValue(double Value);
    double GetStartValue();
    void SetStepValue(double Value);
    double GetStepValue();
    void SetPointerColor(const QColor &Color);
    QColor GetPointerColor();

private:
    int m_Mode;              // 顺时针还是逆时针
    int m_ScaleSum;          // 刻度总数
    double m_StartAngle;     // 起始角度
    double m_StepAngle;      // 阶梯角度
    double m_StartValue;     // 起始值
    double m_StepValue;      // 阶梯值
    QColor m_PointerColor;   // 指针颜色
    void Draw(QPainter *painter);
};

#endif // TMETER_H
