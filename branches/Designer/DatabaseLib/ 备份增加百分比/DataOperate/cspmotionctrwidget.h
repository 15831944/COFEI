#ifndef CSPMOTIONCTRWIDGET_H
#define CSPMOTIONCTRWIDGET_H
#include <QObject>
#include <QWidget>
#include <DatabaseLib/variable.h>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>

class cspMotionctrWidget: public QWidget
{    Q_OBJECT
 public:
     explicit cspMotionctrWidget(QWidget *parent = 0);
     ~cspMotionctrWidget();
     void SetData(VARDB_LCSP_STRU stru);
 private:
    // QComboBox *m_modcombox;
     QLineEdit *m_axisid;
     QLineEdit *m_delayline;
     QLineEdit *m_timoutline;
     QLineEdit *m_accline;
     QLineEdit *m_deccline;
     QCheckBox *m_savetoDb;
     QLabel *m_namelabel;
 private:
     void createUI(QWidget *parent);
 signals:
       void signalAttriDataChanged(QString type, BASEDB_STRU *base);
 private slots:
     void slotMotionCtrAttriChanged();
 private:
    // VARDB_LCSP_STRU *m_struVarValue;

};

#endif // CSPMOTIONCTRWIDGET_H
