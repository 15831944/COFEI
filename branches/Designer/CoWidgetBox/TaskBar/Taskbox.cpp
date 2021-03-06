#include "Taskbox.h"
#include "Taskheader.h"
#include "Taskgroup.h"
#include "TaskbarScheme.h"
#include "Iconlabel.h"

CoTaskBox::CoTaskBox(const QString &title, bool expandable, QWidget *parent)
    : QFrame(parent)
{
    myHeader = new CoTaskHeader(QPixmap(), title, expandable, this);
    m_title = title;
    init();
}

CoTaskBox::CoTaskBox(const QPixmap &icon, const QString &title, bool expandable, QWidget *parent)
    : QFrame(parent)
{
    myHeader = new CoTaskHeader(icon, title, expandable, this);
    m_title = title;
    init();
}

void CoTaskBox::init()
{
    m_foldStep = 0;

    myScheme = CoTaskBarScheme::defaultScheme();

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->setMargin(0);
    vbl->setSpacing(0);
    setLayout(vbl);

    vbl->addWidget(myHeader);

    myGroup = new CoTaskGroup(this, true);
    vbl->addWidget(myGroup);

    myDummy = new QWidget(this);
    vbl->addWidget(myDummy);
    myDummy->hide();

    connect(myHeader, SIGNAL(activated()), this, SLOT(showHide()));
}

CoTaskBox::~CoTaskBox()
{

}

void CoTaskBox::setScheme(CoTaskBarScheme *pointer)
{
    myScheme = pointer;
    myHeader->setScheme(pointer);
    myGroup->setScheme(pointer);
    update();
}

QBoxLayout* CoTaskBox::groupLayout()
{
    return myGroup->groupLayout();
}

void CoTaskBox::addIconLabel(CoIconLabel *label, bool addToLayout)
{
    if (!label)
        return;

    myGroup->addIconLabel(label, addToLayout);
}

void CoTaskBox::addAction(CoAction *action, bool addToLayout)
{
    if (!action)
        return;

    myGroup->addIconLabel(new CoIconLabel(action, this), addToLayout);
}

QString CoTaskBox::GetTitle() const
{
    return m_title;
}

void CoTaskBox::showHide()
{
    if (m_foldStep)
        return;

    if (myGroup->isVisible())
    {
        m_foldPixmap = myGroup->transparentRender();
        //    m_foldPixmap = QPixmap::grabWidget(myGroup, myGroup->rect());

        m_tempHeight = m_fullHeight = myGroup->height();
        m_foldDelta = m_fullHeight / myScheme->groupFoldSteps;
        m_foldStep = myScheme->groupFoldSteps;
        m_foldDirection = -1;

        myGroup->hide();
        myDummy->setFixedSize(myGroup->size());
        myDummy->show();

        QTimer::singleShot(myScheme->groupFoldDelay, this, SLOT(processHide()));
    }
    else
    {
        m_foldStep = myScheme->groupFoldSteps;
        m_foldDirection = 1;
        m_tempHeight = 0;

        QTimer::singleShot(myScheme->groupFoldDelay, this, SLOT(processShow()));
    }

    myDummy->show();
}

void CoTaskBox::processHide()
{
    if (!--m_foldStep) {
        myDummy->setFixedHeight(0);
        myDummy->hide();
        setFixedHeight(myHeader->height());
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        return;
    }

    setUpdatesEnabled(false);

    m_tempHeight -= m_foldDelta;
    myDummy->setFixedHeight(m_tempHeight);
    setFixedHeight(myDummy->height()+myHeader->height());

    QTimer::singleShot(myScheme->groupFoldDelay, this, SLOT(processHide()));

    setUpdatesEnabled(true);
}

void CoTaskBox::processShow()
{
    if (!--m_foldStep) {
        myDummy->hide();
        m_foldPixmap = QPixmap();
        myGroup->show();
        setFixedHeight(m_fullHeight+myHeader->height());
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        setMaximumHeight(9999);
        setMinimumHeight(0);
        return;
    }

    setUpdatesEnabled(false);

    m_tempHeight += m_foldDelta;
    myDummy->setFixedHeight(m_tempHeight);
    setFixedHeight(myDummy->height()+myHeader->height());

    QTimer::singleShot(myScheme->groupFoldDelay, this, SLOT(processShow()));

    setUpdatesEnabled(true);
}

void CoTaskBox::paintEvent ( QPaintEvent * event )
{
    Q_UNUSED(event);
    QPainter p(this);

    if (myDummy->isVisible()) {
        if (myScheme->groupFoldThaw) {
            if (m_foldDirection < 0)
                p.setOpacity((double)m_foldStep / myScheme->groupFoldSteps);
            else
                p.setOpacity((double)(myScheme->groupFoldSteps-m_foldStep) / myScheme->groupFoldSteps);
        }

        switch (myScheme->groupFoldEffect)
        {
        case ShrunkFolding:
            p.drawPixmap(myDummy->pos(), m_foldPixmap.scaled(myDummy->size()) );
            break;

        case SlideFolding:
            p.drawPixmap(myDummy->pos(), m_foldPixmap,
                         QRect(0, m_foldPixmap.height()-myDummy->height(),
                               m_foldPixmap.width(), myDummy->width()
                               )  );
            break;

        default:
            p.drawPixmap(myDummy->pos(), m_foldPixmap);
        }

        return;
    }

}
