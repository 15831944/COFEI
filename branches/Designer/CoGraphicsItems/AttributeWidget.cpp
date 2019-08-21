#include "AttributeWidget.h"
#include <QVBoxLayout>
#include <QMetaEnum>
#include <CoGraphics/Graphics.h>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QApplication>
CoAttributeWidget::CoAttributeWidget(QWidget *parent) :QWidget(parent)
{
    m_Id = 0;
    m_Type = 0;
}

QString CoAttributeWidget::GetEnumToString(int ienum)
{
    const QMetaObject metaObject = CoAttributeWidget::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("CoAttrName");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    return  QString(en.valueToKey(ienum));
}

int CoAttributeWidget::GetStringToEnum(QString str)
{
    const QMetaObject metaObject = CoAttributeWidget::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("CoAttrName");
    QMetaEnum en = metaObject.enumerator(enumIndex);
    QByteArray ba = str.toLatin1();
    return en.keysToValue(ba.data());
}

void CoAttributeWidget::SetItem(CoItemEx *Item)
{
    SetId(Item->GetId());
    SetType(Item->GetType());
}

void CoAttributeWidget::SetWidget(CoProxyWidget *Item)
{
    SetId(Item->GetId());
    SetType(Item->GetType());
}

GraphicAttrWidget::GraphicAttrWidget(QWidget *parent) : CoAttributeWidget(parent)
{
    Q_INIT_RESOURCE(CoGraphicsItems);
    this->setStyleSheet(GetStyleSheetString());

    m_pGridLayout = new QGridLayout();
    QLabel *labelId = TBase::Label(tr("Id"),tr("labelId"),30,22);
    m_pLabelIdValue = TBase::Label(tr("001Test"),tr("labelIdValue"),80,22);
    m_pLabelIdValue->setFixedWidth(80);
    m_pGridLayout->addWidget(labelId,0,0);
    m_pGridLayout->addWidget(m_pLabelIdValue,0,1,Qt::AlignLeft);

    QLabel *labelType = TBase::Label(tr("类型"),tr("labelType"),30,22);
    m_pLabelTypeValue = TBase::Label(tr("TestType"),tr("labelTypeValue"),80,22);
    m_pGridLayout->addWidget(labelType,0,2,Qt::AlignRight);
    m_pGridLayout->addWidget(m_pLabelTypeValue,0,3,Qt::AlignLeft);

    m_pCmbSelectPower = TBase::ComboBox(tr("cmbSelectPermission"));
    m_pCmbSelectPower->setMinimumWidth(43);
    QStringList list;
    list << "0" << "1" << "2" << "3";
    m_pCmbSelectPower->addItems(list);
    m_pCmbSelectPower->setCurrentIndex(3);

    QHBoxLayout *pSPLayout = new QHBoxLayout();
    pSPLayout->addWidget(new QLabel(tr("用户权限")));
    pSPLayout->addWidget(m_pCmbSelectPower);
    pSPLayout->addStretch();

    m_pGridLayout->addLayout(pSPLayout,1,0,1,4);

}

void GraphicAttrWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint point = event->globalPos();
        QWidget * wg = QApplication::widgetAt(point);
        if(wg)
        {
            int iName = GetStringToEnum(wg->objectName());
            if(iName >= CoAttrName::Base && iName <= CoAttrName::Max)
                wg->setFocus();
        }
        event->accept();
    }
}

QString GraphicAttrWidget::GetStyleSheetString()
{
    return tr("QWidget{border-radius:0px;font:14px}"
              "QGroupBox{border:1px solid white;border-radius: 3px;margin-top: 1.5ex; }"
              "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;}"

              "QPushButton{border:1px solid white; border-radius:3px; padding-left:5px;padding-right:5px;}"
              "QPushButton::hover,QPushButton::selected{background-color:rgb(105, 178, 197);}"
              "QPushButton::!enabled{color: rgb(170, 170, 170);}"

              "QLineEdit::!enabled{color: rgb(170, 170, 170);}"

              "QComboBox{border:1px solid white;background:rgb(63, 147, 168);color: white;border-radius:3px;}"
              "QComboBox::drop-down {width: 20px;border: none;background: transparent;}"
              "QComboBox::down-arrow {image: url(:/Images/select-white.png);}"
              "QComboBox::!enabled{color: rgb(155, 155, 155);}"
              "QComboBox::down-arrow:!enabled {image: url(:/Images/select-grey.png);}"
              "QComboBox QAbstractItemView {border:1px solid white;background:rgb(63, 147, 168);outline: none;}"
              "QComboBox QAbstractItemView::item {height: 25px;color: rgb(175, 175, 175);}"
              "QComboBox QAbstractItemView::item:selected {background: rgba(255, 255, 255, 40);color:black;}"

              "QRadioButton{spacing: 2px;}"
              "QRadioButton::indicator{ padding-top: 2px;}"
              "QRadioButton::indicator:unchecked{image: url(:/Images/radiobuttonuncheck-white.png);}"
              "QRadioButton::indicator:checked {image: url(:/Images/radiobuttonchecked-white.png);}"
              "QRadioButton::!enabled{color: rgb(170, 170, 170);}"
              "QRadioButton::indicator:unchecked:!enabled{image: url(:/Images/radiobuttonuncheck-grey.png);}"
              "QRadioButton::indicator:checked:!enabled {image: url(:/Images/radiobuttonchecked-grey.png);}"


              "QSpinBox{border:1px solid white;border-radius:2px;}"
              "QSpinBox::!enabled{color: rgb(170, 170, 170);}"
              "QSpinBox::up-button:!enabled{image :url(:/Images/Up-grey.png);}"
              "QSpinBox::down-button:!enabled{image :url(:/Images/Down-grey.png);}"
              "QSpinBox::up-button{image :url(:/Images/Up.png);}"
              "QSpinBox::up-button:hover{background: rgb(105, 178, 197);}"
              "QSpinBox::down-button{image :url(:/Images/Down.png);}"
              "QSpinBox::down-button:hover{background: rgb(105, 178, 197);}"

              "QCheckBox{spacing: 2px;}"
              "QCheckBox::!enabled{color: rgb(170, 170, 170);}"
              "QCheckBox::indicator:checked:!enabled {image: url(:/Images/checked-grey.png);}"
              "QCheckBox::indicator:unchecked:!enabled {image: url(:/Images/unchecked-grey.png);}"
              "QCheckBox::indicator{ padding-top: 3px;}"
              "QCheckBox::indicator:checked {image: url(:/Images/checked-white.png);}"
              "QCheckBox::indicator:unchecked {image: url(:/Images/unchecked-white.png);}"

              "QTextEdit{border:1px solid white;}"
              "QTextEdit::!enabled{color: rgb(170, 170, 170);}"

              //水平
              "QTabBar::tab{padding-left: 8px;padding-right: 8px;"
                            "border: 1px solid white;border-bottom: none;"
                            "background-color:rgb(84, 148, 159);color: white;height: 24px;"
                            "margin-right: 3px;margin-top: 2px;}"
              "QTabWidget::pane {border: 1px solid white;background: rgb(105, 178, 197);"
                                "border-bottom-left-radius: 3px;border-bottom-right-radius: 3px;border-top-right-radius: 3px;}"
              "QTabBar::tab:selected,QTabBar::tab:hover{color: white;background: rgb(105, 178, 197);}"

              "QTreeView {border: 1px solid white;background-color: transparent;show-decoration-selected: 1;outline:0px;}"
              "QTreeView::item {height: 10px;  color: white;  border: none; background-color: transparent;}"
              "QTreeView::item:hover, QTreeView::branch:hover {background-color: rgba(255, 255, 255, 40);}"
              "QTreeView::item:selected, QTreeView::branch:selected {background-color: rgb(105, 178, 197);}"
              "QTreeView::branch {background-color: transparent;}"
              "QTreeView::branch:open:has-children {image: url(:/Images/tree-arrow-down.png);}"
              "QTreeView::branch:closed:has-children {image: url(:/Images/tree-arrow-right.png);}"
              "");
}


int CoAttributeWidget::GetId() const
{
    return m_Id;
}

int CoAttributeWidget::GetType() const
{
    return m_Type;
}

void CoAttributeWidget::SetId(const int id)
{
    m_Id = id;
}

void CoAttributeWidget::SetType(const int type)
{
    m_Type = type;
}

void GraphicAttrWidget::SetItem(CoItemEx *Item)
{
    if(NULL == Item)
        return;
    m_pLabelIdValue->setText(QString("%1").arg(Item->GetId()));
    m_pLabelTypeValue->setText(Item->GetEnumToString(Item->GetType()));
    m_pCmbSelectPower->setCurrentText(QString::number(Item->GetPower()));
    connect(m_pCmbSelectPower,SIGNAL(currentIndexChanged(int)),this,SLOT(PowerChanged(int)));
    CoAttributeWidget::SetItem(Item);
}

void GraphicAttrWidget::PowerChanged(int index)
{
    emit SendTAttribute(CoAttributeWidget::Power,QString::number(index),0);
}


