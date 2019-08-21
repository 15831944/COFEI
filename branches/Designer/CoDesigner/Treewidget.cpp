#include "Treewidget.h"
#include <QDebug>
#include <QMetaEnum>
#include <QMessageBox>
CoTreeWidget::CoTreeWidget()
{
    this->setColumnCount(1);
    this->setIconSize(QSize(24,24));
    m_pRootItem = new QTreeWidgetItem(this);
    m_pRootItem->setIcon(0,QIcon(":/images/projects.png"));
    m_pRootItem->setText(0,"项目组");
    m_pRootItem->setExpanded(true);
    m_treeName = TreeName::Base;
    this->addTopLevelItem(m_pRootItem);
    this->setHeaderHidden(true);
    connect(this,&CoTreeWidget::currentItemChanged,this,&CoTreeWidget::ChangeNodeItem);
}

void CoTreeWidget::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    QString oldName = this->currentItem()->toolTip(0);
    QString newName = this->currentItem()->text(0);
    if(oldName.compare(newName) != 0)
    {
        bool isrepeating = false;
        for(int i = 0 ; i < this->currentItem()->parent()->childCount();++i)
        {
            if(this->currentItem()->parent()->child(i) != this->currentItem())
            {
                if(this->currentItem()->parent()->child(i)->text(0) == newName)
                {
                    this->currentItem()->setText(0,oldName);
                    isrepeating = true;
                    break;
                }
            }
        }
        if(!isrepeating)
        {
            if(currentItem()->parent()->text(0) == "项目组")
            {
                emit SendRenameProjectEvent(oldName,newName);
            }
            else if(currentItem()->statusTip(0) == "Canvas")
            {
                QString proName = currentItem()->parent()->parent()->text(0);
                emit SendRenameCanvasEvent(proName,oldName,newName);
            }
            else if(currentItem()->statusTip(0) == "Flow")
            {
                QString proName = currentItem()->parent()->parent()->text(0);
                emit SendRenameCanvasEvent(proName,oldName,newName,oldName);
            }
            else if(currentItem()->statusTip(0) == "FlowThread")
            {
                QString flowName = currentItem()->parent()->text(0);
                QString proName = currentItem()->parent()->parent()->parent()->text(0);
                emit SendRenameCanvasEvent(proName,oldName,newName,flowName);
            }
            else if(currentItem()->statusTip(0) == "ResetThread")
            {
                QString flowName = currentItem()->parent()->text(0);
                QString proName = currentItem()->parent()->parent()->parent()->text(0);
                emit SendRenameCanvasEvent(proName,oldName,newName,flowName);
            }
            else if(currentItem()->statusTip(0) == "OrgThread")
            {
                QString flowName = currentItem()->parent()->text(0);
                QString proName = currentItem()->parent()->parent()->parent()->text(0);
                emit SendRenameCanvasEvent(proName,oldName,newName,flowName);
            }
            currentItem()->setToolTip(0,newName);
        }
    }
    QTreeWidget::closeEditor(editor,hint);
}

void CoTreeWidget::AddCanvas()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->text(0);
    m_treeName = TreeName::Canvas;
    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::AddFlow()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->text(0);
    m_treeName = TreeName::Flow;
    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::AddReset()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->text(0);
    m_treeName = TreeName::Reset;
    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::AddOrg()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->text(0);
    m_treeName = TreeName::Org;

    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::AddChlidReset()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString flowName = curItem->text(0);
    QString  proname = curItem->parent()->parent()->text(0);
    int Num = curItem->childCount();
    for(int i = 0; i < Num; ++i)//自复位重名检测
    {
        if(curItem->child(i)->text(0) == "子复位")
        {
            QMessageBox::information(NULL,tr("警告"),tr("该节点下名字不可重复,请重新输入!"));//此时会覆盖原来数据问题
            return;
        }
    }
    m_treeName = TreeName::ChlidReset;

    emit SendNewCanvasEvent(flowName+","+proname,m_treeName);
}

void CoTreeWidget::AddFlowThread()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString flowName = curItem->text(0);
    QString   proname = curItem->parent()->parent()->text(0);
    QString childlist;
    int Num = curItem->childCount();
    for(int i = 0; i < Num; ++i)//收集本节点下的子节点名字
    {
        childlist += curItem->child(i)->text(0)+";";
    }
    if(childlist.size() >= 1)
    {
        childlist.chop(1);
    }
    m_treeName = TreeName::FlowThread;
    emit SendNewCanvasEvent(flowName+","+proname + "#" + childlist ,m_treeName);
}

void CoTreeWidget::AddResetThread()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->parent()->text(0);
    m_treeName = TreeName::ResetThread;
    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::AddOrgThread()
{
    QTreeWidgetItem *curItem =  this->currentItem();
    QString   proname = curItem->parent()->parent()->text(0);
    m_treeName = TreeName::OrgThread;
    emit SendNewCanvasEvent(proname,m_treeName);
}

void CoTreeWidget::DeleteCanvas()
{
    m_treeName = TreeName::Canvas;
    //当前画布，父节点画布区，再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::DeleteFlow()
{
    m_treeName = TreeName::Flow;
    //当前流程，父节点流程图，再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::DeleteReset()
{
    m_treeName = TreeName::Reset;
    //当前复位，父节点流程图，再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::DeleteOrg()
{
    m_treeName = TreeName::Org;
    //当前ORG，父节点流程图，再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::DelChlidReset()
{
    m_treeName = TreeName::ChlidReset;
    //当前子复位，父节点流程,再父节点流程图，再再父节点项目
    QString strflow = this->currentItem()->parent()->text(0);
    QString proName = this->currentItem()->parent()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strflow,m_treeName);
}

void CoTreeWidget::DelFlowThread()
{
    m_treeName = TreeName::FlowThread;
    //当前线程，父节点流程,再父节点流程图，再再父节点项目
    QString strName = this->currentItem()->text(0);
    QString strflow = this->currentItem()->parent()->text(0);
    QString proName = this->currentItem()->parent()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName+","+strflow,m_treeName);
}

void CoTreeWidget::DelResetThread()
{
    m_treeName = TreeName::ResetThread;
    //当前复位线程，父节点复位,再父节点流程图，再再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::DelOrgThread()
{
    m_treeName = TreeName::OrgThread;
    //当前Org线程，父节点ORG,再父节点流程图，再再父节点项目
    QString strName = this->currentItem()->text(0);
    QString proName = this->currentItem()->parent()->parent()->parent()->text(0);

    emit SendDelCanvasEvent(proName,strName,m_treeName);
}

void CoTreeWidget::AddProject()
{
    emit SendNewProjectEvent();
}

void CoTreeWidget::DeleteProject()
{
    QString strName = this->currentItem()->text(0);
    emit SendDelProjectEvent(strName);
}

void CoTreeWidget::ChangeNodeItem(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    int iname = TreeName::Base;
    QString proname = "XXX";
    if(current->parent() == NULL)
    {
        iname = TreeName::Root;
    }
    else if(current->parent()->text(0) == "项目组")
    {
        proname = current->text(0);
        iname = TreeName::Project;
    }
    else if(current->parent()->text(0) == "画布区")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::Canvas;
    }
    else if(current->text(0) == "权限管理")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::Authority;
    }
    else if(current->text(0) == "变量管理")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::DataArea;
    }
    else if(current->text(0) == "硬件设备")
    {
        proname = current->parent()->text(0);
        iname = TreeName::Devices;
    }
    else if(current->text(0) == "安全配置")
    {
        proname = current->parent()->text(0);
        iname = TreeName::SafeArea;
    }
    else if(current->statusTip(0) == "Flow")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::Flow;
    }
    else if(current->statusTip(0) == "Reset")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::Reset;
    }
    else if(current->statusTip(0) == "Org")
    {
        proname = current->parent()->parent()->text(0);
        iname = TreeName::Org;
    }
    else if(current->statusTip(0) == "FlowThread")
    {
        QString str = current->parent()->text(0);
        if(current->parent()->parent()->parent())
        {
            proname = str + "," +current->parent()->parent()->parent()->text(0);
            iname = TreeName::FlowThread;
        }
    }
    else if(current->statusTip(0) == "ChlidReset")
    {
        QString str = current->parent()->text(0);
        if(current->parent()->parent()->parent())
        {
            proname = str + "," +current->parent()->parent()->parent()->text(0);
            iname = TreeName::ChlidReset;
        }
    }
    else if(current->statusTip(0) == "ResetThread")
    {
        proname = current->parent()->parent()->parent()->text(0);
        iname = TreeName::ResetThread;
    }
    else if(current->statusTip(0) == "OrgThread")
    {
        proname = current->parent()->parent()->parent()->text(0);
        iname = TreeName::OrgThread;
    }

    emit SendChangeTreeNodeEvent(proname,current->text(0),iname);
}

void CoTreeWidget::AddCanvasItem(const QString& str,int type)
{
    if(type == TreeName::SafeArea)
        return;
    QTreeWidgetItem  *curItem =  this->currentItem();
    if(!curItem)
        return;
    int Num = curItem->childCount();
    for(int i = 0; i < Num; ++i)//防止重复新建重名的节点
    {
        if(curItem->child(i)->text(0) == str && curItem->parent()->parent()->text(0)=="流程图")
        {
            QMessageBox::information(NULL,tr("警告"),tr("该节点下名字不可重复,请重新输入!"));//此时会覆盖原来数据问题
            return;
        }
    }
    QTreeWidgetItem *CanvasItem = new QTreeWidgetItem();
    CanvasItem->setStatusTip(0,GetEnumToString(type));
    CanvasItem->setIcon(0,QIcon(":/images/draw.png"));
    CanvasItem->setText(0,str);
    if(type != TreeName::Reset && type != TreeName::Org)
        CanvasItem->setFlags(CanvasItem->flags() | Qt::ItemIsEditable);
    CanvasItem->setToolTip(0,str);
    curItem->addChild(CanvasItem);
    curItem->setExpanded(true);
}

void CoTreeWidget::DeleteCanvasItem()
{
    QTreeWidgetItem  *curItem =  this->currentItem();
    if(curItem->childCount() > 0)
    {
        for(int i = 0; i < curItem->childCount(); i ++)
        {
            DeleteItem(curItem->child(i));
        }
    }
    delete curItem;
}

void CoTreeWidget::AddProjectItem(const QString& strname)
{
    QTreeWidgetItem *proitem = new QTreeWidgetItem();
    proitem->setIcon(0,QIcon(":/images/project.png"));
    proitem->setText(0,strname);
    proitem->setStatusTip(0,"Project");
    proitem->setFlags(proitem->flags() | Qt::ItemIsEditable);
    m_pRootItem->addChild(proitem);

    QTreeWidgetItem *canvasItem = new QTreeWidgetItem();
    canvasItem->setIcon(0,QIcon(":/images/canvas.png"));
    canvasItem->setText(0,"画布区");
    canvasItem->setStatusTip(0,"CanvasGroup");
    proitem->addChild(canvasItem);
    setCurrentItem(canvasItem);

    QTreeWidgetItem *databaseItem = new QTreeWidgetItem();
    databaseItem->setIcon(0,QIcon(":/images/datebase.png"));
    databaseItem->setText(0,"数据词典");
    databaseItem->setStatusTip(0,"_DataBase");
    proitem->addChild(databaseItem);

    QTreeWidgetItem *AuthorityItem = new QTreeWidgetItem();
    AuthorityItem->setIcon(0,QIcon(":/images/male.png"));
    AuthorityItem->setText(0,"权限管理");
    AuthorityItem->setStatusTip(0,"Authority");

    QTreeWidgetItem *dataItem = new QTreeWidgetItem();
    dataItem->setIcon(0,QIcon(":/images/data.png"));
    dataItem->setText(0,"变量管理");
    dataItem->setStatusTip(0,"DataArea");
    databaseItem->addChild(AuthorityItem);
    databaseItem->addChild(dataItem);

    databaseItem->setExpanded(true);

    QTreeWidgetItem *deviceItem = new QTreeWidgetItem();
    deviceItem->setIcon(0,QIcon(":/images/device.png"));
    deviceItem->setText(0,"硬件设备");
    deviceItem->setStatusTip(0,"Devices");
    proitem->addChild(deviceItem);

    QTreeWidgetItem *safeItem = new QTreeWidgetItem();
    safeItem->setIcon(0,QIcon(":/images/SafeCofig.png"));
    safeItem->setText(0,"安全配置");
    safeItem->setStatusTip(0,"SafeArea");
    proitem->addChild(safeItem);

    QTreeWidgetItem *flowItem = new QTreeWidgetItem();
    flowItem->setIcon(0,QIcon(":/images/Flow.png"));
    flowItem->setText(0,"流程图");
    flowItem->setStatusTip(0,"FlowChart");
    proitem->addChild(flowItem);
    proitem->setExpanded(true);
}

void CoTreeWidget::DeleteProjectItem()
{
    DeleteCanvasItem();
}

void CoTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    QTreeWidgetItem  *curItem =  this->itemAt(pos);
    if (curItem == NULL)
        return;
    QMenu *menu = NULL;
    if(curItem->child(0) != NULL && curItem->child(0)->text(0) == "画布区")
    {
        menu = CreateContextProMenu();
    }
    else if(curItem->text(0) == "项目组")
    {
        menu = CreateContextProMenu(1);
    }
    else
        menu = CreateContextMenu(GetStringToEnum(curItem->statusTip(0)));

    if(menu)
    {
        menu->exec(QCursor::pos());
    }
    event->accept();
}

void CoTreeWidget::SetCurItem(const QString pro,const int type)
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if(pro == (*it)->text(0) )
        {
            if(0 == type && (*it)->parent()->text(0) == "画布区" )
            {
                setFocus();
                setCurrentItem(*it);
                (*it)->setSelected(true);
                break;
            }
            else if(1 == type && (*it)->parent()->text(0) != "项目组" && (*it)->parent()->text(0) != "画布区")//防止流程名字与项目重名画布名字判断
            {
                setFocus();
                setCurrentItem(*it);
                (*it)->setSelected(true);
                break;
            }
        }
        ++it;
    }
}
QMenu* CoTreeWidget::CreateContextMenu(int index)
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));

    switch (index)
    {
    case CanvasGroup:
    {
        QAction *add = new QAction(this);
        connect(add, &QAction::triggered, this, &CoTreeWidget::AddCanvas);
        add->setText("新建画布");
        menu->addAction(add);
    }
        break;
    case Canvas:
    {
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DeleteCanvas);
        del->setText("删除画布");
        menu->addAction(del);
    }
        break;
    case FlowChart:
    {
        QAction *add = new QAction(this);
        connect(add, &QAction::triggered, this, &CoTreeWidget::AddFlow);
        add->setText("新建流程");
        QAction *add1 = new QAction(this);
        connect(add1, &QAction::triggered, this, &CoTreeWidget::AddReset);
        add1->setText("新建复位");
        QAction *add2 = new QAction(this);
        connect(add2, &QAction::triggered, this, &CoTreeWidget::AddOrg);
        add2->setText("新建回原点");
        menu->addAction(add);
        menu->addAction(add1);
        menu->addAction(add2);
    }
        break;
    case Flow:
    {
        QAction *add = new QAction(this);
        connect(add, &QAction::triggered, this, &CoTreeWidget::AddFlowThread);
        add->setText("新建线程");
        QAction *add1 = new QAction(this);
        connect(add1, &QAction::triggered, this, &CoTreeWidget::AddChlidReset);
        add1->setText("新建子复位");
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DeleteFlow);
        del->setText("删除流程");
        menu->addAction(add);
        menu->addAction(add1);
        menu->addAction(del);
    }
        break;
    case Reset:
    {
        QAction *add = new QAction(this);
        connect(add, &QAction::triggered, this, &CoTreeWidget::AddResetThread);
        add->setText("新建复位线程");
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DeleteReset);
        del->setText("删除复位");
        menu->addAction(add);
        menu->addAction(del);
    }
        break;
    case Org:
    {
        QAction *add = new QAction(this);
        connect(add, &QAction::triggered, this, &CoTreeWidget::AddOrgThread);
        add->setText("新建回原点线程");
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DeleteOrg);
        del->setText("删除回原点");
        menu->addAction(add);
        menu->addAction(del);
    }
        break;
    case FlowThread:
    {
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DelFlowThread);
        del->setText("删除线程");
        menu->addAction(del);
    }
        break;
    case ChlidReset:
    {
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DelChlidReset);
        del->setText("删除子复位");
        menu->addAction(del);
    }
        break;
    case ResetThread:
    {
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DelResetThread);
        del->setText("删除复位线程");
        menu->addAction(del);
    }
        break;
    case OrgThread:
    {
        QAction *del = new QAction(this);
        connect(del, &QAction::triggered, this, &CoTreeWidget::DelOrgThread);
        del->setText("删除回原点线程");
        menu->addAction(del);
    }
        break;
    default:
        break;
    }

    return menu;
}

QMenu *CoTreeWidget::CreateContextProMenu(int index)
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建项目"),this);
    connect(add, &QAction::triggered, this, &CoTreeWidget::AddProject);
    QAction *del = new QAction(QObject::tr("删除项目"),this);
    connect(del, &QAction::triggered, this, &CoTreeWidget::DeleteProject);

    if(1 == index)
    {
        del->setEnabled(false);
    }
    menu->addAction(add);
    menu->addAction(del);
    return menu;
}
void CoTreeWidget::DeleteItem(QTreeWidgetItem* node)
{
    if(node == NULL)
        return;
    if(node->childCount() > 0)
    {
        for(int i = 0; i < node->childCount(); i++)
        {
            DeleteItem(node->child(i));
        }
    }
    else
    {
        delete node;
    }
}
QString CoTreeWidget::GetEnumToString(int ienum)
{
    const QMetaObject metaObject = CoTreeWidget::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("TreeName");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    return  QString(en.valueToKey(ienum));
}
int CoTreeWidget::GetStringToEnum(QString str)
{
    const QMetaObject metaObject = CoTreeWidget::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("TreeName");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    QByteArray ba = str.toLatin1();
    return en.keysToValue(ba.data());
}
