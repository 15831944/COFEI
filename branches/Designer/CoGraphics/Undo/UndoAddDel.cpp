#include "UndoAddDel.h"
#include "../Graphics.h"
#include <QDebug>

CoUndoAddDel::CoUndoAddDel() : CoUndo()
{
    m_Operation = OPERATION_ADD;
}

CoUndoAddDel::CoUndoAddDel(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent)
    : CoUndo(Scene, ItemList,Parent)
{
}

void CoUndoAddDel::undo()
{
    if(OPERATION_ADD == m_Operation)
    {
        DoDel();
    }
    else
    {
        DoAdd();
    }
}

void CoUndoAddDel::redo()
{
    if(OPERATION_ADD == m_Operation)
    {
        DoAdd();
    }
    else
    {
        DoDel();
    }
}

void CoUndoAddDel::DoAdd()
{
    if(m_Scene)
    {
        foreach(CoItem *Item, m_List)
        {
            Item->setVisible(true);
            m_Scene->addItem(Item);
        }
        foreach(CoProxyWidget *Item, m_ProxyList)
        {
            Item->setVisible(true);
            m_Scene->addItem(Item);
        }
    }
}

void CoUndoAddDel::DoDel()
{
    if(m_Scene)
    {
        foreach(CoProxyWidget *Item, m_ProxyList)
        {
            Item->setVisible(false);
            m_Scene->removeItem(Item);
        }
        foreach(CoItem *Item, m_List)
        {
            Item->setVisible(false);
            if(Item->GetType() >= CoItem::FlowStart)
            {
                dynamic_cast<CoFlowChart*>(Item)->RemoveArrows();
            }
            m_Scene->removeItem(Item);
        }
    }
}

void CoUndoAddDel::SetOperation(int Operation)
{
    m_Operation = Operation;
}

int CoUndoAddDel::GetOperation()
{
    return m_Operation;
}
