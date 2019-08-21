#ifndef COSCENE_H
#define COSCENE_H

#include "../Graphics.h"
#include "../Arrow.h"
#include <QGraphicsScene>
class QGraphicsSceneMouseEvent;

class CoScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CoScene(QObject *parent = 0);
    CoScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
    enum ArrowMode
    {
        InsertItem  = 2048,
        InsertArrow
    };
    template<typename ItemType>
    void GetItemsList(QList<ItemType *> &List)
    {
        List.clear();

        foreach(QGraphicsItem *mGraphicsItem, items())
        {
            ItemType *FindItem = dynamic_cast<ItemType *>(mGraphicsItem);

            if(FindItem)
            {
                List.push_back(FindItem);
            }
        }
    }
    template<typename ItemType> void SetItemsSelected(ItemType*)
    {
        foreach(QGraphicsItem *mGraphicsItem, items())
        {
            ItemType *FindItem = dynamic_cast<ItemType *>(mGraphicsItem);

            if(FindItem)
            {
                FindItem->setSelected(true);
            }
        }
    }
    template<typename ItemType> void GetItemsSelected(QList<ItemType *> &List)
    {
        List.clear();

        foreach(QGraphicsItem *mGraphicsItem, selectedItems())
        {
            ItemType *Item = dynamic_cast<ItemType *>(mGraphicsItem);

            if(Item)
            {
                List.push_back(Item);
            }
        }
    }
    void DeleteItemsSelected(QList<CoItem *> &List);

    static  CoItem* GetItemByNameId(QList<CoScene *> &SceneList, const QString &NameId);

public slots:
    QObject* GetItemByNameId(const QString &NameId);

    void    SetId(int Id);
    int     GetId();
    void    SetName(QString Name);
    QString GetName();
    void    SetProjectName(QString Name);
    QString GetProjectName();
    void    ResizeEvent(bool isGrid);
    void    SetBackColor(const QColor &Color);
    void    SetBackColor(int R, int G, int B);
    QColor  GetBackColor();

    double  GetPosX();
    double  GetPosY();
    double  GetWidth();
    double  GetHeight();

    void    SetUserData(void *UserData);
    void*   GetUserData();
    void    SetMode(ArrowMode);
    int     GetMode();
    CoArrow * CreateArrow(CoFlowChart* ,CoFlowChart*);

protected:
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);// 鼠标按下
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);// 鼠标松开
    //void    keyReleaseEvent(QKeyEvent *event);

signals:
    void    mDropFinsh(double StartX, double StartY, double StopX, double StopY);
    void    mClickItem(const QString&,const QString&,const int,const int,CoItemEx*);
    void    mClickWidget(const QString&,const QString&,const int,const int,CoProxyWidget*);
    void    mMoveItemList(CoScene *Scene, QList<CoItem *> ItemList,QList<CoProxyWidget *> WidgetList,
                       double StartX, double StartY, double StopX, double StopY);

private:
    void *  m_UserData;
    double  m_StartDropX;
    double  m_StartDropY;

    int     m_Id;
    QString m_Name;
    QString m_proName;
    QColor  m_BackColor;

    bool    m_SelectedBeforeDrop;
    bool    m_isfirstSelected;
    ArrowMode m_Mode;
    QGraphicsLineItem *m_line1;
    QGraphicsLineItem *m_line2;
    QGraphicsLineItem *m_line3;
    QGraphicsLineItem *m_line4;
    QGraphicsPolygonItem *m_Polygon;
    QLineF m_Leftline;
    QLineF m_Rightline;
    QLineF m_Topline;
    QLineF m_Bottomline;
    QPointF m_FirstPos;
    int m_IntersectedPos;
    int m_EndPost;
    void GetArrowPost(CoItemEx*,QPointF);
    bool IsEqualList(const QList<CoArrow*>,const QList<CoArrow*>);
    int IntersectPos(QLineF , QLineF , QLineF ,QLineF ,QLineF );
    QPointF NearestPos(int &,QPointF , QPointF , QPointF , QPointF , QPointF);
};

#endif // COSCENE_H
