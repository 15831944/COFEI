#include <QMessageBox>
#include "TableView.h"
#include <QDebug>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    m_pModel = new TableModel();
    this->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setStretchLastSection(true); // 设置最后一列填充
    this->setStyleSheet("QTableView {alternate-background-color: rgb(57, 133, 152);}");//设置交替颜色样式
    this->setAlternatingRowColors(true);
}

void TableView::setData(QList<QStringList> &list)
{
    m_variableList = list;
    m_pModel->setModelDatas(&m_variableList);
    m_pModel->refrush();

}

void TableView::SetHeaderData(QStringList strHeaderList)
{
    m_pModel->setHeaderList(strHeaderList);
    this->setModel(m_pModel);
    if(strHeaderList.size() >= 4)
    {
        QStringList strTypeList;
        strTypeList<<tr("字符串型寄存器")<<tr("数值型寄存器")<<tr("开关类")
                  <<tr("计时类")<<tr("计数类")<<tr("气缸类")<<tr("运动控制PP模式")<<tr("运动控制CSP模式");
        ComboBoxDelegate* m_pComboBoxDelegate = new ComboBoxDelegate(strTypeList,this);
        m_pComboBoxDelegate->m_changeinfo = tr("您在修改变量属性类型,修改后该变量的属性值将被清空,是否继续?");
        QStringList fkeyList;
//                for(auto it =DBInstanceManager::getDBInstance()->DBProInfoStru.proinfoMap.begin();
//                    it!=DBInstanceManager::getDBInstance()->DBProInfoStru.proinfoMap.end();++it)
//                {
        QSqlQuery query = DBInstanceManager::getDBInstance()->m_pBDataBase->ExcBatchSelectDb("Productioninformation");
        while(query.next())
        {
            QString pkey = query.value(0).toString();
            fkeyList.append(pkey);
        }
        //   }
        ComboBoxDelegate* fkeyComboBoxDelegate = new ComboBoxDelegate(fkeyList,this);
        //在第二列，第四列，添加combobox代理
        this->setItemDelegateForColumn(1, m_pComboBoxDelegate);
        this->setItemDelegateForColumn(3, fkeyComboBoxDelegate);
        connect(m_pModel,&TableModel::signalComboBoxChanged,this,&TableView::slotTypeAttriChanged, Qt::QueuedConnection);
        connect(m_pModel,&TableModel::signalDataChanged,this,&TableView::slotdatachanged, Qt::QueuedConnection);
        connect(this->selectionModel(),&QItemSelectionModel::currentRowChanged,this,&TableView::slotCurrentRowChanged);
    }
    //设置列宽
    int nColumWidth = this->width() / strHeaderList.count();
    for(int i = 0; i < strHeaderList.count(); i++)
    {
        this->setColumnWidth(i,nColumWidth);
    }
}

void TableView::SetNoEditRow(int row)
{
    GetTablemodel()->m_nRow = row;
}

void TableView::slotAddRow()
{
    int sizerow = m_variableList.size();
    if(sizerow > 0)
    {
        if((m_variableList.at(sizerow -1).at(0)=="")||(m_variableList.at(sizerow -1).at(3) ==""))
        {
            QMessageBox::information(NULL,tr("警告"),tr("变量名字或机种编号不可为空！"));
            return;
        }
    }
    //添加一行
    QStringList list;
    list.clear();
    list<<""<<""<<""<<""<<"";
    m_variableList.append(list);
    m_pModel->refrush();
}

void TableView::slotBatchAddRow()//批量添加多行
{

}

void TableView::slotRemove()
{   
    //删除选中行
    QModelIndexList modelIndexList = this->selectedIndexes();

    int nModeCount = modelIndexList.count();
    if(nModeCount <= 0)
    {
        return;
    }
    int ret = QMessageBox::warning(NULL, tr("提示"),
                                   tr("删除后,不可恢复,请谨慎操作!"),
                                   QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;
    // 获取选中行的行号列表
    QList<int> rowList;
    rowList.clear();
    QMap<QString,QList<QString>> typepkeymap;
    for(int i = nModeCount-1; i>=0; i--)
    {
        QModelIndex modelIndex = modelIndexList.at(i);
        int row = modelIndex.row();
        if(!rowList.contains(row))
        {
            rowList.append(row);
            QModelIndex index = m_pModel->index(row,0);
            QVariant data = m_pModel->data(index);
            QModelIndex typeindex = m_pModel->index(row,1);
            QVariant datatype = m_pModel->data(typeindex);
            if(typepkeymap.contains(datatype.toString()))
            {
                QList<QString> pkeys;
                pkeys = typepkeymap[datatype.toString()];
                pkeys.append(data.toString());
                typepkeymap[datatype.toString()] = pkeys;
            }
            else {
                QList <QString> pkeys;
                pkeys.append(data.toString());
                typepkeymap.insert(datatype.toString(),pkeys);
            }
        }
    }
    if(rowList.isEmpty())
    {
        return;
    }
   // qSort(rowList);
     std::sort(rowList.begin(), rowList.end());
    for(int i = rowList.count()-1; i >= 0; i--)
    {
        m_variableList.removeAt(rowList.at(i));
        if(rowList.at(i) <= GetTablemodel()->m_nRow)
        {
            GetTablemodel()->m_nRow = GetTablemodel()->m_nRow -1;
        }
    }
    m_pModel->refrush();
    emit signalDeleteRows(typepkeymap);
}

void TableView::slotFindname(QString name)
{
    bool find = false;
    for(int i = 0 ; i < m_variableList.size();++i)
    {
        if(m_variableList.at(i).at(0) == name)
        {
            this->selectRow(i);
            find = true;
            break;
        }
    }
    if(!find)
    {
        QMessageBox::warning(this, tr("Warning"), tr("变量不存在!"));
        return;
    }
}

void TableView::slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    int nCurrentRow = current.row();
    QString strType = m_variableList.at(nCurrentRow).at(1);
    QString strname = m_variableList.at(nCurrentRow).at(0);
    if(strType!= "")
    {
        emit signalSelectedRowChange(strType,strname);
    }
}
void TableView::slotdatachanged(const QModelIndex &index, const QVariant &value,QString pretext)
{
    //所有的数据载入
    int row = index.row();
    int col = index.column();
    QString strType = m_variableList.at(row).at(1);
    QString strname = m_variableList.at(row).at(0);
    QString strnote = m_variableList.at(row).at(2);
    QString strfkey = m_variableList.at(row).at(3);
    if(col == 1)
    {
        strType = value.toString();
    }
    QStringList listvalue;
    listvalue<<strname<<strType<<strnote<<strfkey;
    emit signalDataChanged(index,listvalue,pretext);

}

void TableView::slotTypeAttriChanged(int row, const QString &value, QString strPreText)
{
    QString type = value;
    QString strType = m_variableList.at(row).at(1);
    QString strname = m_variableList.at(row).at(0);
    QString strnote = m_variableList.at(row).at(2);
    QString strfkey = m_variableList.at(row).at(3);
    QStringList listvalue;
    listvalue<<strname<<type<<strnote<<strfkey;
    emit signalTypeAttriChanged(listvalue,strPreText);
}
