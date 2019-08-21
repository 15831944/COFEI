/************************************
* @brief 摘要
* @author sunny
* @date 2017/05/23
* 该类是安全线程类，基类是QThread，采用了单例模式
* 重载run实现实时采集安全配置的IO，并根据不同的等级实现三色灯的闪烁
************************************/
#ifndef SAFETHREAD_H
#define SAFETHREAD_H
#include <QThread>
#include <MyGlobal.h>
#include <QTimer>
#include <QMutex>
class SafeThread : public QThread
{
    Q_OBJECT
private:
    SafeThread();
public:
    static SafeThread * GetInstance();
    void CloseInstance();
    void InitSafeVariable();
    void SetLightStatus(int);
    int  GetAlarmLevel(){return m_AlarmLevel;}
    int  GetFlowAlarmLevel(){return m_FlowAlarmLevel;}
    void SetFlowAlarmLevel(int Level);
private:
    static void SetNorLightIOValue(QString ,int);
    void ClearIOValue();
protected:
  void run();
private:
  static void   SetTimerForLight();
  void          AnalysisSafeData();
  QString       GetSafeState(int itype);
signals:
  // param: 流程名称;信号等级标志;输出状态信息
  void SafeSignals(const QString , const QString,const QString);
public:
  bool m_ExitFlag;      //退出while循环标志
  bool m_StopFlag;      //停止标志
  bool m_EmStopFlag;    //急停标志
  bool m_ResetFlag;     //复位标志
  bool m_StartFlag;
  //<flowName,<IO_Type,State>>子流程 IO 状态:EmStop Stop Reset Start
  QMap<QString,QMap<QString,bool>> m_FlowIOStateMap;
private:
  typedef struct _SAFEINFO_STRU
  {
      int ilevel;
      QString info="";
  }SAFEINFO_STRU;
  QMap<int,QMap<QString,QString>> m_FlowFlagMap;

  static SafeThread *INSTANCE;      //类的静态指针
  int m_AlarmLevel;       //IO警示等级:1 急停;2 光栅;3 停止;4 复位;5 启动;6 回原点
  int m_FlowAlarmLevel;   //流程等级:3 停止;4 复位;6 回原点;15 Error;16 Ready; 17 Runing
  static QMap<int,SENSOR_LIGHT_STRU> m_LightMap;
  static QTimer *m_LightTimer;
  static bool m_ispangle;
  QMap<int,QMap<QString,QVector<SWCONFIGXML_SAFEBITINFO_STRU>>> m_LevelMap;
  static QMutex *m_Mutex;

};

#endif // SAFETHREAD_H
