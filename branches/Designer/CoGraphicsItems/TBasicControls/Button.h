#ifndef TBUTTON_H
#define TBUTTON_H

#include <CoGraphics/Graphics.h>
#include <QApplication>
class TButton : public CoWidgets
{
    Q_OBJECT
public:
    TButton(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TButton();

public slots:
    CoItem *Copy();
    void Copy(TButton *ButtonFrom);
    void SetAttributeEvent(const int,const QString,uchar uc = 0);
    void SetPressed(bool b);

    void SetPressMode(const uchar);
    uchar GetPressMode() const;

    void SetVarName(const QString);
    QString GetVarName() const;
    uchar GetVarState() const;
    char GetVarType() const;

    void SetJsTactic(const QString, uchar uc);
    QVector<QString> GetJsTactic() const;

    void SetOpFunc(char);
    char GetOpFunc() const;

    void SetScreen(const QString);
    QString GetScreen() const;

    void SetEnableExpre(const QString);
    QString GetEnableExpre() const;

    void SetEnableState(const int);
    int GetEnableState() const;

    void SetStateVarName(const QString);
    QString GetStateVarName() const;
    uchar GetStateVarType() const;

    void SetStateSelectPix(bool,const uchar);
    QVector<bool> GetStateSelectPix() const;

    void SetStateBGPix(const QString,const uchar);
    QVector<QString> GetStateBGPix() const;

    void SetStateBGColor(const QColor,const uchar);
    QVector<QColor> GetStateBGColor() const;

    void SetStateFontColor(const QColor,const uchar);
    QVector<QColor> GetStateFontColor() const;

    void SetStateText(const QString,const uchar);
    QVector<QString> GetStateText() const;

    void UpdateState();

protected:
    bool m_IsSafe;
    bool m_Pressed;
    uchar m_PressMode;
    QString m_VarName;
    uchar m_VarState;
    char m_VarType;

    QVector<QString> m_JsTacticVec; //m_JsTacticVec[0]抬起;m_JsTacticVec[1]按下
    QString m_ScreenName;
    FunctionType m_euOpFunc;        //启动，暂停，继续，停止，急停，复位，回原点，关闭程序，关机，重启系统。
    QString m_EnableExpre;
    int m_EnableState;

    QString m_stateVarName;  //状态属性|控制变量名
    uchar m_stateVarType;  //状态属性|控制变类型
    QVector<QString> m_StateBGPixVec;   //状态属性|0，1状态背景图
    QVector<bool> m_StateSelectPixVec;   //状态属性|0，1是否使用背景图
    QVector<QColor> m_StateBGColorVec;   //状态属性|0，1状态背景色
    QVector<QColor> m_StateFontColorVec;   //状态属性|0，1状态字体颜色
    QVector<QString> m_StateTextVec;   //状态属性|0，1状态文本


    virtual void Draw(QPainter *painter);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void SetVar(const QString name,uchar state,uchar pressState);
    QString GetOPNameByType(FunctionType);
    /// \param pressState: 1 press;0 release
    double CalculateByState(double value,uchar state,uchar pressState);
    bool m_IsZero;
    uchar m_Count;

};

#endif // TBUTTON_H
