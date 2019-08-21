#ifndef ADDIOFROM_H
#define ADDIOFROM_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Comm.h>
class AddIoFrom : public QWidget
{
    Q_OBJECT
public:
    explicit AddIoFrom(QWidget *parent = 0);

private:
    QPushButton *p_OkBut;
    QPushButton *p_NoBut;
    QLineEdit *p_IdName;
    QLineEdit *p_TpyeLint;
    QSpinBox *p_charNum;
private:
    void initUI();
    void MakeSure();
    HWCONFIGXML_IO_STRU GetIoParameter();
    bool m_isAdd;
    HWCONFIGXML_IO_STRU m_preParam;
public:
    void SetIoParameter(HWCONFIGXML_IO_STRU Iostru);
signals:
    void Submit(HWCONFIGXML_IO_STRU iostru);
    void Modify(HWCONFIGXML_IO_STRU pre,HWCONFIGXML_IO_STRU now);

public slots:
};

#endif // ADDIOFROM_H
