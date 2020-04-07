#include "mainwindow.h"
#include "ui_mainwindow.h"




QByteArray compute_data(QByteArray a);



void MainWindow::send_data(QByteArray a,const QString& hint)
{
    if(serial==nullptr||(!serial->isOpen()))
    {
        QString dlgTitle = QStringLiteral("失败");
        QString strInfo =QStringLiteral("串口")+ui->comboBox_serialPort->currentText()+QStringLiteral("未打开");
        QMessageBox::warning(this,dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    int interval = ui->spinBox_delay->value();
    serial_send_obj* serial_send = new serial_send_obj(serial,a,interval,hint);
    connect(serial_send,&serial_send_obj::fail_send,this,&MainWindow::send_failed);
    serial_send->send();
    send_map.insert(serial_send->data,serial_send);
}

void MainWindow::send_failed(serial_send_obj* a)
{
    ui->status->append(a->hint+QString("fail send ")+a->data.toHex());
    send_map.remove(a->data,a);
    delete a;
}


QByteArray compute_data(QByteArray a)
{
    QByteArray header;
    header.resize(2);
    header[0]=0xa5;
    header[1]=0x5a;
    QByteArray tailer(1,0xaa);
    int len = a.size();
    a.prepend(char(len+3));
    char check=0;
    for(char c:a)
    {
        check += c;
    }
    a.append(check);
    a.append(tailer);
    a.prepend(header);
    return a;
}

serial_send_obj::serial_send_obj(QSerialPort* serial, QByteArray a,int interval,const QString& hint):serial(serial),data(compute_data(a)),interval(interval),hint(hint),timer(new QTimer)
{}

void serial_send_obj::send()
{
    serial->write(data);//发送到串口
    count++;
    timer->setInterval(interval);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
}

void serial_send_obj::timer_timeout()
{
   if (count>3)
        {
            //w->ui->status->append("Message \"" +sMessage.toHex() +"\" is not received!");
            timer->stop();
            emit fail_send(this);
        }
   else
        {
            serial->write(data);
            count++;
            //ui->status->append("Send"+QString::number(count));
        }
    }

void serial_send_obj::sucess_send()
{
    timer->stop();
    delete this;
}
