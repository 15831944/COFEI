#ifndef ADDARM_H
#define ADDARM_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Comm.h>
class AddArm : public QWidget
{
    Q_OBJECT
public:
    explicit AddArm(QWidget *parent = 0);

signals:
    void Submit_armInfo(HWCONFIGXML_ARMINFO_STRU);
private:
    QPushButton *p_OkBut;
    QPushButton *p_NoBut;
    QLineEdit *p_IdName;
    QSpinBox *p_ArmLenght;
    QSpinBox *p_Decelerationratio;
    QSpinBox *p_ElectronicRatio;
private:
    void innitUI();
    void MakeSure();
    HWCONFIGXML_ARMINFO_STRU GetArmParameter();
public:
    void SetArmParameter(HWCONFIGXML_ARMINFO_STRU arm);

public slots:
};

#endif // ADDARM_H
