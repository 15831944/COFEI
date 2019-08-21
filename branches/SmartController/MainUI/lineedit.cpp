#include "lineedit.h"
Lineedit::Lineedit(QWidget *parent)
    :QLineEdit(parent)
{

}
Lineedit::~Lineedit()
{

}
void Lineedit::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QLineEdit::mousePressEvent(event);
}
