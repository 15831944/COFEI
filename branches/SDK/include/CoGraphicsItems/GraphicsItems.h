#ifndef GRAPHICSITEMS_H
#define GRAPHICSITEMS_H

#include "CoGraphicsItems_Global.h"
#include <CoGraphics/ItemFactory.h>
#include <CoGraphics/Scene/Scene.h>
class COGRAPHICSITEMSSHARED_EXPORT CoGraphicsItems : public ItemFactory
{
    Q_OBJECT
public:
    CoGraphicsItems();
public:
    CoItem* CreateItemFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                               const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black) ;
    CoProxyWidget* CreateWidgetFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                               const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black,CoScene *Scene = NULL) ;
signals:

public slots:
};

#endif // GRAPHICSITEMS_H
