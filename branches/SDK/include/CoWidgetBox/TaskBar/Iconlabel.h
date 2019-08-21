#ifndef COICONLABEL_H
#define COICONLABEL_H

#include <QtGui>
#include <QWidget>
#include <CoBase/CoAction/Action.h>
#include <QApplication>

struct CoIconLabelScheme;

class CoIconLabel : public QWidget
{
  Q_OBJECT

public:
  CoIconLabel(CoAction *new_action, QWidget *parent = 0);
  CoIconLabel(const QString &title, QWidget *parent = 0);
  CoIconLabel(const QIcon &icon, const QString &title, QWidget *parent = 0);
  virtual ~CoIconLabel();

  inline CoAction* Action() { return myAction; }

  void setColors(const QColor &color, const QColor &colorOver, const QColor &colorOff);
  void setFont(const QFont &font);
  void setFocusPen(const QPen &pen);

  void setSchemePointer(CoIconLabelScheme **pointer);
  void SetActionType(int type);

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

signals:
  void pressed();
  void released();
  void clicked();
  void activated();
  void contextMenu();

protected slots:
  void actionChanged();

protected:
  void init();

  virtual void paintEvent ( QPaintEvent * event );
  virtual void enterEvent ( QEvent * event );
  virtual void leaveEvent ( QEvent * event );

  virtual void mousePressEvent ( QMouseEvent * event );
  virtual void mouseReleaseEvent ( QMouseEvent * event );
  virtual void keyPressEvent ( QKeyEvent * event );

  virtual void actionEvent ( QActionEvent * event );
  CoAction *myAction;
  QColor myColor, myColorOver, myColorDisabled;
  QFont myFont;
  QPen myPen;

  CoIconLabelScheme **mySchemePointer;

  bool m_over, m_pressed;

  bool m_changeCursorOver, m_underlineOver;
};

#endif // COICONLABEL_H
