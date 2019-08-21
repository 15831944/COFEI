#include "DialogNewProject.h"

DialogNewProject::DialogNewProject(QWidget *parent) : QDialog(parent)
{
    mSave =false;
    QLabel *mLabelNameShow = TBase::Label(tr("名称"), tr("mLabelNameShow"));
    m_LineEdit = TBase::LineEdit(tr("m_LineEdit"), 120, 24);
    m_LineEdit->installEventFilter(this);
    QLabel *mLabelCanvasShow = TBase::Label(tr("屏幕尺寸"), tr("mLabelCanvasShow"));
    m_EditX = TBase::LineEdit(tr("m_EditX"), 80, 24);
    QRegExp rx("^([1-9][0-9]{3})$");
    QRegExpValidator *pReg = new QRegExpValidator(rx);
    m_EditX->setValidator(pReg);
    QLabel *mLabelPoint = TBase::Label(tr("*"), tr("mLabelPoint"));
    m_EditY = TBase::LineEdit(tr("m_EditY"), 80, 24);
    m_EditY->setValidator(pReg);
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
    mLayoutName->addSpacing(70);
    QHBoxLayout *mLayoutRect = new QHBoxLayout();
    mLayoutRect->addWidget(mLabelCanvasShow);
    mLayoutRect->addWidget(m_EditX);
    mLayoutRect->addWidget(mLabelPoint);
    mLayoutRect->addWidget(m_EditY);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new  QVBoxLayout(this);
    mLayoutMain->setSpacing(12);
    mLayoutMain->addLayout(mLayoutName);
    mLayoutMain->addLayout(mLayoutRect);
    mLayoutMain->addLayout(mLayoutButton);
    mLayoutMain->addSpacing(12);
    mLayoutMain->addWidget(m_LabelNoteTip);

    // 设置默认
    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("新建项目"));
    setFixedSize(sizeHint());
    m_LineEdit->setFocus();
}

DialogNewProject::~DialogNewProject()
{

}

void DialogNewProject::Apply()
{
    if(GetName().isEmpty())
    {
        m_LabelNoteTip->show();
        m_LabelNoteTip->setText("项目名称不能为空!");
    }
    else if(m_NameList.contains(GetName()))
    {
        m_LabelNoteTip->show();
        m_LabelNoteTip->setText("项目名称不能重复!");
    }
    else
    {
        m_LabelNoteTip->hide();
        mSave = true;
        close();
    }
}

bool DialogNewProject::eventFilter(QObject *watched, QEvent *event)
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

QString DialogNewProject::GetName() const
{
    return m_LineEdit->text();
}
ushort DialogNewProject::GetWidth() const
{
    return m_EditX->text().toUShort();
}

ushort DialogNewProject::GetHeight() const
{
    return m_EditY->text().toUShort();
}

void DialogNewProject::SetWidth(ushort ut)
{
    m_EditX->setText(QString::number(ut));
}

void DialogNewProject::SetHeight(ushort ut)
{
    m_EditY->setText(QString::number(ut));
}

void DialogNewProject::SetNameList(const QList<QString> &list)
{
    m_NameList = list;
}
