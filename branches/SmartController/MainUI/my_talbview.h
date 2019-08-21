#ifndef MY_TALBVIEW_H
#define MY_TALBVIEW_H
#include <QSqlTableModel>

class My_Talbview : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit My_Talbview(QWidget *parent = 0);
    QVariant data(const QModelIndex &idx, int role) const;
};

#endif // MY_TALBVIEW_H
