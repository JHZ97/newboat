#include "mainwindow.h"
#include "ui_mainwindow.h"



void MainWindow::set_vodeo()
{
    m_deviceTableModel = new CameraModel(this);


    //! [1] init library，alloc session
    OpenNetStream::getInstance()->initLibrary(LocalSetting::getInstance()->authAddress(), LocalSetting::getInstance()->platformAddress(), LocalSetting::getInstance()->appKey());
    QString areaID = LocalSetting::getInstance()->areaId();
    username = LocalSetting::getInstance()->username();
    passwd = LocalSetting::getInstance()->passwd();
    accessId = LocalSetting::getInstance()->accessToken();
    if (!areaID.isEmpty())
    {
        OpenNetStream::getInstance()->setAreaID(areaID);
    }
    QString loginVer = LocalSetting::getInstance()->loginVer();
    if (!loginVer.isEmpty())
    {
        OpenNetStream::getInstance()->setLoginVer(loginVer);
    }
    if (LocalSetting::getInstance()->dataUtf8())
    {
        OpenNetStream::getInstance()->setDataUtf8();
    }

    if (LocalSetting::getInstance()->streamTrans())
    {
        OpenNetStream::getInstance()->setStreamTrans();
    }

    if (LocalSetting::getInstance()->closeP2P())
    {
        OpenNetStream::getInstance()->closeP2P();
    }

    int iP2PMaxNum = LocalSetting::getInstance()->P2PMaxNum();
    if (iP2PMaxNum > 0)
    {
        OpenNetStream::getInstance()->setP2PMaxNum(iP2PMaxNum);
    }

    int iP2PCheckPeriod = LocalSetting::getInstance()->P2PCheckPeriod();
    if (iP2PCheckPeriod > 0)
    {
        OpenNetStream::getInstance()->setP2PCheckPeriod(iP2PCheckPeriod);
    }

    int iP2PExpire = LocalSetting::getInstance()->P2PExpire();
    if (iP2PExpire > 0)
    {
        OpenNetStream::getInstance()->setP2PExpire(iP2PExpire);
    }
    int iLogLevel = LocalSetting::getInstance()->logLevel();
    if (iLogLevel > 0)
    {
        OpenNetStream::getInstance()->setLogLevel(iLogLevel);
    }

    m_sessionId = OpenNetStream::getInstance()->allocSessionEx(MainWindow::messageHandler, this);
   // m_loginWindow = new LoginWindow(this);
    //! [1]
     update_token();
}

void MainWindow::update_token()
{

        QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

        QNetworkRequest request;
        request.setUrl(QUrl("http://47.100.92.173:10010/client/login"));

        QByteArray postData;
        postData.append("accountName=");
        postData.append(username);
        postData.append("&password=");
        postData.append(passwd);
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
//post
        QNetworkReply* reply = accessManager->post(request, postData);
}

void MainWindow::finishedSlot(QNetworkReply *reply)
{
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray bytes = reply->readAll();
         QJsonParseError parseJsonErr;
             QJsonDocument document = QJsonDocument::fromJson(bytes,&parseJsonErr);
             if(!(parseJsonErr.error == QJsonParseError::NoError))
             {
                 qDebug()<<tr("解析json文件错误！");
                 return;
             }
             QJsonObject jsonObject = document.object();
             if(jsonObject.contains(QStringLiteral("result")))
                 {
                     QJsonValue jsonValueList = jsonObject.value(QStringLiteral("result"));
                     //QJsonObject item = jsonValueList.toObject();
                     accessId = jsonValueList["accessToken"].toString();
                     QString appName = "EzvizQtDemo";
                     QString iniFile = "/" + appName + ".ini";
                     QString configFile = QCoreApplication::applicationDirPath()  + iniFile;
                     QSettings settingFile(configFile, QSettings::IniFormat);
                     qDebug() << appName << configFile;
                     settingFile.setValue("OpenAPI/accessToken",accessId);
                 }
         qDebug() << bytes;
     }
     else
     {
         qDebug() << "finishedSlot errors here";
         qDebug( "found error .... code: %d\n", (int)reply->error());
         qDebug(qPrintable(reply->errorString()));
     }
     reply->deleteLater();
     on_loginButton_clicked();
}

/*! @function Login
 * \brief MainWindow::on_loginButton_clicked
 */
void MainWindow::on_loginButton_clicked()
{
    /*
    m_loginWindow->clearParam();
    m_loginWindow->exec();
    QString accessId;
    m_loginWindow->getTokenFromLogin(accessId);
    if (accessId.isEmpty())
    {
        return ;
    }
    */
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
    start_play();
    connect(ui->ptz_up,SIGNAL(pressed()),this,SLOT(send_up()));
    connect(ui->ptz_up,SIGNAL(released()),this,SLOT(stop_up()));
    connect(ui->ptz_down,SIGNAL(pressed()),this,SLOT(send_down()));
    connect(ui->ptz_down,SIGNAL(released()),this,SLOT(stop_down()));
    connect(ui->ptz_left,SIGNAL(pressed()),this,SLOT(send_left()));
    connect(ui->ptz_left,SIGNAL(released()),this,SLOT(stop_left()));
    connect(ui->ptz_right,SIGNAL(pressed()),this,SLOT(send_right()));
    connect(ui->ptz_right,SIGNAL(released()),this,SLOT(stop_right()));
}

int MainWindow::start_play()
{
    DeviceTableViewInfo stDeviceInfo;
    //如果当前账号无设备，则显示提示信息
    if (m_deviceTableModel->rowCount()==0)
    {
        QString dlgTitle = QStringLiteral("设备列表为空失败");
        QString strInfo =QStringLiteral("未检测到设备");
        QMessageBox::warning(this,dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
        return -1;
    }
    stDeviceInfo.strSubserial = m_deviceTableModel->getSerial(0);//3是船上的相机
    stDeviceInfo.iChannelNo = m_deviceTableModel->getCameraNo(0);
    stDeviceInfo.bEncrypt = m_deviceTableModel->getIsEncrypt(0);
    stDeviceInfo.iVideoLevel = m_deviceTableModel->getVideoLevel(0);//超清
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
            return -1;
        }
    }

    //Set the video store path
    //setVideoPath(devSerial);
    //OpenNetStream::getInstance()->setDataCallBack(m_sessionId, videoDataHandler, this);

    HWND hWnd = NULL;
    if (LocalSetting::getInstance()->isPlay())
    {
        ui->previewWindow->winId();
        hWnd = (HWND) ui->previewWindow->winId();
    }

    int iRet = OpenNetStream::getInstance()->startRealPlay(m_sessionId, hWnd, devSerial, iChannelNo, safekey);
    if(0 != iRet)
    {
        this->showErrInfo(tr("RealPlay"));
        return -1;
    }

    m_bRealPlayStarted = true;
    m_bCloudPlayBack = false;
    m_bPlayBackStarted = false;
    return 0;
}

//start play
void MainWindow::on_start_paly_clicked()
{
  start_play();
}



void MainWindow::messageHandler(const char *szSessionId, unsigned int iMsgType, unsigned int iErrorCode, const char *pMessageInfo, void *pUser)
{
    qDebug() << __LINE__ << __FUNCTION__ << szSessionId << iMsgType << iErrorCode << pMessageInfo;
    MainWindow* pInstance = static_cast<MainWindow*>(pUser);
    if (!pInstance) {
        return;
    }

    switch (iMsgType) {
    case INS_PLAY_EXCEPTION:
        emit pInstance->insPlayException(iErrorCode, pMessageInfo);
        break;
    case INS_PLAY_RECONNECT:
        break;
    case INS_PLAY_RECONNECT_EXCEPTION:
        emit pInstance->insPlayReconnectException(iErrorCode, pMessageInfo);
        break;
    case INS_PLAY_START:
        emit pInstance->insPlayStart();
        break;
    case INS_PLAY_STOP:
        emit pInstance->insPlayStop();
        break;
    case INS_PLAY_ARCHIVE_END:
        emit pInstance->insPlayBackEnd(iErrorCode);
        break;
    case INS_RECORD_FILE:
        emit pInstance->insRecordFile(pMessageInfo);
        break;
    case INS_RECORD_SEARCH_END:
        break;
    case INS_RECORD_SEARCH_FAILED:
        emit pInstance->insRecordSearchFailed(iErrorCode, pMessageInfo);
        break;
    case INS_DOWNLOAD_STOP:
        emit pInstance->insDownloadCloudFileEnd(iErrorCode);
        break;
    case INS_DOWNLOAD_EXCEPTION:
       emit pInstance->insDownloadCloudFileException(iErrorCode, pMessageInfo);
        break;
    case INS_USER_STOP_DOWNLOAD:
        emit pInstance->insStopDownloadCloudFile();
        break;

    }
}

void MainWindow::showErrInfo(QString caption)
{
    int iErrCode = OpenNetStream::getInstance()->GetLastErrorCode();
    char szBuf[64] = {0};
    sprintf(szBuf, "ErrorCode:%d", iErrCode);
    QMessageBox::information(this, caption, szBuf);
}

void MainWindow::insPtzStart(PTZCMD emDirect)
{
    /*
    if(!s->is_paly())
    {
        QMessageBox::warning(this,QStringLiteral("失败") , QStringLiteral("请先播放该视频"));
        return;
    }
    */
    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(move_result(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(QUrl("https://open.ys7.com/api/lapp/device/ptz/start"));

    QByteArray postData;
    postData.append("accessToken=");
    postData.append(accessId);
    postData.append("&deviceSerial=");
    postData.append(m_devSerial);
    postData.append("&channelNo=");
    postData.append(QString::number(m_Channel));
    postData.append("&direction=");
    switch (emDirect)
    {//up：0  ,down :1 ,left:2  ,right:3
    case DIRECT_UP:
        postData.append(QString::number(0));
        break;
    case DIRECT_DOWN:
        postData.append(QString::number(1));
        break;
    case DIRECT_LEFT:
        postData.append(QString::number(2));
        break;
    case DIRECT_RIGHT:
        postData.append(QString::number(3));
    }
    postData.append("&speed=1");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
//post
    QNetworkReply* reply = accessManager->post(request, postData);
}

void MainWindow::insPtzStop(PTZCMD emDirect)
{

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(move_result(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(QUrl("https://open.ys7.com/api/lapp/device/ptz/stop"));

    QByteArray postData;
    postData.append("accessToken=");
    postData.append(accessId);
    postData.append("&deviceSerial=");
    postData.append(m_devSerial);
    postData.append("&channelNo=");
    postData.append(QString::number(m_Channel));
    postData.append("&direction=");
    switch (emDirect)
    {//up：0  ,down :1 ,left:2  ,right:3
    case DIRECT_UP:
        postData.append(QString::number(0));
        break;
    case DIRECT_DOWN:
        postData.append(QString::number(1));
        break;
    case DIRECT_LEFT:
        postData.append(QString::number(2));
        break;
    case DIRECT_RIGHT:
        postData.append(QString::number(3));
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
//post
    QNetworkReply* reply = accessManager->post(request, postData);
}


void MainWindow::move_result(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this,QStringLiteral("失败") , QStringLiteral("请求失败！"));
    }
}
