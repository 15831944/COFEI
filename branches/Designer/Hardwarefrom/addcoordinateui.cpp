#include "addcoordinateui.h"

AddCoordinateUI::AddCoordinateUI(QString name, int start, int end, QString contains,QWidget *parent) : QWidget(parent)
{
    this->setFixedWidth(220);
    gbox = new QGroupBox(this);
    gbox->setGeometry(3,3,215,100);
    gbox->setStyleSheet("QGroupBox"
                            "{"
                            "border: 1px solid white;"
                            "border-radius:8px;"
                            "margin-top:6px;"
                            "}"
                            "QGroupBox:title"
                            "{"
                            "color:red;"
                            "subcontrol-origin: margin;"
                            "left: 10px;"
                            "}");
    gbox->setTitle(name);
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = NULL;
    QLabel *start_ = new QLabel(gbox);
    start_->setText("正极限 ：");
    QLabel *start_value = new QLabel(gbox);
    start_value->setText(QString("%1").arg(start));
    hbox = new QHBoxLayout();
    hbox->addWidget(start_);
    hbox->addWidget(start_value);
    vbox->addLayout(hbox);

    QLabel *end_ = new QLabel(gbox);
    end_->setText("负极线 ：");
    QLabel *end_value = new QLabel(gbox);
    end_value->setText(QString("%1").arg(end));
    hbox = new QHBoxLayout();
    hbox->addWidget(end_);
    hbox->addWidget(end_value);
    vbox->addLayout(hbox);

    QLabel *contans_ = new QLabel(gbox);
    contans_->setText("包含轴 ：");
    QLabel *contans_value = new QLabel(gbox);
    contans_value->setText(contains);
    hbox = new QHBoxLayout();
    hbox->addWidget(contans_);
    hbox->addWidget(contans_value);
    vbox->addLayout(hbox);
    gbox->setLayout(vbox);
    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    mainlayout->addWidget(gbox);
    this->setLayout(mainlayout);
}

void AddCoordinateUI::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = CreateMenu();
    menu->exec(QCursor::pos());
    event->accept();
}

QMenu *AddCoordinateUI::CreateMenu()
{
    QMenu * p_menu = new QMenu();
    p_menu->setFont(QFont("黑体",12));
    QAction *addaxis = new QAction(QObject::tr("删除"));
    connect(addaxis,&QAction::triggered,this,&AddCoordinateUI::DeleteMenu);
    QAction *addio = new QAction(QObject::tr("修改"));
   // connect(addio,&QAction::triggered,this,&Hardwarefrom::AddIo);
    QAction *del = new QAction(QObject::tr("...."));
    p_menu->addAction(addaxis);
    p_menu->addAction(addio);
    p_menu->addAction(del);
    return p_menu;
}

void AddCoordinateUI::DeleteMenu()
{
    delete this;
}

void AddCoordinateUI::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(Qt::LeftButton == event->button())
    {
        emit SendCoorTile(gbox->title());
        delete this;
    }
}


