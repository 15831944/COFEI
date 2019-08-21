#ifndef GRAPHICSFACTORY_H
#define GRAPHICSFACTORY_H

#include <QObject>
#include <QWidget>
#include <CoGraphics/ItemFactory.h>
class GraphicsFactory : public ItemFactory
{
    Q_OBJECT
public:
    GraphicsFactory();
public:
    CoItem* CreateFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                               const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black) ;
signals:

public slots:
};

#endif // GRAPHICSFACTORY_H
