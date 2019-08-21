#ifndef positionStore_H
#define positionStore_H

#include <QString>
#include <QFile>
#include <QDataStream>

class positionStore
{
public:
    positionStore(QString path);
    ~positionStore();

    bool SavePosition(int x, int y, int z);
    bool GetPosition(int &x, int &y, int &z);

private:

    QString filePath;
    QFile *file;
    QDataStream *stream;

    bool openFile();
};

#endif // positionStore_H
