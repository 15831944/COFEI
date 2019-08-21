/************************************
* @brief 摘要 注册界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPixmap>
#include <QLineEdit>
#include <QStatusBar>
#include <databasemanager.h>
class LogIn : public QDialog
{
    Q_OBJECT
public:
    LogIn();
private:
    QPixmap m_PixmapChange;
    QLineEdit *m_pPollCode;
    QLineEdit *m_pMechineCode;
    QStatusBar *m_pStatus;
private:
    void InitUI();
    void CreateMeCode();

signals:

public slots:
    void OverAccept();
};

#endif // LOGIN_H
