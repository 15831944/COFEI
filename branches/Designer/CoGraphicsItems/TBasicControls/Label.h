#ifndef TLABLE_H
#define TLABLE_H

#include <CoGraphics/Graphics.h>

///////////////////////////////////////////////// 标签  /////////////////////////////////////////////////////
class TLabel : public CoWidgets
{
    Q_OBJECT
public:
    TLabel(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::green, const QColor &BackColor = Qt::white,
          const QString &Text = tr("Label"));

    virtual ~TLabel();

    enum ValueType
    {
        Double = 0,
        Dec,
        Hex,
        Bin,
        BinAndFrontZero,
        Round,
        FrontZero,
        RoundAndFrontZero,
        String,
        Pwd
    };

public slots:
    CoItem *Copy();
    void Copy(TLabel *TextFrom);
    void SetText(const QString Text);
    void SetAttributeEvent(const int,const QString,uchar uc = 0);

    void SetOutputMode(const uchar);
    uchar GetOutputMode() const;

    void SetVarName(const QString);
    QString GetVarName() const;
    char GetVarType() const;

    void SetUnit(const QString);
    QString GetUnit() const;

    void SetValueType(const uchar);
    uchar GetValueType() const;

    void SetValueFormat(const uchar);
    uchar GetValueFormat() const;

    void SetIntAccurecy(const uchar);
    uchar GetIntAccurecy() const;

    void SetDecAccurecy(const uchar);
    uchar GetDecAccurecy() const;

    void SetJsTactic(const QString);
    QString GetJsTactic() const;


    void SetFontSpacing(int Spacing);
    int GetFontSpacing() const;
    void SetFontBlod(bool Blod);
    bool GetFontBlod() const;
    void SetFontItalic(bool Italic);
    bool GetFontItalic() const;
    void SetFontUnderLine(bool UnderLine);
    bool GetFontUnderLine() const;
    void SetFontStrikeOut(bool StrikeOut);
    bool GetFontStrikeOut() const;

private slots:
    void UpdateText();

private:
    uchar m_OutputMode;     //显示输出0,1
    QString m_strVarName;   //变量名
    char m_VarType;        //变量类型
    QString m_strUnit;      //单位
    uchar m_ValueType;      //输出值类型,0:数值,1:字符
    uchar m_ValueFormat;    //输出格式0~9
    uchar m_IntAccurecy;    //整数位数
    uchar m_DecAccurecy;    //小数位数
    QString m_strJsPath;

    bool m_FontBlod;
    bool m_FontItalic;
    int m_FontSpacing;
    bool m_FontUnderLine;
    bool m_FontStrikeOut;

    QString m_strTextBackup;
    double m_dPreview;

    void Draw(QPainter *painter);
    void SetTextByFormat(int type);
};
#endif // TLABLE_H
