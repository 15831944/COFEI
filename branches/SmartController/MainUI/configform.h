/************************************
* @brief 摘要 参数界面
* @author sunny
* @date 2017/06/01
************************************/
#ifndef CONFIGFORM_H
#define CONFIGFORM_H

#include <QWidget>
#include <Comm.h>
#include <QLineEdit>
class ConfigForm : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigForm(QVector<UIXML_STRU> vec,QWidget *parent = 0);
    ~ConfigForm();
private:
    void CreateUI();
    void SetEditText();
private:
    QVector<UIXML_STRU> m_uivec;
public:
    QMap<QString,QLineEdit*> m_lineEditMap;
signals:

public slots:
    void SaveEditText();

    //add by sylar.liu
    void ShowMessage(const QString str);
    void ChmodUsr();
};

#endif // CONFIGFORM_H
