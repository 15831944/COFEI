#include "converter.h"

namespace MP {

ServoInstructionUnitConverter::ServoInstructionUnitConverter()
{
    // 一个用户指令单位对应伺服电机0.001mm的位移
    kXDisplacementPerUserUnit = 0.001;
    kYDisplacementPerUserUnit = 0.001;
    kZDisplacementPerUserUnit = 0.001;
    kYawDegreePerUserUnit = 0.001;
    kPitchDegreePerUserUnit = 0.001;
    kRollDegreePerUserUnit = 0.001;
}

ServoInstructionUnitConverter::~ServoInstructionUnitConverter()
{}

void ServoInstructionUnitConverter::convert(Pose &pose)
{
    pose.x /= kXDisplacementPerUserUnit;
    pose.y /= kYDisplacementPerUserUnit;
    pose.z /= kZDisplacementPerUserUnit;
    pose.yaw /= kYawDegreePerUserUnit;
    pose.pitch /= kPitchDegreePerUserUnit;
    pose.roll /= kRollDegreePerUserUnit;
}

}   // end of namespace MP
