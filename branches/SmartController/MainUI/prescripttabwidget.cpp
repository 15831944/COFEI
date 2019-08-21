#include "prescripttabwidget.h"
#include <databasemanager.h>
PrescriptTabWidget::PrescriptTabWidget(QStringList list, QWidget * parent):
    CBaseTabWidget(list,parent)
{
    DataBaseManager::GetInstance()->CreateDBTable(
    "create table PrescriptInfo(Name varchar(50) primary key, type varchar(50), MaxCount varchar(50), CurCount varchar(50), Note varchar(100))");

}

void PrescriptTabWidget::DeletePoint()
{
    int curow = this->currentRow();
    QString strname = this->item(curow, 0)->text();
    if(!strname.isEmpty())
        DataBaseManager::GetInstance()->ExcDeleteDb("PrescriptInfo",strname);
    this->removeRow(curow);

}

void PrescriptTabWidget::UpdataPoint()
{
    this->setRowCount(0);
    this->clearContents();
    QSqlQuery query;
    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT Name,type,MaxCount,CurCount,Note FROM PrescriptInfo");
    QStringList valueList;
    while(query.next())
    {
        valueList << query.value("Name").toString();
        valueList << query.value("type").toString();
        valueList << query.value("MaxCount").toString();
        valueList << query.value("CurCount").toString();
        valueList << query.value("Note").toString();
        int irow = this->rowCount();
        this->insertRow(irow);
        for( int i(0); i < valueList.size();++i)
        {
            QTableWidgetItem *item = new QTableWidgetItem(valueList[i]);
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            this->setItem(irow,i,item);
        }
    }
}

void PrescriptTabWidget::SavePoints()
{
    QStringList namelist;
    namelist << "Name" << "type" << "MaxCount" << "CurCount" << "Note";
    QSqlQuery query;
    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT Name,type,MaxCount,CurCount,Note FROM PrescriptInfo");
    QStringList valueList;
    while(query.next())
    {
        valueList << query.value("Name").toString();
    }
    for(int i = 0; i < this->rowCount();++i)
    {
        QStringList vlist;
        QString strName = "";
        for(int j = 0 ;j < namelist.size();++j)
        {
            strName = this->item(i, 0)->text();
            if(strName.isEmpty())
            {
                CreateMessageBox("错误",QString("Key:%1 信息为空,请检查!").arg(namelist[j]));
                break;
            }
            vlist << this->item(i, j)->text();
        }
        if(valueList.contains(strName))
        {
            QString exp = QString("Name = '%1'").arg(strName);
            DataBaseManager::GetInstance()->ExcUpdateDb("PrescriptInfo",namelist,vlist,exp);
        }
        else
            DataBaseManager::GetInstance()->ExcInsertDb("PrescriptInfo",namelist,vlist);
   }
}

