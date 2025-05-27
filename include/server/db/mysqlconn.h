#ifndef MYSQLCONN_H
#define MYSQLCONN_H

#include <mysql/mysql.h>
#include <string>
#include <muduo/base/Logging.h>
#include <chrono>

using namespace std;
using namespace chrono;

// 数据库操作类
class MysqlConn{
public:
    // 初始化数据库连接
    MysqlConn();
    // 释放数据库连接
    ~MysqlConn();
    // 连接数据库
    bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
    // 更新操作
    bool update(string sql);
    // 查询数据库
    bool query(string sql);
    // 遍历查询得到的结果值
    bool next();
    // 取出结果值
    MYSQL_ROW getRow();
    // 得到结果集中的字段值
    string value(int index);
    // 获取连接id
    int getConnId();
    // 事务操作
    bool transaction();
    // 提交事务
    bool commit();
    // 事务回滚
    bool rollback();
    // 刷新起始的空闲时间点
    void refreshAliveTime();
    // 计算连接存活的总时长
    long long getAliveTime();
private:
    void freeResult();
    MYSQL* _conn;
    MYSQL_RES* _result = nullptr;
    MYSQL_ROW _row = nullptr;
    steady_clock::time_point _alivetime;
};

#endif