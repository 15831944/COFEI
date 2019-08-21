#ifndef COTREEWIDGET_H
#define COTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QContextMenuEvent>
#include <QMenu>
class CoTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit CoTreeWidget();
    enum TreeName
    {
        Base            = 0,
        Root            = 1,
        Project         = 2,
        CanvasGroup     = 10,
        Canvas          = 11,
        _DataBase       = 20,
        Authority       = 21,
        DataArea        = 22,
        Devices         = 101,
        SafeArea        = 201,
        FlowChart       = 1010,
        Flow            = 1011,
        Reset           = 1012,
        Org             = 1013,
        FlowThread      = 1020,
        ChlidReset      = 1021,
        ResetThread     = 1022,
        OrgThread       = 1023,
        Max
    };
    Q_ENUMS(TreeName)
private:
    QTreeWidgetItem* m_pRootItem;
    QMenu* CreateContextMenu(int index = 0);
    QMenu* CreateContextProMenu(int index = 0);
    void DeleteItem(QTreeWidgetItem* node);
    QString GetEnumToString(int ienum);
    int GetStringToEnum(QString str);
signals:
    void SendRenameProjectEvent(const QString &,const QString &);
    void SendRenameCanvasEvent(const QString &,const QString &,const QString &,QString flowName= "");
    void SendNewCanvasEvent(const QString &,const int);
    void SendNewProjectEvent();
    void SendDelCanvasEvent(const QString &,const QString &,const int);
    void SendDelProjectEvent(const QString &);
    void SendChangeTreeNodeEvent(const QString &,const QString &,int);
protected slots:
    void closeEditor( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );
private slots:
    void AddCanvas();
    void AddFlow();
    void AddReset();
    void AddOrg();
    void AddChlidReset();
    void AddFlowThread();
    void AddResetThread();
    void AddOrgThread();

    void DeleteCanvas();
    void DeleteFlow();
    void DeleteReset();
    void DeleteOrg();
    void DelChlidReset();
    void DelFlowThread();
    void DelResetThread();
    void DelOrgThread();
    void AddProject();
    void DeleteProject();
    void ChangeNodeItem(QTreeWidgetItem *current, QTreeWidgetItem *previous);
public slots:
    void AddCanvasItem(const QString&,int);
    void DeleteCanvasItem();
    void AddProjectItem(const QString&);
    void DeleteProjectItem();
    void SetCurItem(const QString , const int );
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    TreeName m_treeName;
};

#endif // COTREEWIDGET_H
