#ifndef CSPXYZLIB_H
#define CSPXYZLIB_H
#include <MyGlobal.h>
#include "lcsplib_global.h"
#include <QObject>
#include <QTimer>
#include "coordinate.h"
using namespace CoreGlobal;
class CSPXYZlib:public QObject
{
    Q_OBJECT
private: //关于轴的触发需要的参数情况
public :
    CSPXYZlib(const SWCONFIGXML_LCSP_STRU _ps);
    QMap<QString ,QVector<double>> GetLCSPXYZArray(const SWCONFIGXML_LCSP_STRU _ps);
    QMap<QString, double> GetXYZSincurpos(QMap<QString, double> pulsepos);
private:
    Coordinate *CSPSwLib;
    QMap<QString,QVector<QString>> m_XYZtoJMap;
    QVector<QString> m_AxisIdVec;
    QVector<QMap<QString,double >>m_summap;
public:
     QMap<QString,double> m_xyzcurposMap;
private:

    bool GetLCSPCurpos(QString strKey, int &curPos);
    bool GetlcspaxisinitParam(const SWCONFIGXML_LCSP_STRU _ps);
private://新增机械手函数
     QVector<QMap<QString ,QVector<double>>> HandleArray(const SWCONFIGXML_LCSP_STRU _ps);
    //QMap<QString, double> GetXYZSincurpos(QMap<QString, double> pulsepos);
    bool handleXYZsummap(QVector<QMap<QString, double>>,QVector<QMap<QString,QVector<double>>>&);
    int  GetXYZMaxPosnum(QMap<QString, double> axismappos, QMap<QString, double>axisfrontmappos);
    bool CalXYZculformula(QMap<QString,double>, QMap<QString, double>axisfrontmappos, QVector<QMap<QString, QVector<double> > > &sumlcspvec);
};

#endif // CSPXYZLIB_H
