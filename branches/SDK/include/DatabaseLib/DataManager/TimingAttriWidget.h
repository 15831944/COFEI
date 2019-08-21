#ifndef TIMINGATTRIWIDGET_H
#define TIMINGATTRIWIDGET_H

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
/// \brief 计时类
///
class TimingAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimingAttriWidget(QWidget *parent = 0);

public:
    void SetData(VARDB_TIME_STRU stu);
signals:
     void signalAttriDataChanged(QString type, BASEDB_STRU *base);

private slots:
    void slotTimeAttriChanged();
private:
    bool CheckChar(QString srcstr);
private:
    QStringList m_baselist;
private:
    QCheckBox *pSaveToDBCheckbox;

    QLineEdit *pstartIOEdit;
    QLineEdit *pInitvalueEdit;
    QLineEdit *pSetvalueEdit;
    QLineEdit *pCurvalueEdit;
    QLineEdit *pEndIOEdit;
    QLabel *m_varname;
};

#endif // TIMINGATTRIWIDGET_H
