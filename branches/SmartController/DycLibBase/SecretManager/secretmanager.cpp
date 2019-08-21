#include "secretmanager.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <string>
#include <fstream>
#include <QSettings>
#include <QtGui>
#include "aeshelper.h"
//#ifdef __GNUC__
//#include <cpuid.h>
//#elif defined(_MSC_VER)
//#if _MSC_VER >= 1400
//#include <intrin.h>
//#endif
#include <cpuid.h>
QString HWManager::GetCpuId()
{
    QString cpuid = "";
    unsigned int dwBuf[4];
    int InfoType = 1;
    __cpuid(InfoType, dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);
    __cpuid_count(InfoType, 0, dwBuf[0],dwBuf[1],dwBuf[2],dwBuf[3]);
    cpuid = QString::number(dwBuf[3], 16).toUpper();
    cpuid = cpuid + QString::number(dwBuf[0], 16).toUpper();
    return cpuid;
}
bool HWManager::GetDiskName(std::string &diskname)
{
    diskname.clear();

    std::ifstream ifs("/etc/mtab", std::ios::binary);
    if (!ifs.is_open())
    {
        return false;
    }

    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }

        const char * disk = line;
        while (isspace(disk[0]))
        {
            ++disk;
        }

        const char * space = strchr(disk, ' ');
        if (NULL == space)
        {
            continue;
        }

        const char *mount = space + 1;
        while (isspace(mount[0]))
        {
            ++mount;
        }
        if ('/' != mount[0] || ' ' != mount[1])
        {
            continue;
        }

        while (space > disk && isdigit(space[-1]))
        {
            --space;
        }

        if (space > disk)
        {
            std::string(disk, space).swap(diskname);
            break;
        }
    }

    ifs.close();

    return(!diskname.empty());
}
void HWManager::TrimSerial(const void * serial, size_t seriallen, std::string & serialno)
{
    const char * pserials = static_cast<const char *>(serial);
    const char * pseriale = pserials + seriallen;
    while (pserials < pseriale)
    {
        if (isspace(pserials[0]))
        {
            ++pserials;
        }
        else if ('\0' == pseriale[-1] || isspace(pseriale[-1]))
        {
            --pseriale;
        }
        else
        {
            break;
        }
    }

    if (pserials < pseriale)
    {
        std::string(pserials, pseriale).swap(serialno);
    }
}
bool HWManager::GetDiskSerialNum(const std::string &diskname, std::string &serialno)
{
    serialno.clear();

    int fd = open(diskname.c_str(), O_RDONLY);
    if (-1 == fd)
    {
        return false;
    }

    struct hd_driveid drive ;
    if (0 == ioctl(fd, HDIO_GET_IDENTITY, &drive))
    {
        TrimSerial(drive.serial_no, sizeof(drive.serial_no), serialno);
    }

    close(fd);

    return(!serialno.empty());
}
QString HWManager::GetHdiskId()
{
    std::string diskname;
    if (GetDiskName(diskname))
    {
        std::string serialno;
        GetDiskSerialNum(diskname, serialno);
        return QString(serialno.c_str());
    }
    return NULL;
}
SecretManager::SecretManager()
{
    m_stKey = "R&D";
}
///
/// \brief SecretManager::GetMeCode
/// \return 机器码
///
QString SecretManager::GetMeCode()
{
    QString strMe = HWManager::GetCpuId() + "/***/" + HWManager::GetHdiskId();
    QByteArray ba = strMe.toLatin1();
    QString strTemp = ba.toBase64();
    return strTemp;
}

QString SecretManager::aesEncrypt(QString mingwen)
{
    AesHelper aeshelper;
    return aeshelper.aesEncrypt(mingwen);
}

QString SecretManager::aesUncrypt(QString miwen)
{
    AesHelper aeshelper;
    return aeshelper.aesUncrypt(miwen);
}
///
/// \brief SecretManager::GetEnCode
/// \param strKey  机器码
/// \return 加密码
///
QString SecretManager::GetEnCode(const QString &strKey)
{
    return CreateEnCode(strKey);
}
///
/// \brief SecretManager::CreateEnCode
/// \param strKey 机器码
/// \return 加密码
///
QString SecretManager::CreateEnCode(const QString &strKey)
{
    QByteArray fba = strKey.toLatin1();
    QString tempKey = QByteArray::fromBase64(fba);
    if(!tempKey.contains("/***/"))
    {
        return "机器码不正确！";
    }

    QStringList spl = tempKey.split("/***/");
    QString strEn = spl[0] + m_stKey + spl[1];
    QByteArray ba = strEn.toLatin1();
    QByteArray bb = QCryptographicHash::hash(ba, QCryptographicHash::Md5 );
    QString strTemp = bb.toBase64();
    return strTemp;
}
///
/// \brief SecretManager::SetEnCode
/// \param str 加密码
///
void SecretManager::SetEnCode(const QString str)
{
    SaveEnCode(str);
}
///
/// \brief SecretManager::SaveEnCode
/// \param str 加密码
/// 保存
void SecretManager::SaveEnCode(const QString str)
{
    QSettings *configini = new QSettings("Cowain","MainUI");
    configini->setValue("Root/Passwd",str);
}
///
/// \brief SecretManager::GetMingCode
/// \return 明文
///计算明文
QString SecretManager::GetMingCode()
{
    QString strMe = GetMeCode();
    QString strEn = CreateEnCode(strMe);
    QByteArray ba = strEn.toLatin1();

    return QByteArray::fromBase64(ba);
}
///
/// \brief SecretManager::GetDeCode
/// \return
/// 解码
QString SecretManager::GetDeCode()
{
    QSettings *configini = new QSettings("Cowain","MainUI");
    QByteArray ba = configini->value("Root/Passwd").toByteArray();
    QString strTemp = QByteArray::fromBase64(ba);

    return strTemp;
}
QString SecretManager::GetDeCode(const QString &str)
{
    QByteArray ba = str.toLatin1();
    QString strTemp = QByteArray::fromBase64(ba);

    return strTemp;
}
///
/// \brief SecretManager::CheckKey
/// \return
///比较
bool SecretManager::CheckKey()
{
    if(GetMingCode().compare(GetDeCode()) != 0)
    {
        return false;
    }
    return true;
}
bool SecretManager::CheckKey(const QString &str)
{
    if(GetMingCode().compare(GetDeCode(str)) != 0)
    {
        return false;
    }
    return true;
}
