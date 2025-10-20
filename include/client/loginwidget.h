#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "clientcore.h"  // 引入核心网络类

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(ClientCore* clientCore, QWidget *parent = nullptr); // 修改构造函数
    ~LoginWidget();

signals:
    void loginSuccess(const json &js); // 添加登录成功信号

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();
    
    // 添加处理核心网络信号的槽函数
    void handleLoginSuccess(const json &js);
    void handleLoginFailed(const QString& reason);

private:
    Ui::LoginWidget *ui;
    ClientCore* m_clientCore; // 添加核心网络对象指针
};

#endif // LOGINWIDGET_H