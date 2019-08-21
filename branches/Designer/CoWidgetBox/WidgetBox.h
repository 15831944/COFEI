#ifndef WIDGETBOX_H
#define WIDGETBOX_H
#include <QWidget>
#include <QGridLayout>
#include "CoWidgetBox_global.h"

class COWIDGETBOXSHARED_EXPORT CoWidgetBox : public QWidget
{
    Q_OBJECT
public:
    CoWidgetBox(QWidget *parent = 0);
    void AddWidget(QWidget*);
private:
    QGridLayout *m_pLayout;
signals:
    void TransferHideEvent(bool);
public slots:
    void HandleHideEvent(bool);
};

#endif // WIDGETBOX_H
