#ifndef ITEMSAVE_H
#define ITEMSAVE_H
#include<QFile>
#include<QDataStream>

#include <QByteArray>
#include <QMetaObject>
#include <QHash>

#include "../TFlowGraphics/flowbase.h"


class ItemSavePrivate;

class ItemSave:public QObject
{
    Q_OBJECT
public:
    ItemSave();
public:
    bool saveFile(QString);
    bool readFile(QString);

    bool saveItem_v1(QMap<QString,QMap<QString,QList<TFlowBase*> > >,QString path);
    //<大节点，<名称，<Id,图元对象>>>
    QMap<QString, QMap<QString,QMap<int, TFlowBase *> > > readItem_v1(QString path);

    TFlowBase* CreateFlowItemFactory(QString);

    bool saveItem(QMap<QString,QList<TFlowBase*> >,QString path);
    QMap<QString, QMap<int, TFlowBase *> > readItem(QString path);
private:
    ItemSavePrivate *d_ptr;
    Q_DECLARE_PRIVATE(ItemSave)
    Q_DISABLE_COPY(ItemSave)
    Q_PRIVATE_SLOT(d_func(),void slotSave())
};

class ObjectFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    static TFlowBase* createObject( const QByteArray& className, TFlowBase* parent = NULL )
    {
        Constructor constructor = constructors().value( className );
        if ( constructor == NULL )
            return NULL;
        return (*constructor)(parent);
    }

private:
    typedef TFlowBase* (*Constructor)( TFlowBase* parent );

    template<typename T>
    static TFlowBase* constructorHelper( TFlowBase* parent )
    {
        Q_UNUSED(parent)
        return new T();
    }

    static QHash<QByteArray, Constructor>& constructors()
    {
        static QHash<QByteArray, Constructor> instance;
        return instance;
    }
};

#endif // ITEMSAVE_H
