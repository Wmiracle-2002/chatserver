#ifndef PUBLIC_H
#define PUBLIC_H

enum MsgType{
    LOGIN_MSG = 1,  // 登录消息
    LOGIN_MSG_ACK,  // 登录响应消息
    LOGOUT_MSG, // 注销消息
    REG_MSG, // 注册消息
    REG_MSG_ACK, // 注册响应消息
    ONE_CHAT_MSG,    // 聊天消息
    ADD_FRIEND_MSG,  // 添加好友消息
    ADD_FRIEND_ACK, // 添加好友响应消息
    REMOVE_FRIEND_MSG,  // 删除好友消息
    CREATE_GROUP_MSG,   // 创建群组消息
    CREATE_GROUP_ACK,   // 创建群组响应消息
    ADD_GROUP_MSG,  // 加入群组消息
    ADD_GROUP_ACK,  // 加入群组响应消息
    QUIT_GROUP_MSG, // 退出群组消息
    QUIT_GROUP_ACK, // 退出群组响应消息
    GROUP_CHAT_MSG,  // 群聊天消息
    GROUP_DISSOLVED_MSG, // 群组已解散消息
    FRIEND_DELETED_MSG,  // 好友已删除消息
    MODIFY_NAME_MSG,    // 修改用户名消息
    MODIFY_NAME_ACK,    // 修改用户名响应消息
    MODIFY_PASSWORD_MSG,    // 修改密码消息
    MODIFY_PASSWORD_ACK,    // 修改密码响应消息
    FRIEND_INFO_CHANGED,    // 好友信息更改消息
    MODIFY_PICTURE_MSG, // 头像更改信息
    MODIFY_PICTURE_ACK, // 头像更改响应信息
};

#endif