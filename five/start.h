#ifndef START_H
#define START_H

#include <QMainWindow>
#include <QPainter>
namespace Ui {
class Start;
}

class Start : public QMainWindow
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_Persons_clicked();

    void on_greed_clicked();

    void on_minimax_clicked();

private:
    Ui::Start *ui;
};

#endif // START_H
