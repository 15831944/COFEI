#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter();

signals:

public slots:
    QString GetFileContent(QString);
    bool    CreateFile(QString,QString);
    int     GetAxisCurPos(const QString AxisId);
    int     GetAxisCurtq(const QString AxisId);
    int     GetAxisErrorCode(const QString AxisId);
    int     GetAxisFault(const QString AxisId);
    int     GetAxisReadySw(const QString AxisId);
    QString GetVarValueText(QString varname,QString vartype);
};

#endif // CONVERTER_H
