#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QTableView>
#include <QAbstractTableModel>
#include <QStringList>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QTableWidget>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = 0);

    //设置表头数据
    void setHeaderList(QStringList list);
    //设置表格数据
    void setModelDatas(QList<QStringList> *list);
    void SetNoEditRow(int row);

    // header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // data:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    void refrush();
     int m_nRow;

signals:
    void signalSetComboBox();
    // void signalComboBoxChanged(int row, const QString &value);
    void signalComboBoxChanged(int row, const QString &value, QString strPreText);//新修改
    void signalDataChanged(const QModelIndex &index, const QVariant &value,QString strPreText);

private:
    QStringList m_headerList;
    QList<QStringList>* m_varbriefList;
    //QList<QString> varname;

};




#endif // TABLEMODEL_H
