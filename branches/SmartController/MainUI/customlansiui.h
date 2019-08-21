#ifndef CUSTOMLANSIUI_H
#define CUSTOMLANSIUI_H

#include <QWidget>
#include <QTimer>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
class CustomLanSiUI : public QWidget
{
    Q_OBJECT
public:
    explicit CustomLanSiUI(QWidget *parent = 0);
private:
    QTimer *m_pTimer;
    QFont m_font;
    uint m_CurCountA;
    uint m_CurCountB;
    QStringList m_Id;
    QMap<QString,QLineEdit*> m_lineEditMap;
    void CreateCusUI();
    void InitData();
    QLabel *CreateCusLabel(const QString );
    QLineEdit *CreateCusLineEdit(const QString);
    QPushButton *CreateCusBTN(const QString);
signals:

public slots:
    void TimerUpDateCusUI();
    void ReliefZero();
    void GetUIEnableSig(bool);
};

#endif // CUSTOMLANSIUI_H
