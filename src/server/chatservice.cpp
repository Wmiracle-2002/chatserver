#include "chatservice.hpp"
#include "public.hpp"

// 获取单例对象的接口函数
ChatService* ChatService::instance(){
    static ChatService service;
    return &service;
}

// 构造函数
ChatService::ChatService(){
    _msgHanderMap.insert({LOGIN_MSG, bind(&ChatService::login, this, _1, _2, _3)});
    _msgHanderMap.insert({LOGOUT_MSG, bind(&ChatService::logout, this, _1, _2, _3)});
    _msgHanderMap.insert({REG_MSG, bind(&ChatService::reg, this, _1, _2, _3)});
    _msgHanderMap.insert({ONE_CHAT_MSG, bind(&ChatService::oneChat, this, _1, _2, _3)});
    _msgHanderMap.insert({ADD_FRIEND_MSG, bind(&ChatService::addFriend, this, _1, _2, _3)});
    _msgHanderMap.insert({REMOVE_FRIEND_MSG, bind(&ChatService::removeFriend, this, _1, _2, _3)});
    _msgHanderMap.insert({CREATE_GROUP_MSG, bind(&ChatService::createGroup, this, _1, _2, _3)});
    _msgHanderMap.insert({ADD_GROUP_MSG, bind(&ChatService::addGroup, this, _1, _2, _3)});
    _msgHanderMap.insert({GROUP_CHAT_MSG, bind(&ChatService::groupChat, this, _1, _2, _3)});

    // 连接redis服务器
    if(_redis.connect()){
        // 设置上报消息的回调
        _redis.init_notify_handler(std::bind(&ChatService::handleRedisSubscribeMessage, this, _1, _2));
    }
}

// 析构函数
ChatService::~ChatService(){
    
}

// 处理登录业务
void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time){
    //LOG_INFO << "do login service!!!";
    int id = js["id"];
    string password = js["password"];
    User user = _userModel.query(*cp, id);
    if(user.getId() == id && user.getPwd() == password){
        if(user.getState() == "online"){
            // 重复登陆
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "this id had been already online!";
            conn->send(response.dump());
            return;
        }
        // 登陆成功
        {
            lock_guard<mutex> lock(_connMutex);
            // 记录用户连接信息
            _userConnectionMap.insert({id, conn});
        }
        // id用户登录成功后，向redis订阅channel(id)
        _redis.subscribe(id);
        // 更新登陆状态
        user.setState("online");
        _userModel.updateState(*cp, user);
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        response["name"] = user.getName();
        // 查询该用户是否有离线消息
        vector<string> vec = _offlineMsgModel.query(*cp, id);
        if(!vec.empty()){
            response["offlinemsg"] = vec;
            _offlineMsgModel.remove(*cp, id);
        }
        // 查询该用户的好友信息并返回
        vector<User> userVec = _friendModel.query(*cp, id);
        if(!userVec.empty()){
            vector<string> vec2;
            for(User& user : userVec){
                json js;
                js["id"] = user.getId();
                js["name"] = user.getName();
                js["state"] = user.getState();
                vec2.push_back(js.dump());
            }
            response["friends"] = vec2;
        }
        // 查询用户的群组信息
        vector<Group> groupuserVec = _groupModel.queryGroups(*cp, id);
        if (!groupuserVec.empty()){
            // group:[{groupid:[xxx, xxx, xxx, xxx]}]
            vector<string> groupV;
            for (Group &group : groupuserVec){
                json grpjson;
                grpjson["id"] = group.getId();
                grpjson["groupname"] = group.getName();
                grpjson["groupdesc"] = group.getDesc();
                vector<string> userV;
                for (GroupUser &user : group.getUsers()){
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    js["role"] = user.getRole();
                    userV.push_back(js.dump());
                }
                grpjson["users"] = userV;
                groupV.push_back(grpjson.dump());
            }
            response["groups"] = groupV;
        }
        conn->send(response.dump());
    }
    else{
        // 登陆失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "id or password error!";
        conn->send(response.dump());
    }
}
// 处理注册业务
void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time){
    // LOG_INFO << "do reg service!!!";
    string name = js["name"];
    string password = js["password"];
    User user;
    user.setName(name);
    user.setPwd(password);
    bool state = _userModel.insert(*cp, user);
    if(state){
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else{
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "register fail!";
        conn->send(response.dump());
    }
}

// 一对一聊天业务
void ChatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int toid = js["toid"];
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnectionMap.find(toid);
        if(it != _userConnectionMap.end()){
            // 对方在线，转发消息
            it->second->send(js.dump());
            return;
        }
    }
    // 查询对方是否在线
    User user = _userModel.query(*cp, toid);
    if(user.getState() == "online"){
        _redis.publish(toid, js.dump());
        return;
    }
    // 对方不在线，存储离线消息
    _offlineMsgModel.insert(*cp, toid, js.dump());
}

// 添加好友业务
void ChatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    int friendid = js["friendid"];
    // 存储好友信息
    _friendModel.insert(*cp, userid, friendid);
}

// 删除好友业务
void ChatService::removeFriend(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    int friendid = js["friendid"];
    // 删除好友信息
    _friendModel.remove(*cp, userid, friendid);
}

// 创建群组业务
void ChatService::createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    string groupname = js["groupname"];
    string groupdesc = js["groupdesc"];
    // 存储新创建的群组信息
    Group group(-1, groupname, groupdesc);
    if(_groupModel.createGroup(*cp, group)){
        // 存储群组创建人信息
        _groupModel.addGroup(*cp, userid, group.getId(), "creator");
    }
}

// 加入群组业务
void ChatService::addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    int groupid = js["groupid"];
    _groupModel.addGroup(*cp, userid, groupid, "normal");
}

// 群聊天业务
void ChatService::groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    int groupid = js["groupid"];
    vector<int> useridVec = _groupModel.queryGroupUsers(*cp, userid, groupid);
    lock_guard<mutex> lock(_connMutex);
    for(int id : useridVec){
        auto it = _userConnectionMap.find(id);
        if(it != _userConnectionMap.end()){
            it->second->send(js.dump());
        }
        else{
            // 查询id是否在线
            User user = _userModel.query(*cp, id);
            if(user.getState() == "online"){
                _redis.publish(id, js.dump());
            }
            else{
                _offlineMsgModel.insert(*cp, id, js.dump());
            }
        }
    }
}

// 处理注销业务
void ChatService::logout(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"];
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnectionMap.find(userid);
        if(it != _userConnectionMap.end()){
            _userConnectionMap.erase(it);
        }
    }
    // 用户注销，在redis中取消订阅通道
    _redis.unsubscribe(userid);
    // 更新用户状态信息
    User user(userid, "", "", "offline");
    _userModel.updateState(*cp, user);
}

// 获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid){
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = _msgHanderMap.find(msgid);
    if(it == _msgHanderMap.end()){
        return [=](const TcpConnectionPtr& conn, json& js, Timestamp time){
            LOG_ERROR << "msgid:" << msgid << " can not find handler!";
        };
    }
    else{
        return _msgHanderMap[msgid];
    }
}

// 从reids消息队列中获取订阅的消息
void ChatService::handleRedisSubscribeMessage(int userid, string msg){
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnectionMap.find(userid);
    if(it != _userConnectionMap.end()){
        it->second->send(msg);
        return;
    }
    // 存储该用户的离线消息
    _offlineMsgModel.insert(*cp, userid, msg);
}

// 服务器异常，业务重置
void ChatService::reset(){
    // 把online状态的用户设置成offline
    _userModel.resetState(*cp);
}

// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr& conn){
    User user;
    {
        lock_guard<mutex> lock(_connMutex);
        for(auto it = _userConnectionMap.begin(); it != _userConnectionMap.end(); it++){
            if(it->second == conn){
                // 从map表删除用户的连接信息
                user.setId(it->first);
                _userConnectionMap.erase(it);
                break;
            }
        }
    }
    // 在redis中取消订阅通道
    _redis.unsubscribe(user.getId());

    // 更新用户的状态信息
    if(user.getId() != -1){
        user.setState("offline");
        _userModel.updateState(*cp, user);
    }
}