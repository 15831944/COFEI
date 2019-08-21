#ifndef ATTRIBUTEFACTORY_H
#define ATTRIBUTEFACTORY_H

#include "AttributeWidget.h"
class AttributeFactory
{
public:
    AttributeFactory();
public:
    static CoAttributeWidget* CreateAttributeWidget(int Type,CoItemEx *Item) ;
    static CoAttributeWidget *CreateAttributeWidget(int Type,CoProxyWidget *Item);
};

#endif // ATTRIBUTEFACTORY_H
