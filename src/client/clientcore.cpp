#include "clientcore.h"
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <ctime>
#include <chrono>
#include <QMetaObject>

#ifdef _WIN32
bool ClientCore::s_wsaInitialized = false;

void ClientCore::initWSA()
{
    if (!s_wsaInitialized) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            qWarning() << "WSAStartup failed";
            return;
        }
        s_wsaInitialized = true;
    }
}
#endif

void ClientCore::closeSocket()
{
    if (m_clientfd != -1) {
#ifdef _WIN32
        closesocket(m_clientfd);
#else
        close(m_clientfd);
#endif
        m_clientfd = -1;
    }
}

// 处理登陆响应业务逻辑
void ClientCore::doLoginResponse(const json &responsejs){
    if(0 != responsejs["errno"].get<int>()){
        cerr << responsejs["errmsg"] << endl;
        g_isLoginSuccess = false;
    }
    else{
        // 记录当前用户的id和name
        g_currentUser.setId(responsejs["id"].get<int>());
        g_currentUser.setName(responsejs["name"]);
        // 初始化
        g_currentUserFriendList.clear();
        g_currentUserGroupList.clear();
        // 记录当前用户的好友列表信息
        if(responsejs.contains("friends")){
            vector<string> vec = responsejs["friends"];
            for(string &str : vec){
                json js = json::parse(str);
                User user;
                user.setId(js["id"].get<int>());
                user.setName(js["name"]);
                user.setState(js["state"]);
                g_currentUserFriendList.push_back(user);
            }
        }
        // 记录当前用户的群组列表信息
        if(responsejs.contains("groups")){
            vector<string> vec1 = responsejs["groups"];
            for(string &groupstr : vec1){
                json groupjs = json::parse(groupstr);
                Group group;
                group.setId(groupjs["id"].get<int>());
                group.setName(groupjs["groupname"]);
                group.setDesc(groupjs["groupdesc"]);
                
                vector<string> vec2 = groupjs["users"];
                for(string &userstr : vec2){
                    GroupUser user;
                    json js = json::parse(userstr);
                    user.setId(js["id"].get<int>());
                    user.setName(js["name"]);
                    user.setState(js["state"]);
                    user.setRole(js["role"]);
                    group.getUsers().push_back(user);
                }
                g_currentUserGroupList.push_back(group);
            }
        }

        // 显示当前用户的离线信息（个人聊天信息或者群组信息）
        if(responsejs.contains("offlinemsg")){
            vector<string> vec = responsejs["offlinemsg"];
            for(string &str : vec){
                json js = json::parse(str);
                // time + [id] + name + " said: " + xxx
                emit recvOfflineMsg(QString::fromStdString(js.dump()));
                if (ONE_CHAT_MSG == js["msgid"].get<int>()){
                    // qDebug() << "[Core] emit offline msg" << QString::fromStdString(js.dump());
                    cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                            << " said: " << js["msg"].get<string>() << endl;
                }
                else{
                    cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                            << " said: " << js["msg"].get<string>() << endl;
                }
            }
        }
        g_isLoginSuccess = true;
    }
}

// 处理注册响应业务逻辑
void ClientCore::doRegResponse(const json &responsejs){
    if(0 != responsejs["errno"].get<int>()){
        cerr << "name is already exist, register error!" << endl;
    }
    else{
        cout << "register success, userid is " << responsejs["id"] << ", do not forget it!" << endl;
    }
}

// 处理添加好友业务逻辑
void ClientCore::doAddFriendResponse(const json &responsejs){
    QString title, msg, type;
    if(0 != responsejs["errno"].get<int>()){
        title = "添加好友失败";
        msg = "输入的friendid不存在";
        type = "warning";
        cerr << "friendid is not exist, add friend error!" << endl;
    }
    else{
        User user;
        int friendid = responsejs["friendid"];
        string friendname = responsejs["friendname"];
        string friendpicture = responsejs["friendpicture"];
        user.setId(friendid);
        user.setName(friendname);
        emit friendAdded(g_currentUser.getId(), g_currentUser.getName());
        title = "添加好友成功";
        msg = "可以开始和该好友进行聊天啦";
        type = "information";
    }
    emit friendInfo(type, title, msg);
}

// 处理删除好友业务逻辑
void ClientCore::doDeleteFriendResponse(const json &responsejs){
    int friendid = responsejs["friendid"];
    // for(std::vector<User>::iterator it = g_currentUserFriendList.begin(); it != g_currentUserFriendList.end(); it++){
    //     if(it->getId() == friendid){
    //         g_currentUserFriendList.erase(it);
    //         break;
    //     }
    // }

    QMetaObject::invokeMethod(this, [this, friendid](){
        // 从本地好友列表中删除
        auto& friends = g_currentUserFriendList;
        friends.erase(std::remove_if(friends.begin(), friends.end(), [friendid](User user) {return user.getId() == friendid;}), friends.end());
        
        emit friendDeleted(friendid);
    }, Qt::QueuedConnection);

    emit friendInfo("information", "删除好友成功", "您已无法与该好友进行聊天");
}

// 处理创建群聊业务逻辑
void ClientCore::doCreateGroupResponse(const json &responsejs){
    QString title, msg, type;
    if(0 != responsejs["errno"].get<int>()){
        title = "创建群组失败";
        msg = "该名称已存在";
        type = "warning";
        cerr << "groupname is already exist, create group error!" << endl;
    }
    else{
        Group group;
        int groupid = responsejs["groupid"];
        string groupname = responsejs["groupname"];
        string groupdesc = responsejs["groupdesc"];
        group.setId(groupid);
        group.setName(groupname);
        group.setDesc(groupdesc);
        g_currentUserGroupList.push_back(group);
        emit groupAdded(groupid, groupname);
        title = "创建群组成功";
        msg = "群组id为 " + QString::number(groupid) + "，可以将此id分享给好友，邀请其加入群组";
        type = "information";
    }
    emit groupInfo(type, title, msg);
}

// 处理加入群聊业务逻辑
void ClientCore::doAddGroupResponse(const json &responsejs){
    QString title, msg, type;
    if(0 != responsejs["errno"].get<int>()){
        title = "加入群组失败";
        msg = "输入的groupid不存在";
        type = "warning";
        cerr << "groupid is not exist, add group error!" << endl;
    }
    else{
        Group group;
        int groupid = responsejs["groupid"];
        string groupname = responsejs["groupname"];
        string groupdesc = responsejs["groupdesc"];
        group.setId(groupid);
        group.setName(groupname);
        group.setDesc(groupdesc);
        g_currentUserGroupList.push_back(group);
        emit groupAdded(groupid, groupname);
        title = "加入群组成功";
        msg = "可以开始在该群组中进行聊天啦";
        type = "information";
    }
    emit groupInfo(type, title, msg);
}

// 处理退出群聊业务逻辑
void ClientCore::doQuitGroupResponse(const json &responsejs){
    int groupid = responsejs["groupid"];
    for(std::vector<Group>::iterator it = g_currentUserGroupList.begin(); it != g_currentUserGroupList.end(); it++){
        if(it->getId() == groupid){
            g_currentUserGroupList.erase(it);
            break;
        }
    }
    
    emit groupInfo("information", "退出群组成功", "您已无法在该群组中进行聊天");
}

// 处理解散群聊业务逻辑
void ClientCore::doDissolveGroupResponse(const json &responsejs){
    int groupid = responsejs["groupid"];
    // for(std::vector<Group>::iterator it = g_currentUserGroupList.begin(); it != g_currentUserGroupList.end(); it++){
    //     if(it->getId() == groupid){
    //         g_currentUserGroupList.erase(it);
    //         break;
    //     }
    // }

    // 使用 QMetaObject::invokeMethod 确保在主线程执行
    QMetaObject::invokeMethod(this, [this, groupid]() {
        // 从本地群组列表中移除
        auto& groups = g_currentUserGroupList;
        groups.erase(std::remove_if(groups.begin(), groups.end(), [groupid](Group group) {return group.getId() == groupid;}), groups.end());
        
        emit groupDissolved(groupid);
    }, Qt::QueuedConnection);

    emit groupInfo("information", "解散群组成功", "该群组信息已清除");
}

ClientCore::ClientCore(QObject *parent) : QObject(parent) 
{
#ifdef _WIN32
    initWSA();
#endif
}

ClientCore::~ClientCore()
{
    closeSocket();
    
    if (m_readThread && m_readThread->isRunning()) {
        m_running = false;
        m_readThread->quit();
        m_readThread->wait();
        delete m_readThread;
    }
    
#ifdef _WIN32
    if (s_wsaInitialized) {
        WSACleanup();
        s_wsaInitialized = false;
    }
#endif
}

bool ClientCore::connectToServer(const QString &ip, quint16 port)
{
    if (m_clientfd != -1) {
        return true; // 已经连接
    }

#ifdef _WIN32
    m_clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    m_clientfd = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (m_clientfd == -1) {
        qWarning() << "Socket creation error";
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
#ifdef _WIN32
    InetPtonA(AF_INET, ip.toStdString().c_str(), &serverAddr.sin_addr);
#else
    serverAddr.sin_addr.s_addr = inet_addr(ip.toStdString().c_str());
#endif

#ifdef _WIN32
    if (::connect(m_clientfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        qWarning() << "Connection failed, error:" << WSAGetLastError();
        closesocket(m_clientfd);
        m_clientfd = -1;
        return false;
    }
#else
    if (::connect(m_clientfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        qWarning() << "Connection failed";
        close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
#endif

    m_running = true;
    m_readThread = QThread::create([this] { readTaskHandler(); });
    m_readThread->start();
    
    return true;
}

void ClientCore::readTaskHandler()
{
    while (m_running) {
        char buffer[4096] = {0};
        
#ifdef _WIN32
        int len = recv(m_clientfd, buffer, sizeof(buffer), 0);
        if (len == SOCKET_ERROR || len == 0) {
            emit loginFailed("Connection lost");
            break;
        }
#else
        int len = recv(m_clientfd, buffer, sizeof(buffer), 0);
        if (len <= 0) {
            emit loginFailed("Connection lost");
            break;
        }
#endif

        try {
            json js = json::parse(buffer);
            int msgtype = js["msgid"].get<int>();
            
            if (LOGIN_MSG_ACK == msgtype) {
                if (0 == js["errno"].get<int>()) {
                    // doLoginResponse(js);
                    emit loginSuccess(js);
                } else {
                    emit loginFailed(QString::fromStdString(js["errmsg"].get<std::string>()));
                }
            }
            else if (REG_MSG_ACK == msgtype) {
                if (0 == js["errno"].get<int>()) {
                    emit registrationSuccess(js["id"].get<int>());
                } else {
                    QString errorMsg = QString::fromStdString(js["errmsg"].get<std::string>());
                    emit registrationFailed(errorMsg);
                }
            }
            else if (ADD_FRIEND_ACK == msgtype) {
                // emit recvAddFriendMsg(js);
                doAddFriendResponse(js);
                continue;
            }
            else if (FRIEND_DELETED_MSG == msgtype) {
                doDeleteFriendResponse(js);
                continue;
            }
            else if (CREATE_GROUP_ACK == msgtype) {
                doCreateGroupResponse(js);
                continue;
            }
            else if (ADD_GROUP_ACK == msgtype) {
                doAddGroupResponse(js);
                continue;
            }
            else if (QUIT_GROUP_ACK == msgtype) {
                doQuitGroupResponse(js);
                continue;
            }
            else if (GROUP_DISSOLVED_MSG == msgtype) {
                doDissolveGroupResponse(js);
                continue;
            }
            else if (ONE_CHAT_MSG == msgtype) {
                emit recvOneChatMsg(js);
                continue;
            }
            else if (GROUP_CHAT_MSG == msgtype) {
                emit recvGroupChatMsg(js);
                continue;
            }
            else if (MODIFY_NAME_ACK == msgtype) {
                emit nameModified(js);
                continue;
            }
            else if (MODIFY_PASSWORD_ACK == msgtype) {
                emit passwordModified(js);
                continue;
            }
            else if (MODIFY_PICTURE_ACK == msgtype) {
                emit pictureChanged(js);
            }
            else if (FRIEND_INFO_CHANGED == msgtype) {
                emit friendAdded(js["id"], js["name"]);
                continue;
            }
        } catch (...) {
            qWarning() << "Failed to parse server message";
        }
    }
}

void ClientCore::sendJson(const json &js)
{
    if (m_clientfd == -1) {
        emit loginFailed("Not connected to server");
        return;
    }
    
    std::string buffer = js.dump();
    
#ifdef _WIN32
    if (send(m_clientfd, buffer.c_str(), buffer.size(), 0) == SOCKET_ERROR) {
        emit loginFailed("Failed to send data");
    }
#else
    if (send(m_clientfd, buffer.c_str(), buffer.size(), 0) == -1) {
        emit loginFailed("Failed to send data");
    }
#endif
    cout << buffer << endl;
}

void ClientCore::login(int id, const QString &password)
{
    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = id;
    js["password"] = password.toStdString();
    sendJson(js);
}

void ClientCore::registerUser(const QString &name, const QString &password)
{
    json js;
    js["msgid"] = REG_MSG;
    js["name"] = name.toStdString();
    js["password"] = password.toStdString();
    sendJson(js);
}
