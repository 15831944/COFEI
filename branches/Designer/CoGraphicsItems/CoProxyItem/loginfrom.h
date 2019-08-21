#ifndef LOGINFROM_H
#define LOGINFROM_H

#include <QObject>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <CoGraphics/Scene/Scene.h>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QDebug>

class LoginFrom : public CoProxyWidget
{
public:
    LoginFrom(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
              const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
              const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black,
              CoScene* Scene = NULL);
private:
    QWidget *widget;
    QComboBox *m_pUserName;
    QLineEdit *m_pPassWord;
    QPushButton *m_pLoginBtn;
    QLabel *m_pForget;
    QGraphicsGridLayout *m_pLayout;
    QGridLayout *m_pgridLayout;
    QLabel *errlab ;
    QPushButton *m_pExitBtn;
    void keyPressEvent(QKeyEvent *event);
    void InsertErrInfo();
public:
    void Login();
    void Logout();
public slots:
    CoProxyWidget *Copy();
    void Copy(LoginFrom *loginfrom);
};

#endif // LOGINFROM_H
