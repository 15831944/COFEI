#ifndef IOLAMP_H
#define IOLAMP_H

#include <CoGraphics/Graphics.h>
class TIOLamp : public CoGraphics
{
    Q_OBJECT
public:
    TIOLamp(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TIOLamp();

public slots:
    CoItem *Copy();
    void Copy(TIOLamp *IoFrom);
    void SetAttributeEvent(const int , const QString , uchar);

    void SetVarName(const QString&);
    QString GetVarName() const;
    char GetVarType() const;
    void SetExpre(const QString);
    QString GetExpre() const;

protected slots:
    virtual void AutoRunRepeat();
private:
    QString m_VarName;          //变量名
    char m_VarType;            //变量类型
    QString m_Expre;           //表达式

    bool m_Pressed;
    bool m_IsGreen;
    void Draw(QPainter *painter);
};

#endif // IOLAMP_H
