#include "ItemFactory.h"

ItemFactory* TItemFactory::m_pItFactory = NULL;

TItemFactory::TItemFactory()
{}

TItemFactory::~TItemFactory()
{}

CoItem* TItemFactory::CreateItem(int Type, double StartX, double StartY, double StopX, double StopY,
                                   const Qt::PenStyle &LineStyle, const int LineWidth,
                                   const QColor &LineColor, const QColor &BackColor)
{
    CoItem *item = NULL;
    if(m_pItFactory != NULL)
    {
        item = m_pItFactory->CreateItemFactory(Type,StartX,StartY,StopX,StopY,LineStyle,LineWidth,LineColor,BackColor);
    }
    return item;
}
CoProxyWidget* TItemFactory::CreateProxyWidget(int Type, double StartX, double StartY, double StopX, double StopY,
                                   const Qt::PenStyle &LineStyle, const int LineWidth,
                                   const QColor &LineColor, const QColor &BackColor,CoScene *Scene)
{
    if(Scene == NULL)
        return NULL;
    CoProxyWidget *item = NULL;
    if(m_pItFactory != NULL)
    {
        item = m_pItFactory->CreateWidgetFactory(Type,StartX,StartY,StopX,StopY,LineStyle,LineWidth,LineColor,BackColor,Scene);
    }
    return item;
}
void TItemFactory::SetItemFactory(ItemFactory *p)
{
    if(m_pItFactory != NULL)
        m_pItFactory->deleteLater();
    m_pItFactory = p;
}
CoItem* ItemFactory::CreateItemFactory(int , double , double , double , double ,
                            const Qt::PenStyle &, const int ,
                            const QColor &, const QColor &)
{
    CoItem *item = NULL;

    return item;
}
CoProxyWidget* ItemFactory::CreateWidgetFactory(int , double , double , double , double ,
                            const Qt::PenStyle &, const int ,
                            const QColor &, const QColor &, CoScene *Scene)
{
    CoProxyWidget *item = NULL;
    if(Scene == NULL)
        return item;
    return item;
}
