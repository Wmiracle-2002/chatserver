#include "friendmodel.hpp"

// 添加好友关系
void FriendModel::insert(ConnectionPool &cp, int userid, int friendid){
    // 组装sql语句
    char sql1[1024] = {0};
    char sql2[1024] = {0};
    sprintf(sql1, "insert into Friend values(%d, %d)", userid, friendid);
    sprintf(sql2, "insert into Friend values(%d, %d)", friendid, userid);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql1);
    mysql->update(sql2);
}

// 返回用户的好友列表
vector<User> FriendModel::query(ConnectionPool &cp, int userid){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state from User a inner join Friend b on b.friendid = a.id where b.userid = %d", userid);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    vector<User> vec;
    if(mysql->query(sql)){
        while(mysql->next()){
            MYSQL_ROW row = mysql->getRow();
            if(row != nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
        }
    }
    return vec;
}

// 删除好友关系
void FriendModel::remove(ConnectionPool &cp, int userid, int friendid){
    // 组装sql语句
    char sql1[1024] = {0};
    char sql2[1024] = {0};
    sprintf(sql1, "delete from Friend where userid = %d and friendid = %d", userid, friendid);
    sprintf(sql2, "delete from Friend where userid = %d and friendid = %d", friendid, userid);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql1);
    mysql->update(sql2);
}