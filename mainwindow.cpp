#include "mainwindow.h"
#include "ui_mainwindow.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");
bool MainWindow::isJoystick = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow),
      m_bRealPlayStarted(false),
      m_bPlayBackStarted(false),
      m_bPlayBackFinished(false),
      m_bVideoDownload(false),
      m_bDeviceVideoDownload(false),
      m_bCloudPlayBack(false)
{
    ui->setupUi(this);
    set_serial();//串口
    set_map();//地图
    set_vodeo();//视频
    set_joystick();//摇杆
    set_http();//这个是我写的http通信
    setColGarbage();//初始化控制收垃圾的三个滑块 分别为左舵机、右舵机、抽水电机
//    set_net();//hkw的http通信
//    cout <<"MainWindow::isJoystick:"<< MainWindow::isJoystick;
    boat_id="&id=wrc";//船的id http通信的时候要用
//    ui->groupBox_4->setVisible(false);//groupBox_4用来存放一些暂时用不到的控件 将其设置为不可见
    //terminal_lon和terminal_lat这两个QLabel是可以用的,但是JS不能调用Qt函数 问题存在于Js和Qt的通信
//    ui->terminal_lon->setText(QString::number(1.23456,'f',6));
//    ui->terminal_lat->setText(QString::number(1.23456,'f',6));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getCoordinate(QString lon, QString lat)
{
    cout <<lon<<lat;
}

//stop按下
void MainWindow::on_buttonStop_0_pressed()
{
    //连续按下按键
    ui->buttonStop_0->setAutoRepeat(true);
    ui->buttonStop_0->setAutoRepeatInterval(buttonautoRepeatInterval);
    buttonNumber = 0;
    httpCom6();
}
//front按下
void MainWindow::on_buttonFront_1_pressed()
{
    //连续按下按键
    ui->buttonFront_1->setAutoRepeat(true);
    ui->buttonFront_1->setAutoRepeatInterval(buttonautoRepeatInterval);
    buttonNumber = 1;
    httpCom6();
}
//back按下
void MainWindow::on_buttonBack_5_pressed()
{
    //连续按下按键
    ui->buttonBack_5->setAutoRepeat(true);
    ui->buttonBack_5->setAutoRepeatInterval(buttonautoRepeatInterval);
    buttonNumber = 5;
    httpCom6();
}
//left按下
void MainWindow::on_buttonLeft_7_pressed()
{
    //连续按下按键
    ui->buttonLeft_7->setAutoRepeat(true);
    ui->buttonLeft_7->setAutoRepeatInterval(buttonautoRepeatInterval);
    buttonNumber = 7;
    httpCom6();
}
//right按下
void MainWindow::on_buttonRight_3_pressed()
{
    //连续按下按键
    ui->buttonRight_3->setAutoRepeat(true);
    ui->buttonRight_3->setAutoRepeatInterval(buttonautoRepeatInterval);
    buttonNumber = 3;
    httpCom6();
}
//急停按下 发送给服务器,我这边就不需要管了(服务器那边会自动停止小船行驶方向控制)
void MainWindow::on_buttonStop_9_clicked()
{
    buttonNumber = 9;
    httpCom6();
}





