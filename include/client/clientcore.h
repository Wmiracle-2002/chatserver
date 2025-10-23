#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QObject>
#include <QThread>
#include <atomic>
#include "public.hpp"
#include "user.hpp"
#include "group.hpp"
#include "json.hpp"

// 平台相关的网络头文件
#ifdef _WIN32
    // 在包含Windows头文件之前定义这些宏来避免命名冲突
    #define NOMINMAX  // 避免min/max宏
    #define WIN32_LEAN_AND_MEAN  // 减少Windows头文件包含
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

using json = nlohmann::json;

class ClientCore : public QObject
{
    Q_OBJECT
public:
    explicit ClientCore(QObject *parent = nullptr);
    ~ClientCore();

    bool connectToServer(const QString& ip, quint16 port);
    bool isConnected() const { return m_clientfd != -1; }
    void sendJson(const json &js);
    void doLoginResponse(const json &responsejs);
    void doRegResponse(const json &responsejs);
    void doAddFriendResponse(const json &responsejs);
    void doDeleteFriendResponse(const json &responsejs);
    void doCreateGroupResponse(const json &responsejs);
    void doAddGroupResponse(const json &responsejs);
    void doQuitGroupResponse(const json &responsejs);
    void doDissolveGroupResponse(const json &responsejs);
    // 用户操作
    void login(int id, const QString& password);
    void registerUser(const QString& name, const QString& password);
    json getRegSuccessMsg();

    User g_currentUser; // 记录当前系统登录的用户信息
    vector<User> g_currentUserFriendList;   // 记录当前登录用户的好友列表信息
    vector<Group> g_currentUserGroupList;   // 记录当前登录用户的群组列表信息
    bool isMainMenuRunning = false; // 控制主菜单页面程序
    // sem_t rwsem;    // 用于读写线程之间的通信
    atomic_bool g_isLoginSuccess{false};    // 记录登录状态

signals:
    void loginSuccess(const json &js);
    void loginFailed(const QString& reason);
    void registrationSuccess(int id);
    void registrationFailed(const QString& reason);
    void recvOfflineMsg(const QString &jsStr);
    void recvOneChatMsg(const json &js);
    void recvGroupChatMsg(const json &js);
    void recvAddFriendMsg(const json &js);
    void groupDissolved(int groupid);
    void friendDeleted(int friendid);
    void friendAdded(int id, string name);
    void groupAdded(int id, string name);
    void friendInfo(const QString& type, const QString& title, const QString& msg);
    void groupInfo(const QString& type, const QString& title, const QString& msg);
private:
    void readTaskHandler();
    void closeSocket();

    int m_clientfd = -1;
    std::atomic_bool m_running{false};
    QThread* m_readThread = nullptr;
    
#ifdef _WIN32
    static bool s_wsaInitialized;
    static void initWSA();
#endif
};

#endif // CLIENTCORE_H