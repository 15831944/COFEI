#include "Item.h"
#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QMetaEnum>
#include <LibBase.h>
CoItem::CoItem(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) : QGraphicsItem()
{
    m_Type = CoItem::Base;

    m_StartX = MIN(StartX, StopX);
    m_StartY = MIN(StartY, StopY);
    m_StopX = MAX(StartX, StopX);
    m_StopY = MAX(StartY, StopY);

    m_LineWidth = LineWidth;
    m_LineColor = LineColor;
    m_LineStyle = LineStyle;
    m_BackGroundColor = BackColor;
    m_CurFontColor = LineColor;

    m_DropScale = false;
    m_DropDirection = Direc_Free;
    m_ArrowPost     = Arrow_Free;
    SetSelectAble(true);
    SetMoveAble(true);
    SetDragAble(true);

    setAcceptHoverEvents(true);

    m_RotateAngle = 0;
    m_RotateCenterX = 0;
    m_RotateCenterY = 0;
    m_Power = 2;
    m_isfirstSelected = false;
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void CoItem::TransFormPos()
{
    double StartX   = m_StartX;
    double StartY   = m_StartY;
    double StopX    = m_StopX;
    double StopY    = m_StopY;

    m_StartX = MIN(StartX, StopX);
    m_StartY = MIN(StartY, StopY);
    m_StopX = MAX(StartX, StopX);
    m_StopY = MAX(StartY, StopY);
}

CoItem::~CoItem()
{}

void CoItem::MoveEnable(bool Enable)
{
    setFlag(ItemIsMovable, Enable);
}

bool CoItem::SetMoveAble(bool Able)
{
    m_MoveAble = Able;
    MoveEnable(m_MoveAble);

    return true;
}

bool CoItem::SetSelectAble(bool Able)
{
    m_SelectAble = Able;
    setFlag(ItemIsSelectable, Able);

    return true;
}

bool CoItem::GetSelectAble()
{
    return m_SelectAble;
}

bool CoItem::SetDragAble(bool Able)
{
    m_DragAble = Able;
    //setAcceptHoverEvents(Able);

    return true;
}

bool CoItem::GetDragAble() const
{
    return m_DragAble;
}

int CoItem::GetMouseState() const
{
    return m_DropDirection;
}

bool CoItem::GetMoveAble() const
{
    return m_MoveAble;
}

QRectF CoItem::boundingRect() const
{
    QRectF mRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);

    const int padding = (m_LineWidth + 1) / 2;
    mRect.adjust(-padding, -padding, +padding, +padding);

    return mRect;
}

void CoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    BeforePaint(painter, option, widget);

    Draw(painter);

    AfterDraw(painter);
}

void CoItem::Draw(QPainter* painter)
{
    Q_UNUSED(painter);
}

QPainter* CoItem::BeforePaint(QPainter* &painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPen pen;

    if(GetSelectAble() && (option->state & QStyle::State_Selected))
    {
        if(m_isfirstSelected)
            pen.setColor(Qt::red);
        else
            pen.setColor(QColor(150,0,0));
        pen.setStyle(Qt::DotLine);
        pen.setWidth(3);
        painter->setPen(pen);

        QColor RectColor = m_BackGroundColor;
        RectColor.setAlpha(0);
        painter->setBrush(RectColor);

        painter->drawRect(boundingRect());
    }

    pen.setColor(m_LineColor);
    pen.setStyle(m_LineStyle);

    pen.setWidth(m_LineWidth);

    painter->setPen(pen);
    painter->setBrush(m_BackGroundColor);

    painter->setRenderHint(QPainter::Antialiasing, true);

    return painter;
}

void CoItem::AfterDraw(QPainter *&painter)
{
    Q_UNUSED(painter);
}

bool CoItem::SetType(const int Type)
{
    if(Type > Base && Type < Max)
    {
        m_Type = Type;
        return true;
    }
    else
    {
        return false;
    }
}

int CoItem::GetType() const
{
    return m_Type;
}
void CoItem::SetId(const int id)
{
    m_Id = id;
}

int CoItem::GetId() const
{
    return m_Id;
}

void CoItem::SetPower(const int power)
{
    m_Power = power;
}

int CoItem::GetPower() const
{
    return m_Power;
}

void CoItem::SetProName(const QString proName)
{
    m_ProName = proName;
}

QString CoItem::GetProName() const
{
    return m_ProName;
}
QString CoItem::GetEnumToString(int ienum)
{
    const QMetaObject metaObject = CoItem::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("CoType");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    return  QString(en.valueToKey(ienum));
}
int CoItem::GetStringToEnum(QString str)
{
    const QMetaObject metaObject = CoItem::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("CoType");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    QByteArray ba = str.toLatin1();
    return en.keysToValue(ba.data());
}

void CoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(GetDragAble() && Qt::LeftButton == event->button() && m_DropDirection > Direc_Free)
    {
        m_PressStart = event->scenePos();
        m_DropPos = event->scenePos();
        m_DropScale = true;
    }
    else
    {
        MoveEnable(true); // 临时设置为可移动，否则不能触发mouseReleaseEvent
    }

    Update();

    QGraphicsItem::mousePressEvent(event);
}

void CoItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_DropScale)
    {
        QPointF dis = event->scenePos() - m_DropPos;
        m_DropPos = event->scenePos();

        switch(m_DropDirection)
        {
        case Direc_Top: // 上 dis.y < 0
        {
            m_StartY += dis.y() / 2;
            m_StopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(0, dis.y() / 2);
            break;
        }
        case Direc_RightTop: // 右上
        {
            m_StartX -= dis.x() / 2;
            m_StartY += dis.y() / 2;
            m_StopX += dis.x() / 2;
            m_StopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Right: // 右 dis.x > 0
        {
            m_StartX -= dis.x() / 2;
            m_StopX += dis.x() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, 0);
            break;
        }
        case Direc_RightBottom: // 右下
        {
            m_StartX -= dis.x() / 2;
            m_StartY -= dis.y() / 2;
            m_StopX += dis.x() / 2;
            m_StopY += dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Bottom: // 下 dis.y > 0
        {
            m_StartY -= dis.y() / 2;
            m_StopY += dis.y() / 2;

            update(boundingRect());
            moveBy(0, dis.y() / 2);
            break;
        }
        case Direc_LeftBottom: // 左下
        {
            m_StartX += dis.x() / 2;
            m_StartY -= dis.y() / 2;
            m_StopX -= dis.x() / 2;
            m_StopY += dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }
        case Direc_Left: // 左 dis.x < 0
        {
            m_StartX += dis.x() / 2;
            m_StopX -= dis.x() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, 0);
            break;
        }
        case Direc_LeftTop: // 左上
        {
            m_StartX += dis.x() / 2;
            m_StartY += dis.y() / 2;
            m_StopX -= dis.x() / 2;
            m_StopY -= dis.y() / 2;

            update(boundingRect());
            moveBy(dis.x() / 2, dis.y() / 2);
            break;
        }

        default:
            break;
        }
    }

//    Update();

    if(m_MoveAble) // 避免因为临时设置为可移动，造成不可移动的图形发生移动
    {
        QRectF rect = scene()->sceneRect();
        if(GetStartX() < rect.left() || GetStopX() > rect.right()||
                GetStartY() < rect.top() || GetStopY() > rect.bottom())
        {
            //item宽，高
            int width = ABS(GetStopX() - GetStartX());
            int height = ABS(GetStopY() - GetStartY());
            //当前item中心点
            int px = GetStartX() + (width/2);
            int py = GetStartY() + (height/2);
            //判断上下左右四个方向是否越界，由于不允许越界（超出scene范围），则需重置item位置
            //重新计算item中心点位置，增加3px的阈值。
            if(GetStartX() < rect.left())
            {
                px = rect.left()+(width/2) + 3;
            }
            if(GetStopX() > rect.right())
            {
                px = rect.right() - (width/2) - 3;
            }
            if(GetStartY() < rect.top())
            {
                py = rect.top() + (height/2) + 3;
            }
            if(GetStopY() > rect.bottom())
            {
                py = rect.bottom() - (height/2) - 3;
            }

            //重置item位置
            SetStartPos(px - (width/2), py - (height/2));
            SetStopPos(px + (width/2), py + (height/2));
        }
        QGraphicsItem::mouseMoveEvent(event);
    }
    Update();
}

void CoItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_DropScale)
    {
        double StartDx = 0;
        double StartDy = 0;
        double StopDx = 0;
        double StopDy = 0;

        double Dx = m_DropPos.x() - m_PressStart.x();
        double Dy = m_DropPos.y() - m_PressStart.y();

        switch(m_DropDirection)
        {
        case Direc_Top: // 上 dis.y < 0
            StartDy = Dy;
            break;

        case Direc_RightTop: // 右上
            StartDy = Dy;
            StopDx = Dx;
            break;

        case Direc_Right: // 右 dis.x > 0
            StopDx = Dx;
            break;

        case Direc_RightBottom: // 右下
            StopDx = Dx;
            StopDy = Dy;
            break;

        case Direc_Bottom: // 下 dis.y > 0
            StopDy = Dy;
            break;

        case Direc_LeftBottom: // 左下
            StartDx = Dx;
            StopDy = Dy;
            break;

        case Direc_Left: // 左 dis.x < 0
            StartDx = Dx;
            break;

        case Direc_LeftTop: // 左上
            StartDx = Dx;
            StartDy = Dy;
            break;

        default:
            break;
        }
        emit mDrop(this, StartDx, StartDy, StopDx, StopDy);
    }

//    QRectF rect = scene()->sceneRect();
//    double startX = GetStartX();
//    double stopX = GetStopX();
//    double startY = GetStartY();
//    double stopY = GetStopY();
//    bool isMove = true;
//    if(startX < rect.left())
//    {
//        startX += 5;
//        isMove = false;
//    }
//    else if(stopX > rect.right())
//    {
//        stopX -= 5;
//        isMove = false;
//    }
//    else if(startY < rect.top())
//    {
//        startY += 5;
//        isMove = false;
//    }
//    else if(stopY > rect.bottom())
//    {
//        stopY -= 5;
//        isMove = false;
//    }
//    if(!isMove)
//        MovePos(startX,startY,stopX,stopY);
//    MoveEnable(m_MoveAble); // 关闭临时可移动，恢复正常的移动标志
    Update();

    QGraphicsItem::mouseReleaseEvent(event);
}

void CoItem::UpdateMouse(QPointF &PosNow)
{
    if(!m_DragAble)
    {
        return;
    }
    QCursor mCursor;
    QRectF mRect = boundingRect();
    QPointF lt = this->scenePos() + QPointF(mRect.x(), mRect.y());
    QPointF lb = this->scenePos() + QPointF(mRect.x(), mRect.y() + mRect.height());
    QPointF rt = this->scenePos() + QPointF(mRect.x() + mRect.width(), mRect.y());
    QPointF rb = this->scenePos() + QPointF(mRect.x() + mRect.width(), mRect.y() + mRect.height());
    int Add = m_LineWidth + 2;
    if(PosNow.x() <= lt.x() + Add && PosNow.y() <= lt.y() + Add)      // 左上
    {
        m_DropDirection = Direc_LeftTop;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(PosNow.x() >= rb.x() - Add && PosNow.y() >= rb.y() - Add) // 右下
    {
        m_DropDirection = Direc_RightBottom;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(PosNow.x() <= lb.x() + Add && PosNow.y() >= lb.y() - Add) // 左下
    {
        m_DropDirection = Direc_LeftBottom;
        mCursor.setShape(Qt::SizeBDiagCursor);
    }
    else if(PosNow.x() >= rt.x() - Add && PosNow.y() <= rt.y() + Add) // 右上
    {
        m_DropDirection = Direc_RightTop;
        mCursor.setShape(Qt::SizeBDiagCursor);
    }
    else if(PosNow.x() < lt.x() + Add ) // 左
    {
        m_DropDirection = Direc_Left;
        mCursor.setShape(Qt::SizeHorCursor);
    }
    else if(PosNow.x() > rt.x() - Add ) // 右
    {
        m_DropDirection = Direc_Right;
        mCursor.setShape(Qt::SizeHorCursor);
    }
    else if(PosNow.y() < lt.y() + Add ) // 上
    {
        m_DropDirection = Direc_Top;
        mCursor.setShape(Qt::SizeVerCursor);
    }
    else if(PosNow.y() > lb.y() - Add ) // 下
    {
        m_DropDirection = Direc_Bottom;
        mCursor.setShape(Qt::SizeVerCursor);
    }
    else
    {
        m_DropDirection = Direc_Free;
        mCursor.setShape(Qt::ArrowCursor);
    }

    if(Direc_Free == m_DropDirection)
    {
        MoveEnable(m_MoveAble);
    }
    else
    {
        MoveEnable(false);
    }

    setCursor(mCursor);
}

void CoItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF PosNow = event->scenePos();
    UpdateMouse(PosNow);

    Update();
    QGraphicsItem::hoverEnterEvent(event);
}

void CoItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF PosNow = event->scenePos();
    UpdateMouse(PosNow);

    Update();
    QGraphicsItem::hoverMoveEvent(event);
}

void CoItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_DropScale = false;
    m_DropDirection = Direc_Free;

    // 当鼠标离开图形，恢复移动状态
    MoveEnable(m_MoveAble);

    Update();

    QCursor mCursor;
    mCursor.setShape(Qt::ArrowCursor);
    setCursor(mCursor);

    QGraphicsItem::hoverLeaveEvent(event);
}

void CoItem::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsItem::keyReleaseEvent(event);
}

void CoItem::Copy(CoItem *ItemFrom)
{
    if(NULL == ItemFrom)
    {
        return;
    }

    this->SetStartPos(ItemFrom->GetStartX(), ItemFrom->GetStartY());
    this->SetStopPos(ItemFrom->GetStopX(), ItemFrom->GetStopY());
    this->SetLineWidth(ItemFrom->GetLineWidth());
    this->SetLineColor(ItemFrom->GetLineColor());
    this->SetLineStyle(ItemFrom->GetLineStyle());
    this->SetBackGroundColor(ItemFrom->GetBackGroundColor());
    this->SetSelectAble(ItemFrom->GetSelectAble());
    this->SetMoveAble(ItemFrom->GetMoveAble());
    this->SetDragAble(ItemFrom->GetDragAble());
    this->SetZValue(ItemFrom->GetZValue());
}

double CoItem::GetStartX()
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    double Ret = mapToScene(m_StartX, m_StartY).x();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }

    return Ret;
}

double CoItem::GetStartY()
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    double Ret = mapToScene(m_StartX, m_StartY).y();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }

    return Ret;

    //return mapToScene(mStartX, mStartY).y();
}

double CoItem::GetStopX()
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    double Ret = mapToScene(m_StopX, m_StopY).x();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }

    return Ret;
    //return mapToScene(mStopX, mStopY).x();
}

double CoItem::GetStopY()
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    double Ret = mapToScene(m_StopX, m_StopY).y();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }

    return Ret;
    //return mapToScene(mStopX, mStopY).y();
}

double CoItem::GetCenterX()
{
    double StartX = GetStartX();
    double StopX = GetStopX();

    return StartX + (StopX - StartX) / 2;
}

double CoItem::GetCenterY()
{
    double StartY = GetStartY();
    double StopY = GetStopY();

    return StartY + (StopY - StartY) / 2;
}

double CoItem::GetCurrentStartX()
{
    return mapToScene(m_StartX, m_StartY).x();
}

double CoItem::GetCurrentStartY()
{
    return mapToScene(m_StartX, m_StartY).y();
}

double CoItem::GetCurrentStopX()
{
    return mapToScene(m_StopX, m_StopY).x();
}

double CoItem::GetCurrentStopY()
{
    return mapToScene(m_StopX, m_StopY).y();
}

double CoItem::GetCurrentCenterX()
{
    double StartX = GetCurrentStartX();
    double StopX = GetCurrentStopX();

    return StartX + (StopX - StartX) / 2;
}

double CoItem::GetCurrentCenterY()
{
    double StartY = GetCurrentStartY();
    double StopY = GetCurrentStopY();

    return StartY + (StopY - StartY) / 2;
}

void CoItem::SetStartPos(double StartX, double StartY)
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    QPointF Point = mapFromScene(StartX, StartY);
    m_StartX = Point.x();
    m_StartY = Point.y();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void CoItem::SetStopPos(double StopX, double StopY)
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    QPointF Point = mapFromScene(StopX, StopY);
    m_StopX = Point.x();
    m_StopY = Point.y();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void CoItem::SetStartPos(const QPointF &Start)
{
    SetStartPos(Start.x(), Start.y());
}

void CoItem::SetStopPos(const QPointF &Stop)
{
    SetStopPos(Stop.x(), Stop.y());
}

void CoItem::MovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    QPointF Point1 = mapFromScene(NewStartX, NewStartY);
    QPointF Point2 = mapFromScene(NewStopX, NewStopY);

    double dx1 = (Point1.x() - m_StartX) / 2;
    double dy1 = (Point1.y() - m_StartY) / 2;
    double dx2 = (Point2.x() - m_StopX) / 2;
    double dy2 = (Point2.y() - m_StopY) / 2;

    m_StartX += dx1;
    m_StartY += dy1;
    m_StopX -= dx1;
    m_StopY -= dy1;
    moveBy(dx1, dy1);

    m_StartX -= dx2;
    m_StartY -= dy2;
    m_StopX += dx2;
    m_StopY += dy2;
    moveBy(dx2, dy2);
}

void CoItem::MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    MovePos(NewStartX, NewStartY, NewStopX, NewStopY);
    TransFormPos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void CoItem::SetLineColor(const QColor &color)
{
    if(color.isValid())
    {
        m_LineColor = color;
    }
}

void CoItem::SetLineColor(int r, int g, int b, int a)
{
    m_LineColor = QColor(r, g, b, a);
}

QColor CoItem::GetLineColor() const
{
    return m_LineColor;
}

void CoItem::SetLineStyle(const Qt::PenStyle &style)
{
    m_LineStyle = style;
}

void CoItem::SetLineStyle(int style)
{
    m_LineStyle = Qt::PenStyle(style);
}

Qt::PenStyle CoItem::GetLineStyle() const
{
    return m_LineStyle;
}

void CoItem::SetLineWidth(int width)
{
    m_LineWidth = width;
}

int CoItem::GetLineWidth() const
{
    return m_LineWidth;
}

void CoItem::SetBackGroundColor(const QColor &color)
{
    if(color.isValid())
    {
        m_BackGroundColor = color;
    }
}

void CoItem::SetBackGroundColor(int r, int g, int b, int a)
{
    m_BackGroundColor = QColor(r, g, b, a);
}

QColor CoItem::GetBackGroundColor() const
{
    return m_BackGroundColor;
}

bool CoItem::SetRotateAngle(double Angle)
{
    return DoRotate(Angle - m_RotateAngle);
}

bool CoItem::DoRotate(double Angle)
{
    if(ABS(Angle) < 0.000001)
    {
        return false;
    }

    m_RotateAngle += Angle;

    QTransform transform(this->transform());
    transform.translate(m_RotateCenterX, m_RotateCenterY);
    transform.rotate(Angle);
    transform.translate(-m_RotateCenterX, -m_RotateCenterY);
    this->setTransform(transform);

    return true;
}

double CoItem::GetRotateAngle()
{
    return m_RotateAngle;
}

bool CoItem::SetRotateCenter(double X, double Y)
{
#if 1
    QPointF Point = mapFromScene(X, Y);

    m_RotateCenterX = Point.x();
    m_RotateCenterY = Point.y();
#else
    mRotateCenterX = X;
    mRotateCenterY = Y;
#endif
    return true;
}

double CoItem::GetRotateCenterX()
{
    //return mRotateCenterX;
    return mapToScene(m_RotateCenterX, m_RotateCenterY).x();
}

double CoItem::GetRotateCenterY()
{
    //return mRotateCenterY;
    return mapToScene(m_RotateCenterX, m_RotateCenterY).y();
}

void CoItem::SetZValue(double Z)
{
    setZValue(Z);
}

double CoItem::GetZValue()
{
    return zValue();
}

void CoItem::Update()
{
    update();
}

