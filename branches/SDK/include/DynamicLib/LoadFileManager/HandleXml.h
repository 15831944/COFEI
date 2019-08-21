#ifndef HANDLEXML_H
#define HANDLEXML_H
#include <QObject>
#include <DycLibBase/XmlManager/xmlmanager.h>
#include <MyGlobal.h>
using namespace CoreGlobal;
class HandleXml : public QObject
{
    Q_OBJECT
public:
    explicit HandleXml(QObject *parent = 0);
    ~HandleXml();
    void FaultReset();
signals:
    void operateXmlsError(const QString str);
public slots:
    void AnalysXml();
private:
    template<typename T,typename T2>
    int GetByteNumber(T t1,T2 t2);
    template<typename T>
    void InitIOData(T t1,int inum);
    bool m_isAnalsUIXml;
public:
    bool DeleteDirectory(const QString &path);
    void SetAnalsUIXml(bool isAnals);
};

#endif // HANDLEXML_H
