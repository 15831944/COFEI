#ifndef TGPINPUT_H
#define TGPINPUT_H

#include <CoGraphics/Graphics.h>
class TGPInPut : public CoGraphics
{
    Q_OBJECT
public:
    TGPInPut(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TGPInPut(){}

public slots:
    CoItem *Copy();
    void Copy(TGPInPut *IoFrom);
    void SetAttributeEvent(const int , const QString , uchar);

    void SetIOName(const QString& name);
    QString GetIOName() const;

protected slots:
    virtual void AutoRunRepeat();

private:
    QString m_strIOName;
    bool m_IsGreen;
    void Draw(QPainter *painter);
};

#endif // TGPINPUT_H
