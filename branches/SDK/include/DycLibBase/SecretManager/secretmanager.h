/************************************
* @brief 摘要 加密管理类
* @author sunny
* @date 2017/05/25
************************************/
#ifndef SECRETMANAGER_H
#define SECRETMANAGER_H

#include "secretmanager_global.h"
#include <QString>
class HWManager
{
public:
    static QString GetCpuId();
    static QString GetHdiskId();
private:
    static bool GetDiskSerialNum(const std::string &diskname, std::string &serialno);
    static void TrimSerial(const void *serial, size_t seriallen, std::string &serialno);
    static bool GetDiskName(std::string &diskname);
};

class SECRETMANAGERSHARED_EXPORT SecretManager
{
public:
    SecretManager();
    QString GetEnCode(const QString &);
    void SetEnCode(const QString );
    bool CheckKey();
    bool CheckKey(const QString &);
    QString GetMeCode();
    QString aesEncrypt(QString mingwen);
    QString aesUncrypt(QString miwen);
private:
    QString m_stKey; //key值
private:
    QString GetDeCode();
    QString GetDeCode(const QString &);
    QString CreateEnCode(const QString &);
    void SaveEnCode(const QString );
    QString GetMingCode();

};

#endif // SECRETMANAGER_H
