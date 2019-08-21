/************************************
* @brief 摘要 调试界面下选择轴、P的注释的界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef CHECKDLG_H
#define CHECKDLG_H

#include <QDialog>
#include <Comm.h>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
class CheckDlg : public QDialog
{
public:
    CheckDlg(QMap<QString,HWCONFIGXML_SEROUTPUT_STRU>,bool isNewPoint = true);
private:
    void CreateUI(int,int);
    bool CreateMessageBox(QString strTitle,QString note);
public:
    void resize(int ,int );
private slots:
    void OverAccept();
    void SetAutoCheckWg(int);
    void ChangeType(const QString &);
private:
    QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> m_hwSerOutputMap;
    QWidget *wgCheck;
    QTableWidget *table;
    QLineEdit *lineEditId;
    QLineEdit *lineEditNote;
    QLineEdit *lineEditTimeout;
    QLineEdit *lineEditDelay;
    QLineEdit *lineEditSpeed;
    QLineEdit *lineEditAcc;
    QLabel *lableSpeed;
    QLabel *lableAcc;
    QComboBox *cbb;
    QFont font;
    SWCONFIGXML_QPOSIT_STRU m_swqpStru;
    bool m_isNewPoint;
public:
    QString GetConfigID();
    QString GetConfigNote();
    QString GetConfigType();
    QString GetConfigTimeout();
    QString GetConfigDelay();
    QString GetConfigCspSpeed();
    QString GetConfigCspAcc();
    void SetQPStru(SWCONFIGXML_QPOSIT_STRU stru)
    {
        m_swqpStru = stru;
    }

    void GetCheckBox(QVector<QString>&);
    void SetCheckBox(QVector<QString>);
    void SetConfigID(QString);
    void SetConfigNote(QString);
    void SetConfigType(QString);
    void SetConfigTimeout(int );
    void SetConfigDelay(int );
    void SetConfigCspSpeed(int );
    void SetConfigCspAcc(int );
    void GetTableWidgetData(QMap<QString,int> &temp);
private:
    typedef struct _TABLEWGDATA
    {
        int ispeed = -1;
        bool ischecked = false;
    }TABLEWGDATA;
    void CreateTableHeard(QStringList HeaderList,int iwidth);
    void SetTableWidgetData(QString strId,bool isFixScl,TABLEWGDATA);
    void GetTableWidgetAxisData(QMap<QString,TABLEWGDATA> &temp);
};

#endif // CHECKDLG_H
