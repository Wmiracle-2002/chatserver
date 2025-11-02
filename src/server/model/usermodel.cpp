#include "usermodel.hpp"

// User表的增加方法
bool UserModel::insert(ConnectionPool &cp, User& user){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name, password, state) values('%s', '%s', '%s')", user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->update(sql)){
        // 获取插入成功的用户数据生成的主键id
        user.setId(mysql->getConnId());
        return true;
    }
    return false;
}

// User表的修改在线状态方法
bool UserModel::updateState(ConnectionPool &cp, User user){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set state = '%s' where id = %d", user.getState().c_str(), user.getId());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->update(sql)){
        return true;
    }
    return false;
}

// User表的修改用户名方法
bool UserModel::updateUsername(ConnectionPool &cp, User user){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set name = '%s' where id = %d", user.getName().c_str(), user.getId());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->update(sql)){
        return true;
    }
    return false;
}

// User表的修改密码方法
bool UserModel::updatePassword(ConnectionPool &cp, User user, string new_pwd){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select password from User where id = %d", user.getId());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->query(sql)){
        mysql->next();
        MYSQL_ROW row = mysql->getRow();
        if(row == nullptr || user.getPwd() != row[0]){
            return false;
        }
    }
    sprintf(sql, "update User set password = '%s' where id = %d", new_pwd.c_str(), user.getId());
    if(mysql->update(sql)){
        return true;
    }
    return false;
}

// User表的查询方法
User UserModel::query(ConnectionPool &cp, int id){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d", id);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->query(sql)){
        while(mysql->next()){
            MYSQL_ROW row = mysql->getRow();
            if(row != nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                return user;
            }
        }
    }
    return User();
}

// 重置用户的状态信息
void UserModel::resetState(ConnectionPool &cp){
    // 组装sql语句
    char sql[1024] = "update User set state = 'offline' where state = 'online'";
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql);
}