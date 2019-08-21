#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include "Treewidget.h"
class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(QWidget *parent = 0);
signals:
    void SendNewCanvasEvent(const QString &);
    void SendNewProjectEvent();
    void SendDelCanvasEvent(const QString &,const QString &);
    void SendChangeNodeEvent(const QString &,const QString &,int);
    void SendDelProjectEvent(const QString &);
    void SendRenameProjectEvent(const QString &,const QString &);
    void SendRenameCanvasEvent(const QString &,const QString &,const QString &);

    void FeedbackNewCanvasEventd(const QString&);
    void FeedbackNewProjectEventd(const QString&);
    void FeedbackDelCanvasEventd();
    void FeedbackDelProjectEventd();
public slots:
    void TransferNewProject();
    void TransferDelProject(const QString &);
    void TransferNewCanvas(const QString &);
    void TransferDelCanvas(const QString &,const QString &);
    void TransferChangeTreeNode(const QString &, const QString &, int );
    void TransferRenameProject(const QString &,const QString &);
    void TransferRenameCanvas(const QString &,const QString &,const QString &);

    void TransferNewProjectEnd(const QString&);
    void TransferDelProjectEnd();
    void TransferNewCanvasEnd(const QString&);
    void TransferDelCanvasEnd();
private:
    CoTreeWidget *m_treeWidget;

};

#endif // TASKWIDGET_H
