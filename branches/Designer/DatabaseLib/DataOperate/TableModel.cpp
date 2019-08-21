#include <QDebug>
#include <QMessageBox>
#include "TableModel.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

#define TABLE_COLUMN  4
TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    //初始化
    m_varbriefList = NULL;
    m_headerList.clear();
    m_nRow = -1;
}

void TableModel::setModelDatas(QList<QStringList> *list)
{
    m_varbriefList = list;
    m_nRow = m_varbriefList->size()-1;
}

void TableModel::SetNoEditRow(int row)
{
    m_nRow = row;
}

void TableModel::setHeaderList(QStringList list)
{
    m_headerList = list;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //设置表头数据
    if(role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal) // 水平表头
        {
            if(m_headerList.size() > section)
                return m_headerList[section];
            else
                return QVariant();
        }
        else
        {
            //            if(vertical_header_list.size() > section)
            //                return vertical_header_list[section]; // 垂直表头
            //            else
            //                return QVariant();
        }
    }
    return QVariant();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    //表格数据
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        //        QString text = m_varbriefList.at(row).at(column);
        return m_varbriefList->at(row).at(column);
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //表格数据变化
    if (data(index, role) != value)
    {
        // FIXME: Implement me!
        int row = index.row();
        int column = index.column();
        QVariant vatemp = value;
        QString strcurrenttext = vatemp.toString();
        if(strcurrenttext.isEmpty())
        {
            return false;
        }
        if(column == 1)
        {
            QString strPreText = this->data(index).toString();
            emit signalComboBoxChanged(row,strcurrenttext,strPreText);
        }
        if(column == 0)
        {
            for(int i =0; i < m_varbriefList->count(); i++)
            {
                if(strcurrenttext.compare(m_varbriefList->at(i).at(0))==0)
                {
                    QMessageBox::information(NULL,tr("警告"),tr("变量名重名,请重新输入"));
                    return false;
                }
            }

            if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(strcurrenttext)||strcurrenttext.contains("BUILT_IN_"))
            {
                QMessageBox::information(NULL,tr("警告"),tr("内置类型,不可重名"));
                return false;
            }
        }
        QString PreText = this->data(index).toString();//
        QStringList list =  m_varbriefList->at(row);
        list.replace(column,strcurrenttext);
        m_varbriefList->replace(row,list);       
        emit signalDataChanged(index,value,PreText);
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    Qt::ItemFlags flag = QAbstractItemModel::flags(index);
  //  qDebug()<<"可编辑的行数："<<m_nRow;
    if(index.row() > m_nRow || index.column() >0)
        flag|=Qt::ItemIsEditable; // 设置单元格可编辑
    else {
        // return (~Qt::ItemIsEditable | Qt::ItemIsSelectable);//不能编辑但是可以选中
      // flag = (index.flags()&(~Qt::ItemIsEditable));
        // return (Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
         return (Qt::ItemIsEnabled|Qt::ItemIsUserCheckable |Qt::ItemIsSelectable );
    }
    return flag;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if(NULL == m_varbriefList)
        return 0;
    else
        return m_varbriefList->size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return TABLE_COLUMN;
}


void TableModel::refrush()
{
    beginResetModel();
    endResetModel();
}




