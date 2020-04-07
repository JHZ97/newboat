#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMainWindow>
#include <QWebEngineView>
#include <QDebug>
#include <QInputDialog>
#include <QDir>
#include <QTableView>
#include <QMessageBox>
#include <QJoysticks.h>
#include <QTimer>
#include "cameramodel.h"
#include "loginwindow.h"
#include "util/json/json.h"
#include "opennetstream.h"
#include "localsetting.h"
#include "send_data.h"
#include "GPS.h"
#include "http_manager.h"
#include <QThread>
#include "httpcommunication.h"
#define cout qDebug() << "[" << __FILE__ << ":" <<__LINE__ << "]"

#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include "httpthread.h"
#include <QMetaType>
typedef struct tagDeviceTableViewInfo
{
    QString strSubserial;
    int iChannelNo;
    bool bEncrypt;
    int  iVideoLevel;
}DeviceTableViewInfo;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool isJoystick;
    static void __stdcall messageHandler(const char* szSessionId, unsigned int iMsgType, unsigned int iErrorCode,const char *pMessageInfo, void *pUser);
public slots:
    //要定义为public slots jscai'neng才能够调用Qt中的函数
    void getCoordinate(QString lon,QString lat);
    void showGPS(double,double);
    void getPath(QString);
private:
    Ui::MainWindow *ui;
    QSerialPort *serial=nullptr;
     QJoysticks* joy;
    LoginWindow * m_loginWindow;
    bool loadfished;
    QMultiMap<QByteArray,serial_send_obj*> send_map;
    //! [3] user data
    QString m_sessionId;
    QString m_devSerial;
    QString myReplyMessage;
    int     m_Channel;
    CameraModel*    m_deviceTableModel;

    //! [3]  some flage
    bool m_bSync;
    bool m_bRealPlayStarted;
    bool m_bPlayBackStarted;
    bool m_bPlayBackFinished;
    bool m_bVideoDownload;
    bool m_bDeviceVideoDownload;
    bool m_bCloudPlayBack;           //云存储回放标志
    int  m_iFileSource;
    int  m_iSuportQuickPlayWay;
    QString m_videoPath;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkAccessManager *manager2;
    QNetworkReply *reply2;

    QNetworkAccessManager *manager3;
    QNetworkReply *reply3;

    QNetworkAccessManager *manager4;
    QNetworkReply *reply4;

    QNetworkAccessManager *manager5;
    QNetworkReply *reply5;

    QNetworkAccessManager *manager6;
    QNetworkReply *reply6;

    QJoystickAxisEvent axisE;
    QJoystickButtonEvent buttonE;
    int x_direction;
    int y_direction;
//    double x_direction1;
//    double y_direction1;
//    bool x_bool;
//    bool y_bool;   

    http_manager* net_thread;
    QString boat_id;
    //油门开度
    int leftMotor;
    int rightMotor;
    //偏航角
    int yawAngleValue;
    //电池电量
    double batteryPower = 0;
    //自检
    int selfCheck = 0;
    //hcm365
    int hcm365 = 0;//hcm365状态
    //毫米波雷达
    int millimeterWaveRadar = 0;//毫米波雷达状态
    //gps
    int gpsStatus = 0;
    //判断是人工驾驶还是自动驾驶
    int isAutopilot = 0;
    //QWebChannel
    QWebChannel *channel;
    //线程
    QThread *thread;
    httpThread *myT;
    QTimer *myTimer1;
    QTimer *myTimer2;
    //ui界面上5个按钮控制小船
    int buttonNumber;
    int buttonautoRepeatInterval=150;
    //发送路径按钮是否被按下
    int isSendButton = 0;
    //收垃圾控制
    int colGarbageMode = 0;//2自动收垃圾 1手动收垃圾 0停止收垃圾
    int leftServoSliderValue=0;
    int rightServoSliderValue=0;
    int pumpingMotorSliderValue=0;
    //手动模式下 每隔一段时间获取滑块的当前值 发送给服务器
    QTimer *myTimer3;
signals:
    void frame_ready(QByteArray);
    void insRecordFile(const QByteArray& messageInfo);
    void insPlayException(unsigned errorCode, const QByteArray& messageInfo);
    void insPlayBackEnd(unsigned errorCode);
    void insPlayReconnectException(unsigned errorCode, const QByteArray& messageInfo);
    void insRecordSearchFailed(unsigned errorCode, const QByteArray& messageInfo);
    void insPlayAlarm(QString szCameraId, QString szContent, QString szAlarmTime);
    void insPlayPush(QString szDesc, QString szContent, QString szDetail);
    void insPlayPushAlarm(QString szDesc, QString szContent, QString szDetail);
    void insPlayStart();
    void insPlayStop();
    void insDownloadCloudFileEnd(unsigned errorCode);
    void insDownloadCloudFileException(unsigned errorCode, const QByteArray& messageInfo);
    void insStopDownloadCloudFile();
    //自定义的按钮按下信号
    void myJoyButtonPressed();
    //自定义的摇杆摇动信号
    void myJoyAixChanged();

    //! [3]
private slots:
    void set_serial();
    void set_joystick();
    void set_vodeo();
    void set_map();
    void solve_joy(joyinfoex_tag);
    void on_btn_openConsole_clicked();
    void on_btn_scan_clicked();
    void on_loginButton_clicked();
    void on_btn_set_terminal_clicked();
    void on_btn_path_reset_clicked();
//    void showGPS(double,double);
    void setGPS(double,double);
//    void getPath(QString);
    void loadFinished(bool);
    void showErrInfo(QString);
    int start_play();
    void JoyAixChanged(const QJoystickAxisEvent);
    void JoyButtonChanged(const QJoystickButtonEvent);
    void on_start_paly_clicked();
    void send_data(QByteArray a,const QString&);
    void readData();
    void frame_devide(QByteArray buf);
    void frame_process(QByteArray buf);

   // void send_finished();
    void send_failed(serial_send_obj*);
    //http通信
    void replyFinishedSlot(QNetworkReply*); //网络请求应答处理
    void replyFinishedSlot2(QNetworkReply*); //网络请求应答处理
    void replyFinishedSlot3(QNetworkReply*); //网络请求应答处理
    void replyFinishedSlot4(QNetworkReply*); //网络请求应答处理
    void replyFinishedSlot5(QNetworkReply*); //网络请求应答处理
    void replyFinishedSlot6(QNetworkReply*); //网络请求应答处理
    void set_http();
    void httpCom1();
    void httpCom2();
    void httpCom5();
    void httpCom6();
    void httpCom7();
    //hkw的http代码
    void set_net();
    QByteArray HexStringToByteArray(QString);
    void net_reply(QNetworkReply *reply);
    void on_btn_http_clicked();
    void solve_sensor(QByteArray);
    int hex2int(char c1 ,char c2 ,char c3,char c4);
    int hex2int1(char c1);
    int hex2int2(char c1,char c2);
    short hex2short(char c1,char c2);

    //菜单栏menu的槽
    void on_aboutAwesome_triggered();//联系我们 傲宋官网
    void on_unmannedShip_triggered();//帮助 关于无人船
    void on_loginAction_triggered();//航行数据 登录
    void on_startPlayAction_triggered();//航行数据 播放

    //测试用
    void mytestfunction1();
    //QWebchannel deregisterObject槽函数
    void deregisterSlot();
    //线程
    void destoryThread();//关闭线程的槽函数
    void dealEndThreadSlot();//处理线程结束的槽函数
    void showMyreplyMessageSlot(QString  temp);//显示myreply->readAll()中读出来的信息
    void dealDataSlot(QString temp);//对已经从服务器端获取的gps,油门开度,偏航角等数据进行类型转换等操作
    void on_buttonStop_0_pressed();
    void on_buttonFront_1_pressed();
    void on_buttonBack_5_pressed();
    void on_buttonLeft_7_pressed();
    void on_buttonRight_3_pressed();
    void on_buttonStop_9_clicked();
    //收垃圾控制
    void setColGarbage();//初始化控制收垃圾的三个滑块 分别为左舵机、右舵机、抽水电机
    void on_collectGarbageButtonStop_clicked();//停止收垃圾 collectGarbageButtonStop按钮
    void on_autoCollectGarbageButton_clicked();//收垃圾 自动模式 autoCollectGarbageButton
    void on_manualCollectGarbageButton_clicked();//收垃圾 手动模式 manualCollectGarbageButton按钮
    //三个滑块中任意一个值发生变化，都会触发下面的槽信号 在槽信号中设置为手动收垃圾模式，并开启定时器，每个一段时间读一下滑块当前的值，发送给服务器
    void dealSilderValueChangedSlot(int);
    void dealTimeoutSlot();
};
#endif // MAINWINDOW_H
