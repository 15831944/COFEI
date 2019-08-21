#include "positionstore.h"
#include <QDebug>
#include <QIODevice>

positionStore::positionStore(QString path)
{
    filePath = "/home/." + path;
    openFile();
}

positionStore::~positionStore()
{
    delete stream;
    file->close();
    delete file;
}

// 并发访问需要考虑同步的问题
bool positionStore::openFile()
{
    bool isOk = true;

    if (!(file = new QFile(filePath)))
        isOk = false;

    if (!file->open(QIODevice::ReadWrite))
    {
        qDebug() << "Open File Fail. File Path: " << filePath << endl;
        isOk = false;
    }

    if ( !(stream = new QDataStream(file)) )
        isOk = false;
    else stream->setVersion(QDataStream::Qt_5_7);

    return isOk;
}

bool positionStore::SavePosition(int x, int y, int z)
{
    bool isOk = true;

    if ( !file->seek(0) )
        isOk = false;

    (*stream) << x << y << z;
    file->flush();

/******************************************************/
/****** for debug ******/

//    switch (stream->status()) {
//    case QDataStream::Ok :
//        qDebug() << "status: ok" <<endl;
//        break;
//    case QDataStream::ReadPastEnd :
//        qDebug() << "status: readEnd" << endl;
//        break;
//    case QDataStream::ReadCorruptData :
//        qDebug() << "status: readcorruptDate" << endl;
//    case QDataStream::WriteFailed :
//        qDebug() << "status: write fail" << endl;
//    default:
//        qDebug() << "unknown status" << endl;
//        break;
//    }
/******************************************************/

    return isOk;
}

bool positionStore::GetPosition(int &x, int &y, int &z)
{
    bool isOk = true;

    if (!file->size())
        return false;

    if (!file->seek(0))
        isOk = false;

    (*stream) >> x >> y >> z;

    return isOk;
}
