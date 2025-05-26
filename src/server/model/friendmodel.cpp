#include "friendmodel.hpp"
#include "db.h"

// 添加好友关系
void FriendModel::insert(int userid, int friendid){
    // 组装sql语句
    char sql1[1024] = {0};
    char sql2[1024] = {0};
    sprintf(sql1, "insert into Friend values(%d, %d)", userid, friendid);
    sprintf(sql2, "insert into Friend values(%d, %d)", friendid, userid);
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql1);
        mysql.update(sql2);
    }
}

// 返回用户的好友列表
vector<User> FriendModel::query(int userid){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state from User a inner join Friend b on b.friendid = a.id where b.userid = %d", userid);
    MySQL mysql;
    vector<User> vec;
    if(mysql.connect()){
        MYSQL_RES *res =  mysql.query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}

// 删除好友关系
void FriendModel::remove(int userid, int friendid){
    // 组装sql语句
    char sql1[1024] = {0};
    char sql2[1024] = {0};
    sprintf(sql1, "delete from Friend where userid = %d and friendid = %d", userid, friendid);
    sprintf(sql2, "delete from Friend where userid = %d and friendid = %d", friendid, userid);
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql1);
        mysql.update(sql2);
    }
}