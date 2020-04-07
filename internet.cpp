#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::set_net()
{
//    net_thread = new http_manager(ui->ip->text(),ui->interval->value());
    //打印线程号
//    cout << "Main thread number:" << QThread::currentThread();
    net_thread= new http_manager(QString("http://47.100.92.173:10000/gpsCheck"),1000);
    connect(net_thread,SIGNAL(recv_reply(QNetworkReply*)),this,SLOT(net_reply(QNetworkReply*)));
    net_thread->run();
}

QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl ;
    for(int i=0;i<HexString.size();i+=2)
    {
        sl.append(HexString.mid(i,2));      
    }

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);               
            }else{
                cout<<QStringLiteral("非法的16进制字符:")<<s;
            }
        }
    }
//    cout<<ret;
    return ret;
}



void MainWindow::net_reply(QNetworkReply* reply)
{
    if(reply->error()!=QNetworkReply::NoError){
            //处理中的错误信息
            cout<<"reply error:"<<reply->errorString();
        }else{
            //cout<<"raw header:"<<reply->rawHeaderList();

            //获取响应信息
            const QByteArray reply_data=reply->readAll();
            //QString s(reply_data);
//            cout<<"read all:"<<HexStringToByteArray(reply_data).toHex();
            solve_sensor(HexStringToByteArray(reply_data));
    }
}


void MainWindow::on_btn_http_clicked()
{
//    net_thread->set_para(ui->ip->text(),ui->interval->value());
    net_thread->set_para(QString("http://47.100.92.173:10000/gpsCheck"),1000);
}
