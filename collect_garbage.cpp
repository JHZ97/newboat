#include "mainwindow.h"
#include "ui_mainwindow.h"

//初始化控制收垃圾的三个滑块 分别为左舵机、右舵机、抽水电机
void MainWindow::setColGarbage()
{
    int nMin = 0;//最小值
    int nMax = 100;//最大值
    int nSignalStep = 1;//步长
    int curValue = 0;//当前值

    //leftServoSlider
    ui->leftServoSlider->setMinimum(nMin);
    ui->leftServoSlider->setMaximum(nMax);
    ui->leftServoSlider->setValue(curValue);
    //leftServoSpinBox
    ui->leftServoSpinBox->setMinimum(nMin);
    ui->leftServoSpinBox->setMaximum(nMax);
    ui->leftServoSpinBox->setSingleStep(nSignalStep);
    ui->leftServoSpinBox->setValue(curValue);
    //leftServo和leftServoSpinBox的信号与槽
    connect(ui->leftServoSpinBox,SIGNAL(valueChanged(int)),ui->leftServoSlider,SLOT(setValue(int)));
    connect(ui->leftServoSlider,SIGNAL(valueChanged(int)),ui->leftServoSpinBox,SLOT(setValue(int)));

    //rightServoSlider
    ui->rightServoSlider->setMinimum(nMin);
    ui->rightServoSlider->setMaximum(nMax);
    ui->rightServoSlider->setValue(curValue);
    //rightServoSpinBox
    ui->rightServoSpinBox->setMinimum(nMin);
    ui->rightServoSpinBox->setMaximum(nMax);
    ui->rightServoSpinBox->setSingleStep(nSignalStep);
    ui->rightServoSpinBox->setValue(curValue);
    //rightServoSlider和rightServoSpinBox的信号与槽
    connect(ui->rightServoSpinBox,SIGNAL(valueChanged(int)),ui->rightServoSlider,SLOT(setValue(int)));
    connect(ui->rightServoSlider,SIGNAL(valueChanged(int)),ui->rightServoSpinBox,SLOT(setValue(int)));

    //pumpingMotor
    ui->pumpingMotorSlider->setMinimum(nMin);
    ui->pumpingMotorSlider->setMaximum(nMax);
    ui->pumpingMotorSlider->setValue(curValue);
    //pumpingMotorSpinBox
    ui->pumpingMotorSpinBox->setMinimum(nMin);
    ui->pumpingMotorSpinBox->setMaximum(nMax);
    ui->pumpingMotorSpinBox->setSingleStep(nSignalStep);
    ui->pumpingMotorSpinBox->setValue(curValue);
    //pumpingMotorSlider和pumpingMotorSpinBox的信号与槽
    connect(ui->pumpingMotorSpinBox,SIGNAL(valueChanged(int)),ui->pumpingMotorSlider,SLOT(setValue(int)));
    connect(ui->pumpingMotorSlider,SIGNAL(valueChanged(int)),ui->pumpingMotorSpinBox,SLOT(setValue(int)));

    //向服务器发送收垃圾指令的http通信
    manager6 = new QNetworkAccessManager(this); //新建一个网络协调
    //手动模式下 每隔一段时间获取滑块的当前值 发送给服务器
    myTimer3 = new QTimer(this);
    connect(ui->leftServoSlider,SIGNAL(valueChanged(int)),this,SLOT(dealSilderValueChangedSlot(int)));
    connect(ui->rightServoSlider,SIGNAL(valueChanged(int)),this,SLOT(dealSilderValueChangedSlot(int)));
    connect(ui->pumpingMotorSlider,SIGNAL(valueChanged(int)),this,SLOT(dealSilderValueChangedSlot(int)));
    connect(myTimer3,SIGNAL(timeout()),this,SLOT(dealTimeoutSlot()));
}
//收垃圾 自动模式 autoCollectGarbageButton按钮
void MainWindow::on_autoCollectGarbageButton_clicked()
{   
    ui->leftServoSlider->setValue(0);
    ui->rightServoSlider->setValue(0);
    ui->pumpingMotorSlider->setValue(0);
    myTimer3->stop();
    //将colGarbageMode的值和三个slider的值组包后发送给服务器
    colGarbageMode = 2;//自动收垃圾
    leftServoSliderValue = ui->leftServoSlider->value();
    rightServoSliderValue = ui->rightServoSlider->value();
    pumpingMotorSliderValue = ui->pumpingMotorSlider->value();
    httpCom7();
}
//手动收垃圾按钮暂时不用
//收垃圾 手动模式 manualCollectGarbageButton按钮
void MainWindow::on_manualCollectGarbageButton_clicked()
{
//    //将colGarbageMode的值和三个slider的值组包后发送给服务器
//    colGarbageMode = 1;//手动收垃圾
//    leftServoSliderValue = ui->leftServoSlider->value();
//    rightServoSliderValue = ui->rightServoSlider->value();
//    pumpingMotorSliderValue = ui->pumpingMotorSlider->value();
//    httpCom7();
}
//停止收垃圾 collectGarbageButtonStop按钮
void MainWindow::on_collectGarbageButtonStop_clicked()
{
    ui->leftServoSlider->setValue(0);
    ui->rightServoSlider->setValue(0);
    ui->pumpingMotorSlider->setValue(0);
    myTimer3->stop();
    //将colGarbageMode的值和三个slider的值组包后发送给服务器
    colGarbageMode = 0;//停止收垃圾
    leftServoSliderValue = ui->leftServoSlider->value();
    rightServoSliderValue = ui->rightServoSlider->value();
    pumpingMotorSliderValue = ui->pumpingMotorSlider->value();
    httpCom7();
}
//向服务器发送收垃圾的指令
void MainWindow::httpCom7()
{
    connect(manager6,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinishedSlot6(QNetworkReply*))); //网络请求完成的信号处理

    //form格式访问服务器
    QByteArray postArray;
    QString stringPacket;

    QString str_colGarbageMode = QString("%1").arg(colGarbageMode&0xFF,2,16,QLatin1Char('0'));
    QString str_leftServoSliderValue = QString("%1").arg(leftServoSliderValue&0xFF,2,16,QLatin1Char('0'));
    QString str_rightServoSliderValue = QString("%1").arg(rightServoSliderValue&0xFF,2,16,QLatin1Char('0'));
    QString str_pumpingMotorSliderValue = QString("%1").arg(pumpingMotorSliderValue&0xFF,2,16,QLatin1Char('0'));

    stringPacket.append(str_colGarbageMode);
    stringPacket.append(str_leftServoSliderValue);
    stringPacket.append(str_rightServoSliderValue);
    stringPacket.append(str_pumpingMotorSliderValue);

    postArray.append(QString("msg=%1").arg(stringPacket));//方向
    postArray.append("&id=wrc");//id
    QNetworkRequest request(QUrl("http://47.100.92.173:10000/trash"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    reply6 = manager6->post(request,postArray);
}
void MainWindow::replyFinishedSlot6(QNetworkReply *)
{
    if(reply6 !=nullptr)
    {
        QString trashValue = QString(reply6->readAll());
        if(trashValue!="")
        {
            cout <<"trash:"<< trashValue;
        }
        reply6->deleteLater(); //删除该网络请求
    }
}
void MainWindow::dealSilderValueChangedSlot(int)
{
    if(false == myTimer3->isActive())
    {
        myTimer3->start(500);
    }
}
void MainWindow::dealTimeoutSlot()
{
    cout << "manual";
    colGarbageMode = 1;//手动收垃圾
    leftServoSliderValue = ui->leftServoSlider->value();
    rightServoSliderValue = ui->rightServoSlider->value();
    pumpingMotorSliderValue = ui->pumpingMotorSlider->value();
    cout << "leftServoSliderValue:" << leftServoSliderValue;
    cout << "rightServoSliderValue:" << rightServoSliderValue;
    cout << "pumpingMotorSliderValue:" << pumpingMotorSliderValue;
    httpCom7();
}
