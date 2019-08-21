#ifndef CHECKABBITTHREAD_H
#define CHECKABBITTHREAD_H
#include <QThread>
#define TIMEOUT 500

class CheckABbitThread:public QObject
{
    Q_OBJECT
public:
    CheckABbitThread();
    enum RunDir{
        FWD = 1,//电机正转
        REV = 0,//电机反转
        ZER = 99
    };
public:
    static CheckABbitThread * GetInstance();
    ~CheckABbitThread();
    void SetParam(QString AIOstr,QString BIOstr);
    void Stop();
    void Reset();
    int GetCounterValue(){
        return m_counter;

}
private:
    char GetBitValue(QString IOstr);

public:
    void Get_check_value();
    bool GetValue_falg;
    void StopWhile();
private:
   int m_counter;
   bool m_exit;
   RunDir m_rundir;
   QString m_AbitIO;
   QString m_BbitIO;
   int m_motion_value;
   float m_convertime;
   bool m_check_over;
   //int register a;

};

#endif // CHECKABBITTHREAD_H
