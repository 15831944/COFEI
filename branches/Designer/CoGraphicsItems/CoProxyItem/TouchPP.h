#ifndef TOUCHPP_H
#define TOUCHPP_H
#include <CoGraphics/Graphics.h>
#include <CoGraphics/Scene/Scene.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QGraphicsGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTreeWidget>
#include <QGridLayout>
#include <QMenu>
#include <QPushButton>
class AxisOperater;
class HandMode;
class MyTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MyTree(QWidget *parent = Q_NULLPTR);
    ~MyTree(){}
signals:
    void        ItemClicking(QTreeWidgetItem *item, int column);
    void        UpdatePP();
    void        SavePP();
    void        AddPP();
    void        DeletePP();
protected:
     void       mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
     void       contextMenuEvent(QContextMenuEvent *event);
private:
     QMenu      *CreateContextMenu();

};
enum TouchMode{
    ppMode = 0,
    handMode
};
class TouchPP :public CoProxyWidget
{
    Q_OBJECT
public:
    TouchPP(double StartX, double StartY, double StopX, double StopY,
            const Qt::PenStyle &LineStyle, const int LineWidth, const QColor &LineColor,
            const QColor &BackColor, CoScene *Scene = NULL);
signals:
    void            SendUpdateXMLEvent();
    void            SendHandModeEvent(char mode); //0,退出手轮;1,handmode
    void            SendHandParamEvent(const QString param); //手轮模式参数
    void            SendHandModeState(const char state); // -1 驱动器故障；0 手轮停止；1 手轮运行
public slots:
    CoProxyWidget   *Copy();
    void            Copy(TouchPP *From);
    void            SetAttributeEvent(const int , const QString , uchar);

    void            TreeDoubleClicked(QTreeWidgetItem *item, int column);
    void            AddPP();
    void            AddPPToTree(SWCONFIGXML_QPOSIT_STRU pos);
    void            DeletePP();
    void            UpdatePP();
    void            SavePP();
    void            ButtonClicked();
private slots:
    void            UpdateSwSerXML();
private:
    MyTree          *CreateTreeWidget(QWidget *);
    QWidget         *CreateShowWidget(QWidget *);
    void            UpdateTree(QTreeWidget *);
private:
    MyTree          *m_treeWidget;
    QWidget         *m_showWidget;
    HandMode *m_handWidget;
    QGridLayout     *m_pGrid;
    QLabel          *m_posInfo;
    QTextEdit       *m_debugInfo;
    QPushButton     *m_runBtn;
    QPushButton     *m_stopBtn;
    char m_touchMode;


    QMap<QString,QMap<QString,QString>>     m_PPMap;
    FunModuleInterface                      *m_PPPlugin;
    QMap<QString,AxisOperater*>             m_AxisMap;
    QMap<QString,SWCONFIGXML_QPOSIT_STRU>   m_qpMap;
};

#endif // TOUCHPP_H
