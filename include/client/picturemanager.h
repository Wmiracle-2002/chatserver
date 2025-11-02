#ifndef PICTUREMANAGER_H
#define PICTUREMANAGER_H
#include <QObject>
#include <QPixmap>
#include <QLabel>
#include <QListWidgetItem>
#include "clientcore.h"

class PictureManager : public QObject {
    Q_OBJECT
public:
    explicit PictureManager(ClientCore* clientCore, QObject* parent = nullptr);
    
    // 上传头像到服务器
    void uploadPicture(const QPixmap& pixmap);
    // 保存头像到本地
    void downloadPicture(const QString& userid);
    // 显示头像
    void showPicture(const QPixmap& pixmap, QLabel* label);
    void showPicture(const QString& picture, QLabel* label);
    void showPicture(const QPixmap& pixmap, QListWidgetItem* item);
    void showPicture(const QString& picture, QListWidgetItem* item);
    // 压缩图像
    QPixmap compressPicture(const QPixmap& pixmap);
    // 删除缓存头像
    void deleteCachedPicture(const QString& userid);
signals:
    void pictureUploaded(bool success, const QString& message);
    void pictureDownloaded(const QString& userid, const QPixmap& picture);
    void pictureCacheDeleted(const QString& userid);
private slots:
    // 上传完成
    void onUploadFinished();
    // 下载完成
    void onDownloadFinished();
private:
    ClientCore* m_clientCore;
    QString m_localCachePath;
    QNetworkAccessManager* m_networkManager;
    QString m_httpServerUrl;  // HTTP服务器地址

    // 图片转换工具方法
    QByteArray pixmapToByteArray(const QPixmap& pixmap);
    QPixmap byteArrayToPixmap(const QByteArray& data);
    QPixmap base64ToPixmap(const QString& base64String);
    QString pixmapToBase64(const QPixmap& pixmap);
    // 缓存管理方法
    QString getCachePath(const QString& userid);
    bool saveToCache(const QString& userid, const QPixmap& picture);
    QPixmap loadFromCache(const QString& userid);
};

#endif // !PICTUREMANAGER_H