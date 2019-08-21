/************************************
* @brief 摘要 自定义表格
* @author sunny
* @date 2017/06/01
************************************/
#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include <QTableWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QStringList>
#include "checkdlg.h"
#include <Comm.h>
#include <QTreeWidget>
#include "axisrunpos.h"
class CustomTabWidget : public QTableWidget
{
    Q_OBJECT
public:
    CustomTabWidget(QStringList,QWidget *parent);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    QMenu* CreateContextMenu();
    void CreateTabHeader();
    bool CreateMessageBox(QString strTitle,QString note);
    void InsertData(QVector<QString>,QString str);
    void UpdateData(QList<QTableWidgetItem*> items,QVector<QString> vec,QString str);
    static bool compare(int a,int b)
    {
        return a > b;
    }

private:
    QStringList m_HeaderList;
    QWidget *m_pe;
    QMap<QString,SWCONFIGXML_QPOSIT_STRU> m_serQPtemp;
    QMap<QString,SWCONFIGXML_LCSP_STRU> m_serLCSPtemp;
    QMap<QString,SWCONFIGXML_QPOSIT_STRU> m_QPtemp ;
    QMap<QString,bool> m_test;
    void UpdataTabWidGet();
signals:
    void SendAxisSerConfig(QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
    void UpdateCalibrationUI(QVector<QString>,SWCONFIGXML_BASE_STRU*,QMap<QString,int>,const QString,const QString&);

public slots:
    void AddPoint();
    void DeletePoint();
    void UpdataPoint();
    void SavePoints();
    void UpdataSerTabWidGet(QMap<QString,SWCONFIGXML_QPOSIT_STRU> serMap1,
                            QMap<QString,SWCONFIGXML_LCSP_STRU> serMap2)
    {
        m_serQPtemp = serMap1;
        m_serLCSPtemp = serMap2;
        UpdataTabWidGet();
    }

    void onUserClickListener(QModelIndex);
    void ResizeClickListener(QTableWidgetItem * item);
    void customContextMenu(const QPoint& );
    void ItemExpanded(QTreeWidgetItem* item)
    {
        if(item->text(0) == "轴ID")
        {
            QTreeWidget * tw = (QTreeWidget*)this->cellWidget(item->toolTip(0).toInt(),6);
            this->resizeRowToContents(item->toolTip(0).toInt());
            tw->expandAll();
        }
    }
    void ItemCollapsed(QTreeWidgetItem* item)
    {
        if(item->text(0) == "轴ID")
        {
            QTreeWidget * tw = (QTreeWidget*)this->cellWidget(item->toolTip(0).toInt(),6);
            this->verticalHeader()->setDefaultSectionSize(25);
            tw->collapseAll();
        }
    }

    void checkself(QTreeWidgetItem* item,int cl)
    {
        QTreeWidget * tw = (QTreeWidget*)this->cellWidget(item->toolTip(cl).toInt(),6);
        if(item->text(cl) == "轴ID")
        {
            bool isok = true;
            if(!m_test.contains(item->toolTip(cl)))
            {
                m_test.insert(item->toolTip(cl),isok);
                this->resizeRowToContents(item->toolTip(cl).toInt());

                tw->expandAll();
            }
            else
            {
                if(m_test[item->toolTip(cl)])
                {
                    this->verticalHeader()->setDefaultSectionSize(25);
                    m_test[item->toolTip(cl)] = false;
                }
                else
                {
                    this->resizeRowToContents(item->toolTip(cl).toInt());
                    m_test[item->toolTip(cl)] = true;
                    tw->expandAll();
                }
            }
        }
        return;
    }
private:
    QTimer *m_pTimer;
    QMap<QString,AxisRunPos *> m_RunMap;
    QMap<QString,PROTECT_EVENT_STRU> axisEventMap;
    QMap<QString,QPushButton*> m_runbtnMap;
    QMap<QString,QPushButton*> m_stopbtnMap;
    QString GetStrID(QPushButton *senderObj);
    bool JudgeAsixEvent(QString);
    void DelRunData(QString);//delete
    void ModPRunData(QString strId);//modif
private slots:
    void Run();
    void stop();
    void timerUpInputData();
public slots:
    void SetBtnStatus(bool enable);
    void reject();
signals:
    void DiableQPBtn(bool);
private:
    void Modtablerowstyle(int);
    void Deltablerowstyle();
    QString m_addID;
    bool m_reject;

};

#endif // CUSTOMLISTVIEW_H
