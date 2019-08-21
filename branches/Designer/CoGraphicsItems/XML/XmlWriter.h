#ifndef XML_WRITER_H
#define XML_WRITER_H

#include <CoGraphics/Graphics.h>

#include <QtXml/QDomDocument>
#include <QApplication>
#include <CoGraphics/Scene/Scene.h>
class TXmlWriter
{
    Q_DECLARE_TR_FUNCTIONS(TXmlWriter)

public:
    TXmlWriter();
    ~TXmlWriter();
    void AddHead();
    QDomElement AddRoot(const QString Root);
    QDomElement AddChild(QDomElement &Parent, const QString Name);
    bool AddItem(QDomElement &DomParent, CoItem *Item);
    bool AddArrow(QDomElement &DomParent, CoArrow *Arrow);
    bool AddWidget(QDomElement &DomParent, CoProxyWidget *PWidget);
    bool AddItems(QDomElement &DomParent, CoScene *Scene);
    bool Save(QString FileName);
    QString CreateJson(const QMap<QString, SENSOR_LIGHT_STRU> &);

private:
    QDomDocument m_Doc;
    QString TransExpreSymbol(const QString &Name);
    bool InsertText(QDomElement &XmlItem, CoItem *Item);
    bool InsertLabel(QDomElement &XmlItem, CoItem *Item);
    bool InsertInstLamp(QDomElement &XmlItem, CoItem *Item);
    bool InsertWarnLamp(QDomElement &XmlItem, CoItem *Item);
    bool InsertButton(QDomElement &XmlItem, CoItem *Item);
    bool InsertCalButton(QDomElement &XmlItem, CoItem *Item);
    bool InsertPixmap(QDomElement &XmlItem, CoItem *Item);
    bool InsertGroupbox(QDomElement &XmlItem, CoItem *Item);
    bool InsertCurve(QDomElement &XmlItem, CoItem *Item);
    bool InsertSafeState(QDomElement &XmlItem, CoItem *Item);
    bool InsertDebug(QDomElement &XmlItem, CoProxyWidget *Item);
    bool InsertSafeLevel(QDomElement &XmlItem, CoProxyWidget *Item);
    bool InsertArrow(QDomElement &DomParent, CoArrow *Item);
    bool InsertGPInput(QDomElement &XmlItem, CoItem *Item);
    bool InsertGPOutPut(QDomElement &XmlItem, CoItem *Item);
};

#endif // XML_WRITER_H
