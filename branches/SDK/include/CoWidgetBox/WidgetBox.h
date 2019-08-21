#ifndef WIDGETBOX_H
#define WIDGETBOX_H
#include <QWidget>
#include "CoWidgetBox_global.h"
#include <QGridLayout>
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
