#ifndef ADDTCPFROM_H
#define ADDTCPFROM_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QObject>
//#include <QHostInfo>
#include <Comm.h>

class AddTcpFrom : public QWidget
{
    Q_OBJECT
public:
    explicit AddTcpFrom(QWidget *parent = 0);

private:
    void initUI();
    void Commite();
private:
    QPushButton * p_Okbtun;
    QPushButton * p_Nobtun;
    QLineEdit *ID;
    QLabel *p_PcNameLab;
    QLineEdit *p_PcNameLeit;

    QLabel *p_PortNameLab;
    QLineEdit *p_PortNameLeit;

//    QLabel *p_ContansLab;
//    QLineEdit *p_ContansLeit;

//    QLabel *p_TimeOutLab;
//    QLineEdit *p_TimeOutLeit;
    QVBoxLayout *vbox;
    QLabel *p_Discraption;

//    QLineEdit * p_SendAddress;
//    QLineEdit * p_RessiveAddress;

private:
    void IpLineEditClick(const QString &str);
public:
    void SetTcpParameter(HWCONFIGXML_INTNET_STRU tcp);
signals:
    void tcpparametar(HWCONFIGXML_INTNET_STRU);
};

#endif // ADDTCPFROM_H
