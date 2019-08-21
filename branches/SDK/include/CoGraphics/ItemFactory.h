#ifndef TITEM_FACTORY_H
#define TITEM_FACTORY_H

#include "Item/Item.h"
#include "Graphics.h"
#include "Scene/Scene.h"
#include "cographics_global.h"
class COGRAPHICSSHARED_EXPORT ItemFactory :public QObject
{
public:
    virtual CoItem* CreateItemFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black) ;
    virtual CoProxyWidget* CreateWidgetFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black,CoScene *Scene = NULL) ;
};

class COGRAPHICSSHARED_EXPORT TItemFactory
{
public:
    static  CoItem* CreateItem(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    static  CoProxyWidget* CreateProxyWidget(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black, CoScene *Scene = NULL);
static void SetItemFactory(ItemFactory * );
private:
    TItemFactory();
    ~TItemFactory();
    static ItemFactory * m_pItFactory;
};
#endif // TITEM_FACTORY_H
