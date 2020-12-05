#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPoint>
#include <QList>
#include "chess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isOver(int x, int y); //判断是否结束
    void setHow(int h); //设置用什么算法
    void putChessGreedy();
    void putChessMinimax(int a, int b);
    int MaxValue(int x, int y, int deep);
    int MinValue(int x, int y, int deep);
    int calculate(); //分数计算
    int calculate2();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    QList<Chess*> chessList;
    int color; //1黑, -1白
    QPoint points[13][13]; //点的位置
    int pointState[13][13]; //各点的情况
    int how; //算法
};

#endif // MAINWINDOW_H
