#ifndef CORECT_H
#define CORECT_H

#include <QtGui>

class CoRect
{
public:
    CoRect(int topLeft, int topRight, int bottomLeft, int bottomRight);
    CoRect(int radius = 0);

    void draw(QPainter *painter, const QRect &rect);

public:
    int tl, tr, bl, br;
    bool is_equal;
};

#endif // CORECT_H
