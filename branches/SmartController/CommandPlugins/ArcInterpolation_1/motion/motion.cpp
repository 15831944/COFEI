#include "motion.h"

Motion::Motion()
{
    end_params = NULL;
    converter = NULL;
    motion_control = NULL;

    config = config->GetInstance();
}

Motion::~Motion()
{
    delete motion_control;
    delete converter;
    delete end_params;
}

bool Motion::init(QString & errMsg)
{
    if (!end_params) {
        end_params = new EndParams;
        if (!end_params) {
            errMsg = "Error --> Motion.init(), end_params memory allocate fail.";
            return false;
        }
    }

    if (!converter) {
        converter = new Converter;
        if (!converter) {
            errMsg = "Error --> Motion.init(), converter memory allocate fail.";
            return false;
        }
    }

    if (!motion_control) {
        motion_control = new MotionControl;
        if (!motion_control) {
            errMsg = "Error --> Motion.init(), motion_control memeory allocate fail.";
            return false;
        }
    }
    motion_control->set_axises_id(config->motion_info->axises);

    return true;
}

void Motion::set_data(QVector<Segment *> segments)
{
    this->segments = segments;
}

void Motion::set_stop_var(bool *stop)
{
    this->stop = stop;
}

void Motion::set_data(QVector<mat> * const pose)
{
    pose_in_base = pose;
}

// for temp, to do better...
void Motion::set_data(QVector<QVector<mat> > * const pose)
{
    QVector<mat> * p = new QVector<mat>;
    for (auto item = pose->begin(); item != pose->end(); item++) {
        int size = item->size();
        for (int i = 0; i < size; i++)
            p->push_back((*item)[i]);
    }

    pose_in_base = p;
}

bool Motion::run(QString & errMsg)
{
    if (!motion_control->init(errMsg))
        return false;

    int size = segments.size();
    for (int i = 0; i < size; i++) {
        if (*stop) {
            errMsg = "停止";
            return false;
        }

        Segment* seg = segments[i];
        Pose increase;
        QMap<QString, int> current_position;
        motion_control->get_current_pos(motion_control->axises_["X"].id, current_position["X"]);
        motion_control->get_current_pos(motion_control->axises_["Y"].id, current_position["Y"]);
        motion_control->get_current_pos(motion_control->axises_["Z"].id, current_position["Z"]);
        motion_control->get_current_pos(motion_control->axises_["Roll"].id, current_position["Roll"]);
        QMap<QString, int> target_position = current_position;

        converter->convert(increase);
        for (int j = 0; j < seg->interpolation_num; j++) {
            if (*stop) {
                errMsg = "停止";
                return false;
            }

            increase = seg->get_increase(j);
            converter->convert(increase);
            motion_control->run(increase, target_position, errMsg);

            qDebug() << j << "/" << seg->interpolation_num << ":\n"
                     << "x_increase: " << increase.x
                     << "y_increase: " << increase.y
                     << "z_increase: " << increase.z
                     << "yaw_increase: " << increase.yaw
                     << "pitch_increase: " << increase.pitch
                     << "roll_increase: " << increase.roll << endl;
        }
    }

    return true;
}
