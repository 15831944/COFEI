#ifndef CYLINDERINFOATTRIWIDGET_H
#define CYLINDERINFOATTRIWIDGET_H

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
#include <QTextEdit>
///
/// \brief 气缸类
///
class CylinderInfoAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CylinderInfoAttriWidget(QWidget *parent = 0);

public:
    //void SetData(VARALLTYPEDBCONFIG_STRU stru);
    void SetData(VARDB_CY_STRU stru);
private:
    QLineEdit *m_soltline;
    QLineEdit *m_baseLine;
    QLineEdit *m_moveLine;
    QLineEdit *m_delayLine;
    QLineEdit *m_timoutLine;
    QTextEdit *m_infotext;
    QCheckBox  *m_pSaveToDBCheckbox;
    QLabel *m_Varname;
    QPushButton *m_save;
private:
    void CreateCyAttriUI();
private:
    bool CheckChar(QString srcstr,QChar keychar);
signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);

private slots:
    void slotCylinderAttriChanged();
private:
    QStringList m_baselist;
signals:

public slots:
};

#endif // CYLINDERINFOATTRIWIDGET_H
