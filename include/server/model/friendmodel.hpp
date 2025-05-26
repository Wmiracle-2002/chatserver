#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include <vector>

using namespace std;

// 维护好友信息
class FriendModel{
public:
    // 添加好友关系
    void insert(int userid, int friendid);
    // 返回用户的好友列表
    vector<User> query(int userid);
    // 删除好友关系
    void remove(int userid, int friendid);
};

#endif