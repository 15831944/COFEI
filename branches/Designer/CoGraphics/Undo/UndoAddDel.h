#ifndef UNDO_ADD_DEL_H
#define UNDO_ADD_DEL_H

#include "Undo.h"

class CoUndoAddDel : public CoUndo
{
    Q_OBJECT
public:
    CoUndoAddDel();
    CoUndoAddDel(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent = NULL);

    enum
    {
        OPERATION_ADD,
        OPERATION_DEL
    };

    virtual void undo();
    virtual void redo();

    void SetOperation(int Operation);
    int GetOperation();

private:
    int m_Operation;
    QList<CoItem *> m_ItemList;
    void DoAdd();
    void DoDel();
signals:
    void SendAddDelEvent(int,QList<CoItem *>);
};

#endif // UNDO_ADD_DEL_H
