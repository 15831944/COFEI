#include "GraphicsItems.h"
#include "TFlowGraphics/FlowGraphicsPub.h"
#include "TBasicControls/BasicControlsPub.h"
#include "mytestwidget.h"
#include <Comm.h>
#include "CoProxyItem/nightsetwidget.h"
#include "CoProxyItem/loginfrom.h"
#include "CoProxyItem/TouchPP.h"
CoGraphicsItems::CoGraphicsItems()
{
}
CoItem *CoGraphicsItems::CreateItemFactory(int Type, double StartX, double StartY, double StopX, double StopY,
                                       const Qt::PenStyle &LineStyle, const int LineWidth, const QColor &LineColor,
                                       const QColor &BackColor)
{
    CoItem *Item = NULL;
    switch (Type)
    {
    case CoItem::Pixmap:
    {
        Item = new TPixmap(StartX, StartY, StopX, StopY,
                           LineStyle, LineWidth, LineColor, BackColor);
    }
        break;

    case CoItem::Button:
    {
        Item = new TButton(StartX, StartY, StopX, StopY,
                           LineStyle, LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::CalBtn:
    {
        Item = new TCalButton(StartX, StartY, StopX, StopY,
                              LineStyle, LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Line:
    {
        Item = new TLine(StartX, StartY, StopX, StopY, LineStyle,
                         LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::GroupBox:
    {
        Item = new TGroupBox(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
        Item->SetZValue(-1000);
    }
        break;
    case CoItem::Iolamp:
    {
        Item = new TIOLamp(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Warnlamp:
    {
        Item = new TWarnLamp(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::TextEdit:
    {
        Item = new TTextEdit(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Label:
    {
        Item = new TLabel(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::GPInPut:
    {
        Item = new TGPInPut(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::GPOutPut:
    {
        Item = new TGPOutPut(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Rect:
    {
        Item = new TRect(StartX, StartY, StopX, StopY, LineStyle,
                         LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Diamond:
    {
        Item = new TDiamond(StartX, StartY, StopX, StopY, LineStyle,
                            LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Ellipse:
    {
        Item = new TEllipse(StartX, StartY, StopX, StopY, LineStyle,
                            LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Arrow:
    {
        Item = new TArrow(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Meter:
    {
        Item = new TMeter(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::CricClock:
    {
        Item = new TCircClock(StartX, StartY, StopX, StopY, LineStyle,
                              LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::RectClock:
    {
        Item = new TRectClock(StartX, StartY, StopX, StopY, LineStyle,
                              LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::SliderF:
    {
        Item = new TSlider(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::ComboBox:
    {
        Item = new TComboBox(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::TableView:
    {
        Item = new TTableView(StartX, StartY, StopX, StopY, LineStyle,
                              LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Chart:
    {
        Item = new TCurve(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
        Item->SetZValue(-500);
    }
        break;
    case CoItem::SafeState:
    {
        Item = new TSafeState(StartX,StartY,StopX,StopY,LineStyle,
                              LineWidth,LineColor,BackColor);
    }
        break;
    case CoItem::FlowStart:
    {
        Item = new TFlowStart(StartX, StartY, StopX, StopY, LineStyle,
                              LineWidth, LineColor, BackColor);

    }
        break;
    case CoItem::FlowEnd:
    {
        Item = new TFlowEnd(StartX, StartY, StopX, StopY, LineStyle,
                            LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::FlowThread:
    {
        Item = new TFlowThread(StartX, StartY, StopX, StopY, LineStyle,
                               LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::FlowSport:
    {
        Item = new TFlowCyLinder(StartX, StartY, StopX, StopY, LineStyle,
                                 LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::FlowCall:
    {
        Item = new TFlowCall(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Flowflow:
    {
        Item = new FlowSub(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::In:
    {
        Item = new FlowIn(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Out:
    {
        Item = new FlowOut(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Lin:
    {
        Item = new FlowLin(StartX, StartY, StopX, StopY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Lout:
    {
        Item = new FlowLout(StartX, StartY, StopX, StopY, LineStyle,
                            LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::IF:
    {
        Item = new TFlowIf (StartX, StartY, StopX, StopY, LineStyle,
                            LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::IDs:
    {
        Item = new flowlabel (StartX, StartY, StopX, StopY, LineStyle,
                              LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::G00:
    {
        Item = new TFlowMoveG00 (StartX, StartY, StopX, StopY, LineStyle,
                                 LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::G101:
    {
        Item = new TFlowMoveG101 (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;

    case CoItem::Js:
    {
        Item = new TFlowJS (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::G04:
    {
        Item = new TFlowG04 (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Tcp_W:
    {
        Item = new TFlowTCPW (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Tcp_R:
    {
        Item = new TFlowTCPR (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Dely:
    {
        Item = new TFlowDelay (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Org:
    {
        Item = new TFlowORG (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Wait:
    {
        Item = new FlowWAIT (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Log:
    {
        Item = new FlowLOG (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Ds:
    {
        Item = new TFlowDS (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::RS_R:
    {
        Item = new TFlowUart (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Cyt:
    {
        Item = new TFlowCyt (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Add:
    {
        Item = new TFlowCount (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    case CoItem::Jog:
    {
        Item = new FlowJog (StartX, StartY, StopX, StopY, LineStyle,
                                  LineWidth, LineColor, BackColor);
    }
        break;
    default:
        break;
    }
    return Item;
}

CoProxyWidget *CoGraphicsItems::CreateWidgetFactory(int Type, double StartX, double StartY,
                                                    double StopX, double StopY, const Qt::PenStyle &LineStyle,
                                                    const int LineWidth, const QColor &LineColor,
                                                    const QColor &BackColor, CoScene *Scene)
{
    if(!Scene)
        return NULL;
    CoProxyWidget *Item = NULL;
    switch (Type) {
    case CoItem::Mde_Safe_Sig:
         Item = new NightSetWidget(StartX, StartY, StopX, StopY, LineStyle,
                                 LineWidth, LineColor, BackColor,Scene);
        break;
    case CoItem::Mde_Login:
         Item = new LoginFrom(StartX, StartY, StopX, StopY, LineStyle,
                                 LineWidth, LineColor, BackColor,Scene);
        break;
    case CoItem::Mde_Axis_dug:
         Item = new TouchPP(StartX, StartY, StopX, StopY, LineStyle,
                                 LineWidth, LineColor, BackColor,Scene);
        break;

    default:
        break;
    }
    return Item;
}
