#ifndef TCOMBOBOX_H
#define TCOMBOBOX_H

#include <CoGraphics/Graphics.h>
#include <QComboBox>
#include <QGridLayout>
/////////////////////////////////////////////////  组合  ////////////////////////////////////////////////////
class TComboBox : public CoWidgets
{
    Q_OBJECT
public:
    TComboBox(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TComboBox();

public slots:
    CoItem *Copy();
    void Copy(TComboBox *ComboBoxFrom);
    void SetText(QString Text);
    QString GetText();

private:
    QString m_CurText;
    double m_CurStopY;
    QVector<QString> m_TextVec;
    bool m_isDown;
    bool m_isDrop;
    void Draw(QPainter *painter);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QRectF  boundingRect() const;
};

#endif // TCOMBOBOX_H
