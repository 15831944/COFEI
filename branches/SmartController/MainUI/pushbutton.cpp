#include "pushbutton.h"
PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
    Rmouse_press = false;
    m_uc = 3;
    m_aligment = Qt::AlignCenter;
    m_ucSpacing = 0;
}
PushButton::~PushButton()
{
}
void PushButton::getIconPath(QString Icon_path)
{
    this->icon_path = Icon_path;
    setFixedSize(QPixmap(Icon_path).size());//设置按钮与图片一样的大小
}
void PushButton::setStatusImageNum(uchar uc)
{
    m_uc = uc;
}
void PushButton::enterEvent(QEvent *)
{
    if(m_uc >=3)
    {
        status = HOVER;
        update();
    }
}
void PushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
        emit pressed();
        if(m_uc >= 2)
        {
            status = PRESS;
            update();
        }
    }
    if(event->button() == Qt::RightButton)
    {
        Rmouse_press = !Rmouse_press;
        emit RClicked(Rmouse_press);
    }
}
void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //若点击鼠标左键响应
    if(mouse_press&&this->rect().contains(event->pos()))
    {
        mouse_press = false;
        emit released();
        if(m_uc >=3)
        {
            status = HOVER;
            update();
        }
        emit clicked();
    }
}
void PushButton::leaveEvent(QEvent *)
{
    if(mouse_press)
    {
        mouse_press = false;
        emit released();
    }
    status = NORMAL;
    update();
}
void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;
    switch(status)
    {
    case NORMAL:
        {
            pixmap.load(icon_path);
            break;
        }
    case HOVER:
        {
            pixmap.load(icon_path + QString("_hover"));
            break;
        }
    case PRESS:
        {
            pixmap.load(icon_path + QString("_press"));
            break;
        }
    default:
        pixmap.load(icon_path);
    }
    painter.drawPixmap(rect(), pixmap);
    QRect rect(this->rect());
    rect.setLeft(this->rect().left() +m_ucSpacing);
    rect.setBottom(this->rect().bottom() +3);
    painter.drawText(rect, m_aligment, this->text());
}
