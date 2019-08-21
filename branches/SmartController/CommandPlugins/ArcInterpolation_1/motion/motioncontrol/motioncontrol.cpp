#include "motioncontrol.h"
#include "QDebug"

using namespace CoreGlobal;

MotionControl::MotionControl()
{}

MotionControl::~MotionControl()
{}

bool MotionControl::init(QString & eMsg)
{
    if (!initParams()) {
        eMsg = " 读取轴初始化参数失败";
        return false;
    }

    if (!set_axises_mode_to_csp()) {    // 确保轴的模式为csp模式
        eMsg = " 轴切换都CSP模式失败";
        return false;
    }

    if (!init_all_axises()) {
        eMsg = " 轴初始化失败";
        return false;
    }

    return true;
}

bool MotionControl::run(Pose & increase, QMap<QString, int> & target_position, QString & errMsg)
{
    /********************************************/

    QVector<QPair<int,int>> temp;
    uchar output[InOutPutData::GetInstance()->outPutNum];

    if (is_displacement_too_large(increase)) {
        errMsg = " 插补位置增量超过限制";
        return false;
    }

    // 四舍五入
    target_position["X"] += qRound(increase.x);
    target_position["Y"] += qRound(increase.y);
    target_position["Z"] += qRound(increase.z);
    target_position["Roll"] += qRound(increase.roll);

    // 判断是否超出设置的轴软极限
    for (auto it = axises_.begin(); it != axises_.end(); it++) {
        if ("" == it.value().id)
            continue;

        if (it.value().is_out_of_soft_limit(target_position[it.key()])) {
            errMsg = " " + it.key() + " 超出软极限";
            return false;
        }

        BaseAxisOperate::SetAxisPos("I32", 4, target_position_address_[it.value().id], target_position[it.key()] ,output);
        QPair<int, int> pair = QPair<int, int>(target_position_address_[it.value().id], 4);
        temp.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(output, temp);
    sleep(4000000);   // sleep，控制插补周期, 单位：ns

    return true;
}

void MotionControl::stop()
{
    if (path_) {
        delete path_;
        path_ = NULL;
    }
}

bool MotionControl::initParams()
{
    bool isOk = false;
    get_axises_id();
    isOk = get_excitation_params();
    for (auto it = axises_.begin(); it != axises_.end(); it++)
    {
        get_statusword_pos(it.value().id);
        get_controlword_pos(it.value().id);
    }

    return isOk;
}

void MotionControl::set_axises_id(QMap<QString, AxisInfo> ids)
{
    for (auto it = ids.begin(); it != ids.end(); it++)
        axises_.insert(it.key(), it.value());
}

// 获取轴Id
bool MotionControl::get_axises_id()
{
    return true;
}

// 获取励磁参数
bool MotionControl::get_excitation_params()
{
    bool isOk = true;

    //获取检查励磁是否成功的参数（bit，vaule）
    //获取励磁操作返回的数据，表示励磁返回数据的是哪些位和值的含义
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec)
    {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    if (m_tempInOP.length() <= 0)
        isOk = false;

    //获取如何励磁的参数（bit，vaule）
    //如何励磁，应该设置的位和应该设置的值
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
        int offValue = 0;
        if(0 == item.value)
            offValue = 1;

        //如何关闭励磁
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offValue);
        m_tempOFFSON.append(pairoff);
    }
    if (m_tempOutOP.length() <= 0)
        isOk = false;

    return isOk;
}

// 获取控制相关字节的地址
bool MotionControl::get_controlword_pos(const QString axis_id)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);

    if (!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(axis_id))
        return false;

    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[axis_id].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[axis_id].modelType;

    if (!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;

    for (auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
         item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();
         ++item)
    {
        int ibyteNum = item->byteNum;
        if ("Contrlworld" == item->key)
        {
            control_word_address_.insert(axis_id, ibytePos);
            v_control_word_address_.append(ibytePos);
        }
        else if("Model" == item->key)
            operate_mode_address_.insert(axis_id, ibytePos);
        else if("Trapos" == item->key)
            target_position_address_.insert(axis_id, ibytePos);
        else if("Traspeed" == item->key)
            target_speed_address_.insert(axis_id, ibytePos);
        else if("Accel" == item->key)
            accelerate_address_.insert(axis_id, ibytePos);
        else if("Decel" == item->key)
            decelerate_address_.insert(axis_id, ibytePos);

        ibytePos += ibyteNum;
    }

    return true;
}

// 获取状态相关字节的地址
bool MotionControl::get_statusword_pos(const QString axis_id)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);

    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[axis_id].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[axis_id].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;

    for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
        it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();
        ++it)
    {
        int ibyteNum = it->byteNum;
        if("Statusworld" == it->key)
        {
            status_word_address_.insert(axis_id, ibytePos);
            v_status_word_address.append(ibytePos);
        }
        else if("Model" == it->key)
            status_word_current_mode_address_.insert(axis_id, ibytePos);
        else if("Curpos" == it->key)
            status_word_current_position_address_.insert(axis_id, ibytePos);

        ibytePos += ibyteNum;
    }

    return true;
}

// 获取轴的当前位置
// to do better...
// 不需要重复去读取状态字节的地址信息
bool MotionControl::get_current_pos(const QString axis_id, int & curPos)
{
    QMutexLocker locker( &ConfigData::GetInstance()->m_mutex );

    if( !ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(axis_id) )
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[axis_id].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[axis_id].modelType;
    if( !ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType) )
        return false;

    for( auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
         it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();
         ++it )
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;

        if(it->key == "Curpos")
        {
            // 数据结构赋值存储
            curPos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);
            return true;
        }
        ibytePos += ibyteNum;
    }
    return false;
}

// 初始化所有的轴
bool MotionControl::init_all_axises()
{
    for (auto it = axises_.begin(); it != axises_.end(); it++) {
        if (it.value().id != "")    // 对不存在的轴进行初始化，会报错，80.4
            if (!init_axis(it.value().id))
                return false;
    }

    return true;
}

// 初始化轴
bool MotionControl::init_axis(const QString axis_id)
{
    if (is_axis_init(axis_id))   // 轴已初始化
        return true;

    excit(axis_id);    // 励磁轴

    for (int i = 0; i < 10; i++)    // 若励磁失败，最多尝试10次重新励磁
    {
        if (is_axis_init(axis_id))
            return true;

        excit(axis_id);
    }

    if (is_axis_init(axis_id))
        return true;

    return false;
}

// 励磁轴
void MotionControl::excit(const QString axis_id)
{
    init_current_pos(axis_id);

    // 先关闭轴的励磁
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON, control_word_address_[axis_id]);
    QThread::msleep(100);

    // 打开轴的励磁
    BaseAxisOperate::SetAxisExcite(m_tempOutOP, control_word_address_[axis_id]);
    QThread::msleep(100);
}

// 检查轴是否初始化
bool MotionControl::is_axis_init(const QString axis_id)
{
    if (BaseAxisOperate::CheckAxisFault(3, 2, status_word_address_[axis_id]))
        return false;

    if (!BaseAxisOperate::CheckAxisExcited(m_tempInOP, 2, status_word_address_[axis_id]))
        return false;

    return true;
}

// 初始化当前位置,把当前位置写入驱动器，防止励磁时意外
void MotionControl::init_current_pos(const QString axis_id)
{
    int curPos = 0;
    get_current_pos(axis_id, curPos);
    BaseAxisOperate::SetAxisPos("I32", 4, target_position_address_[axis_id], curPos);
    QThread::msleep(100);
}

// 切换所有轴的模式为csp
bool MotionControl::set_axises_mode_to_csp()
{
    for (auto it = status_word_current_mode_address_.begin(); it != status_word_current_mode_address_.end(); it++)
    {
        if (8 == get_current_mode(it.key()))   // 当前已经是csp模式
            continue;

        init_current_pos(it.key());   // 初始化当前位置

        switch_mode_to_csp(it.key());
        for (int i = 0; i < 10; i++)    // 若模式设置失败，尝试重新设置10次
        {
            if (8 == get_current_mode(it.key()))
                break;

            switch_mode_to_csp(it.key());
        }

        if (8 != get_current_mode(it.key()))
            return false;
    }

    return true;
}

/*
 *  return value
 *  8 - csp
 */
int MotionControl::get_current_mode(const QString axis_id)
{
    return BaseCalcFun::GetByteValue("I8", 1, status_word_current_mode_address_[axis_id]);
}

// 切换所有轴的模式为csp模式
void MotionControl::switch_mode_to_csp(const QString axis_id)
{
    BaseAxisOperate::ChangeAxisModel("I8", 1, operate_mode_address_[axis_id], 8);
    QThread::msleep(50);
}

// 获取位移形式表示的各个轴的位置
void MotionControl::set_axises_motion_path(const QMap<QString, QVector<int> > * mPath)
{
    path_ = mPath;
}

//void MotionControl::set_axises_motion_params(QVector<MotionParams> * motion_params)
//{
//    this->motion_params = *motion_params;
//}
void MotionControl::set_axises_motion_params(QVector<Segment *> motion_params)
{
    this->motion_params = motion_params;
}

// 计算插补周期,单位：ns
void MotionControl::calculate_interpolation_cycle()
{
    // to do...
    interpolation_cycle_ = 1000;
}

// 线程sleep，控制插补周期
void MotionControl::sleep(int time)
{
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    t.tv_nsec += time;
    while (t.tv_nsec >= kNsecPerSec) {
        t.tv_nsec -= kNsecPerSec;
        t.tv_sec++;
    }

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
}

bool MotionControl::is_displacement_too_large(const double & d)
{
    if (abs(d) > kMaxDisplacement)
        return true;
    return false;
}

bool MotionControl::is_displacement_too_large(const Pose & p)
{
    if (abs(p.x) > kMaxDisplacement || abs(p.y) > kMaxDisplacement || abs(p.z) > kMaxDisplacement)
        return true;
    return false;
}
