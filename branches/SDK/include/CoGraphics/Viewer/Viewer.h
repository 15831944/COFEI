#ifndef COVIEWER_H
#define COVIEWER_H

#include <QGraphicsView>
class QWidget;

class CoView : public QGraphicsView
{
    Q_OBJECT

public:
    CoView(QWidget *parent = 0);
    void Zoom(double _Multiple);
    void SetZoom(double _Zoom);
    double GetZoom() const;
    void SetRotate(int rotate);
    int GetRotate() const;

signals:
    void MousePress(int x, int y);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    double m_Zoom;
    int m_Rotate;
};

#endif // COVIEWER_H
