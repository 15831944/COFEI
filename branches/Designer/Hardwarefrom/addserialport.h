#ifndef ADDSERIALPORT_H
#define ADDSERIALPORT_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Comm.h>
class AddSerialPort : public QWidget
{
    Q_OBJECT
public:
    explicit AddSerialPort(QWidget *parent = 0);
private:
    QLineEdit* p_id;
    QLineEdit* p_serialname;
    QLineEdit* p_headendname;
  //  QLineEdit* p_contain;
    QComboBox* p_buadrate;
//    QLineEdit* p_stop;
//    QLineEdit* p_stopchar;
    QComboBox* p_NunChar;
//    QLineEdit* p_sendpath;
    QComboBox* p_Paritycheck;
//    QLineEdit* p_savepath;
    QPushButton* p_Okbut;
    QPushButton* p_cleanbut;
private:
    void initUI();
    HWCONFIGXML_COMPORT_STRU GetSerialPortParameter();
    void MakeSure();
public:
    void SetSerialPortParameter(HWCONFIGXML_COMPORT_STRU serial);
signals:
    void Conmit_serialport(HWCONFIGXML_COMPORT_STRU);
public slots:
};

#endif // ADDSERIALPORT_H
