#ifndef PERMISSONMANAGER_H
#define PERMISSONMANAGER_H
#include <QObject>
#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QTableWidget>
#include <QItemDelegate>
#include <QObject>
#include <QSpinBox>
#include "dataManagerpermiss_global.h"
class DATAMANAGERPERMISSSHARED_EXPORT databasepermiss :public QWidget
{
    Q_OBJECT
public:
    explicit databasepermiss(QWidget *parent = 0);
    ~databasepermiss();
public:
    QWidget *GetPerAttrArea(){
        m_attWg = NULL;
        m_attWg = new QWidget(this);
        return m_attWg;
    }

private:
    QString path;
    QWidget *m_attWg;
private:
    QMenu* CreatQmenu();
    void AddData();
    void UpdateData();
    void DeleteData();
    void Save();
    QSqlTableModel *m_pertabmodel;
    void InitPermanagertable();
    bool Excisexisttable(QString ,  QSqlDatabase database);
    void InitDataBase();
    QSqlTableModel *m_tabmodel;
    QTableView *m_tabview;

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};
class SpinBoxDelegate : public QItemDelegate
{
      Q_OBJECT
public:
   SpinBoxDelegate(QObject *parent = 0);
    // QObject interface
public:
   // bool eventFilter(QObject *watched, QEvent *event);

    // QAbstractItemDelegate interface
public:
   // void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
   // bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // PERMISSONMANAGER_H
