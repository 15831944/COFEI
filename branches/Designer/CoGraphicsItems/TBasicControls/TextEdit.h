#ifndef TTEXT_EDIT_H
#define TTEXT_EDIT_H

#include <CoGraphics/Graphics.h>
#include <CoGraphics/Js/Js.h>
#include <QGraphicsTextItem>
///////////////////////  控件基类 /////////////////////////////////////
class TTextEdit : public CoWidgets
{
    Q_OBJECT
public:
    TTextEdit(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TTextEdit();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);

public slots:
    CoItem *Copy();
    void Copy(TTextEdit *TextEditFrom);
    void HandleText(const QString& Text);
    void SetAttributeEvent(const int,const QString,uchar uc = 0);

    /// \param varInfo:post string = "<InVarName>,<OutVarName>,<VarType>"
    void SetVarName(const QString varInfo);
    QString GetInputVarName() const;
    QString GetOutputVarName() const;
    char GetVarType() const;

    void SetUnit(const QString);
    QString GetUnit() const;

    void SetValueFormat(const uchar);
    uchar GetValueFormat() const;

    void SetIntAccurecy(const uchar);
    uchar GetIntAccurecy() const;

    void SetDecAccurecy(const uchar);
    uchar GetDecAccurecy() const;

    void SetMaxValue(const double);
    double GetMaxValue() const;

    void SetMinValue(const double);
    double GetMinValue() const;

    void SetEnableExpre(const QString);
    QString GetEnableExpre() const;

    void SetEnableState(const int);
    int GetEnableState() const;

    void SetJsTactic(const QString);
    QString GetJsTactic() const;

    void SetPreviewMode(bool);

signals:
    void TextChange(const QString& Text);

private:
    QString m_strInputVarName;   //变量名
    QString m_strOutputVarName;
    char    m_VarType;        //变量类型
    QString m_strUnit;      //单位
    uchar   m_ValueFormat;    //输出格式0~9
    uchar   m_IntAccurecy;    //整数位数
    uchar   m_DecAccurecy;    //小数位数
    double  m_dMaxVal;       //最大值
    double  m_dMinVal;       //最小值
    QString m_EnableExpre;  //可用性条件表达式
    int    m_EnableState;     //0不可用,1不可用
    QString m_strJsTactic;  //JS脚本

    QString m_strInput;     //用户键盘输入的值
    bool    isFousOut;
    uchar   m_TextType;
    bool    m_IsSafe;
    bool    m_bPreview;
    CoJS    *m_jsExc;
    void Draw(QPainter *painter);
    void SetTextByFormat(int type);


protected:
    virtual void focusInEvent(QFocusEvent * event);
    virtual void focusOutEvent(QFocusEvent * event);
    Q_SLOT void UpdateText();
};

#endif // TText_EDIT_H
