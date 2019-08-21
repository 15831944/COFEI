#include "DialogNewScene.h"

DialogNewScene::DialogNewScene(QWidget *parent) : QDialog(parent)
{
    mSave = false;

    QLabel *mLabelNameShow = TBase::Label(tr("名称"), tr("mLabelNameShow"));
    m_LineEdit = TBase::LineEdit(tr("m_LineEdit"), 120, 24);
    m_LineEdit->installEventFilter(this);
    QLabel *mLabelBackColorShow = TBase::Label(tr("背景"), tr("mLabelBackColorShow"));
    m_LabelBackColor = TBase::Label(tr(""), tr("m_LabelBackColor"), 54, 24);
    m_BackColor = Qt::black;
    TBase::ColorLabel(m_LabelBackColor, m_BackColor);
    QPushButton *mButtonColor = TBase::Button(tr("浏览"), tr("mButtonColor"), 54, 24);
    connect(mButtonColor, SIGNAL(clicked()), this, SLOT(ChooseBackColor()));

    QPushButton *mButtonClose = TBase::Button(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *mButtonOk = TBase::Button(tr("确定"), tr("mButtonOk"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(Apply()));

    m_LabelNoteTip = TBase::Label(tr(" "), tr("m_LabelNoteTip"));
    m_LabelNoteTip->setAlignment(Qt::AlignCenter);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    m_LabelNoteTip->setPalette(pa);
    m_LabelNoteTip->hide();
    QHBoxLayout *mLayoutName = new QHBoxLayout();
    mLayoutName->addWidget(mLabelNameShow);
    mLayoutName->addWidget(m_LineEdit);
    QHBoxLayout *mLayoutBackClr = new QHBoxLayout();
    mLayoutBackClr->addWidget(mLabelBackColorShow);
    mLayoutBackClr->addWidget(m_LabelBackColor);
    mLayoutBackClr->addWidget(mButtonColor);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new  QVBoxLayout(this);
    mLayoutMain->setSpacing(12);
    mLayoutMain->addLayout(mLayoutName);
    mLayoutMain->addLayout(mLayoutBackClr);
    mLayoutMain->addLayout(mLayoutButton); 
    mLayoutMain->addSpacing(12);
    mLayoutMain->addWidget(m_LabelNoteTip);

    // 设置默认
    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("新建画布"));
    setFixedSize(sizeHint());
    m_LineEdit->setFocus();
}

void DialogNewScene::ChooseBackColor()
{
    if(TBase::ChooseColor(m_BackColor, this))
    {
        TBase::ColorLabel(m_LabelBackColor, m_BackColor);
    }
}

void DialogNewScene::Apply()
{
    if(GetName().isEmpty())
    {
        m_LabelNoteTip->show();
        m_LabelNoteTip->setText("画布名称不能为空!");
    }
    else if(m_NameList.contains(GetName()))
    {
        m_LabelNoteTip->show();
        m_LabelNoteTip->setText("画布名称不能重复!");
    }
    else
    {
        m_LabelNoteTip->hide();
        mSave = true;
        close();
    }
}
bool DialogNewScene::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_LineEdit)
    {
        if (event->type()==QEvent::FocusIn)
        {
            m_LabelNoteTip->hide();
        }
    }
    return QWidget::eventFilter(watched,event);
}
void DialogNewScene::SetTitle(const QString &Title)
{
    setWindowTitle(Title);
}

bool DialogNewScene::SetName(const QString &Name)
{
    m_LineEdit->setText(Name);
    m_LineEdit->setReadOnly(true);
    return true;
}

bool DialogNewScene::SetBackColor(const QColor &Color)
{
    m_BackColor = Color;
    TBase::ColorLabel(m_LabelBackColor, m_BackColor);

    return true;
}

QString DialogNewScene::GetName() const
{
    return m_LineEdit->text();
}

void DialogNewScene::SetNameList(const QList<QString> &list)
{
     m_NameList = list;
}

QColor DialogNewScene::GetBackColor() const
{
    return m_BackColor;
}

DialogNewScene::~DialogNewScene()
{}
