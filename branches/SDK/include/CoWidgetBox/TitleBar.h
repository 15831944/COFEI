#ifndef TITLEBAR_H
#define TITLEBAR_H
#include <QWidget>
class QLabel;
class QPushButton;

class  CoTitleBar : public QWidget
{
    Q_OBJECT
public:
    CoTitleBar(QWidget *parent = 0);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
signals:
    void SendHideEvent(bool);
private slots:
    void onClicked();
private:
    QPushButton *m_pHideBtn;
    QLabel *m_pTitleLabel;
    bool ishide;

};
#endif // TITLEBAR_H
