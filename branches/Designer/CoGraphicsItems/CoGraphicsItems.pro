#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T11:21:38
#
#-------------------------------------------------

QT       += core gui script  xml
QT       += sql
QT       += script
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
TARGET = CoGraphicsItems
TEMPLATE = lib

DEFINES += COGRAPHICSITEMS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES +=  QT_DISABLE_DEPRECATED_BEFORE=0
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../SDK/lib -lCoGraphics
LIBS += -L../../SDK/lib -lStackManager -lXmlManager -lScriptManager -lTeachDeviceRunLib -lMainFrame
LIBS += -L../../SDK/lib -lDBInstanceManager -lDataBaseBatchManager

INCLUDEPATH += ../../SDK/include/

SOURCES += GraphicsItems.cpp \
    AttributeWidget.cpp \
    AttributeFactory.cpp \
    mytestwidget.cpp \
    TFlowGraphics/flowbase.cpp \
    TFlowGraphics/flowcall.cpp \
    TFlowGraphics/flowclear.cpp \
    TFlowGraphics/Flowcylinder.cpp \
    TFlowGraphics/flowdelay.cpp \
    TFlowGraphics/flowds.cpp \   
    TFlowGraphics/FlowEnd.cpp \
    TFlowGraphics/FlowIf.cpp \
    TFlowGraphics/flowin.cpp \
    TFlowGraphics/flowlabel.cpp \
    TFlowGraphics/flowlin.cpp \
    TFlowGraphics/flowlout.cpp \
    TFlowGraphics/FlowMoveG00.cpp \
    TFlowGraphics/flowmoveg101.cpp \
    TFlowGraphics/flowout.cpp \
    TFlowGraphics/flowthread.cpp \
    TFlowGraphics/tflowstart.cpp \
    TFlowGraphics/flowsub.cpp \
    TFlowGraphics/tflowtcpw.cpp \
    TFlowGraphics/tflowtcpr.cpp \
    TFlowGraphics/tflowg04.cpp \
    TFlowGraphics/tflowjs.cpp \
    TFlowGraphics/tfloworg.cpp \
    TFlowGraphics/flowwait.cpp \
    TFlowGraphics/flowlog.cpp \
    FlowPropery/PropertyBrowser/objectcontroller.cpp \
    FlowPropery/PropertyBrowser/qtbuttonpropertybrowser.cpp \
    FlowPropery/PropertyBrowser/qteditorfactory.cpp \
    FlowPropery/PropertyBrowser/qtgroupboxpropertybrowser.cpp \
    FlowPropery/PropertyBrowser/qtpropertybrowser.cpp \
    FlowPropery/PropertyBrowser/qtpropertybrowserutils.cpp \
    FlowPropery/PropertyBrowser/qtpropertymanager.cpp \
    FlowPropery/PropertyBrowser/qttreepropertybrowser.cpp \
    FlowPropery/PropertyBrowser/qtvariantproperty.cpp \
    FlowPropery/PropertyItems/callproperty.cpp \
    FlowPropery/PropertyItems/clearproperty.cpp \
    FlowPropery/PropertyItems/cylinderproperty.cpp \
    FlowPropery/PropertyItems/cytproperty.cpp \
    FlowPropery/PropertyItems/delayproperty.cpp \
    FlowPropery/PropertyItems/dsproperty.cpp \
    FlowPropery/PropertyItems/endproperty.cpp \
    FlowPropery/PropertyItems/flowproperty.cpp \
    FlowPropery/PropertyItems/ifproperty.cpp \
    FlowPropery/PropertyItems/inproperty.cpp \
    FlowPropery/PropertyItems/labelproperty.cpp \
    FlowPropery/PropertyItems/linproperty.cpp \
    FlowPropery/PropertyItems/loutproperty.cpp \
    FlowPropery/PropertyItems/moveg00property.cpp \
    FlowPropery/PropertyItems/moveg101property.cpp \
    FlowPropery/PropertyItems/outproperty.cpp \
    FlowPropery/PropertyItems/startproperty.cpp \
    FlowPropery/PropertyItems/subproperty.cpp \
    FlowPropery/PropertyItems/testproperty.cpp \
    FlowPropery/PropertyItems/threadproperty.cpp \
    FlowPropery/PropertyItems/jsproperty.cpp \
    FlowPropery/PropertyItems/g04property.cpp \
    FlowPropery/PropertyItems/tcpwproperty.cpp \
    FlowPropery/PropertyItems/tcprproperty.cpp \
    FlowPropery/PropertyBrowser/pointset.cpp \
    FlowPropery/PropertyItems/logproperty.cpp \
    FlowPropery/PropertyItems/waitproperty.cpp \
    FlowPropery/PropertyItems/orgproperty.cpp \
    FlowPropery/libproperybase.cpp \
    FlowPropery/itemsave.cpp \
    XML/XmlReader.cpp \
    XML/XmlWriter.cpp\
    TCharts/Chart.cpp \
    AttributeWidget/AttributeButton.cpp \
    AttributeWidget/SelectVar/DialogSelectVar.cpp \
    AttributeWidget/NewExcuteJs/DialogNewExcuteJs.cpp \
    AttributeWidget/NewXML/DialogNewSafeXml.cpp \
    AttributeWidget/NewXML/xmlsyntaxhighlighter.cpp\
    AttributeWidget/AttriBase.cpp\
    AttributeWidget/AttriLabel.cpp \
    AttributeWidget/AttriIOLamp.cpp \
    AttributeWidget/AttriTextEdit.cpp \
    AttributeWidget/AttriPixmap.cpp \
    AttributeWidget/AttriGroupBox.cpp \
    AttributeWidget/AttriCurve.cpp\
    AttributeWidget/AttriCalButton.cpp \
    AttributeWidget/AttriSafeState.cpp \
    AttributeWidget/AttriWarmLamp.cpp \
    AttributeWidget/AttriTableView.cpp \
    AttributeWidget/AttriTouchPP.cpp \
    TBasicControls/Button.cpp \
    TBasicControls/Label.cpp \
    TBasicControls/ComboBox.cpp \
    TBasicControls/GroupBox.cpp \
    TBasicControls/Pixmap.cpp \
    TBasicControls/Slider.cpp \
    TBasicControls/TextEdit.cpp \
    TBasicControls/CalButton.cpp \
    TBasicControls/CircClock.cpp \
    TBasicControls/Curve.cpp \
    TBasicControls/Diamond.cpp \
    TBasicControls/Ellipse.cpp \
    TBasicControls/IOLamp.cpp \
    TBasicControls/Line.cpp \
    TBasicControls/Meter.cpp \
    TBasicControls/Rect.cpp \
    TBasicControls/RectClock.cpp \
    TBasicControls/TableView.cpp \
    TBasicControls/TArrow.cpp \
    TBasicControls/WarnLamp.cpp \
    TBasicControls/SafeState.cpp \
    CoProxyItem/nightsetwidget.cpp \
    CoProxyItem/koroldelegate.cpp \
    CoProxyItem/loginfrom.cpp \
    FlowPropery/PropertyItems/uartproperty.cpp \
    TFlowGraphics/tflowuart.cpp \
    TFlowGraphics/tflowcyt.cpp \
    FlowPropery/PropertyItems/countproperty.cpp \
    TFlowGraphics/tflowcount.cpp \
    CoProxyItem/TouchPP.cpp \
    CoProxyItem/AxisOperater.cpp\
    FlowPropery/PropertyItems/jogproperty.cpp \
    TFlowGraphics/flowjog.cpp \
    TBasicControls/GPInPut.cpp \
    TBasicControls/GPOutPut.cpp \
    AttributeWidget/AttriGPOutPut.cpp \
    AttributeWidget/AttriGPInPut.cpp \
    CoProxyItem/HandMode.cpp \
    CoProxyItem/NewPPDialog.cpp
HEADERS += GraphicsItems.h \
    CoGraphicsItems_Global.h \
    mytestwidget.h \
    AttributeWidget.h \
    AttributeFactory.h \
    TFlowGraphics/flowbase.h \
    TFlowGraphics/flowcall.h \
    TFlowGraphics/flowclear.h \
    TFlowGraphics/Flowcylinder.h \
    TFlowGraphics/flowdelay.h \
    TFlowGraphics/flowds.h \
    TFlowGraphics/FlowEnd.h \
    TFlowGraphics/FlowIf.h \
    TFlowGraphics/flowin.h \
    TFlowGraphics/flowlabel.h \
    TFlowGraphics/flowlin.h \
    TFlowGraphics/flowlout.h \
    TFlowGraphics/FlowMoveG00.h \
    TFlowGraphics/flowmoveg101.h \
    TFlowGraphics/flowout.h \
    TFlowGraphics/flowthread.h \
    TFlowGraphics/tflowstart.h \
    TFlowGraphics/flowsub.h \
    TFlowGraphics/tflowtcpw.h \
    TFlowGraphics/tflowtcpr.h \
    TFlowGraphics/tflowg04.h \
    TFlowGraphics/tflowjs.h \
    TFlowGraphics/flowwait.h \
    TFlowGraphics/flowlog.h \
    TFlowGraphics/tfloworg.h \
    TFlowGraphics/FlowGraphicsPub.h \
    FlowPropery/PropertyBrowser/objectcontroller.h \
    FlowPropery/PropertyBrowser/qtbuttonpropertybrowser.h \
    FlowPropery/PropertyBrowser/qteditorfactory.h \
    FlowPropery/PropertyBrowser/qtgroupboxpropertybrowser.h \
    FlowPropery/PropertyBrowser/qtpropertybrowser.h \
    FlowPropery/PropertyBrowser/qtpropertybrowserutils_p.h \
    FlowPropery/PropertyBrowser/qtpropertymanager.h \
    FlowPropery/PropertyBrowser/qttreepropertybrowser.h \
    FlowPropery/PropertyBrowser/qtvariantproperty.h \
    FlowPropery/PropertyItems/callproperty.h \
    FlowPropery/PropertyItems/clearproperty.h \
    FlowPropery/PropertyItems/cylinderproperty.h \
    FlowPropery/PropertyItems/cytproperty.h \
    FlowPropery/PropertyItems/delayproperty.h \
    FlowPropery/PropertyItems/dsproperty.h \
    FlowPropery/PropertyItems/endproperty.h \
    FlowPropery/PropertyItems/flowproperty.h \
    FlowPropery/PropertyItems/ifproperty.h \
    FlowPropery/PropertyItems/inproperty.h \
    FlowPropery/PropertyItems/labelproperty.h \
    FlowPropery/PropertyItems/linproperty.h \
    FlowPropery/PropertyItems/loutproperty.h \
    FlowPropery/PropertyItems/moveg00property.h \
    FlowPropery/PropertyItems/moveg101property.h \
    FlowPropery/PropertyItems/outproperty.h \
    FlowPropery/PropertyItems/startproperty.h \
    FlowPropery/PropertyItems/subproperty.h \
    FlowPropery/PropertyItems/testproperty.h \
    FlowPropery/PropertyItems/threadproperty.h \
    FlowPropery/PropertyItems/jsproperty.h \
    FlowPropery/PropertyItems/g04property.h \
    FlowPropery/PropertyItems/tcpwproperty.h \
    FlowPropery/PropertyItems/tcprproperty.h \
    FlowPropery/PropertyItems/orgproperty.h \
    FlowPropery/PropertyBrowser/pointset.h \
    FlowPropery/PropertyItems/logproperty.h \
    FlowPropery/PropertyItems/waitproperty.h \
    FlowPropery/libproperybase.h \
    FlowPropery/itemsave.h \
    XML/XmlReader.h \
    XML/XmlWriter.h \
    TCharts/Chart.h \
    AttributeWidget/AttriLabel.h \
    AttributeWidget/AttriIOLamp.h \
    AttributeWidget/AttriTextEdit.h \
    AttributeWidget/AttriPixmap.h \
    AttributeWidget/AttriGroupBox.h \
    AttributeWidget/AttriCurve.h \
    AttributeWidget/AttriCalButton.h \
    AttributeWidget/SelectVar/DialogSelectVar.h \
    AttributeWidget/NewExcuteJs/DialogNewExcuteJs.h \
    AttributeWidget/NewXML/DialogNewSafeXml.h \
    AttributeWidget/NewXML/xmlsyntaxhighlighter.h \
    AttributeWidget/AttriBase.h\
    AttributeWidget/AttributeButton.h \
    AttributeWidget/AttriSafeState.h  \
    AttributeWidget/AttriWarmLamp.h \
    AttributeWidget/AttriTableView.h \
    AttributeWidget/AttriTouchPP.h \
    TBasicControls/Button.h \
    TBasicControls/Label.h \
    TBasicControls/ComboBox.h \
    TBasicControls/GroupBox.h \
    TBasicControls/Pixmap.h \
    TBasicControls/Slider.h \
    TBasicControls/TextEdit.h \
    TBasicControls/CircClock.h \
    TBasicControls/Curve.h \
    TBasicControls/Diamond.h \
    TBasicControls/Ellipse.h \
    TBasicControls/IOLamp.h \
    TBasicControls/Line.h \
    TBasicControls/Meter.h \
    TBasicControls/Rect.h \
    TBasicControls/RectClock.h \
    TBasicControls/TableView.h \
    TBasicControls/TArrow.h \
    TBasicControls/CalButton.h \
    TBasicControls/WarnLamp.h \    
    TBasicControls/BasicControlsPub.h \
    TBasicControls/SafeState.h \
    CoProxyItem/nightsetwidget.h \
    CoProxyItem/koroldelegate.h \
    CoProxyItem/loginfrom.h \
    FlowPropery/PropertyItems/uartproperty.h \
    TFlowGraphics/tflowuart.h \
    TFlowGraphics/tflowcyt.h \
    FlowPropery/PropertyItems/countproperty.h \
    TFlowGraphics/tflowcount.h \
    CoProxyItem/TouchPP.h \
    CoProxyItem/AxisOperater.h\
    FlowPropery/PropertyItems/jogproperty.h \
    TFlowGraphics/flowjog.h \
    AttributeWidget/AttriGPInPut.h \
    AttributeWidget/AttriGPOutPut.h \
    TBasicControls/GPInPut.h \
    TBasicControls/GPOutPut.h \
    CoProxyItem/HandMode.h \
    CoProxyItem/NewPPDialog.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    CoGraphicsItems.qrc


