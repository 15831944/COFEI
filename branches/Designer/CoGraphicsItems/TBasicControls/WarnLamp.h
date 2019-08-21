#ifndef WARNLAMP_H
#define WARNLAMP_H

#include <CoGraphics/Graphics.h>
class TWarnLamp : public CoGraphics
{
    Q_OBJECT
public:
    TWarnLamp(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TWarnLamp();
public slots:
    CoItem* Copy();
    void    Copy(TWarnLamp *IoFrom);
    void    Update();

    void    SetAttributeEvent(const int , const QString , uchar);

    void SetVarName(const QString, uchar uc);
    QVector<QString> GetVarName();

private:
    //m_VarNameVec[0] 红灯,m_VarNameVec[1] 黄灯,m_VarNameVec[2] 绿灯,m_VarNameVec[3] 蜂鸣器
    QVector<QString> m_VarNameVec;

    int     m_CurrentIndex;
    int     m_RowSum;
    int     m_RowLength;
    void    Draw(QPainter *painter);
    bool    SetSize();
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // WARNLAMP_H
