#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
//#include "mainwindow.h"//不要重复包含头文件 不然会报错
//#include "ui_mainwindow.h"

class httpThread : public QObject
{
    Q_OBJECT
public:
    explicit httpThread(QObject *parent = nullptr);

    void threadDealFun();//线程处理函数
    void httpCom3();//将摇杆参数发送给服务器端的函数
    void httpCom4();//向服务器端获取gps,油门开度,偏航角等数据的函数
    int myx_direction;
    int myy_direction;
signals:
    void myThreadEndSignal();//线程处理函数处理完成发出信号
    void showMyreplyMessageSignal(QString temp);//在网络请求应答处理槽中发射这个信号 通过信号与槽的方式把myreply->readAll()中的内容传递出去
    void dealDataSignal(QString temp);//已经从服务器端获取了gps,油门开度,偏航角等数据,但这个数据需要进行类型转换
private:
    QNetworkAccessManager *mymanager;
    QNetworkReply *myreply;

    QNetworkAccessManager *mymanager1;
    QNetworkReply *myreply1;
private slots:
    void replyFinishedSlot1(QNetworkReply*); //网络请求应答处理 摇杆
    void replyFinishedSlot2(QNetworkReply*); //网络请求应答处理 gps,油门开度,偏航角等数据

};

#endif // HTTPTHREAD_H
