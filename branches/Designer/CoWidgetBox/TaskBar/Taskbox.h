#ifndef COTASKBOX_H
#define COTASKBOX_H

#include <QtGui>
#include <QFrame>
#include <QBoxLayout>

#include <CoBase/CoAction/Action.h>
class CoTaskHeader;
class CoTaskGroup;
class CoTaskBarScheme;
class CoIconLabel;

class CoTaskBox : public QFrame
{
    Q_OBJECT

public:
    CoTaskBox(const QString &title, bool expandable = true, QWidget *parent = 0);
    CoTaskBox(const QPixmap &icon, const QString &title, bool expandable = true, QWidget *parent = 0);
    virtual ~CoTaskBox();

    void setScheme(CoTaskBarScheme *pointer);

    QBoxLayout* groupLayout();

    void addIconLabel(CoIconLabel *label, bool addToLayout = true);
    void addAction(CoAction *action, bool addToLayout = true);
    QString GetTitle() const;
public slots:
    void showHide();

protected slots:
    void processHide();
    void processShow();

protected:
    void init();

    virtual void paintEvent ( QPaintEvent * event );


    double m_foldStep, m_foldDelta, m_fullHeight, m_tempHeight;
    int m_foldDirection;

    QPixmap m_foldPixmap;

    CoTaskHeader *myHeader;
    CoTaskGroup *myGroup;
    QWidget *myDummy;
    QString m_title;
    CoTaskBarScheme *myScheme;
};

#endif // COTASKBOX_H
