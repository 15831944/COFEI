#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include <time.h>
#include <QVector>
#include <QString>
#include <MyGlobal.h>
#include "trajectory/typedef.h"
#include "converter.h"

using namespace MP;

class ServoControl
{
public:
    ServoControl();
    ~ServoControl();

    void set_axises_id(QMap<QString, AxisInfo>);
    void init();
    bool run(const Pose &increase, QMap<QString, int> &target_position);

    bool getAxisCurrentPos(const QString &axis_id, int &curPos);    // 根据轴ID获取该轴的当前位置, 用户单位

private:
    bool initParams();  // 参数初始化

    /* 轴初始化相关函数 */
    bool initAllAxises();    // 初始化所有的轴
    bool getExcitationParams(); // 获取励磁参数
    bool initAxis(const QString &axis_id);    // 初始化轴
    bool isAxisInit(const QString &axis_id);  // 检查轴是否初始化
    void excit(const QString &axis_id);   // 励磁轴
    bool getControlwordPos(const QString &axis_id);    // 根据轴ID获取该轴控制字节地址,并保存在map结构中
    bool getStatuswordPos(const QString &axis_id);    // 根据轴ID获取该轴状态字节地址,并保存在map结构中

    /* 轴位置获取与设置相关函数 */
    void initCurrentPos(const QString &axis_id);  // 初始化当前位置

    /* 轴模式检测与切换相关函数 */
    bool setAxisesMode2Csp();
    int getCurrentMode(const QString &axis_id);   // 读取当前模式
    void switchMode2Csp(const QString &axis_id); // 切换模式为csp模式

    void sleep(int time);    // 线程sleep，控制插补周期

    bool isIncreaseExceedTheLimit(const Pose &increase);

public:
    QMap <QString, AxisInfo> axises_;    // 所有轴的id

private:
    QMap<QString, int> control_word_address_; // 控制字节地址
    QVector<int> v_control_word_address_; //控制字节地址
    QMap<QString, int> operate_mode_address_; // 模式字节地址
    QMap<QString, int> target_position_address_;  // 目标位置字节地址
    QMap<QString, int> target_speed_address_;    // 目标速度字节地址
    QMap<QString, int> accelerate_address_;    // 加速度字节地址
    QMap<QString, int> decelerate_address_;    // 减速度字节地址

    QMap<QString, int> status_word_address_;  // 状态字位置
    QVector<int> v_status_word_address;  // 状态字位置
    QMap<QString, int> status_word_current_mode_address_; // 状态字－当前模式字节位置
    QMap<QString, int> status_word_current_position_address_;   // 状态字－当前位置字节位置


    uchar *tempOutputData;
    int ierror;

    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;

    const int kNsecPerSec = 1000000000;
    const int kMaxDisplacement = 5000;  // 指令单位
};

#endif // MOTIONCONTROL_H
