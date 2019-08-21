#ifndef HARDWAREFROM_H
#define HARDWAREFROM_H

#include "hardwarefrom_global.h"
#include <QWidget>
#include <QObject>
#include <QScrollArea>
#include <QDesktopWidget>
#include <QApplication>
#include <QTreeView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QStandardItem>
#include <QStandardItemModel>
#include "addaxisfrom.h"
#include "xmlinfosave.h"
#include "addiofrom.h"
#include "addtcpfrom.h"
#include "addserialport.h"
#include <QNetworkInterface>
#include "addarm.h"
#include "addcoordinateui.h"
#include "addcoordinate.h"
#include <QPixmap>
#include <QHeaderView>
#include <QFile>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
//#include "XmlManager/xmlmanager.h"
#include <DycLibBase/XmlManager/xmlmanager.h>
enum MenuClickState
{
    HardeWare,
    Io,
    Axis,
    TcpIp,
    Serialport,
    AxisIndex,
    Other
};
typedef struct TREEVIEWSTATE
{
    int row = -1;
    QString atname = "";
}TREEVIEWSTATE;
class HARDWAREFROMSHARED_EXPORT Hardwarefrom: public QWidget
{
    Q_OBJECT
public:
    Hardwarefrom();
    ~Hardwarefrom();
    QWidget * GetAttrArea()
    {
        mp_scrollarea = NULL;
        mp_scrollarea = new QScrollArea();
        return mp_scrollarea;
    }

public:
        QScrollArea *mp_scrollarea;
        QString m_path;
private:

    QScrollArea *mp_scrollarea_arm;
    QTreeView *p_TreeView;

    QStandardItemModel *model;

    QStandardItem *p_tcp;   //toltel
    QStandardItem *p_server;
    QStandardItem *p_cline;

    QStandardItem *p_sreial;
    QStandardItem *p_other;
    QStandardItem *p_arm;
    MenuClickState click_state;
    TREEVIEWSTATE m_treeviewstate;   // Treeview index
   XMLInfoSave saveaxisinfo;
   QVBoxLayout *m_pscrollVbox;
   int m_higth = 0;
   QLineEdit *p_mNetName;
   QLabel *p_mSaveTips;
private:
    QMenu* CreatMenu();
    QMenu* CreatMenu_subitem();
    void ShowContextMenu(const QPoint point);
    void treeview_click(const QModelIndex & index);
    void AddAxis();
    bool HandleAddAxis(HWCONFIGXML_SEROUTPUT_STRU axisinfo);
    bool HandleAddAxisFix(HWCONFIGXML_SEROUTPUT_STRU axisinfo);
    void IsertAxis();
    void deleteAxis();

    bool HandleAddSerialPort(HWCONFIGXML_COMPORT_STRU serialinfo);
    bool HandleAddSerialPortFix(HWCONFIGXML_COMPORT_STRU serialinfo);

private slots:
    void AddIo();
    void AddIo_Tofrom(HWCONFIGXML_IO_STRU IoParamter);
    bool HandleIo(HWCONFIGXML_IO_STRU ioinfo);
    bool HandleIo(HWCONFIGXML_IO_STRU pre,HWCONFIGXML_IO_STRU now);
    bool HandleIoFix(HWCONFIGXML_IO_STRU ioinfo);//tree not add just fix
    void IsertIO();

    bool HandleAddtcp(HWCONFIGXML_INTNET_STRU TcpIp);
    bool HandleAddtcpFix(HWCONFIGXML_INTNET_STRU TcpIp);

    bool handleAddcoordUI(HWCONFIGXML_ARMINFO_STRU arm);
    bool handleAddcoordUIFix(HWCONFIGXML_ARMINFO_STRU arm);

    void AddCoorUI();
    void AcoorInUI(COORDINATESTUR CoorInfo);
    void FixCoordinate(QString CoorName);

    //处理插入修改硬件是的保存xml
    bool SaveHardwareInfo();
    void SaveNetName();

public:
    QString getHostIpAddress();
    QMap<QString,int>GetIoInfo();
    QVector<QString>GetAxisInfo();
    void CreatXmlRoot();
    bool LoadHwXml(QString path);

private:
    static QMap<QString,HWCONFIGXML_IO_STRU>IoMap;
    QMap<QString,HWCONFIGXML_INTNET_STRU>TcpMap;
    QString Key;
     QMap<QString,HWCONFIGXML_COMPORT_STRU>SerialMap;
    static QMap<QString,HWCONFIGXML_SEROUTPUT_STRU>AxisMap;
    QMap<QString,HWCONFIGXML_ARMINFO_STRU>ArmMap;
    QVector<COORDINATESTUR>CoorVec;
    QMap<QString,COORDINATESTUR>CoorMap;
    bool AxisIsert;
    bool IoIsert;


};

#endif // HARDWAREFROM_H
