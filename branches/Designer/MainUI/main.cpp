#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <QPixmap>
#include <QLabel>
#include <QSplashScreen>
#include <QMovie>
#include <DynamicLib/LoadFileManager/LoadFileManager.h>
#include <DycLibBase/LogManager/logmanager.h>
#include <BaseDefine.h>
#include <csingleapplication.h>
bool DEF_RUN_AS_DESIGNER = false;
int main(int argc, char *argv[])
{
   // QApplication a(argc, argv);
    CSingleApplication a(argc, argv,"COFEI R&D");
    if(a.isRunning())
    {
        return 0;
    }
    DEF_RUN_AS_DESIGNER = false;
    QString pathUsr = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    QString logPath = pathUsr + "/logs";
    QDir dir(logPath);
    if (!dir.exists())
        dir.mkdir(logPath);
    QString logPath1 = logPath + "/" + QString(defaultLogModule) +".log";
    CoreLog::QLoggerManager::getInstance()->addLogInstance(logPath1,defaultLogModule,CoreLog::InfoLevel);

    QPixmap pix;
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
    LoadFileManager::GetInstance()->InitConfig(false);
    for(int i = 0; i < 5000; i += mv.speed()/5)
    {
        splash.showMessage(QObject::tr("Load plugins and Analysis XML,Please wait a moment..."),Qt::AlignLeft | Qt::AlignBottom, Qt::blue);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        QThread::msleep(mv.speed()/5);
    }
    MainWindow w;
    w.show();
    splash.close();
    return a.exec();
}
