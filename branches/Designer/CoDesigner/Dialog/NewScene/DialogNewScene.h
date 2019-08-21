#ifndef DIALOG_NEW_SCENE_H
#define DIALOG_NEW_SCENE_H

#include <CoGraphics/Base/Base.h>
class DialogNewScene : public QDialog
{
    Q_OBJECT

public:
    DialogNewScene(QWidget *parent = 0);
    ~DialogNewScene();

    bool    mSave;
    void    SetTitle(const QString &Title);
    bool    SetName(const QString &Name);
    QString GetName() const;
    void SetNameList(const QList<QString> &list) ;
//    ushort GetWidth() const;
//    ushort GetHeight() const;
    bool    SetBackColor(const QColor &Color);
    QColor  GetBackColor() const;

private slots:
    void    ChooseBackColor();
    void    Apply();

private:
    QLineEdit * m_LineEdit;
    QLineEdit * m_EditX;
    QLineEdit * m_EditY;
    QLabel *    m_LabelNoteTip;
    QLabel *    m_LabelBackColor;

    QColor m_BackColor;
    QList<QString> m_NameList;
public slots:
    bool eventFilter(QObject *,QEvent *);
};

#endif // DIALOG_NEW_SCENE_H
