#include "arc.h"
#include "curve.h"
#include "trajectories.h"
#include "straightline.h"
#include "BaseTrajectory.h"
#include "ServoControl.h"

#include <string>
#include <fstream>
#include <QStringList>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace MP {

Trajectories::Trajectories()
{
    trajectories_info = nullptr;
    trajectories.clear();
}

Trajectories::~Trajectories()
{
}

void Trajectories::SetTrajectoriesInfo(const QMap<QString, TrajectoryInfo> *trajectories_info)
{
    this->trajectories_info = trajectories_info;
}

Trajectory &Trajectories::GetTrajectory(const QString id)
{
    if (!trajectories.contains(id)) {
        if (!trajectories_info->contains(id))
            throw std::logic_error(("can not find trajectory: " + id).toStdString());

        for (auto item = trajectories_info->begin(); item != trajectories_info->end(); ++item)
            if (id == item.key()) {
                Trajectory *trajectory = new Trajectory(&(item.value()));
                trajectories.insert(id, trajectory);
            }
    }
    trajectories[id]->init();

    return *(trajectories.value(id));
}

Trajectory::Trajectory(const TrajectoryInfo *trajectory_info)
{
    info = trajectory_info;
    id = info->id;
    //    init();
}

Trajectory::~Trajectory()
{
    info = nullptr;
}

void Trajectory::SetInterpolationCycle(double cycle)
{
    interpolation_cycle = cycle;
}

void Trajectory::SetToolProjectiveLength(double length)
{
    tool_projective_length = length;
}

void Trajectory::SetToolOriginAngle(double angle)
{
    tool_origin_angle = angle;
}

void Trajectory::init()
{
    SetInterpolationCycle(INTERPOLATION_CYCLE);
    readToolParamsFromFile(Config::GetInstance()->motion_info->tool_param_file);
    setCartSamples();
    calCartIncrease();
    calInterPolationCount();
    calJointIncrease();
}

QString Trajectory::GetId() const
{
    return id;
}

int Trajectory::GetInterpolationCount() const
{
    return interpolation_count;
}

void Trajectory::setCartSamples()
{
    if ("示教" == info->path_info.mode) {
        setJointSamplesFromTeachPoint();
        setCartSamplesFromJoint();
    }
    else if ("文件导入" == info->path_info.mode)
        setCartSamplesFromTxtFile();
    else
        throw std::logic_error("undefined path mode");

    scale();
    setOffsets();
}

void Trajectory::setJointSamplesFromTeachPoint()
{
    joint_samples.clear();

//    QString J1 = Config::GetInstance()->motion_info->axises_info["J1"].id;
//    QString J2 = Config::GetInstance()->motion_info->axises_info["J2"].id;
//    QString J3 = Config::GetInstance()->motion_info->axises_info["J3"].id;
//    QString J4 = Config::GetInstance()->motion_info->axises_info["J4"].id;

    QString J1 = Config::GetInstance()->motion_info->axises_info[Config::GetInstance()->motion_info->trajectories_info[id].path_info.axis_group]["J1"].id;
    QString J2 = Config::GetInstance()->motion_info->axises_info[Config::GetInstance()->motion_info->trajectories_info[id].path_info.axis_group]["J2"].id;
    QString J3 = Config::GetInstance()->motion_info->axises_info[Config::GetInstance()->motion_info->trajectories_info[id].path_info.axis_group]["J3"].id;
    QString J4 = Config::GetInstance()->motion_info->axises_info[Config::GetInstance()->motion_info->trajectories_info[id].path_info.axis_group]["J4"].id;

    const auto &Teach_Points = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
    for (auto teach_point : info->path_info.teach_points) {
        if (!Teach_Points.contains(teach_point))
            throw std::logic_error("setTeachPoints(): can not find teach point " + teach_point.toStdString());

        Pose p;
        if (!J1.isEmpty() && Teach_Points[teach_point].axisMap.contains(J1))
            p.x = Teach_Points[teach_point].axisMap[J1].pos;
        if (!J2.isEmpty() && Teach_Points[teach_point].axisMap.contains(J2))
            p.y = Teach_Points[teach_point].axisMap[J2].pos;
        if (!J3.isEmpty() && Teach_Points[teach_point].axisMap.contains(J3))
            p.z = Teach_Points[teach_point].axisMap[J3].pos;
        if (!J4.isEmpty() && Teach_Points[teach_point].axisMap.contains(J4))
            p.roll = Teach_Points[teach_point].axisMap[J4].pos;

        joint_samples.push_back(p);
    }
}

void Trajectory::setCartSamplesFromTxtFile()
{
    cart_samples.clear();

    using std::__cxx11::string;

    string file = (info->path_info.data_file).toStdString();
    std::ifstream in(file);
    if (!in.is_open())
        throw std::runtime_error("open data file fail.");

    cart_samples.clear();

    string str;
    getline(in, str);
    QString s = QString::fromStdString(str);
    QStringList list = s.split(",");
    if (list.size() != 2)
        throw std::runtime_error("data file format wrong.");
    int count = list[1].toDouble();
    std::cout << "count: " << count << std::endl;


    while (getline(in, str)) {
        QString s = QString::fromStdString(str);
        QStringList list = s.split(",");
        if (list.size() != 3)
            throw std::runtime_error("data file format wrong.");

        Pose pos(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), 0, 0, 0);
        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

        cart_samples.push_back(pos);
    }
    if (cart_samples.size() != count) {
        cart_samples.clear();
        throw std::runtime_error("data file error.");
    }
}

void Trajectory::scale()
{
    if (!cart_samples.isEmpty())
        for (Pose &p : cart_samples) {
            p.x *= info->path_info.x_scale_coefficient;
            p.y *= info->path_info.y_scale_coefficient;
        }
}

void Trajectory::setOffsets()
{
    offsets.clear();
    using std::__cxx11::string;
    if (info->path_info.offset_file != "") {
        string file = (info->path_info.offset_file).toStdString();
        std::ifstream in(file);
        if (!in.is_open())
            throw std::runtime_error("open offset file fail.");

        string str;
        while (getline(in, str)) {
            QString s = QString::fromStdString(str);
            QStringList list = s.split(",");
            if (list.size() != 3)
                throw std::runtime_error("offset file format wrong");

            Pose offs(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), 0, 0, 0);
            offsets.push_back(offs);
        }

        for (auto i : offsets)
            std::cout << i.x << " " << i.y << " " << i.z << std::endl;

        std::cout << offsets.size() << std::endl;
        std::cout << cart_samples.size() << std::endl;
        if (offsets.size() != cart_samples.size())
            throw std::runtime_error("数据文件和偏移文件数据长度不一致");

        for (int i = 0; i < offsets.size(); ++i) {
            cart_samples[i].x += offsets[i].x;
            cart_samples[i].y += offsets[i].y;
            cart_samples[i].z += offsets[i].z;
        }
    }

    for (auto i : cart_samples)
        std::cout << i.x << " " << i.y << " " << i.z << std::endl;
}

QVector<Pose> *Trajectory::GetJointSamples()
{
    return &joint_samples;
}

void Trajectory::setCartSamplesFromJoint()
{
    cart_samples.clear();

    XyzrFourAxisFK fk(tool_projective_length, tool_origin_angle);
    Pose p;
    for (Pose &pose : joint_samples) {
        p = fk.JntToCart(pose.x, pose.y, pose.z, pose.roll);
        cart_samples.push_back(p);
    }
}

QVector<Pose> *Trajectory::GetCartSamples()
{
    return &cart_samples;
}

Pose Trajectory::GetJointIncrease(int i)
{
    if (i < joint_increase.size())
        return joint_increase[i];

    throw std::runtime_error("index out of range.");
}

Position Trajectory::GetCartIncrease(int i)
{
    if (i < cart_increase.size())
        return cart_increase[i];

    throw std::runtime_error("index out of range.");
}

QString Trajectory::GetIOPort() const
{
    return info->path_info.IO_port;
}

int Trajectory::GetDelayBeforeStart() const
{
    return info->path_info.ahead_of_start;
}

void Trajectory::calCartIncrease()
{
    BaseTrajectory *traj = nullptr;
    if ("直线" == info->path_info.type)
        traj = new StraightLine(&cart_samples, &(info->velocity_curve_info));
    //    else if ("圆弧" == info->path_info.type)
    //        traj = new Arc(cart_samples, info->velocity_curve_info);
    else if ("样条曲线" == info->path_info.type)
        traj = new Curve(&cart_samples, &(info->velocity_curve_info));
    else
        throw std::logic_error("unknown path type");

    if (!traj->IsSamplesAvailable())
        throw std::runtime_error("samples is not available");

    cart_increase.clear();
    traj->GetIncrease(interpolation_cycle, &cart_increase, true, GenerateLogFileName());
    pos_to_set_io_disable = traj->GetNumberOfPos(info->path_info.ahead_of_end, interpolation_cycle);
    std::cout << "pos to set io disable: " << pos_to_set_io_disable << std::endl;
}

void Trajectory::calJointIncrease()
{
    joint_increase.clear();

    XyzrFourAxisIk ik(tool_projective_length, tool_origin_angle);
    double angle_increase = calAngleIncrease();
    double current_angle = getCurrentAngle();
    double next_angle = current_angle;

    std::cout << "current angle: " << current_angle << "\n"
              << "angle increase: " << angle_increase << std::endl;

    int size = cart_increase.size();
    Pose increase;
    for (int i = 0; i < size; ++i) {
        next_angle += angle_increase;
        increase = ik.CartToJnt(cart_increase[i], current_angle, next_angle);
        joint_increase.push_back(increase);
        current_angle = next_angle;
    }
    std::cout << "Interpolation count: " << interpolation_count << std::endl;
}

void Trajectory::calInterPolationCount()
{
    interpolation_count = cart_increase.size();
}

double Trajectory::getCurrentAngle()
{
    return Config::GetInstance()->R_current_angle;
}

double Trajectory::calAngleIncrease()
{
    double angle_increase;
    if ("示教" == info->path_info.mode) {
        if (!joint_samples.isEmpty())
            angle_increase = (joint_samples.last().roll - joint_samples.first().roll) / interpolation_count;
    }
    else if ("文件导入" == info->path_info.mode)
        if ("enable" == info->path_info.rotate_R)
            angle_increase = info->path_info.rotation_angle_R / interpolation_count;
        else
            angle_increase = 0;

    return angle_increase;
}

void Trajectory::readToolParamsFromFile(QString file)
{
    tool_projective_length = 0;
    tool_origin_angle = 0;

    std::ifstream in(file.toStdString());
    if (!in.is_open())
        throw std::runtime_error("Open tool param file fail.");

    string str;
    getline(in, str);
    QString s = QString::fromStdString(str);
    QStringList list = s.split(",");
    if (list.size() != 2)
        throw std::runtime_error("tool param file format wrong.");
    if (!isDigitalStr(list[0]))
        throw std::runtime_error("tool projective length wrong.");
    if (!isDigitalStr(list[1]))
        throw std::runtime_error("tool origin angle wrong.");

    bool ok;
    tool_projective_length = list[0].toDouble(&ok);
    if (!ok)
        throw std::runtime_error("read tool projective length fail.");

    tool_origin_angle = list[1].toDouble(&ok);
    if (!ok)
        throw std::runtime_error("read tool origin angle fail.");

    std::cout << "tool projective length: " << tool_projective_length << "\n"
              << "tool origin angle: " << tool_origin_angle << std::endl;
}

bool Trajectory::isDigitalStr(QString str)
{
    QByteArray d = str.toLatin1();
    const char *p = d.data();
    while (*p && ((*p >= '0' && *p <= '9') || ('.' == *p) || ('-' == *p)))
        p++;
    if (*p)
        return false;
    return true;
}

std::__cxx11::string Trajectory::GenerateLogFileName()
{
    return std::__cxx11::string("/tmp/" + id.toStdString() + "_" +
                                boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) + ".log");
}

}   // end of namespace MP
