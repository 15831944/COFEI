#ifndef COARROW_H
#define COARROW_H
#include "Item/ItemEx.h"
#include "Graphics.h"
#include <QObject>
class QMenu;
class QGraphicsLineItem;

class CoArrow :public QObject,public QGraphicsLineItem
{
    Q_OBJECT
public:
    CoArrow(CoFlowChart * startItem,CoFlowChart * endItem);
    virtual ~CoArrow();
    void UpdatePos();
    QRectF boundingRect() const;
    QPainterPath shape() const ;

public:
    CoFlowChart* GetStartItem();
    CoFlowChart *GetEndItem();
    int GetStartPost();
    int GetEndPost();
    bool GetYNPolicy();
    void SetYNPolicy(bool);

signals:
private slots:
    void SetYes();
    void SetNo();
private:
    QPolygonF m_arrowHead;
    QPolygonF m_arrowLine;
    CoFlowChart * m_StartItem;
    CoFlowChart * m_EndItem;
    int m_StartPost;
    int m_EndPost;
    QColor m_Color;
    QPointF m_p1;
    QPointF m_p2;
    QPointF m_p3;
    QPointF m_p4;
    QPointF m_p5;
    QPointF m_p6;
    bool m_isYes;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);
    void CalcVertexes(double startX, double startY, double stopX, double stopY,
                      double& destX1, double& destY1, double& destX2, double& destY2);
    void StartEndPos();
    void SetLeftEndItemPos();
    void SetRightEndItemPos();
    void SetTopEndItemPos();
    void SetBottomEndItemPos();
    bool IntersectPos(QLineF  ,QPointF &);
    void SetPoints(const double&,const double&,const double = -1,const char = 0);
    QMenu* CreateMenus();
//protected:
//    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
};

#endif // COARROW_H
