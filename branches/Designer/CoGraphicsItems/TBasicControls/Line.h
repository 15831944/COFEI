#ifndef LINE_H
#define LINE_H
#include <CoGraphics/Graphics.h>

class TLine : public CoGraphics
{
    Q_OBJECT
public:
    TLine(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);
    virtual ~TLine();

    QRectF boundingRect() const;

public slots:
    CoItem *Copy();
    void Copy(TLine *LineFrom);

    virtual void SetStartPos(double StartX, double StartY);
    virtual void SetStopPos(double StopX, double StopY);
    virtual void SetStartPos(const QPointF &Start);
    virtual void SetStopPos(const QPointF &Stop);
    virtual void MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);

private:
    double m_PosX;
    double m_PosY;
    double m_Width;
    double m_Height;

    void UpdatePos();

    void UpdateMouse(QPointF &PosNow);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void Draw(QPainter *painter);
};

#endif // LINE_H
