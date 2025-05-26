#ifndef USERMODEL_H
#define USERMODEL_H

#include "db.h"
#include "user.hpp"
#include <iostream>

using namespace std;

class UserModel{
public:
    // User表的增加方法
    bool insert(User& user);
    // User表的删除方法
    // User表的修改方法
    bool updateState(User user);
    // User表的查询方法
    User query(int id);
    // 重置用户的状态信息
    void resetState();
private:

};

#endif