#ifndef TGROUPBOX_H
#define TGROUPBOX_H

#include <CoGraphics/Graphics.h>

/////////////////////////////////////////////////  组合  ////////////////////////////////////////////////////
class TGroupBox : public CoGraphics
{
    Q_OBJECT
public:
    TGroupBox(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TGroupBox();

public slots:
    CoItem *Copy();
    void Copy(TGroupBox *GroupFrom);
    void SetAttributeEvent(const int , const QString , uchar);

    // 执行以下选项时，统一采用Grop的样式，丢弃之前的样式
    void SetLineColor(const QColor &color);
    void SetLineColor(int r, int g, int b, int a = 255);
    void SetLineStyle(const Qt::PenStyle &style);
    void SetLineStyle(int style);
    void SetLineWidth(int width);
    void SetTitle(QString Text);
    QString GetTitle();

private:
    QString m_Text;
    void Draw(QPainter* painter);
    QRectF  boundingRect() const;
};

#endif // TGROUPBOX_H
