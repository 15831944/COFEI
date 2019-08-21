#ifndef KOROLDELEGATE_H
#define KOROLDELEGATE_H
#include <QStyledItemDelegate>
#include <QObject>

class KorolDelegate : public  QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KorolDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // KOROLDELEGATE_H
