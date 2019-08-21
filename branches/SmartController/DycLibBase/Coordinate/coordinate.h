#ifndef COORDINATE_H
#define COORDINATE_H

#include "coordinate_global.h"
#include <QPair>
#include <QMap>
#include <MyGlobal.h>
#include <QMutex>
#include <QMutexLocker>
typedef struct _PointStr
{
    double elegeart = -1;
    QString axisId = "";
}PointStr;
class COORDINATESHARED_EXPORT Coordinate
{

public:
    Coordinate();
private:
    double line_l;//对边长
    double angle_l13;//1 3 边的夹角
    double angel;//夹边和y轴的夹角
    QMutex m_mutex;

public:
    QPair<double,double> XYtoJog(double l1, double l2, double  x, double y, int lr1, int lr2);
    QPair<double,double> GetXYpoint(double l1,double l2,int J1, int J2, double lr1, double lr2);
    QMap<QString,double> XYtoJ_W(QMap<QString,QMap<QString,double>> PointMap);//_SWCONFIGXML_LCSP_STRU   csp struct;
    void CovertTo_J(SWCONFIGXML_LCSP_STRU &p_pointStr);
    void CovertTo_X(SWCONFIGXML_LCSP_STRU &p_pointStr,QMap<QString,QVector<QString>> coordMap);
    PointStr Getgear(QString coordina);

};

#endif // COORDINATE_H
