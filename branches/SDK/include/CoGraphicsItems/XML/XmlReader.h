#ifndef XML_READER_H
#define XML_READER_H

#include <CoGraphics/Graphics.h>
#include <CoGraphics/Scene/Scene.h>
#include <QtXml/QDomDocument>
#include <QApplication>
#include "../FlowPropery/itemsave.h"
#include <Comm.h>
typedef struct _FLOW_STRU
{
    QMap<QString,CoScene*> SceneMap;
    _FLOW_STRU()
    {
        SceneMap.clear();
    }

}FLOW_STRU;
class TXmlReader
{
    Q_DECLARE_TR_FUNCTIONS(TXmlWriter)
public:
    TXmlReader();
    ~TXmlReader();


    bool Open(const QString &FileName);
    bool IsOpen();
    QDomElement GetRoot();

    CoScene* AnalyScene(QMap<QString, CoScene *> &SceneMap, QMap<QString, FLOW_STRU> &FlowMap,bool &isGridLine);
    int AnalyViewRotate();
    void AnalyFlowChart(const QString &);

    CoItem* AnalyItem(QDomElement mXmlNode);
    CoProxyWidget* AnalyWidget(QDomElement mXmlNode,CoScene*);
    void AnalySafeState(const QString &,QMap<QString,SWCONFIGXML_SAFESTATE_STRU>&);
    void AnalySigLevel(const QString &,QMap<QString,SENSOR_LIGHT_STRU>&);
private:
    QDomElement FindElement(QString Name);
    QString ReTransExpreSymbol(const QString &Name);
    CoScene* AnalyFlow(QDomElement mXmlScene, bool isGridLine = true,QString flowName = "");
    QDomDocument mDoc;
    bool m_IsOpen;
    QMap<QString, QMap<QString,QMap<int, TFlowBase *> > > m_flowItems;
};

#endif // XML_READER_H
