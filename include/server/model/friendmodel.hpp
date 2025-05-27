#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include <vector>
#include "connpool.h"

using namespace std;

// 维护好友信息
class FriendModel{
public:
    // 添加好友关系
    void insert(ConnectionPool &cp, int userid, int friendid);
    // 返回用户的好友列表
    vector<User> query(ConnectionPool &cp, int userid);
    // 删除好友关系
    void remove(ConnectionPool &cp, int userid, int friendid);
};

#endif