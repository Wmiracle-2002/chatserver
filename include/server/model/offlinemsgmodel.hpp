#ifndef OFFLINEMSGMODEL_H
#define OFFLINEMSGMODEL_H

#include <string>
#include <vector>
#include "connpool.h"

using namespace std;

// 提供离线消息表的操作接口
class OfflineMsgModel{
public:
    // 存储用户的离线消息
    void insert(ConnectionPool &cp, int userid, string msg);
    // 删除用户的离线消息
    void remove(ConnectionPool &cp, int userid);
    // 查询用户的离线消息
    vector<string> query(ConnectionPool &cp, int userid);
};

#endif