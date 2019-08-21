#ifndef CSPTRITABLE_H
#define CSPTRITABLE_H
#include <QWidget>
#include <QVector>
#include <Comm.h>
#include<QTableWidget>
#include <QObject>
#include <addtabel.h>
#include<QStringList>
#include <DycLibBase/Coordinate/coordinate.h>

class dialog: public QDialog
{
    Q_OBJECT
public:
    dialog(QVector<QString> listtext);
private:
    void createUI();
private:
    QString m_checkedid;
    QVector<QString> m_str;
    QButtonGroup *m_idgroupbtn;
    QLabel *m_showid;
    QPushButton *m_pok;
    QPushButton *m_pcleal;
    QLineEdit *m_line[3];
signals:
    void GetData(QString id,QVector<QString> text);
    //void buttonClicked(QAbstractButton*);
public slots:
   void onButtonClicked(QAbstractButton*);
   void sure();
};

class CSPTriTable: public QTableWidget
{
    Q_OBJECT
public:
    CSPTriTable(QWidget *parent,QVector<QString>axisid,int row);
private:
    Coordinate *m_xytoJ;
    QMenu *CreateContextMenu();
    QMenu *menu ;
    SWCONFIGXML_LCSP_STRU m_cspSWxml;
    QVector<LCSP_TRIGGERINFO_STRU> m_TriConf;
    int m_row;
    QVector<QString>m_axisid;
    dialog *m_adilog;
     QVector<QString> m_list;
    int m_maxnum;
    QString m_str;
    QMap<QString,double> m_frontpos;
     QMap<QString,double> m_curpos;
    QMap<QString,double> m_trik;
    QString m_ID;

private:
    void NewCSPIdCreateUI();
    void ExistCSPIdCreateUI();
    int Calposindex(double pos);
    int  GetMaxPosnum(QMap<QString, LCSP_CPOSINFO_STRU> map0, QMap<QString, LCSP_CPOSINFO_STRU> map1);
    bool Calculformula(QMap<QString, LCSP_CPOSINFO_STRU> map0, QMap<QString, LCSP_CPOSINFO_STRU> map1);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void AddPoint();
    void DeletePoint();
    void SaveTri();
    void CloseTri();
    void Dataupdate(QString id, QVector<QString> data);
    void CalTripos(QMap<QString ,LCSP_CPOSINFO_STRU>,QMap<QString,LCSP_CPOSINFO_STRU>);

signals:
    void CloseTw();
    void SaveTriInfo(QVector<LCSP_TRIGGERINFO_STRU> LCSPTriggerInfo);

};

#endif // CSPTRITABLE_H
