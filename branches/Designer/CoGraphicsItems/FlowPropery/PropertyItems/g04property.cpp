#include "g04property.h"
#include <QMetaEnum>

QStringList G04Property::Joint={"X","Y","Z","R","",""};
G04Property::G04Property()
{
    ID="1";
    note="";
    commandString="G04";

    type=LINE;
    pathID="";

    JointNum=6;

    PathPoint.append(QStringList{"",""});
    PointNum=2;
    MaxVel = 0;
}

G04Property::TYPEPATH G04Property::getType()
{
    return type;
}

void G04Property::setType(G04Property::TYPEPATH val)
{
    type=val;
}

QString G04Property::getPathID()
{
    return pathID;
}

void G04Property::setPathID(QString val)
{
    pathID=val;
    sendval();
}

QStringList G04Property::getJoint()
{
    return Joint;
}

void G04Property::setJoint(QStringList val)
{
    Joint=val;
}

int G04Property::getJointNum()
{
    return JointNum;
}

void G04Property::setJointNum(int val)
{

    if(val<=1)
    {
        return;
    }
    JointNum=val;
    QStringList src(Joint);
    int count=src.count();
    count=JointNum-count;
    if(count>0)
    {
        for(int i=0;i<count;++i)
        {
            src.append("");
        }
    }
    else if(count<0)
    {
        count=-count;
        for(int i=0;i<count;++i)
        {
            src.removeLast();
        }
    }
    setJoint(src);
}

QStringList G04Property::getPathPoint()
{
    return PathPoint;
}

void G04Property::setPathPoint(QStringList val)
{
    PathPoint=val;
}

int G04Property::getPointNum()
{
    return PointNum;
}

void G04Property::setPointNum(int val)
{

    if(val<=1)
    {
        return;
    }
    PointNum=val;
    QStringList src(PathPoint);
    int count=src.count();
    count=PointNum-count;
    if(count>0)
    {
        for(int i=0;i<count;++i)
        {
            src.append("");
        }
    }
    else if(count<0)
    {
        count=-count;
        for(int i=0;i<count;++i)
        {
            src.removeLast();
        }
    }
    setPathPoint(src);
}

int G04Property::getMaxVel()
{
    return MaxVel;
}

void G04Property::setMaxVel(int val)
{
    MaxVel = val;
    sendval();
}

void G04Property::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("%1 %2 %3\n%4").arg(commandString).arg(ID).arg(pathID).arg(note);
    emit ValueChange(str);
}

void G04Property::copyPropertyChild(LibProperyBase *inPro)
{
    G04Property *in=qobject_cast<G04Property*>(inPro);
    ID =in->ID;
    note = in->note;

    type=in->type;
    pathID=in->pathID;
//    Joint=in->Joint;
//    JointNum=in->JointNum;

    PathPoint=in->PathPoint;
    PointNum=in->PointNum;

    setID(ID);
}

bool G04Property::SaveXml(QDomDocument *doc)
{
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() != "Data")
    {
        return false;
    }

    QDomElement xmlItems = docElem.firstChildElement("程序配置");
    if(xmlItems.isNull())
    {
        return false;
    }

    QDomElement srailElmt;
    srailElmt  = xmlItems.firstChildElement("运动");

    if(srailElmt.isNull())
    {
        return false;
    }

        QDomNodeList IoNodeList= doc->elementsByTagName("路径信息");
        if(IoNodeList.size()>0)
        {
            //if writen in the doc
            for(int i = 0;i < IoNodeList.size();i++)
            {
                QDomElement pathchild = IoNodeList.at(i).toElement().firstChildElement("路径");
                QDomAttr arrpath=pathchild.attributeNode("Id");
                if(arrpath.value()==pathID)
                {
                    srailElmt.removeChild(IoNodeList.at(i));
                }

                //                QDomElement pathnode=doc->createElement("路径");

                //                QMetaEnum metaEnum=QMetaEnum::fromType<TYPEPATH>();
                //                QDomAttr attrType=doc->createAttribute("Type");
                //                attrType.setValue(metaEnum.valueToKey(type));

                //                QDomAttr attrPathID=doc->createAttribute("Id");
                //                attrPathID.setValue(pathID);
                //                pathnode.setAttributeNode(attrType);
                //                pathnode.setAttributeNode(attrPathID);

                //                for(int j=0;j<PathPoint.count();++j)
                //                {
                //                    QDomElement child=doc->createElement("示教点");
                //                    QDomAttr childAttr=doc->createAttribute("Id");
                //                    QString num;
                //                    childAttr.setValue(num.setNum(j,10));
                //                    child.setAttributeNode(childAttr);
                //                    QDomText text=doc->createTextNode(PathPoint.at(j));
                //                    child.appendChild(text);
                //                    pathnode.appendChild(child);
                //                }
                //                pathmsgNode.appendChild(pathnode);

                //                return true;


            }
        }


    //this is new ,write new doc---fixed by korol delete 运动规划
//    QDomElement parent = doc->createElement("运动规划");
//    QDomAttr attr=doc->createAttribute("Id");
//    attr.setValue(ID);
//    parent.setAttributeNode(attr);


    QDomElement Axiselm =srailElmt.firstChildElement("轴信息");
    if(Axiselm.isNull())
    {
        QDomElement axisMsg=doc->createElement("轴信息");
            for(int ii=0;ii<JointNum;++ii)
            {
                QDomElement child=doc->createElement("J"+QString(QString::number(ii+1,10)));
                QString msg=Joint.at(ii);

                QDomText text=doc->createTextNode(msg);
                child.appendChild(text);
                axisMsg.appendChild(child);
            }

        srailElmt.appendChild(axisMsg);
    }
    QDomElement pathMsg = doc->createElement("路径信息");
    QDomElement pathnode=doc->createElement("路径");

    QMetaEnum metaEnum=QMetaEnum::fromType<TYPEPATH>();
    QDomAttr attrType=doc->createAttribute("Type");
    attrType.setValue(metaEnum.valueToKey(type));

    QDomAttr attrPathID=doc->createAttribute("Id");
    attrPathID.setValue(pathID);

    QDomAttr attrMaxVel = doc->createAttribute("MaxVel");
    attrMaxVel.setValue(QString::number(MaxVel,10));

    QDomAttr attrMode = doc->createAttribute("Mode");
    attrMode.setValue("示教");

    QDomAttr attrVecProfile = doc->createAttribute("VelocityProfile");
    attrVecProfile.setValue("矩形");


    pathnode.setAttributeNode(attrType);
    pathnode.setAttributeNode(attrPathID);
    pathnode.setAttributeNode(attrMaxVel);
    pathnode.setAttributeNode(attrMode);
    pathnode.setAttributeNode(attrVecProfile);

    for(int j=0;j<PathPoint.count();++j)
    {
        QDomElement child=doc->createElement("示教点");
//        QDomAttr childAttr=doc->createAttribute("Id");
//        QString num;
//        childAttr.setValue(num.setNum(j,10));
//        child.setAttributeNode(childAttr);
        QDomText text=doc->createTextNode(PathPoint.at(j));
        child.appendChild(text);
        pathnode.appendChild(child);
    }
    pathMsg.appendChild(pathnode);

//    parent.appendChild(axisMsg);
//    parent.appendChild(pathMsg);

    srailElmt.appendChild(pathMsg);



    return true;


}
