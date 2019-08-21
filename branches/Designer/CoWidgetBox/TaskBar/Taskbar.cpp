#include "Taskbar.h"
#include "TaskbarScheme.h"
#include "Taskbox.h"
#include "Taskgroup.h"

CoTaskBar::CoTaskBar(QWidget *parent) :
  QWidget(parent)
{
  myScheme = CoTaskBarScheme::defaultScheme();

  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

  QVBoxLayout *vbl = new QVBoxLayout();
  vbl->setMargin(8);
  vbl->setSpacing(8);
  setLayout(vbl);
}

CoTaskBar::~CoTaskBar()
{
}

void CoTaskBar::setScheme(CoTaskBarScheme *scheme)
{
  if (scheme) {
    myScheme = scheme;

    // set scheme for children
    QObjectList list(children());
    foreach(QObject *obj, list) {
      if (dynamic_cast<CoTaskBox*>(obj)) {
        ((CoTaskBox*)obj)->setScheme(scheme);
        continue;
      }

      if (dynamic_cast<CoTaskGroup*>(obj)) {
        ((CoTaskGroup*)obj)->setScheme(scheme);
        continue;
      }
    }

    update();
  }
}

void CoTaskBar::paintEvent ( QPaintEvent * event )
{
    Q_UNUSED(event);
  QPainter p(this);

  //p.setOpacity(0.5);
  p.fillRect(rect(), myScheme->panelBackground);
}

void CoTaskBar::addWidget(QWidget *w)
{
  if (w)
    layout()->addWidget(w);
}

void CoTaskBar::addStretch(int s)
{
  ((QVBoxLayout*)layout())->addStretch(s);
}

