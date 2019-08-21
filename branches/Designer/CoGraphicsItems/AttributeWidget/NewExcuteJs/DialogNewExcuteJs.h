#ifndef DIALOGNEWEXCUTEJS_H
#define DIALOGNEWEXCUTEJS_H

#include <CoGraphics/Base/Base.h>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class DialogNewExcuteJs : public QDialog
{
    Q_OBJECT
public:
    DialogNewExcuteJs(QWidget *parent = 0);
    ~DialogNewExcuteJs(){}

signals:
    void signalSendNewJs(const QString text);

private:
    void InitFuncTree();
    QString GetTextByItem(QTreeWidgetItem *);

private slots:
    void ImpotJs();
    void SendJsString();
    void AddFunc(QTreeWidgetItem *,int);

private:
    QTextEdit   *m_pTextEditJsString;
    QLabel      *m_pLblError;
    QPushButton *m_pBtnImport;
    QPushButton *m_pBtnOK;
    QTreeWidget *m_pFuncTree;
};

#endif // DIALOGNEWEXCUTEJS_H
