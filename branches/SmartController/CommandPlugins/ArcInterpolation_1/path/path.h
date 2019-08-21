#ifndef PATH_H
#define PATH_H

#include "motion/end/end_params.h"
#include "Comm.h"
#include <MyGlobal.h>
#include "typedef.h"
#include "config.h"
#include "path/profile/profile.h"
#include "path/coordinate_transformation/coordinate_transformation.h"

// 表示所有定义的路径在基坐标系中的位姿
class Path
{
public:
    static Path * GetInstance() // 单例模式
    {
        static Path p;
        return &p;
    }

    bool init(QString & errMsg);
    QVector<QVector<mat>> * get(QString profile_id, QString & errMsg);

    QVector<Segment* > * mode_2_get(QString profile_id, QString & errMsg);

private:
    Path();
    virtual ~Path();

private:
    QMap<QString, ProfileInfo> * path_info;
    QMap<QString, Profile> * path;
    CoordinateTransformation * coordinate_transformation;
    Config * conf;
};

#endif // PATH_H
