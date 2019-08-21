#include "Graphicsfactory.h"
#include <CoGraphicsItems/TButton/Button.h>
#include <CoGraphicsItems/TPixmap/Pixmap.h>
#include <CoGraphicsItems/TLine/Line.h>
GraphicsFactory::GraphicsFactory()
{

}

CoItem *GraphicsFactory::CreateFactory(int Type, double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle, const int LineWidth, const QColor &LineColor, const QColor &BackColor)
{
    CoItem *Item = NULL;
    switch (Type)
    {
    case CoItem::Pixmap:
    {
        Item = new TPixmap(StartX, StartY, StopX, StopY,
                            LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case CoItem::Button:
    {
        Item = new TButton(StartX, StartY, StopX, StopY,
                           LineStyle, LineWidth, LineColor, BackColor);
    }
    break;
    case CoItem::Line:
    {
        Item = new TLine(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
    break;
    default:
        break;
    }
    return Item;
}
