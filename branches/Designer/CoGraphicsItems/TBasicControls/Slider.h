#ifndef TSLIDER_H
#define TSLIDER_H

#include <CoGraphics/Graphics.h>
#include <QMouseEvent>
/////////////////////////////////////////////////  单滑块  ////////////////////////////////////////////////////
class TSlider : public CoWidgets
{
    Q_OBJECT
public:
    TSlider(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TSlider();
    enum State{ Hovered,None};
public slots:
    CoItem *Copy();
    void Copy(TSlider *SliderFrom);
    void SetLabel(const QString& label);
    void SetValue(float val);
    void SetRange(float min, float max);
    void SetSingleStep(float step);
    float MinRange() const;
    float MaxRange() const;
    float Value()    const;
  signals:
      void valueChanged(float);

  private:
      float m_Min;
      float m_Max;
      float m_SingleStep;

      float m_Value;
      QRectF m_HandleRegion;

      QString m_Label;
      State m_State;

protected:
    void Draw(QPainter* painter);
    void PaintColoredRect(QRectF rect, QColor color, QPainter* painter);
    void PaintValueLabel(QPainter* painter);
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    float GetValidValue(float,float,float);
};

#endif // TSLIDER_H
