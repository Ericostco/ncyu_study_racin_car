#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>

#define BLOCK01_X_POS 300
#define BLOCK01_Y_POS 40
#define BLOCK02_X_POS 2300
#define BLOCK02_Y_POS 110
#define BLOCK03_X_POS 900
#define BLOCK03_Y_POS 190

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    game_status = 0;

    ui->block_01->setGeometry(QRect(-100 , 0 , 64 , 64));
    ui->block_02->setGeometry(QRect(-100 , 0 , 64 , 64));
    ui->block_03->setGeometry(QRect(-100 , 0 , 64 , 64));

    bgm_pos = 0;
    object_timer = new QTimer(this);
    connect(object_timer , SIGNAL(timeout()) , this , SLOT(update_object()));
    object_timer->start(10);

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
        move_blocks();
        detect_blocks();
    }
}

void MainWindow::move_blocks()
{
    ui->block_01->setGeometry((QRect(BLOCK01_X_POS - car_distance , BLOCK01_Y_POS , 64 , 64)));
    ui->block_02->setGeometry((QRect(BLOCK02_X_POS - car_distance , BLOCK02_Y_POS , 64 , 64)));
    ui->block_03->setGeometry((QRect(BLOCK03_X_POS - car_distance , BLOCK03_Y_POS , 64 , 64)));
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

    if(game_status == 3)
    {
        ui -> label_title -> setPixmap(QPixmap(":/src/game_timeout.png"));
    }

    else if(game_status == 4){
        ui->label_title->setPixmap(QPixmap(":/src/game_falied.png"));
    }

    ui->label_title->setVisible(true);
    ui->button_start_game->setVisible(true);

    ui->actionstart_game->setText("開始遊戲");
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

    bgm_pos = 0;

    if(!object_timer->isActive()){
        object_timer->start(10);
    }

    ui->actionstart_game->setText("暫停遊戲");
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

        case Qt::Key_Space:
            game_pause();
            qDebug() << "space";
        break;
    }
}

void MainWindow::detect_blocks()
{
    int tmp_padding_x , tmp_padding_y;

    tmp_padding_x = BLOCK01_X_POS - car_distance;
    tmp_padding_y = BLOCK01_Y_POS - ui->car->y();
    if(is_colliion(tmp_padding_x , tmp_padding_y))
    {
        return;
    }
    tmp_padding_x = BLOCK02_X_POS - car_distance;
    tmp_padding_y = BLOCK02_Y_POS - ui->car->y();
    if(is_colliion(tmp_padding_x , tmp_padding_y))
    {
        return;
    }
    tmp_padding_x = BLOCK03_X_POS - car_distance;
    tmp_padding_y = BLOCK03_Y_POS - ui->car->y();

    if(is_colliion(tmp_padding_x , tmp_padding_y))
    {
        return;
    }
}

bool MainWindow::is_colliion(int x , int y)
{
    if((x < 247) && (x > -50))
    {
        if((y < 55) && (y > -55))
        {
            game_status = 4;
            game_stop();
            return true;
        }
    }
    return false;
}

void MainWindow::on_actionstart_game_triggered()
{
    switch (game_status) {
        case 0:
        case 3:
        case 4:
            game_start();
            break;
        case 1:
        case 2:
            game_pause();
            break;

    }
}

void MainWindow::on_actionclose_game_triggered()
{
    close();
}

void MainWindow::game_pause(){
    if(game_status == 1)
    {
        game_status = 2;

        clock_timer->stop();
        object_timer->stop();

        ui->label_title->setPixmap(QPixmap(":/src/game_pause.png"));
        ui->label_title->setVisible(true);
        ui->actionstart_game->setText("開始遊戲");
    }

    else if(game_status == 2){
        game_status = 1;

        clock_timer->start(1000);
        object_timer->start(10);

        ui->label_title->setVisible(false);
        ui->actionstart_game->setText("暫停遊戲");
    }
}

