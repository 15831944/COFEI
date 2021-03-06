#include "Viewer.h"

#include <QtGui>
#include <cmath>

CoView::CoView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    setDragMode(QGraphicsView::RubberBandDrag);//拖动模式，鼠标可拖动
    setCacheMode(QGraphicsView::CacheBackground);//缓存模式
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);//指定Render标签，抗锯齿
    setStyleSheet("background: transparent;");
    m_Zoom = 0;
    m_Rotate = 0;
}

void CoView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        m_Zoom -= 5;
    else
        m_Zoom += 5;
    Zoom(m_Zoom);

}

void CoView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit MousePress(event->pos().x(), event->pos().y());

    QGraphicsView::mousePressEvent(event);
}

void CoView::keyReleaseEvent(QKeyEvent *event)
{
    QTransform transform = this->transform();


    if(event->modifiers() == Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
        {
            m_Rotate = 0;
            transform.reset();
        }
            break;
        case Qt::Key_Down:
        {
            transform.reset();
            m_Rotate = 180;
            transform.rotate(m_Rotate);
        }
            break;
        case Qt::Key_Left:
        {
            transform.reset();
            m_Rotate = -90;
            transform.rotate(m_Rotate);
        }
            break;
        case Qt::Key_Right:
        {
            transform.reset();
            m_Rotate = 90;
            transform.rotate(m_Rotate);
        }
            break;
        default:
            break;
        }
    }
    this->setTransform(transform);
    QGraphicsView::keyReleaseEvent(event);
}

void CoView::Zoom(double _Multiple)
{
    if(_Multiple < -100)
        _Multiple = -100;
    if(_Multiple > 150)
         _Multiple = 150;
    m_Zoom = _Multiple;
    qreal scale = qPow(qreal(2), (m_Zoom) / qreal(50));

    QMatrix matrix;
    matrix.reset();
    matrix.scale(scale, scale);
    matrix.rotate(m_Rotate);

    this->setMatrix(matrix);
}

void CoView::SetZoom(double _Zoom)
{
       Zoom(_Zoom);
}

double CoView::GetZoom() const
{
    return m_Zoom;
}

void CoView::SetRotate(int rotate)
{
    m_Rotate = rotate;
}

int CoView::GetRotate() const
{
    return m_Rotate;
}
