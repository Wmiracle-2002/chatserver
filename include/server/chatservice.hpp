#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include <mutex>
#include <vector>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Logging.h>
#include "json.hpp"
#include "redis.hpp"
#include "usermodel.hpp"
#include "offlinemsgmodel.hpp"
#include "friendmodel.hpp"
#include "group.hpp"
#include "groupmodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;
// 表示处理消息事件的回调函数类型
using MsgHandler = function<void(const TcpConnectionPtr&, json&, Timestamp)>;

// 聊天服务器业务类（单例模式）
class ChatService{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();
    // 处理登录业务
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 删除好友业务
    void removeFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 群聊天业务
    void groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 处理注销业务
    void logout(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 从reids消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
    // 服务器异常，业务重置
    void reset();
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);
private:
    ChatService();

    unordered_map<int, MsgHandler> _msgHanderMap;   // 存储消息id和其对应的业务处理方法
    unordered_map<int, TcpConnectionPtr> _userConnectionMap;    // 存储在线用户的通信连接
    mutex _connMutex; // 互斥锁，保证_userConnectionMap的线程安全
    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;
};

#endif