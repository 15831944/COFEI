#include "koroldelegate.h"
#include <QDebug>
#include <QLineEdit>
KorolDelegate::KorolDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

QWidget *KorolDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QRegExp reg("[1-4]");
    QLineEdit *edit = new QLineEdit(parent);
    edit->setValidator(new QRegExpValidator(reg));
    return edit;
}

void KorolDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(value);
}

void KorolDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    QString text = edit->text();
    model->setData(index,text);
}

void KorolDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
