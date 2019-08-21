#include "handleuixml.h"

HandleUIXml::HandleUIXml()
{

}
QString HandleUIXml::ReadUIXml(QString strPath,UIXML_Map &uixml)
{
    QString msg("");
    QFile file(strPath);
    if(!file.exists())
    {
       msg = QString("%1 is not exists!").arg(strPath);

       return msg;
    }

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //构建QXmlStreamReader对象
        QXmlStreamReader reader(&file);
        while(!reader.atEnd())
        {
            reader.readNext();
            //判断是否是节点的开始
            if(reader.isStartElement())
            {
                //判断当前节点的名字是否为DebugUI
                if(reader.name() == "DebugUI")
                {
                    ReadUIType(reader,"DebugUI",uixml.uiXmlMap);
                }
                //判断当前节点的名字是否为MyTestUI
                else if(reader.name() == "MonitorUI")
                {
                    ReadUIType(reader,"MonitorUI",uixml.uiXmlMap);
                }
                else if(reader.name() == "ConfigUI")
                {
                    ReadUIType(reader,"ConfigUI",uixml.uiXmlMap);
                }
                else if(reader.name() == "ShowOtherUI")
                {
                    ReadUIType(reader,"ShowOtherUI",uixml.uiXmlMap);
                }

            }

            //节点结束、并且节点名字为Data（含有子节点）
            else if(reader.isEndElement() && reader.name() == "Data")
            {
                break;
            }
        }
        file.close();
    }
    else
    {
        msg = QString("Open file %1 failure").arg(strPath);
    }

    return msg;
}
void HandleUIXml::ReadUIType(QXmlStreamReader &reader,QString strElement,QMap<QString,QVector<UIXML_STRU>> &uixmlMap)
{
    QVector<UIXML_STRU> uiconVec;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "controls")
            {
                UIXML_STRU uiconStru;
                ReadControls(reader,uiconStru);
                uiconVec.append(uiconStru);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
    if(!uixmlMap.contains(strElement))
    {
        uixmlMap.insert(strElement,uiconVec);
    }
    else
    {
        uixmlMap[strElement].append(uiconVec);
    }
}
void HandleUIXml::ReadControls(QXmlStreamReader &reader,UIXML_STRU &uiconStru)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "编号")
            {
                uiconStru.id = reader.readElementText();
            }
            else if(reader.name() == "控件名")
            {
                //todo
                uiconStru.className = reader.readElementText();
            }
            else if(reader.name() == "图片路径")
            {
                //todo
                uiconStru.IcoPath = reader.readElementText();
            }
            else if(reader.name() == "文本内容")
            {
                //todo
                uiconStru.strText = reader.readElementText();
            }
            else if(reader.name() == "字体大小")
            {
                //todo
                uiconStru.fontsize = reader.readElementText().toInt();
            }
            else if(reader.name() == "地址")
            {
                //todo
                uiconStru.address = reader.readElementText();
            }
            else if(reader.name() == "信号类型")
            {
                //todo
                uiconStru.signalType = reader.readElementText();
            }
            else if(reader.name() == "模式")
            {
                //todo
                uiconStru.model = reader.readElementText();
            }
            else if(reader.name() == "颜色")
            {
                //todo
                uiconStru.strColor = reader.readElementText();
            }
            else if(reader.name() == "控件大小")
            {
                //todo
                QStringList qlt = reader.readElementText().split(",");
                if(qlt.count() > 1)
                    uiconStru.csize = QSize(QString(qlt[0]).toInt(),QString(qlt[1]).toInt());
            }
            else if(reader.name() == "事件")
            {
                //todo
                uiconStru.strEvent = reader.readElementText();
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == "controls")
            {
                break;
            }
        }
    }
}

QString HandleUIXml::WriteUIXml(QString strPath)
{
    QString msg("");

    QFile file(strPath);
    if(!file.exists())
    {
       msg = QString("%1 is not exists!").arg(strPath);
    }


    return msg;
}
