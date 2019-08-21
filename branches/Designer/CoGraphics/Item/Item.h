#ifndef COITEM_H
#define COITEM_H

#include <QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QPainter>

class CoItem : public QObject, public QGraphicsItem
{

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    CoItem(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~CoItem();

    enum CoType{
        Base         = 0,
        GroupBox        ,
        SliderF         ,
        ComboBox        ,
        TableView       ,
        Button          ,
        CalBtn          ,
        TextEdit        ,
        Label           ,
        Warnlamp        ,
        Iolamp          ,
        GPInPut         ,
        GPOutPut        ,
        Pixmap          ,
        Chart           ,
        Voice           ,
        Line            ,
        Ellipse         ,
        Rect            ,
        Polygon         ,
        Arrow           ,
        Diamond         ,
        Meter           ,
        CricClock       ,
        RectClock       ,
        SigLevel        ,
        SafeState       ,
        FlowStart       ,
        FlowThread      ,
        FlowCall        ,
        FlowIF          ,
        FlowSport       ,
        FlowArrow       ,
        FlowEnd         ,
        Flowflow        ,
        M00             ,
        M10             ,
        Goto            ,
        IDs             ,
        IF              ,
        G00             ,
        G101            ,
        Dely            ,
        Add             ,
        Ds              ,
        Post            ,
        In              ,
        Out             ,
        Lin             ,
        Lout            ,
        Jog             ,
        T01             ,
        Org             ,
        Log             ,
        Cyt             ,
        G01             ,
        G02             ,
        G03             ,
        G04             ,
        G05             ,
        G06             ,
        Js              ,
        Tcp_R           ,
        Tcp_W           ,
        RS_R            ,
        RS_w            ,
        Wait            ,
        Mov             ,
        B_1             ,
        B_2             ,
        B_3             ,
        Mde_Axis_dug    ,
        Mde_Safe_Sig    ,
        Mde_Login       ,
        Mde_3           ,
        Mde_4           ,
        Mde_5           ,
        Mde_6           ,
        Max
    };
    Q_ENUMS(CoType)
    enum ArrowPost{
        Arrow_Free          = 0,
        Arrow_Top           = 1,
        Arrow_Right         = 2,
        Arrow_Bottom        = 3,
        Arrow_Left          = 4
    };
    int m_ArrowPost;

public:
    virtual QRectF      boundingRect() const;
    virtual void        paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QPainter*   BeforePaint(QPainter* &painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void        Draw(QPainter* painter) = 0;
    virtual void        AfterDraw(QPainter* &painter);
    // 鼠标事件
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    // 键盘事件
    virtual void    keyReleaseEvent(QKeyEvent *event);

signals:
    void mDrop(CoItem *Item, double StartDx, double StartDy, double StopDx, double StopDy);
    void mSwithScene(const QString&,const QString&);

public slots:
    virtual CoItem* Copy() = 0;             // 创建一个
    virtual void Copy(CoItem *ItemFrom);  // 根据拷贝已有对象的属性

    virtual bool SetType(const int Type);
    virtual int GetType() const;
    virtual void SetId(const int id);
    virtual int GetId() const;
    virtual void SetPower(const int power);
    virtual int GetPower() const;
    virtual void SetProName(const QString);
    virtual QString GetProName() const;

    virtual bool GetMoveAble() const;
    virtual bool SetMoveAble(bool Able);
    virtual bool SetSelectAble(bool Able);
    virtual bool GetSelectAble();
    virtual bool SetDragAble(bool Able);
    virtual bool GetDragAble() const;
    int GetMouseState() const;

    // 旋转
    bool SetRotateAngle(double Angle);
    bool DoRotate(double Angle);
    double GetRotateAngle();
    bool SetRotateCenter(double X, double Y);
    double GetRotateCenterX();
    double GetRotateCenterY();

    // 坐标，未进行旋转等变换
    virtual double GetStartX();
    virtual double GetStartY();
    virtual double GetStopX();
    virtual double GetStopY();
    virtual double GetCenterX();
    virtual double GetCenterY();
    virtual void SetStartPos(double StartX, double StartY);
    virtual void SetStopPos(double StopX, double StopY);
    virtual void SetStartPos(const QPointF &Start);
    virtual void SetStopPos(const QPointF &Stop);
    virtual void MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);

    // 坐标，旋转等变换以后
    virtual double GetCurrentStartX();
    virtual double GetCurrentStartY();
    virtual double GetCurrentStopX();
    virtual double GetCurrentStopY();
    virtual double GetCurrentCenterX();
    virtual double GetCurrentCenterY();

    // 样式
    virtual void SetLineColor(const QColor &color);
    virtual void SetLineColor(int r, int g, int b, int a = 255);
    virtual QColor GetLineColor() const;

    virtual void SetLineStyle(const Qt::PenStyle &style);
    virtual void SetLineStyle(int style);
    virtual Qt::PenStyle GetLineStyle() const;

    virtual void SetLineWidth(int width);
    virtual int GetLineWidth() const;

    virtual void SetBackGroundColor(const QColor &color);
    virtual void SetBackGroundColor(int r, int g, int b, int a = 255);
    virtual QColor GetBackGroundColor() const;

    virtual void SetZValue(double Z);
    virtual double GetZValue();

    virtual void Update();

protected:
    int m_Type;
    int m_Id;
    int m_Power;
    QString m_ProName;
    double m_RotateAngle;
    double m_RotateCenterX;
    double m_RotateCenterY;

    double m_StartX;
    double m_StartY;
    double m_StopX;
    double m_StopY;

    int m_LineWidth;
    QColor m_LineColor;
    Qt::PenStyle m_LineStyle;
    QColor m_BackGroundColor;
    QColor m_CurFontColor;

    bool m_SelectAble;
    bool m_MoveAble;
    bool m_DragAble;
    bool m_DropScale;        // 是否开始拖动

    enum
    {
        Direc_Free          = 0,
        Direc_Top           = 1,
        Direc_RightTop      = 2,
        Direc_Right         = 3,
        Direc_RightBottom   = 4,
        Direc_Bottom        = 5,
        Direc_LeftBottom    = 6,
        Direc_Left          = 7,
        Direc_LeftTop       = 8,
    };
    int m_DropDirection;     // 拖动方向

    QPointF m_DropPos;     // 拖动点,会随着拖动过程改变
    QPointF m_PressStart;  // 拖动起点

    virtual void MoveEnable(bool Enable);
    virtual void UpdateMouse(QPointF &PosNow);

    virtual void MovePos(double NewStartX, double NewStartY, double NewStopX, double NewStopY);
    virtual void TransFormPos();
public:
    static QString GetEnumToString(int ienum);
    static int GetStringToEnum(QString str);
    bool    m_isfirstSelected;
};

#endif // COITEM_H
