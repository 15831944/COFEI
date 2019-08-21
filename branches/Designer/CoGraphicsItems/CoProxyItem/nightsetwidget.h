#ifndef NIGHTSETWIDGET_H
#define NIGHTSETWIDGET_H
#include <CoGraphics/Graphics.h>
#include <CoGraphics/Scene/Scene.h>
#include <QTableWidget>
#include <QGraphicsGridLayout>
#include <QGridLayout>
#include <QPushButton>
#include <Comm.h>
#include <QMouseEvent>
#include "koroldelegate.h"
#include <QAbstractItemDelegate>
class NightSetWidget : public CoProxyWidget
{
public:
    NightSetWidget(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                   const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                   const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black,
                   CoScene* Scene = NULL);
private:
    QTableWidget *m_ptabwidget;
    QPushButton *btn;
    QGraphicsGridLayout *m_pLayout;

public:
    void SetTabWidgetValue(const QMap<QString,SENSOR_LIGHT_STRU> tabValueMap);
    QMap<QString,SENSOR_LIGHT_STRU> GetTabWidgetValue();
private:
    bool eventFilter(QObject *watched, QEvent *event);
    void SetTabWidgetStyle(QTableWidget *tabwidget);
    QPointF m_point;
    int m_xoffsict;
    QPoint m_globlePoint;
    KorolDelegate *m_pdelegate;
public slots:
    CoProxyWidget *Copy();
    void Copy(NightSetWidget *LightFrom);

};

#endif // NIGHTSETWIDGET_H
