#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QResizeEvent>
#include <QUndoStack>
#include <QTimer>
#include <CoGraphics/Scene/Scene.h>
#include <CoGraphics/Viewer/Viewer.h>
#include <CoGraphics/Base/Base.h>
#include <CoBase/CoAction/Action.h>
#include "Taskbar.h"
#include <CoWidgetBox/AttributeBox/AttributeBox.h>
#include <CoGraphicsItems/GraphicsItems.h>
#include <Hardware/HardwareFrom.h>
#include <QMessageBox>
#include <DatabaseLib/DataManager/DataManagerForm.h>
#include <DatabaseLib/DataManagerPermiss/dataManagerpermiss.h>
#include "AttributeData.h"
#include "CoGraphicsItems/AttributeWidget.h"
#include <CoGraphicsItems/FlowPropery/itemsave.h>
#include <CoGraphicsItems/XML/XmlWriter.h>
#include <CoGraphicsItems/XML/XmlReader.h>
#include <CoGraphicsItems/AttributeInstance.h>
#include <ec_control.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void    SendNewCanvasEnd(const QString&,int);
    void    SendNewProjectEnd(const QString&);
    void    SendDelCanvasEnd();
    void    SendDelProjectEnd();
    void    SendTreeCurItem(const QString,const int);
public slots:
    void    CreateNewCanvas(const QString&,const int);
    void    CreateNewProject();
    void    DeleteCanvas(const QString &Proname, const QString &Cvasname, const int);
    void    ChangeTreeNode(const QString &Proname, const QString &chlidname, int);
    void    DeleteProject(const QString&);
    void    RenameProject(const QString&,const QString&);
    void    RenameCanvas(const QString &, const QString&, const QString& , QString flowName);
    void    TaskBarItemClicked(CoAction *);
    void    HandleAttrHideEvent(bool);
    void    HandleTaskHideEvent(bool);
private slots:
    void    ViewClicked(int x, int y);
    void    ClickItemForAttrWidget(const QString&, const QString&, const int, const int, CoItemEx *);
    void    ClickWidgetForAttrWidget(const QString&,const QString&,const int,const int,CoProxyWidget*);
    void    DrawItem(double StartX, double StartY, double StopX, double StopY);
    void    ItemDrop(CoItem *Item, double StartDx, double StartDy, double StopDx, double StopDy);
    void    SwithScene(const QString&,const QString&);
    void    MoveItemList(CoScene *Scene, QList<CoItem *> ItemList,QList<CoProxyWidget*> WidgetList,
                            double StartX, double StartY, double StopX, double StopY);

    void    LineStyleChanged(int Style);
    void    LineWidthChanged(int Width);

    void    UpdateLineColor();
    void    UpdateBackColor();

    void    OpenProject();
    void    CreateFlowScene(const QString flowName,int flowtype,const QMap<QString,CoScene*> threads,int threadType);
    void    SaveXml();
    void    SaveXmlAs();

    void    SelectAllItems();
    void    CutItem();
    void    CopyItem();
    void    PasteItem();
    void    DeleteItem();

    void    MergeItems();
    void    SplitItems();
    void    AlignLeft();
    void    AlignRight();
    void    AlignTop();
    void    AlignBottom();
    void    AlignHWEqual();
    void    AlignHEqual();
    void    AlignWEqual();

    void    Up();
    void    Down();
    void    Left();
    void    Right();

    void    ZoomOut();
    void    ZoomIn();
    void    ZoomNormal();
    void    SetGridLine();
    void    SetDebugMode();
    void    SetHost();

    void    SetAttribute(uchar, int, const QString&);
    void    EnableEditAction();
    void    HandleAddDelEvent(int, QList<CoItem *>);
    void    ThreadReadXml();
    void    ShowLog(QString strmsg);
private:
    void    InitVariable();
    void    CreateActions();
    void    InitMenus();
    void    InitToolBars();
    void    InitStatusBars();
    QWidget *CreateTaskArea(QWidget * parent);
    QWidget *CreateShowArea(QWidget *parent);
    void    InitShowWidget();
    QWidget *CreateAttributeArea(QWidget *parent);
    TaskBar *CreateTaskBar(QWidget *parent);
    QTimer  *CreateTimer(QWidget *parent);
    void    ChangeShowWidget(int);

    CoScene *CreateNewScene(int SceneId, QString proName, QString Name,QColor BackColor,int SceneType,
                            double PosX = 0, double PosY = 0, double Width = 800, double Height = 600);
    void    SetCoScene(CoScene *Scene,int SceneType);
    void    AddItemToScene(CoItem *Item,CoProxyWidget *Widget);
    void    CopyItemListToScene(QList<CoItem *> &ItemList);
    CoItem  *CopyItem(CoItem *SourceItem);
    void    AddItemList(CoScene *Scene, QList<CoItem *> &ItemList, QList<CoProxyWidget *> WidgetList = QList<CoProxyWidget *>());
    void    SetItem(CoItem *Item);
    void    UpItems(CoItem *Item);
    void    MoveItems(double Dx1, double Dy1, double Dx2, double Dy2);
    void    SetAttrWidget(CoAttributeWidget *);
    CoAction *AddFileAction(QString _Name);
    CoAction *AddEditAction(QString _Name, QString _IconDir, QString _ShutCut);
    void    AddItemAttrToMap(CoItem *Item);
    void    DelItemAttrFromMap(CoItem *Item);
    bool    SaveToFile(const QString);
    bool    IsDirExist(QString fullPath);
    void    UpdateAfterOpenFile(const QString &);
    int     CreateMsgBox(QMessageBox::Icon, const QString, const QString,
                        QMessageBox::StandardButtons buttons = QMessageBox::NoButton );
    void    GetCurSceneItemId(CoScene*);
    void    OpenXml();
    bool    CreateCommunicator();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
private:
    Ui::MainWindow  *ui;
    bool            m_IsSaveXml;
public:
    QTimer          *m_pTimer;
    QGridLayout     *m_GridLayout;

    QLabel          *m_LabelLineWidth;
    QLabel          *m_LabelLineColor;
    QLabel          *m_LabelBackColor;

    int             m_SceneId;
    int             m_SceneHost;
    int             m_ItemId;
    CoScene         *m_pCurScene; // 当前场景
    CoView          *m_pView;
    Hardwarefrom    *m_hardView;
    DataManagerForm *m_pDataManagerView;
    /***********权限管理界面****/
    databasepermiss *m_pDataManagerPerView;

    int         m_zMax;// 置顶
    int         m_zMin;// 置底

    // 选择的按钮
    CoAction    *m_pDrawSelected;

    // 文件按钮
    CoAction    *m_ActFileNew;
    CoAction    *m_ActFileOpen;
    CoAction    *m_ActFileSave;
    CoAction    *m_ActFileSaveAs;
    CoAction    *m_ActFileClose;

    // 编辑
    CoAction    *m_ActEditAll;
    CoAction    *m_ActEditCut;
    CoAction    *m_ActEditCopy;
    CoAction    *m_ActEditPaste;
    CoAction    *m_ActEditDel;

    // 图形
    CoAction    *m_ActGraphMerge;
    CoAction    *m_ActGraphSplit;
    CoAction    *m_ActAlignTop;
    CoAction    *m_ActAlignLeft;
    CoAction    *m_ActAlignRight;
    CoAction    *m_ActAlignBottom;
    CoAction    *m_ActEditHWEqual;
    CoAction    *m_ActEditHEqual;
    CoAction    *m_ActEditWEqual;
    CoAction    *m_ActMoveUp;
    CoAction    *m_ActMoveDown;
    CoAction    *m_ActMoveLeft;
    CoAction    *m_ActMoveRight;

    // 撤销、重做
    QUndoStack  *m_UndoStack;
    QAction     *m_ActUndo;
    QAction     *m_ActRedo;

    // 窗口
    CoAction    *m_ActSetHost;
    CoAction    *m_ActZoomOut;
    CoAction    *m_ActZoomIn;
    CoAction    *m_ActZoomNormal;
    CoAction    *m_ActGrid;
    CoAction    *m_ActDebug;

    // 关于
    CoAction    *m_ActAbout;

    // 全局绘图属性
    Qt::PenStyle    m_LineStyle;
    int             m_LineWidth;
    QColor          m_LineColor;
    QColor          m_BackColor;
    QLabel          *m_Note;
    // 剪切、复制列表
    QList<CoItem *> m_CopyList;

    QWidget         *m_TaskWidget;
    TaskBar         *m_TaskBar;
    QScrollArea     *m_pShowCanvasArea;
    QScrollArea     *m_pShowDataArea;
    QScrollArea     *m_pShowAuthorityArea;
    QScrollArea     *m_pShowDeviceArea;
    QVBoxLayout     *m_pShowVbox;
    QWidget         *m_AttrWidget;
    CoAttributeBox  *m_pAttrBaseDlg;
    bool            m_isHideAttrWidget;
    bool            m_isHideTaskWidget;
    Ec_control      m_etherCat;

    typedef struct _PROJECT_STRU
    {
        QMap<QString,CoScene*> sceneMap;
        QMap<QString,FLOW_STRU> flowMap;//
        CoScene *safeScene;
        ushort viewWidth;
        ushort viewHeight;
        QString xmlPathFile;
        _PROJECT_STRU()
        {
            sceneMap.clear();
            flowMap.clear();
            safeScene = NULL;
            viewWidth = 400;
            viewHeight = 300;
            xmlPathFile = "";
        }
        void SetGrid(bool isGrid)
        {
            foreach (CoScene* scene, sceneMap.values()) {
                scene->ResizeEvent(isGrid);
            }
            foreach (FLOW_STRU flowStru, flowMap.values())
            {
                foreach (CoScene* scene, flowStru.SceneMap.values())
                {
                    scene->ResizeEvent(isGrid);
                }
            }
            if(safeScene)
                safeScene->ResizeEvent(isGrid);
        }
        void SetDebugMode(bool isDebug)
        {
            if(isDebug)
                AttributeInstance::GetInstance()->Power = 0;
            else
                AttributeInstance::GetInstance()->Power = 3;
            foreach (CoScene* scene, sceneMap.values()) {
                SetItemsToRun(scene,isDebug);
            }
            foreach (FLOW_STRU flowStru, flowMap.values())
            {
                foreach (CoScene* scene, flowStru.SceneMap.values())
                {
                    SetItemsToRun(scene,isDebug);
                }
            }
            if(safeScene)
            {
                SetItemsToRun(safeScene,isDebug);
            }
        }
        void Clear()
        {
            sceneMap.clear();
            flowMap.clear();
            safeScene = NULL;
            viewWidth = 400;
            viewHeight = 300;
            xmlPathFile = "";
        }
        void ChangeProName(const QString name)
        {
            for(auto it = sceneMap.begin();it != sceneMap.end();++it)
            {
                it.value()->SetProjectName(name);
            }
            foreach (FLOW_STRU flowStru, flowMap.values())
            {
                foreach (CoScene* scene, flowStru.SceneMap.values())
                {
                    scene->SetProjectName(name);
                }
            }
            if(safeScene)
                safeScene->SetProjectName(name);
        }
    private:
        void SetItemsToRun(CoScene* scene,bool isDebug)
        {
            QList<CoItemEx *> list;
            scene->GetItemsList(list);
            foreach (CoItemEx * Item, list)
            {
                Item->SetMoveAble(!isDebug);
                Item->SetDragAble(!isDebug);
                Item->SetSelectAble(!isDebug);
                Item->StartAuto(BoolToInt(isDebug));
            }
            QList<CoProxyWidget *> listwg;
            scene->GetItemsList(listwg);
            foreach (CoProxyWidget * Item, listwg)
            {
                Item->SetMoveAble(!isDebug);
                Item->SetDragAble(!isDebug);
                Item->SetSelectAble(!isDebug);
            }
        }
    }PROJECT_STRU;

    PROJECT_STRU    m_ProStru;
    QThread         *m_pThread;
    QMap<QString,PROJECT_STRU> m_SceneAttrMap;
public:
    bool        SaveSWXML(QString ProName, QString path);
    bool        IsEnd(TFlowBase *p_Fchart);
    TFlowBase   *GetStartChart(CoScene* Scene);
    bool        writeTxt(char * str,QString filename);
    void        writeTxtAppend(char * str,QString filename);
    void        write(QMap<QString,QMap<int,QString>> &FlowTxtMap,QString path);
    QString     GetCode(QString str);
    bool        SaveFlowScene(QString ProName, QString path);
    void        CreateHWxml(QString path);
    ItemSave    *saveflowscene;

    //***********2018/07/24 by korol*********
    bool        GetScenCode(CoScene * Scene,QVector<QString> &TxtVec,QString &Err);
    bool        WriteFlowScene(QString path,QVector<QString>);
    void        MakeDir(QDir &dir,const QString path);
    bool        HandelFlowScene_v1(QString PorStu,QString path,QString &Err);



protected:
    void        closeEvent(QCloseEvent *ev);
};

#endif // MAINWINDOW_H
