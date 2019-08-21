#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <Comm.h>
#include <QMetaType>
#include <DycLibBase/LogManager/logmanager.h>
#include <MyEC_Shared.h>
#include <MyConfig_Shared>
#include <QMessageBox>
#include <DycLibBase/CSingleApplication/csingleapplication.h>
#include <DycLibBase/SecretManager/secretmanager.h>
#include <QStandardPaths>
#include <QPixmap>
#include <QSplashScreen>
#include <QMovie>
#include <QDebug>
#include <DynamicLib/LoadFileManager/LoadFileManager.h>
#include "login.h"

int main(int argc, char *argv[])
{
    CSingleApplication a(argc, argv,"COFEI R&D");
    if(a.isRunning())
    {
        return 0;
    }
    SecretManager secretManager;
    if(!secretManager.CheckKey())
    {
        LogIn login;
        if(QDialog::Rejected == login.exec())
        {
            QString strTemp = QString("当前的系统未进行注册，请先联系客服进行注册。");
            QMessageBox::critical(NULL, "Title", strTemp, QMessageBox::Yes);
            return 0;
        }
    }
    QString pathUsr = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    /***/
    //qRegisterMetaType<QMap<QString,QVector<UIXML_STRU>>>("QMap<QString,QVector<UIXML_STRU>>");
    //qRegisterMetaType<QMap<QString,QMap<QString,SWCONFIGXML_QPOSIT_STRU>>>("QMap<QString,QMap<QString,SWCONFIGXML_QPOSIT_STRU>>");
    //qRegisterMetaType<QMap<QString,QMap<QString,SWCONFIGXML_LCSP_STRU>>>("QMap<QString,QMap<QString,SWCONFIGXML_LCSP_STRU>>");
    QString logPath = pathUsr + "/logs";
//    {
//        QProcess *poclog = new QProcess;
//        QString str = "sudo chattr -R -ai " + logPath;
//        poclog->start(str);
//    }
//    QString dataPath = pathUsr + "/Data";
//    {
//        QProcess *pocdata = new QProcess;
//        QString str = "sudo chattr -R -ai " + dataPath;
//        pocdata->start(str);
//    }
    QDir dir(logPath);
    if (!dir.exists())
        dir.mkdir(logPath);
    QString logPath1 = logPath + "/" + QString(defaultLogModule) +".log";
    QString logPath2 = logPath + "/" + QString(CTLogModule) +".log";
    CoreLog::QLoggerManager::getInstance()->addLogInstance(logPath1,defaultLogModule,CoreLog::InfoLevel);
    CoreLog::QLoggerManager::getInstance()->addLogInstance(logPath2,CTLogModule,CoreLog::InfoLevel);
    QFile styleSheet(":/qss/checkbox");
    styleSheet.open(QIODevice::ReadOnly);
    a.setStyleSheet(styleSheet.readAll());

    QPixmap pix(":/images/balboa");
    QSplashScreen splash(pix);
    splash.setFixedSize(500,357);
    splash.setWindowOpacity(0.9);// 设置窗口透明度
    QLabel label(&splash);
    QString path = QApplication::applicationDirPath() + "/login.gif";
    QMovie mv(path);
    label.setMovie(&mv);
    mv.start();
    splash.show();
    splash.setCursor(Qt::BlankCursor);
    QFont ft;
    ft.setPointSize(12);
    splash.setFont(ft);
    LoadFileManager::GetInstance()->InitConfig(true);
    for(int i = 0; i < 5000; i += mv.speed()/5)
    {
        splash.showMessage(QObject::tr("Load plugins and Analysis XML,Please wait a moment..."),Qt::AlignLeft | Qt::AlignBottom, Qt::blue);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        QThread::msleep(mv.speed()/5);
    }
    MainWindow w;
    QObject::connect(&a, SIGNAL(messageAvailable(QString)), &w, SLOT(receiveMessage(QString)));
    w.show();
    splash.close();
    return a.exec();
}
