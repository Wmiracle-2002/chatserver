/********************************************************************************
** Form generated from reading UI file 'registerwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWIDGET_H
#define UI_REGISTERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterWidget
{
public:
    QPushButton *registerButton;
    QLabel *nameLabel;
    QLabel *againLabel;
    QLineEdit *nameEdit;
    QLineEdit *againEdit;
    QLabel *QChatLabel;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QPushButton *backButton;

    void setupUi(QWidget *RegisterWidget)
    {
        if (RegisterWidget->objectName().isEmpty())
            RegisterWidget->setObjectName("RegisterWidget");
        RegisterWidget->resize(400, 300);
        registerButton = new QPushButton(RegisterWidget);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(100, 220, 81, 31));
        nameLabel = new QLabel(RegisterWidget);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setGeometry(QRect(70, 100, 61, 21));
        nameLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        againLabel = new QLabel(RegisterWidget);
        againLabel->setObjectName("againLabel");
        againLabel->setGeometry(QRect(20, 180, 111, 21));
        againLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        nameEdit = new QLineEdit(RegisterWidget);
        nameEdit->setObjectName("nameEdit");
        nameEdit->setGeometry(QRect(140, 100, 141, 25));
        againEdit = new QLineEdit(RegisterWidget);
        againEdit->setObjectName("againEdit");
        againEdit->setGeometry(QRect(140, 180, 141, 25));
        QChatLabel = new QLabel(RegisterWidget);
        QChatLabel->setObjectName("QChatLabel");
        QChatLabel->setGeometry(QRect(140, 20, 111, 71));
        QFont font;
        font.setPointSize(20);
        QChatLabel->setFont(font);
        QChatLabel->setTextFormat(Qt::TextFormat::AutoText);
        QChatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        passwordLabel = new QLabel(RegisterWidget);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setGeometry(QRect(70, 140, 61, 21));
        passwordLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        passwordEdit = new QLineEdit(RegisterWidget);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(140, 140, 141, 25));
        backButton = new QPushButton(RegisterWidget);
        backButton->setObjectName("backButton");
        backButton->setGeometry(QRect(240, 220, 81, 31));

        retranslateUi(RegisterWidget);

        QMetaObject::connectSlotsByName(RegisterWidget);
    } // setupUi

    void retranslateUi(QWidget *RegisterWidget)
    {
        RegisterWidget->setWindowTitle(QCoreApplication::translate("RegisterWidget", "Form", nullptr));
        registerButton->setText(QCoreApplication::translate("RegisterWidget", "\346\263\250\345\206\214", nullptr));
        nameLabel->setText(QCoreApplication::translate("RegisterWidget", "\346\230\265\347\247\260\357\274\232", nullptr));
        againLabel->setText(QCoreApplication::translate("RegisterWidget", "\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201\357\274\232", nullptr));
        QChatLabel->setText(QCoreApplication::translate("RegisterWidget", "QChat", nullptr));
        passwordLabel->setText(QCoreApplication::translate("RegisterWidget", "\345\257\206\347\240\201\357\274\232", nullptr));
        backButton->setText(QCoreApplication::translate("RegisterWidget", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterWidget: public Ui_RegisterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWIDGET_H
