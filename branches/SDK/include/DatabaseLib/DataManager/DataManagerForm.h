#ifndef DataManagerForm_H
#define DataManagerForm_H

#include <QWidget>
#include <QScrollArea>
#include "datamanagerform_global.h"
#include "VariableInforWidget.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

class DATAMANAGERFORMSHARED_EXPORT DataManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataManagerForm(QWidget *parent = 0);
    ~DataManagerForm();
public:
    QWidget *GetAttriArea();
private:
    void CreateDataUI();
private slots:
     void slotShowAttriWidget(QString type, BASEDB_STRU *base);
private:
     QTabWidget *m_datatabletabwg;
     QScrollArea *m_pAttriArea;
     VariableInforWidget *m_pVariableWidget;
    int m_index;
signals:
    void changeattriWgsig(QString);
public slots:
   void ChangetableAttrWgslot();

};

#endif // DataManagerForm_H
