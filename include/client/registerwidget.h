#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include "clientcore.h"

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(ClientCore* clientCore, QWidget *parent = nullptr);
    ~RegisterWidget();
    void onRegistrationSuccess(int id);
    void onRegistrationFailed(QString errorMsg);
private slots:
    void on_registerButton_clicked();

    void on_backButton_clicked();

private:
    Ui::RegisterWidget *ui;
    ClientCore* m_clientCore; // 添加核心网络对象指针
};

#endif // REGISTERWIDGET_H
