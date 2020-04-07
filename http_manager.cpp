#include <http_manager.h>
#include "mainwindow.h"
http_manager::http_manager(QString url,int interval):url(url),interval(interval),manager(new QNetworkAccessManager),timer(new QTimer)
{
    //打印线程号
//    cout << "Main thread number:" << QThread::currentThread();
    connect(manager,&QNetworkAccessManager::finished,this,&http_manager::reply);
    //构建请求对象
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    timer->setInterval(interval);
}


void http_manager::run()
{
    //打印线程号
//    cout << "Child thread number:" << QThread::currentThread();
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(send_net()));
}

void http_manager::send_net()
{
     manager->post(request,"id=wrc");//"id=wrc"用来识别船的id
}

void http_manager::set_para(QString url, int inter)
{
    cout << url;
    cout << inter;
    timer->stop();
    timer->setInterval(inter);
    request.setUrl(QUrl(url));
    timer->start();
}

void http_manager::reply(QNetworkReply* reply)
{
    emit recv_reply(reply);
}
