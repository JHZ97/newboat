#ifndef HTTP_THREAD_H
#define HTTP_THREAD_H
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class http_manager:public QObject
{
    Q_OBJECT
public:
    http_manager(QString ,int);
    void set_para(QString,int);
    ~http_manager() override{delete manager;}
    void run();//线程处理函数
signals:
    void recv_reply(QNetworkReply*);
private:
    QTimer* timer;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString url;
    int interval;
private slots:
    void reply(QNetworkReply*);
    void send_net();
};


#endif // HTTP_THREAD_H
