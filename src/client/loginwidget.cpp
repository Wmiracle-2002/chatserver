#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "mainchatwindow.h"
#include "ui_mainchatwindow.h"
#include <QMessageBox>
#include <iostream>

LoginWidget::LoginWidget(ClientCore* clientCore, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , m_clientCore(clientCore) // 初始化核心网络对象
{
    ui->setupUi(this);
    
    // 连接核心网络信号
    connect(m_clientCore, &ClientCore::loginSuccess, this, &LoginWidget::handleLoginSuccess);
    connect(m_clientCore, &ClientCore::loginFailed, this, &LoginWidget::handleLoginFailed);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_registerButton_clicked()
{
    std::cout << "Register..." << std::endl;

    RegisterWidget* registerWidget = new RegisterWidget(m_clientCore);
    registerWidget->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    this->close();
    registerWidget->show();
}

void LoginWidget::on_loginButton_clicked()
{
    QString userid = ui->usernameEdit->text();
    QString pwd = ui->passwordEdit->text();
    
    if (userid.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "用户名和密码不能为空");
        return;
    }
    
    // 尝试连接服务器（这里简化处理，实际应该先配置服务器地址）
    if (!m_clientCore->isConnected()) {
        // 简单使用默认地址，实际应用中应该让用户输入或使用配置
        m_clientCore->connectToServer("127.0.0.1", 8000);
    }
    
    bool ok;
    int id = userid.toInt(&ok);
    
    if (ok) {
        m_clientCore->login(id, pwd);
    } else {
        QMessageBox::warning(this, "登录失败", "用户ID必须是数字");
    }
}

void LoginWidget::handleLoginSuccess(const json &js)
{
    emit loginSuccess(js); // 发出登录成功信号
    this->close();
}

void LoginWidget::handleLoginFailed(const QString& reason)
{
    QMessageBox::warning(this, "登录失败", reason);
}