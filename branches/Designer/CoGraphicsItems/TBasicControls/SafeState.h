#ifndef SAFESTATE_H
#define SAFESTATE_H

#include <CoGraphics/Graphics.h>

class TSafeState : public CoWidgets
{
    Q_OBJECT
public:
    TSafeState(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::green, const QColor &BackColor = Qt::white,
              const QString &Text = tr("总状态"));
    virtual ~TSafeState(){}

public slots:
    CoItem *Copy();
    void Copy(TSafeState *From);
    void SetAttributeEvent(const int,const QString,uchar uc = 0);

    void SetSafeState(uchar);
    uchar GetSafeState();

    void SetFlowName(const QString&);
    QString GetFlowName();

    void SetSafeString(const QString&);
    QString GetSafeString();

private:
    uchar m_SafeState;
    QString m_strFlowName;
    QString m_strSafeString;

private:
    void Draw(QPainter *painter);
};

#endif // SAFESTATE_H
