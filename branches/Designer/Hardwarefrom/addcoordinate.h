#ifndef ADDCOORDINATE_H
#define ADDCOORDINATE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Comm.h>
typedef struct COORDINATESTUR
{
    QString coordname = "";
    int limtstart = 0;
    int limtend = 0;
    QString contain = "";
}COORDINATESTUR;
class AddCoordinate : public QWidget
{
    Q_OBJECT
public:
    explicit AddCoordinate(QWidget *parent = 0);
     void SetParameter(COORDINATESTUR corstru);
signals:
    void Submit(COORDINATESTUR);
private:
    void initUI();
    void MakeSure();
    COORDINATESTUR GetCoordinate();

private:
    QPushButton *p_OkBut;
    QPushButton *p_NoBut;
    QLineEdit *p_IdName;
    QSpinBox *p_limitStart;
    QSpinBox *p_limitEnd;
    QLineEdit *p_Contain;
public slots:
};

#endif // ADDCOORDINATE_H
