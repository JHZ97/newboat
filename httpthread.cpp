#include "httpthread.h"
#include "mainwindow.h"//不要重复包含头文件 不然会报错
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ << ":" <<__LINE__ << "]"
httpThread::httpThread(QObject *parent) : QObject(parent)
{

}
//线程处理函数
void httpThread::threadDealFun()
{
//    cout << "*****************************Child thread id:"<<  QThread::currentThreadId()<<"*****************************";
    //因摇杆变动而进入线程 摇杆
//    if(true == MainWindow::isJoystick)
//    {
//        MainWindow::isJoystick = false;
//        httpCom3();
//        return;
//        emit myThreadEndSignal();//线程处理函数处理完成发出信号
//    }
    //因定时器时间到了而进入线程 gps,油门开度,偏航角等数据
//    else{
//        httpCom4();
//    }
    httpCom4();
}
//摇杆
void httpThread::httpCom3()
{
    mymanager = new QNetworkAccessManager(this); //新建一个网络协调
    connect(mymanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot1(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器

    QByteArray postArray;
    //用摇杆的话，把下面的注释去掉即可
    switch(myx_direction)
    {
        case -1:
            switch(myy_direction)
            {
                case -1:
                    postArray.append("direction=4");//右下
                    postArray.append("&id=wrc");
                    break;
                case 0:
                    postArray.append("direction=3");//右
                    postArray.append("&id=wrc");
                    break;
                case 1:
                    postArray.append("direction=2");//右上
                    postArray.append("&id=wrc");
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append("&id=wrc");
                    break;
            }
            break;
        case 0:
            switch(myy_direction)
            {
                case -1:
                    postArray.append("direction=5");//下
                    postArray.append("&id=wrc");
                    break;
                case 0:
                    postArray.append("direction=0");//中
                    postArray.append("&id=wrc");
                    break;
                case 1:
                    postArray.append("direction=1");//上
                    postArray.append("&id=wrc");
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append("&id=wrc");
                    break;
            }
            break;
        case 1:
            switch(myy_direction)
            {
                case -1:
                    postArray.append("direction=6");//左下
                    postArray.append("&id=wrc");
                    break;
                case 0:
                    postArray.append("direction=7");//左
                    postArray.append("&id=wrc");
                    break;
                case 1:
                    postArray.append("direction=8");//左上
                    postArray.append("&id=wrc");
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append("&id=wrc");
                    break;
            }
            break;
        default:
            postArray.append("direction=0");//中
            postArray.append("&id=wrc");
            break;
    }
    cout << "myx_direction:"<<myx_direction;
    cout <<"myy_direction:"<<myy_direction;
//    cout << "myx_direction:"<<myx_direction;
//    cout <<"myy_direction:"<<myy_direction;
//    postArray.append("direction=1");//上
//    postArray.append("&id=wrc");

    QNetworkRequest request(QUrl("http://47.100.92.173:10000/move"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    myreply = mymanager->post(request,postArray);
}

//摇杆
void httpThread::replyFinishedSlot1(QNetworkReply *)
{
    QString myMessage = QString(myreply->readAll());
    emit showMyreplyMessageSignal(myMessage);
    myreply->deleteLater(); //删除该网络请求
}
//gps,油门开度,偏航角等数据的函数
void httpThread::httpCom4()
{
    mymanager1 = new QNetworkAccessManager(this); //新建一个网络协调
    connect(mymanager1,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot2(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    postArray.append("&id=wrc");//发送给服务器端的指令
    QNetworkRequest request(QUrl("http://47.100.92.173:10000/gpsCheck"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    myreply1 = mymanager1->post(request,postArray);
}
//网络请求应答处理 gps,油门开度,偏航角等数据
void httpThread::replyFinishedSlot2(QNetworkReply *)
{
//    cout <<"myreply1->readAll():"<< myreply1->readAll();

//    if(myreply1->error()!=QNetworkReply::NoError){
//            //处理中的错误信息
//            cout<<"reply error:"<<myreply1->errorString();
//        }else{
//            //cout<<"raw header:"<<reply->rawHeaderList();

//            //获取响应信息
//            QString reply_data=QString(myreply1->readAll());
////           const QByteArray reply_data=myreply1->readAll();
//            //QString s(reply_data);
////            cout<<"read all:"<<HexStringToByteArray(reply_data).toHex();
////            solve_sensor(HexStringToByteArray(reply_data));
//            emit dealDataSignal(reply_data);

//    }
    QString reply_data=QString(myreply1->readAll());
    emit dealDataSignal(reply_data);
    myreply1->deleteLater(); //删除该网络请求
}
