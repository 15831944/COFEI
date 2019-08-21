#include "Taskheader.h"
#include "TaskbarScheme.h"
#include "Iconlabel.h"
#include <QBoxLayout>
CoTaskHeader::CoTaskHeader(const QIcon &icon, const QString &title, bool expandable, QWidget *parent)
    : QFrame(parent),
      myExpandable(expandable),
      m_over(false),
      m_buttonOver(false),
      m_fold(true),
      m_opacity(0.1),
      myButton(0)
{
    myTitle = new CoIconLabel(icon, title, this);
    myTitle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    connect(myTitle, SIGNAL(activated()), this, SLOT(fold()));

    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->setMargin(2);
    setLayout(hbl);

    hbl->addWidget(myTitle);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    setScheme(CoTaskBarScheme::defaultScheme());
    myTitle->setSchemePointer(&myLabelScheme);


    if (myExpandable) {
        myButton = new QLabel(this);
        hbl->addWidget(myButton);
        myButton->installEventFilter(this);
        myButton->setFixedWidth(myScheme->headerButtonSize.width());
        changeIcons();
    }
}

CoTaskHeader::~CoTaskHeader()
{
}

bool CoTaskHeader::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (myExpandable)
            fold();
        return true;

    case QEvent::Enter:
        m_buttonOver = true;
        changeIcons();
        return true;

    case QEvent::Leave:
        m_buttonOver = false;
        changeIcons();
        return true;

    default:;
    }

    return QFrame::eventFilter(obj, event);
}

void CoTaskHeader::setScheme(CoTaskBarScheme *scheme){
    if (scheme) {
        myScheme = scheme;
        myLabelScheme = &(scheme->headerLabelScheme);

        if (myExpandable) {
            setCursor(myLabelScheme->cursorOver ? Qt::PointingHandCursor : cursor());
            changeIcons();
        }

        setFixedHeight(scheme->headerSize);

        update();
    }
}

void CoTaskHeader::paintEvent ( QPaintEvent * event )
{
    Q_UNUSED(event);
    QPainter p(this);

    if (myScheme->headerAnimation)
        p.setOpacity(m_opacity+0.7);

    p.setPen(m_over ? myScheme->headerBorderOver : myScheme->headerBorder);
    p.setBrush(m_over ? myScheme->headerBackgroundOver : myScheme->headerBackground);

    //  if (p.pen().style() == Qt::NoPen)
    //    p.drawRect(rect());
    //  else
    //    p.drawRect(rect().adjusted(0,0,-1,-1));

    myScheme->headerCorners.draw(&p, rect());
}

void CoTaskHeader::animate()
{
    if (!myScheme->headerAnimation)
        return;

    if (!isEnabled()) {
        m_opacity = 0.1;
        update();
        return;
    }

    if (m_over) {
        if (m_opacity >= 0.3) {
            m_opacity = 0.3;
            return;
        }
        m_opacity += 0.05;
    } else {
        if (m_opacity <= 0.1) {
            m_opacity = 0.1;
            return;
        }
        m_opacity = qMax(0.1, m_opacity-0.05);
    }

    QTimer::singleShot(100, this, SLOT(animate()));
    update();
}

void CoTaskHeader::enterEvent ( QEvent * /*event*/ )
{
    m_over = true;

    if (isEnabled())
        QTimer::singleShot(100, this, SLOT(animate()));

    update();
}

void CoTaskHeader::leaveEvent ( QEvent * /*event*/ )
{
    m_over = false;

    if (isEnabled())
        QTimer::singleShot(100, this, SLOT(animate()));

    update();
}

void CoTaskHeader::fold()
{
    if (myExpandable) {
        emit activated();

        m_fold = !m_fold;
        changeIcons();
    }
}

void CoTaskHeader::changeIcons()
{
    if (!myButton)
        return;

    if (m_buttonOver)
    {
        if (m_fold)
            myButton->setPixmap(myScheme->headerButtonFoldOver.pixmap(myScheme->headerButtonSize));
        else
            myButton->setPixmap(myScheme->headerButtonUnfoldOver.pixmap(myScheme->headerButtonSize));
    } else
    {
        if (m_fold)
            myButton->setPixmap(myScheme->headerButtonFold.pixmap(myScheme->headerButtonSize));
        else
            myButton->setPixmap(myScheme->headerButtonUnfold.pixmap(myScheme->headerButtonSize));
    }
}

void CoTaskHeader::mouseReleaseEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton) {
        emit activated();
    }
}

void CoTaskHeader::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
    case Qt::Key_Down:
    {
        QKeyEvent ke(QEvent::KeyPress, Qt::Key_Tab, 0);
        QApplication::sendEvent(this, &ke);
        return;
    }

    case Qt::Key_Up:
    {
        QKeyEvent ke(QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier);
        QApplication::sendEvent(this, &ke);
        return;
    }

    default:;
    }

    QWidget::keyPressEvent(event);
}

void CoTaskHeader::keyReleaseEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
    case Qt::Key_Down:
    {
        QKeyEvent ke(QEvent::KeyRelease, Qt::Key_Tab, 0);
        QApplication::sendEvent(this, &ke);
        return;
    }

    case Qt::Key_Up:
    {
        QKeyEvent ke(QEvent::KeyRelease, Qt::Key_Tab, Qt::ShiftModifier);
        QApplication::sendEvent(this, &ke);
        return;
    }

    default:;
    }

    QWidget::keyReleaseEvent(event);
}
