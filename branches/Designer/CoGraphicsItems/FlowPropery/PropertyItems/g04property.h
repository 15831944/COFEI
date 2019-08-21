#ifndef G04PROPERTY_H
#define G04PROPERTY_H
#include "../libproperybase.h"


class G04Property:public LibProperyBase
{
    Q_OBJECT

//    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString pathID READ getPathID WRITE setPathID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QStringList Joint READ getJoint WRITE setJoint)
    Q_PROPERTY(int JointNum READ getJointNum )//WRITE setJointNum)

    Q_PROPERTY(TYPEPATH type READ getType WRITE setType)

    Q_PROPERTY(QStringList PathPoint READ getPathPoint WRITE setPathPoint)
    Q_PROPERTY(int PointNum READ getPointNum WRITE setPointNum)
    Q_PROPERTY(int MaxVel READ getMaxVel WRITE setMaxVel)


public:
    G04Property();
    enum TYPEPATH
    {
        LINE=1,
        ARC,
        CURVE
    };
    Q_ENUM(TYPEPATH)
    TYPEPATH type;
    QString pathID;
    static QStringList Joint;
    int JointNum;
    QStringList PathPoint;
    int PointNum;
    int MaxVel;
public:
    TYPEPATH getType();
    void setType(TYPEPATH);

    QString getPathID();
    void setPathID(QString);

    QStringList getJoint();
    void setJoint(QStringList);

    int getJointNum();
    void setJointNum(int);

    QStringList getPathPoint();
    void setPathPoint(QStringList);

    int getPointNum();
    void setPointNum(int);

    int getMaxVel();
    void setMaxVel(int val);

private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) ;

    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // G04PROPERTY_H
