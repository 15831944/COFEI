#include "nightsetwidget.h"
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <CoGraphics/Scene/Scene.h>
#include <QHeaderView>
#include <QDebug>
NightSetWidget::NightSetWidget(double StartX, double StartY, double StopX, double StopY,
                               const Qt::PenStyle &LineStyle, const int LineWidth, const QColor &LineColor,
                               const QColor &BackColor, CoScene *Scene):
    CoProxyWidget(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{

   this->setSelected(true);
    SetType(CoItem::Mde_Safe_Sig);
    this->resize(420,348);
    QFont headerfont("黑体",10,QFont::Black);
    m_ptabwidget = new QTableWidget();
    m_pdelegate = new KorolDelegate();
    btn = new QPushButton(m_ptabwidget);
    btn->setText("Light");
    btn->setStyleSheet("color:rgb(169,169,169);lborder:1px groove gray;");//border-radius:10px;padding:2px 4px;
    btn->setFont(headerfont);
    btn->resize(101,25);//25
    btn->move(119,1);
    m_ptabwidget->horizontalHeader()->setFont(headerfont);
    m_ptabwidget->setColumnCount(5);
    m_ptabwidget->setRowCount(10);
    m_ptabwidget->resize(380,320);
    m_ptabwidget->setColumnWidth(0,42);
    m_ptabwidget->setColumnWidth(1,77);
    m_ptabwidget->setColumnWidth(2,50);
    m_ptabwidget->setColumnWidth(3,50);
    m_ptabwidget->setColumnWidth(4,180);
    QStringList heardlist ;
    heardlist<<"ID"<<"Type"<<"灯名称1"<<"灯名称2"<<"注释";
    m_ptabwidget->setHorizontalHeaderLabels(heardlist);
    //con't change count about horizontal and vertical
    m_ptabwidget->horizontalHeader()->setDisabled(true);
    m_ptabwidget->verticalHeader()->setDisabled(true);
    m_ptabwidget->verticalHeader()->hide();
    m_ptabwidget->setItemDelegateForColumn(2,m_pdelegate);
    m_ptabwidget->setItemDelegateForColumn(3,m_pdelegate);
    m_ptabwidget->setItem(0,0,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(0,1,new QTableWidgetItem("escape"));
    m_ptabwidget->setItem(0,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(0,3,new QTableWidgetItem("4"));
    m_ptabwidget->setItem(0,4,new QTableWidgetItem("急停"));

    m_ptabwidget->setItem(1,0,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(1,1,new QTableWidgetItem("slow"));
    m_ptabwidget->setItem(1,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(1,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(1,4,new QTableWidgetItem("急停"));

    m_ptabwidget->setItem(2,0,new QTableWidgetItem("3"));
    m_ptabwidget->setItem(2,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(2,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(2,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(2,4,new QTableWidgetItem("普通停止，执行完一个循环"));

    m_ptabwidget->setItem(3,0,new QTableWidgetItem("4"));
    m_ptabwidget->setItem(3,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(3,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(3,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(3,4,new QTableWidgetItem("复位"));

    m_ptabwidget->setItem(4,0,new QTableWidgetItem("5"));
    m_ptabwidget->setItem(4,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(4,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(4,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(4,4,new QTableWidgetItem("启动"));

    m_ptabwidget->setItem(5,0,new QTableWidgetItem("6"));
    m_ptabwidget->setItem(5,1,new QTableWidgetItem("escape"));
    m_ptabwidget->setItem(5,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(5,3,new QTableWidgetItem("4"));
    m_ptabwidget->setItem(5,4,new QTableWidgetItem("回原点"));

    m_ptabwidget->setItem(6,0,new QTableWidgetItem("10"));
    m_ptabwidget->setItem(6,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(6,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(6,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(6,4,new QTableWidgetItem("程序Ready"));

    m_ptabwidget->setItem(7,0,new QTableWidgetItem("15"));
    m_ptabwidget->setItem(7,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(7,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(7,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(7,4,new QTableWidgetItem("运行报错"));

    m_ptabwidget->setItem(8,0,new QTableWidgetItem("16"));
    m_ptabwidget->setItem(8,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(8,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(8,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(8,4,new QTableWidgetItem("准备运行"));

    m_ptabwidget->setItem(9,0,new QTableWidgetItem("17"));
    m_ptabwidget->setItem(9,1,new QTableWidgetItem("longBright"));
    m_ptabwidget->setItem(9,2,new QTableWidgetItem("1"));
    m_ptabwidget->setItem(9,3,new QTableWidgetItem("2"));
    m_ptabwidget->setItem(9,4,new QTableWidgetItem("运行正常"));
    //m_ptabwidget->setModel(m_pmodel);

    SetTabWidgetStyle(m_ptabwidget);
    QGraphicsProxyWidget *pPixmapWidget = Scene->addWidget(m_ptabwidget);
    m_pLayout = new QGraphicsGridLayout();
    m_pLayout->addItem(pPixmapWidget,0,0,10,10);
    m_pLayout->setContentsMargins(2, 2, 10, 10);

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setLayout(m_pLayout);
    m_ptabwidget->installEventFilter(this);


//    QMap<QString, SENSOR_LIGHT_STRU> valmap =  GetTabWidgetValue();
//    QString Str = getUiInfoStr(valmap);
//    qDebug()<<Str;
//    QMap<QString, SENSOR_LIGHT_STRU> valmap1 = StrToMap(Str);
//    qDebug()<<valmap1.size();
    //id=:1,name=escape,night=1&4;id=2,name=slow,night=1&2;
    //sw内存里面还需要一份
   // qDebug()<<valmap.size();

//   QMap<QString, SENSOR_LIGHT_STRU> valmap;
//   SENSOR_LIGHT_STRU stu;
//   stu.name = "llllll";
//   stu.light.push_back("1");
//   stu.light.push_back("2");
//   valmap.insert("1",stu);
//   SENSOR_LIGHT_STRU stu1;
//   stu1.name = "coll";
//   stu1.light.push_back("5");
//   stu1.light.push_back("8");
//   valmap.insert("2",stu1);
//   SENSOR_LIGHT_STRU stu21;
//   stu21.name = "coll";
//   stu21.light.push_back("5");
//   stu21.light.push_back("4");
//   valmap.insert("4",stu21);
//   SENSOR_LIGHT_STRU stu221;
//   stu221.name = "coll";
//   stu221.light.push_back("8");
//   stu221.light.push_back("6");
//   valmap.insert("124",stu221);
//   SENSOR_LIGHT_STRU stu2221;
//   stu2221.name = "coll";
//   stu2221.light.push_back("6");
//   stu2221.light.push_back("2");
//   valmap.insert("45",stu2221);
//   SetTabWidgetValue(valmap);

}

void NightSetWidget::SetTabWidgetValue(const QMap<QString, SENSOR_LIGHT_STRU> tabValueMap)
{
    for(int i = 0;i<m_ptabwidget->rowCount();++i)
    {
        for(int j = 0;j < m_ptabwidget->columnCount()-1;++j)
        {
            m_ptabwidget->item(i,j)->setText(NULL);
        }
    }
    QMap<int,SENSOR_LIGHT_STRU>tabValueMap_1;
    tabValueMap_1.clear();
    for(auto ito = tabValueMap.begin();ito != tabValueMap.end();ito++)
    {
        tabValueMap_1.insert(ito.key().toInt(),ito.value());
    }
    int i = 0;
    for(auto it = tabValueMap_1.begin();it!= tabValueMap_1.end();++it)
    {
        if(i < m_ptabwidget->rowCount())
        {
            int j = 0;
            m_ptabwidget->setItem(i,j,new QTableWidgetItem(QString("%1").arg(it.key())));
            j++;
            m_ptabwidget->setItem(i,j,new QTableWidgetItem(it.value().name));
            j++;
            foreach (QString var, it.value().light)
            {
                if(j < m_ptabwidget->columnCount()){
                m_ptabwidget->setItem(i,j,new QTableWidgetItem(var));
                j++;
                }
            }
            i++;
        }
    }
    SetTabWidgetStyle(m_ptabwidget);
}

QMap<QString, SENSOR_LIGHT_STRU> NightSetWidget::GetTabWidgetValue()
{
    QMap<QString,SENSOR_LIGHT_STRU>tabVal;
    tabVal.clear();
    for(int i = 0;i<m_ptabwidget->rowCount();++i)
    {
        QString key = m_ptabwidget->item(i,0)->text();
        SENSOR_LIGHT_STRU lightstu;
        lightstu.light.clear();
        lightstu.name = m_ptabwidget->item(i,1)->text();
        for(int j = 2;j < m_ptabwidget->columnCount()-1;++j)
        {
            if(m_ptabwidget->item(i,j)->text()!= "")
            {
                lightstu.light.push_back(m_ptabwidget->item(i,j)->text());
            }
        }
       tabVal.insert(key,lightstu);
    }
    return tabVal;
}

bool NightSetWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_ptabwidget)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *event_mouse = static_cast<QMouseEvent*>(event);
            if(event_mouse->button() == Qt::LeftButton)
            {
                this->setSelected(true);
                m_point = this->pos();
                m_globlePoint = event_mouse->globalPos();
                m_xoffsict = qAbs(qAbs(event_mouse->y()) - qAbs(m_point.y()));

            }
        }
        if(event->type() == QEvent::MouseMove)
        {
              QMouseEvent *event_mouseMove = static_cast<QMouseEvent*>(event);
            this->moveBy(event_mouseMove->x()-  m_point.x(),event_mouseMove->y());
        }

    }
    return CoProxyWidget::eventFilter(watched,event);
}

void NightSetWidget::SetTabWidgetStyle(QTableWidget *tabwidget)
{
    QFont font ("楷体", 10, 73);
    for (int i = 0;i<tabwidget->rowCount();++i) {
        for(int j = 0;j< tabwidget->columnCount();++j){
            if(tabwidget->item(i,j))
            {
                if(j==0||j==4)
                {
                    tabwidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
                }
                tabwidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                tabwidget->item(i,j)->setFont(font);
            }
        }
    }
}

CoProxyWidget *NightSetWidget::Copy()
{
    NightSetWidget *LightWidget = new NightSetWidget();

    LightWidget->Copy(this);

    return LightWidget;
}

void NightSetWidget::Copy(NightSetWidget *LightFrom)
{
    CoProxyWidget::Copy(LightFrom);
}
