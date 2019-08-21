#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <BaseDefine.h>
#include <DycLibBase/LogManager/logmanager.h>
#include <QTranslator>
bool DEF_RUN_AS_DESIGNER = false;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString pathUsr = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    QString logPath = pathUsr + "/logs";
    QDir dir(logPath);
    if (!dir.exists())
        dir.mkdir(logPath);
    QString logPath1 = logPath + "/" + QString(defaultLogModule) +".log";
    CoreLog::QLoggerManager::getInstance()->addLogInstance(logPath1,defaultLogModule,CoreLog::InfoLevel);
    QFile file(":/images/qss/base.qss");
    if(file.open(QFile::ReadOnly))
    {
        a.setStyleSheet(file.readAll());
    }
    file.close();
    DEF_RUN_AS_DESIGNER = true;

    QTranslator qtTranslator;
    qtTranslator.load(":/images/qrc/qt_zh_CN.qm.qm");
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
