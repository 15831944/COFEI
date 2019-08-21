#ifndef PRESCRIPTTABWIDGET_H
#define PRESCRIPTTABWIDGET_H
#include "cbasetabwidget.h"
#include <QSqlTableModel>
class PrescriptTabWidget : public CBaseTabWidget
{
public:
    PrescriptTabWidget(QStringList, QWidget *);
private:
    QSqlTableModel *tabmodel;
protected slots:
    void DeletePoint();
    void UpdataPoint();
    void SavePoints();
};

#endif // PRESCRIPTTABWIDGET_H
