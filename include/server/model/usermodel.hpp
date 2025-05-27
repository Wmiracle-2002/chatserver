#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"
#include <iostream>
#include "connpool.h"

using namespace std;

class UserModel{
public:
    // User表的增加方法
    bool insert(ConnectionPool &cp, User& user);
    // User表的删除方法
    // User表的修改方法
    bool updateState(ConnectionPool &cp, User user);
    // User表的查询方法
    User query(ConnectionPool &cp, int id);
    // 重置用户的状态信息
    void resetState(ConnectionPool &cp);
};

#endif