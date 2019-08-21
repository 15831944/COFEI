#include "xmlmanager.h"
#include <QFile>

XmlManager::XmlManager()
{
}

QString XmlManager::ReadUIXml(QString strPath,UIXML_Map &uiXml)
{
    return m_handleUIXml.ReadUIXml(strPath,uiXml);
}

QString XmlManager::WriteUIXml(QString strPath)
{
    return m_handleUIXml.WriteUIXml(strPath);
}

QString XmlManager::ReadSWConfigXml(QString strPath,SWCONFIGXML_MAP &swConfigXml)
{
    return m_handleSWXml.ReadSWConfigXml(strPath,swConfigXml);
}

QString XmlManager::WriteSWConfigXml(QString strPath)
{
    return m_handleSWXml.WriteSWConfigXml(strPath);
}
bool XmlManager::WriteSWSerXml(QString strPath,QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp1,QMap<QString,SWCONFIGXML_LCSP_STRU> temp2)
{
    return m_handleSWXml.WriteSWSerXml(strPath,temp1,temp2);
}

QString XmlManager::ReadHWConfigXml(QString strPath,HWCONFIGXML_MAP &hwConfigXml)
{
    return m_handleHWXml.ReadHWConfigXml(strPath,hwConfigXml);
}

QString XmlManager::WriteHWConfigXml(QString strPath)
{
    return m_handleHWXml.WriteHWConfigXml(strPath);
}

QString XmlManager::ReadFlowTxt(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadFlowTxt(strPath,flowXml);
}

QString XmlManager::WriteFlowTxt(QString strPath)
{
    return m_handelFlowTxt.WriteFlowTxt(strPath);
}

QString XmlManager::ReadChildFlowPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadChildFlowPath(strPath,flowXml);
}
QString XmlManager::ReadFlowPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadFlowPath(strPath,flowXml);
}
QString XmlManager::ReadORGPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadORGPath(strPath,flowXml);
}
QString XmlManager::ReadResetPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadResetPath(strPath,flowXml);
}
QString XmlManager::ReadResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadResetFlowPath(strPath,flowXml);
}
QString XmlManager::ReadChildResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadChildResetFlowPath(strPath,flowXml);
}
QString XmlManager::ReadORGFlowPath(QString strPath,FLOWTXT_MAP &flowXml)
{
    return m_handelFlowTxt.ReadORGFlowPath(strPath,flowXml);
}
