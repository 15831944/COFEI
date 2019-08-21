#ifndef SMTCOMMLIB_H
#define SMTCOMMLIB_H

#include "smtcommlib_global.h"
#include <DycLibBase/Communication/communication.h>
class SMTCOMMLIBSHARED_EXPORT SMTCommLib :public QObject
{
    Q_OBJECT
private:
    SMTCommLib();
private:
    typedef struct _COMMINFO
    {
        _COMMINFO()
        {
            com = new Communication();
            oValue = 0;
            iValue = 0;
        }
        Communication *com ;
        ushort oValue;
        ushort iValue;
    }COMMINFO;
public:
    static SMTCommLib* GetInstance();
    bool OpenPort(QString strId,QString name,int BaudRate);
    void WriteData(QString strId,QByteArray buf);
    ushort CalculateIOVaule(QString strId,uchar ipos, ushort uc);
    QString ReadData();
private:
    QMap<QString,COMMINFO> m_CommInfoMap;
    QString m_readData;
private slots:
    void PortRead(QByteArray value);
};

#endif // SMTCOMMLIB_H
