#ifndef HANDELFLOWTXT_H
#define HANDELFLOWTXT_H
#include <Comm.h>
#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
class HandelFlowTxt
{
public:
    HandelFlowTxt();
    QString ReadFlowTxt(QString strPath,FLOWTXT_MAP &flowXml);
    QString WriteFlowTxt(QString strPath);

    QString ReadChildFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadORGPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadResetPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadChildResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
    QString ReadORGFlowPath(QString strPath,FLOWTXT_MAP &flowXml);
private:
    bool ReadFlow(QString strLine,int ,QMap<int,QMap<QString,FLOWTXT_STRU>> &flowxmlMap);
    bool ReadPath(QString strPath,FLOW_CMD_STRU &flowData);
    bool ReadText(QString strPath,QMap<int,QMap<QString,FLOWTXT_STRU>> &CmdMap);
};

#endif // HANDELFLOWTXT_H
