#include "json.hpp"
#include <iostream>
#include <string>
#include "loginwidget.h"
#include "clientcore.h"
#include "mainchatwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QChat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    
    // 创建核心网络组件
    ClientCore *clientCore = new ClientCore;
    clientCore->connectToServer("192.168.253.129", 8000);
    // 创建登录界面，传入核心网络对象
    LoginWidget loginWidget(clientCore);
    loginWidget.show();

    // 连接登录成功信号
    QObject::connect(&loginWidget, &LoginWidget::loginSuccess, [&](const json &js) {

        std::cout << "Login successful! Opening main window..." << std::endl;
        
        MainChatWindow* mWindow = new MainChatWindow(clientCore);
        
        mWindow->show();
        clientCore->doLoginResponse(js);
        mWindow->fillFriendList();
    });
    
    return a.exec();
}
