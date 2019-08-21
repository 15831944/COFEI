#ifndef CSPTABLEWIDGET_H
#define CSPTABLEWIDGET_H
#include <QVector>
#include<QWidget>
#include <QTableWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPoint>
#include <QStringList>
#include <QString>
#include <QMap>
#include <Comm.h>
#include <QObject>
#include <addtabel.h>
class CSPTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    CSPTableWidget(QWidget *parent, QVector<QString> axisidstr, QVector<QMap<QString,double>> vec);
private:
    QMenu *CreateContextMenu();
    QMenu *menu ;
    QVector<QString> idstr;
    QVector<QMap<QString,double>> CSPvec;
    QVector<QMap<QString,double>> m_updatetabledata;
    QString id;
    QAction *m_setTr;
    QAction *m_clearTr;
    QAction *m_add ;
    Addtabel *m_Dilog;
    QStringList m_list;
signals:
    void TriInfo(int);
    void ClearTri(int);
    void SavePosTableRow();
    void DeletePosTableRow();
    void updatedata(QVector<QMap<QString,double>>,QVector<QString>);

private:
    void NewCSPIdCreateUI();
    void CSPIdCreateUI();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void AddPoint();
    void DeletePoint();
    void SetTrigger();
    void ClearTrigger();
    void Datastr(QStringList);
    void UpdatePoint();
    void disable();

};

#endif // CSPTABLEWIDGET_H
