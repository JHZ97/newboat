#ifndef HTTPCOMMUNICATION_H
#define HTTPCOMMUNICATION_H

#include <QObject>
#include <QNetworkAccessManager> //协调网络操作
#include <QNetworkReply>         //网络请求应答
#include <QNetworkRequest>       //网络请求
#include <QUrl>
#include <QVariant>
class httpCommunication : public QObject
{
    Q_OBJECT
public:
    explicit httpCommunication(QObject *parent = nullptr);

signals:

private:
//    QNetworkAccessManager *manager;
//    QNetworkReply *reply;

};

#endif // HTTPCOMMUNICATION_H
