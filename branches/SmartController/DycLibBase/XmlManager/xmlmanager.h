#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include "xmlmanager_global.h"
#include "handelflowtxt.h"
#include "handlehwxml.h"
#include "handleswxml.h"
#include "handleuixml.h"

class XMLMANAGERSHARED_EXPORT XmlManager
{
public:
    XmlManager();
public:
    QString ReadUIXml(QString strPath,UIXML_Map &uiXml);
    QString WriteUIXml(QString strPath);

    QString ReadSWConfigXml(QString strPath,SWCONFIGXML_MAP &swConfigXml);
    QString WriteSWConfigXml(QString strPath);
    bool WriteSWSerXml(QString strPath,QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
    QString ReadHWConfigXml(QString strPath,HWCONFIGXML_MAP &hwConfigXml);
    QString WriteHWConfigXml(QString strPath);

    QString ReadFlowTxt(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadORGPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadResetPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString WriteFlowTxt(QString strPath);

    QString ReadChildFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadChildResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadORGFlowPath(QString strPath,FLOWTXT_MAP &flowXml);

private:
    HandelFlowTxt m_handelFlowTxt;
    HandleHWXml   m_handleHWXml;
    HandleSWXml   m_handleSWXml;
    HandleUIXml   m_handleUIXml;
};

#endif // XMLMANAGER_H
