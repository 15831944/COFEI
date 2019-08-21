#include "ComboBoxDelegate.h"
#include <QMessageBox>
#include <QDebug>
#include <QCompleter>

/********************ComboBoxDelegate********************/
ComboBoxDelegate::ComboBoxDelegate(QStringList list, QObject *parent) : QItemDelegate(parent)
{
    m_itemList = list;
    m_changeinfo = "";//tr("您在修改变量属性类型,修改后该变量的属性值将被清空,是否继续?")
}

/// \brief 双击时，创建控件
QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *editor = new QComboBox(parent);
    QCompleter *pCompleter = new QCompleter(editor->model());
    pCompleter->setFilterMode(Qt::MatchContains);
    editor->setEditable(true);
    editor->setCompleter(pCompleter);
    editor->setMaxVisibleItems(6);
    return editor;
}

/// \brief 设置QComboBox当前editor的数据
void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->addItems(m_itemList);
    comboBox->setCurrentText(index.data().toString());
}

/// \brief 设置QTableView的model的数据
void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString strPreText = model->data(index).toString();
    if(m_changeinfo != "")
    { if(!strPreText.isEmpty())
        {
            QMessageBox mb(tr("警告"),
                           m_changeinfo,
                           QMessageBox::Question,
                           QMessageBox::Yes ,
                           QMessageBox::No | QMessageBox::Escape | QMessageBox::Default,
                           QMessageBox::NoButton);
            if (mb.exec() == QMessageBox::No)
            {
                return;
            }
        }
    }
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    if (!m_itemList.contains(text))
    {
        QMessageBox::information(NULL,tr("警告"),tr("数据输入错误,请确保数据正确!"));
        return;
    }
    model->setData(index, text, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

