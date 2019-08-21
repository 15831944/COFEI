#include "Graphics.h"
#include <QDebug>
#include <QThread>
#include <QMetaEnum>
#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <LibBase.h>
#include <CoGraphicsItems/AttributeWidget.h>
#include "Arrow.h"

CoGraphics::CoGraphics(double StartX, double StartY, double StopX, double StopY,
                        const Qt::PenStyle &LineStyle, const int LineWidth,
                        const QColor &LineColor, const QColor &BackColor) :
        CoItemEx(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_Value = 0;
    m_Coe = 1;
    m_ValueUpdateTime = 0;

    m_NodeId = 0;
    m_ServiceId = 0;

    m_Hint = tr("");
    m_LinkScene = 0;

    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(AutoRunRepeat()));

    m_AutoType = AUTO_NONE;
    m_LinkageRecv = 0;
}

void CoGraphics::Copy(CoGraphics *GraphicsFrom)
{
    if(NULL == GraphicsFrom)
    {
        return;
    }

    CoItemEx::Copy(GraphicsFrom);
    this->SetPower(GraphicsFrom->GetPower());

    this->SetLinkScene(GraphicsFrom->GetLinkScene());

    this->SetNodeId(GraphicsFrom->GetNodeId());
    this->SetValue(GraphicsFrom->GetValue());

}

CoGraphics::~CoGraphics()
{

}

void CoGraphics::SetLinkScene(int LinkId)
{
    m_LinkScene = LinkId;
}

int CoGraphics::GetLinkScene() const
{
    return m_LinkScene;
}

void CoGraphics::SetAutoType(int Type)
{
    m_AutoType = Type;
}

int CoGraphics::GetAutoType()
{
    return m_AutoType;
}

void CoGraphics::StartAlarm()
{
    qint64 AlarmTime = m_ValueUpdateTime;

    emit mStartAlarm(GetValue(), AlarmTime);
}

void CoGraphics::AutoRunRepeat()
{
//    qDebug() << "CoGraphics::AutoRunRepeat";
    return;
}

void CoGraphics::SetNodeId(const int Id)
{
    m_NodeId = Id;
}

int CoGraphics::GetNodeId() const
{
    return m_NodeId;
}


void CoGraphics::SetCoe(const double Coe)
{
    m_Coe = Coe;
}

double CoGraphics::GetCoe() const
{
    return m_Coe;
}

void CoGraphics::SetValue(const double Value)
{
    m_Value = Value * m_Coe;

    Update();
}

double CoGraphics::GetValue() const
{
    return m_Value;
}

int CoGraphics::GetLinkageRecv()
{
    return m_LinkageRecv;
}

void CoGraphics::SendLinkage(bool Enable, int _iLinkage)
{
    emit mSendLinkage(Enable, _iLinkage);
}

void CoGraphics::RecvLinkage(bool Enable, int _iLinkage)
{
    if(Enable)
    {
        m_LinkageRecv |= _iLinkage;
    }
    else
    {
        m_LinkageRecv &= (~_iLinkage);
    }

}

void CoGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_LinkScene > 0)
    {
        emit mGotoLink(m_LinkScene);
    }
    CoItem::mousePressEvent(event);
}

void CoGraphics::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CoItem::mouseReleaseEvent(event);
}

CoWidgets::CoWidgets(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    CoItemEx(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_Js = NULL;
    m_FontColor = m_CurFontColor;
    m_AlignVer = Qt::AlignVCenter ;
    m_AlignHor = Qt::AlignHCenter ;
    m_Text = tr("");
    m_CurPixFile = tr("");
    m_PixFileVec.reserve(2);
    m_PixFileVec << tr("") << tr("");
    m_SelectPixVec.reserve(2);
    m_SelectPixVec << false << false;
    m_BackColorVec.reserve(2);
    m_BackColorVec << m_BackGroundColor << m_BackGroundColor;
    m_AttriIndex = 0;
}

CoWidgets::~CoWidgets()
{}

void CoWidgets::SendTo(int , const QString , double , int , QString )
{
    return;
}

void CoWidgets::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    switch (type)
    {
    case CoAttributeWidget::Power:
        SetPower(value.toInt());
        break;
    case CoAttributeWidget::Text:
        SetText(value);
        break;
    case CoAttributeWidget::FontSize:
        SetFontSize(value.toUShort());
        break;
    case CoAttributeWidget::FontColor:
        SetFontColor(QColor(value));
        break;
    case CoAttributeWidget::AlignHorizontal:
        SetFontAlignHor(value.toUShort());
        break;
    case CoAttributeWidget::AlignVertical:
        SetFontAlignVer(value.toUShort());
        break;
    case CoAttributeWidget::PixMap:
        SetPixFile(value,uc);
        break;
    case CoAttributeWidget::BGColor:
        SetBackColor(QColor(value),uc);
        break;
    case CoAttributeWidget::SelectPix:
        SetSelectPix(!value.isEmpty(),uc);
        break;
    case CoAttributeWidget::Index:
        SetAttriIndex(value.toShort());
        break;
    default:
        break;
    }
    Update();
}

void CoWidgets::Copy(CoWidgets *WidgetsFrom)
{
    if(NULL == WidgetsFrom)
    {
        return;
    }

    CoItemEx::Copy(WidgetsFrom);
    this->SetPower(WidgetsFrom->GetPower());
    this->SetFont(WidgetsFrom->m_font);
    this->SetFontColor(WidgetsFrom->m_FontColor);
    this->SetFontAlignHor(WidgetsFrom->GetFontAlignHor());
    this->SetFontAlignVer(WidgetsFrom->GetFontAlignVer());
}
void CoWidgets::SetText(const QString Text)
{
    m_Text = Text;
}

QString CoWidgets::GetText()
{
    return m_Text;
}

void CoWidgets::SetFontSize(const uchar uc)
{
    m_font.setPointSize(uc);
}

uchar CoWidgets::GetFontSize() const
{
    return m_font.pointSize();
}

void CoWidgets::SetFontColor(const QColor color)
{
    m_FontColor = color;
    m_CurFontColor = m_FontColor;
}

QColor CoWidgets::GetFontColor() const
{
    return m_FontColor;
}

void CoWidgets::SetFontAlignHor(const uchar uc)
{
    switch (uc)
    {
    case 0:
        m_AlignHor = Qt::AlignLeft;
        break;
    case 1:
        m_AlignHor = Qt::AlignHCenter;
        break;
    case 2:
        m_AlignHor = Qt::AlignRight;
        break;
    default:
        m_AlignHor = Qt::AlignHCenter;
        break;
    }
}

uchar CoWidgets::GetFontAlignHor() const
{
    uchar uc = 0;
    if(m_AlignHor == Qt::AlignHCenter)
        uc = 1;
    else if(m_AlignHor == Qt::AlignRight)
        uc = 2;
    return uc;
}

void CoWidgets::SetFontAlignVer(const uchar uc)
{
    switch (uc)
    {
    case 0:
        m_AlignVer = Qt::AlignTop;
        break;
    case 1:
        m_AlignVer = Qt::AlignVCenter;
        break;
    case 2:
        m_AlignVer = Qt::AlignBottom;
        break;
    default:
        m_AlignVer = Qt::AlignVCenter;
        break;
    }
}

uchar CoWidgets::GetFontAlignVer() const
{
    uchar uc = 0;
    if(m_AlignVer == Qt::AlignVCenter)
        uc = 1;
    else if(m_AlignVer == Qt::AlignBottom)
        uc = 2;
    return uc;
}
void CoWidgets::SetPixFile(const QString pixFile, const uchar uc)
{
    if(m_PixFileVec.size() > uc)
    {
        m_PixFileVec[uc] = pixFile;
    }
    if(0 == uc)
        m_CurPixFile = pixFile;
}

QVector<QString> CoWidgets::GetPixFiles() const
{
    return m_PixFileVec;
}

void CoWidgets::SetSelectPix(bool isSelect, const uchar uc)
{
    if(m_SelectPixVec.size() > uc)
    {
        m_SelectPixVec[uc] = isSelect;
    }
    if(0 == uc)
    {
        m_CurPixFile = "";
        if(isSelect)
            m_CurPixFile = m_PixFileVec[0];
    }
}

QVector<bool> CoWidgets::GetSelectPix() const
{
    return m_SelectPixVec;
}

void CoWidgets::SetBackColor(const QColor color, const uchar uc)
{
    if(m_BackColorVec.size() > uc)
    {
        m_BackColorVec[uc] = color;
    }
    if(0 == uc)
        m_BackGroundColor = color;
}

QVector<QColor> CoWidgets::GetBackColor() const
{
    return m_BackColorVec;
}

void CoWidgets::SetAttriIndex(const uchar index)
{
    m_AttriIndex = index;
}

uchar CoWidgets::GetAttriIndex() const
{
    return m_AttriIndex;
}

CoFlowChart::CoFlowChart(double StartX, double StartY, double StopX, double StopY,
                         const Qt::PenStyle &LineStyle, const int LineWidth,
                         const QColor &LineColor, const QColor &BackColor) :
    CoItemEx(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_InputList.clear();
    m_OutputList.clear();
}

CoFlowChart::~CoFlowChart()
{

}

void CoFlowChart::Copy(CoFlowChart *FlowFrom)
{
    if(NULL == FlowFrom)
    {
        return;
    }

    CoItemEx::Copy(FlowFrom);
}
void CoFlowChart::AddInputArrow(CoArrow *arrow)
{
    m_InputList.append(arrow);
}

void CoFlowChart::AddOutputArrow(CoArrow *arrow)
{
    m_OutputList.append(arrow);
}

void CoFlowChart::RemoveInputArrow(CoArrow *arrow)
{
    int index = m_InputList.indexOf(arrow);

    if (index != -1)
        m_InputList.removeAt(index);
}
void CoFlowChart::RemoveOutputArrow(CoArrow *arrow)
{
    int index = m_OutputList.indexOf(arrow);

    if (index != -1)
        m_OutputList.removeAt(index);
}
void CoFlowChart::RemoveArrows()
{
    foreach (CoArrow *arrow, m_InputList) {
        arrow->GetStartItem()->RemoveOutputArrow(arrow);
        arrow->GetEndItem()->RemoveInputArrow(arrow);
        scene()->removeItem(arrow);
    }
    foreach (CoArrow *arrow, m_OutputList) {
        arrow->GetStartItem()->RemoveOutputArrow(arrow);
        arrow->GetEndItem()->RemoveInputArrow(arrow);
        scene()->removeItem(arrow);
    }
    m_InputList.clear();
    m_OutputList.clear();
}

QList<CoArrow *> CoFlowChart::GetInputArrows()
{
    return m_InputList;
}

QList<CoArrow *> CoFlowChart::GetOutputArrows()
{
    return m_OutputList;
}
QVariant CoFlowChart::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (CoArrow *arrow, m_InputList) {
            arrow->UpdatePos();
        }
        foreach (CoArrow *arrow, m_OutputList) {
            arrow->UpdatePos();
        }
//        QPointF newPos = value.toPointF();
//        QRectF curRect = this->boundingRect();
//        curRect.setRect(curRect.left()+newPos.x(),curRect.top()+newPos.y(),
//                        curRect.width(),curRect.height());
//        QRectF rect = scene()->sceneRect();
//        if (!rect.contains(curRect.topLeft()) || !rect.contains(curRect.bottomRight())) {
//            newPos.setX(0);
//            newPos.setY(0);
//            return newPos;
//        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void CoFlowChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CoItem::mousePressEvent(event);
}

void CoFlowChart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CoItem::mouseReleaseEvent(event);
}

void CoFlowChart::AfterDraw(QPainter *&painter)
{
    CoItem::AfterDraw(painter);
}

CoProxyWidget::CoProxyWidget(double StartX, double StartY, double StopX, double StopY,
                             const Qt::PenStyle &LineStyle, const int LineWidth,
                             const QColor &LineColor, const QColor &BackColor)
{
    m_Type = 0;

    m_StartX = MIN(StartX, StopX);
    m_StartY = MIN(StartY, StopY);
    m_StopX = MAX(StartX, StopX);
    m_StopY = MAX(StartY, StopY);
    setGeometry(m_StartX,m_StartY,m_StopX-m_StartX,m_StopY-m_StartY);
    m_StartX = 0;
    m_StartY = 0;
    m_LineWidth = LineWidth;
    m_LineColor = LineColor;
    m_LineStyle = LineStyle;
    m_BackGroundColor = BackColor;

    m_DropScale = false;
    m_DropDirection = Direc_Free;
    SetSelectAble(true);
    SetMoveAble(true);
    SetDragAble(true);

    setAcceptHoverEvents(true);

    m_RotateAngle = 0;
    m_RotateCenterX = 0;
    m_RotateCenterY = 0;
    m_Power = 2;
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//void CoProxyWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    MoveEnable(true); // 临时设置为可移动，否则不能触发mouseReleaseEvent
//    update();

//    QGraphicsWidget::mousePressEvent(event);

//}

//void CoProxyWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(m_MoveAble) // 避免因为临时设置为可移动，造成不可移动的图形发生移动
//    {
//        QGraphicsWidget::mouseMoveEvent(event);
//    }
//}

//void CoProxyWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    MoveEnable(m_MoveAble); // 关闭临时可移动，恢复正常的移动标志
//    update();

//    QGraphicsWidget::mouseReleaseEvent(event);
//}

//void CoProxyWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    QPointF PosNow = event->scenePos();
//    UpdateMouse(PosNow);

//    update();
//    QGraphicsWidget::hoverEnterEvent(event);
//}

//void CoProxyWidget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//    QPointF PosNow = event->scenePos();
//    UpdateMouse(PosNow);

//    update();
//    QGraphicsWidget::hoverMoveEvent(event);
//}

//void CoProxyWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    m_DropScale = false;
//    m_DropDirection = Direc_Free;

//    // 当鼠标离开图形，恢复移动状态
//    MoveEnable(m_MoveAble);

//    update();

//    QCursor mCursor;
//    mCursor.setShape(Qt::ArrowCursor);
//    setCursor(mCursor);

//    QGraphicsWidget::hoverLeaveEvent(event);
//}

QRectF CoProxyWidget::boundingRect() const
{
    return QGraphicsWidget::boundingRect();
}

void CoProxyWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QPen pen;

    if(GetSelectAble() && (option->state & QStyle::State_Selected))
    {
        pen.setColor(Qt::red);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(3);
        painter->setPen(pen);

        QColor RectColor = m_BackGroundColor;
        RectColor.setAlpha(0);
        painter->setBrush(RectColor);

        painter->drawRect(boundingRect());
    }
    painter->setBrush(m_BackGroundColor);
}

void CoProxyWidget::Copy(CoProxyWidget *ItemFrom)
{
    if(NULL == ItemFrom)
    {
        return;
    }
    this->SetStartPos(ItemFrom->GetStartX(), ItemFrom->GetStartY());
    this->SetStopPos(ItemFrom->GetStopX(), ItemFrom->GetStopY());
    this->SetBackGroundColor(ItemFrom->GetBackGroundColor());
    this->SetSelectAble(ItemFrom->GetSelectAble());
    this->SetMoveAble(ItemFrom->GetMoveAble());
    this->SetDragAble(ItemFrom->GetDragAble());
    this->SetZValue(ItemFrom->GetZValue());
}

void CoProxyWidget::SetType(const int Type)
{
    m_Type = Type;
}

int CoProxyWidget::GetType() const
{
    return m_Type;
}

void CoProxyWidget::SetId(const int id)
{
    m_Id = id;
}

int CoProxyWidget::GetId() const
{
    return m_Id;
}

void CoProxyWidget::SetPower(const int power)
{
    m_Power = power;
}

int CoProxyWidget::GetPower() const
{
    return m_Power;
}

void CoProxyWidget::SetProName(const QString name)
{
    m_ProName = name;
}

QString CoProxyWidget::GetProName() const
{
    return m_ProName;
}

void CoProxyWidget::SetStartPos(double StartX, double StartY)
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

void CoProxyWidget::SetStopPos(double StopX, double StopY)
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

bool CoProxyWidget::GetMoveAble() const
{
    return m_MoveAble;
}

bool CoProxyWidget::SetMoveAble(bool Able)
{
    m_MoveAble = Able;
    MoveEnable(m_MoveAble);
    return true;
}

bool CoProxyWidget::SetSelectAble(bool Able)
{
    m_SelectAble = Able;
    setFlag(ItemIsSelectable, Able);

    return true;
}

bool CoProxyWidget::GetSelectAble()
{
    return m_SelectAble;
}

bool CoProxyWidget::SetDragAble(bool Able)
{
    m_DragAble = Able;
    return true;
}

bool CoProxyWidget::GetDragAble() const
{
    return m_DragAble;
}
void CoProxyWidget::SetLineColor(const QColor &color)
{
    if(color.isValid())
    {
        m_LineColor = color;
    }
}

void CoProxyWidget::SetLineColor(int r, int g, int b, int a)
{
    m_LineColor = QColor(r, g, b, a);
}

QColor CoProxyWidget::GetLineColor() const
{
    return m_LineColor;
}

void CoProxyWidget::SetLineStyle(const Qt::PenStyle &style)
{
    m_LineStyle = style;
}

void CoProxyWidget::SetLineStyle(int style)
{
    m_LineStyle = Qt::PenStyle(style);
}

Qt::PenStyle CoProxyWidget::GetLineStyle() const
{
    return m_LineStyle;
}

void CoProxyWidget::SetLineWidth(int width)
{
    m_LineWidth = width;
}

int CoProxyWidget::GetLineWidth() const
{
    return m_LineWidth;
}
void CoProxyWidget::SetBackGroundColor(const QColor &color)
{
    if(color.isValid())
    {
        m_BackGroundColor = color;
    }
}

void CoProxyWidget::SetBackGroundColor(int r, int g, int b, int a)
{
    m_BackGroundColor = QColor(r, g, b, a);
}

QColor CoProxyWidget::GetBackGroundColor() const
{
    return m_BackGroundColor;
}
bool CoProxyWidget::SetRotateAngle(double Angle)
{
    return DoRotate(Angle - m_RotateAngle);
}

bool CoProxyWidget::DoRotate(double Angle)
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

double CoProxyWidget::GetRotateAngle()
{
    return m_RotateAngle;
}

bool CoProxyWidget::SetRotateCenter(double X, double Y)
{
    QPointF Point = mapFromScene(X, Y);

    m_RotateCenterX = Point.x();
    m_RotateCenterY = Point.y();
    return true;
}

double CoProxyWidget::GetRotateCenterX()
{
    //return mRotateCenterX;
    return mapToScene(m_RotateCenterX, m_RotateCenterY).x();
}

double CoProxyWidget::GetRotateCenterY()
{
    //return mRotateCenterY;
    return mapToScene(m_RotateCenterX, m_RotateCenterY).y();
}
double CoProxyWidget::GetCurrentStartX()
{
    return mapToScene(m_StartX, m_StartY).x();
}

double CoProxyWidget::GetCurrentStartY()
{
    return mapToScene(m_StartX, m_StartY).y();
}

double CoProxyWidget::GetCurrentStopX()
{
    return mapToScene(m_StopX, m_StopY).x();
}

double CoProxyWidget::GetCurrentStopY()
{
    return mapToScene(m_StopX, m_StopY).y();
}

double CoProxyWidget::GetCurrentCenterX()
{
    double StartX = GetCurrentStartX();
    double StopX = GetCurrentStopX();

    return StartX + (StopX - StartX) / 2;
}

double CoProxyWidget::GetCurrentCenterY()
{
    double StartY = GetCurrentStartY();
    double StopY = GetCurrentStopY();

    return StartY + (StopY - StartY) / 2;
}

void CoProxyWidget::SetZValue(double Z)
{
    setZValue(Z);
}

double CoProxyWidget::GetZValue()
{
    return zValue();
}

void CoProxyWidget::MoveEnable(bool Enable)
{
    setFlag(ItemIsMovable, Enable);
}

void CoProxyWidget::UpdateMouse(QPointF &PosNow)
{
    if(!m_DragAble)
    {
        return;
    }
    QCursor mCursor;
    QRectF mRect = this->rect();
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
double CoProxyWidget::GetStartX()
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

double CoProxyWidget::GetStartY()
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

double CoProxyWidget::GetStopX()
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

double CoProxyWidget::GetStopY()
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
double CoProxyWidget::GetCenterX()
{
    double StartX = GetStartX();
    double StopX = GetStopX();

    return StartX + (StopX - StartX) / 2;
}

double CoProxyWidget::GetCenterY()
{
    double StartY = GetStartY();
    double StopY = GetStopY();

    return StartY + (StopY - StartY) / 2;
}
void CoProxyWidget::TransFormPos()
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
void CoProxyWidget::MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
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
void CoProxyWidget::MovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
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

