#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QMessageBox>
#include "public.hpp"
#include "json.hpp"
#include <string>
#include "loginwidget.h"
#include "ui_loginwidget.h"

RegisterWidget::RegisterWidget(ClientCore* clientCore, QWidget *parent)
    : QWidget(parent)
    , m_clientCore(clientCore)
    , ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    // 连接注册相关的信号
    connect(m_clientCore, &ClientCore::registrationSuccess, this, &RegisterWidget::onRegistrationSuccess);
    connect(m_clientCore, &ClientCore::registrationFailed, this, &RegisterWidget::onRegistrationFailed);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::onRegistrationSuccess(int id)
{
    // 重新启用按钮
    ui->registerButton->setEnabled(true);
    
    const QString messageA = QString("注册成功");
    const QString messageB = QString("您的用户ID是: %1, 请牢记此ID").arg(id);
    QMessageBox::information(this, messageA, messageB);
    
    // 可选：自动跳转到登录界面
    on_backButton_clicked();
}

void RegisterWidget::onRegistrationFailed(QString errorMsg)
{
    // 重新启用按钮
    ui->registerButton->setEnabled(true);
    
    QMessageBox::warning(this, "注册失败", errorMsg);
}

void RegisterWidget::on_registerButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString password = ui->passwordEdit->text();
    QString again = ui->againEdit->text();
    
    if (name.isEmpty() || password.isEmpty() || again.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空");
        return;
    }

    if(password != again)
    {
        QMessageBox::warning(this, "注册失败", "重复输入的密码与第一次输入不一致");
        return;
    }
    // 禁用按钮防止重复点击
    ui->registerButton->setEnabled(false);
    
    // 异步注册，不等待立即返回
    m_clientCore->registerUser(name, password);
}


void RegisterWidget::on_backButton_clicked()
{
    LoginWidget* loginWidget = new LoginWidget(m_clientCore);
    loginWidget->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    loginWidget->show();
    this->close();
}

