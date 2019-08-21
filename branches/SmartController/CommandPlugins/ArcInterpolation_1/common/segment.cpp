#include "segment.h"
#include <QDebug>
#include <QFile>
#include <QIODevice>

namespace route_planning_typedef {

Segment::Segment(SegmentInfo & segment_info) : SegmentInfo(segment_info)
{
    calibration_file = nullptr;
    stream = nullptr;

    config = config->GetInstance();
    position = new QVector<Position>;
    pose = new QVector<mat>;

    set_tool_init_angle();
    set_tool_projection_length();
}

Segment::~Segment()
{
    delete pose;
    delete position;

    close_calibration_file();
}

bool Segment::Init(QString & errMsg)
{
    if (!get_position(errMsg))
        return false;

    int size = position->size();
    set_start((*position)[0]);
    set_end((*position)[size - 1]);

    return true;
}

QVector<Position> * Segment::get_position(QString & errMsg)
{
    if (!position) {
        position = new QVector<Position>;
        if (!position) {
            errMsg = "path get_position() : memory allocate fail.";
            return NULL;
        }
    }

    if (position->isEmpty()) {    // only read once
        if (!read(errMsg))
            return NULL;
    }

    return position;
}

Posture Segment::get_posture() const
{
    //    return (this->posture);
}

bool Segment::read(QString & err)
{
    position->clear();

    Position p;
    std::string str;
    std::string f = this->file.toStdString();
    std::fstream file(f);

    str = "";
    double x, y, z;
    while ( !file.eof() )
    {
        std::getline(file, str);
        std::stringstream strin(str);

        // to do better
        strin >> x >> y >> z;
        p.set_position(x, y, z);
        position->push_back(p);
    }

    return true;
}

QString Segment::get_file() const
{
    return file;
}

SegmentType Segment::get_type() const
{
    return type;
}

void Segment::set_start(Position start)
{
    this->start = start;
}

Position Segment::get_start() const
{
    return start;
}

void Segment::set_end(Position end)
{
    this->end = end;
}

Position Segment::get_end() const
{
    return end;
}

double Segment::degree2radian(double degree)
{
    return degree * M_PI / 180.0;
}

double Segment::radian2degree(double radian)
{
    return radian * 180.0 / M_PI;
}

bool Segment::is_equal(const Position & p1, const Position & p2)
{
    double x = p1.x - p2.x;
    if (x < 0)
        x = -x;
    double y = p1.y - p2.y;
    if (y < 0)
        y = -y;
    double z = p1.z - p2.z;
    if (z < 0)
        z = -z;

    if (x < 0.01 && y < 0.01 && z < 0.01)
        return true;
    return false;
}

bool Segment::is_equal(const double & a, const double & b)
{
    double c = a - b;
    if (c < 0)
        c = -c;

    if (c < 0.01)
        return true;
    return false;
}

bool Segment::set_tool_projection_length()
{
    tool_projection_length = config->motion_info->tool_projection_length;
    qDebug() << "tool projection length is: " << tool_projection_length << endl;

    return true;
}

bool Segment::set_tool_init_angle()
{
    tool_init_degree = config->motion_info->tool_init_degree;
    qDebug() << "tool init degree is: " << tool_init_degree << endl;

    return true;
}

bool Segment::get_offset(mat & m)
{
    if (!calibration_file->size())
        return false;

    if (!calibration_file->seek(0))
        return false;

    (*stream) >> offset.x_trans >> offset.y_trans >> offset.z_rot;
    m = HomogeneousTransformationMatrix::get_trans_matrix(offset.x_trans, offset.y_trans, 0) *
            HomogeneousTransformationMatrix::get_rot_matrix(0, 0, offset.z_rot);

    return true;
}

bool Segment::open_calibration_file(QString & errMsg)
{
    bool isOk = true;
    if (!(calibration_file = new QFile(this->config->motion_info->camera_calibration_file)))
        isOk = false;
    if (!calibration_file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Open Calibration File Fail. File Path: " << this->config->motion_info->camera_calibration_file << endl;
        errMsg = "Open Calibration File Fail";
        isOk = false;
    }

    if ( !(stream = new QDataStream(calibration_file)) )
        isOk = false;
    else stream->setVersion(QDataStream::Qt_5_7);

    return true;
}

void Segment::close_calibration_file()
{
    if (calibration_file) {
        delete calibration_file;
        calibration_file = nullptr;
    }

    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

} //end namespace route_planning_typedef
