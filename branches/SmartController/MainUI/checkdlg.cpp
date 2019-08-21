#include "checkdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QMainWindow>
#include <QSpinBox>
#include <QCheckBox>
#include <MyConfig_Shared>
CheckDlg::CheckDlg(QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> hwSerOutputMap,bool isNewPoint)
{
    m_isNewPoint = isNewPoint;
    font.setPointSize(13);
    font.setFamily("黑体");
    this->setFont(font);
    this->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
    m_hwSerOutputMap = hwSerOutputMap;
    //CreateUI(hwSerOutputMap);
}
void CheckDlg::CreateUI(int x,int y)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    this->setLayout(vBox);

    QWidget *wgHeader = new QWidget(this);
    wgHeader->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    wgHeader->setFixedHeight(180);
    QLabel *lableType = new QLabel(this);
    lableType->setAlignment(Qt::AlignRight);
    lableType->setText("定位类别");
    QLabel *lableId = new QLabel(this);
    lableId->setAlignment(Qt::AlignRight);
    lableId->setText("参数ID");
    QLabel *lableNote = new QLabel(this);
    lableNote->setAlignment(Qt::AlignRight);
    lableNote->setText("参数注释");
    QLabel *lableTimeout = new QLabel(this);
    lableTimeout->setAlignment(Qt::AlignRight);
    lableTimeout->setText("超时");
    QLabel *lableDelay = new QLabel(this);
    lableDelay->setAlignment(Qt::AlignRight);
    lableDelay->setText("单步延迟");

    lableSpeed = new QLabel(this);
    lableSpeed->setAlignment(Qt::AlignRight);
    lableSpeed->setText("插补速度");
    lableAcc = new QLabel(this);
    lableAcc->setAlignment(Qt::AlignRight);
    lableAcc->setText("插补加速");

    cbb = new QComboBox(this);
    cbb->setEnabled(m_isNewPoint);
    QStringList list;
    list << "快速定位" << "直线插补";
    cbb->addItems(list);
    cbb->setFixedSize(150,30);
    cbb->setFont(font);

    lineEditId = new QLineEdit(this);
    lineEditId->setAlignment(Qt::AlignLeft);
    lineEditId->setFixedHeight(30);
    lineEditId->setFont(font);
    lineEditId->setEnabled(m_isNewPoint);

    lineEditNote = new QLineEdit(this);
    lineEditNote->setAlignment(Qt::AlignLeft);
    lineEditNote->setFixedSize(x/2,30);
    lineEditNote->setFont(font);

    lineEditTimeout = new QLineEdit(this);
    lineEditTimeout->setAlignment(Qt::AlignLeft);
    lineEditTimeout->setFixedHeight(30);
    lineEditTimeout->setFont(font);
    lineEditTimeout->setValidator(new QIntValidator(0, 65535, this));
    lineEditTimeout->setText("15000");

    lineEditDelay = new QLineEdit(this);
    lineEditDelay->setAlignment(Qt::AlignLeft);
    lineEditDelay->setFixedSize(x/4,30);
    lineEditDelay->setFont(font);
    lineEditDelay->setValidator(new QIntValidator(0, 32767, this));
    lineEditDelay->setText("0");

    lineEditSpeed = new QLineEdit(this);
    lineEditSpeed->setAlignment(Qt::AlignLeft);
    lineEditSpeed->setFixedHeight(30);
    lineEditSpeed->setFont(font);
    lineEditSpeed->setValidator(new QIntValidator(1, 500, this));
    lineEditSpeed->setText("500");

    lineEditAcc = new QLineEdit(this);
    lineEditAcc->setAlignment(Qt::AlignLeft);
    lineEditAcc->setFixedSize(x/4,30);
    lineEditAcc->setFont(font);
    lineEditAcc->setValidator(new QIntValidator(1, 10000, this));
    lineEditAcc->setText("1000");

    lineEditAcc->setVisible(false);
    lineEditSpeed->setVisible(false);
    lableSpeed->setVisible(false);
    lableAcc->setVisible(false);
    cbb->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditId->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditNote->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditDelay->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditTimeout->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditSpeed->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    lineEditAcc->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:5px;"));
    QGridLayout *gridLayoutHeader = new QGridLayout(wgHeader);
    gridLayoutHeader->addWidget(lableType,0,0);
    gridLayoutHeader->addWidget(cbb,0,1);
    gridLayoutHeader->addWidget(lableId,1,0);
    gridLayoutHeader->addWidget(lineEditId,1,1);
    gridLayoutHeader->addWidget(lableNote,1,2);
    gridLayoutHeader->addWidget(lineEditNote,1,3);
    gridLayoutHeader->addWidget(lableTimeout,2,0);
    gridLayoutHeader->addWidget(lineEditTimeout,2,1);
    gridLayoutHeader->addWidget(lableDelay,2,2);
    gridLayoutHeader->addWidget(lineEditDelay,2,3);
    gridLayoutHeader->addWidget(lableSpeed,3,0);
    gridLayoutHeader->addWidget(lineEditSpeed,3,1);
    gridLayoutHeader->addWidget(lableAcc,3,2);
    gridLayoutHeader->addWidget(lineEditAcc,3,3);
    wgHeader->setLayout(gridLayoutHeader);
    vBox->addWidget(wgHeader);
    QScrollArea *pArea = new QScrollArea(this);
    wgCheck = new QWidget(this);
    QGridLayout *baseLayout = new QGridLayout(wgCheck);
    pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pArea->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    baseLayout->setSpacing(6);
    wgCheck->setLayout(baseLayout);
    wgCheck->setGeometry(10,10,x-10,(m_hwSerOutputMap.count() / 2 +2) * 30);
    uchar uc = 0;
    uchar irow = 0;
    for(auto item = m_hwSerOutputMap.begin();item != m_hwSerOutputMap.end();++item)
    {
        if(item.value().modelType == "1")
        {
            QCheckBox *cb = new QCheckBox(this);
            connect(cb,&QCheckBox::stateChanged,this,&CheckDlg::SetAutoCheckWg);
            cb->setFixedSize((x-10)/3,24);
            cb->setText(item.key());
            baseLayout->addWidget(cb,irow,uc++);
            if(uc >= 3)
            {
                ++irow;
                uc = 0;
            }
        }
    }
    pArea->setWidget(wgCheck);
    vBox->addWidget(pArea);

    QScrollArea *pAreaAuto = new QScrollArea(this);
    QWidget *wgAutoCheck = new QWidget(this);
    wgAutoCheck->setGeometry(5,5,x-10,(y-240)/2);
    table = new QTableWidget(wgAutoCheck);
    table->setGeometry(5,5,x-10,(y-240)/2);
    QStringList Headlist;
    Headlist << "轴的ID" << "速度比率" << "是否选中";
    CreateTableHeard(Headlist,x-10);
    pAreaAuto->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pAreaAuto->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));


    pAreaAuto->setWidget(wgAutoCheck);
    vBox->addWidget(pAreaAuto);

    QWidget *wgBack = new QWidget(this);
    wgBack->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    wgBack->setFixedHeight(100);
    QHBoxLayout *hBox = new QHBoxLayout(wgBack);
    QPushButton *cancel = new QPushButton("取消",this);
    connect(cancel,&QPushButton::clicked,this,&CheckDlg::reject);
    QPushButton *ok = new QPushButton("确认",this);
    connect(ok,&QPushButton::clicked,this,&CheckDlg::OverAccept);
    hBox->addWidget(cancel);
    hBox->addWidget(ok);
    wgBack->setLayout(hBox);
    vBox->addWidget(wgBack);

    connect(cbb,&QComboBox::currentTextChanged,this,&CheckDlg::ChangeType);
    return;
}
void CheckDlg::CreateTableHeard(QStringList HeaderList,int iwidth)
{
    table->setColumnCount(HeaderList.count());
    table->horizontalHeader()->setDefaultSectionSize(iwidth/HeaderList.count());
    table->resizeColumnToContents(HeaderList.count());
    table->setHorizontalHeaderLabels(HeaderList);
    QFont font;
    font.setPointSize(13);
    font.setFamily("黑体");
    table->setFont(font);

    font.setBold(true);
    table->horizontalHeader()->setFont(font);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setDefaultSectionSize(20);
    table->setFrameShape(QFrame::NoFrame);
    table->setShowGrid(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->resizeSection(0,100);
    table->horizontalHeader()->resizeSection(1,100);
    table->horizontalHeader()->setFixedHeight(25);
    table->setStyleSheet("selection-background-color:lightblue;border-radius:15px;");
    TABLEWGDATA tablewg;
    SetTableWidgetData("全局",true,tablewg);
}
void CheckDlg::SetAutoCheckWg(int )
{
    QVector<QString> vec;
    GetCheckBox(vec);
    QMap<QString,TABLEWGDATA> temp;
    GetTableWidgetAxisData(temp);

    for(int i = table->rowCount(); i > 0;i--)
    {
        table->removeRow(i);
    }
    for(int i(0);i < vec.count();++i)
    {
        TABLEWGDATA tablewg;
        if(temp.contains(vec[i]))
        {
            tablewg = temp[vec[i]];
        }
        SetTableWidgetData(vec[i],false,tablewg);
    }
}

void CheckDlg::ChangeType(const QString &str)
{
    QObjectList list = wgCheck->children();
    foreach (QObject* obj, list)
    {
        delete obj;
    }
    if(str.compare("快速定位") == 0)
    {
        table->setVisible(true);
        lineEditAcc->setVisible(false);
        lineEditSpeed->setVisible(false);
        lableSpeed->setVisible(false);
        lableAcc->setVisible(false);
        TABLEWGDATA tablewg;
        SetTableWidgetData("全局",true,tablewg);
        QGridLayout *baseLayout = new QGridLayout(wgCheck);
        baseLayout->setSpacing(6);
        wgCheck->setLayout(baseLayout);
        uchar uc = 0;
        uchar irow = 0;
        for(auto item = m_hwSerOutputMap.begin();item != m_hwSerOutputMap.end();++item)
        {
            if(item.value().modelType == "1")
            {
                QCheckBox *cb = new QCheckBox(this);
                connect(cb,&QCheckBox::stateChanged,this,&CheckDlg::SetAutoCheckWg);
                cb->setFixedSize((wgCheck->width())/3,24);
                cb->setText(item.key());
                cb->setFont(font);
                baseLayout->addWidget(cb,irow,uc++);
                if(uc >= 3)
                {
                    ++irow;
                    uc = 0;
                }
            }
        }

    }
    else if(str.compare("直线插补") == 0)
    {
        table->setVisible(false);
        lineEditAcc->setVisible(true);
        lineEditSpeed->setVisible(true);
        lableSpeed->setVisible(true);
        lableAcc->setVisible(true);
        table->setRowCount(0);
        table->clearContents();
        QGridLayout *baseLayout = new QGridLayout(wgCheck);
        baseLayout->setSpacing(6);
        wgCheck->setLayout(baseLayout);
        uchar uc = 0;
        uchar irow = 0;
        for(auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.begin();
            item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.end();++item)
        {
            QCheckBox *cb = new QCheckBox(this);
            cb->setFixedSize((wgCheck->width())/3,24);
            cb->setText(item.key());
            cb->setFont(font);
            baseLayout->addWidget(cb,irow,uc++);
            if(uc >= 3)
            {
                ++irow;
                uc = 0;
            }
        }
    }
}
void CheckDlg::GetTableWidgetAxisData(QMap<QString,TABLEWGDATA> &temp)
{
    for(int i = 1;i < table->rowCount();++i)
    {
        TABLEWGDATA tablewgData;
        QCheckBox * cb = (QCheckBox*)table->cellWidget(i,2);
        tablewgData.ischecked = cb->isChecked();
        QString str = table->item(i,0)->text();
        QSpinBox *spb = (QSpinBox*)table->cellWidget(i,1);
        tablewgData.ispeed = spb->value();
        temp.insert(str,tablewgData);

    }
}
void CheckDlg::GetTableWidgetData(QMap<QString,int> &temp)
{
    for(int i = 0;i < table->rowCount();++i)
    {
        QCheckBox * cb = (QCheckBox*)table->cellWidget(i,2);
        if(cb->isChecked())
        {
            QString str = table->item(i,0)->text();
            QSpinBox *spb = (QSpinBox*)table->cellWidget(i,1);
            int ivalue = spb->value();
            temp.insert(str,ivalue);
        }
    }
}

void CheckDlg::SetTableWidgetData(QString strId, bool isFixScl, TABLEWGDATA tablewg)
{
    int irow = table->rowCount();
    table->insertRow(irow);
    QTableWidgetItem *item = new QTableWidgetItem(strId);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    item->setFont(font);
    table->setItem(irow,0,item);
    QSpinBox *spb = new QSpinBox();
    QCheckBox *cb = new QCheckBox(this);
    spb->setRange(1,100);

    spb->setValue(80);
    if(m_swqpStru.axisMap.contains(strId))
    {
        cb->setChecked(true);
        spb->setValue(m_swqpStru.axisMap[strId].speed);
    }
    spb->setFont(font);
    table->setCellWidget(irow,1,spb);

    //cb->setStyleSheet("QCheckBox::indicator{border: 1px solid white;border-radius:2px;width:15px;height:15px;} ");
    if(isFixScl)
    {
        cb->setChecked(true);
        cb->setEnabled(false);
        if(m_swqpStru.speed != -1)
        {
            spb->setValue(m_swqpStru.speed);
        }
    }
    else
    {
        if(tablewg.ispeed > -1)
        {
            spb->setValue(tablewg.ispeed);
            cb->setChecked(tablewg.ischecked);
        }
    }
    cb->setFixedSize(42,20);
    cb->setStyleSheet(tr("background-color:rgb(35,35,35);"));
    table->setCellWidget(irow,2,cb);
}

QString CheckDlg::GetConfigID()
{
    return lineEditId->text();
}

QString CheckDlg::GetConfigNote()
{
    return lineEditNote->text();
}
QString CheckDlg::GetConfigTimeout()
{
    if(lineEditTimeout->text().isEmpty())
        return QString("0");
    return lineEditTimeout->text();
}
QString CheckDlg::GetConfigDelay()
{
    if(lineEditDelay->text().isEmpty())
        return QString("0");
    return lineEditDelay->text();
}

QString CheckDlg::GetConfigCspSpeed()
{
    if(lineEditSpeed->text().isEmpty())
        return QString("0");
    return lineEditSpeed->text();
}

QString CheckDlg::GetConfigCspAcc()
{
    if(lineEditAcc->text().isEmpty())
        return QString("0");
    return lineEditAcc->text();
}
QString CheckDlg::GetConfigType()
{
    return cbb->currentText();
}
void CheckDlg::SetConfigID(QString str)
{
    lineEditId->setText(str);
}

void CheckDlg::SetConfigNote(QString str)
{
    lineEditNote->setText(str);
}
void CheckDlg::SetConfigTimeout(int itimeout)
{
    if(itimeout > 65535)
        itimeout = 65535;
    if(itimeout < 0)
        itimeout = 0;
    lineEditTimeout->setText(QString("%1").arg(itimeout));
}
void CheckDlg::SetConfigDelay(int idelay)
{
    if(idelay > 32767)
        idelay = 32767;
    if(idelay < 0)
        idelay = 0;

    lineEditDelay->setText(QString("%1").arg(idelay));
}

void CheckDlg::SetConfigCspSpeed(int ispeed)
{
    if(ispeed > 500)
        ispeed = 500;
    if(ispeed < 1)
        ispeed = 1;

    lineEditSpeed->setText(QString("%1").arg(ispeed));
}

void CheckDlg::SetConfigCspAcc(int iacc)
{
    if(iacc > 10000)
        iacc = 10000;
    if(iacc < 1)
        iacc = 1;

    lineEditAcc->setText(QString("%1").arg(iacc));
}
void CheckDlg::SetConfigType(QString str)
{
    cbb->setCurrentText(str);
}

void CheckDlg::OverAccept()
{
    QString strtype = cbb->currentText();
    bool isexist = true;
    if(strtype.compare("快速定位") == 0)
    {
        if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(lineEditId->text()))
        {
            isexist = false;
        }
    }
    else
    {
        if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_LCSP_Map.contains(lineEditId->text()))
        {
            isexist = false;
        }
    }
    if(!isexist)
    {
        if(lineEditId->text().isEmpty() || lineEditNote->text().isEmpty())
        {
            if(!CreateMessageBox("警告","参数Id或者注释没有填写,是否重新填写？请确认!"))
                reject();
        }
        else
            accept();
    }
    else
    {
        if(!m_isNewPoint)
             accept();
        else{
            if(!CreateMessageBox("警告","参数Id已经存在，是否重新填写？请确认!"))
            reject();
        }
    }
}
bool CheckDlg::CreateMessageBox(QString strTitle,QString note)
{
    QFont font;
    font.setPointSize(13);
    font.setFamily("黑体");
    QDialog box ;
    box.setWindowTitle(strTitle);
    box.setFixedSize(256,200);
    box.setFont(font);

    QLabel *lable = new QLabel(&box);
    lable->setText(note);
    lable->setFont(font);
    lable->setWordWrap(true);
    lable->setFixedSize(250,100);
    QVBoxLayout *vbox = new QVBoxLayout(&box);
    box.setLayout(vbox);

    vbox->addWidget(lable);

    box.setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    QHBoxLayout *hBox = new QHBoxLayout(&box);
    QPushButton *btn = new QPushButton("确认",this);
    connect(btn,&QPushButton::clicked,&box,&QDialog::accept);
    btn->setFont(font);
    QPushButton *btn1 = new QPushButton("取消",this);
    connect(btn1,&QPushButton::clicked,&box,&QDialog::reject);
     btn1->setFont(font);
     hBox->addWidget(btn);
     hBox->addWidget(btn1);
    vbox->addLayout(hBox);
    if(QDialog::Accepted == box.exec())
        return true;

    return false;


}

void CheckDlg::resize(int x, int y)
{
    CreateUI(x,y);
}
void CheckDlg::GetCheckBox(QVector<QString>& vec)
{
    QObjectList list = wgCheck->children();
    foreach (QObject* obj, list)
    {
        QString str = obj->metaObject()->className();
        if(str == "QCheckBox")
        {
            QCheckBox *cb = (QCheckBox*)obj;
            if(cb->isChecked())
            {
                vec.append(cb->text());
            }
        }
    }

}
void CheckDlg::SetCheckBox(QVector<QString> vec)
{
    QObjectList list = wgCheck->children();
    foreach (QObject* obj, list)
    {
        QString str = obj->metaObject()->className();
        if(str == "QCheckBox")
        {
            QCheckBox *cb = (QCheckBox*)obj;
            if(vec.contains(cb->text()))
            {
                cb->setChecked(true);
            }
        }
    }
}
