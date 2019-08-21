#ifndef TESTPROPERTY_H
#define TESTPROPERTY_H

#include <QWidget>
#include "../libproperybase.h"
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class TestProperty : public QWidget ,public LibProperyBase
{

public:
    explicit TestProperty(QWidget *parent = 0);

signals:
private:
    QPushButton *p_OkBut;
    QPushButton *p_NoBut;
    QLineEdit *p_IdName;
    QSpinBox *p_ArmLenght;
    QSpinBox *p_Decelerationratio;
    QSpinBox *p_ElectronicRatio;
private:
    void innitUI();

public slots:
};

#endif // TESTPROPERTY_H
