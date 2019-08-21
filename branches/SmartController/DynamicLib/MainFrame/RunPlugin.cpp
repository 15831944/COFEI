#include "RunPlugin.h"
#include <DycLibBase/LogManager/logmanager.h>
#include <MyConfig_Shared>
#include "safethread.h"
RunPlugin::RunPlugin()
{

}

void RunPlugin::Exe(const QString strkey, QString strMsg, int &iresult, const FLOWTXT_STRU flowStru, FunModuleInterface* pfun)
{
    pfun->Initialization();
    SWCONFIGXML_STRU ps;
    QString strnote = "";
    QString str = QString("plugin %1 参数 is not find").arg(strkey);
    if(strkey.compare("G00") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(flowStru.id))
        {
            ps.swXml_QP_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[flowStru.id];
            strnote = ps.swXml_QP_Stru.note;
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("M00") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_CY_Map.contains(flowStru.id))
        {
            ps.swXml_CY_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_CY_Map[flowStru.id];
            ps.swXml_CY_Stru.io = flowStru.istatus.toInt();
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("T") == 0)
    {
        ps.iTimeOut = flowStru.id;
    }
    else if(strkey.compare("JOG") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_JOG_Map.contains(flowStru.id))
        {
            ps.swXml_JOG_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_JOG_Map[flowStru.id];
            strnote = ps.swXml_JOG_Stru.note;
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("G02") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(flowStru.id))
        {
            ps.swXml_QP_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[flowStru.id];
            strnote = ps.swXml_QP_Stru.note;
        }
        else
           iresult = -101;

    }
    else if(strkey.compare("ORG") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_OP_Map.contains(flowStru.id))
        {
            ps.swXml_ORG_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_OP_Map[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("G101") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QTorque_Map.contains(flowStru.id))
        {
            ps.swXml_TQ_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QTorque_Map[flowStru.id];
            strnote = ps.swXml_TQ_Stru.note;
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("G01") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(flowStru.id))
        {
            if(!SafeThread::GetInstance()->m_StopFlag)
                ps.swXml_LCSP_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map[flowStru.id];
            else
            {
                str = QString("运行直线插补时急停，请复位后再启动!");
                iresult = -101;
            }
            strnote = ps.swXml_LCSP_Stru.note;
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("POST") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_Upload_Map.swXmlUpLoadMap.contains(flowStru.id))
        {
            ps.swXml_Upload_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_Upload_Map.swXmlUpLoadMap[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("DS") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_DOUBLEBUTTON_Map.doubleButtonInfoMap.contains(flowStru.id))
        {
            CoreLog::QLog_Info(defaultLogModule,strMsg);
            ps .swXml_DB_Stru= ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_DOUBLEBUTTON_Map.doubleButtonInfoMap[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("JS") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_JS_Map.contains(flowStru.id))
        {
            ps.swXml_JS_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_JS_Map[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("网络通信") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["网络通信"].commDataMap.contains(flowStru.id))
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["网络通信"].commDataMap[flowStru.id].opType = flowStru.command;
            ps.swXml_COMM_data = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["网络通信"].commDataMap[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("串口通信") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["串口通信"].commDataMap.contains(flowStru.id))
        {
            ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["串口通信"].commDataMap[flowStru.id].opType = flowStru.command;
            ps.swXml_COMM_data = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_COMM_Map["串口通信"].commDataMap[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("MQTT_W") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_MQTT_Map.contains(flowStru.id))
        {
            ps.swXml_MQTT_data = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_MQTT_Map[flowStru.id];
        }
        else
            iresult = -101;
    }

    /*add by sylar.liu, for pallet*/
    else if (strkey.compare("G03") == 0)
    {
        //指令中传递的参数
        ps.palletCmdParam.name = flowStru.command;
        ps.palletCmdParam.param1 = flowStru.id;
        if(flowStru.istatus.compare("TPV") == 0)
            ps.palletCmdParam.param2 = ConfigData::GetInstance()->m_tpPos;
        else
            ps.palletCmdParam.param2 = flowStru.istatus;
    }
    else if(strkey.compare("G04") == 0)
      {
          if(!SafeThread::GetInstance()->m_StopFlag)
          {
              ps.dispensingParam.name = flowStru.command;
              ps.dispensingParam.param1 = flowStru.id;
              ps.dispensingParam.param2 = flowStru.istatus;
          }
          else{
              str = QString("运行G04指令时急停，请复位后再启动!");
              iresult = -101;
          }
      }
    /*******************************/

    else if(strkey.compare("CNT") == 0)
    {
        ps.iAddCount = flowStru.id;
    }
    else if(strkey.compare("TPV") == 0)
    {
        ps.iAddCount = flowStru.id.toInt();
    }
    else if (strkey.compare("LOG") == 0)
    {
        ps.DIY_Info.ErrStr = flowStru.id;
        ps.DIY_Info.log = flowStru.istatus;
    }
    else if (strkey.compare("CYT") == 0)
    {
        ps.DIY_Info.ErrStr = flowStru.id;
        ps.DIY_Info.log = flowStru.istatus;
    }
    else if (strkey.compare("T01") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_TIMER_Map.contains(flowStru.id))
        {
            ps.swXml_TI_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_TIMER_Map[flowStru.id];
            ps.swXml_TI_Stru.status = flowStru.istatus;
        }
        else
           iresult = -101;
    }
    else if (strkey.compare("WAIT") == 0)
    {
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_Wait_Map.contains(flowStru.id))

        {
            ps.swXml_WAIT_Stru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_Wait_Map[flowStru.id];
        }
        else
           iresult = -101;
    }
    else if(strkey.compare("MOV") == 0 || strkey.compare("DMOV") == 0)
      {
          ps.move_var.Variable = flowStru.istatus;
          ps.move_var.Value = flowStru.id;
      }
// else if(strkey.compare("PY") == 0)
//    {
//        ps.py_file_name = flowStru.id;
//    }
    else if((strkey.compare("IN") == 0) || (strkey.compare("OUT") == 0)\
            || (strkey.compare("LIN") == 0) || (strkey.compare("LOUT") == 0))
    {
        ps.IO_stru.bitName = flowStru.id;
        ps.IO_stru.stat = flowStru.istatus.toInt();
    }
    else{
          iresult = -1;
          CoreLog::QLog_Error(defaultLogModule, strMsg +  " " + str);
          return;
    }
    if(iresult == -101)
    {
        CoreLog::QLog_Error(defaultLogModule, strMsg +  " " + str);
        return;
    }
    pfun->SetModuleParam(ps);
    pfun->RunModule();
    iresult = pfun->GetExcResult(str);
    pfun->StopModule();
    if(iresult >= 0)
    {
        if(iresult == 100)
        {
            CoreLog::QLog_Warning(defaultLogModule, strMsg +  strnote + " " + str);
        }
        else
        {
            if(ConfigData::GetInstance()->L_state == 1)
                CoreLog::QLog_Info(defaultLogModule,strMsg + strnote + " " + str);
        }
    }
    else
    {
        if(iresult != -99)
            CoreLog::QLog_Error(defaultLogModule, strMsg +  strnote + " " + str);
        else
            CoreLog::QLog_Warning(defaultLogModule, strMsg +  strnote + " " + str);
    }
}
