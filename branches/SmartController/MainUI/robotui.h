#ifndef ROBOTUI_H
#define ROBOTUI_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

class RobotUI : public QWidget
{
    Q_OBJECT
public:
    explicit RobotUI(QWidget *parent = 0);

signals:
private:
    QWidget *mwidget;
    int m_width;
    int m_height;
  //  QPushButton *but[];
private:
    void CreatUI();

public slots:
};

#endif // ROBOTUI_H
