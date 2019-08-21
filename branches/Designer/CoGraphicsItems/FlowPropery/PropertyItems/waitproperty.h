#ifndef WAITPROPERTY_H
#define WAITPROPERTY_H

#include "../libproperybase.h"

class WAITProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString Note READ getNote WRITE setNote)

    Q_PROPERTY(QString information READ getInformation WRITE setInformation)

    Q_PROPERTY(QString warning READ getWarning WRITE setWarning)

    Q_PROPERTY(int timeout READ getTimeOut WRITE setTimeout)
public:
    WAITProperty();

    bool SaveXml(QDomDocument *doc);

    QString getWarning();
    void setWarning(QString);

    QString getInformation();
    void setInformation(QString);

    int getTimeOut();
    void setTimeout(int);
private:
    QString information;
    QString warning;
    int timeout;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) ;
};

#endif // WAITPROPERTY_H
