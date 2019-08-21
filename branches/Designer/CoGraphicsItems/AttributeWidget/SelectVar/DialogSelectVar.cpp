#include <QDebug>
#include <QMetaEnum>
#include <QHeaderView>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QLineEdit>
#include <QAction>
#include "DialogSelectVar.h"

DialogSelectVar::DialogSelectVar(QWidget *parent) : QDialog(parent)
{
    m_bIsHWSwitch = false;

    this->setWindowTitle(tr("变量选择"));
    this->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    this->resize(800,500);

    InitLayout();

    QStringList header;
    header << tr("对象名") << tr("对象类型");
    m_pVarTable->setHorizontalHeaderLabels(header);
    m_pVarTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pVarTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pVarTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pVarTable->horizontalHeader()->setStretchLastSection(true);
    m_pVarTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void DialogSelectVar::InitLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);

    QGroupBox *pGroup = new QGroupBox(tr("从数据中心选择"));
    QHBoxLayout *pTopLayout = new QHBoxLayout();
    pTopLayout->addWidget(pGroup);
    pMainLayout->addLayout(pTopLayout);

    QVBoxLayout *pGrpLayout = new QVBoxLayout();
    pGroup->setLayout(pGrpLayout);

    QHBoxLayout *pVarNameLayout = new QHBoxLayout();
    pGrpLayout->addLayout(pVarNameLayout);

    m_pLineEditVarName = TBase::LineEdit(tr("LineEditVar"));
    m_pLineEditVarName->setReadOnly(true);
    pVarNameLayout->addWidget(new QLabel(tr("选择变量")));
    pVarNameLayout->addWidget(m_pLineEditVarName);

    QHBoxLayout *pVarSelectLayout = new QHBoxLayout();
    pGrpLayout->addLayout(pVarSelectLayout);
    m_pChkNumber = TBase::CheckBox(tr("数值型"),tr("ChkNumber"));
    connect(m_pChkNumber,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    m_pChkSwitch = TBase::CheckBox(tr("开关类"),tr("ChkSwitch"));
    connect(m_pChkSwitch,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    m_pChkString = TBase::CheckBox(tr("字符串型"),tr("ChkString"));
    connect(m_pChkString,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    m_pChkTime = TBase::CheckBox(tr("计时类"),tr("ChkTime"));
    connect(m_pChkTime,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    m_pChkCount = TBase::CheckBox(tr("计数类"),tr("ChkCount"));
    connect(m_pChkCount,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    m_pChkRealTime = TBase::CheckBox(tr("实时变量"),tr(("")));
    m_pChkRealTime->hide();
    connect(m_pChkRealTime,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryRealTimeVar);

    m_pChkNumber->hide();
    m_pChkSwitch->hide();
    m_pChkString->hide();
    m_pChkTime->hide();
    m_pChkCount->hide();

    pVarSelectLayout->addWidget(m_pChkNumber);
    pVarSelectLayout->addWidget(m_pChkSwitch);
    pVarSelectLayout->addWidget(m_pChkString);
    pVarSelectLayout->addWidget(m_pChkTime);
    pVarSelectLayout->addWidget(m_pChkCount);
    pVarSelectLayout->addWidget(m_pChkRealTime);
    pVarSelectLayout->addStretch(1);

    m_pSearchEdit = new QLineEdit();
    connect(m_pSearchEdit,&QLineEdit::editingFinished,this,&DialogSelectVar::DoSearch);
    QAction *pSearchAction = new QAction();
    pSearchAction->setIcon(QIcon(":/Images/search.png"));
    m_pSearchEdit->addAction(pSearchAction,QLineEdit::TrailingPosition);
    connect(pSearchAction,&QAction::triggered,this,&DialogSelectVar::DoSearch);
    pVarSelectLayout->addWidget(m_pSearchEdit);

    QVBoxLayout *pBtnLayout = new QVBoxLayout();
    pTopLayout->addLayout(pBtnLayout);
    m_pBtnOK = TBase::Button(tr("确认"),tr("BtnOK"),60,30);
    m_pBtnOK->setAutoDefault(false);
    connect(m_pBtnOK,&QPushButton::clicked,this,&DialogSelectVar::SendVarName);
    m_pBtnQuit = TBase::Button(tr("退出"),tr("BtnQuit"),60,30);
    m_pBtnQuit->setAutoDefault(false);
    connect(m_pBtnQuit,&QPushButton::clicked,this,&DialogSelectVar::close);

    pBtnLayout->addSpacing(15);
    pBtnLayout->addWidget(m_pBtnOK,Qt::AlignBottom);
    pBtnLayout->addWidget(m_pBtnQuit,Qt::AlignBottom);

    m_pVarTable = new QTableWidget();
    pMainLayout->addWidget(m_pVarTable);
    connect(m_pVarTable,&QTableWidget::itemSelectionChanged,this,&DialogSelectVar::TableSelectChanged);
    connect(m_pVarTable,&QTableWidget::itemDoubleClicked,this,&DialogSelectVar::SendVarName);
}

void DialogSelectVar::SetIsRealtimeVar(bool visible)
{
    if(visible)
        m_pChkRealTime->show();
    if((!m_pChkNumber->isChecked()) && (!m_pChkString->isChecked()) && (!m_pChkSwitch->isChecked())
            && (!m_pChkTime->isChecked()) && (!m_pChkCount->isChecked()))
        m_pChkRealTime->setChecked(true);
}

void DialogSelectVar::IsHWSwitch(bool b)
{
    m_bIsHWSwitch = b;
    if(m_bIsHWSwitch)
    {
        //重新筛选一次，仅显示硬件io对应的开关量
        QueryVar();
    }
}

void DialogSelectVar::SetSelectVarType(QString type)
{
    QStringList list = type.split('+');
    if(list.size() < 1)
        return;

    if(list.contains(tr("Num")))
    {
        m_pChkNumber->show();
        m_pChkNumber->setChecked(true);
    }

    if(list.contains(tr("Switch")))
    {
        m_pChkSwitch->show();
        m_pChkSwitch->setChecked(true);
    }

    if(list.contains(tr("String")))
    {
        m_pChkString->show();
        m_pChkString->setChecked(true);
    }

    if(list.contains(tr("Time")))
    {
        m_pChkTime->show();
        m_pChkTime->setChecked(true);
    }

    if(list.contains(tr("Count")))
    {
        m_pChkCount->show();
        m_pChkCount->setChecked(true);
    }
}

void DialogSelectVar::QueryVar()
{
    if(m_pChkRealTime->isVisible())
    {
        m_pChkRealTime->disconnect();
        m_pChkRealTime->setChecked(false);
        connect(m_pChkRealTime,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryRealTimeVar);
    }

    QList<QList<QString>> dataList;
    dataList.clear();

    VARCONFIG_STRU varStu =  DBInstanceManager::getDBInstance()->DBVarStru;
    //数值型变量
    if(m_pChkNumber->isChecked())
    {
        QList<QString> names = varStu.regnumMap.keys();
        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name << tr("数值型寄存器");
            dataList.append(tmp);
        }
    }

    //开关型变量
    if(m_pChkSwitch->isChecked())
    {
        QList<QString> names = varStu.SwitchMap.keys();
        //筛选出硬件IO
        if(m_bIsHWSwitch)
        {
            names.clear();
            QMap<QString,VARDB_SWITCH_STRU> map = varStu.SwitchMap;
            foreach (VARDB_SWITCH_STRU stu, map) {
                if(stu.ishwIO)
                {
                    QString tmp(stu.pkey);
                    names.append(tmp);
                }
            }
        }

        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name << tr("开关类");
            dataList.append(tmp);
        }
    }

    //字符串型变量
    if(m_pChkString->isChecked())
    {
        QList<QString> names = varStu.regtextMap.keys();
        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name << tr("字符串型寄存器");
            dataList.append(tmp);
        }
    }

    //计时类变量
    if(m_pChkTime->isChecked())
    {
        QList<QString> names = varStu.timeMap.keys();
        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name << tr("计时类");
            dataList.append(tmp);
        }
    }

    //计数类变量
    if(m_pChkCount->isChecked())
    {
        QList<QString> names = varStu.countMap.keys();
        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name << tr("计数类");
            dataList.append(tmp);
        }
    }

    ResetTableByData(dataList);
    QStringList header;
    header << tr("对象名") << tr("对象类型");
    m_pVarTable->setHorizontalHeaderLabels(header);
}

void DialogSelectVar::QueryRealTimeVar()
{
    m_pChkNumber->disconnect();
    m_pChkSwitch->disconnect();
    m_pChkString->disconnect();
    m_pChkTime->disconnect();
    m_pChkCount->disconnect();
    m_pChkNumber->setChecked(false);
    m_pChkSwitch->setChecked(false);
    m_pChkString->setChecked(false);
    m_pChkTime->setChecked(false);
    m_pChkCount->setChecked(false);
    connect(m_pChkNumber,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    connect(m_pChkSwitch,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    connect(m_pChkString,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    connect(m_pChkTime,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);
    connect(m_pChkCount,&QCheckBox::stateChanged,this,&DialogSelectVar::QueryVar);

    QList<QList<QString>> dataList;
    dataList.clear();

    if(m_pChkRealTime->isChecked())
    {
        QList<QString> names = DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.keys();
        foreach(QString name,names)
        {
            QList<QString> tmp;
            tmp << name;
            dataList.append(tmp);
        }
    }
    ResetTableByData(dataList);
    QStringList header;
    header << tr("对象名");
    m_pVarTable->setHorizontalHeaderLabels(header);
}

void DialogSelectVar::DoSearch()
{
    QString text = m_pSearchEdit->text();
    if (text.isEmpty())
    {
        for (int i = 0; i < m_pVarTable->rowCount(); i++)
        {
            m_pVarTable->setRowHidden(i, false);
        }
    }
    else
    {
        //获取符合条件的cell索引
        QList <QTableWidgetItem *> item = m_pVarTable->findItems(text, Qt::MatchContains);
        for (int i = 0; i < m_pVarTable->rowCount(); i++)
        {
            m_pVarTable->setRowHidden(i, true);
        }

        if (!item.isEmpty())
        {
            for (int i = 0; i < item.count(); i++)
            {
                m_pVarTable->setRowHidden(item.at(i)->row(),false);
            }
        }
    }
}

void DialogSelectVar::TableSelectChanged()
{
    int row = m_pVarTable->currentRow();
    QString name =  m_pVarTable->item(row,0)->text();
    m_pLineEditVarName->clear();
    m_pLineEditVarName->setText(name);
}

void DialogSelectVar::SendVarName()
{
    QString name = m_pLineEditVarName->text();
    if(name.isEmpty())
        return;

    int type = -1;
    if(m_pVarTable->columnCount() > 1)
        type = GetVarTypeByName(m_pVarTable->item(m_pVarTable->currentRow(),1)->text());

    emit SendVar(name,QString::number(type));
    this->close();
}

void DialogSelectVar::ResetTableByData(QList<QList<QString>> &dataList)
{
    m_pVarTable->clear();
    m_pVarTable->setRowCount(0);
    m_pVarTable->setColumnCount(0);

    if(dataList.size() > 0)
    {
        int row = dataList.size();
        int column = dataList.at(0).size();
        m_pVarTable->setRowCount(row);
        m_pVarTable->setColumnCount(column);

        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < column; j++)
                m_pVarTable->setItem(i,j,new QTableWidgetItem(dataList.at(i).at(j)));
        }
        m_pVarTable->setColumnWidth(0,280);
    }
}

int DialogSelectVar::GetVarTypeByName(const QString &varName)
{
    if(varName == tr("字符串型寄存器"))
    {
        return 0;
    }
    else if(varName == tr("数值型寄存器"))
    {
        return 1;
    }
    else if(varName == tr("开关类"))
    {
        return 2;
    }
    else if(varName == tr("计时类"))
    {
        return 3;
    }
    else if(varName == tr("计数类"))
    {
        return 4;
    }
    else if(varName == tr("气缸类"))
    {
        return 5;
    }
    else if(varName == tr("运动控制PP模式"))
    {
        return 6;
    }
    else if(varName == tr("运动控制CSP模式"))
    {
        return 7;
    }
    return -1;
}

