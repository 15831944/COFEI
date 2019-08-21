#ifndef STRINGREGISTERATTRIWIDGET_H
#define STRINGREGISTERATTRIWIDGET_H

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

///
/// \brief 字符型寄存器
///
class StringRegisterAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StringRegisterAttriWidget(QWidget *parent = 0);

public:
    void SetData(VARDB_REGTEXT_STRU);

signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);
private slots:
    void slotstringAttriChanged();
private:
    QStringList m_baselist;

private:
    QCheckBox *pSaveToDBCheckbox;
    QLineEdit *pchildtypeEdit;
    QLabel *m_varname;
    QLineEdit *ptextvalEdit;
};

#endif // STRINGREGISTERATTRIWIDGET_H
