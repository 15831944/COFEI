#include "Taskwidget.h"
#include <QVBoxLayout>
#include <QMenu>
TaskWidget::TaskWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    this->setLayout(vbox);
    vbox->setContentsMargins(0,0,0,0);
    m_treeWidget = new CoTreeWidget();
    vbox->addWidget(m_treeWidget);
    connect(m_treeWidget,&CoTreeWidget::SendDelCanvasEvent,this,&TaskWidget::TransferDelCanvas);
    connect(m_treeWidget,&CoTreeWidget::SendDelProjectEvent,this,&TaskWidget::TransferDelProject);
    connect(m_treeWidget,&CoTreeWidget::SendNewCanvasEvent,this,&TaskWidget::TransferNewCanvas);
    connect(m_treeWidget,&CoTreeWidget::SendNewProjectEvent,this,&TaskWidget::TransferNewProject);
    connect(m_treeWidget,&CoTreeWidget::SendRenameCanvasEvent,this,&TaskWidget::TransferRenameCanvas);
    connect(m_treeWidget,&CoTreeWidget::SendRenameProjectEvent,this,&TaskWidget::TransferRenameProject);
    connect(m_treeWidget,&CoTreeWidget::SendChangeTreeNodeEvent,this,&TaskWidget::TransferChangeTreeNode);

    connect(this,&TaskWidget::FeedbackNewCanvasEventd,m_treeWidget,&CoTreeWidget::AddCanvasItem);
    connect(this,&TaskWidget::FeedbackNewProjectEventd,m_treeWidget,&CoTreeWidget::AddProjectItem);
    connect(this,&TaskWidget::FeedbackDelCanvasEventd,m_treeWidget,&CoTreeWidget::DeleteCanvasItem);
    connect(this,&TaskWidget::FeedbackDelProjectEventd,m_treeWidget,&CoTreeWidget::DeleteProjectItem);
}

void TaskWidget::TransferNewProject()
{
    emit SendNewProjectEvent();
}

void TaskWidget::TransferDelProject(const QString & Name)
{
    emit SendDelProjectEvent(Name);
}

void TaskWidget::TransferNewCanvas(const QString & name)
{
    emit SendNewCanvasEvent(name);
}

void TaskWidget::TransferDelCanvas(const QString& proName,const QString & Name)
{
    emit SendDelCanvasEvent(proName,Name);
}

void TaskWidget::TransferChangeTreeNode(const QString &proName, const QString &Name,int iName)
{
    emit SendChangeNodeEvent(proName,Name,iName);
}

void TaskWidget::TransferRenameProject(const QString & oldName, const QString & newName)
{
    emit SendRenameProjectEvent(oldName,newName);
}

void TaskWidget::TransferRenameCanvas(const QString & proName,const QString & oldName, const QString & newName)
{
    emit SendRenameCanvasEvent(proName,oldName,newName);
}

void TaskWidget::TransferNewProjectEnd(const QString & name)
{
    emit FeedbackNewProjectEventd(name);
}

void TaskWidget::TransferDelProjectEnd()
{
    emit FeedbackDelProjectEventd();
}

void TaskWidget::TransferNewCanvasEnd(const QString & name)
{
    emit FeedbackNewCanvasEventd(name);
}

void TaskWidget::TransferDelCanvasEnd()
{
    emit FeedbackDelCanvasEventd();
}
