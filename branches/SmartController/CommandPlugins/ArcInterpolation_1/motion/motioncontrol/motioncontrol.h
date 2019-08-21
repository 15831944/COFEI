#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include <time.h>
#include <QVector>
#include <QString>
#include <MyGlobal.h>
#include <typedef.h>
#include "converter/converter.h"

using namespace route_planning_typedef;

class MotionControl
{
public:
    MotionControl();
    ~MotionControl();

    void set_axises_id(QMap<QString, AxisInfo>);
    bool init(QString & eMsg);
    void set_axises_motion_path(const QMap<QString, QVector<int>> * path);  // 位移形式表示的各个轴的位置
    bool run(Pose & increase, QMap<QString, int> & target_position, QString & errMsg);
    void stop();

    //    void set_axises_motion_params(QVector<MotionParams> * motion_params);
    void set_axises_motion_params(QVector<Segment *> motion_params);

    bool get_current_pos(const QString axis_id, int & curPos);    // 根据轴ID获取该轴的当前位置, 用户单位

private:
    bool initParams();  // 参数初始化
    bool get_axises_id();  // 获取轴id

    /* 轴初始化相关函数 */
    bool init_all_axises();    // 初始化所有的轴
    bool get_excitation_params(); // 获取励磁参数
    bool init_axis(const QString axis_id);    // 初始化轴
    bool is_axis_init(const QString axis_id);  // 检查轴是否初始化
    void excit(const QString axis_id);   // 励磁轴
    bool get_controlword_pos(const QString axis_id);    // 根据轴ID获取该轴控制字节地址,并保存在map结构中
    bool get_statusword_pos(const QString axis_id);    // 根据轴ID获取该轴状态字节地址,并保存在map结构中

    /* 轴位置获取与设置相关函数 */

    void init_current_pos(const QString axis_id);  // 初始化当前位置

    /* 轴模式检测与切换相关函数 */

    bool set_axises_mode_to_csp();
    int get_current_mode(const QString axis_id);   // 读取当前模式
    void switch_mode_to_csp(const QString axis_id); // 切换模式为csp模式

    void calculate_interpolation_cycle();   // 计算插补周期
    void sleep(int time);    // 线程sleep，控制插补周期

    bool is_displacement_too_large(const double & d); // 判断位移是否过大
    bool is_displacement_too_large(const Pose & p);

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

    // to do better
    // 用这样的数据结构保存不同轴的位置信息，程序的局部性差
    const QMap<QString, QVector<int>> * path_;   // 各个轴运动的目标位置

    //    QVector<MotionParams> motion_params;
    QVector<Segment *> motion_params;

    long interpolation_cycle_;    // 插补周期，单位：ns
    const int kNsecPerSec = 1000000000;
    const int kMaxDisplacement = 5000;  // 单位: 0.001mm
};

#endif // MOTIONCONTROL_H
