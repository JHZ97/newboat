#include <QWebChannel>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

static double pi =  3.1415926535897932384626;
static double a = 6378245.0;
static double ee = 0.00669342162296594323;
static double x_Pi = 3.14159265358979324 * 3000.0 / 180.0;


void GPS::GPS_to_BD()
{
    gps84_To_Gcj02(lat,lon);
    gcj02_To_Bd09(lat,lon);
}

void GPS::BD_to_GPS()
{
    Bd09_To_gcj02(lat,lon);
    gcj02_To_gps84(lat,lon);
}

double GPS::transformLat(double x, double y) {
        double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y
                + 0.2 * sqrt(abs(x));
        ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
        ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
        ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
        return ret;
    }

double GPS::transformLon(double x, double y) {
        double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1
                * sqrt(abs(x));
        ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
        ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
        ret += (150.0 *sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0
                * pi)) * 2.0 / 3.0;
        return ret;
    }

void GPS::gps84_To_Gcj02(double& lat, double& lon) {

        double dLat = transformLat(lon - 105.0, lat - 35.0);
        double dLon = transformLon(lon - 105.0, lat - 35.0);
        double radLat = lat / 180.0 * pi;
        double magic =sin(radLat);
        magic = 1 - ee * magic * magic;
        double sqrtMagic =sqrt(magic);
        dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
        dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
        lat = lat + dLat;
        lon = lon + dLon;

    }

void GPS::gcj02_To_Bd09(double& gg_lat, double& gg_lon) {
        double x = gg_lon, y = gg_lat;
        double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_Pi);
        double theta = atan2(y, x) + 0.000003 *cos(x * x_Pi);
        gg_lon = z *cos(theta) + 0.0065;
        gg_lat = z *sin(theta) + 0.006;
}

void GPS::Bd09_To_gcj02(double& gg_lat, double& gg_lon) {
        double x = gg_lon - 0.0065;
        double y = gg_lat - 0.006;
        double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_Pi);
        double theta = atan2(y, x) - 0.000003 *cos(x * x_Pi);
        gg_lon = z *cos(theta);
        gg_lat = z *sin(theta);
}

void GPS::gcj02_To_gps84(double &lat, double &lon)
{
    double dLat = transformLat(lon - 105.0, lat - 35.0);
    double dLon = transformLon(lon - 105.0, lat - 35.0);
    double radLat = lat / 180.0 * pi;
    double magic =sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic =sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
    lat -= dLat;
    lon -= dLon;
}

void MainWindow::loadFinished(bool a)
{
    if (a == true)
    {
        loadfished = true;
    }
}


//120.161567  30.332146


void MainWindow::setGPS(double lon,double lat )
{
    auto gps=GPS(lon,lat);
    gps.GPS_to_BD();
    if (lon==0||lat==0) return;
    QString command = QString("theLocation(%1,%2)").arg(QString::number(gps.lon, 'f', 6)).arg(QString::number(gps.lat, 'f', 6));
    //    QString command = QString("theLocation(120.138288,30.477319)");//恒泽
    //    QString command = QString("theLocation(120.075278,30.277765)");//西溪
    ui->webEngine->page()->runJavaScript(command);
}

QByteArray GPS_to_QByte(GPS gps);
QByteArray sendPath(QVector<GPS> &path);

void MainWindow::getPath(QString path_baidu){
    QString points = path_baidu.mid(1,path_baidu.length()-2);
    auto list = points.split(",",QString::SkipEmptyParts);
    QVector<GPS> path;
    for(int i =0;i<list.length()-1;i+=2)
    {
        GPS temp(list[i].toDouble(),list[i+1].toDouble());
        temp.BD_to_GPS();
        path.append(temp);
    }
    QByteArray senddata = sendPath(path);
    //compute_data(senddata);
    //send_message(senddata);
    ui->status->append("Send"+QString::number(path.length()) +"GPS:");
    for(auto &g:path)
    {
        ui->status->append("("+QString::number(g.lat) +","+QString::number(g.lon)+")");
    }
    //在这里写向http发送数据的函数
//    manager3 = new QNetworkAccessManager(this); //新建一个网络协调
    connect(manager3,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot3(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    QNetworkRequest request;
    //第二个包 包含点的个数和每个点的gps数值
    QString stringPacket;
    //组包 点的个数
    int pointNumber = list.length()/2;
    QString str_pointNumber = QString("%1").arg(pointNumber&0xFF,2,16,QLatin1Char('0'));
    stringPacket.append(str_pointNumber);
    //组包 每个点的gps数据
    for(auto &g:path)
    {
        //纬度
       int int_lat = g.lat*1000000;
       QString str_lat = QString("%1").arg(int_lat&0xFFFFFFFF,8,16,QLatin1Char('0'));
       //经度
       int int_lon = g.lon*1000000;
       QString str_lon = QString("%1").arg(int_lon&0xFFFFFFFF,8,16,QLatin1Char('0'));

       stringPacket.append(str_lat);//纬度
       stringPacket.append(str_lon);//经度
    }
    cout<<"set gps:"<<stringPacket;
    postArray.append(QString("msg=%1").arg(stringPacket));
    postArray.append("&id=wrc"); //id
    request.setUrl(QUrl("http://47.100.92.173:10000/allMsg"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply3 = manager3->post(request,postArray);
}

QByteArray GPS_to_QByte(GPS gps)
{
    QByteArray lngdata,latdata;
    lngdata.resize(4);
    latdata.resize(4);
    unsigned int lng = gps.lon*10000000;
    unsigned int lat = gps.lat*10000000;
    lngdata[3]= lng & 0xff;
    lngdata[2]= (lng>>8)&0xff;
    lngdata[1]= (lng>>16)&0xff;
    lngdata[0]= (lng>>24)&0xff;
    latdata[3]= lat & 0xff;
    latdata[2]= (lat>>8)&0xff;
    latdata[1]= (lat>>16)&0xff;
    latdata[0]= (lat>>24)&0xff;
    lngdata.prepend(latdata);
    return lngdata;
}

QByteArray sendPath(QVector<GPS> &path)
{
     QByteArray data;
     data.resize(2);
     data[0]=0x02;
     data[1]=0x22;
     for(int i=0;i<path.length();i++)
     {
         data.append(GPS_to_QByte(path[i]));
     }
     return data;
}

void MainWindow::showGPS(double lng,double lat)
{
    GPS gps(lng,lat);

    gps.BD_to_GPS();
    cout << gps.lon;
    cout << gps.lat;
    ui->terminal_lon->setText(QString::number(gps.lon,'f',6));
    ui->terminal_lat->setText(QString::number(gps.lat,'f',6));
}

//设置路径
void MainWindow::on_btn_set_terminal_clicked()
{
    if(ui->btn_set_terminal->text()==QStringLiteral("设置路径"))
    {
        QString command = QString("path_model()");
        ui->webEngine->page()->runJavaScript(command);
        ui->btn_path_reset->setText(QStringLiteral("取消设置"));
        ui->btn_set_terminal->setText(QStringLiteral("发送路径"));
    }
    else if (ui->btn_set_terminal->text()==QStringLiteral("发送路径"))
    {     
        isSendButton =1;//发送路径按钮按下
        buttonNumber =0;//发送停止信号 在设置路径的时候不要让电机转动
        //在这里写向http发送数据的函数
//        manager5 = new QNetworkAccessManager(this); //新建一个网络协调
        connect(manager5,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot5(QNetworkReply*))); //网络请求完成的信号处理
        QByteArray postArray;
        QNetworkRequest request;
        //第一个包
        QString stringPacket;
        QString str_isSendButton = QString("%1").arg(isSendButton&0xFF,2,16,QLatin1Char('0'));
        QString str_buttonNumber = QString("%1").arg(buttonNumber&0xFF,2,16,QLatin1Char('0'));
        stringPacket.append(str_isSendButton);
        stringPacket.append(str_buttonNumber);

        postArray.append(QString("direction=%1").arg(stringPacket));
        postArray.append("&id=wrc"); //id
        request.setUrl(QUrl("http://47.100.92.173:10000/move"));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
        reply5 = manager5->post(request,postArray);

        QString command = QString("send_path()");
        ui->webEngine->page()->runJavaScript(command);
        ui->btn_set_terminal->setText(QStringLiteral("设置路径"));
        ui->btn_path_reset->setText(QStringLiteral("清除路径"));
    }
}
//清除路径
void MainWindow::on_btn_path_reset_clicked()
{
    if(ui->btn_path_reset->text()==QStringLiteral("清除路径"))
        {
        QString command = QString("path_clean()");
        ui->webEngine->page()->runJavaScript(command);
    }
    else if (ui->btn_path_reset->text()==QStringLiteral("取消设置"))
    {
        QString command = QString("quit_path_set()");
        ui->webEngine->page()->runJavaScript(command);
        ui->btn_set_terminal->setText(QStringLiteral("设置路径"));
        ui->btn_path_reset->setText(QStringLiteral("清除路径"));
    }
}
//设置地图
void MainWindow::set_map()
{
//    QWebChannel* channel=new QWebChannel(this);
    channel=new QWebChannel(this);
    channel->registerObject(QString("passId"),this);
    ui->webEngine->page()->setWebChannel(channel);
    QUrl url = QUrl::fromLocalFile(QDir::currentPath()+"\\map_online.htm");
    //QUrl url = QUrl::fromLocalFile("C:\\Users\\huangkw\\Desktop\\boat\\map_online.htm");
    ui->webEngine->page()->load(url);
    connect(ui->webEngine, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    //为了消除ASSERT:"m_signalArgumentTypes.contains(object->metaObject())"in file错误
    connect(this,&MainWindow::destroyed,this,&MainWindow::deregisterSlot);
}
void MainWindow::deregisterSlot()
{
    channel->deregisterObject(this);
}
void MainWindow::mytestfunction1()
{
    cout << "************************";
    cout << "************************";
}
//发送设置的路径信息给服务器端
void MainWindow::replyFinishedSlot3(QNetworkReply *)
{
    if(reply3 != nullptr)
    {
        ui->instruction_status->append(QString(reply3->readAll()));
        //    ui->instruction_status->setText(reply3->readAll());//读取所有请求的内容，显示在textBrowser中
        reply3->deleteLater(); //删除该网络请求
    }
}
void MainWindow::replyFinishedSlot5(QNetworkReply *)
{
    if(reply5 != nullptr)
    {
        ui->instruction_status->append(QString(reply5->readAll()));       
        reply5->deleteLater(); //删除该网络请求
    }
}
