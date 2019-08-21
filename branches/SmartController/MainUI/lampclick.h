/************************************
* @brief 摘要 指示灯的点击事件
* @author sunny
* @date 2017/06/01
************************************/
#ifndef LAMPCLICK_H
#define LAMPCLICK_H

#include <QWidget>

class LampClick : public QWidget
{
    Q_OBJECT
public:
    explicit LampClick(QWidget *parent = 0);

signals:
    void ChangeArm(bool);
public slots:
    void Clicking()
    {
        m_isClicked = !m_isClicked;
       emit ChangeArm(m_isClicked);
    }

private:
    bool m_isClicked;
};

#endif // LAMPCLICK_H
