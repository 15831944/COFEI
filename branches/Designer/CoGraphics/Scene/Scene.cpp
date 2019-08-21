#include "Scene.h"

#include <QPointF>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QtMath>
#include <QDebug>
#include "Graphics.h"
CoScene::CoScene(QObject *parent) : QGraphicsScene(parent)
{
    m_Id = 0;
    m_Name = tr("");
    m_StartDropX = 0;
    m_StartDropY = 0;
    m_UserData = 0;
    m_line1 = 0;
    m_line2 = 0;
    m_line3 = 0;
    m_line4 = 0;
    m_Leftline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Rightline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Topline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Bottomline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_FirstPos = QPointF(0, 0);
    m_SelectedBeforeDrop = false;
    m_isfirstSelected = false;
    m_Mode = InsertItem;
    m_IntersectedPos = CoItem::Arrow_Free;
    m_EndPost = CoItem::Arrow_Free;

}

CoScene::CoScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x, y, width, height, parent)
{
    m_Id = 0;
    m_Name = tr("");
    m_StartDropX = 0;
    m_StartDropY = 0;
    m_line1 = 0;
    m_line2 = 0;
    m_line3 = 0;
    m_line4 = 0;
    m_Leftline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Rightline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Topline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_Bottomline = QLineF(QPointF(0, 0),QPointF(0, 0));
    m_FirstPos = QPointF(0, 0);
    m_SelectedBeforeDrop = false;
    m_isfirstSelected = false;
    m_IntersectedPos = CoItem::Arrow_Free;
    m_EndPost = CoItem::Arrow_Free;
    m_Mode = InsertItem;
    // 绘制多边形
    QPolygonF polygon;
    polygon << QPointF(x, y) << QPointF(x, height)
            << QPointF(width, height) << QPointF(width, y);
    // 设置画笔、画刷
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::white);
    m_Polygon = addPolygon(polygon,pen,QBrush(m_BackColor));
    m_Polygon->setZValue(-9999);
}

void CoScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_StartDropX = event->scenePos().x();
    m_StartDropY = event->scenePos().y();
    QTransform deviceTransform;

    QGraphicsItem *UnderItem = itemAt(m_StartDropX, m_StartDropY, deviceTransform);
    if(UnderItem )
    {

        CoItemEx *GraphicsItem = dynamic_cast<CoItemEx *>(UnderItem);
        if(GraphicsItem && GraphicsItem->GetSelectAble())
        {
//            GraphicsItem->m_isfirstSelected = false;
//            QList<CoItemEx *> List;
//            GetItemsSelected(List);
//            if(List.size() == 0)
//            {
//                GraphicsItem->m_isfirstSelected = true;
//            }

            m_isfirstSelected = true;

            for(int i = 0; i < items().size();i++) {
                CoItemEx *item = dynamic_cast<CoItemEx *>(items().at(i));
                if(item){
                    item->m_isfirstSelected = false;
                }
            }
            GraphicsItem->m_isfirstSelected = true;

            if(InsertArrow == m_Mode )
            {
                if(dynamic_cast<CoFlowChart *>(UnderItem))
                {
                    m_line1 = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
                    m_FirstPos = event->scenePos();
                    m_line1->setPen(QPen(Qt::gray, 2));
                    addItem(m_line1);
                    m_line2 = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
                    m_line3 = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
                    m_line4 = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
                    m_line2->setPen(QPen(Qt::gray, 2));
                    m_line3->setPen(QPen(Qt::gray, 2));
                    m_line4->setPen(QPen(Qt::red, 2));
                    addItem(m_line2);
                    addItem(m_line3);
                    addItem(m_line4);
                    m_Leftline.setP1(QPointF(GraphicsItem->GetCurrentStartX(),GraphicsItem->GetCurrentStartY()));
                    m_Leftline.setP2(QPointF(GraphicsItem->GetCurrentStartX(),GraphicsItem->GetCurrentStopY()));

                    m_Rightline.setP1(QPointF(GraphicsItem->GetCurrentStopX(),GraphicsItem->GetCurrentStartY()));
                    m_Rightline.setP2(QPointF(GraphicsItem->GetCurrentStopX(),GraphicsItem->GetCurrentStopY()));

                    m_Topline.setP1(QPointF(GraphicsItem->GetCurrentStartX(),GraphicsItem->GetCurrentStartY()));
                    m_Topline.setP2(QPointF(GraphicsItem->GetCurrentStopX(),GraphicsItem->GetCurrentStartY()));

                    m_Bottomline.setP1(QPointF(GraphicsItem->GetCurrentStartX(),GraphicsItem->GetCurrentStopY()));
                    m_Bottomline.setP2(QPointF(GraphicsItem->GetCurrentStopX(),GraphicsItem->GetCurrentStopY()));
                }
            }
            emit mClickItem(GetProjectName(),GetName(),GraphicsItem->GetType(),GraphicsItem->GetId(),GraphicsItem);
        }
        else
        {
            CoProxyWidget *GraphicsWidget = dynamic_cast<CoProxyWidget *>(UnderItem);
            if(GraphicsWidget && GraphicsWidget->GetSelectAble())
            {
                m_isfirstSelected = true;
                emit mClickWidget(GetProjectName(),GetName(),GraphicsWidget->GetType(),GraphicsWidget->GetId(),GraphicsWidget);
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
    update();
}

void CoScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_Mode == InsertArrow && m_line1 != 0)
    {
        QLineF oldLine(m_FirstPos, event->scenePos());
        int arrowDirec = IntersectPos(oldLine,m_Leftline,m_Rightline,m_Topline,m_Bottomline);
        if( m_IntersectedPos ==  CoItem::Arrow_Free)
        {
            m_IntersectedPos = arrowDirec;
        }
        if(arrowDirec == CoItem::Arrow_Free)
        {
            m_line1->setLine(oldLine);
            m_line2->setLine(oldLine);
            m_line3->setLine(oldLine);
            m_line4->setLine(oldLine);
            m_IntersectedPos = CoItem::Arrow_Free;
            m_EndPost = CoItem::Arrow_Free;
        }
        else if(m_IntersectedPos == CoItem::Arrow_Left)
        {
            double centerX = (m_Leftline.p1().x() + m_Leftline.p2().x()) / 2;
            double centerY = (m_Leftline.p1().y() + m_Leftline.p2().y()) / 2;
            QPointF pos = QPointF(centerX,centerY);

            if(centerY != event->scenePos().y())
            {
                QLineF newLine1(pos, QPointF((pos.x()+event->scenePos().x())/2,pos.y()));
                QLineF newLine2(QPointF((pos.x()+event->scenePos().x())/2,pos.y()),
                                QPointF((pos.x()+event->scenePos().x())/2,event->scenePos().y()));
                QLineF newLine3(event->scenePos(), QPointF((pos.x()+event->scenePos().x())/2,event->scenePos().y()));
                m_line1->setLine(newLine1);
                m_line2->setLine(newLine2);
                m_line3->setLine(newLine3);
            }
            else
            {
                QLineF newLine(pos, event->scenePos());
                m_line1->setLine(newLine);
            }

        }
        else if(m_IntersectedPos == CoItem::Arrow_Right)
        {
            double centerX = (m_Rightline.p1().x() + m_Rightline.p2().x()) / 2;
            double centerY = (m_Rightline.p1().y() + m_Rightline.p2().y()) / 2;
            QPointF pos = QPointF(centerX,centerY);

            if(centerY != event->scenePos().y())
            {
                QLineF newLine1(pos, QPointF((pos.x()+event->scenePos().x())/2,pos.y()));
                QLineF newLine2(QPointF((pos.x()+event->scenePos().x())/2,pos.y()),
                                QPointF((pos.x()+event->scenePos().x())/2,event->scenePos().y()));
                QLineF newLine3(event->scenePos(), QPointF((pos.x()+event->scenePos().x())/2,event->scenePos().y()));
                m_line1->setLine(newLine1);
                m_line2->setLine(newLine2);
                m_line3->setLine(newLine3);
            }
            else
            {
                QLineF newLine(pos, event->scenePos());
                m_line1->setLine(newLine);
            }
        }
        else if(m_IntersectedPos == CoItem::Arrow_Top)
        {
            double centerX = (m_Topline.p1().x() + m_Topline.p2().x()) / 2;
            double centerY = (m_Topline.p1().y() + m_Topline.p2().y()) / 2;
            QPointF pos = QPointF(centerX,centerY);

            if(centerX != event->scenePos().x())
            {
                QLineF newLine1(pos, QPointF(pos.x(),(pos.y() + event->scenePos().y())/2));
                QLineF newLine2(QPointF(pos.x(),(pos.y() + event->scenePos().y())/2),
                                QPointF(event->scenePos().x(),(pos.y() + event->scenePos().y())/2));
                QLineF newLine3(event->scenePos(), QPointF(event->scenePos().x(),(pos.y() + event->scenePos().y())/2));
                m_line1->setLine(newLine1);
                m_line2->setLine(newLine2);
                m_line3->setLine(newLine3);
            }
            else
            {
                QLineF newLine(pos, event->scenePos());
                m_line1->setLine(newLine);
            }
        }
        else if(m_IntersectedPos == CoItem::Arrow_Bottom)
        {
            double centerX = (m_Bottomline.p1().x() + m_Bottomline.p2().x()) / 2;
            double centerY = (m_Bottomline.p1().y() + m_Bottomline.p2().y()) / 2;
            QPointF pos = QPointF(centerX,centerY);

            if(centerX != event->scenePos().x())
            {
                QLineF newLine1(pos, QPointF(pos.x(),(pos.y() + event->scenePos().y())/2));
                QLineF newLine2(QPointF(pos.x(),(pos.y() + event->scenePos().y())/2),
                                QPointF(event->scenePos().x(),(pos.y() + event->scenePos().y())/2));
                QLineF newLine3(event->scenePos(), QPointF(event->scenePos().x(),(pos.y() + event->scenePos().y())/2));
                m_line1->setLine(newLine1);
                m_line2->setLine(newLine2);
                m_line3->setLine(newLine3);
            }
            else
            {
                QLineF newLine(pos, event->scenePos());
                m_line1->setLine(newLine);
            }
        }
        QLineF newLine4(event->scenePos(), event->scenePos());
        if( m_IntersectedPos != CoItem::Arrow_Free)
        {
            QList<QGraphicsItem *> endItems = items(event->scenePos());
            if (endItems.count() > 0)
            {
                if(endItems.contains(m_line1))
                    endItems.removeOne(m_line1);
                if(endItems.contains(m_line2))
                    endItems.removeOne(m_line2);
                if(endItems.contains(m_line3))
                    endItems.removeOne(m_line3);
                if(endItems.contains(m_line4))
                    endItems.removeOne(m_line4);
            }
            if(endItems.count() > 0)
            {
                CoFlowChart *endItem = dynamic_cast<CoFlowChart *>(endItems.first());
                if(endItem)
                {
                    QPointF pos = NearestPos( m_EndPost ,event->scenePos(),
                                             QPointF(endItem->GetCurrentStartX(),endItem->GetCurrentCenterY()),
                                             QPointF(endItem->GetCurrentStopX(),endItem->GetCurrentCenterY()),
                                             QPointF(endItem->GetCurrentCenterX(),endItem->GetCurrentStartY()),
                                             QPointF(endItem->GetCurrentCenterX(),endItem->GetCurrentStopY()));
                    newLine4.setP2(pos);
                }
            }
        }
        m_line4->setLine(newLine4);
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void CoScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isfirstSelected)
    {
        m_SelectedBeforeDrop = true;
        if(abs(m_StartDropX - event->scenePos().x()) <= 1 ||
        abs(m_StartDropY - event->scenePos().y()) <= 1)
            m_isfirstSelected = false;
    }
    emit mDropFinsh(m_StartDropX, m_StartDropY, event->scenePos().x(), event->scenePos().y());

    if (m_line1 != 0 && m_line2 != 0 && m_line3 != 0 && m_line4 != 0 &&m_Mode == InsertArrow)
    {
        removeItem(m_line2);
        delete m_line2;
        removeItem(m_line3);
        delete m_line3;
        m_line2 = 0;
        m_line3 = 0;
        QList<QGraphicsItem *> startItems = items(m_line1->line().p1());
        if (startItems.count() > 0)
        {
            if(startItems.contains(m_line1))
                startItems.removeOne(m_line1);
//            if(startItems.contains(m_line2))
//                startItems.removeOne(m_line2);
//            if(startItems.contains(m_line3))
//                startItems.removeOne(m_line3);
            if(startItems.contains(m_line4))
                startItems.removeOne(m_line4);
        }
        QList<QGraphicsItem *> endItems = items(m_line4->line().p1());
        if (endItems.count() > 0)
        {
            if(endItems.contains(m_line1))
                endItems.removeOne(m_line1);
//            if(endItems.contains(m_line2))
//                endItems.removeOne(m_line2);
//            if(endItems.contains(m_line3))
//                endItems.removeOne(m_line3);
            if(endItems.contains(m_line4))
                endItems.removeOne(m_line4);
        }

        removeItem(m_line1);
        delete m_line1;
        removeItem(m_line4);
        delete m_line4;
        m_line1 = 0;
        m_line4 = 0;
        if (startItems.count() > 0 && endItems.count() > 0 &&startItems.first() != endItems.first())
        {
            CoFlowChart *startItem = dynamic_cast<CoFlowChart *>(startItems.first());
            CoFlowChart *endItem = dynamic_cast<CoFlowChart *>(endItems.first());
            if(endItem && startItem)
            {
                if(!IsEqualList(startItem->GetOutputArrows(),endItem->GetInputArrows()))
                {
                    startItem->m_ArrowPost = m_IntersectedPos;
                    endItem->m_ArrowPost = m_EndPost;
                    CreateArrow(startItem, endItem);
                }
            }

        }
    }

    if(m_SelectedBeforeDrop && m_isfirstSelected)
    {
        QList<CoItem *> ItemList;
        GetItemsSelected(ItemList);
        QList<CoProxyWidget *> WidgetList;
        GetItemsSelected(WidgetList);
        if(ItemList.length() > 0 || WidgetList.length() > 0)
        {
            bool SendEmit = true;
            foreach(CoItem *Item, ItemList)
            {
                if(Item->GetMouseState() > 0)
                {
                    SendEmit = false;
                    break;
                }
            }
            foreach(CoProxyWidget *Widget, WidgetList)
            {
                if(!Widget->GetMoveAble())
                {
                    SendEmit = false;
                    break;
                }
            }
            if(SendEmit)
            {
                emit mMoveItemList(this, ItemList,WidgetList,m_StartDropX, m_StartDropY,
                                   event->scenePos().x(), event->scenePos().y());
            }
        }
    }

    m_SelectedBeforeDrop = false;
    m_isfirstSelected = false;
    QGraphicsScene::mouseReleaseEvent(event);
}

//void CoScene::keyReleaseEvent(QKeyEvent *event)
//{
//    if(selectedItems().length() <= 0)
//    {
//        return;
//    }

//    CoItem *Item = dynamic_cast<CoItem *>(selectedItems().first());

//    if(Item)
//    {
//        Item->keyReleaseEvent(event);
//    }
//}
int CoScene::IntersectPos(QLineF sourceline, QLineF leftline, QLineF rightline,QLineF topline,QLineF bottomline)
{
    QPointF interPos(0,0);
    QLineF::IntersectType type = sourceline.intersect(leftline, &interPos);
    if (type == QLineF::BoundedIntersection)
        return CoItem::Arrow_Left;

    type = sourceline.intersect(rightline, &interPos);
    if (type == QLineF::BoundedIntersection)
        return CoItem::Arrow_Right;

    type = sourceline.intersect(topline, &interPos);
    if (type == QLineF::BoundedIntersection)
        return CoItem::Arrow_Top;

    type = sourceline.intersect(bottomline, &interPos);
    if (type == QLineF::BoundedIntersection)
        return CoItem::Arrow_Bottom;

    return CoItem::Arrow_Free;
}

QPointF CoScene::NearestPos(int &iResult, QPointF point,QPointF leftPoint, QPointF rightPoint, QPointF topPoint, QPointF bottomPoint )
{
    double left = sqrt((leftPoint.x()-point.x()) *(leftPoint.x()-point.x()) +
                       (leftPoint.y()-point.y()) *(leftPoint.y()-point.y()) );
    double right = sqrt((rightPoint.x()-point.x()) *(rightPoint.x()-point.x()) +
                        (rightPoint.y()-point.y()) *(rightPoint.y()-point.y()) );
    double top = sqrt((topPoint.x()-point.x()) *(topPoint.x()-point.x()) +
                       (topPoint.y()-point.y()) *(topPoint.y()-point.y()) );
    double bottom = sqrt((bottomPoint.x()-point.x()) *(bottomPoint.x()-point.x()) +
                       (bottomPoint.y()-point.y()) *(bottomPoint.y()-point.y()) );

    double iL = left;
    iResult = CoItem::Arrow_Left;
    QPointF pos = leftPoint;
    if(left - right > 0.0001)
    {
        iL = right;
        iResult = CoItem::Arrow_Right;
        pos = rightPoint;
    }
    if(iL - top > 0.0001)
    {
        iL = top;
        iResult = CoItem::Arrow_Top;
        pos = topPoint;
    }
    if(iL - bottom > 0.0001)
    {
        iL = bottom;
        iResult = CoItem::Arrow_Bottom;
        pos = bottomPoint;
    }
    return pos;
}
void CoScene::GetArrowPost(CoItemEx *Item, QPointF point)
{
    double StartDropX = point.x();
    double StartDropY = point.y();
    double startX = Item->GetCurrentStartX();
    double startY = Item->GetCurrentStartY();
    double width = Item->GetCurrentStopX() - startX;
    double heigth = Item->GetCurrentStopY() - startY;
    if(startX + width/3 > StartDropX && startY + heigth/3 < StartDropY && startY + 2*heigth/3 > StartDropY)
    {
        Item->m_ArrowPost = CoItem::Arrow_Left;
    }
    else if(startX + 2*width/3 < StartDropX && startY + heigth/3 < StartDropY && startY +2 *heigth/3 > StartDropY)
    {
        Item->m_ArrowPost = CoItem::Arrow_Right;
    }
    else if(startY + heigth/3 > StartDropY && startX + width/3 < StartDropX && startX + 2*width/3 > StartDropX)
    {
        Item->m_ArrowPost = CoItem::Arrow_Top;
    }
    else if(startY + 2*heigth/3 < StartDropY && startX + width/3 < StartDropX && startX + 2*width/3 > StartDropX)
    {
        Item->m_ArrowPost = CoItem::Arrow_Bottom;
    }
    else
    {
        Item->m_ArrowPost = CoItem::Arrow_Free;
    }
}

bool CoScene::IsEqualList(const QList<CoArrow *> firstList, const QList<CoArrow *> secondList)
{
    QHash<CoArrow *,int> hash = QHash<CoArrow *,int>();
    foreach (CoArrow *arrow, firstList)
    {
        hash.insert(arrow,1);
    }
    foreach (CoArrow *arrow, secondList)
    {
        int i = hash[arrow];
        if(i == 1)
            hash[arrow]++;
        else
            hash[arrow] = 1;
    }
    for(auto it = hash.begin();it != hash.end();++it)
    {
        if(it.value() > 1)
            return true;
    }
    return false;
}
void CoScene::SetId(int Id)
{
    m_Id = Id;
}

int CoScene::GetId()
{
    return m_Id;
}

void CoScene::SetName(QString Name)
{
    m_Name = Name;
}

QString CoScene::GetName()
{
    return m_Name;
}

void CoScene::SetProjectName(QString Name)
{
    m_proName = Name;
}

QString CoScene::GetProjectName()
{
    return m_proName;
}

void CoScene::ResizeEvent(bool isGrid)
{
    QPolygonF polygon(sceneRect());
    m_Polygon->setPolygon(polygon);
    if(isGrid)
    {
        QPolygonF myPolygon1;
        myPolygon1 << QPointF(0,10) << QPointF(20,10);
        QPolygonF myPolygon2;
        myPolygon2 << QPointF(10,0) << QPointF(10,20);
        QPixmap pixmap(20, 20);
        pixmap.fill(m_BackColor);
        QPainter painter(&pixmap);

        QVector<qreal> dashes;//line style--
        qreal space = 2;
        dashes << 2 << space << 2 <<space;
        QPen pen(Qt::lightGray,1);
        pen.setDashPattern(dashes);
        pen.setWidth(1);

        painter.setPen(pen);
        painter.translate(0, 0);
        painter.drawPolyline(myPolygon1);
        painter.drawPolyline(myPolygon2);
        m_Polygon->setBrush(pixmap);
    }
    else
        m_Polygon->setBrush(m_BackColor);
}

void CoScene::SetBackColor(const QColor &Color)
{
    m_BackColor = Color;
    setBackgroundBrush(QColor(63, 147, 168));
}

void CoScene::SetBackColor(int R, int G, int B)
{
    SetBackColor(QColor(R, G, B));
}

QColor CoScene::GetBackColor()
{
    return m_BackColor;
}

double CoScene::GetPosX()
{
    return sceneRect().x();
}

double CoScene::GetPosY()
{
    return sceneRect().y();
}

double CoScene::GetWidth()
{
    return sceneRect().width();
}

double CoScene::GetHeight()
{
    return sceneRect().height();
}

void CoScene::SetUserData(void *UserData)
{
    m_UserData = UserData;
}

void* CoScene::GetUserData()
{
    return m_UserData;
}

void CoScene::SetMode(ArrowMode arrow)
{
    m_Mode = arrow;
}

int CoScene::GetMode()
{
    return m_Mode;
}

CoArrow *CoScene::CreateArrow(CoFlowChart *startItem, CoFlowChart *endItem)
{
    QList<CoArrow *> startArrows = startItem->GetOutputArrows();
    QList<CoArrow *> endArrows = endItem->GetInputArrows();
    foreach (CoArrow* aw, startArrows) {
        if(endArrows.contains(aw))
            return NULL;
    }
    CoArrow *arrow = new CoArrow(startItem, endItem);
    startItem->AddOutputArrow(arrow);
    endItem->AddInputArrow(arrow);
    arrow->setZValue(-1000.0);
    arrow->UpdatePos();
    addItem(arrow);
    return arrow;
}

CoItem* CoScene::GetItemByNameId(QList<CoScene *> &SceneList, const QString &NameId)
{
    if(NameId.length() <= 0)
    {
        return NULL;
    }

    foreach(CoScene *Scene, SceneList)
    {
        QList<CoItemEx *>ItemExList;
        Scene->GetItemsList(ItemExList);

        foreach(CoItemEx *ItemEx, ItemExList)
        {
            if(NameId == ItemEx->GetNameId())
            {
                return ItemEx;
            }
        }
    }

    return NULL;
}

QObject* CoScene::GetItemByNameId(const QString &NameId)
{
    QList<CoItemEx *>ItemExList;
    GetItemsList(ItemExList);

    foreach(CoItemEx *ItemEx, ItemExList)
    {
        if(NameId == ItemEx->GetNameId())
        {
            return ItemEx;
        }
    }

    return NULL;
}

void CoScene::DeleteItemsSelected(QList<CoItem *> &List)
{
    List.clear();
    foreach(QGraphicsItem *mGraphicsItem, selectedItems())
    {
        CoItem *Item = dynamic_cast<CoItem *>(mGraphicsItem);
        if(Item)
        {
            List.push_back(Item);
        }
        else
        {
            CoArrow * Arrow = dynamic_cast<CoArrow *>(mGraphicsItem);
            if(Arrow)
            {
                Arrow->GetStartItem()->RemoveOutputArrow(Arrow);
                Arrow->GetEndItem()->RemoveInputArrow(Arrow);
                Arrow->deleteLater();
            }
        }
    }
}
