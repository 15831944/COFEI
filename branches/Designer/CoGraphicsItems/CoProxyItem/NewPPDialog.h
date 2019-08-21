#ifndef NEWPPDIALOG_H
#define NEWPPDIALOG_H

#include <QDialog>
#include <Comm.h>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QSpinBox>
class NewPPDialog : public QDialog
{
    Q_OBJECT
public:
    NewPPDialog(QWidget *parent = 0);
    ~NewPPDialog(){}

signals:
    void SendPP(SWCONFIGXML_QPOSIT_STRU pos);

public:
    void SetHWParm(QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> HWConfig);
    void SetIsNew(bool isnew);

private slots:
    void ChangeType(const QString& text);
    void SetAutoCheck(int state);
    void OverAccept();
    void IDChanged();
private:
    bool CreateMessageBox(QString strTitle,QString note);
private:
    QWidget *wgCheck;
    QTableWidget *table;
    QLineEdit *lineEditId;
    QLineEdit *lineEditNote;
    QLineEdit *lineEditTimeout;
    QLineEdit *lineEditDelay;
    QLineEdit *lineEditDec;
    QLineEdit *lineEditAcc;
    QComboBox *cbb;
    QGridLayout *checkLayout;
    bool m_isNew;
private:
    QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> m_hwSerOutputMap; //硬件轴信息
};

#endif // NEWPPDIALOG_H
