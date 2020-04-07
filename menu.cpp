#include "mainwindow.h"
#include "ui_mainwindow.h"


//帮助 关于无人船
void MainWindow::on_unmannedShip_triggered()
{
    //QStringLiteral  QString::fromLocal8Bit QString::fromUtf8
    //出现中文乱码时，上面这三种方法都可以尝试下
    QMessageBox msgBox; //创建QMessageBox实例 msgBox
    msgBox.setIconPixmap(QPixmap("://awesome.png")); //设置图标 此为傲宋的图标
    //msgBox.setWindowTitle(QStringLiteral("关于无人船")); //设置窗口标题
    msgBox.setWindowModality(Qt::ApplicationModal);//设置窗口模态
   // msgBox.setText(QStringLiteral("无人船"));  //设置文本标题
    //msgBox.setInformativeText(QStringLiteral("版权所有:浙江傲宋智能科技有限责任公司"));  //设置文本内容
    msgBox.setStandardButtons(QMessageBox::Close);  //向信息框中添加按钮
    msgBox.setDefaultButton(QMessageBox::Close);  //设置默认选择的按钮

    msgBox.setWindowTitle(QString::fromLocal8Bit("关于无人船")); //设置窗口标题
    msgBox.setText(QString::fromLocal8Bit("无人船"));  //设置文本标题
    msgBox.setInformativeText(QString::fromLocal8Bit("版权所有:浙江傲宋智能科技有限责任公司"));  //设置文本内容
    msgBox.exec();  //程序悬挂在这 等待用户响应
}

//联系我们 傲宋官网
void MainWindow::on_aboutAwesome_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.aszn.org.cn/")); //跳转到外部网站 此为傲宋官网
}
//航行数据 登录
void MainWindow::on_loginAction_triggered()
{
    m_loginWindow->clearParam();
    m_loginWindow->exec();
    QString accessId;
    m_loginWindow->getTokenFromLogin(accessId);
    if (accessId.isEmpty())
    {
        return ;
    }
    OpenNetStream::getInstance()->setAccessToken(accessId);
    const char* pTokenExpireTime = OpenNetStream::getInstance()->getTokenExpireTime();
    if (pTokenExpireTime != NULL)
    {
        qDebug() << __LINE__ << __FUNCTION__ << "Token Expire Time:" << pTokenExpireTime;
    }
    const char* pAreaDomain = OpenNetStream::getInstance()->getAreaDomain();
    if (pAreaDomain != NULL)
    {
        qDebug() << __LINE__ << __FUNCTION__ << "Platform Domain:" << pAreaDomain;
    }

    //on_deviceListButton_clicked();
    int lPageStart = LocalSetting::getInstance()->PageStart();
    int lPageSize = LocalSetting::getInstance()->PageSize();
    int pageStart = 0;
    if(lPageStart > 0)
    {
        pageStart = lPageStart;
    }
    int pageSize = 1000;
    if (lPageSize > 0)
    {
        pageSize = lPageSize;
    }
    void* pBuf = nullptr;
    int length = 0;

    int iRet = OpenNetStream::getInstance()->getDevListEx(pageStart, pageSize, &pBuf, &length);
    if (iRet != OPEN_SDK_NOERROR)
    {
        this->showErrInfo(tr("Get Device List failed!"));
        return ;
    }
    QString json;
    if (LocalSetting::getInstance()->dataUtf8())
    {
        json = static_cast<char*>(pBuf);
    }
    else
    {
        json = QString::fromLocal8Bit(static_cast<char*>(pBuf));
    }

    OpenNetStream::getInstance()->freeData(pBuf);
    Json::Reader reader;
    Json::Value	value;
    QByteArray jsonByte = json.toUtf8();

    m_deviceTableModel->setCameraModel(jsonByte);//camera model
    /*
    DeviceTableViewInfo stDeviceInfo;
    stDeviceInfo.strSubserial = m_deviceTableModel->getSerial(0);
    stDeviceInfo.iChannelNo = m_deviceTableModel->getCameraNo(0);
    stDeviceInfo.bEncrypt = m_deviceTableModel->getIsEncrypt(0);
    stDeviceInfo.iVideoLevel = m_deviceTableModel->getVideoLevel(0);

    bool bEncrypt = stDeviceInfo.bEncrypt;
    QString devSerial = stDeviceInfo.strSubserial;
    int iChannelNo = stDeviceInfo.iChannelNo;

    m_devSerial = devSerial;
    m_Channel = iChannelNo;

    QString safekey;
    safekey.clear();
    if(bEncrypt)
    {
        bool ok;
        safekey = QInputDialog::getText(this, tr("Input"), tr("Please Input the verification code"), QLineEdit::Password, "", &ok);
        if (ok == false)
        {
            return ;
        }
    }
    */
}
//航行数据 播放
void MainWindow::on_startPlayAction_triggered()
{
    start_play();
}
