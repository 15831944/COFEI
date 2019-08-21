#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QStringList>
#include <QTableView>
#include "TableModel.h"
#include "ComboBoxDelegate.h"
#include <QStyledItemDelegate>


class TableView : public QTableView
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent= 0);
    ~TableView(void){}
public:
    void setData(QList<QStringList>& list);
    void SetHeaderData(QStringList strHeaderList);
    TableModel * GetTablemodel(){
        return m_pModel;
    }
    void SetNoEditRow(int row);
public slots:
    void slotAddRow();
    void slotBatchAddRow();
    void slotRemove();
    void slotFindname(QString name);
signals:
    //选择行改变
       void signalSelectedRowChange(QString type, QString name);
    //表格数据修改
     void signalDataChanged(const QModelIndex &index,QStringList listvalue,QString strPreText);
    //表格删除
    void signalDeleteRows(QMap<QString, QList<QString>> typepkeymap);
    //属性改变
      void signalTypeAttriChanged(QStringList listvalue, QString strPreText);
private slots:
    void slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void slotdatachanged(const QModelIndex &index, const QVariant &value,QString);
    void slotTypeAttriChanged(int row, const QString &value, QString strPreText);


private:
    TableModel *m_pModel;
    QList<QStringList> m_variableList;
};

#endif // TABLEVIEW_H
