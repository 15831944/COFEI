#include "handelflowtxt.h"
#include <QDir>
HandelFlowTxt::HandelFlowTxt()
{

}
/*---------主流程txt解析----------*/
QString HandelFlowTxt::ReadFlowTxt(QString strPath,FLOWTXT_MAP  &flowxml)
{
    QString msg("");//初始化

    QFile file(strPath);
    if(!file.exists())
    {
        msg = QString("%1 is not exists!").arg(strPath);
    }
    int index = 0;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))//只读方式打开文本文件
    {
        index = 0;
        QTextStream in(&file);//读写文本
        QString line = in.readLine();//读一行文本
        QMap<int,QMap<QString,FLOWTXT_STRU>> temp;//定义数据结构
        temp.clear();//初始化数据结构ReadChildFlowPath
        QString key = "主程序";
        while (!line.isNull() && !line.isEmpty())
        {
            ++index;
            if(line.indexOf("[") != -1 && line.indexOf("]") != -1)//判断第一行中是否有中括号【】，有的话替换掉
            {
                key = line.replace("[","").replace("]","");
                temp.clear();
                line = in.readLine();
                continue;
            }
            if(key.indexOf("主程序") != -1)
            {
                ReadFlow(line,index,flowxml.flowXmlMap);//解析主程序
            }
            else if(key.indexOf("子程序") != -1)
            {
                ReadFlow(line,index,flowxml.subflowXmlMap);//解析子程序
            }
            else//其他单行指令
            {
                ReadFlow(line,index,temp);
                if(!temp.isEmpty())
                {
                    if(flowxml.childFlowXmlMap.contains(key))
                    {
                        flowxml.childFlowXmlMap[key] = temp;//包含键，则插入数值
                    }
                    else
                    {
                        flowxml.childFlowXmlMap.insert(key,temp);//不包含键和值，则键和值一并插入数据结构中
                    }
                }
            }
            line = in.readLine();
        }
        if(flowxml.flowXmlMap.size() > 0)
        {
            flowxml.childFlowXmlMap.insert("主程序",flowxml.flowXmlMap);
        }
        file.close();
    }
    else
    {
     msg = QString("Open file %1 failure").arg(strPath);
    }

    return msg;
}
bool HandelFlowTxt::ReadFlow(QString strLine, int index, QMap<int,QMap<QString,FLOWTXT_STRU>> &flowxmlMap)
{
    strLine=strLine.replace(QRegExp("\t+")," ");// \t为制表符，+表示至少匹配一个
    strLine=strLine.replace(QRegExp(" +")," ");//  用一个空格代替多个空格
    QStringList qlt = strLine.split("//");     //以注释为标志截取
    qlt = QString(qlt[0]).split(",");          //以逗号分割 
    if(qlt.count() >= 2)//表示此时有指令
    {
        int iNum =  QString(qlt[0]).trimmed().toInt();//trimmed属性删除字符串两端的空格
        QMap<QString,FLOWTXT_STRU> tempMap;
        qlt = QString(qlt[1]).split(" ");//此处分隔第一个逗号后面的用户指令
        if(qlt.count() >= 1 )//如果有用户指令
        {
            QString name = QString(qlt[0]).trimmed();
            FLOWTXT_STRU flowstru;
            flowstru.command = name;
            if(qlt.count() == 2)
            {
                flowstru.id = QString(qlt[1]).trimmed();
            }
            else if(qlt.count() >= 3)
            {
                flowstru.id = QString(qlt[1]).trimmed();
                flowstru.istatus = QString(qlt[2]).trimmed();
            }
            if(!tempMap.contains(name))//    保证主程序和子程序为txt中最后一个
            {
                tempMap.insert(name,flowstru);
            }
            else
            {
                tempMap[name] = flowstru;
            }
        }
        else
        {
            return false;
        }
        if(flowxmlMap.contains(iNum))
        {
            flowxmlMap[iNum] = tempMap;
        }
        else
        {
            flowxmlMap.insert(iNum,tempMap);
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool HandelFlowTxt::ReadPath(QString strPath, FLOW_CMD_STRU &flowData)
{
    QDir dir(strPath);
    dir.setFilter(QDir::Dirs | QDir::Files |QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst);//优先显示目录
    QStringList filters;
    filters<<QString("*.txt")<<QString("*.Txt")<<QString("*.TXT") <<  QString("线程");
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int j = 0; j < list.size(); ++j)
    {
        QFileInfo fileInfo = list.at(j);
        if(fileInfo.isDir())
        {
            QDir fileDir(fileInfo.absoluteFilePath());
            fileDir.setFilter(QDir::Files |QDir::NoSymLinks);
            QStringList filters;
            filters<<QString("*.txt")<<QString("*.Txt")<<QString("*.TXT") ;
            fileDir.setNameFilters(filters);
            QFileInfoList list1 = fileDir.entryInfoList();
            for (int i = 0; i < list1.size(); ++i)
            {
                QFileInfo fInfo = list1.at(i);//文件信息
                QString name = fInfo.baseName();
                QMap<int,QMap<QString,FLOWTXT_STRU>> temp;
                if(ReadText(fInfo.filePath(),temp))
                {
                    if(flowData.threadCmdMap.contains(name))
                    {
                        flowData.threadCmdMap[name] = temp;
                    }
                    else
                    {
                        flowData.threadCmdMap.insert(name,temp);
                    }

                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if(fileInfo.baseName() == "子复位")
            {
                QMap<int,QMap<QString,FLOWTXT_STRU>> temp;
                if(ReadText(fileInfo.filePath(),temp))
                {
                    flowData.ResetCmdMap.swap(temp);
                }
                else
                {
                    return false;
                }
            }
            else if(fileInfo.baseName() == dir.dirName())
            {
                QMap<int,QMap<QString,FLOWTXT_STRU>> temp;
                if(ReadText(fileInfo.filePath(),temp))
                {
                    flowData.flowCmdMap.swap(temp);
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool HandelFlowTxt::ReadText(QString strPath, QMap<int, QMap<QString, FLOWTXT_STRU>> &CmdMap)
{
    bool isOK = false;
    QFile file(strPath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        isOK = true;
        int index = 0;
        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
            ReadFlow(line,++index,CmdMap);
            line = in.readLine();
        }
    }
    file.close();
    return isOK;
}
QString HandelFlowTxt::WriteFlowTxt(QString strPath)
{
    return strPath;
}
QString HandelFlowTxt::ReadChildFlowPath(QString strPath,FLOWTXT_MAP &flowXml)//解析子程序
{
     QString msg("");
     QDir dir(strPath);
     dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，此处表示只为文件格式
     QStringList filters;
     filters<<QString("*.txt")<<QString("*.Txt")<<QString("*.TXT");
     dir.setNameFilters(filters);
     QFileInfoList list = dir.entryInfoList();//获取过滤后所得到的文件夹下的文件信息列表
     int index = 0;
     for (int i = 0; i < list.size(); ++i)//size 返回文件字节大小
     {
         QFileInfo fileInfo = list.at(i);//文件信息
         QFile file(fileInfo.filePath());
         QString name = fileInfo.fileName();
         name = name.toUpper();//字母全部转换成大写
         name = name.replace(".TXT","");//删除文本格式后缀的txt
         if(file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
             index = 0;
             QTextStream in(&file);
              QString line = in.readLine();
              QMap<int,QMap<QString,FLOWTXT_STRU>> temp;
              while (!line.isNull()) {
                  ReadFlow(line,++index,temp);
                  line = in.readLine();
              }
             file.close();
             if(flowXml.childFlowXmlMap.contains(name))
             {
                 flowXml.childFlowXmlMap[name] = temp;
             }
             else
             {
                 flowXml.childFlowXmlMap.insert(name,temp);
             }

         }
         else
         {
             msg = QString("Open file %1 failure").arg(strPath);
         }
     }
     return msg;
}

QString HandelFlowTxt::ReadFlowPath(QString strPath, FLOWTXT_MAP &flowXml)
{
    QString msg("");
    QDir dir(strPath);
    dir.setFilter(QDir::Dirs |QDir::NoSymLinks);//设置类型过滤器
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)//size 返回文件字节大小
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        if(fileInfo.isDir())
        {
            QString flowName = fileInfo.fileName();
            FLOW_CMD_STRU flowStru;                     
            if(ReadPath(fileInfo.absoluteFilePath(),flowStru))
            {
                if(!strPath.contains("/运行配置/自动运行"))
                {
                    flowXml.FlowMap.insert(flowName,flowStru);
                }
                else{
                    flowXml.autoFlowMap.insert(flowName,flowStru);
                }
            }
            else
            {
                return QString("Open file %1 failure").arg(strPath);
            }

        }
    }
    return msg;
}

QString HandelFlowTxt::ReadORGPath(QString strPath, FLOWTXT_MAP &flowXml)
{
    FLOW_CMD_STRU flowStru;
    if(ReadPath(strPath,flowStru))
    {
        flowXml.FlowMap.insert("回原点",flowStru);
    }
    else
    {
        return QString("Open file %1 failure").arg(strPath);
    }
    return "";
}

QString HandelFlowTxt::ReadResetPath(QString strPath, FLOWTXT_MAP &flowXml)
{
    FLOW_CMD_STRU flowStru;
    if(ReadPath(strPath,flowStru))
    {
        flowXml.FlowMap.insert("总复位",flowStru);
    }
    else
    {
        return QString("Open file %1 failure").arg(strPath);
    }
    return "";
}
QString HandelFlowTxt::ReadResetFlowPath(QString strPath,FLOWTXT_MAP &flowXml)//解析子程序
{
    QString msg("");//初始化

    QFile file(strPath);
    if(!file.exists())
    {
        msg = QString("%1 is not exists!").arg(strPath);
    }
    int index = 0;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        index = 0;
        QTextStream in(&file);
        QString line = in.readLine();
        flowXml.reSetFlowMap.clear();
        while (!line.isNull())
        {
            ++index;
            ReadFlow(line,index,flowXml.reSetFlowMap);
            line = in.readLine();
        }
        file.close();
    }
    else
    {
        msg = QString("Open file %1 failure").arg(strPath);
    }

    return msg;
}

QString HandelFlowTxt::ReadChildResetFlowPath(QString strPath, FLOWTXT_MAP &flowXml)
{
    QString msg("");
    QDir dir(strPath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，此处表示只为文件格式
    QStringList filters;
    filters<<QString("*.txt")<<QString("*.Txt")<<QString("*.TXT");
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();//获取过滤后所得到的文件夹下的文件信息列表
    int index = 0;
    for (int i = 0; i < list.size(); ++i)//size 返回文件字节大小
    {
        QFileInfo fileInfo = list.at(i);//文件信息
        QFile file(fileInfo.filePath());
        QString name = fileInfo.fileName();
        name = name.toUpper();//字母全部转换成大写
        name = name.replace(".TXT","");//删除文本格式后缀的txt
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            index = 0;
            QTextStream in(&file);
             QString line = in.readLine();
             QMap<int,QMap<QString,FLOWTXT_STRU>> temp;
             while (!line.isNull())
             {
                 ReadFlow(line,++index,temp);
                 line = in.readLine();
             }
            file.close();
            if(flowXml.chlidResetXmlMap.contains(name))
            {
                flowXml.chlidResetXmlMap[name] = temp;
            }
            else
            {
                flowXml.chlidResetXmlMap.insert(name,temp);
            }

        }
        else
        {
            msg = QString("Open file %1 failure").arg(strPath);
        }
    }
    return msg;
}
QString HandelFlowTxt::ReadORGFlowPath(QString strPath,FLOWTXT_MAP &flowXml)//解析子程序
{
    QString msg("");//初始化

    QFile file(strPath);
    if(!file.exists())
    {
        msg = QString("%1 is not exists!").arg(strPath);
    }
    int index = 0;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        index = 0;
        QTextStream in(&file);
        QString line = in.readLine();
        flowXml.ORGFlowMap.clear();
        while (!line.isNull())
        {
            ReadFlow(line,++index,flowXml.ORGFlowMap);
            line = in.readLine();
        }
        file.close();
    }
    else
    {
        msg = QString("Open file %1 failure").arg(strPath);
    }

    return msg;
}
