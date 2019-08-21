#ifndef CONFIG__H
#define CONFIG__H

#include "trajectory/typedef.h"
#include <QMap>
#include <QDebug>
#include <Comm.h>
#include <QString>
#include <MyGlobal.h>
#include <iostream>
#include <QXmlStreamReader>

namespace MP {

class Config
{
public:
    static Config *GetInstance() {
        static Config conf;
        return &conf;
    }

    const TrajectoryInfo *getTrajectoryInfo(const QString &trajectory_id) const;

    friend std::ostream &operator <<(std::ostream &out, const Config &c);

private:
    Config();
    ~Config();

    void ReadMotionInfo(QString config_file, const QString &strElement, MotionInfo &motion_info);
    void getAxisId(QXmlStreamReader &reader, const QString &node, MotionInfo &info);
    void getPathes(QXmlStreamReader &reader, const QString &node, MotionInfo &info);
    void getPathAttribute(QXmlStreamReader &reader, TrajectoryInfo &path);
    void getPathContent(QXmlStreamReader &reader, const QString &end_node, TrajectoryInfo &path);
    void getToolParamFile(QXmlStreamReader &reader, const QString &end_node, MotionInfo &info);

    void GetAxisLimit(AxisInfo &axis_info);
    VelocityCurveInfo::VelocityCurveType GetVelocityCurveType(const QString &str) const;

public:
    QXmlStreamReader *xml_reader;
    MotionInfo *motion_info;

    double R_current_angle;
};

}   // end of namespace MP

#endif // CONFIG_H
