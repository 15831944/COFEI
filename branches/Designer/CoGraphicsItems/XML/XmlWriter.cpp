#include "XmlWriter.h"

#include <LibBase.h>
#include <QFile>
#include <QTextStream>
#include <CoGraphics/Arrow.h>
#include "../TBasicControls/BasicControlsPub.h"
#include "../CoProxyItem/nightsetwidget.h"

TXmlWriter::TXmlWriter()
{
}

TXmlWriter::~TXmlWriter()
{
}

void TXmlWriter::AddHead()
{
    QDomProcessingInstruction instruction;
    instruction = m_Doc.createProcessingInstruction(tr("xml"), tr("version=\"1.0\" encoding=\"UTF-8\""));
    m_Doc.appendChild(instruction);
}

QDomElement TXmlWriter::AddRoot(const QString Root)
{
    QDomElement XmlRoot = m_Doc.createElement(Root);
    m_Doc.appendChild(XmlRoot);

    return XmlRoot;
}

QDomElement TXmlWriter::AddChild(QDomElement &Parent, const QString Name)
{
    QDomElement XmlNode = m_Doc.createElement(Name);
    Parent.appendChild(XmlNode);

    return XmlNode;
}

bool TXmlWriter::InsertText(QDomElement &XmlItem, CoItem *Item)
{
    TTextEdit *Text = dynamic_cast<TTextEdit *>(Item);

    if(NULL == Text || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("FontSize"), Text->GetFontSize());
    XmlBaseAttr.setAttribute(tr("FontColor"), Text->GetFontColor().name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("AlignHor"), Text->GetFontAlignHor());
    XmlBaseAttr.setAttribute(tr("AlignVer"), Text->GetFontAlignVer());
    XmlBaseAttr.setAttribute(tr("BackColor1"), Text->GetBackColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackPix1"), Text->GetPixFiles()[0]);
    XmlBaseAttr.setAttribute(tr("SelectPix1"), Text->GetSelectPix()[0]);

    QDomElement XmlOPAttr = AddChild(XmlItem, tr("OpAttrs"));
    XmlOPAttr.setAttribute(tr("JsTactic"), Text->GetJsTactic());
    XmlOPAttr.setAttribute(tr("InVarName"), Text->GetInputVarName());
    XmlOPAttr.setAttribute(tr("OutVarName"), Text->GetOutputVarName());
    XmlOPAttr.setAttribute(tr("VarType"), Text->GetVarType());
    XmlOPAttr.setAttribute(tr("AutoTime"), Text->GetAutoTime());
    XmlOPAttr.setAttribute(tr("Unit"), Text->GetUnit());
    XmlOPAttr.setAttribute(tr("ValFormat"), Text->GetValueFormat());
    XmlOPAttr.setAttribute(tr("IntAccy"), Text->GetIntAccurecy());
    XmlOPAttr.setAttribute(tr("DecAccy"), Text->GetDecAccurecy());
    XmlOPAttr.setAttribute(tr("Max"), Text->GetMaxValue());
    XmlOPAttr.setAttribute(tr("Min"), Text->GetMinValue());

    QDomElement XmlEnabelAttr = AddChild(XmlItem, tr("EnabelAttrs"));
    XmlEnabelAttr.setAttribute(tr("EnableExpre"), TransExpreSymbol(Text->GetEnableExpre()));
    XmlEnabelAttr.setAttribute(tr("EnableState"), Text->GetEnableState());

    return true;
}

bool TXmlWriter::InsertLabel(QDomElement &XmlItem, CoItem *Item)
{
    TLabel *Label = dynamic_cast<TLabel *>(Item);

    if(NULL == Label || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("OutPTMode"), Label->GetOutputMode());
    XmlBaseAttr.setAttribute(tr("Text"), Label->GetText());
    XmlBaseAttr.setAttribute(tr("FontSize"), Label->GetFontSize());
    XmlBaseAttr.setAttribute(tr("FontColor"), Label->GetFontColor().name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("AlignHor"), Label->GetFontAlignHor());
    XmlBaseAttr.setAttribute(tr("AlignVer"), Label->GetFontAlignVer());
    XmlBaseAttr.setAttribute(tr("BackColor1"), Label->GetBackColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackPix1"), Label->GetPixFiles()[0]);
    XmlBaseAttr.setAttribute(tr("SelectPix1"), Label->GetSelectPix()[0]);
    QDomElement XmlShowAttr = AddChild(XmlItem, tr("ShowAttrs"));
    XmlShowAttr.setAttribute(tr("VarName"), Label->GetVarName());
    XmlShowAttr.setAttribute(tr("VarType"), Label->GetVarType());
    XmlShowAttr.setAttribute(tr("Unit"), Label->GetUnit());
    XmlShowAttr.setAttribute(tr("AutoTime"), Label->GetAutoTime());
    XmlShowAttr.setAttribute(tr("ValType"), Label->GetValueType());
    XmlShowAttr.setAttribute(tr("ValFormat"), Label->GetValueFormat());
    XmlShowAttr.setAttribute(tr("IntAccy"), Label->GetIntAccurecy());
    XmlShowAttr.setAttribute(tr("DecAccy"), Label->GetDecAccurecy());
    XmlShowAttr.setAttribute(tr("JsTactic"), Label->GetJsTactic());
    return true;
}

bool TXmlWriter::InsertInstLamp(QDomElement &XmlItem, CoItem *Item)
{
    TIOLamp *IOLamp = dynamic_cast<TIOLamp *>(Item);

    if(NULL == IOLamp || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("VarName"), IOLamp->GetVarName());
    XmlBaseAttr.setAttribute(tr("VarType"), IOLamp->GetVarType());
    XmlBaseAttr.setAttribute(tr("AutoTime"), IOLamp->GetAutoTime());
    XmlBaseAttr.setAttribute(tr("Express"), IOLamp->GetExpre());

    return true;
}

bool TXmlWriter::InsertWarnLamp(QDomElement &XmlItem, CoItem *Item)
{
    TWarnLamp *warnLamp = dynamic_cast<TWarnLamp *>(Item);

    if(NULL == warnLamp || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("RedLight"), warnLamp->GetVarName()[0]);
    XmlBaseAttr.setAttribute(tr("YellowLight"), warnLamp->GetVarName()[1]);
    XmlBaseAttr.setAttribute(tr("GreenLight"), warnLamp->GetVarName()[2]);
    XmlBaseAttr.setAttribute(tr("Buzzer"), warnLamp->GetVarName()[3]);
    return true;
}

bool TXmlWriter::InsertButton(QDomElement &XmlItem, CoItem *Item)
{
    TButton *Button = dynamic_cast<TButton *>(Item);

    if(NULL == Button || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("PressMode"), Button->GetPressMode());
    XmlBaseAttr.setAttribute(tr("Text"), Button->GetText());
    XmlBaseAttr.setAttribute(tr("FontSize"), Button->GetFontSize());
    XmlBaseAttr.setAttribute(tr("FontColor"), Button->GetFontColor().name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("AlignHor"), Button->GetFontAlignHor());
    XmlBaseAttr.setAttribute(tr("AlignVer"), Button->GetFontAlignVer());
    XmlBaseAttr.setAttribute(tr("BackColor1"), Button->GetBackColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackColor2"), Button->GetBackColor()[1].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackPix1"), Button->GetPixFiles()[0]);
    XmlBaseAttr.setAttribute(tr("BackPix2"), Button->GetPixFiles()[1]);
    XmlBaseAttr.setAttribute(tr("SelectPix1"), Button->GetSelectPix()[0]);
    XmlBaseAttr.setAttribute(tr("SelectPix2"), Button->GetSelectPix()[1]);

    QDomElement XmlOPAttr = AddChild(XmlItem, tr("OpAttrs"));
    XmlOPAttr.setAttribute(tr("JsTactic1"), Button->GetJsTactic()[0]);
    XmlOPAttr.setAttribute(tr("JsTactic2"), Button->GetJsTactic()[1]);
    XmlOPAttr.setAttribute(tr("VarName"), Button->GetVarName());
    XmlOPAttr.setAttribute(tr("VarState"), Button->GetVarState());
    XmlOPAttr.setAttribute(tr("VarType"), Button->GetVarType());
    XmlOPAttr.setAttribute(tr("ScreenName"), Button->GetScreen());
    XmlOPAttr.setAttribute(tr("OpFunc"), Button->GetOpFunc());

    QDomElement XmlEnabelAttr = AddChild(XmlItem, tr("EnabelAttrs"));
    XmlEnabelAttr.setAttribute(tr("EnableExpre"), TransExpreSymbol(Button->GetEnableExpre()));
    XmlEnabelAttr.setAttribute(tr("EnableState"), Button->GetEnableState());

    QDomElement XmlStateAttr = AddChild(XmlItem, tr("StateAttrs"));
    XmlStateAttr.setAttribute(tr("StateVarName"), Button->GetStateVarName());
    XmlStateAttr.setAttribute(tr("StateVarType"), Button->GetStateVarType());
    XmlStateAttr.setAttribute(tr("AutoTime"), Button->GetAutoTime());
    XmlStateAttr.setAttribute(tr("StateBGColor1"), Button->GetStateBGColor()[0].name(QColor::HexArgb));
    XmlStateAttr.setAttribute(tr("StateBGColor2"), Button->GetStateBGColor()[1].name(QColor::HexArgb));
    XmlStateAttr.setAttribute(tr("StateBGPix1"), Button->GetStateBGPix()[0]);
    XmlStateAttr.setAttribute(tr("StateBGPix2"), Button->GetStateBGPix()[1]);
    XmlStateAttr.setAttribute(tr("StateSelectPix1"), Button->GetStateSelectPix()[0]);
    XmlStateAttr.setAttribute(tr("StateSelectPix2"), Button->GetStateSelectPix()[1]);
    XmlStateAttr.setAttribute(tr("StateText1"), Button->GetStateText()[0]);
    XmlStateAttr.setAttribute(tr("StateText2"), Button->GetStateText()[1]);
    XmlStateAttr.setAttribute(tr("StateFColor1"), Button->GetStateFontColor()[0].name(QColor::HexArgb));
    XmlStateAttr.setAttribute(tr("StateFColor2"), Button->GetStateFontColor()[1].name(QColor::HexArgb));
    return true;
}

bool TXmlWriter::InsertCalButton(QDomElement &XmlItem, CoItem *Item)
{
    TCalButton *Button = dynamic_cast<TCalButton *>(Item);

    if(NULL == Button || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("Text"), Button->GetText());
    XmlBaseAttr.setAttribute(tr("FontSize"), Button->GetFontSize());
    XmlBaseAttr.setAttribute(tr("FontColor"), Button->GetFontColor().name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("AlignHor"), Button->GetFontAlignHor());
    XmlBaseAttr.setAttribute(tr("AlignVer"), Button->GetFontAlignVer());
    XmlBaseAttr.setAttribute(tr("BackColor1"), Button->GetBackColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackColor2"), Button->GetBackColor()[1].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackPix1"), Button->GetPixFiles()[0]);
    XmlBaseAttr.setAttribute(tr("BackPix2"), Button->GetPixFiles()[1]);
    XmlBaseAttr.setAttribute(tr("SelectPix1"), Button->GetSelectPix()[0]);
    XmlBaseAttr.setAttribute(tr("SelectPix2"), Button->GetSelectPix()[1]);

    QDomElement XmlOPAttr = AddChild(XmlItem, tr("OpAttrs"));
    XmlOPAttr.setAttribute(tr("JsTactic1"), Button->GetJsTactic()[0]);
    XmlOPAttr.setAttribute(tr("JsTactic2"), Button->GetJsTactic()[1]);
    XmlOPAttr.setAttribute(tr("CalVar1"), Button->GetCalVarComb1());
    XmlOPAttr.setAttribute(tr("CalVar2"), Button->GetCalVarComb2());
    XmlOPAttr.setAttribute(tr("CalVar3"), Button->GetCalVarComb3());
    XmlOPAttr.setAttribute(tr("CalVar4"), Button->GetCalVarComb4());
    XmlOPAttr.setAttribute(tr("CalVar5"), Button->GetCalVarComb5());
    XmlOPAttr.setAttribute(tr("CalVar6"), Button->GetCalVarComb6());

    QDomElement XmlEnabelAttr = AddChild(XmlItem, tr("EnabelAttrs"));
    XmlEnabelAttr.setAttribute(tr("EnableExpre"), TransExpreSymbol(Button->GetEnableExpre()));
    XmlEnabelAttr.setAttribute(tr("EnableState"), Button->GetEnableState());
    return true;
}

bool TXmlWriter::InsertPixmap(QDomElement &XmlItem, CoItem *Item)
{
    TPixmap *pixmap = dynamic_cast<TPixmap *>(Item);

    if(NULL == pixmap || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("PixPath"), pixmap->GetFileDir());
    XmlBaseAttr.setAttribute(tr("AutoTime"), pixmap->GetAutoTime());

    QDomElement XmlStateAttr = AddChild(XmlItem, tr("StateAttrs"));
    XmlStateAttr.setAttribute(tr("StateVarName"), pixmap->GetStateVarName());
    XmlStateAttr.setAttribute(tr("StateVarType"), pixmap->GetStateVarType());
    XmlStateAttr.setAttribute(tr("StateExpress"), pixmap->GetStateExpre());
    XmlStateAttr.setAttribute(tr("StateBGPix1"), pixmap->GetStateBGPix()[0]);
    XmlStateAttr.setAttribute(tr("StateBGPix2"), pixmap->GetStateBGPix()[1]);

    return true;
}

bool TXmlWriter::InsertGroupbox(QDomElement &XmlItem, CoItem *Item)
{
    TGroupBox *Groupbox = dynamic_cast<TGroupBox *>(Item);

    if(NULL == Groupbox || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("Title"), Groupbox->GetTitle());

    return true;
}

bool TXmlWriter::InsertCurve(QDomElement &XmlItem, CoItem *Item)
{
    TCurve *Curve = dynamic_cast<TCurve *>(Item);

    if(NULL == Curve || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("Title"), Curve->GetTitle());
    XmlBaseAttr.setAttribute(tr("XTicker"), Curve->GetXAxisCount());
    XmlBaseAttr.setAttribute(tr("YTicker"), Curve->GetYAxisCount());
    XmlBaseAttr.setAttribute(tr("GridVst"), Curve->GetGridVisible());
    XmlBaseAttr.setAttribute(tr("XTitle"), Curve->GetAxisLblTxt()[0]);
    XmlBaseAttr.setAttribute(tr("YTitle"), Curve->GetAxisLblTxt()[1]);
    XmlBaseAttr.setAttribute(tr("XTitleColor"), Curve->GetAxisLblColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("YTitleColor"), Curve->GetAxisLblColor()[1].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("XGridColor"), Curve->GetGridColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("XSubGridColor"), Curve->GetGridColor()[1].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("YGridColor"), Curve->GetGridColor()[2].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("YSubGridColor"), Curve->GetGridColor()[3].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackColor1"), Curve->GetBackColor()[0].name(QColor::HexArgb));

    QDomElement XmlCvAttr = AddChild(XmlItem, tr("CrvAttrs"));
    XmlCvAttr.setAttribute(tr("TimeFormat"), Curve->GetTimeFormat());
    XmlCvAttr.setAttribute(tr("TimeUnit"), Curve->GetTimeUnit());
    XmlCvAttr.setAttribute(tr("XLen"), Curve->GetXAxisLength());
    XmlCvAttr.setAttribute(tr("DecAccy"), Curve->GetDecAccurecy());
    XmlCvAttr.setAttribute(tr("Max"), Curve->GetMax());
    XmlCvAttr.setAttribute(tr("Min"), Curve->GetMin());
    XmlCvAttr.setAttribute(tr("Freq"), Curve->GetAutoTime());
    XmlCvAttr.setAttribute(tr("CtrlVarName"), Curve->GetCtrlVarName());
    XmlCvAttr.setAttribute(tr("CtrlVarType"), Curve->GetCtrlVarType());
    XmlCvAttr.setAttribute(tr("VarName"), Curve->GetVarName());
    XmlCvAttr.setAttribute(tr("VarType"), Curve->GetVarType());
    XmlCvAttr.setAttribute(tr("CrvColor"), Curve->GetCurveColor().name(QColor::HexArgb));

    return true;
}

bool TXmlWriter::InsertDebug(QDomElement &XmlItem, CoProxyWidget *)
{
    if(XmlItem.isNull())
    {
        return false;
    }
    return true;
}

bool TXmlWriter::InsertSafeLevel(QDomElement &XmlItem, CoProxyWidget *Item)
{
    NightSetWidget *widget = dynamic_cast<NightSetWidget *>(Item);
    if(widget == NULL || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("JsonValue"),CreateJson(widget->GetTabWidgetValue()));
    return true;
}

bool TXmlWriter::AddArrow(QDomElement &DomParent, CoArrow *Arrow)
{
    if(NULL == Arrow || DomParent.isNull())
    {
        return false;
    }
    InsertArrow(DomParent, Arrow);
    return true;
}

bool TXmlWriter::AddWidget(QDomElement &DomParent, CoProxyWidget *pWidget)
{
    if(NULL == pWidget || DomParent.isNull())
    {
        return false;
    }
    QDomElement XmlItem;
    QColor mColor;

    XmlItem = AddChild(DomParent, tr("widget"));

    XmlItem.setAttribute(tr("Id"), pWidget->GetId());
    XmlItem.setAttribute(tr("Type"), pWidget->GetType());
    XmlItem.setAttribute(tr("Power"), pWidget->GetPower());

    XmlItem.setAttribute(tr("StartX"), pWidget->GetStartX());
    XmlItem.setAttribute(tr("StartY"), pWidget->GetStartY());
    XmlItem.setAttribute(tr("StopX"), pWidget->GetStopX());
    XmlItem.setAttribute(tr("StopY"), pWidget->GetStopY());

    XmlItem.setAttribute(tr("LineWidth"), pWidget->GetLineWidth());
    XmlItem.setAttribute(tr("LineStyle"), (int)(pWidget->GetLineStyle()));

    XmlItem.setAttribute(tr("Z"), pWidget->GetZValue());
    XmlItem.setAttribute(tr("Rotate"), pWidget->GetRotateAngle());
    XmlItem.setAttribute(tr("RotateX"), pWidget->GetRotateCenterX());
    XmlItem.setAttribute(tr("RotateY"), pWidget->GetRotateCenterY());

    mColor = pWidget->GetLineColor();
    XmlItem.setAttribute(tr("LineColor"), mColor.name(QColor::HexArgb));

    mColor = pWidget->GetBackGroundColor();
    XmlItem.setAttribute(tr("BackColor"), mColor.name(QColor::HexArgb));

    InsertDebug(XmlItem,pWidget);

    InsertSafeLevel(XmlItem,pWidget);
    return true;
}

bool TXmlWriter::AddItems(QDomElement &DomParent, CoScene *Scene)
{
    if(NULL == Scene || DomParent.isNull())
    {
        return false;
    }

    DomParent.setAttribute(tr("Id"), Scene->GetId());
    DomParent.setAttribute(tr("Name"), Scene->GetName());
    DomParent.setAttribute(tr("BackColor"), Scene->GetBackColor().name(QColor::HexArgb));
    DomParent.setAttribute(tr("Width"), Scene->GetWidth());
    DomParent.setAttribute(tr("Height"), Scene->GetHeight());
    QList<CoItem *>ItemList;
    Scene->GetItemsList(ItemList);
    foreach(CoItem *Item, ItemList)
    {
        AddItem(DomParent, Item);
    }
    QList<CoProxyWidget *>WidgetList;
    Scene->GetItemsList(WidgetList);
    foreach(CoProxyWidget *widget, WidgetList)
    {
        AddWidget(DomParent, widget);
    }
    return true;
}
bool TXmlWriter::InsertArrow(QDomElement &DomParent, CoArrow *Arrow)
{
    if(NULL == Arrow || DomParent.isNull())
    {
        return false;
    }

    QDomElement XmlItem = AddChild(DomParent, tr("item"));
    // 插入流程图属性
    XmlItem.setAttribute(tr("StartItem"), Arrow->GetStartItem()->GetId());
    XmlItem.setAttribute(tr("StartPost"), Arrow->GetStartPost());
    XmlItem.setAttribute(tr("EndItem"), Arrow->GetEndItem()->GetId());
    XmlItem.setAttribute(tr("EndPost"), Arrow->GetEndPost());
    XmlItem.setAttribute(tr("YESNO"), Arrow->GetYNPolicy());
    return true;
}

bool TXmlWriter::InsertGPInput(QDomElement &XmlItem, CoItem *Item)
{    
    TGPInPut *In = dynamic_cast<TGPInPut *>(Item);

    if(NULL == In || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("IOName"), In->GetIOName());
    XmlBaseAttr.setAttribute(tr("AutoTime"), In->GetAutoTime());

    return true;
}

bool TXmlWriter::InsertGPOutPut(QDomElement &XmlItem, CoItem *Item)
{
    TGPOutPut *Out = dynamic_cast<TGPOutPut *>(Item);

    if(NULL == Out || XmlItem.isNull())
    {
        return false;
    }
    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("Text"), Out->GetText());
    XmlBaseAttr.setAttribute(tr("FontSize"), Out->GetFontSize());
    XmlBaseAttr.setAttribute(tr("FontColor"), Out->GetFontColor().name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("AlignHor"), Out->GetFontAlignHor());
    XmlBaseAttr.setAttribute(tr("AlignVer"), Out->GetFontAlignVer());
    XmlBaseAttr.setAttribute(tr("BackColor1"), Out->GetBackColor()[0].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackColor2"), Out->GetBackColor()[1].name(QColor::HexArgb));
    XmlBaseAttr.setAttribute(tr("BackPix1"), Out->GetPixFiles()[0]);
    XmlBaseAttr.setAttribute(tr("BackPix2"), Out->GetPixFiles()[1]);
    XmlBaseAttr.setAttribute(tr("SelectPix1"), Out->GetSelectPix()[0]);
    XmlBaseAttr.setAttribute(tr("SelectPix2"), Out->GetSelectPix()[1]);

    QDomElement XmlOPAttr = AddChild(XmlItem, tr("OpAttrs"));
    XmlOPAttr.setAttribute(tr("JsTactic1"), Out->GetJsTactic()[0]);
    XmlOPAttr.setAttribute(tr("JsTactic2"), Out->GetJsTactic()[1]);
    XmlOPAttr.setAttribute(tr("IOName"), Out->GetIOName());
    XmlOPAttr.setAttribute(tr("IOState"), Out->GetIOState());

    QDomElement XmlEnabelAttr = AddChild(XmlItem, tr("EnabelAttrs"));
    XmlEnabelAttr.setAttribute(tr("EnableExpre"), TransExpreSymbol(Out->GetEnableExpre()));
    XmlEnabelAttr.setAttribute(tr("EnableState"), Out->GetEnableState());
    return true;
}

bool TXmlWriter::InsertSafeState(QDomElement &XmlItem, CoItem *Item)
{
    TSafeState *state = dynamic_cast<TSafeState *>(Item);
    if(NULL == state || XmlItem.isNull())
    {
        return false;
    }

    QDomElement XmlBaseAttr = AddChild(XmlItem, tr("BaseAttrs"));
    XmlBaseAttr.setAttribute(tr("StateType"), state->GetSafeState());
    XmlBaseAttr.setAttribute(tr("FlowName"), state->GetFlowName());
    XmlBaseAttr.setAttribute(tr("SafeInfo"), state->GetSafeString());
    return true;
}

bool TXmlWriter::AddItem(QDomElement &DomParent, CoItem *Item)
{
    if(NULL == Item || DomParent.isNull())
    {
        return false;
    }

    QDomElement XmlItem;
    QColor mColor;

    XmlItem = AddChild(DomParent, tr("item"));

    XmlItem.setAttribute(tr("Id"), Item->GetId());
    XmlItem.setAttribute(tr("Type"), Item->GetType());
    XmlItem.setAttribute(tr("Power"), Item->GetPower());

    XmlItem.setAttribute(tr("StartX"), Item->GetStartX());
    XmlItem.setAttribute(tr("StartY"), Item->GetStartY());
    XmlItem.setAttribute(tr("StopX"), Item->GetStopX());
    XmlItem.setAttribute(tr("StopY"), Item->GetStopY());


    XmlItem.setAttribute(tr("LineWidth"), Item->GetLineWidth());
    XmlItem.setAttribute(tr("LineStyle"), (int)(Item->GetLineStyle()));
    XmlItem.setAttribute(tr("Z"), Item->GetZValue());
    XmlItem.setAttribute(tr("Rotate"), Item->GetRotateAngle());
    XmlItem.setAttribute(tr("RotateX"), Item->GetRotateCenterX());
    XmlItem.setAttribute(tr("RotateY"), Item->GetRotateCenterY());

    mColor = Item->GetLineColor();
    XmlItem.setAttribute(tr("LineColor"), mColor.name(QColor::HexArgb));

    mColor = Item->GetBackGroundColor();
    XmlItem.setAttribute(tr("BackColor"), mColor.name(QColor::HexArgb));

    // 插入文本属性
    InsertText(XmlItem, Item);

    // 插入按钮属性
    InsertButton(XmlItem, Item);

    InsertInstLamp(XmlItem, Item);
    InsertLabel(XmlItem, Item);

    InsertPixmap(XmlItem, Item);
    InsertGroupbox(XmlItem, Item);
    InsertCurve(XmlItem, Item);
    InsertCalButton(XmlItem, Item);
    InsertWarnLamp(XmlItem, Item);
    InsertSafeState(XmlItem, Item);
    InsertGPInput(XmlItem, Item);
    InsertGPOutPut(XmlItem, Item);

    return true;
}

bool TXmlWriter::Save(QString FileName)
{
    QFile mFile(FileName);

    if(!mFile.open( QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream mOut(&mFile);
    mOut.setCodec("UTF-8");
    m_Doc.save(mOut, 4);

    return true;
}

QString TXmlWriter::CreateJson(const QMap<QString, SENSOR_LIGHT_STRU> &temp)
{
    QString strJson = "";

    QVariantMap Lights;
    for(auto item = temp.begin(); item != temp.end(); ++item)
    {
        QString Id = item.key();
        QVariantList LightIds;
        foreach (QString str, item.value().light) {
            QVariantMap LightId;
            LightId.insert("信号灯",str);
            LightIds << LightId;
        }
        QVariantMap LightType;
        LightType.insert("亮灯种类",LightIds);
        LightType.insert("类型",item.value().name);

        Lights.insert(Id,LightType);
    }
    QJsonDocument doc = QJsonDocument::fromVariant(Lights);
    strJson = doc.toJson();
    return strJson;
}

QString TXmlWriter::TransExpreSymbol(const QString &Name)
{
    QString strExpre = Name;
    strExpre = strExpre.replace("&","$");     //用$替代&，防止xml把&当作转义符使用，则用$符号来替代&
    strExpre = strExpre.replace(">","≥");    //用≥替代>，防止xml在解析的时候，把<>作为标签对来解析，从而丢失了大于符号。
    strExpre = strExpre.replace("<","≤");    //用≤替代<，防止xml在解析的时候，把<>作为标签对来解析，从而丢失了小于符号。
    return strExpre;
}
