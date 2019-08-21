#ifndef UNDO_MOVE_DRAG_H
#define UNDO_MOVE_DRAG_H

#include "Undo.h"

class CoUndoMoveDrag : public CoUndo
{
    Q_OBJECT
public:
    CoUndoMoveDrag();
    CoUndoMoveDrag(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent = NULL);

    virtual void undo();
    virtual void redo();

    void SetVector(double StartDx, double StartDy, double StopDx, double StopDy);

private:
    double m_StartDx;
    double m_StartDy;
    double m_StopDx;
    double m_StopDy;
signals:
    void SendMoveDragEvent(QList<CoItem *>);
};

#endif // UNDO_MOVE_DRAG_H
