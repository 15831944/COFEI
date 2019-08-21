#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>
#include <QMouseEvent>
class Lineedit : public QLineEdit
{
    Q_OBJECT
public:
   explicit Lineedit(QWidget *parent = 0);
    ~Lineedit();
protected:
    void mousePressEvent(QMouseEvent *event);
signals:
    void clicked();
};

#endif // LINEEDIT_H
