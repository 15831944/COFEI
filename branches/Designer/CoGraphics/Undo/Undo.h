#ifndef UNDO_H
#define UNDO_H

#include "../Scene/Scene.h"
#include "../Graphics.h"
#include "../Arrow.h"
#include <QUndoCommand>
class CoUndo : public QUndoCommand , public QObject
{
public:
    CoUndo();
    CoUndo(CoScene * Scene, QList<CoItem *> &ItemList, QUndoCommand * Parent = NULL);

    virtual void undo();
    virtual void redo();

    void SetScene(CoScene *Scene);
    void AddItem(CoItem *Item);
    void AddProxyWidgets(QList<CoProxyWidget *> Items);
    void AddArrows(QList<CoArrow *> Items);

protected:
    CoScene *m_Scene;
    QList<CoItem *>m_List;
    QList<CoProxyWidget *>m_ProxyList;
    QList<CoArrow *> m_ArrowList;
};

#endif // UNDO_H
