#include "XmlReader.h"

#include <LibBase.h>

#include "../GraphicsItems.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "../TBasicControls/BasicControlsPub.h"
#include "../CoProxyItem/nightsetwidget.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
extern bool DEF_RUN_AS_DESIGNER;
TXmlReader::TXmlReader()
{
    m_IsOpen = false;
}

TXmlReader::~TXmlReader()
{
}

bool TXmlReader::Open(const QString &FileName)
{
    QFile mFile(FileName);
    m_IsOpen = false;
    if(mFile.open(QFile::ReadOnly | QFile::Text))
    {
        if(mDoc.setContent(&mFile, false))
        {
            m_IsOpen = true;
        }
    }
    mFile.close();
    return m_IsOpen;
}

bool TXmlReader::IsOpen()
{
    return m_IsOpen;
}

QDomElement TXmlReader::GetRoot()
{
    return mDoc.documentElement();
}
CoProxyWidget* TXmlReader::AnalyWidget(QDomElement mXmlNode, CoScene *Scene)
{
    CoProxyWidget *Widget = NULL;

    int Type = mXmlNode.attribute(tr("Type")).toInt();

    double StartX = mXmlNode.attribute(tr("StartX")).toDouble();
    double StartY = mXmlNode.attribute(tr("StartY")).toDouble();
    double StopX = mXmlNode.attribute(tr("StopX")).toDouble();
    double StopY = mXmlNode.attribute(tr("StopY")).toDouble();
    int LineWidth = mXmlNode.attribute(tr("LineWidth")).toInt();
    int LineStyle = mXmlNode.attribute(tr("LineStyle")).toInt();

    QColor LineColor(mXmlNode.attribute(tr("LineColor")));
    QColor BackColor(mXmlNode.attribute(tr("BackColor")));
    Widget = TItemFactory::CreateProxyWidget(Type, StartX, StartY, StopX, StopY,
                                             Qt::PenStyle(LineStyle), LineWidth, LineColor, BackColor,Scene);

    if(Widget == NULL)
        return Widget;
    Widget->SetId(mXmlNode.attribute(tr("Id")).toInt());
    Widget->SetPower(mXmlNode.attribute(tr("Power")).toInt());
    Widget->SetZValue(mXmlNode.attribute(tr("Z")).toDouble());
    Widget->SetRotateCenter(mXmlNode.attribute(tr("RotateX")).toDouble(),
                           mXmlNode.attribute(tr("RotateY")).toDouble());
    Widget->SetRotateAngle(mXmlNode.attribute(tr("Rotate")).toDouble());

    switch (Type) {
    case CoItem::Mde_Safe_Sig:
    {
        NightSetWidget *widget = dynamic_cast<NightSetWidget *>(Widget);
        if(widget)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            QMap<QString,SENSOR_LIGHT_STRU> Temp1;
            AnalySigLevel(mXmlBase.attribute(tr("JsonValue")),Temp1);
            widget->SetTabWidgetValue(Temp1);
            if(!DEF_RUN_AS_DESIGNER)
            {
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map.swap(Temp1);
            }
        }
    }
        break;
    default:
        break;
    }

    return Widget;
}

void TXmlReader::AnalySafeState(const QString& str,QMap<QString,SWCONFIGXML_SAFESTATE_STRU>& temp)
{
    QString strHead = "<?xml version = '1.0' encoding = 'UTF-8'?>";
    QString strContent = strHead + tr("<Data>%1</Data>").arg(str);
    //strContent.replace(QRegExp("\n+"),"");// \t为制表符，+表示至少匹配一个
    QDomDocument Doc;
    int errorLine;
    int errorCol;
    QString errorStr;
    if(Doc.setContent(strContent,false,&errorStr,&errorLine,&errorCol))
    {
        QDomElement mXmlScada = Doc.documentElement();
        if(mXmlScada.isNull() || mXmlScada.tagName() != tr("Data"))
        {
            return;
        }
        QDomElement mXmlScene = mXmlScada.firstChildElement("安全信息");
        while(!mXmlScene.isNull())
        {
            QString Id = mXmlScene.attribute(tr("ID"));
            SWCONFIGXML_SAFESTATE_STRU swSafeState;
            QDomElement mXmlLevel = mXmlScene.firstChildElement("信号等级");
            if(!mXmlLevel.isNull())
            {
                swSafeState.level = mXmlLevel.text();
            }
            QDomElement mXmlSigVec = mXmlScene.firstChildElement("信号点位集合");
            if(!mXmlSigVec.isNull())
            {
                //信号点位
                QDomElement mXmlSig = mXmlSigVec.firstChildElement("信号点位");
                while(!mXmlSig.isNull())
                {
                    SWCONFIGXML_SAFEBITINFO_STRU swSafeBit;

                    swSafeBit.sensor_bit.bitName = mXmlSig.attribute(tr("bitname"));
                    swSafeBit.sensor_bit.stat = mXmlSig.attribute(tr("value")).toInt();
                    swSafeBit.info = mXmlSig.firstChildElement("输出状态信息").text();
                    swSafeState.BitInfoVec.append(swSafeBit);
                    mXmlSig = mXmlSig.nextSiblingElement("信号点位");
                }
            }
            temp.insert(Id,swSafeState);
            mXmlScene = mXmlScene.nextSiblingElement("安全信息");
        }
    }
    else
        qDebug() << "TXmlReader::AnalySafeState "<< errorStr << "line: " << errorLine << "col: " << errorCol;
}

void TXmlReader::AnalySigLevel(const QString & strJson, QMap<QString, SENSOR_LIGHT_STRU> & temp)
{
     QByteArray jsonba = strJson.toUtf8();
     QJsonParseError jsonError;
     QJsonDocument doucment = QJsonDocument::fromJson(jsonba, &jsonError);
     if(jsonError.error == QJsonParseError::NoError)
     {
         QVariant data = doucment.toVariant();
         QVariantMap Lights = data.toMap();
         for(auto it = Lights.begin(); it != Lights.end(); ++it)
         {
             QString Id = it.key();
             SENSOR_LIGHT_STRU LightStru;
             QVariantMap LightType = it.value().toMap();
             LightStru.name = LightType.value("类型").toString();
             QVariantList Ids = LightType.value("亮灯种类").toList();
             for(int i = 0; i < Ids.size(); i++)
             {
                 QVariant value = Ids.at(i);
                 QVariantMap map =  value.toMap();
                 QString  lightId = map.value("信号灯").toString();
                 LightStru.light.append(lightId);
             }
             temp.insert(Id,LightStru);
         }
     }
}
CoItem* TXmlReader::AnalyItem(QDomElement mXmlNode)
{
    CoItem *Item = NULL;

    int Type = mXmlNode.attribute(tr("Type")).toInt();

    double StartX = mXmlNode.attribute(tr("StartX")).toDouble();
    double StartY = mXmlNode.attribute(tr("StartY")).toDouble();
    double StopX = mXmlNode.attribute(tr("StopX")).toDouble();
    double StopY = mXmlNode.attribute(tr("StopY")).toDouble();
    int LineWidth = mXmlNode.attribute(tr("LineWidth")).toInt();
    int LineStyle = mXmlNode.attribute(tr("LineStyle")).toInt();

    QColor LineColor(mXmlNode.attribute(tr("LineColor")));
    QColor BackColor(mXmlNode.attribute(tr("BackColor")));

    //////////////////////////////////////////////////////////////////////
    Item = TItemFactory::CreateItem(Type, StartX, StartY, StopX, StopY,
                                  Qt::PenStyle(LineStyle), LineWidth, LineColor, BackColor);

    if(Item == NULL)
        return Item;
    Item->SetId(mXmlNode.attribute(tr("Id")).toInt());
    Item->SetPower(mXmlNode.attribute(tr("Power")).toInt());
    Item->SetZValue(mXmlNode.attribute(tr("Z")).toDouble());
    Item->SetRotateCenter(mXmlNode.attribute(tr("RotateX")).toDouble(),
                           mXmlNode.attribute(tr("RotateY")).toDouble());
    Item->SetRotateAngle(mXmlNode.attribute(tr("Rotate")).toDouble());


    switch (Type)
    {
    case CoItem::Button:
    {
        TButton *Button = dynamic_cast<TButton *>(Item);
        if(Button)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Button->SetText(mXmlBase.attribute(tr("Text")));
            Button->SetPressMode(mXmlBase.attribute(tr("PressMode")).toShort());
            Button->SetFontSize(mXmlBase.attribute(tr("FontSize")).toShort());
            Button->SetFontAlignHor(mXmlBase.attribute(tr("AlignHor")).toShort());
            Button->SetFontAlignVer(mXmlBase.attribute(tr("AlignVer")).toShort());
            Button->SetFontColor(mXmlBase.attribute(tr("FontColor")));
            Button->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor1"))),0);
            Button->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor2"))),1);
            Button->SetPixFile(mXmlBase.attribute(tr("BackPix1")),0);
            Button->SetPixFile(mXmlBase.attribute(tr("BackPix2")),1);
            Button->SetSelectPix(mXmlBase.attribute(tr("SelectPix1")).toShort(),0);
            Button->SetSelectPix(mXmlBase.attribute(tr("SelectPix2")).toShort(),1);

            mXmlBase = mXmlBase.nextSiblingElement("OpAttrs");
            Button->SetJsTactic(mXmlBase.attribute(tr("JsTactic1")),0);
            Button->SetJsTactic(mXmlBase.attribute(tr("JsTactic2")),1);
            QString varName = mXmlBase.attribute(tr("VarName")) + "," +
                    mXmlBase.attribute(tr("VarState")) + ","+mXmlBase.attribute(tr("VarType"));
            Button->SetVarName(varName);
            Button->SetScreen(mXmlBase.attribute(tr("ScreenName")));
            Button->SetOpFunc(mXmlBase.attribute(tr("OpFunc")).toShort());

            mXmlBase = mXmlBase.nextSiblingElement("EnabelAttrs");
            Button->SetEnableExpre(ReTransExpreSymbol(mXmlBase.attribute(tr("EnableExpre"))));
            Button->SetEnableState(mXmlBase.attribute(tr("EnableState")).toInt());

            mXmlBase = mXmlBase.nextSiblingElement("StateAttrs");
            varName = mXmlBase.attribute(tr("StateVarName")) + "," + mXmlBase.attribute(tr("StateVarType"));
            Button->SetStateVarName(varName);
            Button->SetAutoTime(mXmlBase.attribute(tr("AutoTime")).toInt());
            Button->SetStateFontColor(QColor(mXmlBase.attribute(tr("StateFColor1"))),0);
            Button->SetStateFontColor(QColor(mXmlBase.attribute(tr("StateFColor2"))),1);
            Button->SetStateText(mXmlBase.attribute(tr("StateText1")),0);
            Button->SetStateText(mXmlBase.attribute(tr("StateText2")),1);
            Button->SetStateBGColor(QColor(mXmlBase.attribute(tr("StateBGColor1"))),0);
            Button->SetStateBGColor(QColor(mXmlBase.attribute(tr("StateBGColor2"))),1);
            Button->SetStateBGPix(mXmlBase.attribute(tr("StateBGPix1")),0);
            Button->SetStateBGPix(mXmlBase.attribute(tr("StateBGPix2")),1);
            Button->SetStateSelectPix(mXmlBase.attribute(tr("StateSelectPix1")).toShort(),0);
            Button->SetStateSelectPix(mXmlBase.attribute(tr("StateSelectPix2")).toShort(),1);
        }
    }
        break;
    case CoItem::CalBtn:
    {
        TCalButton *Button = dynamic_cast<TCalButton *>(Item);
        if(Button)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Button->SetText(mXmlBase.attribute(tr("Text")));
            Button->SetFontSize(mXmlBase.attribute(tr("FontSize")).toShort());
            Button->SetFontAlignHor(mXmlBase.attribute(tr("AlignHor")).toShort());
            Button->SetFontAlignVer(mXmlBase.attribute(tr("AlignVer")).toShort());
            Button->SetFontColor(mXmlBase.attribute(tr("FontColor")));
            Button->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor1"))),0);
            Button->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor2"))),1);
            Button->SetPixFile(mXmlBase.attribute(tr("BackPix1")),0);
            Button->SetPixFile(mXmlBase.attribute(tr("BackPix2")),1);
            Button->SetSelectPix(mXmlBase.attribute(tr("SelectPix1")).toShort(),0);
            Button->SetSelectPix(mXmlBase.attribute(tr("SelectPix2")).toShort(),1);

            mXmlBase = mXmlBase.nextSiblingElement("OpAttrs");
            Button->SetJsTactic(mXmlBase.attribute(tr("JsTactic1")),0);
            Button->SetJsTactic(mXmlBase.attribute(tr("JsTactic2")),1);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar1")),1);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar2")),2);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar3")),3);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar4")),4);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar5")),5);
            Button->SetCalVar(mXmlBase.attribute(tr("CalVar6")),6);

            mXmlBase = mXmlBase.nextSiblingElement("EnabelAttrs");
            Button->SetEnableExpre(ReTransExpreSymbol(mXmlBase.attribute(tr("EnableExpre"))));
            Button->SetEnableState(mXmlBase.attribute(tr("EnableState")).toInt());
        }
    }
        break;
    case CoItem::TextEdit:
    {
        TTextEdit *Text = dynamic_cast<TTextEdit *>(Item);
        if(Text)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Text->SetFontSize(mXmlBase.attribute(tr("FontSize")).toShort());
            Text->SetFontColor(mXmlBase.attribute(tr("FontColor")));
            Text->SetFontAlignHor(mXmlBase.attribute(tr("AlignHor")).toShort());
            Text->SetFontAlignVer(mXmlBase.attribute(tr("AlignVer")).toShort());

            Text->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor1"))),0);
            Text->SetPixFile(mXmlBase.attribute(tr("BackPix1")),0);
            Text->SetSelectPix(mXmlBase.attribute(tr("SelectPix1")).toShort(),0);

            mXmlBase = mXmlBase.nextSiblingElement("OpAttrs");
            QString varName = mXmlBase.attribute(tr("InVarName")) + "," +
                    mXmlBase.attribute(tr("OutVarName")) + "," +mXmlBase.attribute(tr("VarType"));
            Text->SetVarName(varName);
            Text->SetJsTactic(mXmlBase.attribute(tr("JsTactic")));
            Text->SetAutoTime( mXmlBase.attribute(tr("AutoTime")).toInt());
            Text->SetUnit(mXmlBase.attribute(tr("Unit")));
            Text->SetValueFormat(mXmlBase.attribute(tr("ValFormat")).toShort());
            Text->SetIntAccurecy(mXmlBase.attribute(tr("IntAccy")).toShort());
            Text->SetDecAccurecy(mXmlBase.attribute(tr("DecAccy")).toShort());
            Text->SetMaxValue(mXmlBase.attribute(tr("Max")).toDouble());
            Text->SetMinValue(mXmlBase.attribute(tr("Min")).toDouble());

            mXmlBase = mXmlBase.nextSiblingElement("EnabelAttrs");
            Text->SetEnableExpre(ReTransExpreSymbol(mXmlBase.attribute(tr("EnableExpre"))));
            Text->SetEnableState(mXmlBase.attribute(tr("EnableState")).toInt());
        }
    }
        break;
    case CoItem::Label:
    {
        TLabel *Label = dynamic_cast<TLabel *>(Item);
        if(Label)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Label->SetText(mXmlBase.attribute(tr("Text")));
            Label->SetOutputMode(mXmlBase.attribute(tr("OutPTMode")).toShort());
            Label->SetFontSize(mXmlBase.attribute(tr("FontSize")).toShort());
            Label->SetFontAlignHor(mXmlBase.attribute(tr("AlignHor")).toShort());
            Label->SetFontAlignVer(mXmlBase.attribute(tr("AlignVer")).toShort());
            Label->SetFontColor(mXmlBase.attribute(tr("FontColor")));
            Label->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor1"))),0);
            Label->SetPixFile(mXmlBase.attribute(tr("BackPix1")),0);
            Label->SetSelectPix(mXmlBase.attribute(tr("SelectPix1")).toShort(),0);

            mXmlBase = mXmlBase.nextSiblingElement("ShowAttrs");
            QString varName = mXmlBase.attribute(tr("VarName")) + "," +
                    mXmlBase.attribute(tr("VarType"));
            Label->SetVarName(varName);
            Label->SetUnit(mXmlBase.attribute(tr("Unit")));
            Label->SetAutoTime(mXmlBase.attribute(tr("AutoTime")).toInt());
            Label->SetValueType(mXmlBase.attribute(tr("ValType")).toShort());
            Label->SetValueFormat(mXmlBase.attribute(tr("ValFormat")).toShort());
            Label->SetIntAccurecy(mXmlBase.attribute(tr("IntAccy")).toShort());
            Label->SetDecAccurecy(mXmlBase.attribute(tr("DecAccy")).toShort());
            Label->SetJsTactic(mXmlBase.attribute(tr("JsTactic")));
        }
    }
        break;
    case CoItem::Iolamp:
    {
        TIOLamp *IoLamp = dynamic_cast<TIOLamp *>(Item);
        if(IoLamp)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            QString varName = mXmlBase.attribute(tr("VarName")) + "," +
                    mXmlBase.attribute(tr("VarType"));
            IoLamp->SetVarName(varName);
            IoLamp->SetAutoTime(mXmlBase.attribute(tr("AutoTime")).toInt());
            IoLamp->SetExpre(mXmlBase.attribute(tr("Express")));
        }
    }
        break;
    case CoItem::Warnlamp:
    {
        TWarnLamp *warnLamp = dynamic_cast<TWarnLamp *>(Item);
        if(warnLamp)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");

            QString RedName = mXmlBase.attribute(tr("RedLight"));
            QString YeName = mXmlBase.attribute(tr("YellowLight"));
            QString GreName = mXmlBase.attribute(tr("GreenLight"));
            QString BzName = mXmlBase.attribute(tr("Buzzer"));
            if(!DEF_RUN_AS_DESIGNER)
            {
                QMap<QString,INOUTPUT_STRU> Temp1;
                INOUTPUT_STRU stru;
                stru.stat = 1;
                stru.bitName = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[RedName].IOaddr;
                Temp1.insert("1",stru);
                stru.bitName = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[YeName].IOaddr;
                Temp1.insert("2",stru);
                stru.bitName = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[GreName].IOaddr;
                Temp1.insert("3",stru);
                stru.bitName = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[BzName].IOaddr;
                Temp1.insert("4",stru);
                ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_THREE_SOFTWARE_Map.swap(Temp1);
            }
            warnLamp->SetVarName(RedName,1);
            warnLamp->SetVarName(YeName,2);
            warnLamp->SetVarName(GreName,3);
            warnLamp->SetVarName(BzName,4);
        }
    }
        break;
    case CoItem::GroupBox:
    {
        TGroupBox *Groupbox = dynamic_cast<TGroupBox *>(Item);
        if(Groupbox)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Groupbox->SetTitle(mXmlBase.attribute(tr("Title")));
        }
    }
        break;
    case CoItem::Pixmap:
    {
        TPixmap *Pixmap = dynamic_cast<TPixmap *>(Item);
        if(Pixmap)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Pixmap->SetFileDir(mXmlBase.attribute(tr("PixPath")));
            Pixmap->SetAutoTime(mXmlBase.attribute(tr("AutoTime")).toInt());

            mXmlBase = mXmlBase.nextSiblingElement("StateAttrs");
            QString varName = mXmlBase.attribute(tr("StateVarName")) + "," + mXmlBase.attribute(tr("StateVarType"));
            Pixmap->SetStateVarName(varName);
            Pixmap->SetStateExpre(mXmlBase.attribute(tr("StateExpress")));
            Pixmap->SetStateBGPix(mXmlBase.attribute(tr("StateBGPix1")),0);
            Pixmap->SetStateBGPix(mXmlBase.attribute(tr("StateBGPix2")),1);
        }
    }
        break;
    case CoItem::Chart:
    {
        TCurve *Curve = dynamic_cast<TCurve *>(Item);
        if(Curve)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            Curve->SetTitle(mXmlBase.attribute(tr("Title")));
            Curve->SetXAxisCount(mXmlBase.attribute(tr("XTicker")).toUShort());
            Curve->SetYAxisCount(mXmlBase.attribute(tr("YTicker")).toUShort());
            Curve->SetGridVisible(mXmlBase.attribute(tr("GridVst")).toUShort());
            Curve->SetAxisLblTxt(mXmlBase.attribute(tr("XTitle")),0);
            Curve->SetAxisLblTxt(mXmlBase.attribute(tr("YTitle")),1);
            Curve->SetAxisLblColor(QColor(mXmlBase.attribute(tr("XTitleColor"))),0);
            Curve->SetAxisLblColor(QColor(mXmlBase.attribute(tr("YTitleColor"))),1);
            Curve->SetGridColor(QColor(mXmlBase.attribute(tr("XGridColor"))),0);
            Curve->SetGridColor(QColor(mXmlBase.attribute(tr("XSubGridColor"))),1);
            Curve->SetGridColor(QColor(mXmlBase.attribute(tr("YGridColor"))),2);
            Curve->SetGridColor(QColor(mXmlBase.attribute(tr("YSubGridColor"))),3);
            Curve->SetBackColor(QColor(mXmlBase.attribute(tr("BackColor1"))),0);
            Curve->SetCurveBGColor(QColor(mXmlBase.attribute(tr("BackColor1"))));


            mXmlBase =  mXmlNode.firstChildElement(tr("CrvAttrs"));
            Curve->SetTimeFormat(mXmlBase.attribute(tr("TimeFormat")));
            Curve->SetTimeUnit(mXmlBase.attribute(tr("TimeUnit")));
            Curve->SetXAxisLength(mXmlBase.attribute(tr("XLen")).toInt());
            Curve->SetDecAccurecy(mXmlBase.attribute(tr("DecAccy")).toInt());
            Curve->SetMax(mXmlBase.attribute(tr("Max")).toDouble());
            Curve->SetMin(mXmlBase.attribute(tr("Min")).toDouble());
            Curve->SetAutoTime(mXmlBase.attribute(tr("Freq")).toInt());
            QString CtrlVar = mXmlBase.attribute(tr("CtrlVarName")) + "," +
                    mXmlBase.attribute(tr("CtrlVarType"));
            Curve->SetCtrlVarName(CtrlVar);
            QString varName = mXmlBase.attribute(tr("VarName")) + "," +
                    mXmlBase.attribute(tr("VarType"));
            Curve->SetVarName(varName);
            Curve->SetCurveColor(QColor(mXmlBase.attribute(tr("CrvColor"))));
        }
    }
        break;
    case CoItem::SafeState:
    {
        TSafeState *state = dynamic_cast<TSafeState *>(Item);
        if(state)
        {
            QDomElement mXmlBase = mXmlNode.firstChildElement("BaseAttrs");
            uchar ustate = mXmlBase.attribute(tr("StateType")).toUShort();
            state->SetSafeState(ustate);
            QString flowName = mXmlBase.attribute(tr("FlowName"));
            state->SetFlowName(flowName);
            state->SetSafeString(mXmlBase.attribute(tr("SafeInfo")));
            if(!DEF_RUN_AS_DESIGNER)
            {
                QMap<QString,SWCONFIGXML_SAFESTATE_STRU> temp1;
                AnalySafeState(mXmlBase.attribute(tr("SafeInfo")),temp1);
                if(ustate == 0)
                {
                    if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map.contains("总状态"))
                        ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map.insert("总状态",temp1);
                }
                else
                {
                    if(!ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map.contains(flowName))
                    {
                        ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map.insert(flowName,temp1);
                    }
                }
            }
        }
    }
        break;
    default:
        break;
    }

    return Item;
}

QDomElement TXmlReader::FindElement(QString Name)
{
    QDomElement XmlFind;
    XmlFind.clear();

    if(!IsOpen())
    {
        return XmlFind;
    }

    QDomElement mXmlScada = GetRoot();
    if(mXmlScada.isNull() || mXmlScada.tagName() != tr("Data"))
    {
        return XmlFind;
    }

    XmlFind = mXmlScada.firstChild().toElement();

    while((!XmlFind.isNull()) && (XmlFind.tagName() != Name))
    {
        XmlFind = XmlFind.nextSibling().toElement();
    }

    return XmlFind;
}

QString TXmlReader::ReTransExpreSymbol(const QString &Name)
{
    QString strExpre = Name;
//    strExpre = strExpre.replace("$","&");
    strExpre = strExpre.replace("≥",">");
    strExpre = strExpre.replace("≤","<");
    return strExpre;
}

CoScene *TXmlReader::AnalyFlow(QDomElement mXmlScene, bool isGridLine, QString flowName)
{
    int Id = 0;
    QString Name;
    Id = mXmlScene.attribute(tr("Id")).toInt();
    Name = mXmlScene.attribute(tr("Name"));
    QString BackColor = mXmlScene.attribute(tr("BackColor"));
    ushort iWidth = mXmlScene.attribute(tr("Width")).toUShort();
    ushort iHeight = mXmlScene.attribute(tr("Height")).toUShort();
    CoScene *mScene = 0;
    mScene = new CoScene(0, 0, iWidth, iHeight);
    mScene->SetId(Id);
    mScene->SetName(Name);
    mScene->SetBackColor(QColor(BackColor));
    mScene->ResizeEvent(isGridLine);
    QMap<QString,QMap<int, TFlowBase *> > tempFlowItems;
    if(flowName.isEmpty())
    {
        tempFlowItems = m_flowItems[Name];
    }
    else
    {
        tempFlowItems = m_flowItems[flowName];
    }
    // item
    if(tempFlowItems.contains(Name))
    {
        for(auto it = tempFlowItems[Name].begin();it != tempFlowItems[Name].end();++it)
        {
            mScene->addItem(it.value());
        }
        QDomElement mXmlNodes = mXmlScene.firstChildElement("Items");
        if(!mXmlNodes.isNull())
        {
            QDomElement mXmlNode = mXmlNodes.firstChildElement("item");
            while(!mXmlNode.isNull())
            {
                int EndItemId = mXmlNode.attribute(tr("EndItem")).toInt();
                int StartItemId = mXmlNode.attribute(tr("StartItem")).toInt();
                bool isYN = mXmlNode.attribute(tr("YESNO")).toInt();
                CoFlowChart *StartItem = qgraphicsitem_cast<CoFlowChart*>(tempFlowItems[Name][StartItemId]);
                CoFlowChart *EndItem = qgraphicsitem_cast<CoFlowChart*>(tempFlowItems[Name][EndItemId]);
                StartItem->m_ArrowPost = mXmlNode.attribute(tr("StartPost")).toInt();
                EndItem->m_ArrowPost = mXmlNode.attribute(tr("EndPost")).toInt();
                CoArrow *Item = mScene->CreateArrow(StartItem,EndItem);
                if(Item)
                    Item->SetYNPolicy(isYN);

                mXmlNode = mXmlNode.nextSiblingElement("item");
            }
        }
    }
    return mScene;
}

CoScene* TXmlReader::AnalyScene(QMap<QString,CoScene*> &SceneMap, QMap<QString, FLOW_STRU> &FlowMap,bool &isGridLine)
{
    QDomElement mXmlScenes = FindElement(tr("Scenes"));

    if(mXmlScenes.isNull())
    {
        return NULL;
    }
    int mHostId = 0;
    CoScene *mSceneHost = 0;
    QString proName = mXmlScenes.attribute(tr("Name"));
    mHostId = mXmlScenes.attribute(tr("Host")).toInt();
    isGridLine = IntToBool(mXmlScenes.attribute(tr("GridLine")).toInt());
    ushort Width = mXmlScenes.attribute(tr("Width")).toUShort();
    ushort Height = mXmlScenes.attribute(tr("Height")).toUShort();
    // Canvas scene
    QDomElement mXmlCanvas = mXmlScenes.firstChildElement("Canvas");
    if(!mXmlCanvas.isNull())
    {
        QDomElement mXmlScene = mXmlCanvas.firstChild().toElement();
        while(!mXmlScene.isNull())
        {
            int Id = 0;
            QString Name;

            Id = mXmlScene.attribute(tr("Id")).toInt();
            Name = mXmlScene.attribute(tr("Name"));
            QString BackColor = mXmlScene.attribute(tr("BackColor"));

            CoScene *mScene = 0;
            mScene = new CoScene(0, 0, Width, Height);
            mScene->SetProjectName(proName);
            mScene->SetId(Id);
            mScene->SetName(Name);
            mScene->SetBackColor(QColor(BackColor));
            mScene->ResizeEvent(isGridLine);
            if(Id == mHostId)
            {
                // 保证主视图是第一个
                mSceneHost = mScene;
                SceneMap.insert(Name,mScene);
            }
            else
            {
                SceneMap.insert(Name,mScene);
            }

            // item
            QDomElement mXmlNode = mXmlScene.firstChildElement("item");
            // Item的Map用于对Item按照id由小到大排序，map会自动进行排序
            QMap<int ,CoItem*>ItemMap;
            ItemMap.clear();
            while(!mXmlNode.isNull())
            {
                CoItem *Item = AnalyItem(mXmlNode);

                if(Item)
                {
                    Item->SetProName(proName);
//                    mScene->addItem(Item);
                    ItemMap[Item->GetId()] = Item;
                }
                mXmlNode = mXmlNode.nextSiblingElement("item");
            }
            //item由id的小到大加入scene中
            foreach (CoItem* item, ItemMap) {
                mScene->addItem(item);
            }

            // widget
            // widget的Map用于对widget按照id由小到大排序，map会自动进行排序
            QMap<int ,CoProxyWidget*>widgetMap;
            widgetMap.clear();
            mXmlNode = mXmlScene.firstChildElement("widget");
            while(!mXmlNode.isNull())
            {
                CoProxyWidget* widget = AnalyWidget(mXmlNode,mScene);
                if(widget)
                {
                    widget->SetProName(proName);
//                    mScene->addItem(widget);
                    widgetMap[widget->GetId()] = widget;
                }
                mXmlNode = mXmlNode.nextSiblingElement("widget");
            }
            //widget由id的小到大加入scene中
            foreach (CoProxyWidget* widget, widgetMap) {
                mScene->addItem(widget);
            }

            mXmlScene = mXmlScene.nextSibling().toElement();
        }
    }
    QDomElement mXmlSafe = mXmlScenes.firstChildElement("SafeConfig");
    if(!mXmlSafe.isNull())
    {
        int Id = 0;
        QString Name;

        Id = mXmlSafe.attribute(tr("Id")).toInt();
        Name = mXmlSafe.attribute(tr("Name"));
        QString BackColor = mXmlSafe.attribute(tr("BackColor"));
        ushort safeWidth = mXmlSafe.attribute(tr("Width")).toUShort();
        ushort safeHeight = mXmlSafe.attribute(tr("Height")).toUShort();
        CoScene *Safe = 0;
        Safe = new CoScene(0, 0, safeWidth, safeHeight);
        Safe->SetProjectName(proName);
        Safe->SetId(Id);
        Safe->SetName(Name);
        Safe->SetBackColor(QColor(BackColor));
        Safe->ResizeEvent(isGridLine);
        // item
        QDomElement mXmlNode = mXmlSafe.firstChildElement("item");
        while(!mXmlNode.isNull())
        {
            CoItem *Item = AnalyItem(mXmlNode);
            if(Item)
            {
                Item->SetProName(proName);
                Safe->addItem(Item);
            }
            mXmlNode = mXmlNode.nextSiblingElement("item");
        }
        mXmlNode = mXmlSafe.firstChildElement("widget");
        while(!mXmlNode.isNull())
        {
            CoProxyWidget* widget = AnalyWidget(mXmlNode,Safe);
            if(widget)
            {
                widget->SetProName(proName);
                Safe->addItem(widget);
            }
            mXmlNode = mXmlNode.nextSiblingElement("widget");
        }
        if(DEF_RUN_AS_DESIGNER)
        {
            SceneMap.insert(Name,Safe);
        }
    }

    //FlowChart
    if(DEF_RUN_AS_DESIGNER)
    {
        QDomElement mXmlFlow = mXmlScenes.firstChildElement("FlowChart");
        if(!mXmlFlow.isNull())
        {
            QDomElement mXmlScene = mXmlFlow.firstChild().toElement();
            while(!mXmlScene.isNull())
            {
                FLOW_STRU flowStru;
                CoScene* flow = AnalyFlow(mXmlScene,isGridLine);
                flowStru.SceneMap.insert(flow->GetName(),flow);
                QDomElement mXmlThreads = mXmlScene.firstChildElement("Threads");
                if(!mXmlThreads.isNull())
                {
                    QDomElement mXmlthread = mXmlThreads.firstChild().toElement();
                    while(!mXmlthread.isNull())
                    {
                        CoScene *sc = AnalyFlow(mXmlthread,isGridLine,flow->GetName());
                        flowStru.SceneMap.insert(sc->GetName(),sc);

                        mXmlthread = mXmlthread.nextSibling().toElement();
                    }
                }
                QDomElement mXmlChlidReset = mXmlScene.firstChildElement("childReset");
                if(!mXmlChlidReset.isNull())
                {
                    CoScene* childReset = AnalyFlow(mXmlChlidReset,isGridLine,flow->GetName());
                    flowStru.SceneMap.insert(childReset->GetName(),childReset);
                }

                FlowMap.insert(flow->GetName(),flowStru);
                mXmlScene = mXmlScene.nextSibling().toElement();
            }
        }
    }
    return mSceneHost;
}

int TXmlReader::AnalyViewRotate()
{
    QDomElement mXmlScenes = FindElement(tr("Scenes"));

    if(mXmlScenes.isNull())
    {
        return 0;
    }
    return mXmlScenes.attribute(tr("Rotate")).toInt();
}

void TXmlReader::AnalyFlowChart(const QString &fileName)
{
    ItemSave *saveflowscene = new ItemSave;
    m_flowItems = saveflowscene->readItem_v1(fileName);
    return ;
}
