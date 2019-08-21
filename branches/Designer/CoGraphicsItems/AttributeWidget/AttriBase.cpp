#include <QDebug>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QtCore>
#include <QMessageBox>
#include "AttriBase.h"
#include "SelectVar/DialogSelectVar.h"
#include "NewExcuteJs/DialogNewExcuteJs.h"

AttriBase::AttriBase()
{

}

TextAttri *AttriBase::textAttri(QWidget *parent /*=0*/)
{
    return new TextAttri(parent);
}

FontAttri *AttriBase::fontAttri(QWidget *parent /*=0*/)
{
    return new FontAttri(parent);
}

BackgroundAttri *AttriBase::backgroundAttri(QWidget *parent /*=0*/)
{
    return new BackgroundAttri(parent);
}

EnableAttri *AttriBase::enableAttri(QWidget *parent /*=0*/)
{
    return new EnableAttri(parent);
}

VarSelectedAttri *AttriBase::varSelectedAttri(QWidget *parent)
{
    return new VarSelectedAttri(parent);
}

ExpreSelectedAttri *AttriBase::expreSelectAttri(QWidget *parent)
{
    return new ExpreSelectedAttri(parent);
}

JsTacticAttri *AttriBase::jsTacticAttri(QWidget *parent)
{
    return new JsTacticAttri(parent);
}

ColorSelect *AttriBase::colorSelect(QWidget *parent)
{
    return new ColorSelect(parent);
}

TopBottom *AttriBase::topBottom(QWidget *parent)
{
    return new TopBottom(parent);
}

//---------------------文本属性-----------------------------------
TextAttri::TextAttri(QWidget *parent) : QWidget(parent)
{
    m_bFlag = false;

    QVBoxLayout *pTextLayout = new QVBoxLayout();
    this->setLayout(pTextLayout);
    this->setParent(parent);

    QLabel *labelText = TBase::Label(tr("文本"),tr("labelText"));
    labelText->setFixedWidth(30);

    m_pTextEditText = TBase::TextEdit(tr("Text"));
    m_pTextEditText->setMaximumHeight(60);
    m_pTextEditText->setAcceptRichText(false);
    m_pTextEditText->installEventFilter(this);

    pTextLayout->addWidget(labelText,0,Qt::AlignLeft);
    pTextLayout->addWidget(m_pTextEditText);
}

void TextAttri::SetText(const QString &text)
{
    m_pTextEditText->setPlainText(text);
}

QString TextAttri::GetText() const
{
    return m_pTextEditText->toPlainText();
}

bool TextAttri::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_pTextEditText)
    {
        if(event->type() == QEvent::FocusIn)
        {
            m_bFlag = true;
        }
        else if(event->type() == QEvent::FocusOut)
        {
            if(m_bFlag)
            {
                m_bFlag = false;
                emit SendTextFinfished(GetText());
                return true;
            }
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Enter)
            {
                emit SendTextFinfished(GetText());
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}
//----------------------end--------------------------------------

//---------------------字体属性-----------------------------------
#define MINFONTSIZE 9
#define MAXFONTSIZE 31
FontAttri::FontAttri(QWidget *parent)
{
    m_FontColor = Qt::white;
    QGridLayout *pFontLayout = new QGridLayout();
    this->setLayout(pFontLayout);
    this->setParent(parent);

    QLabel *lblFontSize = TBase::Label(tr("字体大小"),tr("lblFontSize"));
    m_pCmbFontSize = TBase::ComboBox(tr("FontSize"));
    m_pCmbFontSize->setMinimumWidth(42);
    QStringList list;
//    for(int i = MINFONTSIZE; i <= MAXFONTSIZE; i++)
//    {
//        list.append(QString::number(i));
//        i++;
//    }
    list << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "14" << "16"
         << "18" << "20" << "22" << "26" << "28" << "36" << "48" << "72";
    m_pCmbFontSize->addItems(list);
    connect(m_pCmbFontSize,
            static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
            this,&FontAttri::FontSizeChanged);


    pFontLayout->addWidget(lblFontSize,0,0,Qt::AlignRight);
    pFontLayout->addWidget(m_pCmbFontSize,0,1,1,2,Qt::AlignLeft);

    QLabel *lblFontColor = TBase::Label(tr("颜色"),tr("lblFontColor"));
    m_plblFontColor = TBase::Label(tr(""), tr("lblFontColor"), 30, 22);
    TBase::ColorLabel(m_plblFontColor, m_FontColor);

    QPushButton *pBtnFontColor = TBase::Button(tr("浏览"), tr("pBtnFontColor"));
    connect(pBtnFontColor,SIGNAL(clicked()), this, SLOT(ColorChanged()));

    pFontLayout->addWidget(lblFontColor,0,3,Qt::AlignRight);
    pFontLayout->addWidget(m_plblFontColor,0,4,Qt::AlignCenter);
    pFontLayout->addWidget(pBtnFontColor,0,5,Qt::AlignLeft);

    QGroupBox *pGroupHAlign = new QGroupBox(tr("水平对齐"));
    pGroupHAlign->setContentsMargins(5,10,5,10);
    pFontLayout->addWidget(pGroupHAlign,1,0,1,3);
    QVBoxLayout *pHALayout = new QVBoxLayout();
    m_pHorButtonGroup = new QButtonGroup();
    m_pRdoHLeft = new QRadioButton(tr("左对齐"));
    m_pRdoHCenter = new QRadioButton(tr("中对齐"));
    m_pRdoHRight = new QRadioButton(tr("右对齐"));

    m_pHorButtonGroup->addButton(m_pRdoHLeft,0);
    m_pHorButtonGroup->addButton(m_pRdoHCenter,1);
    m_pHorButtonGroup->addButton(m_pRdoHRight,2);
    m_pRdoHCenter->setChecked(true);
    connect(m_pHorButtonGroup,SIGNAL(buttonClicked(int)),this,SIGNAL(AlignHorChanged(int)));

    pHALayout->addWidget(m_pRdoHLeft);
    pHALayout->addWidget(m_pRdoHCenter);
    pHALayout->addWidget(m_pRdoHRight);
    pGroupHAlign->setLayout(pHALayout);

    QGroupBox *pGroupVAlign = new QGroupBox(tr("垂直对齐"));
    pGroupVAlign->setContentsMargins(5,10,5,10);
    pFontLayout->addWidget(pGroupVAlign,1,3,1,3);
    QVBoxLayout *pVALayout = new QVBoxLayout();
    m_pVerButtonGroup = new QButtonGroup();
    m_pRdoVTop = new QRadioButton(tr("上对齐"));
    m_pRdoVCenter = new QRadioButton(tr("中对齐"));
    m_pRdoVBottom = new QRadioButton(tr("下对齐"));

    m_pVerButtonGroup->addButton(m_pRdoVTop,0);
    m_pVerButtonGroup->addButton(m_pRdoVCenter,1);
    m_pVerButtonGroup->addButton(m_pRdoVBottom,2);
    m_pRdoVCenter->setChecked(true);
    connect(m_pVerButtonGroup,SIGNAL(buttonClicked(int)),this,SIGNAL(AlignVerChanged(int)));

    pVALayout->addWidget(m_pRdoVTop);
    pVALayout->addWidget(m_pRdoVCenter);
    pVALayout->addWidget(m_pRdoVBottom);
    pGroupVAlign->setLayout(pVALayout);
}

void FontAttri::SetFontSize(const QString &text)
{
    m_pCmbFontSize->setCurrentText(text);
}

QString FontAttri::GetFontSize() const
{
    return m_pCmbFontSize->currentText();
}

void FontAttri::SetFontColor(QColor color)
{
    m_FontColor = color;
    TBase::ColorLabel(m_plblFontColor,m_FontColor);
}

QColor FontAttri::GetFontColor()
{
    return m_FontColor;
}

void FontAttri::SetAlignVer(int id)
{
    m_pVerButtonGroup->button(id)->setChecked(true);
}

int FontAttri::GetAlignVer()
{
    return m_pVerButtonGroup->checkedId();
}

void FontAttri::SetAligHor(int id)
{
    m_pHorButtonGroup->button(id)->setChecked(true);
}

int FontAttri::GetAlignHor()
{
    return m_pHorButtonGroup->checkedId();
}

void FontAttri::ColorChanged()
{
    if(TBase::ChooseColor(m_FontColor))
    {
        TBase::ColorLabel(m_plblFontColor, m_FontColor);
    }
    emit FontColorChanged(m_FontColor);
}
//----------------------end--------------------------------------


//---------------------背景属性-----------------------------------
BackgroundAttri::BackgroundAttri(QWidget *parent):
    QWidget(parent)
{
    QGridLayout *pBackgroundLayout = new QGridLayout();
    this->setLayout(pBackgroundLayout);
    this->setParent(parent);

    QLabel *lblBackgroundColor = TBase::Label(tr("颜色"),tr("lblBackgroundColor"),40,22);
    m_pLblBackgroundColor = TBase::Label(tr(""),tr("labelBackgroundColor"), 30, 22);
    TBase::ColorLabel(m_pLblBackgroundColor, Qt::black);
    m_pBtnBackgroundColor = TBase::Button(tr("浏览"), tr("pButtonBackgroundColor"),40,22);
    connect(m_pBtnBackgroundColor,SIGNAL(clicked()), this, SLOT(ColorChanged()));

    m_pChkIsImage = TBase::CheckBox(tr("使用图"),tr("checkboxIsImage"));
    m_pChkIsImage->setFixedWidth(80);
    m_pChkIsImage->setParent(this);
    connect(m_pChkIsImage,SIGNAL(stateChanged(int)),this,SLOT(IsUseImage(int)));

    QLabel *lblImageColor = TBase::Label(tr("背景图"),tr("lblImageColor"));
    m_pLineEditImagePath = TBase::LineEdit(tr("lineEditImagePath"));
    m_pLineEditImagePath->setParent(this);
    m_pLineEditImagePath->setFixedHeight(25);

    m_pBtnSelectImagePath = TBase::Button(tr("浏览"),tr("buttonSelectImagePath"),40,22);
    connect(m_pBtnSelectImagePath,&QPushButton::clicked,this,&BackgroundAttri::SelectImagePath);

    pBackgroundLayout->addWidget(lblBackgroundColor,0,0);
    pBackgroundLayout->addWidget(m_pLblBackgroundColor,0,1,Qt::AlignCenter);
    pBackgroundLayout->addWidget(m_pBtnBackgroundColor,0,2,Qt::AlignLeft);
    pBackgroundLayout->addWidget(m_pChkIsImage,0,3,Qt::AlignCenter);
    pBackgroundLayout->addWidget(lblImageColor,1,0);
    pBackgroundLayout->addWidget(m_pLineEditImagePath,1,1,1,2);
    pBackgroundLayout->addWidget(m_pBtnSelectImagePath,1,3);
    pBackgroundLayout->setAlignment(Qt::AlignTop);
}

void BackgroundAttri::SetColor(QColor color)
{
    m_BackgroundColor = color;
    TBase::ColorLabel(m_pLblBackgroundColor, m_BackgroundColor);
}

QColor BackgroundAttri::GetColor()
{
    return m_BackgroundColor;
}

void BackgroundAttri::SetImageCheckState(bool state)
{
    m_pChkIsImage->setChecked(state);
    IsUseImage(state);
}

bool BackgroundAttri::GetImageCheckState()
{
    return m_pChkIsImage->isChecked();
}

void BackgroundAttri::SetImage(const QString &path)
{
    m_pLineEditImagePath->setText(path);
}

QString BackgroundAttri::GetImage() const
{
    return m_pLineEditImagePath->text();
}

void BackgroundAttri::IsUseImage(int state)
{
    if(state > 0)
    {
        m_pLineEditImagePath->setEnabled(true);
        m_pBtnSelectImagePath->setEnabled(true);
    }
    else
    {
        m_pLineEditImagePath->setEnabled(false);
        m_pBtnSelectImagePath->setEnabled(false);
        m_pLineEditImagePath->clear();
        emit ImageChanged(tr(""));
    }
    emit ImageCheckStateChanged(m_pChkIsImage->isChecked());
}

void BackgroundAttri::ColorChanged()
{
    if(TBase::ChooseColor(m_BackgroundColor))
    {
        TBase::ColorLabel(m_pLblBackgroundColor, m_BackgroundColor);
    }
    emit BackgroundColorChanged(m_BackgroundColor);
}

void BackgroundAttri::SelectImagePath()
{
    QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    QString strPath = QFileDialog::getOpenFileName(NULL,tr("选择图片路径"),strUser,
                                                   tr("Images(*.png *.jpg *.jpeg *.bmp);;(*.png);;(*.jpg);;(*.jpeg);;(*.bmp)"));
    if(strPath.isEmpty())
        return;

    if(!strPath.contains(strUser))
    {
        QMessageBox msg(QMessageBox::Warning,tr("提示"),tr("请选择“用户文件夹”下的图片"),
                        QMessageBox::StandardButton::Yes,this);
        msg.exec();
        return;
    }

    QString str = tr("用户文件夹");
    int index = strPath.indexOf(str,0,Qt::CaseInsensitive);
    QString strRelativePath = ":" + strPath.right(strPath.length() - index - str.length());

    m_pLineEditImagePath->clear();
    m_pLineEditImagePath->setText(strRelativePath);
    emit ImageChanged(strRelativePath);
}

//----------------------end--------------------------------------


//-----------------------可用属性---------------------------------
EnableAttri::EnableAttri(QWidget *parent) : QWidget(parent)
{
    m_bFlag = false;
    m_strSelectVarType = "";

    QVBoxLayout *pEnableAttriLayout = new QVBoxLayout();
    this->setLayout(pEnableAttriLayout);
    this->setParent(parent);

    QGroupBox *pExpressGroup = new QGroupBox(tr("表达式"));
    pEnableAttriLayout->addWidget(pExpressGroup);
    QHBoxLayout *pExpressLayout = new QHBoxLayout();
    pExpressGroup->setLayout(pExpressLayout);
    m_pTextEditExpre = TBase::TextEdit(tr("Text"));
    m_pTextEditExpre->setAcceptRichText(false);
    m_pTextEditExpre->installEventFilter(this);
    m_pTextEditExpre->setMaximumHeight(60);
    pExpressLayout->addWidget(m_pTextEditExpre,0,Qt::AlignTop);

    m_pBtnSelectVarExt = TBase::Button(tr("?"),tr("BtnSelectVarExt"));
    pExpressLayout->addWidget(m_pBtnSelectVarExt,0,Qt::AlignTop);
    connect(m_pBtnSelectVarExt,&QPushButton::clicked,this,&EnableAttri::SelectVarExt);

    m_pBtnGroup = new QButtonGroup();
    m_pRdoEnable = new QRadioButton(tr(""));
    m_pRdoNotEnable = new QRadioButton(tr(""));

    m_pBtnGroup->addButton(m_pRdoNotEnable,0);
    m_pBtnGroup->addButton(m_pRdoEnable,1);
    m_pBtnGroup->button(1)->setChecked(true);
    connect(m_pBtnGroup,SIGNAL(buttonClicked(int)),this,SIGNAL(EnableStateChanged(int)));

    QGroupBox *pBtnEnableGroup = new QGroupBox(tr("当表达式非零时"));
    pEnableAttriLayout->addWidget(pBtnEnableGroup);
    pEnableAttriLayout->addStretch();
    QHBoxLayout *pBtnEnableLayout = new QHBoxLayout();
    pBtnEnableGroup->setLayout(pBtnEnableLayout);
    pBtnEnableLayout->addWidget(m_pRdoEnable);
    pBtnEnableLayout->addWidget(m_pRdoNotEnable);
}

void EnableAttri::SetButtonText(const QString &type)
{
    m_pRdoEnable->setText(tr("%1可用").arg(type));
    m_pRdoNotEnable->setText(tr("%1不可用").arg(type));
}

void EnableAttri::SetSelectVarType(const QString &type)
{
    m_strSelectVarType = type;
}

void EnableAttri::SetEnableExpre(const QString &text)
{
    m_pTextEditExpre->setPlainText(text);
}

QString EnableAttri::GetEnableExpre() const
{
    QString str = m_pTextEditExpre->toPlainText();
    str.remove(tr("\n"));
    return str;
}

void EnableAttri::SetEnableState(int id)
{
    m_pBtnGroup->button(id)->setChecked(true);
}

int EnableAttri::GetEnableState()
{
    return m_pBtnGroup->checkedId();
}

bool EnableAttri::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_pTextEditExpre)
    {
        if(event->type() == QEvent::FocusIn)
        {
            m_bFlag = true;
        }
        else if(event->type() == QEvent::FocusOut)
        {
            if(m_bFlag)
            {
                m_bFlag = false;
                emit SendTextFinfished(GetEnableExpre());
                return true;
            }
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Enter)
            {
                emit SendTextFinfished(GetEnableExpre());
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void EnableAttri::SelectVarExt()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(m_strSelectVarType);
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name)
    {
        //在输入框的光标处插入字符
        m_pTextEditExpre->textCursor().insertText(name);
        emit SendTextFinfished(GetEnableExpre());
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

//----------------------end--------------------------------------


//----------------------变量选择----------------------------------
VarSelectedAttri::VarSelectedAttri(QWidget *parent, bool isappend)
    : QWidget(parent)
{
    m_VarType = -1;
    m_strVarName = "";
    m_strSelectVarType = "";
    m_bIsRealtimeVar = false;
    m_bIsHWSwitch = false;
    m_bAppend = isappend;
    this->setContentsMargins(-9,-9,-9,-9);

    QHBoxLayout *pHLayout = new QHBoxLayout();
    this->setLayout(pHLayout);
    this->setParent(parent);

    m_pLineEditVar = TBase::LineEdit(tr("VarName"));
    pHLayout->addWidget(m_pLineEditVar);
    connect(m_pLineEditVar,&QLineEdit::editingFinished,this,&VarSelectedAttri::VarChangedExt);

    m_pBtnSelectVar = TBase::Button(tr("?"),tr(""));
    pHLayout->addWidget(m_pBtnSelectVar);
    connect(m_pBtnSelectVar,&QPushButton::clicked,this,&VarSelectedAttri::SelectVar);
}

void VarSelectedAttri::SetSelectVarType(const QString &type)
{
    m_strSelectVarType = type;
}

void VarSelectedAttri::SetIsRealtimeVar(bool visible)
{
    m_bIsRealtimeVar = visible;
}

void VarSelectedAttri::IsHWSwitch(bool b)
{
    m_bIsHWSwitch = b;
}

void VarSelectedAttri::SetVarName(const QString &text)
{
    m_strVarName = text;
    m_pLineEditVar->setText(text);
}

void VarSelectedAttri::SetVarType(char type)
{
    m_VarType = type;
}

QString VarSelectedAttri::GetVarName() const
{
    return m_strVarName;
}

char VarSelectedAttri::GetVarType() const
{
    return m_VarType;
}

void VarSelectedAttri::SelectVar()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(m_strSelectVarType);
    pDSV->SetIsRealtimeVar(m_bIsRealtimeVar);
    pDSV->IsHWSwitch(m_bIsHWSwitch);
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name,const QString type)
    {
        QString text = name;
        if(m_bAppend)
        {
            //获取当前光标位置
            int pos = m_pLineEditVar->cursorPosition();
            //在输入框的光标处插入字符
            text = m_pLineEditVar->text().insert(pos,name);
        }
        m_strVarName = name;
        m_VarType = type.toShort();
        m_pLineEditVar->setText(text);
        m_pLineEditVar->editingFinished();
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

void VarSelectedAttri::VarChangedExt()
{
    QString str = tr("%1,%2").arg(m_pLineEditVar->text()).arg(QString::number(m_VarType));
    emit SendTextFinfished(str);
}
//----------------------end--------------------------------------

//----------------------表达式选择---------------------------------
ExpreSelectedAttri::ExpreSelectedAttri(QWidget *parent) : QWidget(parent)
{
    m_bFlag = false;
    m_strSelectVarType = "";

    QVBoxLayout *pEnableAttriLayout = new QVBoxLayout();
    this->setLayout(pEnableAttriLayout);
    this->setParent(parent);
    this->setContentsMargins(-9,-9,-9,-9);

    pEnableAttriLayout->addWidget(new QLabel(tr("表达式")));
    QHBoxLayout *pExpressLayout = new QHBoxLayout();
    m_pTextEditExpre = TBase::TextEdit(tr("Text"));
    m_pTextEditExpre->setAcceptRichText(false);
    m_pTextEditExpre->installEventFilter(this);
    m_pTextEditExpre->setMaximumHeight(60);
    pExpressLayout->addWidget(m_pTextEditExpre,0,Qt::AlignTop);

    m_pBtnSelectVarExt = TBase::Button(tr("?"),tr("BtnSelectVarExt"));
    pExpressLayout->addWidget(m_pBtnSelectVarExt,0,Qt::AlignTop);
    pEnableAttriLayout->addLayout(pExpressLayout);

    connect(m_pBtnSelectVarExt,&QPushButton::clicked,this,&ExpreSelectedAttri::SelectVarExt);
}

void ExpreSelectedAttri::SetSelectVarType(const QString &type)
{
    m_strSelectVarType = type;
}

void ExpreSelectedAttri::SetExpre(const QString &text)
{
    m_pTextEditExpre->setPlainText(text);
}

QString ExpreSelectedAttri::GetExpre() const
{
    QString str = m_pTextEditExpre->toPlainText();
    str.remove(tr("\n"));
    return str;
}

void ExpreSelectedAttri::SelectVarExt()
{
    DialogSelectVar *pDSV = new DialogSelectVar(this);
    pDSV->SetSelectVarType(m_strSelectVarType);
    connect(pDSV,&DialogSelectVar::SendVar,[=] (const QString name)
    {
        //在输入框的光标处插入字符
        m_pTextEditExpre->textCursor().insertText(name);
        emit SendTextFinfished(GetExpre());
    });
    //对话框模态显示,主窗口不允许操作
    pDSV->setModal(true);
    pDSV->show();
}

bool ExpreSelectedAttri::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_pTextEditExpre)
    {
        if(event->type() == QEvent::FocusIn)
        {
            m_bFlag = true;
        }
        else if(event->type() == QEvent::FocusOut)
        {
            if(m_bFlag)
            {
                m_bFlag = false;
                emit SendTextFinfished(GetExpre());
                return true;
            }
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Enter)
            {
                emit SendTextFinfished(GetExpre());
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}
//----------------------------end------------------------------

//-------------------------------JS-----------------------------------------------
JsTacticAttri::JsTacticAttri(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *pERPLayout = new QGridLayout();
    this->setLayout(pERPLayout);
    this->setParent(parent);

    m_pChkIsUseJsTactic  = TBase::CheckBox(tr("执行运行策略块"),tr("JsTactic"));

    m_pBtnNewJsTactic = TBase::Button(tr("新增"),tr(""),40,22);
    connect(m_pBtnNewJsTactic,&QPushButton::clicked,this,&JsTacticAttri::NewJsTactic);

    m_pCmbJsTactic = TBase::ComboBox(tr("CmbJsTactic"));
    connect(m_pCmbJsTactic,&QComboBox::currentTextChanged,this,&JsTacticAttri::JsTacticChanged);

    pERPLayout->addWidget(m_pChkIsUseJsTactic,0,0,Qt::AlignLeft);
    pERPLayout->addWidget(m_pBtnNewJsTactic,0,1,Qt::AlignRight);
    pERPLayout->addWidget(m_pCmbJsTactic,1,0,1,2);
    pERPLayout->setAlignment(Qt::AlignTop);
}

void JsTacticAttri::SetJsTactic(const QString &filename)
{
    QStringList jsList = GetCurJSFile();
    jsList.insert(0,"");
    m_pCmbJsTactic->addItems(jsList);
    if(!filename.isEmpty())
    {
        if(jsList.contains(filename))
        {
            m_pCmbJsTactic->setCurrentText(filename);
            m_pChkIsUseJsTactic->setChecked(true);
        }
    }

    m_pCmbJsTactic->setEnabled(m_pChkIsUseJsTactic->isChecked());
    m_pBtnNewJsTactic->setEnabled(m_pChkIsUseJsTactic->isChecked());
    connect(m_pChkIsUseJsTactic,SIGNAL(stateChanged(int)),this,SLOT(CheckStateChanged(int)));
}

QString JsTacticAttri::GetJsTactic() const
{
    return m_pCmbJsTactic->currentText();
}

void JsTacticAttri::NewJsTactic()
{
    DialogNewExcuteJs *pDNE = new DialogNewExcuteJs(this);
    connect(pDNE,&DialogNewExcuteJs::signalSendNewJs,[=] (const QString text)
    {
        if(text.isEmpty())
            return;
        m_pCmbJsTactic->addItem(text);
        m_pCmbJsTactic->setCurrentText(text);
    });
    //对话框模态显示,主窗口不允许操作
    pDNE->setModal(true);
    pDNE->show();
}

void JsTacticAttri::CheckStateChanged(int state)
{
    Q_UNUSED(state)
    bool bCheckState = m_pChkIsUseJsTactic->isChecked();

    m_pCmbJsTactic->setEnabled(bCheckState);
    m_pBtnNewJsTactic->setEnabled(bCheckState);

    if(bCheckState)
    {
        QStringList jsList = GetCurJSFile();
        jsList.insert(0,"");
        m_pCmbJsTactic->clear();
        m_pCmbJsTactic->addItems(jsList);
    }
    else
    {
        m_pCmbJsTactic->setCurrentIndex(0);
    }
}

QStringList JsTacticAttri::GetCurJSFile()
{
    QStringList jsList;
    jsList.clear();

    QString strJSDir = QApplication::applicationDirPath() + tr("/脚本");
    QDir dir(strJSDir);
    if(!dir.exists())
        dir.mkdir(strJSDir);

    QStringList filters;
    filters<< "*.js";
    QFileInfoList list = dir.entryInfoList(filters);
    for(auto var:list)
    {
        jsList.append(var.fileName());
    }
    return jsList;
}
//----------------------------end-------------------------------------------


//--------------------------颜色选择-----------------------------
ColorSelect::ColorSelect(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *pHLayout = new QHBoxLayout();
    this->setLayout(pHLayout);
    this->setParent(parent);
    this->setContentsMargins(-8,-8,-8,-8);
    m_pLblText = TBase::Label(tr("颜色"),tr(""),40,22);
    m_pLblColor = TBase::Label(tr(""),tr(""), 30, 22);
    TBase::ColorLabel(m_pLblColor, Qt::black);
    m_pBtnSelectColor = TBase::Button(tr("浏览"), tr(""),40,22);
    connect(m_pBtnSelectColor,SIGNAL(clicked()), this, SLOT(SelectColor()));

    pHLayout->addWidget(m_pLblText);
    pHLayout->addWidget(m_pLblColor);
    pHLayout->addWidget(m_pBtnSelectColor);
    pHLayout->addStretch();
}

void ColorSelect::SetColor(QColor color)
{
    m_Color = color;
    TBase::ColorLabel(m_pLblColor, m_Color);
}

QColor ColorSelect::GetColor()
{
    return m_Color;
}

void ColorSelect::SelectColor()
{
    if(TBase::ChooseColor(m_Color))
    {
        TBase::ColorLabel(m_pLblColor, m_Color);
    }
    emit ColorChanged(m_Color);
}
//----------------------------end-------------------------------------------

//----------------------------置顶置底---------------------------
TopBottom::TopBottom(QWidget *parent)
    : QWidget(parent)
{
    m_ZPlace = 0;
    QHBoxLayout *pHLayout = new QHBoxLayout();
    this->setLayout(pHLayout);
    this->setContentsMargins(-8,-8,-8,-8);
    m_pChkTop = TBase::CheckBox(tr("置顶"),"Top");
    m_pChkBottom = TBase::CheckBox(tr("置底"),"Bottom");
    pHLayout->addWidget(m_pChkTop);
    pHLayout->addWidget(m_pChkBottom);
}

void TopBottom::SetZPlace(int value)
{
    m_ZPlace = value;

    if(m_ZPlace == 0)
    {
        m_pChkTop->setChecked(false);
        m_pChkBottom->setChecked(false);
    }
    else if(m_ZPlace == 2000)
    {
        m_pChkTop->setChecked(true);
        m_pChkBottom->setChecked(false);
    }
    else if(m_ZPlace == -2000)
    {
        m_pChkTop->setChecked(false);
        m_pChkBottom->setChecked(true);
    }

    connect(m_pChkTop,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);
    connect(m_pChkBottom,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);
}

int TopBottom::GetZPlace()
{
    return m_ZPlace;
}

void TopBottom::PlaceChange(int state)
{
    QCheckBox *chk = qobject_cast<QCheckBox*>(sender());

    disconnect(m_pChkTop,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);
    disconnect(m_pChkBottom,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);

    if(chk->objectName() == "Top")
    {
        m_pChkBottom->setChecked(false);
        m_pChkTop->setChecked(state);
    }
    else if(chk->objectName() == "Bottom")
    {
        m_pChkBottom->setChecked(state);
        m_pChkTop->setChecked(false);
    }
    connect(m_pChkTop,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);
    connect(m_pChkBottom,&QCheckBox::stateChanged,this,&TopBottom::PlaceChange);

    if((!m_pChkTop->isChecked()) && (!m_pChkBottom->isChecked()))
        m_ZPlace = 0;
    else if(m_pChkTop->isChecked() && (!m_pChkBottom->isChecked()))
        m_ZPlace = 2000;
    else if((!m_pChkTop->isChecked()) && m_pChkBottom->isChecked())
        m_ZPlace = -2000;

    emit ZPlaceChanged(m_ZPlace);
}
//----------------------------end------------------------------


