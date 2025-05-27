#include "offlinemsgmodel.hpp"

// 存储用户的离线消息
void OfflineMsgModel::insert(ConnectionPool &cp, int userid, string msg){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into OfflineMessage values(%d, '%s')", userid, msg.c_str());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql);
}

// 删除用户的离线消息
void OfflineMsgModel::remove(ConnectionPool &cp, int userid){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid = %d", userid);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql);
}

// 查询用户的离线消息
vector<string> OfflineMsgModel::query(ConnectionPool &cp, int userid){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid = %d", userid);
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    vector<string> vec;
    if(mysql->query(sql)){
        while(mysql->next()){
            MYSQL_ROW row = mysql->getRow();
            if(row != nullptr){
                vec.push_back(row[0]);
            }
        }
    }
    return vec;
}