#ifndef ADDTABEL_H
#define ADDTABEL_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>

class Addtabel : public QDialog
{
    Q_OBJECT
public:
    Addtabel(QVector<QString> str);
private:
    QLabel *m_label[9];
    QVector<QString> m_strvec;
    QLineEdit *m_lineedit[9];
    QPushButton *m_pok;
    QPushButton *m_pcleal;
public:
    void initUi();
    void sure();
    bool createmessage();
signals:
    void data(QStringList);
    //void update(QStringList);

};

#endif // ADDTABEL_H
