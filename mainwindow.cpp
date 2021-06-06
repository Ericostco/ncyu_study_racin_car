#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    game_status = 0;

    bgm_pos = 0;
    object_timer = new QTimer(this);
    connect(object_timer , SIGNAL(timeout()) , this , SLOT(update_object()));
    object_timer -> start(5);

    time = 30;
    clock_timer = new QTimer(this);
    connect(clock_timer , SIGNAL(timeout()) , this , SLOT(update_time()));
    //clock_timer -> start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_object(){
    bgm_pos -= 1;

    ui->background->setGeometry(QRect(bgm_pos , 0 , 4800 , 512));
    if(bgm_pos == -32)
    {
        bgm_pos = 0;
    }

    if(game_status == 1)
    {
        car_distance +=1;
        ui->label_distance->setText("Distance: " + QString::number(car_distance , 'f' , 1) + "meters");
        move_car();
    }
}

void MainWindow::move_car()
{
    int car_new_pos = car_pos + car_direction;
    if((car_new_pos) >= 30 && (car_new_pos) <= 410)
    {
        car_pos = car_new_pos;
        ui->car->setGeometry(QRect(10 , car_pos , 237 , 71));
    }
    else
    {
        game_status = 4;
        game_stop();
    }
}

void MainWindow::update_time()
{
    time -= 1;

    ui->lcd_clock->display(time);

    if(time == 0)
    {
        game_status = 3;
        game_stop();
    }
}

void MainWindow::game_stop()
{
    clock_timer -> stop();
    object_timer -> stop();
}

void MainWindow::on_button_start_game_clicked()
{
    game_start();
}

void MainWindow::game_start()
{
    game_status = 1;
    ui->label_title->setVisible(false);
    ui->button_start_game->setVisible(false);

    clock_timer->start(1000);
    time = 30;
    ui->lcd_clock->display(time);

    car_pos = 220;
    car_distance = 0;
    car_direction = 0;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:
            car_direction = -1;
            break;
        case Qt::Key_Down:
            car_direction = 1;
            break;
    }
}
