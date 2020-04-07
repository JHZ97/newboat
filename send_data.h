#include <QTimer>
#include <QSerialPort>
#ifndef SEND_DATA_H
#define SEND_DATA_H


class serial_send_obj:public QObject
{
     Q_OBJECT
signals:
void fail_send(serial_send_obj*);

public:
   QByteArray data;
   QString hint;
   serial_send_obj(QSerialPort* serial, QByteArray a,int interval,const QString& hint);
   ~serial_send_obj()
   {delete timer;}
   // void run() override;
   void send();
   void sucess_send();
private:
    QSerialPort* serial;
    QTimer* timer;
    int interval;
    int count=0;
private slots:
    void timer_timeout();
};



#endif // SEND_DATA_H
