#ifndef CBASETABWIDGET_H
#define CBASETABWIDGET_H
#include <QTableWidget>
#include <QStringList>
#include <QContextMenuEvent>
#include <QMenu>
class CBaseTabWidget : public QTableWidget
{
    Q_OBJECT
public:
    CBaseTabWidget(QStringList, QWidget *);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
protected:
    virtual QMenu* CreateContextMenu(bool isempty = false);
    void CreateTabHeader(const QStringList);
    virtual bool CreateMessageBox(QString strTitle,QString note);
private:
    QWidget * m_pe;
protected slots:
    virtual void AddPoint();
    virtual void DeletePoint();
    virtual void UpdataPoint();
    virtual void SavePoints();
};

#endif // CBASETABWIDGET_H
