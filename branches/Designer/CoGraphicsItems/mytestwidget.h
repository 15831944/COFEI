#ifndef MYTESTWIDGET_H
#define MYTESTWIDGET_H

#include <CoGraphics/Graphics.h>
#include <CoGraphics/Scene/Scene.h>
class MyTestWidget :public CoProxyWidget
{
public:
    MyTestWidget(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                 const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                 const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black,
                 CoScene* Scene = NULL);
public slots:
    CoProxyWidget *Copy();
    void Copy(MyTestWidget *ButtonFrom);
};

#endif // MYTESTWIDGET_H
