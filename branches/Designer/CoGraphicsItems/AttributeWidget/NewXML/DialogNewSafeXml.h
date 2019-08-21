#ifndef DIALOGNEWSAFEXML_H
#define DIALOGNEWSAFEXML_H

#include <QWidget>
#include <CoGraphics/Base/Base.h>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "xmlsyntaxhighlighter.h"
class DialogNewSafeXml : public QDialog
{
    Q_OBJECT
public:
    DialogNewSafeXml(QWidget *parent = 0);
    ~DialogNewSafeXml(){}

public:
    void SetText(const QString&text);

signals:
    void SafeStringChanged(const QString text);

private:
    void InitFuncTree();
    QString GetTextByItem(QTreeWidgetItem *);

private slots:
    void SendJsString();
    void AddFunc(QTreeWidgetItem *,int);

private:
    XmlSyntaxHighlighter *m_pHightLighter;
    QTextEdit   *m_pXMLEdit;
    QLabel      *m_pLblError;
    QPushButton *m_pBtnOK;
    QTreeWidget *m_pFuncTree;
};

#endif // DIALOGNEWSAFEXML_H
