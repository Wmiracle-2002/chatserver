#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <string>
#include <vector>
#include "group.hpp"
#include "connpool.h"

using namespace std;

class GroupModel{
public:
    // 创建群组
    bool createGroup(ConnectionPool &cp, Group &group);
    // 加入群组
    void addGroup(ConnectionPool &cp, int userid, int groupid, string role);
    // 查询用户所在群组信息
    vector<Group> queryGroups(ConnectionPool &cp, int userid);
    // 根据指定的groupid查询群组用户id列表，除userid自己，主要用于群聊业务给群组其它成员群发消息
    vector<int> queryGroupUsers(ConnectionPool &cp, int userid, int groupid);
};

#endif