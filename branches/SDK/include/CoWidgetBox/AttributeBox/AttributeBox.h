#ifndef ATTRIBUTEBASEDLG_H
#define ATTRIBUTEBASEDLG_H

#include <QWidget>
#include <QGridLayout>
class CoAttributeBox : public QWidget
{
    Q_OBJECT
public:
    CoAttributeBox(QWidget *parent = 0);
    void AddWidget(QWidget*);
private:
    QWidget * m_widget;
    QGridLayout *m_pLayout;
signals:
    void TransferHideEvent(bool);
public slots:
    void HandleHideEvent(bool);
};

#endif // ATTRIBUTEBASEDLG_H
