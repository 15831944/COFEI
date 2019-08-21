#include "AttributeFactory.h"
#include <CoGraphics/Graphics.h>
#include "AttributeWidget/AttributeButton.h"
#include "AttributeWidget/AttriLabel.h"
#include "AttributeWidget/AttriIOLamp.h"
#include "AttributeWidget/AttriTextEdit.h"
#include "AttributeWidget/AttriGroupBox.h"
#include "AttributeWidget/AttriPixmap.h"
#include "AttributeWidget/AttriCurve.h"
#include "AttributeWidget/AttriCalButton.h"
#include "AttributeWidget/AttriSafeState.h"
#include "AttributeWidget/AttriWarmLamp.h"
#include "AttributeWidget/AttriTableView.h"
#include "AttributeWidget/AttriTouchPP.h"
#include "AttributeWidget/AttriGPOutPut.h"
#include "AttributeWidget/AttriGPInPut.h"

#include "FlowPropery/PropertyBrowser/objectcontroller.h"
#include "FlowPropery/libproperybase.h"
#include "FlowPropery/PropertyBrowser/pointset.h"
#include "TFlowGraphics/flowbase.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>

AttributeFactory::AttributeFactory()
{

}
CoAttributeWidget *AttributeFactory::CreateAttributeWidget(int Type,CoProxyWidget*)
{
    CoAttributeWidget * attr = NULL;
    switch (Type)
    {
//    case CoItem::Mde_Safe_Sig:
//    {
//        attr = new CoAttributeWidget();
//        attr->SetWidget(Item);
//        break;
//    }
    default:
        break;
    }
    return attr;
}

CoAttributeWidget *AttributeFactory::CreateAttributeWidget(int Type,CoItemEx *Item)
{
    CoAttributeWidget * attr = NULL;
    switch (Type)
    {
    case CoItem::Button:
    {
        attr = new AttributeButton();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Label:
    {
        attr = new AttriLabel();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Iolamp:
    {
        attr = new AttriIOLamp();
        attr->SetItem(Item);
        break;
    }
    case CoItem::TextEdit:
    {
        attr = new AttriTextEdit();
        attr->SetItem(Item);
        break;
    }
    case CoItem::GroupBox:
    {
        attr = new AttriGroupBox();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Pixmap:
    {
        attr = new AttriPixmap();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Chart:
    {
        attr = new AttriCurve();
        attr->SetItem(Item);
        break;
    }
    case CoItem::CalBtn:
    {
        attr = new AttriCalButton();
        attr->SetItem(Item);
        break;
    }
    case CoItem::SafeState:
    {
        attr = new AttriSafeState();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Warnlamp:
    {
        attr = new AttriWarmLamp();
        attr->SetItem(Item);
        break;
    }
    case CoItem::TableView:
    {
        attr = new AttriTableView();
        attr->SetItem(Item);
        break;
    }
    case CoItem::Mde_Axis_dug:
    {
        attr = new AttriTouchPP();
        attr->SetItem(Item);
        break;
    }
    case CoItem::GPInPut:
    {
        attr = new AttriGPInPut();
        attr->SetItem(Item);
        break;
    }
    case CoItem::GPOutPut:
    {
        attr = new AttriGPOutPut();
        attr->SetItem(Item);
        break;
    }
    case CoItem::FlowStart:
    case CoItem::FlowThread:
    case CoItem::FlowCall:
    case CoItem::FlowIF:
    case CoItem::FlowSport:
    case CoItem::FlowArrow:
    case CoItem::FlowEnd:
    case CoItem::Flowflow:
    case CoItem::M00:
    case CoItem::M10:
    case CoItem::Goto:
    case CoItem::IDs:
    case CoItem::G00:
    case CoItem::G101:
    case CoItem::Dely:
    case CoItem::Add:
    case CoItem::Ds:
    case CoItem::Post:
    case CoItem::In:
    case CoItem::Out:
    case CoItem::Lin:
    case CoItem::Lout:
    case CoItem::Jog:
    case CoItem::T01:
    case CoItem::Org:
    case CoItem::Log:
    case CoItem::Cyt:
    case CoItem::G01:
    case CoItem::G02:
    case CoItem::G03:
    case CoItem::G04:
    case CoItem::G05:
    case CoItem::Js:
    case CoItem::Tcp_R:
    case CoItem::Tcp_W:
    case CoItem::RS_R:
    case CoItem::RS_w:
    case CoItem::Wait:
    case CoItem::Mov:
    case CoItem::B_1:
    case CoItem::IF:
    {
        attr = new ObjectController();
        attr->SetItem(Item);
        TFlowBase *bs=dynamic_cast<TFlowBase*>(Item);
        LibProperyBase *bsPro=bs->PropertyBase;
        //Read Data From data base
        QStringList PointName;

        for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.qpMap.begin();
            it != DBInstanceManager::getDBInstance()->DBVarStru.qpMap.end(); it++)
        {
            PointName.append(it.key());
        }
        bsPro->pointName.value=PointName;
        //Updata Point
        dynamic_cast<ObjectController*>(attr)->updataDynamic();
        break;
    }
    default:
        break;
    }

    return attr;
}
