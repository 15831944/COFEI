#ifndef STACKMANAGER_H
#define STACKMANAGER_H

#include "stackmanager_global.h"
#include <QStack>
class STACKMANAGERSHARED_EXPORT StackManager
{

public:
    StackManager();
    bool AnalysisEvent(const QString);
private:
    QStack<QChar> m_sign;
    QStack<QString> m_data;//栈
private:
    QString HandleData(const QString,const QString ,const QChar );
private:
    bool CompareData(double, double, const QChar);
    bool CompareData(const QString , const QString , const QChar );
    bool CompareSign(const QChar, int type = 1);
    bool SlectSaveAddress(const QString, const QString, const QChar sign);
    QString ReadTxt(QString path);
};

#endif // STACKMANAGER_H
