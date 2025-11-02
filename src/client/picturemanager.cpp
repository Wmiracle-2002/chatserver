#include "picturemanager.h"
#include <QBuffer>
#include <QIODevice>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

PictureManager::PictureManager(ClientCore* clientCore, QObject* parent)
    : QObject(parent)
    , m_clientCore(clientCore)
    , m_httpServerUrl("http://47.111.183.136:8080")
    , m_networkManager(new QNetworkAccessManager(this))
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir(appDataPath);
    if (!appDataDir.exists("resources")) {
        appDataDir.mkpath("resources");
    }
    m_localCachePath = appDataDir.absoluteFilePath("resources/");
}

// 上传头像到服务器
void PictureManager::uploadPicture(const QPixmap& pixmap)
{   
    // 压缩头像
    QPixmap compressedPixmap = compressPicture(pixmap);
    
    // 1. 将QPixmap转换为Base64字符串
    QString base64Data = pixmapToBase64(compressedPixmap);
    
    // 2. 构建JSON请求
    QJsonObject js;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["picture"] = base64Data;
    js["name"] = QString::fromStdString(m_clientCore->g_currentUser.getName());
    
    QJsonDocument doc(js);
    QByteArray requestData = doc.toJson();
    // 3. 发送HTTP POST请求
    qDebug() << "准备发送HTTP POST请求";
    QNetworkRequest request;
    request.setUrl(QUrl(m_httpServerUrl + "/api/picture/upload"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "requestUrl: " << request.url() << "requestHeader: " << request.headers();
    QNetworkReply* reply = m_networkManager->post(request, requestData);
    
    connect(reply, &QNetworkReply::finished, this, &PictureManager::onUploadFinished);
    qDebug() << "开始上传头像，用户ID:" << m_clientCore->g_currentUser.getId();
}

// 保存头像到本地
void PictureManager::downloadPicture(const QString& userid)
{
    // 先检查本地缓存
    QPixmap cachedPicture = loadFromCache(userid);
    if (!cachedPicture.isNull()) {
        qDebug() << "从缓存加载头像，用户ID:" << userid;
        emit pictureDownloaded(userid, cachedPicture);
        return;
    }

    // 发送HTTP GET请求
    QNetworkRequest request;
    request.setUrl(QUrl(m_httpServerUrl + "/api/picture/" + userid));
    
    QNetworkReply* reply = m_networkManager->get(request);
    
    // 存储用户ID，用于回调时识别
    reply->setProperty("id", userid);
    
    // 连接下载完成信号
    connect(reply, &QNetworkReply::finished, this, &PictureManager::onDownloadFinished);
    
    qDebug() << "开始下载头像，用户ID:" << userid;
}

void PictureManager::onUploadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }
    bool success = false;
    QString message;
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject js = doc.object();
        success = js["success"].toBool();
        message = js["message"].toString();
        if (success) {
            qDebug() << "头像上传成功";
        }
        else {
            qWarning() << "头像上传失败:" << message;
        }
    }
    else {
        message = "网络错误: " + reply->errorString();
        qWarning() << "头像上传网络错误:" << reply->errorString();
    }
    
    emit pictureUploaded(success, message);
    reply->deleteLater();
}

void PictureManager::onDownloadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }
    QString userid = reply->property("id").toString();
    QPixmap picture;
    
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject json = doc.object();
        
        if (json["success"].toBool()) {
            QString base64Data = json["picture"].toString();
            picture = base64ToPixmap(base64Data);
            
            if (!picture.isNull()) {
                // 保存到缓存
                saveToCache(userid, picture);
                qDebug() << "头像下载成功，用户ID:" << userid;
            }
            else {
                qWarning() << "头像数据转换失败，用户ID:" << userid;
            }
        }
        else {
            qWarning() << "头像下载失败，用户ID:" << userid << "错误:" << json["message"].toString();
        }
    }
    else {
        qWarning() << "头像下载网络错误，用户ID:" << userid << "错误:" << reply->errorString();
    }
    
    emit pictureDownloaded(userid, picture);
    reply->deleteLater();
}

// 显示头像
void PictureManager::showPicture(const QPixmap& pixmap, QLabel* label)
{
    QPixmap ppixmap = pixmap;
    if(ppixmap.isNull()){
        QString path = QString::fromStdString("../resources/default.jpg");
        ppixmap.load(path);
    }
    // 1. 获取label大小
    QSize labelSize = label->size();
    // 2. 缩放原图
    QPixmap scaledPixmap = ppixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 3. 缩放图显示在label上
    label->setPixmap(scaledPixmap);
}

// 显示头像（QString版）
void PictureManager::showPicture(const QString& picture, QLabel* label)
{
    QPixmap pixmap;
    if(picture.isEmpty()){
        QString path = QString::fromStdString("../resources/default.jpg");
        pixmap.load(path);
    }
    else{
        // 1. 将Base64字符串转换为QPixmap
        pixmap = base64ToPixmap(picture);
    }
    // 2. 获取label大小
    QSize labelSize = label->size();
    // 3. 缩放原图
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 4. 缩放图显示在label上
    label->setPixmap(scaledPixmap);
}

// 显示头像（item版）（QPixmap版）
void PictureManager::showPicture(const QPixmap& pixmap, QListWidgetItem* item)
{
    QPixmap ppixmap = pixmap;
    if(ppixmap.isNull()){
        QString path = QString::fromStdString("../resources/default.jpg");
        ppixmap.load(path);
    }
    // 1. 获取item大小
    int height = item->sizeHint().height();
    QSize labelSize = QSize(height, height);
    // 2. 缩放原图
    QPixmap scaledPixmap = ppixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 3. 缩放图显示在item上
    item->setIcon(QIcon(scaledPixmap));

}

// 显示头像（item版）（QString版）
void PictureManager::showPicture(const QString& picture, QListWidgetItem* item)
{
    QPixmap pixmap;
    if(picture.isEmpty()){
        QString path = QString::fromStdString("../resources/default.jpg");
        pixmap.load(path);
    }
    else{
        // 1. 将Base64字符串转换为QPixmap
        pixmap = base64ToPixmap(picture);
    }
    // 2. 获取item大小
    int height = item->sizeHint().height();
    QSize labelSize = QSize(height, height);
    // 3. 缩放原图
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 4. 缩放图显示在item上
    item->setIcon(QIcon(scaledPixmap));
}

// 压缩图像
QPixmap PictureManager::compressPicture(const QPixmap& pixmap)
{
    if (pixmap.isNull()) {
        return QPixmap();
    }
    
    QPixmap compressed = pixmap;
    
    // 1. 尺寸压缩：如果图片太大，缩放
    if (compressed.width() > 64 || compressed.height() > 64) {
        compressed = compressed.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    return compressed;
}

// 删除缓存头像
void PictureManager::deleteCachedPicture(const QString& userid)
{
    QString cachePath = getCachePath(userid);
    QFile file(cachePath);
    if (file.exists()) {
        if (file.remove()) {
            qDebug() << "删除本地头像文件，用户ID:" << userid;
        }
        else {
            qWarning() << "删除本地头像文件失败，用户ID:" << userid;
        }
    }
    // 发出信号通知头像已删除
    emit pictureCacheDeleted(userid);
}

QByteArray PictureManager::pixmapToByteArray(const QPixmap& pixmap)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");  // 保存为PNG格式
    return imageData;
}

QPixmap PictureManager::byteArrayToPixmap(const QByteArray& data)
{
    QPixmap pixmap;
    pixmap.loadFromData(data);
    return pixmap;
}

QPixmap PictureManager::base64ToPixmap(const QString& base64String)
{
    if (base64String.isEmpty()) {
        qWarning() << "PictureManager: Base64 string is empty";
        return QPixmap();
    }
    
    // 处理可能包含的数据URI前缀
    QString cleanBase64 = base64String;
    if (base64String.startsWith("data:image")) {
        // 找到base64,之后的部分
        int base64Index = base64String.indexOf("base64,");
        if (base64Index != -1) {
            cleanBase64 = base64String.mid(base64Index + 7); // 7是"base64,"的长度
        }
    }
    
    // 将Base64字符串转换为字节数组
    QByteArray imageData = QByteArray::fromBase64(cleanBase64.toUtf8());
    
    if (imageData.isEmpty()) {
        qWarning() << "PictureManager: Failed to decode Base64 string";
        return QPixmap();
    }
    
    // 从字节数据加载图片
    QPixmap pixmap;
    if (!pixmap.loadFromData(imageData)) {
        qWarning() << "PictureManager: Failed to load pixmap from Base64 data";
        return QPixmap();
    }
    return pixmap;
}

QString PictureManager::pixmapToBase64(const QPixmap& pixmap)
{
    if (pixmap.isNull()) {
        qWarning() << "PictureManager: Cannot convert null pixmap to Base64";
        return QString();
    }
    
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    
    // 保存为PNG格式到buffer
    if (!pixmap.save(&buffer, "PNG")) {
        qWarning() << "PictureManager: Failed to save pixmap to buffer";
        return QString();
    }
    
    buffer.close();
    
    // 转换为Base64字符串
    QString base64String = QString::fromLatin1(imageData.toBase64());
    
    return base64String;
}

QString PictureManager::getCachePath(const QString& userid)
{
    return m_localCachePath + userid + ".png";
}

bool PictureManager::saveToCache(const QString& userid, const QPixmap& picture)
{
    if (userid.isEmpty() || picture.isNull()) {
        return false;
    }
    QString filePath = getCachePath(userid);
    if(QFile::exists(filePath)) return true;
    QPixmap compressedPicture = compressPicture(picture);
    return compressedPicture.save(filePath, "PNG");
}

QPixmap PictureManager::loadFromCache(const QString& userid)
{
    QString filePath = getCachePath(userid);
    if (QFile::exists(filePath)) {
        QPixmap pixmap;
        if (pixmap.load(filePath)) {
            return pixmap;
        }
    }
    return QPixmap();  // 返回空QPixmap表示缓存不存在
}