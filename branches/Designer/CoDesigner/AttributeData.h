#ifndef ATTRIBUTEDATA_H
#define ATTRIBUTEDATA_H
#include <QStringList>
typedef struct _BASE_STRU
{
    int id = 0;
    int type = 0;
    QString text = "";
    uchar pressMode = 0;
    uchar fontSize = 10;
    QString fontColor = "#000000";
    uchar fontAlignVer = 1;
    uchar fontAlignHor = 1;
    QList<uchar> usePixList;
    QStringList bgColorList;
    QStringList bgPixList;
    uchar power = 3;
    _BASE_STRU()
    {
        bgColorList.reserve(2);
        bgPixList.reserve(2);
        usePixList.reserve(2);
        bgColorList.clear();
        bgPixList.clear();
        usePixList << false << false;
    }
}BASE_STRU;

typedef struct _BTNATTR_STRU : public BASE_STRU
{
    _BTNATTR_STRU()
    {
        text = "Button";
    }
    QString jsContent = "";
    QString varName = "";
    uchar varState = 0;
    QString canvasName = "";
    char systemOpState = -1;
    QString regExp = "";
    char regExpState = 0;

}BTNATTR_STRU;

#endif // ATTRIBUTEDATA_H
