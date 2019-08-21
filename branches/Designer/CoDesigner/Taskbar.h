#ifndef TASKPANEL_H
#define TASKPANEL_H

#include <QWidget>
#include <CoWidgetBox/TaskBar/Taskbar.h>
#include <CoWidgetBox/TaskBar/Taskbox.h>
class TaskBar : public QWidget
{
    Q_OBJECT
public:
    explicit TaskBar(QWidget *parent = 0);

signals:
    void SendBarItemClick(CoAction*);
public slots:
    void HideTaskBox(QList<QString>);
private slots:
    void IconLabelClicked();
private:
    CoTaskBar *m_panel;
    CoTaskBox *m_tRootbox;
    CoIconLabel * CreateIconLabel(const QString, const QString, const int,QWidget *parent = 0);
};

#endif // TASKPANEL_H
