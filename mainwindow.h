#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);


private slots:
    void update_time();
    void update_object(); // 物體移動更新

    void on_button_start_game_clicked();

private:
    Ui::MainWindow *ui;

    int time;
    int game_status; // 遊戲狀態
    int bgm_pos; // 背景位置
    int car_pos;
    int car_direction;
    float car_distance;

    QTimer *object_timer;
    QTimer *clock_timer;

    void game_start();
    void game_pause();
    void game_stop();
    void move_car();

};
#endif // MAINWINDOW_H
