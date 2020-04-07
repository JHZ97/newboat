#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"




void MainWindow::solve_joy(joyinfoex_tag state_row)
{
    unsigned long joy_btn =state_row.dwButtons;
    unsigned long x_pos = state_row.dwXpos;
    unsigned long y_pos = state_row.dwYpos;
    ui->status->append(QString("btn:")+QString::number(joy_btn));
}


void MainWindow::set_joystick()
{
    joy = QJoysticks::getInstance();//获取实例
    /*
    if(joy->nonBlacklistedCount()==0)
    {
        QString dlgTitle = QStringLiteral("手柄连接失败");
        QString strInfo =QStringLiteral("未检测到手柄");
        QMessageBox::warning(this,dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
        set_joystick();
    }
    */
    connect(joy,&QJoysticks::axisEvent,this,&MainWindow::JoyAixChanged);
    connect(joy,&QJoysticks::buttonEvent,this,&MainWindow::JoyButtonChanged);
}
//摇杆坐标
void MainWindow::JoyAixChanged(const QJoystickAxisEvent e)
{
    axisE = e;//axisE作为全局变量 未使用
    //横向
    x_direction = int(joy->getAxis(e.joystick->id,0));
    myT->myx_direction = x_direction;
    ui->x->setValue(joy->getAxis(e.joystick->id,0));
    cout<< "x_direction:"<<x_direction;

    //纵向
    y_direction = int(joy->getAxis(e.joystick->id,1));
    myT->myy_direction = y_direction;
    ui->y->setValue(joy->getAxis(e.joystick->id,1));
    cout<< "y_direction:"<<y_direction;

    MainWindow::isJoystick = true;
    emit myJoyAixChanged();
}
//按钮
void MainWindow::JoyButtonChanged(const QJoystickButtonEvent e)
{
   MainWindow::isJoystick = true;
   buttonE = e;//buttonE作为全局变量 使用了
   emit myJoyButtonPressed();
   if (e.pressed)
   {
    ui->btn_status->append(QString("btn")+QString::number(e.button));
   }
}
