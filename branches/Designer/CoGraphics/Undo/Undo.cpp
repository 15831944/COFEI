#include "Undo.h"

CoUndo::CoUndo()
{
    m_Scene = NULL;
}

CoUndo::CoUndo(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent)
    : QUndoCommand(Parent)
{
    m_Scene = Scene;
    m_List = ItemList;
}

void CoUndo::SetScene(CoScene *Scene)
{
    m_Scene = Scene;
}

void CoUndo::AddItem(CoItem *Item)
{
    m_List.push_back(Item);
}

void CoUndo::AddProxyWidgets(QList<CoProxyWidget *> Items)
{
    m_ProxyList = Items;
}

void CoUndo::AddArrows(QList<CoArrow *> Items)
{
    m_ArrowList = Items;
}

void CoUndo::undo()
{}

void CoUndo::redo()
{}
