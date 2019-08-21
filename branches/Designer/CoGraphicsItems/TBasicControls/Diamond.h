#ifndef TDIAMOND_H
#define TDIAMOND_H

#include <CoGraphics/Graphics.h>
/////////////////////////////////////////////////  矩形  /////////////////////////////////////////////////////
class TDiamond : public CoGraphics
{
    Q_OBJECT
public:
    TDiamond(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TDiamond();

public slots:
    CoItem *Copy();
    void Copy(TDiamond *DiamFrom);
    virtual bool SetSelectAble(bool Able);
    virtual bool GetSelectAble();

    void SetText(const QString &Text);
    QString GetText();

private:
    void Draw(QPainter *painter);
    void keyReleaseEvent(QKeyEvent *event);
    QString m_Text;
};

#endif // TDIAMOND_H
