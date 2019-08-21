#ifndef COGRAPHICS_H
#define COGRAPHICS_H

#include "cographics_global.h"
#include "Item/ItemEx.h"
#include "Js/Js.h"
#include <LibBase.h>
#include <QTimer>
#include <QGraphicsWidget>
class QVariant;
class CoArrow;
///////////////////////  图形基类 /////////////////////////////////////
class COGRAPHICSSHARED_EXPORT CoGraphics : public CoItemEx
{
    Q_OBJECT
public:
    CoGraphics(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~CoGraphics();

    enum
    {
        JS_MIN,
        JS_VALUE,
        JS_AUTO,
        JS_MAX
    };

    enum
    {
        AUTO_NONE           = -1,
        AUTO_AFTER_ALARM    = 0,
        AUTO_REPEAT         = 1
    };

public slots:
    virtual void Copy(CoGraphics *GraphicsFrom);

    // 链接图形
    virtual void SetLinkScene(int LinkId);
    virtual int GetLinkScene() const;

    // 设备地址
    virtual void SetNodeId(const int Id);
    virtual int GetNodeId() const;

    // 值
    virtual void SetCoe(const double Coe);
    virtual double GetCoe() const;
    virtual void SetValue(const double Value);                     // 设定值，并运行值脚本，初始化自身外形,联动脚本
    virtual double GetValue() const;

    virtual inline void UpdateValue(const double Value, qint64 Time) // 设定值，并运行值脚本、报警脚本、联动脚本等
    {
        double ValueChange = Value * m_Coe - m_Value;
        if(ABS(ValueChange) > 0.000001)
        {
            SetValue(Value);

            m_ValueUpdateTime = Time;
        }
    }

    // 报警
    virtual void StartAlarm();

    void SetAutoType(int Type);
    int GetAutoType();

    // 图形联动(在值脚本中，获取该值，然后根据该值执行不同的动作)
    virtual int GetLinkageRecv(); // 当前已经接收到的联动信号相与以后的值

    virtual void SendLinkage(bool Enable, int _iLinkage);     // 向外发送联动信号
    virtual void RecvLinkage(bool Enable, int _iLinkage);     // 接收联动信号的槽

signals:
    void mStartAlarm(double Value, qint64 Time);
    void mGotoLink(int LinkId);
    void mSendLinkage(bool enable, int _iLinkage);       // 向外发送联动信号

protected:
    int m_LinkScene;         // 链接的Scene号

    int m_NodeId;

    double m_Value;
    double m_Coe;
    quint64 m_ValueUpdateTime;

    int m_LinkageRecv;       // 当前已经接收到的联动信号相与以后的值

    int m_AutoType;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected slots:
    virtual void AutoRunRepeat();
};

///////////////////////  流程图基类 /////////////////////////////////////
class COGRAPHICSSHARED_EXPORT CoFlowChart : public CoItemEx
{
    Q_OBJECT
public:
    CoFlowChart(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~CoFlowChart();

public slots:
    virtual void Copy(CoFlowChart *FlowFrom);

    void AddInputArrow(CoArrow*);
    void AddOutputArrow(CoArrow*);
    void RemoveInputArrow(CoArrow *);
    void RemoveOutputArrow(CoArrow *);
    void RemoveArrows();
    QList<CoArrow*> GetInputArrows();
    QList<CoArrow*> GetOutputArrows();
protected:
    QList<CoArrow*> m_InputList;
    QList<CoArrow*> m_OutputList;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void AfterDraw(QPainter *&painter);
};

///////////////////////  控件基类 /////////////////////////////////////
class COGRAPHICSSHARED_EXPORT CoWidgets : public CoItemEx
{
    Q_OBJECT
public:
    CoWidgets(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~CoWidgets();
    //启动，暂停，继续，停止，急停，复位，回原点，关闭程序，关机，重启系统。
    enum FunctionType{
        Function_Free          = 0,
        Function_Start            ,
        Function_Suspend          ,
        Function_Resume           ,
        Function_Stop             ,
        Function_EmrgStop         ,
        Function_Reset            ,
        Function_ORG              ,
        Function_Close            ,
        Function_PowerOff         ,
        Function_Reboot
    };
    TJs *m_Js;
public slots:
    virtual void Copy(CoWidgets *WidgetsFrom);
    virtual void SendTo(int Type , const QString msg,double ValueFloat = 0,
                         int ValueInt = 0, QString Text = tr(""));

signals:
    void mSendTo(int Type, QString Text);
public slots:
    virtual void SetAttributeEvent(const int,const QString,uchar uc = 0);

    virtual void SetText(const QString Text);
    virtual QString GetText();
    virtual void SetFontSize(const uchar);
    virtual uchar GetFontSize() const;
    virtual void SetFontColor(const QColor);
    virtual QColor GetFontColor() const;
    virtual void SetFontAlignHor(const uchar);
    virtual uchar GetFontAlignHor() const;
    virtual void SetFontAlignVer(const uchar);
    virtual uchar GetFontAlignVer() const;
    ///
    /// \brief SetPixFile
    ///QString Pix filePath
    /// uchar save filePath in m_PixFileVec post 0,1
    void SetPixFile(const QString,const uchar);
    virtual QVector<QString> GetPixFiles() const;
    ///
    /// \brief SetSelectPix
    ///bool select Pix yes or no
    /// uchar save bool in m_SelectPixVec post 0,1
    virtual void SetSelectPix(bool,const uchar);
    virtual QVector<bool> GetSelectPix() const;
    ///
    /// \brief SetBackColor
    ///QColor BackGround Color
    /// uchar save color in m_BackColorVec post 0,1
    virtual void SetBackColor(const QColor,const uchar);
    virtual QVector<QColor> GetBackColor() const;

    virtual void SetAttriIndex(const uchar);
    virtual uchar GetAttriIndex() const;

protected:
    QString m_Text;
    QColor m_FontColor;
    int m_AlignHor;
    int m_AlignVer;
    QString m_CurPixFile;
    QVector<QString> m_PixFileVec;
    QVector<QColor> m_BackColorVec;
    QVector<bool> m_SelectPixVec;
    uchar m_AttriIndex; //属性窗体tab页索引

};

class CoProxyWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    CoProxyWidget(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                  const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                  const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
protected:
//    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *event);
//    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    virtual void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    virtual void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
//    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual QRectF  boundingRect() const;
    virtual void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void mDrop(CoProxyWidget *Item, double StartDx, double StartDy, double StopDx, double StopDy);
public slots:
    virtual CoProxyWidget* Copy() = 0;             // 创建一个
    virtual void Copy(CoProxyWidget *ItemFrom);  // 根据拷贝已有对象的属性

    virtual void SetType(const int Type);
    virtual int GetType() const;
    virtual void SetId(const int id);
    virtual int GetId() const;
    virtual void SetPower(const int power);
    virtual int GetPower() const;
    virtual void SetProName(const QString);
    virtual QString GetProName() const;

    virtual void SetStartPos(double StartX, double StartY);
    virtual void SetStopPos(double StopX, double StopY);
    virtual void MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY);
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
    // 坐标，旋转等变换以后
    virtual double GetCurrentStartX();
    virtual double GetCurrentStartY();
    virtual double GetCurrentStopX();
    virtual double GetCurrentStopY();
    virtual double GetCurrentCenterX();
    virtual double GetCurrentCenterY();

    virtual bool GetMoveAble() const;
    virtual bool SetMoveAble(bool Able);
    virtual bool SetSelectAble(bool Able);
    virtual bool GetSelectAble();
    virtual bool SetDragAble(bool Able);
    virtual bool GetDragAble() const;

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
};

#endif // COGRAPHICS_H
