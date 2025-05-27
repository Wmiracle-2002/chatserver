#include "mysqlconn.h"

// 初始化数据库连接
MysqlConn::MysqlConn(){
    _conn = mysql_init(nullptr);
    mysql_set_character_set(_conn, "utf8");
}

// 释放数据库连接
MysqlConn::~MysqlConn(){
    if(_conn != nullptr){
        mysql_close(_conn);
    }
    freeResult();
}

// 连接数据库
bool MysqlConn::connect(string user, string passwd, string dbName, string ip, unsigned short port){
    MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    if(p != nullptr){
        LOG_INFO << "connect mysql success!";
    }
    else{
        LOG_INFO << "connect mysql fail!";
    }
    return p != nullptr;
}

// 更新操作
bool MysqlConn::update(string sql){
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "更新失败！";
        return false;
    }
    return true;
}
#include <iostream>
// 查询数据库
bool MysqlConn::query(string sql){
    freeResult();
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "查询失败！";
        return false;
    }
    _result = mysql_store_result(_conn);
    return true;
}

// 遍历查询得到的结果值
bool MysqlConn::next(){
    if(_result != nullptr){
        _row = mysql_fetch_row(_result);
        if(_row != nullptr){
            return true;
        }
    }
    return false;
}

// 取出结果值
MYSQL_ROW MysqlConn::getRow(){
    return _row;
}

// 得到结果集中的字段值
string MysqlConn::value(int index){
    int fieldCount = mysql_num_fields(_result);
    if(index >= fieldCount || index < 0){
        return string();
    }
    char *val = _row[index];
    unsigned long length = mysql_fetch_lengths(_result)[index];
    return string(val, length);
}

// 获取连接id
int MysqlConn::getConnId(){
    return mysql_insert_id(_conn);
}

// 事务操作
bool MysqlConn::transaction(){
    return mysql_autocommit(_conn, false);
}

// 提交事务
bool MysqlConn::commit(){
    return mysql_commit(_conn);
}

// 事务回滚
bool MysqlConn::rollback(){
    return mysql_rollback(_conn);
}

// 刷新起始的空闲时间点
void MysqlConn::refreshAliveTime(){
    _alivetime = steady_clock::now();
}

// 计算连接存活的总时长
long long MysqlConn::getAliveTime(){
    nanoseconds res = steady_clock::now() - _alivetime;
    milliseconds millsec = duration_cast<milliseconds>(res);
    return millsec.count();
}

// 释放结果资源
void MysqlConn::freeResult() {
	if(_result){
		mysql_free_result(_result);
		_result = nullptr;
	}
}