#include "my_talbview.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QApplication>
#include <QMessageBox>
My_Talbview::My_Talbview(QWidget* parent):
QSqlTableModel(parent)
{

}

QVariant My_Talbview::data(const QModelIndex &idx, int role) const
{
    if(role == Qt::TextAlignmentRole)
    {
        QVariant value = Qt::AlignCenter;
        return value;
    }
    else
        return QSqlTableModel::data(idx,role);
}
