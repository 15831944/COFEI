#ifndef CONFIG__H
#define CONFIG__H

#include <QMap>
#include <QDebug>
#include <Comm.h>
#include <QString>
#include <MyGlobal.h>
#include "typedef.h"
#include <iostream>

namespace route_planning_typedef {

// 表示xml中路径配置信息
class Config
{
private:
    Config();
    ~Config();

public:
    static Config * GetInstance() {
        static Config conf;
        return &conf;
    }

    ProfileInfo * get_one_profile_info(QString profile_id) const;

    friend std::ostream & operator <<(std::ostream & os, const Config & c);

public:
    MotionInfo * motion_info;
};

}   // end namespace route_planning_typedef

#endif // CONFIG_H
