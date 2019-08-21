#ifndef COUNTATTRIWIDGET_H
#define COUNTATTRIWIDGET_H

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
#include <QComboBox>

///
/// \brief 计数属性窗
///
class CountAttriWidget  : public QWidget
{
    Q_OBJECT
public:
    CountAttriWidget();
//    ~CountAttriWidget(){}

public:
    void SetData(VARDB_COUNT_STRU stu);

signals:
     void signalAttriDataChanged(QString type, BASEDB_STRU *base);
private slots:
    void slotCountAttriChanged();
private:
    QStringList m_baselist;
private:
    bool CheckChar(QString srcstr);
private:
    QLineEdit *pstartIOEdit;
    QLineEdit *pInitvalueEdit;
    QLineEdit *pSetvalueEdit;
    QLineEdit *pCurvalueEdit;
    QLineEdit *pAddvalueEdit;
    QLineEdit *pEndIOEdit;
    QCheckBox *pSaveToDBCheckbox;
    QLabel *m_Varname;
    QComboBox *m_funtype;
};

#endif // COUNTATTRIWIDGET_H
