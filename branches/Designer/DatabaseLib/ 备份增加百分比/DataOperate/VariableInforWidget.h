#ifndef VARIABLEINFORWIDGET_H
#define VARIABLEINFORWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include "TableView.h"
#include <DatabaseLib/variable.h>
#include <QLineEdit>
//#include "databasesharedconfig.h"

class VariableInforWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VariableInforWidget(QWidget *parent = 0);
    ~VariableInforWidget();

public:
    TableView *GetTableView()
    {
        return m_pTableView;
    }
private:
    QLineEdit *m_nameline;
public slots:
    //变量属性修改
    void slotVarAttriDataChanged(QString type,BASEDB_STRU *base);
    void slotSelectedRowChanged(QString type,QString name);

signals:
    void signalShowAttriWidget(QString type, BASEDB_STRU *base);
    void signalFindName(QString name);
private slots:
    //提交到数据库
    void slotCommit();
    //新增一行
    void slotAddRow();
    void  slotBatchAddRow();
    void slotDeleteRows(QMap<QString, QList<QString>> typepkeymap);
    //刷新
    void slotRefreh();
    void slotAttriTypeChanged(QStringList listvalue, QString strPreText);
    //表格数据修改
    void slotTableViewDataChanged(const QModelIndex &index,QStringList listvalue,QString strPreText);
    //查找变量
    void slotfindname();
private:
    void GetDataFromDB();
    void GetTablelist(QList<QStringList> &tableList);
   void ChangeAttriOperateData(QStringList listvalue, QString strPreText);
 template<typename T>
  void GetVarDataFromRAM(QMap<QString,T> VarMap,QList<QStringList> &tableList ){
      for(auto it = VarMap.begin(); it != VarMap.end();++it)
      {
          QStringList variableList;
          variableList.clear();
          if(it.key().contains("BUILT_IN_"))
          {
              continue;
          }
          variableList.append(it.value().pkey);
          variableList.append(it.value().type);
          variableList.append(it.value().note);
          variableList.append(it.value().fkey);
          tableList.append(variableList);
      }
  }
private:
    TableView *m_pTableView;
    QPushButton *m_pCommitBtn;
    QMap<QString,VARDB_COUNT_STRU> m_ModVarcountMap;
    QMap<QString,VARDB_TIME_STRU> m_ModVartimeMap;
    QMap<QString,VARDB_REGTEXT_STRU> m_ModVarregtextMap;
    QMap<QString,VARDB_CY_STRU> m_ModVarcyMap;
    QMap<QString,VARDB_REGNUM_STRU> m_ModVarregnumMap;
    QMap<QString,VARDB_PPMOTCTR_STRU> m_ModVarqpMap;
    QMap<QString,VARDB_LCSP_STRU> m_ModvarcspMap;
    QMap<QString,VARDB_SWITCH_STRU> m_ModVarswMap;
    QList<QVariant> m_DelpkeyList;
    VARCONFIG_STRU m_DBVarstru;
};

#endif // VariableInforWidget_H
