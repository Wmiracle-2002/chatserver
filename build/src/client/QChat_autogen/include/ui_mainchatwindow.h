/********************************************************************************
** Form generated from reading UI file 'mainchatwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINCHATWINDOW_H
#define UI_MAINCHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainChatWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *itemLayout;
    QTabWidget *tabWidget;
    QWidget *Friend;
    QPushButton *addfriendButton;
    QLineEdit *addfriendEdit;
    QListWidget *friendWidget;
    QWidget *Group;
    QLineEdit *addgroupEdit;
    QPushButton *addgroupButton;
    QPushButton *creategroupButton;
    QLineEdit *creategroupEdit;
    QListWidget *groupWidget;
    QWidget *chatWidget;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QPushButton *sendButton;
    QWidget *userinfo;
    QLabel *idLabel;
    QLabel *nameLabel;
    QWidget *profile;
    QLabel *pictureLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainChatWindow)
    {
        if (MainChatWindow->objectName().isEmpty())
            MainChatWindow->setObjectName("MainChatWindow");
        MainChatWindow->resize(800, 595);
        centralwidget = new QWidget(MainChatWindow);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 50, 241, 521));
        itemLayout = new QVBoxLayout(layoutWidget);
        itemLayout->setObjectName("itemLayout");
        itemLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName("tabWidget");
        Friend = new QWidget();
        Friend->setObjectName("Friend");
        Friend->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Friend->sizePolicy().hasHeightForWidth());
        Friend->setSizePolicy(sizePolicy);
        Friend->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        addfriendButton = new QPushButton(Friend);
        addfriendButton->setObjectName("addfriendButton");
        addfriendButton->setGeometry(QRect(140, 460, 91, 25));
        addfriendEdit = new QLineEdit(Friend);
        addfriendEdit->setObjectName("addfriendEdit");
        addfriendEdit->setGeometry(QRect(0, 460, 141, 25));
        friendWidget = new QListWidget(Friend);
        friendWidget->setObjectName("friendWidget");
        friendWidget->setGeometry(QRect(0, 0, 231, 451));
        tabWidget->addTab(Friend, QString());
        Group = new QWidget();
        Group->setObjectName("Group");
        addgroupEdit = new QLineEdit(Group);
        addgroupEdit->setObjectName("addgroupEdit");
        addgroupEdit->setGeometry(QRect(0, 460, 141, 25));
        addgroupButton = new QPushButton(Group);
        addgroupButton->setObjectName("addgroupButton");
        addgroupButton->setGeometry(QRect(140, 460, 91, 25));
        creategroupButton = new QPushButton(Group);
        creategroupButton->setObjectName("creategroupButton");
        creategroupButton->setGeometry(QRect(140, 430, 91, 25));
        creategroupEdit = new QLineEdit(Group);
        creategroupEdit->setObjectName("creategroupEdit");
        creategroupEdit->setGeometry(QRect(0, 430, 141, 25));
        groupWidget = new QListWidget(Group);
        groupWidget->setObjectName("groupWidget");
        groupWidget->setGeometry(QRect(0, 0, 231, 421));
        tabWidget->addTab(Group, QString());

        itemLayout->addWidget(tabWidget);

        chatWidget = new QWidget(centralwidget);
        chatWidget->setObjectName("chatWidget");
        chatWidget->setGeometry(QRect(239, 0, 562, 571));
        textEdit = new QTextEdit(chatWidget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 0, 561, 451));
        textEdit->setReadOnly(true);
        lineEdit = new QLineEdit(chatWidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(0, 450, 461, 121));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);
        sendButton = new QPushButton(chatWidget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(460, 450, 101, 121));
        userinfo = new QWidget(centralwidget);
        userinfo->setObjectName("userinfo");
        userinfo->setGeometry(QRect(50, 0, 181, 50));
        idLabel = new QLabel(userinfo);
        idLabel->setObjectName("idLabel");
        idLabel->setGeometry(QRect(0, 0, 181, 25));
        idLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        nameLabel = new QLabel(userinfo);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setGeometry(QRect(0, 25, 181, 25));
        nameLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        profile = new QWidget(centralwidget);
        profile->setObjectName("profile");
        profile->setGeometry(QRect(0, 0, 50, 50));
        pictureLabel = new QLabel(profile);
        pictureLabel->setObjectName("pictureLabel");
        pictureLabel->setGeometry(QRect(0, 0, 50, 50));
        MainChatWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainChatWindow);
        statusbar->setObjectName("statusbar");
        MainChatWindow->setStatusBar(statusbar);

        retranslateUi(MainChatWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainChatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainChatWindow)
    {
        MainChatWindow->setWindowTitle(QCoreApplication::translate("MainChatWindow", "MainWindow", nullptr));
        addfriendButton->setText(QCoreApplication::translate("MainChatWindow", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Friend), QCoreApplication::translate("MainChatWindow", "\346\210\221\347\232\204\345\245\275\345\217\213", nullptr));
        addgroupButton->setText(QCoreApplication::translate("MainChatWindow", "\345\212\240\345\205\245\347\276\244\347\273\204", nullptr));
        creategroupButton->setText(QCoreApplication::translate("MainChatWindow", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Group), QCoreApplication::translate("MainChatWindow", "\346\210\221\347\232\204\347\276\244\347\273\204", nullptr));
        sendButton->setText(QCoreApplication::translate("MainChatWindow", "\345\217\221\351\200\201", nullptr));
        idLabel->setText(QString());
        nameLabel->setText(QString());
        pictureLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainChatWindow: public Ui_MainChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINCHATWINDOW_H
