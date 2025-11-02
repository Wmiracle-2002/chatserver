/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QPushButton *registerButton;
    QPushButton *loginButton;
    QLabel *QChatLabel;
    QLabel *usernameLable;
    QLabel *passwordLable;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(400, 300);
        registerButton = new QPushButton(LoginWidget);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(100, 220, 81, 31));
        loginButton = new QPushButton(LoginWidget);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(220, 220, 81, 31));
        QChatLabel = new QLabel(LoginWidget);
        QChatLabel->setObjectName("QChatLabel");
        QChatLabel->setGeometry(QRect(140, 30, 111, 71));
        QFont font;
        font.setPointSize(20);
        QChatLabel->setFont(font);
        QChatLabel->setTextFormat(Qt::TextFormat::AutoText);
        QChatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        usernameLable = new QLabel(LoginWidget);
        usernameLable->setObjectName("usernameLable");
        usernameLable->setGeometry(QRect(50, 130, 71, 17));
        usernameLable->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        passwordLable = new QLabel(LoginWidget);
        passwordLable->setObjectName("passwordLable");
        passwordLable->setGeometry(QRect(50, 170, 71, 17));
        passwordLable->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        usernameEdit = new QLineEdit(LoginWidget);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(130, 130, 161, 20));
        passwordEdit = new QLineEdit(LoginWidget);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(130, 170, 161, 20));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        retranslateUi(LoginWidget);

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QCoreApplication::translate("LoginWidget", "Form", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginWidget", "\346\263\250\345\206\214", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWidget", "\347\231\273\345\275\225", nullptr));
        QChatLabel->setText(QCoreApplication::translate("LoginWidget", "QChat", nullptr));
        usernameLable->setText(QCoreApplication::translate("LoginWidget", "\350\264\246\345\217\267\357\274\232", nullptr));
        passwordLable->setText(QCoreApplication::translate("LoginWidget", "\345\257\206\347\240\201\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
