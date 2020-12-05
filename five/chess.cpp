#include "chess.h"
#include <QPoint>
#include <QPainter>
#include <QPixmap>
Chess::Chess(int x, int y, int color1) {
        pos.setX(x);
        pos.setY(y);
        color = color1;
}
void Chess::draw(QPainter *painter) const {
    if(color == 1) {
        QPen pen;
        QBrush brush(Qt::black);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawEllipse(pos.x(), pos.y(), 50, 50);
    }
    else if(color == -1) {
        QPen pen;
        QBrush brush(Qt::white);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawEllipse(pos.x(), pos.y(), 50, 50);
    }
}

