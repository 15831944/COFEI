#ifndef COTASKGROUP_H
#define COTASKGROUP_H

#include <QtGui>
#include <QFrame>
#include <QBoxLayout>

class CoTaskBarScheme;
struct CoIconLabelScheme;

class CoIconLabel;

class CoTaskGroup : public QFrame
{
  Q_OBJECT

public:
  CoTaskGroup(QWidget *parent, bool hasHeader = false);
  virtual ~CoTaskGroup();

  void setScheme(CoTaskBarScheme *scheme);

  inline QBoxLayout* groupLayout()
  {
    return (QBoxLayout*)layout();
  }

  void addIconLabel(CoIconLabel *label, bool addToLayout = true);

  QPixmap transparentRender();

protected:
  virtual void paintEvent ( QPaintEvent * event );
  virtual void keyPressEvent ( QKeyEvent * event );
  virtual void keyReleaseEvent ( QKeyEvent * event );

  CoTaskBarScheme *myScheme;
  CoIconLabelScheme *myLabelScheme;

  bool myHasHeader;
};

#endif // COTASKGROUP_H
