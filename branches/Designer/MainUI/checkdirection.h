#ifndef CHECKDIRECTION_H
#define CHECKDIRECTION_H

#include <QObject>
class CheckDirection : public QObject
{
    Q_OBJECT
public:
    explicit CheckDirection(QObject *parent = 0);

signals:

public slots:
    bool Direction_judgement();
public:
    char GetBitValue(QString IOstr);
public:
   bool  m_check_over;


};

#endif // CHECKDIRECTION_H
