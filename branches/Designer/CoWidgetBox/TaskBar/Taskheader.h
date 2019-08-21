#ifndef COTASKHEADER_H
#define COTASKHEADER_H

#include <QtGui>
#include <QLabel>

class CoTaskBarScheme;
struct CoIconLabelScheme;
class CoIconLabel;

class CoTaskHeader : public QFrame
{
  Q_OBJECT

public:
  CoTaskHeader(const QIcon &icon, const QString &title, bool expandable, QWidget *parent = 0);
  virtual ~CoTaskHeader();

  void setScheme(CoTaskBarScheme *scheme);

signals:
  void activated();

public slots:
  void fold();

protected slots:
  void animate();

protected:
  virtual void paintEvent ( QPaintEvent * event );
  virtual void enterEvent ( QEvent * event );
  virtual void leaveEvent ( QEvent * event );
  virtual void mouseReleaseEvent ( QMouseEvent * event );
  virtual void keyPressEvent ( QKeyEvent * event );
  virtual void keyReleaseEvent ( QKeyEvent * event );

  bool eventFilter(QObject *obj, QEvent *event);

  void changeIcons();

  CoTaskBarScheme *myScheme;
  CoIconLabelScheme *myLabelScheme;

  bool myExpandable;
  bool m_over, m_buttonOver, m_fold;
  double m_opacity;

  CoIconLabel *myTitle;
  QLabel *myButton;
};

#endif // COTASKHEADER_H
