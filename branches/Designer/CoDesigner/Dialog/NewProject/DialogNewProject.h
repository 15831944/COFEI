#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <CoGraphics/Base/Base.h>

class DialogNewProject : public QDialog
{
    Q_OBJECT
public:
    DialogNewProject(QWidget *parent = 0);
    ~DialogNewProject();
    bool mSave;
    QString GetName() const;
    ushort GetWidth() const;
    ushort GetHeight() const;
    void SetWidth(ushort ut) ;
    void SetHeight(ushort ut) ;
    void SetNameList(const QList<QString> &list) ;
private slots:
    void Apply();
private:
    QLineEdit *m_LineEdit;
    QLineEdit * m_EditX;
    QLineEdit * m_EditY;
    QLabel *m_LabelNoteTip;
    QList<QString> m_NameList;
public slots:
   bool eventFilter(QObject *,QEvent *);
};

#endif // DIALOGNEWPROJECT_H
