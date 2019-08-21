#include <QDebug>
#include <QScrollArea>
#include <QGridLayout>
#include "NewPPDialog.h"

NewPPDialog::NewPPDialog(QWidget *parent) : QDialog(parent)
{
    m_isNew = true;
    this->setWindowTitle(tr("新建P点"));
    this->resize(600,500);

    QFont font("黑体",13);
    QVBoxLayout *vBox = new QVBoxLayout(this);
    this->setLayout(vBox);

    QWidget *wgHeader = new QWidget(this);
    wgHeader->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:5px;"));
    QLabel *lableType = new QLabel(tr("定位类别"));
    lableType->setFont(font);
    QLabel *lableId = new QLabel(tr("参数ID"));
    lableId->setFont(font);
    QLabel *lableTimeout = new QLabel(tr("超时"));
    lableTimeout->setFont(font);
    QLabel *lableDelay = new QLabel(tr("延迟"));
    lableDelay->setFont(font);
    QLabel *labelDcc = new QLabel(tr("加速度"));
    labelDcc->setFont(font);
    QLabel *labelAcc = new QLabel(tr("减速度"));
    labelAcc->setFont(font);

    cbb = new QComboBox(this);
    QStringList list;
    list << "快速定位" /*<< "直线插补"*/;
    cbb->addItems(list);
    cbb->setFixedSize(150,30);
    cbb->setFont(font);

    lineEditId = new QLineEdit(this);
    connect(lineEditId,&QLineEdit::editingFinished,this,&NewPPDialog::IDChanged);
    lineEditId->setFixedHeight(30);
    lineEditId->setFont(font);

    lineEditTimeout = new QLineEdit(this);
    lineEditTimeout->setFixedHeight(30);
    lineEditTimeout->setFont(font);
    lineEditTimeout->setValidator(new QIntValidator(0, 65535, this));
    lineEditTimeout->setText("15000");

    lineEditDelay = new QLineEdit(this);
    lineEditDelay->setFont(font);
    lineEditDelay->setValidator(new QIntValidator(0, 32767, this));
    lineEditDelay->setText("0");

    lineEditDec = new QLineEdit(this);
    lineEditDec->setFixedHeight(30);
    lineEditDec->setFont(font);
    lineEditDec->setValidator(new QIntValidator(1, 500, this));
    lineEditDec->setText("500");

    lineEditAcc = new QLineEdit(this);
    lineEditAcc->setFixedHeight(30);
    lineEditAcc->setFont(font);
    lineEditAcc->setValidator(new QIntValidator(1, 10000, this));
    lineEditAcc->setText("1000");

    QGridLayout *gridLayoutHeader = new QGridLayout(wgHeader);
    gridLayoutHeader->addWidget(lableType,0,0,Qt::AlignRight);
    gridLayoutHeader->addWidget(cbb,0,1,Qt::AlignLeft);
    gridLayoutHeader->addWidget(lableId,1,0,Qt::AlignRight);
    gridLayoutHeader->addWidget(lineEditId,1,1,Qt::AlignLeft);
    gridLayoutHeader->addWidget(lableTimeout,2,0,Qt::AlignRight);
    gridLayoutHeader->addWidget(lineEditTimeout,2,1,Qt::AlignLeft);
    gridLayoutHeader->addWidget(lableDelay,2,2,Qt::AlignRight);
    gridLayoutHeader->addWidget(lineEditDelay,2,3,Qt::AlignLeft);
    gridLayoutHeader->addWidget(labelAcc,3,0,Qt::AlignRight);
    gridLayoutHeader->addWidget(lineEditAcc,3,1,Qt::AlignLeft);
    gridLayoutHeader->addWidget(labelDcc,3,2,Qt::AlignRight);
    gridLayoutHeader->addWidget(lineEditDec,3,3,Qt::AlignLeft);
    wgHeader->setLayout(gridLayoutHeader);
    vBox->addWidget(wgHeader);
    QScrollArea *pArea = new QScrollArea(this);
    checkLayout = new QGridLayout();
    pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    checkLayout->setSpacing(6);
    pArea->setLayout(checkLayout);
    vBox->addWidget(pArea,1);

    QScrollArea *pAreaAuto = new QScrollArea(this);
    table = new QTableWidget();
    table->setColumnCount(2);
    table->setColumnWidth(0,100);
    table->setColumnWidth(1,100);
    QHBoxLayout *tableLayout = new QHBoxLayout();
    tableLayout->addWidget(table);
    QStringList Headlist;
    Headlist << tr("轴的ID") << tr("速度比率");
    table->setHorizontalHeaderLabels(Headlist);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(true);
    pAreaAuto->setLayout(tableLayout);
    vBox->addWidget(pAreaAuto,3);

    QHBoxLayout *hBox = new QHBoxLayout();
    QPushButton *cancel = new QPushButton("取消",this);
    cancel->setAutoDefault(false);
    connect(cancel,&QPushButton::clicked,this,&NewPPDialog::reject);
    QPushButton *ok = new QPushButton("确认",this);
    connect(ok,&QPushButton::clicked,this,&NewPPDialog::OverAccept);
    hBox->addStretch(1);
    hBox->addWidget(ok);
    hBox->addWidget(cancel);
    vBox->addLayout(hBox);

    connect(cbb,&QComboBox::currentTextChanged,this,&NewPPDialog::ChangeType);
    this->setStyleSheet(tr("QLineEdit,QComboBox,QTableWidget{background-color:rgb(63, 147, 168);color:white; border: 2px solid white;}"
                           "QCheckBox{background-color:rgb(63, 147, 168);color:white;}"
                           "QScrollArea{background-color:rgb(63, 147, 168);color:white;border-radius:15px;}"));

}

void NewPPDialog::SetHWParm(QMap<QString, HWCONFIGXML_SEROUTPUT_STRU> HWConfig)
{
    m_hwSerOutputMap = HWConfig;

    uchar uc = 0;
    uchar irow = 0;
    for(auto item = m_hwSerOutputMap.begin();item != m_hwSerOutputMap.end();++item)
    {
        if(item.value().modelType == "1")
        {
            QCheckBox *cb = new QCheckBox(this);
            connect(cb,&QCheckBox::stateChanged,this,&NewPPDialog::SetAutoCheck);
            cb->setFixedSize(40,24);
            cb->setText(item.key());
            checkLayout->addWidget(cb,irow,uc++);
            if(uc >= 3)
            {
                ++irow;
                uc = 0;
            }
        }
    }
}

void NewPPDialog::SetIsNew(bool isnew)
{
    m_isNew = isnew;
}

void NewPPDialog::ChangeType(const QString &)
{

}

void NewPPDialog::SetAutoCheck(int state)
{
    QCheckBox *chk = qobject_cast<QCheckBox*>(sender());
    QString text = chk->text();
    if(state == Qt::Unchecked)
    {
        QList<QTableWidgetItem*> items = table->findItems(text,Qt::MatchContains);
        foreach (QTableWidgetItem* item, items) {
            table->removeRow(item->row());
        }
    }
    else if(state == Qt::Checked)
    {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row,0,new QTableWidgetItem(chk->text()));
        QSpinBox *spb = new QSpinBox();
        spb->setRange(1,100);
        spb->setValue(80);
        table->setCellWidget(row,1,spb);
    }
}

void NewPPDialog::OverAccept()
{
    if(lineEditId->text().isEmpty())
    {
        CreateMessageBox(tr("警告"),tr("参数Id未填写,是否重新填写？请确认!"));
        return;
    }

    if(table->rowCount() < 1)
    {
        CreateMessageBox(tr("警告"),tr("未选择轴，请确认！"));
        return;
    }

    SWCONFIGXML_QPOSIT_STRU pos;
    pos.id = lineEditId->text();
    pos.acc = lineEditAcc->text().toInt();
    pos.dec = lineEditDec->text().toInt();
    pos.delayMsec = lineEditDelay->text().toInt();
    pos.timeOut = lineEditTimeout->text().toInt();
    pos.axisNum = table->rowCount();
    for(int i = 0; i < table->rowCount(); i++)
    {
        AXISINFO_STRU stu;
        QSpinBox* spb = (QSpinBox*)table->cellWidget(i,1);
        stu.speed = spb->value();
        pos.axisMap.insert(table->item(i,0)->text(),stu);
    }
    emit SendPP(pos);

    if(!m_isNew)
    {
        CreateMessageBox(tr("警告"),tr("该P点已存在，请修改ID"));
        return;
    }
    this->close();
}

void NewPPDialog::IDChanged()
{
    m_isNew = true;
}

bool NewPPDialog::CreateMessageBox(QString strTitle, QString note)
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

    box.setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:15px;"));
    QHBoxLayout *hBox = new QHBoxLayout(&box);
    QPushButton *btn = new QPushButton("确认",this);
    connect(btn,&QPushButton::clicked,&box,&QDialog::accept);
    btn->setFont(font);
    hBox->addWidget(btn);
    vbox->addLayout(hBox);
    if(QDialog::Accepted == box.exec())
        return true;

    return false;
}

