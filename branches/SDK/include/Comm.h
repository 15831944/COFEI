#ifndef COMM_H
#define COMM_H
#include <QString>
#include <QMap>
#include <QVector>
#include <QSize>

enum UIType
{
    debug,
    mytest
};

//UI部分xml的结构体
typedef struct _UIXML_STRU
{
    QString id = "";
    QString strText = "";
    int fontsize = -1;
    QString strColor = "";
    QString className = "";
    QString IcoPath = "";
    QString address = "";
    QString signalType = "";
    QString model = "";
    QSize csize = QSize(100,100);
    QString strEvent = "";

}UIXML_STRU;
//UI部分XML内存中的数据结构
typedef struct _UIXML_Map
{
    _UIXML_Map()
    {
        uiXmlMap.clear();
    }
    QMap<QString,QVector<UIXML_STRU>> uiXmlMap;
}UIXML_Map;

//硬件配置部分XML结构体
/***************************************************/
typedef struct _BITINFO_STRU
{
    QString key = "";
    QString name = "";
    int bit  = -1;
}BITINFO_STRU;
typedef struct _BYTEINFO_STRU
{
    QString key = "";
    QString name = "";
    int byteNum = -1;
    QString type = "";
    QMap<QString,BITINFO_STRU> bitInfoMap;
    _BYTEINFO_STRU()
    {
        bitInfoMap.clear();
    }
}BYTEINFO_STRU;

typedef struct _HWCONFIGXML_IO_STRU
{
    QString id = "";
    QString strType = "";
    int iByteNum = -1;
    int iBytePos = -1;

}HWCONFIGXML_IO_STRU;

typedef struct _HWCONFIGXML_SERINPUT_STRU
{
    QString id = "";
    int iByteNum = -1;
    int iBytePos = -1;
    QString modelType = "";

}HWCONFIGXML_SERINPUT_STRU;
typedef struct _HWCONFIGXML_SEROUTPUT_STRU
{
    QString id = "";
    QString hwLimitStart = "";
    QString orgLimit = "";//新增原点极限 2018/01/23
    QString hwLimitEnd = "";
    QString swLimitStart = "";
    QString swLimitEnd = "";
    int lead = 1;
    int adled = 1;
    int iByteNum = -1;
    int iBytePos = -1;
    int error = 0;
    QString modelType = "";


}HWCONFIGXML_SEROUTPUT_STRU;

typedef struct _HWCONFIGXML_MODEL_STRU
{
    int iByteNum = -1;
    QVector<BYTEINFO_STRU> byteInfoVec;
    _HWCONFIGXML_MODEL_STRU()
    {
        byteInfoVec.clear();
    }

}HWCONFIGXML_MODEL_STRU;

typedef struct _HWCONFIGXML_MODEL_MAP
{
    QMap<QString,QMap<QString,HWCONFIGXML_MODEL_STRU>> hwModelMap;

}HWCONFIGXML_MODEL_MAP;

typedef struct _HWCONFIGXML_INPUT_MAP
{
    QMap<QString,HWCONFIGXML_IO_STRU> hwIOMap;
    QMap<QString,HWCONFIGXML_SERINPUT_STRU> hwSerInputMap;
}HWCONFIGXML_INPUT_MAP;

typedef struct _HWCONFIGXML_OUTPUT_MAP
{
    QMap<QString,HWCONFIGXML_IO_STRU> hwIOMap;
    QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> hwSerOutputMap;
}HWCONFIGXML_OUTPUT_MAP;

typedef struct _HWCONFIGXML_NETWORK_STRU
{
    QString portName = "";

}HWCONFIGXML_NETWORK_STRU;

typedef struct _HWCONFIGXML_WEB_STRU
{
    QString Id = "";
    QString URL = "";

}HWCONFIGXML_WEB_STRU;

typedef struct _HWCONFIGXML_WEB_MAP
{
    QMap<QString,HWCONFIGXML_WEB_STRU> hwconfigWebMap;
    _HWCONFIGXML_WEB_MAP()
    {
        hwconfigWebMap.clear();
    }

}HWCONFIGXML_WEB_MAP;

typedef struct _HWCONFIGXML_COMPORT_STRU
{
    QString Id = "";
    QString portName = "";
    QString baudRate = "";
    QString headendData = "";

}HWCONFIGXML_COMPORT_STRU;
typedef struct _MQTT_STRU
{
    QString  id = "";
    QString  ip = "";
    QString  port = "";
    QMap<QString ,QString>topicalMap;

}MQTT_STRU;
typedef struct _HWCONFIGXML_INTNET_STRU
{
    QString Id = "";
    QString agrType = "";
    QString serType = "";
    QString ipAdd   = "";
    QString portName = "";

}HWCONFIGXML_INTNET_STRU;

//HW坐标系配置___________________________________
typedef struct _HWCONFIGXML_ARMINFO_STRU
{
    QString Id = "";
    double  armlen = 0.00;
    double decrat =  0.00;
    double elegearat = 0.00;

}HWCONFIGXML_ARMINFO_STRU;
typedef struct _HWCONFIGXML_CSYS_CONFIG_STRU
{
    QString Id = "";
    double  stalim = 0.00;
    double endlim =  0.00;
    QMap<QString, HWCONFIGXML_ARMINFO_STRU> hwArmInfoMap;
    _HWCONFIGXML_CSYS_CONFIG_STRU()
    {
        hwArmInfoMap.clear();
    }
}HWCONFIGXML_CSYS_CONFIG_STRU;

typedef struct _HWCONFIGXML_MAP
{
    HWCONFIGXML_INPUT_MAP hwConfigXmlInputMap;
    HWCONFIGXML_OUTPUT_MAP hwConfigXmlOutputMap;
    HWCONFIGXML_MODEL_MAP  hwConfigXmlModelMap;
    HWCONFIGXML_NETWORK_STRU hwConfigXmlNetworkStru;
    HWCONFIGXML_WEB_MAP hwConfigXmlWebMap;
    QMap<QString,HWCONFIGXML_COMPORT_STRU> hwConfigXmlComportMap;
    QMap<QString,HWCONFIGXML_INTNET_STRU> hwConfigXmlIntnetMap;
    QMap<QString,MQTT_STRU> hwConfigxmlMqttMap;
    QMap<QString,HWCONFIGXML_CSYS_CONFIG_STRU> hwConfigXmlCoordinateMap;//增加坐标配置
    _HWCONFIGXML_MAP()
    {
        hwConfigXmlComportMap.clear();
        hwConfigXmlIntnetMap.clear();
        hwConfigXmlCoordinateMap.clear();
    }
}HWCONFIGXML_MAP;


/*****************************************************/
typedef struct _SWCONFIGXML_BASE_STRU
{
}SWCONFIGXML_BASE_STRU;
//软件配置气缸部分的结构体
typedef struct _SWCONFIGXML_CY_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QVector<QString> soltCompVec;
    int io = -1;
    QString basePoint = "";
    QString movePoint = "";
    int timeOut = -1;
    int delayMsec = -1;
    QMap<int ,QString> MsgMap;
    _SWCONFIGXML_CY_STRU()
    {
        MsgMap.clear();
    }
}SWCONFIGXML_CY_STRU;

typedef struct _SWCONFIGXML_COMM_DATA_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString opType = "";
    QString savePathKey = "";
    QString commId = "";
    QString data = "";
    QString Ip = "";
    QString style= "";

}SWCONFIGXML_COMM_DATA_STRU;
typedef struct _SWCONFIGXML_MQTT_DATA_STRU
{
    QString id = "";
    QString clinet_id = "";
    QString topical = "";
    QString data_save = "";
    QString data_contain = "";
    QString timeout = "";

}SWCONFIGXML_MQTT_DATA_STRU;
typedef struct _SWCONFIGXML_SAVEPATH_STRU
{
    QString key = "";
    QString address = "";

}SWCONFIGXML_SAVEPATH_STRU;
typedef struct _SWCONFIGXML_SAVEPATH_MAP
{
    QVector<SWCONFIGXML_SAVEPATH_STRU> savePathVec;
    _SWCONFIGXML_SAVEPATH_MAP()
    {
        savePathVec.clear();
    }
}SWCONFIGXML_SAVEPATH_MAP;

typedef struct _SWCONFIGXML_UPLOAD_STRU: public SWCONFIGXML_BASE_STRU
{
    QString Id = "";
    QString dataRecovery = "";
    QVector<QString> dataSetKeyVec;
    _SWCONFIGXML_UPLOAD_STRU()
    {
        dataSetKeyVec.clear();
    }

}SWCONFIGXML_UPLOAD_STRU;
typedef struct _SWCONFIGXML_UPLOAD_MAP
{
    QMap<QString,SWCONFIGXML_UPLOAD_STRU> swXmlUpLoadMap;
    _SWCONFIGXML_UPLOAD_MAP()
    {
        swXmlUpLoadMap.clear();
    }
}SWCONFIGXML_UPLOAD_MAP;



typedef struct _SWCONFIGXML_COMM_STRU
{
    QMap<QString,SWCONFIGXML_COMM_DATA_STRU> commDataMap;

}SWCONFIGXML_COMM_STRU;

//软件配置快速定位轴信息部分结构体
typedef struct _AXISINFO_STRU
{
    float speed = -1;
    double pos  = -1.0;
    QString offsetPos = "0";

}AXISINFO_STRU;

//软件配置（扭矩）轴信息部分结构体　　　　write by daiqwie 2017-06-06
typedef struct _TORQUE_INFO_STRU
{
    int speed = -1;
    double torque_value  = -1.0;

}TORQUE_INFO_STRU;

typedef struct _BIT_VALUE_STRU
{
    uchar bit = 0;
    uchar value  = 0;

}BIT_VALUE_STRU;
typedef struct _SON_STRU
{
    QVector<BIT_VALUE_STRU> OutPutVec;
    QVector<BIT_VALUE_STRU> InPutVec;
    _SON_STRU()
    {
        OutPutVec.clear();
        InPutVec.clear();
    }

}SON_STRU;
typedef struct _SW_JS_OFFSETOBJECT_STRU
{
    QString id = "";
    QString type = "";
}SW_JS_OFFSETOBJECT_STRU;

typedef struct _SWCONFIGXML_JS_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString funtionName = "";
    QString savePathKey = "";
    SW_JS_OFFSETOBJECT_STRU offsetObject;

}SWCONFIGXML_JS_STRU;
//软件配置快速定位部分的结构体
typedef struct _SWCONFIGXML_QPOSIT_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString note = "";
    int speed = -1;
    int acc = -1;
    int dec = -1;
    int timeOut = -1;
    int delayMsec = -1;
    int axisNum = -1;
    QMap<QString ,AXISINFO_STRU> axisMap;
    _SWCONFIGXML_QPOSIT_STRU()
    {
        axisMap.clear();
    }
}SWCONFIGXML_QPOSIT_STRU;

//软件配置扭矩模式的结构体　　write by daiqiwei 2017-06-06
typedef struct _SWCONFIGXML_QTORQUE_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString note = "";
    int speed = -1;
    int acc = -1;
    int dec = -1;
    int timeOut = -1;
    int delayMsec = -1;
    int axisNum = -1;
    QMap<QString ,TORQUE_INFO_STRU> torqueMap;
    _SWCONFIGXML_QTORQUE_STRU()
    {
        torqueMap.clear();
    }
}SWCONFIGXML_QTORQUE_STRU;

typedef struct _SWCONFIGXML_ORG_AXIS_STRU
{
    QString axisId = "";
    QString orgmode= "1";// 新增模式节点 ，默认值为1 可以配置模式2 2018 01/23
    uint S1 = 0;
    uint S2 = 0;
    uchar direction = 0;
    int directTime = -1;
    ushort orgmDelay = 0;
    ushort orgTimeout = 0;

}SWCONFIGXML_ORG_AXIS_STRU;

typedef struct _SWCONFIGXML_ORG_OP_STRU: public SWCONFIGXML_BASE_STRU
{
    QString orgID = "";
    ushort utimeout = 0;
    QVector<QString> swXml_ORG_AxisId_Vec;
    _SWCONFIGXML_ORG_OP_STRU()
    {
        swXml_ORG_AxisId_Vec.clear();
    }
}SWCONFIGXML_ORG_OP_STRU;
typedef struct _SWCONFIGXML_ORG_STRU
{
    QMap<QString,SWCONFIGXML_ORG_OP_STRU> swXml_ORG_OP_Map;
    QMap<QString,SWCONFIGXML_ORG_AXIS_STRU> swXml_ORG_Axis_Map;
    _SWCONFIGXML_ORG_STRU()
    {
        swXml_ORG_OP_Map.clear();
        swXml_ORG_Axis_Map.clear();
    }
}SWCONFIGXML_ORG_STRU;

typedef struct _INOUTPUT_STRU: public SWCONFIGXML_BASE_STRU
{
    QString bitName = "";
    int stat = -1;
}INOUTPUT_STRU;
//------自定义报警--------
typedef struct _DIYLOG_STRU
{
    QString ErrStr = "0";
    QString log ="";
}DIYLOG_STRU;
typedef struct _MOVE_VAR_VALUE{
    QString Variable = "";
    QString Value = "";

}MOVE_VAR_VALUE;

//软件配置直线插补模式的结构体－－－－－－－－－－－－－－－－－－－
//软件配置中直线插补轴的ＩＤ的目标位置结构体
typedef struct  _LCSP_CPOSINFO_STRU{
    double pos = 0;
}LCSP_CPOSINFO_STRU;
//软件配置中直线插补触发位置信息的结构体
typedef struct _LCSP_TRIGGER_CPOS_STRU{
    QString  id= "";
    double pos = 0;

}LCSP_TRIGGER_CPOS_STRU;
//软件配置中触发信息的结构体
typedef struct _LCSP_TRIGGERINFO_STRU{
    QString bitName = "";
    int stat = -1;
    QVector<LCSP_TRIGGER_CPOS_STRU> LCSPTrigCPos;
    _LCSP_TRIGGERINFO_STRU()
    {
        LCSPTrigCPos.clear();
    }

}LCSP_TRIGGERINFO_STRU;
//软件配置直线插补直线位置轴结构体
typedef struct _LCSP_CSYSINFO_STRU{
    QVector<LCSP_TRIGGERINFO_STRU> LCSPTrigInfo;
    QMap<QString ,LCSP_CPOSINFO_STRU> LCSCPosMap;
    _LCSP_CSYSINFO_STRU(){
        LCSPTrigInfo.clear();
        LCSCPosMap.clear();
    }
}LCSP_CSYSINFO_STRU;
typedef struct _SWCONFIGXML_LCSP_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString note = "";
    int speed = -1;
    int acc = -1;
    int timeOut = -1;
    int delayMsec = -1;
    QVector<LCSP_CSYSINFO_STRU> LCSPCPos;
    QMap<QString ,double> LCSPOffsetPosMap;//增加坐标偏移位置--------2017-11-24
    _SWCONFIGXML_LCSP_STRU()
    {
        LCSPCPos.clear();
    }
}SWCONFIGXML_LCSP_STRU;

//直线插补－－－－－－－－－－－－－－－－－－－－－－－－－－－－
//-----------------JOG----start----------
typedef struct _JOGAxisInfo_STRU
{
    QString axisId = "";
    float speed = 1;
    INOUTPUT_STRU bitStru;
}JOGAxisInfo_STRU;

typedef struct _SWCONFIGXML_JOG_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString note = "";
    int acc = 1;
    int dec = 1;
    int timeOut = -1;
    int delayMsec = -1;
    QString strDirection = "ADD";
    int MaxPos=0;
    QMap<QString,JOGAxisInfo_STRU> jogAxisInfoMap;
}SWCONFIGXML_JOG_STRU;
//-----------------JOG----end-----------
//-------------条件语句-----start-------by sunny

typedef struct _IFInfo_STRU
{
    QString id = "";
    QString type = "";
    QString strEvent = "";
}IFInfo_STRU;

typedef struct _TIMERInfo_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QString note = "";
    QString status = "start";
}TIMERInfo_STRU;
//-------------条件语句------end------by sunny

//双启动数据结构  --by 代其伟
typedef struct _DOUBLEBUTTON_STRU: public SWCONFIGXML_BASE_STRU
{
    QString id = "";
    QVector<INOUTPUT_STRU> doubleButtonInfoVec;
    _DOUBLEBUTTON_STRU()
    {
        doubleButtonInfoVec.clear();
    }
}DOUBLEBUTTON_STRU;

typedef struct _DOUBLEBUTTON_MAP
{
    QMap<QString,DOUBLEBUTTON_STRU> doubleButtonInfoMap;
    _DOUBLEBUTTON_MAP()
    {
        doubleButtonInfoMap.clear();
    }
}DOUBLEBUTTON_MAP;
//双启动数据结构

//WRITE BY DAI
typedef struct _SENSOR_LIGHT_STRU//处理节点:    <信号灯> 1</信号灯>
{
    QString name="";
    QVector<QString> light;

}SENSOR_LIGHT_STRU;

typedef struct _SWCONFIGXML_SAFEBITINFO_STRU
{
    INOUTPUT_STRU sensor_bit;
    QString info="";
}SWCONFIGXML_SAFEBITINFO_STRU;

typedef struct _SWCONFIGXML_SAFESTATE_STRU//按钮状态结构体
{
    QString  level=0;
    QVector<SWCONFIGXML_SAFEBITINFO_STRU> BitInfoVec;

}SWCONFIGXML_SAFESTATE_STRU;

typedef struct _SWCONFIGXML_SAFECON_MAP//安全配置总数据结构
{
    QMap<QString,INOUTPUT_STRU> swXml_THREE_SOFTWARE_Map;//三色灯软元件数据结构
    QMap<QString,SENSOR_LIGHT_STRU>  swXml_LIGHT_CONFIG_Map;//三色灯配置数据结构
    QMap<QString,QMap<QString,SWCONFIGXML_SAFESTATE_STRU>> swXml_SAFESTATE_Map;//按钮状态数据结构

    _SWCONFIGXML_SAFECON_MAP()
    {
        swXml_THREE_SOFTWARE_Map.clear();
        swXml_LIGHT_CONFIG_Map.clear();
        swXml_SAFESTATE_Map.clear();
    }
}SWCONFIGXML_SAFECON_MAP;
//WRITE BY DAI

/**********************************************/
//add by sylar.liu
//structure for pallet informaiton

struct Position
{
    double x, y, z;
};

struct PalletCommandParams
{
    QString name;
    QString param1;
    QString param2;
};

struct _RunRarams
{
    double timeout;
    double delay;
    double speed;
    double acc;
    double dec;
};


struct SignalSetting : public SWCONFIGXML_BASE_STRU
{
    QString bit = "";
    int value = -1;
};

struct PalletInfo
{
    QString palletName;
    QMap<QString, double> palletDimension;
//    QMap<QString, Position> palletCheckpoint;
    QMap<QString, QString> palletCheckpoint;
    QMap<QString, int> palletMode;
    QMap<QString, _RunRarams> runParams;
    QMap<QString, QString> coordinateToAxis;
    QMap<QString, SignalSetting> endSignal;
    QMap<QString, SignalSetting> resetSignal;
};

/**********************************************/

/**********************************************/
//to do better

struct _Posture
{
    double yaw, pitch, roll;
};

struct _Pose
{
    double x, y, z;
    double yaw, pitch, roll;
};

struct _SegmentMode_1
{
    QString file;
    QString start_teach_point;
};

struct _SegmentMode_2
{
    QMap<int, QString> teach_points;
};

struct _SegmentInfo
{
    int id;
    QString type;
    int mode;
    _SegmentMode_1 segment_mode_1;
    _SegmentMode_2 segment_mode_2;
};

struct _ProfileInfo
{
    QString id;
    int mode;
    int num;    // 路径模式１和模式２，num表示路径中段的数量;路径模式3，num表示示教点的数量
    QMap<int, _SegmentInfo> segments;
    QMap<int, QString> points;
};

struct _TransRot
{
    double x_trans, y_trans, z_trans;
    double x_rot, y_rot, z_rot;
};

struct _MotionInfo  // 点胶信息
{
    QString id;
    QMap<QString, QString> axises_id;   // 每个方向对应的轴ID
    _TransRot trans_rot_1;   // 基坐标系到工作台坐标系的变换
    _TransRot trans_rot_2;   // 工作台坐标系到工件坐标系的变换
    QMap<QString, _ProfileInfo > all_profile_info;
    double tool_projection_length;   // 胶筒在平面的投影长度
    double tool_init_degree;    // 工具起始状态下和x轴正方向的角度（以角度为单位）
    QString camera_calibration_file;
};

/**********************************************/

typedef struct _SWCONFIGXML_T_STRU: public SWCONFIGXML_BASE_STRU
{
    int iTimeOut = 0;

}SWCONFIGXML_T_STRU;
typedef struct _SWCONFIGXML_CT_STRU: public SWCONFIGXML_BASE_STRU
{
    int iAddCount = 0;

}SWCONFIGXML_CT_STRU;

typedef struct _SWCONFIGXML_WAIT_STRU: public SWCONFIGXML_BASE_STRU
{
    QString Id = "";
    QString strEvent = "";
    QString strError = "";
    ushort uTimeout = 0;

}SWCONFIGXML_WAIT_STRU;

typedef struct _PROTECT_EVENT_STRU
{
    QString id = "";
    QString strEvent = "";
    QString strNote = "";

}PROTECT_EVENT_STRU;
typedef struct _PROTECT_EVENT_MAP
{
    QMap<QString,PROTECT_EVENT_STRU> axisEventMap;
    QMap<QString,PROTECT_EVENT_STRU> coordEventMap;

}PROTECT_EVENT_MAP;

typedef struct _SWCONFIGXML_SER_MAP
{
    QMap<QString,SWCONFIGXML_QPOSIT_STRU> swXml_QP_Map;
    QMap<QString,SWCONFIGXML_LCSP_STRU> swXml_LCSP_Map;
    QMap<QString,SWCONFIGXML_QTORQUE_STRU> swXml_QTorque_Map;   //write by daiqiwei 2017-06-06
    QMap<QString,SWCONFIGXML_JOG_STRU> swXml_JOG_Map;
    SWCONFIGXML_ORG_STRU swXml_ORG_Map;
    QMap<QString,SWCONFIGXML_JS_STRU> swXml_JS_Map;
    SON_STRU son_Stru;
    _SWCONFIGXML_SER_MAP()
    {
        swXml_QP_Map.clear();
        swXml_JS_Map.clear();
    }
}SWCONFIGXML_SER_MAP;

//软件配置部分XML内存中的数据结构
typedef struct _SWCONFIGXML_MAP
{
    QMap<QString,SWCONFIGXML_CY_STRU> swXml_CY_Map;
    QMap<QString,SWCONFIGXML_COMM_STRU> swXml_COMM_Map;
    QMap<QString,SWCONFIGXML_MQTT_DATA_STRU> swXml_MQTT_Map;
    SWCONFIGXML_SAVEPATH_MAP swXml_SavePath_Map;
    QMap<QString,IFInfo_STRU> swXml_IF_Map;
    QMap<QString,TIMERInfo_STRU> swXml_TIMER_Map;
    SWCONFIGXML_UPLOAD_MAP swXml_Upload_Map;
    PROTECT_EVENT_MAP swXml_ProtectEvent_Map;
    SWCONFIGXML_SER_MAP swXml_SER_Map;
    DOUBLEBUTTON_MAP	swXml_DOUBLEBUTTON_Map;//双按钮启动  by --dai
    SWCONFIGXML_SAFECON_MAP swXml_SAFETYCONFIG_Map;//安全配置 by --dai

    QMap<QString, PalletInfo> swXml_PalletInfo; // add by sylar.liu, for pallet.
    _MotionInfo swXml_MotionInfo; // add by sylar.liu, for motion control.


    QMap<QString,SWCONFIGXML_WAIT_STRU> swXml_Wait_Map;
    _SWCONFIGXML_MAP()
    {
        swXml_CY_Map.clear();
    }

}SWCONFIGXML_MAP;


//软件配置部分XML结构体
typedef struct _SWCONFIGXML_STRU
{
    SWCONFIGXML_CY_STRU swXml_CY_Stru;
    SWCONFIGXML_QPOSIT_STRU swXml_QP_Stru;
    SWCONFIGXML_QTORQUE_STRU swXml_TQ_Stru;
    SWCONFIGXML_LCSP_STRU swXml_LCSP_Stru;//增加直线插补
    INOUTPUT_STRU IO_stru;
    QString iTimeOut = "0";
    QString iAddCount = 0;
    DIYLOG_STRU DIY_Info;
    TIMERInfo_STRU swXml_TI_Stru;
    SWCONFIGXML_WAIT_STRU swXml_WAIT_Stru;
    SWCONFIGXML_JOG_STRU swXml_JOG_Stru;
    HWCONFIGXML_IO_STRU   hwXml_IO_Stru;
    SWCONFIGXML_ORG_OP_STRU swXml_ORG_Stru;
    DOUBLEBUTTON_STRU swXml_DB_Stru;
    SWCONFIGXML_COMM_DATA_STRU swXml_COMM_data;
    SWCONFIGXML_MQTT_DATA_STRU swXml_MQTT_data;
    SWCONFIGXML_JS_STRU swXml_JS_Stru;
    SWCONFIGXML_UPLOAD_STRU swXml_Upload_Stru;

    //add by sylar.liu, for pallet
    PalletCommandParams palletCmdParam;

    // add by sylar.liu, for dispensing
    PalletCommandParams dispensingParam;
    MOVE_VAR_VALUE move_var;

}SWCONFIGXML_STRU;

//流程配置部分TXT的结构体
typedef struct _FLOWTXT_STRU
{
    QString command = "";
    QString id = "";
    QString istatus = "";
}FLOWTXT_STRU;

typedef struct _FLOW_CMD_STRU
{
    QMap<int,QMap<QString,FLOWTXT_STRU>> flowCmdMap;
    QMap<int,QMap<QString,FLOWTXT_STRU>> ResetCmdMap;
    QMap<QString,QMap<int,QMap<QString,FLOWTXT_STRU>>> threadCmdMap;
}FLOW_CMD_STRU;
//流程配置部分TXT内存中的数据结构
typedef struct _FLOWTXT_MAP
{
    _FLOWTXT_MAP()
    {
        //flowXmlMap.clear();
        //subflowXmlMap.clear();
        //childFlowXmlMap.clear();
        FlowMap.clear();
    }

    QMap<QString,FLOW_CMD_STRU> FlowMap;
    QMap<QString,FLOW_CMD_STRU> autoFlowMap;
    QMap<int,QMap<QString,FLOWTXT_STRU>> reSetFlowMap;
    QMap<QString,QMap<int,QMap<QString,FLOWTXT_STRU>>> chlidResetXmlMap;
    QMap<int,QMap<QString,FLOWTXT_STRU>> ORGFlowMap;
    QMap<int,QMap<QString,FLOWTXT_STRU>> flowXmlMap;
    QMap<int,QMap<QString,FLOWTXT_STRU>> subflowXmlMap;
    QMap<QString,QMap<int,QMap<QString,FLOWTXT_STRU>>> childFlowXmlMap;
}FLOWTXT_MAP;

//所有用户配置的内存中的数据结构
typedef struct _XmlStru
{
    UIXML_Map uiXml;
    SWCONFIGXML_MAP swConfigXml;
    FLOWTXT_MAP flowTxt;
    HWCONFIGXML_MAP hwConfigXml;

}XmlStru;

#endif // COMM_H
