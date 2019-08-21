#include "itemsave.h"
#include "../TFlowGraphics/FlowGraphicsPub.h"
#include"../FlowPropery/PropertyBrowser/objectcontroller.h"
#include <QDebug>

class ItemSavePrivate:public QObject
{
    Q_OBJECT
    ItemSave *q_ptr;
    Q_DECLARE_PUBLIC(ItemSave)
public:
    ItemSavePrivate();
    void slotSave();
};

void ItemSavePrivate::slotSave()
{

}

ItemSave::ItemSave()
{
    ObjectController::RegisterPoint();
    ObjectFactory::registerClass<TFlowCall>();
    ObjectFactory::registerClass<TFlowClear>();
    ObjectFactory::registerClass<TFlowCyLinder>();
    ObjectFactory::registerClass<TFlowDelay>();
    ObjectFactory::registerClass<TFlowIf>();
    ObjectFactory::registerClass<TFlowDS>();
    ObjectFactory::registerClass<TFlowEnd>();
    ObjectFactory::registerClass<FlowIn>();
    ObjectFactory::registerClass<flowlabel>();
    ObjectFactory::registerClass<FlowLin>();
    ObjectFactory::registerClass<FlowLout>();
    ObjectFactory::registerClass<TFlowMoveG00>();
    ObjectFactory::registerClass<TFlowMoveG101>();
    ObjectFactory::registerClass<FlowOut>();
    ObjectFactory::registerClass<FlowSub>();
    ObjectFactory::registerClass<TFlowThread>();
    ObjectFactory::registerClass<TFlowStart>();
    ObjectFactory::registerClass<TFlowJS>();
    ObjectFactory::registerClass<TFlowG04>();
    ObjectFactory::registerClass<TFlowTCPR>();
    ObjectFactory::registerClass<TFlowTCPW>();
    ObjectFactory::registerClass<TFlowORG>();
    ObjectFactory::registerClass<FlowLOG>();
    ObjectFactory::registerClass<FlowWAIT>();
    ObjectFactory::registerClass<TFlowUart>();
    ObjectFactory::registerClass<TFlowCyt>();
    ObjectFactory::registerClass<TFlowCount>();
    ObjectFactory::registerClass<FlowJog>();
}

bool ItemSave::saveFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out<<(quint32)0xAAFEEEAA;
    out<<(qint32)001;
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    TFlowCall *call=new TFlowCall(1,2,3,4);
    call->PropertyBase->setID("OK");
    TFlowBase *val;
    val=call;
    QString className;
    className=QLatin1String(call->metaObject()->className());
    out<<className;
    out<<(*val);
    file.close();
    return true;
}

bool ItemSave::readFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    quint32 magic;
    in>>magic;
    if(magic!=0xAAFEEEAA)
    {
        return false;
    }
    qint32 version;
    in>>version;

    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    TFlowCall *call=new TFlowCall();
    TFlowBase *val;
    val=call;
    QString className;
    in>>className;
    in>>(*val);

    file.close();
    qDebug()<<call->GetLineStyle()<<endl<<className;

    return true;
}

bool ItemSave::saveItem_v1(QMap<QString, QMap<QString, QList<TFlowBase *> > > pro, QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDataStream out(&file);
    out<<(quint32)0xAAFEEEAA;
    out<<(qint32)001;
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    //Project Szie
    int size=pro.size();
    out<<size;


    for(auto proit=pro.constBegin();proit!=pro.constEnd();++proit)
    {
       //Scene Name,Size
        out<<proit.key();
        out<<proit.value().size();

        //QMap<QString,QList<TFlowBase*> >::const_iterator mapit;

        for(auto mapit=proit.value().constBegin();mapit!=proit.value().constEnd();++mapit)
        {
            //child Scene name
            out<<mapit.key();

            //Items number,items Write
            QList<TFlowBase*>::const_iterator it;

            QString className;
            int count=mapit.value().count();
            out<<count;

            for(it=mapit.value().constBegin();it!=mapit.value().constEnd();++it)
            {
                TFlowBase *item;
                item=*it;
                className=QLatin1String(item->metaObject()->className());
                out<<className;
                out<<(*item);
            }
        }
    }

    file.close();
    return true;
}

QMap<QString, QMap<QString, QMap<int, TFlowBase *> > > ItemSave::readItem_v1(QString path)
{
    QFile file(path);
    QMap<QString, QMap<QString, QMap<int, TFlowBase *> > > mapData;
    mapData.clear();
    if(!file.open(QIODevice::ReadOnly))
    {
        return mapData;
    }

    QDataStream in(&file);
    quint32 magic;
    in>>magic;
    if(magic!=0xAAFEEEAA)
    {
        return mapData;
    }
    qint32 version;
    in>>version;
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    int size=0;
    in>>size;

    for(auto proj=0;proj<size;++proj)
    {
        int sceneSize=0;
        QString scenename;
        in>>scenename;
        in>>sceneSize;

        QMap<QString, QMap<int, TFlowBase *> > scene;
        for(int i=0;i<sceneSize;++i)
        {
            QString name;
            in>>name;

            int count=0;
            in>>count;
            QMap<int,TFlowBase*> data;
            QString className;

            for(int i=0;i<count;++i)
            {
                TFlowBase *Item;
                in>>className;
                Item=CreateFlowItemFactory(className);
                in>>(*Item);
                data.insert(Item->GetId(),Item);
            }
            scene.insert(name,data);

        }

        mapData.insert(scenename,scene);
    }

    file.close();
    return mapData;
}

/**
 * @brief ItemSave::saveItem
 * @param pro
 * @param path
 * @return
 */
bool ItemSave::saveItem(QMap<QString,QList<TFlowBase*> > pro,QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDataStream out(&file);
    out<<(quint32)0xAAFEEEAA;
    out<<(qint32)001;
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    //Scene number,each Scene write
    int size=pro.size();
    out<<size;

    QMap<QString,QList<TFlowBase*> >::const_iterator mapit;

    for(mapit=pro.constBegin();mapit!=pro.constEnd();++mapit)
    {
        //Scene name
        out<<mapit.key();

        //Items number,items Write
        QList<TFlowBase*>::const_iterator it;

        QString className;
        int count=mapit.value().count();
        out<<count;

        for(it=mapit.value().constBegin();it!=mapit.value().constEnd();++it)
        {
            TFlowBase *item;
            item=*it;
            className=QLatin1String(item->metaObject()->className());
            out<<className;
            out<<(*item);
        }


    }


    file.close();
    return true;
}

/**
 * @brief ItemSave::readItem
 * @param path
 * @return
 */
QMap<QString,QMap<int,TFlowBase*> > ItemSave::readItem(QString path)
{
    QFile file(path);
    QMap<QString,QMap<int,TFlowBase*> > mapData;
    mapData.clear();
    if(!file.open(QIODevice::ReadOnly))
    {
        return mapData;
    }

    QDataStream in(&file);
    quint32 magic;
    in>>magic;
    if(magic!=0xAAFEEEAA)
    {
        return mapData;
    }
    qint32 version;
    in>>version;
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    int size=0;
    in>>size;

    for(int j=0;j<size;++j)
    {
        QString name;
        in>>name;

        int count=0;
        in>>count;
        QMap<int,TFlowBase*> data;
        QString className;

        for(int i=0;i<count;++i)
        {
            TFlowBase *Item;
            in>>className;
            Item=CreateFlowItemFactory(className);
            in>>(*Item);
            data.insert(Item->GetId(),Item);
        }
        mapData.insert(name,data);

    }


    file.close();
    return mapData;

}

TFlowBase *ItemSave::CreateFlowItemFactory(QString className)
{
    TFlowBase *item;
    item=Q_NULLPTR;
    item=ObjectFactory::createObject(className.toLatin1());
    return item;
}

#include"moc_itemsave.cpp"
//#include"itemsave.moc"


