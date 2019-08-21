#ifndef NUMBERREGISTERATTRIWIDGET_H
#define NUMBERREGISTERATTRIWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <DatabaseLib/variable.h>
#include <QSpinBox>
///
/// \brief 数值型寄存器
///
class NumberRegisterAttriWidget : public QWidget
{
    Q_OBJECT
public:
    NumberRegisterAttriWidget(QWidget *parent = 0);

public:
    void SetData(VARDB_REGNUM_STRU stu);

signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);

private slots:
    void slotCountAttriChanged();

private:

    QStringList m_baselist;
private:
    QCheckBox *pSaveToDBCheckbox;
    QLineEdit *pchildtypeEdit;
    QSpinBox *pspointnumpinbox;
    QLineEdit *pInitvalueEdit;
    QLineEdit *pupvalueEdit;
    QLineEdit *plowvalueEdit;
    QLabel *m_Varname;

};
#endif // NUMBERREGISTERATTRIWIDGET_H
