#include "mainwindow.h"
#include "ui_mainwindow.h"




void MainWindow::set_vodeo()
{
    m_deviceTableModel = new CameraModel(this);


    //! [1] init library，alloc session
    OpenNetStream::getInstance()->initLibrary(LocalSetting::getInstance()->authAddress(), LocalSetting::getInstance()->platformAddress(), LocalSetting::getInstance()->appKey());
    QString areaID = LocalSetting::getInstance()->areaId();
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
    m_loginWindow = new LoginWindow(this);
    //! [1]
}



/*! @function Login
 * \brief MainWindow::on_loginButton_clicked
 */
void MainWindow::on_loginButton_clicked()
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
    stDeviceInfo.strSubserial = m_deviceTableModel->getSerial(3);//3是船上的相机
    stDeviceInfo.iChannelNo = m_deviceTableModel->getCameraNo(1);
    stDeviceInfo.bEncrypt = m_deviceTableModel->getIsEncrypt(0);
    stDeviceInfo.iVideoLevel = m_deviceTableModel->getVideoLevel(3);//超清
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

