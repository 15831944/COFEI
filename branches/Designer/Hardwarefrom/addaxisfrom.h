#ifndef ADDAXISFROM_H
#define ADDAXISFROM_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Comm.h>
class AddAxisFrom : public QWidget
{
    Q_OBJECT
public:
    explicit AddAxisFrom(QWidget *parent = 0);
private:
    QPushButton *p_Okbutn;
    QPushButton *p_Cancelbut;

    QLineEdit *p_namelet;
    QSpinBox  *p_Limit_upVal;
    QSpinBox  *p_Limit_lowVal;
    QSpinBox  *p_AddVal;
    QSpinBox  *p_Offsict;
    QSpinBox  *p_Mode;
    QSpinBox *m_pDccAccValue;
    QLineEdit *p_Limit_upIo;
    QLineEdit *p_Limit_lowIo;
    QLineEdit *p_mOrg;
private:
    void innitUI();
    void MakeSure();
    HWCONFIGXML_SEROUTPUT_STRU GetAxisParameter();
   public:
    void SetAxisParameter(HWCONFIGXML_SEROUTPUT_STRU Axis);
private:
    HWCONFIGXML_SEROUTPUT_STRU AXIS_STRU;
signals:
    void Submit_axisinfo(HWCONFIGXML_SEROUTPUT_STRU);

public slots:
};

#endif // ADDAXISFROM_H
