#ifndef TARROW_H
#define TARROW_H
#include <CoGraphics/Graphics.h>

class TArrow : public CoGraphics
{
    Q_OBJECT
public:
    TArrow(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);
    virtual ~TArrow();

    QRectF boundingRect() const;
    QPainterPath shape() const ;

public slots:
    CoItem *Copy();
    void Copy(TArrow *ArrowFrom);

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
    QPolygonF m_arrowHead;

    void UpdatePos();

    void UpdateMouse(QPointF &PosNow);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void Draw(QPainter *painter);
    void CalcVertexes(double startX, double startY, double stopX, double stopY,
                      double& destX1, double& destY1, double& destX2, double& destY2);
};

#endif // TARROW_H
