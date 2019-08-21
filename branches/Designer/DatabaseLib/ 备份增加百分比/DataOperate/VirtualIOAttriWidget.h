#ifndef VIRTUALIOATTRIWIDGET_H
#define VIRTUALIOATTRIWIDGET_H

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
/// \brief 虚拟IO
///
class VirtualIOAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualIOAttriWidget(QWidget *parent = 0);

public:
    void SetData(VARDB_SWITCH_STRU stu);

signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);

private slots:
    void slotVirtualIOAttriChanged();
    void slotshowhwIO(bool value);
private:
    bool CheckIOAddr();

private:
    QStringList m_baselist;

private:
    QCheckBox *pSaveToDBCheckbox;
    QCheckBox *m_ishwioCheckbox;
    QLineEdit *m_IOaddrEdit;
    QLineEdit *pinitvalueEdit;
    QLabel *m_varname;

};

#endif // VIRTUALIOATTRIWIDGET_H
