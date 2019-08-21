#include "UndoMoveDrag.h"
#include <QDebug>

CoUndoMoveDrag::CoUndoMoveDrag() : CoUndo()
{
    m_StartDx = 0;
    m_StartDy = 0;
    m_StopDx = 0;
    m_StopDy = 0;
}

CoUndoMoveDrag::CoUndoMoveDrag(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent)
    : CoUndo(Scene, ItemList,Parent)
{
    m_StartDx = 0;
    m_StartDy = 0;
    m_StopDx = 0;
    m_StopDy = 0;
}

void CoUndoMoveDrag::undo()
{
    foreach(CoItem *Item, m_List)
    {
        Item->MoveTo(Item->GetStartX() - m_StartDx,
                     Item->GetStartY() - m_StartDy,
                     Item->GetStopX() - m_StopDx,
                     Item->GetStopY() - m_StopDy);
    }
    foreach(CoProxyWidget *Item, m_ProxyList)
    {
        Item->MoveTo(Item->GetStartX() - m_StartDx,
                     Item->GetStartY() - m_StartDy,
                     Item->GetStopX() - m_StopDx,
                     Item->GetStopY() - m_StopDy);
    }
}

void CoUndoMoveDrag::redo()
{
    foreach(CoItem *Item, m_List)
    {
        Item->MoveTo(Item->GetStartX() + m_StartDx,
                     Item->GetStartY() + m_StartDy,
                     Item->GetStopX() + m_StopDx,
                     Item->GetStopY() + m_StopDy);
    }
    foreach(CoProxyWidget *Item, m_ProxyList)
    {
        Item->MoveTo(Item->GetStartX() + m_StartDx,
                     Item->GetStartY() + m_StartDy,
                     Item->GetStopX() + m_StopDx,
                     Item->GetStopY() + m_StopDy);
    }
}

void CoUndoMoveDrag::SetVector(double StartDx, double StartDy, double StopDx, double StopDy)
{
    m_StartDx = StartDx;
    m_StartDy = StartDy;
    m_StopDx = StopDx;
    m_StopDy = StopDy;
}

