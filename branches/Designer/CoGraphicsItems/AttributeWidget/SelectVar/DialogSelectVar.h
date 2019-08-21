#ifndef DIALOGSELECTVAR_H
#define DIALOGSELECTVAR_H

#include <QTableWidget>
#include "CoGraphics/Base/Base.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
class DialogSelectVar : public QDialog
{
    Q_OBJECT
public:
    DialogSelectVar(QWidget *parent = 0);
    ~DialogSelectVar(){}

signals:
    void SendVar(const QString name,const QString type);

public:
    void SetSelectVarType(QString type);
    void SetIsRealtimeVar(bool visible);
    void IsHWSwitch(bool b);

private slots:
    void SendVarName();
    void TableSelectChanged();
    void QueryVar();
    void QueryRealTimeVar();
    void DoSearch();

private:
    void InitLayout();
    void ResetTableByData(QList<QList<QString> > &dataList);
    int GetVarTypeByName(const QString& varName);

private:
    QLineEdit       *m_pLineEditVarName;
    QCheckBox       *m_pChkNumber;
    QCheckBox       *m_pChkSwitch;
    QCheckBox       *m_pChkString;
    QCheckBox       *m_pChkTime;
    QCheckBox       *m_pChkCount;
    QCheckBox       *m_pChkRealTime;
    QPushButton     *m_pBtnOK;
    QPushButton     *m_pBtnQuit;
    QTableWidget    *m_pVarTable;
    QLineEdit       *m_pSearchEdit;
    bool            m_bIsHWSwitch;
};

#endif // DIALOGSELECTVAR_H
