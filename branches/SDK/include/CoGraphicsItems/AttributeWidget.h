#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QRadioButton>
#include <QButtonGroup>
#include <CoGraphics/Base/Base.h>
#include <CoGraphicsItems/AttributeInstance.h>
class CoItemEx;
class CoProxyWidget;
class CoAttributeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoAttributeWidget(QWidget *parent = 0);
    enum CoAttrName
    {
        Base            = 0,
        Index,
        Power              ,
        Text               ,
        FontSize           ,
        FontColor          ,
        AlignHorizontal    ,
        AlignVertical      ,
        PressMode          ,
        PixMap             ,
        BGColor            ,
        SelectPix          ,
        StateVarName,
        StateBGColor,
        StateSelectPix,
        StateBGPix,
        StateFColor,
        StateText,

        JsTactic,
        SwitchScreen,
        OPFunc,
        VarName,

        EnableExpre,
        EnableState,

        OutputMode,
        Unit,
        ValueType,
        ValueFormat,
        IntAccurecy,
        DecAccurecy,

        RefreshFreq,

        MaxValue,
        MinValue,

        Title,

        AxisLblTxt,
        GridColor,
        XAxisCount,
        YAxisCount,
        XAxisLength,
        GridVisiableState,
        AxisLblColor,
        TimeFormat,
        CtrlVarName,
        CurveColor,
        CalVar,

        SafeState,
        FlowName,
        SafeString,
        RowSum,
        ZValue,

        Max
    };
    Q_ENUMS(CoAttrName)
signals:
    void SendTextEditedEvent(uchar,int,const QString&);
protected:
    QString GetEnumToString(int ienum);
    int GetStringToEnum(QString str);
public:
    virtual void SetItem(CoItemEx*) ;
    virtual void SetWidget(CoProxyWidget*) ;

    int GetId() const;
    int GetType() const;
    void SetId(const int);
    void SetType(const int);
private:
    int m_Id;
    int m_Type;
};

class GraphicAttrWidget : public CoAttributeWidget
{
    Q_OBJECT
public:
    explicit GraphicAttrWidget(QWidget *parent = 0);

signals:
    void SendTAttribute(const int type, const QString value, uchar uc);

protected:
    QLabel      *m_pLabelIdValue;
    QLabel      *m_pLabelTypeValue;
    QComboBox   *m_pCmbSelectPower;
    QGridLayout *m_pGridLayout;

    virtual void SetItem(CoItemEx*);
    Q_SLOT void PowerChanged(int);
protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    QString GetStyleSheetString();
};

#endif // ATTRIBUTEWIDGET_H
