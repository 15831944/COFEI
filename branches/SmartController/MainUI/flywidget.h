/************************************
* @brief 摘要  悬浮窗体
* @author sunny
* @date 2017/06/01
************************************/
#ifndef FLYWIDGET_H
#define FLYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QBitmap>
#include <QTimer>
#include "pushbutton.h"
class FlyWidget : public QWidget
{
    Q_OBJECT
private:
    QWidget* pe;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QPoint m_CurrentPos;
    QPoint m_OldPos;
    QPixmap m_Pixmap;
    QPixmap m_PixmapChange;
    bool m_isClicked;
    PushButton *m_pMainUiBtn;
    PushButton *m_pDebugUiBtn;
    PushButton *m_pMonitorUiBtn;
    PushButton *m_pShowUiBtn;
    PushButton *m_pConfigUiBtn;
    bool m_isMove;
    bool m_isEnter;
    QTimer * m_pTimer;
public:
    QMenu* m_pSettingMenu;
public:
    explicit FlyWidget(QWidget *parent = 0);
    void InitData();
    void InitUI();
    void InitConnect();
    void SetMenu(QMenu* menu);
    void CreateBtn();
signals:
    void DoubleClickedSignal();
signals:

public slots:
    void TimerUpdate();
    void SwitchAuthor(const QString&);
};

#endif // FLYWIDGET_H
