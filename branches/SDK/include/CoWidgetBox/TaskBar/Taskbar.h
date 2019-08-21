#ifndef COTASKPANEL_H
#define COTASKPANEL_H

#include <QtGui>
#include <QtCore>
#include <QWidget>

class CoTaskBarScheme;

class CoTaskBar : public QWidget
{
public:
    CoTaskBar(QWidget *parent = 0);
    ~CoTaskBar();

    void addWidget(QWidget *w);
    void addStretch(int s = 0);

    void setScheme(CoTaskBarScheme *scheme);

protected:
    virtual void paintEvent ( QPaintEvent * event );

    CoTaskBarScheme *myScheme;
};

#endif // COTASKPANEL_H
