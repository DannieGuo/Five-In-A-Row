#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"start.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>
#include "chess.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    color(1),
    how(0) {
    ui->setupUi(this);

    QCursor cursor ;//设置鼠标
    QPixmap pixmap(":/image/black1.png") ;
    cursor = QCursor(pixmap, -1, -1);
    setCursor(cursor) ;

    for(int i = 0; i < 13; i++)
         for(int j = 0; j < 13; j++) {
             pointState[i][j] = 0;
             points[i][j].setX(50 + i * 70);
             points[i][j].setY(50 + j * 70);
         }

}

MainWindow::~MainWindow() {
    delete ui;
}

//设置用哪种算法
void MainWindow::setHow(int h) {
    how = h;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 950, 950, QPixmap(":/image/map.png"));

    QPen pen;
    pen.setWidth(5);
    painter.setPen(pen);

    painter.drawRect(50, 50, 850, 850);

    pen.setWidth(2);
    painter.setPen(pen);

    //画棋盘
    for(int i = 1; i <= 11; i++) {
        painter.drawLine(50 + i * 70, 50, 50 + i * 70, 900);
        painter.drawLine(50, 50 + i * 70, 900, 50 + i * 70);
    }

    foreach(const Chess *chess, chessList)//画棋子
        chess->draw(&painter);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPoint pressPos=event->pos();
    for(int i = 0; i < 13; i++)
        for(int j = 0; j < 13; j++) {
            if(pressPos.x() < points[i][j].x() + 15 && pressPos.x() > points[i][j].x() - 15
                    && pressPos.y() < points[i][j].y() + 15 && pressPos.y() > points[i][j].y() - 15
                    && pointState[i][j] == 0) {
                Chess *chess = new Chess(points[i][j].x() - 25, points[i][j].y() - 25, color);
                chessList.push_back(chess);
                pointState[i][j] = color;
                update();

                if(isOver(i, j)) {
                    if(color == 1)
                    {
                        QMessageBox::information(NULL, " ", "Black Win",  QMessageBox::Yes | QMessageBox::Yes);
                        Start *mw = new Start();
                        mw->show();
                        this->close();
                    }
                    else if(color == -1) {
                            QMessageBox::information(NULL, " ", "White Win",  QMessageBox::Yes |  QMessageBox::Yes);
                            Start *mw = new Start();
                            mw->show();
                            this->close();
                        }

                }

                color = -color;

                if(how == 1)
                    putChessGreedy();
                else if(how == 2)
                    putChessMinimax(i, j);
                else if(how == 3) {
                    if(color == 1) {
                        QCursor cursor ;
                        QPixmap pixmap(":/image/black1.png") ;
                        cursor = QCursor(pixmap, -1, -1);
                        setCursor(cursor) ;
                    } else {
                        QCursor cursor ;
                        QPixmap pixmap(":/image/white1.png") ;
                        cursor = QCursor(pixmap, -1, -1);
                        setCursor(cursor) ;
                    }
                }
                break;
            }
        }

}
void MainWindow::putChessGreedy() {
    int max = -10000;
    int now = 0;
    int over = 0;
    int x, y;
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 13; j++) {
            if(pointState[i][j] == 0) {
                pointState[i][j] = -1;
                if(isOver(i, j)) {
                    x = i;
                    y = j;
                    over = 1;
                    break;
                }
                now = calculate();
                //now=calculate2();
                if(now > max) {
                    max = now;
                    x = i;
                    y = j;
                }
                pointState[i][j] = 0;
            }
        } 
        if(over == 1)
            break;
       }

    pointState[x][y] = -1;
    Chess *chess = new Chess(points[x][y].x() - 25, points[x][y].y() - 25, color);
    chessList.push_back(chess);
    update();

    if(over == 1) {
        QMessageBox::information(NULL, " ", "You Lose",  QMessageBox::Yes | QMessageBox::Yes);
        Start *mw = new Start();
        mw->show();
        this->close();
    }
    color = -color;
}

bool MainWindow::isOver(int x, int y) {
    int t = 0;
    for(int i = -4; i <= 4; i++) { //行
        if(t == 5)
            return true;
        if(x + i >= 0 && x + i <= 12 && pointState[x + i][y] == pointState[x][y])
            t++;
        if(x + i >= 0 && x + i <= 12 && pointState[x + i][y] != pointState[x][y])
            t = 0;
    }
    if(t == 5) 
        return true;

    t = 0;
    for(int i = -4; i <= 4; i++) {//列
        if(t == 5)
            return true;
        if(y + i >= 0 && y + i <= 12 && pointState[x][y + i] == pointState[x][y])
            t++;
        if(y + i >= 0 && y + i <= 12 && pointState[x][y + i] != pointState[x][y])
            t = 0;
    }
    if(t == 5) 
        return true;

    t = 0;
    for(int i = -4; i <= 4; i++) { //左斜
        if(t == 5)
            return true;
        if(y + i >= 0 && y + i <= 12 && x + i >= 0 && x + i <= 12 && pointState[x + i][y + i] == pointState[x][y])
            t++;
        if(y + i >= 0 && y + i <= 12 && x + i >= 0 && x + i <= 12 && pointState[x + i][y + i] != pointState[x][y])
            t = 0;
    }
    if(t == 5) 
        return true;

    t = 0;
    for(int i = -4; i <= 4; i++) { //右斜
        if(t == 5)
            return true;
        if(y - i >= 0 && y - i <= 12 && x + i >= 0 && x + i <= 12 && pointState[x + i][y - i] == pointState[x][y])
            t++;
        if(y - i >= 0 && y - i <= 12 && x + i >= 0 && x + i <= 12 && pointState[x + i][y - i] != pointState[x][y])
            t = 0;
    }
    if(t == 5) 
        return true;

    t = 0;
    for(int i = 0; i < 13; i++)
        for(int j = 0; j < 13; j++) {
            if(pointState[i][j] != 0)//判断是否没地方下
                t = 1;
        }
    if(t == 0) {
        QMessageBox::information(NULL, " ", "No Space",  QMessageBox::Yes | QMessageBox::Yes);
        Start *mw = new Start();
        mw->show();
        this->close();
    }
    return false;
}

int MainWindow::calculate() {
    int value = 0;

    for(int i = 0; i < 13; i++)
        for(int j = 0; j < 13; j++) {
            if(pointState[i][j] == 1) {
                if(i + 1 < 13 && pointState[i + 1][j] == -1)//黑的后面一个白的
                    value = value + 20;
                if(i - 1 > 0 && pointState[i - 1][j] == -1)
                    value = value + 20;
                if(j + 1 < 13 && pointState[i][j + 1] == -1)
                    value = value + 20;
                if(j - 1 > 0 && pointState[i][j - 1] == -1)
                    value = value + 20;
                if(j - 1 > 0 && i - 1 > 0 && pointState[i - 1][j - 1] == -1)
                    value = value + 20;
                if(j - 1 > 0 && i + 1 < 13 && pointState[i + 1][j - 1] == -1)
                    value = value + 20;
                if(j + 1 < 13 && i - 1 > 0 && pointState[i - 1][j + 1] == -1)
                    value = value + 20;
                if(j + 1 < 13 && i + 1 < 13 && pointState[i + 1][j + 1] == -1)
                    value = value + 20;

                if(i + 1 < 13 && pointState[i + 1][j] == 1) { //如果两个黑的连着   
                    value = value - 10;
                    if(i + 2 < 13 && pointState[i + 2][j] == 1) {//如果三个黑的连着
                        value = value - 200;
                        if(i + 3 < 13 && pointState[i + 3][j] == 1) { //四个黑的连着
                            value = value - 1000;
                            if(i + 4 < 13 && pointState[i + 4][j] == -1)//四个黑的后有个白的
                                value = value + 2000;
                        }
                        if(i + 3 < 13 && pointState[i + 3][j] == -1) { //三个黑的后有个白的
                            value = value + 700;
                            if(i + 4 < 13 && pointState[i + 4][j] == -1)
                                value = value - 200;
                        }
                    }
                    if(i + 2 < 13 && pointState[i + 2][j] == -1) { //两个黑的后有个白的
                        value = value + 100;
                        if(i + 3 < 13 && pointState[i + 3][j] == 1) { //三个黑的中插了个白的
                            value = value + 200;
                            if(i + 4 < 13 && pointState[i + 4][j] == 1)//四个黑的中插了个白的
                                value = value + 200;
                        }
                    }
                }

                if(j + 1 < 13 && pointState[i][j + 1] == 1) {
                    value = value - 10;
                    if(j + 2 < 13 && pointState[i][j + 2] == 1) {
                        value = value - 200;
                        if(j + 3 < 13 && pointState[i][j + 3] == 1) {
                            value = value - 1000;
                            if(j + 4 > 0 && pointState[i][j + 4] == -1)
                                value = value + 2000;
                        }
                        if(j + 3 < 13 && pointState[i][j + 3] == -1) {
                            value = value + 700;
                            if(j + 4 > 0 && pointState[i][j + 4] == -1)
                                value = value - 200;
                        }
                    }
                    if(j + 2 < 13 && pointState[i][j + 2] == -1) {
                        value = value + 100;
                        if(j + 3 < 13 && pointState[i][j + 3] == 1) {
                            value = value + 200;
                            if(j + 4 > 0 && pointState[i][j + 4] == 1)
                                value = value + 200;
                        }
                    }
                }

                if(j - 1 > 0 && i + 1 < 13 && pointState[i + 1][j - 1] == 1) {
                    value = value - 10;
                    if(j - 2 > 0 && i + 2 < 13 && pointState[i + 2][j - 2] == 1) {
                        value = value - 200;
                        if(j - 3 > 0 && i + 3 < 13 && pointState[i + 3][j - 3] == 1) {
                            value = value - 1000;
                            if(j - 4 > 0 && i + 4 < 13 && pointState[i + 4][j - 4] == -1)
                                value = value + 2000;
                        }
                        if(j - 3 > 0 && i + 3 < 13 && pointState[i + 3][j - 3] == -1) {
                            value = value + 700;
                            if(j - 4 > 0 && i + 4 < 13 && pointState[i + 4][j - 4] == -1)
                                value = value - 200;
                        }
                    }
                    if(j - 2 > 0 && i + 2 < 13 && pointState[i + 2][j - 2] == -1) {
                        value = value + 100;
                        if(j - 3 > 0 && i + 3 < 13 && pointState[i + 3][j - 3] == 1) {
                            value = value + 200;
                            if(j - 4 > 0 && i + 4 < 13 && pointState[i + 4][j - 4] == 1)
                                value = value + 200;
                        }
                    }
                }

                if(j + 1 < 13 && i + 1 < 13 && pointState[i + 1][j + 1] == 1) {
                    value = value - 10;
                    if(j + 2 < 13 && i + 2 < 13 && pointState[i + 2][j + 2] == 1)
                    {
                        value=value-200;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                            value=value-1000;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==-1)
                                value=value+2000;
                        }
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==-1)
                        {
                            value=value+700;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==-1)
                                value=value-200;
                        }
                    }
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==-1)
                    {
                        value=value+100;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                            value=value+200;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==1)
                                value=value+200;
                        }
                    }
                }


            }
            if(pointState[i][j]==-1)
            {
               if(i+1<13&&pointState[i+1][j]==-1)//两个白的
                    value=value+10;
                if(i-1>0&&pointState[i-1][j]==-1)
                    value=value+10;
                if(j+1<13&&pointState[i][j+1]==-1)
                    value=value+10;
                if(j-1>0&&pointState[i][j-1]==-1)
                    value=value+10;
                if(j-1>0&&i-1>0&&pointState[i-1][j-1]==-1)
                    value=value+10;
                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==-1)
                    value=value+10;
                if(j+1<13&&i-1>0&&pointState[i-1][j+1]==-1)
                    value=value+10;
                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==-1)
                    value=value+10;

                if(i+1<13&&pointState[i+1][j]==-1)
                {   value=value+10;
                    if(i+2<13&&pointState[i+2][j]==-1)
                    {
                        value=value+50;
                        if(i+3<13&&pointState[i+3][j]==-1)
                        {
                            value=value+2000;
                            if(i+4==13)
                                value=value-300;
                        }
                    }
                }
                if(i+1<13&&pointState[i+1][j]==1)
                {   value=value+10;
                    if(i+2<13&&pointState[i+2][j]==1)
                    {
                        value=value+100;
                        if(i+3<13&&pointState[i+3][j]==1)
                        {
                            value=value+2000;
                        }
                    }
                }


                if(j+1<13&&pointState[i][j+1]==-1)
                {
                    value=value+10;
                    if(j+2<13&&pointState[i][j+2]==-1)
                    {
                        value=value+50;
                        if(j+3<13&&pointState[i][j+3]==-1)
                        {
                            value=value+2000;
                            if(j+4==0)
                                value=value-300;
                        }
                    }
                }

                if(j+1<13&&pointState[i][j+1]==1)
                {
                    value=value+10;
                    if(j+2<13&&pointState[i][j+2]==1)
                    {
                        value=value+100;
                        if(j+3<13&&pointState[i][j+3]==1)
                        {
                            value=value+2000;
                        }
                    }
                }

                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==-1)
                {
                    value=value+10;
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==-1)
                    {
                        value=value+50;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==-1)
                        {
                            value=value+2000;
                            if(j-4==0||i+4==13)
                                value=value-300;
                        }
                    }
                }


                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==1)
                {
                    value=value+10;
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==1)
                    {
                        value=value+100;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==1)
                        {
                            value=value+2000;
                        }
                    }
                }

                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==-1)
                {
                    value=value+10;
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==-1)
                    {
                        value=value+50;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==-1)
                        {
                           value=value+2000;
                            if(j+4==13||i+4==13)
                                value=value-300;
                        }
                    }
                }


                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==1)
                {
                    value=value+10;
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==1)
                    {
                        value=value+100;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                           value=value+2000;
                        }
                    }
                }

            }
        }
    return value;
}

void MainWindow::putChessMinimax(int a, int b)
{
    int max = -1000000;
    int x, y;
    int over = 0;
    qDebug("another");
    for(int i = -5; i <= 5; i++) { //只在对手放的点的附近找
        for(int j = -5; j <= 5; j++) {
            if(a + i >= 0 && a + i < 13 && b + j >= 0 && b + j < 13 
                && pointState[a + i][b + j] == 0) {
                pointState[a + i][b + j] = -1;
                if(isOver(a + i, b + j)) {
                    x = a + i;
                    y = b + j;
                    over = 1;
                    break;
                }
                int temp = MinValue(a+i, b+j, 0);//对方要挑一个最小的
                qDebug("%d", temp);
                if(max < temp) {
                    max = temp;
                    x = a + i;
                    y = b + j;
                }
                pointState[a + i][b + j] = 0;
            }
        }
        if(over == 1)
            break;
    }
    Chess *chess = new Chess(points[x][y].x() - 25, points[x][y].y() - 25, color);
    chessList.push_back(chess);
    pointState[x][y] = -1;
    update();
    if(over == 1) {
        QMessageBox::information(NULL, " ", "You Lose",  QMessageBox::Yes | QMessageBox::Yes);
        Start *mw = new Start();
        mw->show();
        this->close();
    }
    color = -color;
}
int MainWindow::MinValue(int x, int y, int deep)
{
    if(deep > 2)
        return calculate2();

    int min = 1000000;
    deep++;
    for(int i = -3; i <= 3; i++)//只在刚放的点的附近判断
        for(int j = -3; j <= 3; j++) {
            if(x + i >= 0 && y + j >= 0 &&
                x + i < 13 && y + j < 13 
                    && pointState[x + i][y + j] == 0) {
                pointState[x + i][y + j] = 1;
               if(isOver(x + i, y + j)) {
                    pointState[x + i][y + j] = 0;
                    return -999999;
                }
                int temp = MaxValue(x + i, y + j, deep);
                if(temp < min)
                    min = temp;
                pointState[x + i][y + j] = 0;
            }
        }
    return min;

}
int MainWindow::MaxValue(int x, int y, int deep)
{
    if(deep > 2)
        return calculate2();

    int max = -1000000;
    deep++;
    for(int i = -3; i <= 3; i++)
        for(int j = -3; j <= 3; j++) {
            if(x + i >= 0 && y + j >= 0
                && x + i < 13 && y + j < 13 
                    && pointState[x + i][y + j] == 0) {
                pointState[x + i][y + j] = -1;
               if(isOver(x + i, y + j)) {
                    pointState[x + i][y + j] = 0;
                    return 999999;
                }
                int temp = MinValue(x + i, y + j, deep);
                if(temp > max)
                    max = temp;
                pointState[x + i][y + j] = 0;
            }
        }
    return max;
}
int MainWindow::calculate2() { //分值略不同,minimax用这个更好
    int value = 0;

    for(int i = 0; i < 13; i++)
        for(int j = 0; j < 13; j++) {
            if(pointState[i][j] == 1)
            {
                if(i+1<13&&pointState[i+1][j]==-1)//黑的后面一个白的
                    value=value+50;
                if(i-1>0&&pointState[i-1][j]==-1)
                    value=value+50;
                if(j+1<13&&pointState[i][j+1]==-1)
                    value=value+50;
                if(j-1>0&&pointState[i][j-1]==-1)
                    value=value+50;
                if(j-1>0&&i-1>0&&pointState[i-1][j-1]==-1)
                    value=value+50;
                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==-1)
                    value=value+50;
                if(j+1<13&&i-1>0&&pointState[i-1][j+1]==-1)
                    value=value+50;
                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==-1)
                    value=value+50;

                if(i+1<13&&pointState[i+1][j]==1)//如果两个黑的连着
                {   value=value-100;
                    if(i+2<13&&pointState[i+2][j]==1)//如果三个黑的连着
                    {
                        value=value-1000;
                        if(i+3<13&&pointState[i+3][j]==1)//四个黑的连着
                        {
                            value=value-2000;
                            if(i+4<13&&pointState[i+4][j]==-1)//四个黑的后有个白的
                                value=value+2000;
                        }
                        if(i+3<13&&pointState[i+3][j]==-1)//三个黑的后有个白的
                        {
                            value=value+2000;
                            if(i+4<13&&pointState[i+4][j]==-1)
                                value=value-200;
                        }
                    }
                    if(i+2<13&&pointState[i+2][j]==-1)//两个黑的后有个白的
                    {
                        value=value+200;
                        if(i+3<13&&pointState[i+3][j]==1)//三个黑的中插了个白的
                        {
                            value=value+400;
                            if(i+4<13&&pointState[i+4][j]==1)//四个黑的中插了个白的
                                value=value+800;
                        }
                    }
                }

                if(j+1<13&&pointState[i][j+1]==1)
                {
                    value=value-100;
                    if(j+2<13&&pointState[i][j+2]==1)
                    {
                        value=value-1000;
                        if(j+3<13&&pointState[i][j+3]==1)
                        {
                            value=value-2000;
                            if(j+4>0&&pointState[i][j+4]==-1)
                                value=value+2000;
                        }
                        if(j+3<13&&pointState[i][j+3]==-1)
                        {
                            value=value+2000;
                            if(j+4>0&&pointState[i][j+4]==-1)
                                value=value-200;
                        }
                    }
                    if(j+2<13&&pointState[i][j+2]==-1)
                    {
                        value=value+200;
                        if(j+3<13&&pointState[i][j+3]==1)
                        {
                            value=value+400;
                            if(j+4>0&&pointState[i][j+4]==1)
                                value=value+800;
                        }
                    }

                }

                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==1)
                {
                    value=value-100;
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==1)
                    {
                        value=value-1000;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==1)
                        {
                            value=value-2000;
                            if(j-4>0&&i+4<13&&pointState[i+4][j-4]==-1)
                                value=value+2000;
                        }
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==-1)
                        {
                            value=value+2000;
                            if(j-4>0&&i+4<13&&pointState[i+4][j-4]==-1)
                                value=value-200;
                        }
                    }
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==-1)
                    {
                        value=value+200;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==1)
                        {
                            value=value+400;
                            if(j-4>0&&i+4<13&&pointState[i+4][j-4]==1)
                                value=value+800;
                        }
                    }
                }

                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==1)
                {
                    value=value-100;
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==1)
                    {
                        value=value-1000;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                            value=value-2000;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==-1)
                                value=value+2000;
                        }
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==-1)
                        {
                            value=value+2000;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==-1)
                                value=value-200;
                        }
                    }
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==-1)
                    {
                        value=value+200;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                            value=value+400;
                            if(j+4<13&&i+4<13&&pointState[i+4][j+4]==1)
                                value=value+800;
                        }
                    }
                }


            }
            if(pointState[i][j] == -1) {
               if(i+1<13&&pointState[i+1][j]==-1)
                    value=value+50;
                if(i-1>0&&pointState[i-1][j]==-1)
                    value=value+50;
                if(j+1<13&&pointState[i][j+1]==-1)
                    value=value+50;
                if(j-1>0&&pointState[i][j-1]==-1)
                    value=value+50;
                if(j-1>0&&i-1>0&&pointState[i-1][j-1]==-1)
                    value=value+50;
                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==-1)
                    value=value+50;
                if(j+1<13&&i-1>0&&pointState[i-1][j+1]==-1)
                    value=value+50;
                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==-1)
                    value=value+50;

                if(i+1<13&&pointState[i+1][j]==-1)
                {   value=value+100;
                    if(i+2<13&&pointState[i+2][j]==-1)
                    {
                        value=value+400;
                        if(i+3<13&&pointState[i+3][j]==-1)
                        {
                            value=value+2000;
                            if(i+4==13)
                                value=value-300;
                        }
                    }
                }
                if(i+1<13&&pointState[i+1][j]==1)
                {   value=value+100;
                    if(i+2<13&&pointState[i+2][j]==1)
                    {
                        value=value+400;
                        if(i+3<13&&pointState[i+3][j]==1)
                        {
                            value=value+2000;
                        }
                    }
                }


                if(j+1<13&&pointState[i][j+1]==-1)
                {
                    value=value+100;
                    if(j+2<13&&pointState[i][j+2]==-1)
                    {
                        value=value+400;
                        if(j+3<13&&pointState[i][j+3]==-1)
                        {
                            value=value+2000;
                            if(j+4==0)
                                value=value-300;
                        }
                    }
                }

                if(j+1<13&&pointState[i][j+1]==1)
                {
                    value=value+100;
                    if(j+2<13&&pointState[i][j+2]==1)
                    {
                        value=value+400;
                        if(j+3<13&&pointState[i][j+3]==1)
                        {
                            value=value+2000;
                        }
                    }
                }

                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==-1)
                {
                    value=value+100;
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==-1)
                    {
                        value=value+400;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==-1)
                        {
                            value=value+2000;
                            if(j-4==0||i+4==13)
                                value=value-300;
                        }
                    }
                }


                if(j-1>0&&i+1<13&&pointState[i+1][j-1]==1)
                {
                    value=value+100;
                    if(j-2>0&&i+2<13&&pointState[i+2][j-2]==1)
                    {
                        value=value+400;
                        if(j-3>0&&i+3<13&&pointState[i+3][j-3]==1)
                        {
                            value=value+2000;
                        }
                    }
                }

                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==-1)
                {
                    value=value+100;
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==-1)
                    {
                        value=value+400;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==-1)
                        {
                           value=value+2000;
                            if(j+4==13||i+4==13)
                                value=value-300;
                        }
                    }
                }


                if(j+1<13&&i+1<13&&pointState[i+1][j+1]==1)
                {
                    value=value+100;
                    if(j+2<13&&i+2<13&&pointState[i+2][j+2]==1)
                    {
                        value=value+400;
                        if(j+3<13&&i+3<13&&pointState[i+3][j+3]==1)
                        {
                           value=value+2000;
                        }
                    }
                }

            }
        }
    return value;
}
