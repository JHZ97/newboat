#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::solve_sensor(QByteArray buf)
{
    //GPS
    double GPS_N = hex2int(buf[3],buf[4],buf[5],buf[6])/1000000.0;
    //ui->GPS_lat->setText(QString::number(GPS_N,'f',4));
    double GPS_E = hex2int(buf[7],buf[8],buf[9],buf[10])/1000000.0;
    //ui->GPS_lon->setText(QString::number(GPS_E,'f',4));
    cout << "GPS_N:"<<GPS_N;
    cout << "GPS_E:"<<GPS_E;
    //油门开度
    leftMotor = hex2int1(buf[11]);
    rightMotor = hex2int1(buf[12]);
    ui->leftMotorValue->setText(QString::number(leftMotor));
    ui->rightMotorValue->setText(QString::number(rightMotor));
    cout << "leftMotor:" << leftMotor;
    cout << "rightMotor:" << rightMotor;
    //偏航角
    yawAngleValue = hex2int2(buf[13],buf[14]);
    ui->yawAngle->setText(QString::number(yawAngleValue));
    cout <<"yawAngleValue:"<<yawAngleValue;
    //判断是人工驾驶还是自动驾驶 isAutopilot =1 自动模式
    isAutopilot = hex2int1(buf[15]);
    cout <<"isAutopilot:"<< isAutopilot;
    if(isAutopilot==1)
    {
        ui->curMode->setText(QString::fromLocal8Bit("自动"));
    }
    else
    {
         ui->curMode->setText(QString::fromLocal8Bit("手动"));
    }
    //电池电量
    batteryPower = hex2int2(buf[16],buf[17])/10.0;
    cout << "batteryPower:" << batteryPower;
    ui->batteryPower->setText(QString::number(batteryPower));
    //自检
    selfCheck = hex2int1(buf[18]);
    //hcm365
    hcm365 = selfCheck & 0x01;
    if(hcm365 == 1)
    {
        ui->hcm365->setText(QString::fromLocal8Bit("hcm365开启"));
    }
    else
    {
      ui->hcm365->setText(QString::fromLocal8Bit("hcm365关闭"));
    }
    //millimeterWaveRadar
    millimeterWaveRadar = selfCheck & 0x02;
    if(millimeterWaveRadar == 2)
    {
        ui->millimeterWaveRadar->setText(QString::fromLocal8Bit("毫米波雷达开启"));
    }
    else
    {
      ui->millimeterWaveRadar->setText(QString::fromLocal8Bit("毫米波雷达关闭"));
    }
    //gps
    gpsStatus = selfCheck & 0x04;
    if(gpsStatus == 4)
    {
        ui->gpsStatus->setText(QString::fromLocal8Bit("gps开启"));
    }
    else
    {
      ui->gpsStatus->setText(QString::fromLocal8Bit("gps关闭"));
    }
//    qDebug()<< yawAngleValue;   
//    qDebug()<< GPS_E<<GPS_N;
    //gps地图设置
    setGPS(GPS_E,GPS_N);
}



int MainWindow::hex2int(char c1 ,char c2 ,char c3,char c4)
{
    unsigned char a1,a2,a3,a4;
    a1 = c1;
    a2 = c2;
    a3=c3;
    a4=c4;
    unsigned int d=a1;
    d<<=8;
    d|=a2;
    d<<=8;
    d|=a3;
    d<<=8;
    d|=a4;
    int m=d;
    return m;
}
int MainWindow::hex2int1(char c1)
{
    unsigned char a1;
    a1 = c1;
    unsigned int d = a1;
    int m = d;
    return m;
}
int MainWindow::hex2int2(char c1,char c2)
{
    unsigned char a1,a2;
    a1 = c1;
    a2 = c2;
    unsigned int d = a1;
    d<<=8;
    d|=a2;
    int m = d;
    return m;
}
short MainWindow::hex2short(char c1,char c2)
{
    unsigned char a1;
    unsigned char a2;
    unsigned short dis;
    a1=c1;
    a2 =c2;
    dis = a1;
    dis<<=8;
    dis= dis|a2;
    short m = dis;
    return m;
}
