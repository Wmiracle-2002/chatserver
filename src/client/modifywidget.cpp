#include "modifywidget.h"
#include "ui_modifywidget.h"
#include "public.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "picturemanager.h"

ModifyWidget::ModifyWidget(ClientCore *clientCore, QWidget *parent)
    : QWidget(parent)
    , m_clientCore(clientCore)
    , m_pm(new PictureManager(m_clientCore, this))
    , ui(new Ui::ModifyWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText("原昵称: " + QString::fromStdString(m_clientCore->g_currentUser.getName()) + "\t新昵称: ");
    
    connect(m_pm, &PictureManager::pictureDownloaded, this, &ModifyWidget::onPictureDownloaded);
    connect(m_pm, &PictureManager::pictureUploaded, this, &ModifyWidget::onPictureUploaded);

    m_pm->showPicture("", ui->pictureLabel);
    m_pm->downloadPicture(QString::number(m_clientCore->g_currentUser.getId()));

    connect(m_clientCore, &ClientCore::nameModified, this, &ModifyWidget::onNameModified);
    connect(m_clientCore, &ClientCore::passwordModified, this, &ModifyWidget::onPwdModified);
}

ModifyWidget::~ModifyWidget()
{
    delete ui;
}

void ModifyWidget::on_modifynameButton_clicked()
{
    // 检查昵称是否合法（目前只检测是否重复）
    QString new_name = ui->modifynameEdit->text();
    ui->modifynameEdit->clear();
    modifyname(new_name);
}

void ModifyWidget::on_modifypwdButton_clicked()
{
    // 检查密码是否合法（目前检测 1.原密码输入是否正确，2.新密码再次输入是否和第一次输入相同）
    QString old_pwd = ui->oldpwdEdit->text();
    QString new_pwd = ui->newpwdEdit->text();
    QString again_pwd = ui->againpwdEdit->text();
    ui->oldpwdEdit->clear();
    ui->newpwdEdit->clear();
    ui->againpwdEdit->clear();
    if(again_pwd != new_pwd){
        QMessageBox::warning(this, "修改密码失败", "两次输入的新密码不一致");
        return;
    }
    modifypassword(old_pwd, new_pwd);
}

void ModifyWidget::on_selectpictureButton_clicked()
{
    // 设置文件对话框的过滤条件，只显示图片文件
    QStringList filters;
    filters << "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif *.ico)"
            << "所有文件 (*.*)";
    // 弹出选择本地图片框
    QString fileName = QFileDialog::getOpenFileName(
        this,                                       // 父窗口
        "选择头像图片",                              // 对话框标题
        QDir::homePath(),                           // 初始目录（用户主目录）
        filters.join(";;")                          // 文件过滤器
    );
    // 加载原图
    QPixmap originalPixmap;
    if(QFile::exists(fileName) && originalPixmap.load(fileName)){
        m_pm->showPicture(originalPixmap, ui->pictureLabel);
    }
}

void ModifyWidget::on_modifypictureButton_clicked()
{
    // 1. 将图片保存到服务器上
    QPixmap picture = ui->pictureLabel->pixmap();
    m_pm->uploadPicture(picture);

    // 2. 将图片同时保存在客户端上
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir(appDataPath);
    if (!appDataDir.exists("resources")) {
        appDataDir.mkpath("resources");
    }
    QString fileName = QString("%1.png").arg(m_clientCore->g_currentUser.getId());
    QString filePath = appDataDir.absoluteFilePath("resources/" + fileName);
    ui->pictureLabel->pixmap().save(filePath, "PNG");
}

void ModifyWidget::onNameModified(const json &js)
{
    if(js["errno"] == 1){
        QMessageBox::warning(this, "修改用户名失败", "用户名已存在");
        return;
    }
    string new_name = js["name"];
    QMessageBox::information(this, "修改用户名成功", "新用户名为：" + QString::fromStdString(new_name));
    m_clientCore->g_currentUser.setName(new_name);
    ui->nameLabel->setText("原昵称: " + QString::fromStdString(m_clientCore->g_currentUser.getName()) + "\t新昵称: ");
    emit m_clientCore->refreshUserInfo();
}

void ModifyWidget::onPwdModified(const json &js)
{
    if(js["errno"] == 1){
        QMessageBox::warning(this, "修改密码失败", "原密码输入错误");
        return;
    }
    string new_pwd = js["password"];
    QMessageBox::information(this, "修改密码成功", "请妥善保管好新密码");
    m_clientCore->g_currentUser.setPwd(new_pwd);
    emit m_clientCore->refreshUserInfo();
}

void ModifyWidget::onPictureUploaded(bool success, const QString& message)
{
    if (success) {
        QMessageBox::information(this, "修改头像成功", "头像已更新");
        json js;
        js["id"] = m_clientCore->g_currentUser.getId();
        js["msgid"] = MODIFY_PICTURE_MSG;
        m_clientCore->sendJson(js);
        emit m_clientCore->refreshUserInfo();
    }
    else {
        QMessageBox::warning(this, "修改头像失败", message);
    }
}

void ModifyWidget::onPictureDownloaded(const QString& userid, const QPixmap& picture)
{
    QString currentUserId = QString::number(m_clientCore->g_currentUser.getId());
    if (userid == currentUserId && !picture.isNull()) {
        m_pm->showPicture(picture, ui->pictureLabel);
    }
}

// 修改用户名命令
void ModifyWidget::modifyname(QString new_name){
    json js;
    js["msgid"] = MODIFY_NAME_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["name"] = new_name.toStdString();
    m_clientCore->sendJson(js);
}

// 修改密码命令
void ModifyWidget::modifypassword(QString old_password, QString new_password){
    json js;
    js["msgid"] = MODIFY_PASSWORD_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["name"] = m_clientCore->g_currentUser.getName();
    js["old_password"] = old_password.toStdString();
    js["new_password"] = new_password.toStdString();
    m_clientCore->sendJson(js);
}