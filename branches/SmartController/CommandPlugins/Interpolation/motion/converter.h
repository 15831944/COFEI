#ifndef CONVERTER_H
#define CONVERTER_H


#include "config.h"
#include "trajectory/typedef.h"
#include "trajectory/BaseTrajectory.h"

#include <QMap>
#include <QVector>
#include <QString>

namespace MP {

// 将位移单位转为为给伺服的指令单位
class ServoInstructionUnitConverter
{
public:
    ServoInstructionUnitConverter();
    virtual ~ServoInstructionUnitConverter();

    void convert(Pose &pose);

public:
    double kXDisplacementPerUserUnit;   // X轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kYDisplacementPerUserUnit;   // Y轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kZDisplacementPerUserUnit;   // Z轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kYawDegreePerUserUnit;   // 控制偏航角的伺服电机,一个指令单位对应的旋转角度, 单位: degree
    double kPitchDegreePerUserUnit;    // 控制仰角的伺服电机, 一个指令单位对应的旋转角度, 单位: degree
    double kRollDegreePerUserUnit; // 控制滚动角的伺服电机, 一个指令单位对应的旋转角度, 单位: degree
};

}   // end of namespace MP

#endif // CONVERTER_H
