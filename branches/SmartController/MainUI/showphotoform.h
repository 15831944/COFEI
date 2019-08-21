#ifndef MYTESTFORM_H
#define MYTESTFORM_H

#include <QWidget>
#include <Comm.h>
class ShowPhotoForm : public QWidget
{
    Q_OBJECT
public:
    explicit ShowPhotoForm(QVector<UIXML_STRU> vec,QWidget *parent = 0);

signals:

public slots:
};

#endif // MYTESTFORM_H
