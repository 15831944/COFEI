#ifndef MOTIONCTRATTRIWIDGET_H
#define MOTIONCTRATTRIWIDGET_H

#include <QObject>
#include <QWidget>
#include <DatabaseLib/variable.h>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMutex>
#include <QTableWidget>
///
/// \brief The MotionCtrAttriWidget class
///运动控制类pp csp
class MotionCtrAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MotionCtrAttriWidget(QWidget *parent = 0);
    ~MotionCtrAttriWidget();
    void SetData(VARDB_PPMOTCTR_STRU stru);
private:
    QLineEdit *m_delayline;
    QLineEdit *m_timoutline;
    QLineEdit *m_accline;
    QLineEdit *m_deccline;
    QCheckBox *m_savetoDb;
    QLabel *m_namelabel;
    QTableWidget *m_tableWg;
private:
    void createUI();
    QString GetAxisInfoString();
    void GetAxisandRegNumNames(QStringList &axisNames,QStringList &regnumNames);
    void SetAxisInfoTable(QString axisInfo);
signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);
private slots:
    void slotMotionCtrAttriChanged();
    void slotCheckAxisInfo(QTableWidgetItem *current);
private:
    QStringList m_baselist;

public slots:
};

#endif // MOTIONCTRATTRIWIDGET_H
