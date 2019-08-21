#ifndef HANDLEUIXML_H
#define HANDLEUIXML_H
#include <Comm.h>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMap>
#include <QVector>
#include <QSize>
class HandleUIXml
{
public:
    HandleUIXml();
public:
    QString ReadUIXml(QString strPath,UIXML_Map &uiXml);
    QString WriteUIXml(QString strPath);
private:
    void ReadControls(QXmlStreamReader &reader,UIXML_STRU &uiconVec);
    void ReadUIType(QXmlStreamReader &reader,QString strElement,QMap<QString,QVector<UIXML_STRU>> &uixmlMap);
};

#endif // HANDLEUIXML_H
