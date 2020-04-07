#include "mainwindow.h"
#include "ui_mainwindow.h"
httpCommunication::httpCommunication(QObject *parent) : QObject(parent)
{

}

void MainWindow::set_http()
{  
//    myTimer1 = new QTimer(this);
//    if(false == myTimer1->isActive())
//    {
//        myTimer1->start(800);//开启定时器
//    }
    //这个定时器myTimer2用来收gps,油门开度,偏航角等数据

//    manager2 = new QNetworkAccessManager(this); //新建一个网络协调
    //这个manager2在QTimer的线程里，只初始化一次的话，不能使用。所以每次定时器时间到了都得分配内存，所以要手动删除
    manager3 = new QNetworkAccessManager(this); //新建一个网络协调
    manager4 = new QNetworkAccessManager(this); //新建一个网络协调
    manager5 = new QNetworkAccessManager(this); //新建一个网络协调

    cout << "manager initialization success.";
    myTimer2 = new QTimer(this);
    if(false == myTimer2->isActive())
    {
         myTimer2->start(800);//开启定时器
    }
//    thread = new QThread(this);
//    myT = new httpThread();
//    myT->moveToThread(thread);
//    thread->start();
    //打印线程号
//    cout << "*****************************Main thread id:"<<  QThread::currentThreadId()<<"*****************************";
    //定时器时间到了触发线程处理函数 这是测试用的，后期注释掉
//    connect(myTimer1,&QTimer::timeout,myT,&httpThread::threadDealFun);
    //摇杆变动触发线程处理函数
//    connect(this,&MainWindow::myJoyAixChanged,myT,&httpThread::threadDealFun);

    //定时器时间到了触发线程处理函数  收gps,油门开度,偏航角等数据
//    connect(myTimer2,&QTimer::timeout,myT,&httpThread::threadDealFun);


    //gps 油门开度 偏航角等数据
    connect(myTimer2,&QTimer::timeout,this,&MainWindow::httpCom5);
    //按钮
//    connect(this,&MainWindow::myJoyButtonPressed,this,&MainWindow::httpCom2);
    //摇杆
//    connect(this,&MainWindow::myJoyAixChanged,this,&MainWindow::httpCom1);
    //点击窗口右上角的关闭时，同时关闭子线程
//    connect(this,&QMainWindow::destroyed,this,&MainWindow::destoryThread);

    //线程处理函数处理完成后发出信号，然后执行相应的槽函数
//    connect(myT,&httpThread::myThreadEndSignal,this,&MainWindow::dealEndThreadSlot);

    //在网络请求应答处理槽中发射这个信号 通过信号与槽的方式把myreply->readAll()中的内容传递出去
//    connect(myT,&httpThread::showMyreplyMessageSignal,this,&MainWindow::showMyreplyMessageSlot);
    //已经从服务器端获取了gps,油门开度,偏航角等数据,但这个数据需要进行类型转换
//    connect(myT,&httpThread::dealDataSignal,this,&MainWindow::dealDataSlot);
}
//摇杆
void MainWindow::httpCom1()
{   
    manager = new QNetworkAccessManager(this); //新建一个网络协调
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器

    QByteArray postArray;

    //获取摇杆的状态，并向服务器发送指令
    //摇杆共9个状态，分别为上、下、左、右、左上、左下、右上、右下、中
    //x_direction  y_direction

    switch(x_direction)
    {
        case -1:
            switch(y_direction)
            {
                case -1:
                    postArray.append("direction=4");//右下
                    postArray.append(boat_id);
                    break;
                case 0:
                    postArray.append("direction=3");//右
                    postArray.append(boat_id);
                    break;
                case 1:
                    postArray.append("direction=2");//右上
                    postArray.append(boat_id);
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append(boat_id);
                    break;
            }
            break;
        case 0:
            switch(y_direction)
            {
                case -1:
                    postArray.append("direction=5");//下
                    postArray.append(boat_id);
                    break;
                case 0:
                    postArray.append("direction=0");//中
                    postArray.append(boat_id);
                    break;
                case 1:
                    postArray.append("direction=1");//上
                    postArray.append(boat_id);
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append(boat_id);
                    break;
            }
            break;
        case 1:
            switch(y_direction)
            {
                case -1:
                    postArray.append("direction=6");//左下
                    postArray.append(boat_id);
                    break;
                case 0:
                    postArray.append("direction=7");//左
                    postArray.append(boat_id);
                    break;
                case 1:
                    postArray.append("direction=8");//左上
                    postArray.append(boat_id);
                    break;
                default:
                    postArray.append("direction=0");//中
                    postArray.append(boat_id);
                    break;
            }
            break;
        default:
            postArray.append("direction=0");//中
            postArray.append(boat_id);
            break;
    }
    cout<< "x_direction:"<< x_direction;
    cout<< "y_direction:"<< y_direction;

    QNetworkRequest request(QUrl("http://47.100.92.173:10000/move"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply = manager->post(request,postArray);

}
//按钮
void MainWindow::httpCom2()
{
    manager = new QNetworkAccessManager(this); //新建一个网络协调
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    //获取按钮的标号，并向服务器发送指令
    switch(buttonE.button)
    {
        case 7:
            postArray.append("direction=1");//上
            postArray.append(boat_id);
            break;
        case 0:
            postArray.append("direction=2");//下
            postArray.append(boat_id);
            break;
        case 4:
            postArray.append("direction=3");//左
            postArray.append(boat_id);
            break;
        case 1:
            postArray.append("direction=4");//右
            postArray.append(boat_id);
            break;
        default:
            postArray.append("direction=5");//停止
            postArray.append(boat_id);
            break;
    }

    QNetworkRequest request(QUrl("http://47.100.92.173:10000/move"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply = manager->post(request,postArray);


}
void MainWindow::replyFinishedSlot(QNetworkReply*)
{
    if(reply != nullptr)
    {
        ui->instruction_status->setText(reply->readAll());//读取所有请求的内容，显示在textBrowser中
        reply->deleteLater(); //删除该网络请求
    }
}
//gps 油门开度 偏航角等数据
void MainWindow::httpCom5()
{
    manager2 = new QNetworkAccessManager(this); //新建一个网络协调
    connect(manager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot2(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    postArray.append("&id=wrc");//发送给服务器端的指令
    QNetworkRequest request(QUrl("http://47.100.92.173:10000/gpsCheck"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply2 = manager2->post(request,postArray);    
}
void MainWindow::replyFinishedSlot2(QNetworkReply *)
{
    if(reply2 != nullptr)
	{ 
        QString reply_data = QString(reply2->readAll());
        cout << reply_data;
        solve_sensor(HexStringToByteArray(reply_data));
        reply2->deleteLater(); //删除该网络请求
        manager2->deleteLater();//删除网络管理，这个manager2在QTimer的线程里，只初始化一次的话，不能使用。所以每次定时器时间到了都得分配内存，所以要手动删除
	}
}
//ui界面上5个按钮控制小船
void MainWindow::httpCom6()
{
    isSendButton = 0;//发送路径按钮未按下
    isAutopilot = 0; //手动
//    manager4 = new QNetworkAccessManager(this); //新建一个网络协调
    connect(manager4,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot4(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    QString stringPacket;

    QString str_isSendButton = QString("%1").arg(isSendButton&0xFF,2,16,QLatin1Char('0'));
    QString str_buttonNumber = QString("%1").arg(buttonNumber&0xFF,2,16,QLatin1Char('0'));
    stringPacket.append(str_isSendButton);
    stringPacket.append(str_buttonNumber);

    postArray.append(QString("direction=%1").arg(stringPacket));//方向
    postArray.append("&id=wrc");//id
    QNetworkRequest request(QUrl("http://47.100.92.173:10000/move"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply4 = manager4->post(request,postArray);
}
void MainWindow::replyFinishedSlot4(QNetworkReply *)
{
    if(reply4 != nullptr)
    {
        ui->instruction_status->append(QString(reply4->readAll()));
        reply4->deleteLater(); //删除该网络请求
    }
}
void MainWindow::destoryThread()
{
    //停止进程
    thread->quit();
    //等待资源回收完成
    thread->wait();
    //回收指针资源
    delete myT;
}

//处理线程结束的槽函数
void MainWindow::dealEndThreadSlot()
{
//     ui->instruction_status->setText(reply->readAll());//读取所有请求的内容，显示在textBrowser中
}
//显示myreply->readAll()中读出来的信息
void MainWindow::showMyreplyMessageSlot(QString  temp)
{
    QString myMessage = temp;
    ui->instruction_status->setText(myMessage);//读取所有请求的内容，显示在textBrowser中
}
//对已经从服务器端获取的gps,油门开度,偏航角等数据进行类型转换等操作
void MainWindow::dealDataSlot(QString temp)
{
    solve_sensor(HexStringToByteArray(temp));
}
