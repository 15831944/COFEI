#ifndef CALBUTTON_H
#define CALBUTTON_H
#include "Button.h"


class TCalButton : public CoWidgets
{
    Q_OBJECT
public:
    TCalButton(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    ~TCalButton(){}

public:
    Q_SLOT CoItem *Copy();
    Q_SLOT void Copy(TCalButton *calBtn);
    Q_SLOT void SetAttributeEvent(const int,const QString,uchar uc = 0);

    void SetCalVar(const QString varInfo,uchar uc);
    QString GetCalVarComb1() const;
    QString GetCalVarComb2() const;
    QString GetCalVarComb3() const;
    QString GetCalVarComb4() const;
    QString GetCalVarComb5() const;
    QString GetCalVarComb6() const;

    void SetJsTactic(const QString, uchar uc);
    QVector<QString> GetJsTactic() const;

    void SetEnableExpre(const QString);
    QString GetEnableExpre() const;

    void SetEnableState(const int);
    int GetEnableState() const;

protected:
    bool m_IsSafe;
    QString m_EnableExpre;
    int m_EnableState;

    QString m_strCalVarComb1;
    QString m_strCalVarComb2;
    QString m_strCalVarComb3;
    QString m_strCalVarComb4;
    QString m_strCalVarComb5;
    QString m_strCalVarComb6;
    QVector<QString> m_JsTacticVec; //m_JsTacticVec[0]抬起;m_JsTacticVec[1]按下

    virtual void Draw(QPainter *painter);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void ExcCal(QString calVarComb);
};

#endif // CALBUTTON_H
