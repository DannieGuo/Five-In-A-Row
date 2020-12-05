#include "start.h"
#include "ui_start.h"
#include <QPainter>
#include <QPixmap>
#include "mainwindow.h"
Start::Start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Start) {
    ui->setupUi(this);
    connect(ui->Persons, SIGNAL(clicked()), this, SLOT(Persons_clicked()));
    connect(ui->greed, SIGNAL(clicked()), this, SLOT(greed_clicked()));
    connect(ui->minimax, SIGNAL(clicked()), this, SLOT(minimax_clicked()));
}

Start::~Start() {
    delete ui;
}
void Start::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 950, 950, QPixmap(":/image/map.png"));
}
void Start::on_Persons_clicked() {
    MainWindow *mw = new MainWindow();
    mw->show();
    mw->setHow(3);
    this->close();
}

void Start::on_greed_clicked() {
    MainWindow *mw = new MainWindow();
    mw->show();
    mw->setHow(1);
    this->close();
}

void Start::on_minimax_clicked() {
    MainWindow *mw = new MainWindow();
    mw->show();
    mw->setHow(2);
    this->close();
}
