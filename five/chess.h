#ifndef CHESS_H
#define CHESS_H
#include <QPoint>
#include <QPainter>

class Chess
{
public:
    Chess(int x, int y, int color1);
    void draw(QPainter *painter) const; 

private:
    QPoint pos;
    int color;
};

#endif // CHESS_H
