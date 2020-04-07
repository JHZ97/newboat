#include "mainwindow.h"
#include "ui_mainwindow.h"


static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

void MainWindow::set_serial()
{
    QString description;
    QString manufacturer;
    QString serialNumber;

    //获取可以用的串口
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

    //输出当前系统可以使用的串口个数
    qDebug() << "Total numbers of ports: " << serialPortInfos.count();

    //将所有可以使用的串口设备添加到ComboBox中
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
       QStringList list;
       description = serialPortInfo.description();
       manufacturer = serialPortInfo.manufacturer();
       serialNumber = serialPortInfo.serialNumber();

       list << serialPortInfo.portName()
            << (!description.isEmpty() ? description : blankString)
            << (!manufacturer.isEmpty() ? manufacturer : blankString)
            << (!serialNumber.isEmpty() ? serialNumber : blankString)
            << serialPortInfo.systemLocation()
            << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
            << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);

       ui->comboBox_serialPort->addItem(list.first(), list);
           }


       //设置波特率
       ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
       ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
       ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
       ui->comboBox_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

       //设置默认值
       ui->comboBox_baudRate->setCurrentIndex(3);
}


void MainWindow::on_btn_openConsole_clicked()
{

    qDebug() << ui->btn_openConsole->text();
    if (ui->btn_openConsole->text() == QStringLiteral("打开串口"))
       {
          serial = new QSerialPort;
          //设置串口名字
           serial->setPortName(ui->comboBox_serialPort->currentText());

           //设置波特率
           serial->setBaudRate(ui->comboBox_baudRate->currentText().toInt());

           //设置数据位
           serial->setDataBits(QSerialPort::Data8);

           //设置奇偶校验位
           serial->setParity(QSerialPort::NoParity);

           //设置停止位
           serial->setStopBits(QSerialPort::OneStop);

           //设置流控
           serial->setFlowControl(QSerialPort::NoFlowControl);

           //打开串口
           if (serial->open(QIODevice::ReadWrite))
           {
               ui->comboBox_baudRate->setEnabled(false);

               ui->comboBox_serialPort->setEnabled(false);

               ui->btn_scan->setEnabled(false);


               ui->btn_openConsole->setText(QStringLiteral("关闭串口"));

               ui->comboBox_com->addItem(QStringLiteral("串口"),0);

               //ui->status->append("Success to open serial "+ui->comboBox_serialPort->currentText());


               //信号与槽函数关联
               connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
               connect(this,&MainWindow::frame_ready,this,&MainWindow::frame_process);
           }
           else
           {
               ui->status->append("Failed to open serial "+ui->comboBox_serialPort->currentText());
               QString dlgTitle = QStringLiteral("失败");
               QString strInfo =QStringLiteral("串口")+ui->comboBox_serialPort->currentText()+QStringLiteral("打开失败");
               //ui->widget->setEnabled(false);
               //ui->btn_path_reset->setEnabled(false);
               //ui->btn_set_terminal->setEnabled(false);
               QMessageBox::warning(this,dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
               serial->deleteLater();
       }
    }
       else
       {
           //关闭串口
           //serial->clear();
           serial->close();
           serial->deleteLater();
           //delete serial;
           //恢复设置功能
           serial = nullptr;
           ui->comboBox_baudRate->setEnabled(true);
           ui->comboBox_serialPort->setEnabled(true);
           ui->btn_scan->setEnabled(true);
           ui->btn_openConsole->setText(QStringLiteral("打开串口"));
       }

   }

void MainWindow::on_btn_scan_clicked()
{
    QString description;
    QString manufacturer;
    QString serialNumber;

    //获取可以用的串口
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

    //输出当前系统可以使用的串口个数
    qDebug() << "Total numbers of ports: " << serialPortInfos.count();

    //清空ComboBox
    ui->comboBox_serialPort->clear();

    //将所有可以使用的串口设备添加到ComboBox中
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
        QStringList list;
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();

        list << serialPortInfo.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << serialPortInfo.systemLocation()
             << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
             << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);

        ui->comboBox_serialPort->addItem(list.first(), list);
    }

}

