#include <QDebug>
#include <QDomDocument>
#include "DialogNewSafeXml.h"

DialogNewSafeXml::DialogNewSafeXml(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    this->setWindowTitle(tr("XML编辑"));
    this->resize(800,600);
    this->setStyleSheet(tr("QLabel{color:red;}"));
    this->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    m_pXMLEdit = new QTextEdit();
    m_pXMLEdit->setAcceptRichText(false);
    //编辑框增加语法高亮
    m_pHightLighter = new XmlSyntaxHighlighter(m_pXMLEdit->document());

    m_pFuncTree = new QTreeWidget();
    InitFuncTree();
    connect(m_pFuncTree,&QTreeWidget::itemDoubleClicked,this,&DialogNewSafeXml::AddFunc);

    QHBoxLayout *pTopLayout = new QHBoxLayout();
    pTopLayout->addWidget(m_pXMLEdit,3);
    pTopLayout->addWidget(m_pFuncTree,1);
    pMainLayout->addLayout(pTopLayout);

    QHBoxLayout *pBottomLayout = new QHBoxLayout();
    pMainLayout->addLayout(pBottomLayout);

    m_pLblError = TBase::Label(tr(""),tr("LblError"));
    m_pLblError->setFixedWidth(500);
    m_pLblError->setWordWrap(true);
    pBottomLayout->addWidget(m_pLblError,6,Qt::AlignLeft);

    m_pBtnOK = TBase::Button(tr("确定"),tr("BtnOK"),65,30);
    pBottomLayout->addWidget(m_pBtnOK,1,Qt::AlignTop);
    connect(m_pBtnOK,&QPushButton::clicked,this,&DialogNewSafeXml::SendJsString);

    QPushButton *pBtnQuit = TBase::Button(tr("取消"),tr("BtnQuit"),60,30);
    pBottomLayout->addWidget(pBtnQuit,1,Qt::AlignTop);
    connect(pBtnQuit,&QPushButton::clicked,this,&DialogNewSafeXml::close);
}

void DialogNewSafeXml::SetText(const QString &text)
{
    m_pXMLEdit->setPlainText(text);
}

void DialogNewSafeXml::InitFuncTree()
{
    m_pFuncTree->setColumnCount(1);
    m_pFuncTree->setHeaderLabel(tr(""));

    QTreeWidgetItem *pTopItem = new QTreeWidgetItem(m_pFuncTree,QStringList(QString(tr("安全信息"))));
    m_pFuncTree->addTopLevelItem(pTopItem);

    QList<QString> Texts;
    Texts << tr("急停") << tr("光栅") << tr("停止")
          << tr("复位") << tr("启动") << tr("回原点");

    QList<QTreeWidgetItem*> Items;
    for(int i = 0; i < Texts.size(); i++)
    {
        Items.append(new QTreeWidgetItem(pTopItem,QStringList(QString(Texts.at(i)))));
    }

    pTopItem->addChildren(Items);
    m_pFuncTree->expandAll();
}

QString DialogNewSafeXml::GetTextByItem(QTreeWidgetItem *item)
{
    QString strId = "";
    QString strMsg = "";
    if(item->text(0) == tr("急停"))
    {
        strId = tr("1");
        strMsg = tr("急停被按下");
    }
    else if(item->text(0) == tr("光栅"))
    {
        strId = tr("2");
        strMsg = tr("光栅被触发");
    }
    else if(item->text(0) == tr("停止"))
    {
        strId = tr("3");
        strMsg = tr("停止被按下");
    }
    else if(item->text(0) == tr("复位"))
    {
        strId = tr("4");
        strMsg = tr("复位被按下");
    }
    else if(item->text(0) == tr("启动"))
    {
        strId = tr("5");
        strMsg = tr("启动被按下");
    }
    else if(item->text(0) == tr("回原点"))
    {
        strId = tr("6");
        strMsg = tr("回原点被按下");
    }

    QString strText = tr("<安全信息 ID=\"%1\">\n"
                         "    <信号等级>%1</信号等级>\n"
                         "    <信号点位集合>\n"
                         "        <信号点位 bitname=\"E10.6\" value=\"0\">\n"
                         "            <输出状态信息>%2</输出状态信息>\n"
                         "        </信号点位>\n"
                         "        <信号点位 bitname=\"E10.5\" value=\"0\">\n"
                         "            <输出状态信息>%2</输出状态信息>\n"
                         "        </信号点位>\n"
                         "    </信号点位集合>\n"
                         "</安全信息>\n").arg(strId).arg(strMsg);

    return strText;
}

void DialogNewSafeXml::SendJsString()
{
    m_pLblError->setText("");

    //语法检查
    QString text = m_pXMLEdit->toPlainText();
    QString errorMsg = "";
    int errorLine = -1;
    int errorColumn = -1;
    QDomDocument doc;
    QString temp = tr("<root>%1</root>").arg(text);
    if(!doc.setContent(temp,&errorMsg,&errorLine,&errorColumn))
    {
        m_pLblError->setText(tr("Error: line: %1; %2").arg(errorLine).arg(errorMsg));
        return;
    }
    emit SafeStringChanged(text);
    this->close();
}

void DialogNewSafeXml::AddFunc(QTreeWidgetItem *item, int)
{
    if(!(item->text(0) == tr("安全信息")))
            m_pXMLEdit->textCursor().insertText(GetTextByItem(item));
    m_pXMLEdit->setFocus();
}
