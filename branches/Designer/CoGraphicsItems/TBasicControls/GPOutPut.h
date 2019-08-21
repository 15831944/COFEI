#ifndef TGPOUTPUT_H
#define TGPOUTPUT_H

#include <CoGraphics/Graphics.h>
class TGPOutPut : public CoWidgets
{
    Q_OBJECT
public:
    TGPOutPut(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TGPOutPut(){}

public slots:
    CoItem *Copy();
    void Copy(TGPOutPut *IoFrom);
    void SetAttributeEvent(const int , const QString , uchar);

    void SetIOName(const QString);
    QString GetIOName() const;
    char GetIOState() const;

    void SetJsTactic(const QString, uchar uc);
    QVector<QString> GetJsTactic() const;

    void SetEnableExpre(const QString);
    QString GetEnableExpre() const;

    void SetEnableState(const int);
    int GetEnableState() const;

private:
    bool m_Pressed;
    bool m_IsSafe;
    QString m_IOName;
    char m_IOState;
    QVector<QString> m_JsTacticVec; //m_JsTacticVec[0]抬起;m_JsTacticVec[1]按下
    QString m_EnableExpre;
    int m_EnableState;

    void Draw(QPainter *painter);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TGPOUTPUT_H
