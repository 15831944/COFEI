#ifndef USERLOGIN_H
#define USERLOGIN_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QSqlTableModel>
#include <QHorizontalBarSeries>
#include <QSqlQuery>
#include <lineedit.h>
#include <QFont>
#include <databasemanager.h>
#include <QKeyEvent>
class UserLogin : public QDialog
{
public:
    UserLogin();
    ~UserLogin();
     Lineedit *m_puser;
private:
    QPixmap m_PixmapChange;
    QLabel *m_puserlable;  //账号：
    QLabel *m_pPaswordlable;//密码：

    Lineedit *m_pPasword;
    QPushButton *m_plogin;//确认
    QPushButton *m_pcancel;//取消
    //DataBaseT<int32_t,mystruct> *m_pdata;
    //QVector<QString> Readata;
    QSqlTableModel *tabmodel;
    QSqlQuery query;
    QLabel *tips; //提示标签
    QLabel *user;//用户图片
    QLabel *pas;//密码图片
    char uc[10]{1,'a',5,'h',2,'k',9,'#','r','p'};
    QSqlDatabase db;
private:
    void InitUI();
    void login();
    void clearuser();
    void clearpas();
    bool changepasword(QString username,QString newpasword);
    QString AddCode(QString str);
    QString DeCode(QString str);
    void CheckLogin(QString,QString);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // USERLOGIN_H
