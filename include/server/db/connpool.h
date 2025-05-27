#ifndef CONNPOOL_H
#define CONNPOOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "mysqlconn.h"
#include <json.hpp>
#include <fstream>
#include <thread>

using namespace std;

class ConnectionPool{
public:
	// 静态成员函数通过类名获取实例
	static ConnectionPool* getConnectPool();
	// 删除拷贝构造函数
	ConnectionPool(const ConnectionPool& obj) = delete;
	// 删除赋值运算重载
	ConnectionPool& operator=(const ConnectionPool& obj) = delete;
	shared_ptr<MysqlConn> getConnection();
	~ConnectionPool();
private:
	ConnectionPool();
	bool parseJsonFile();
	void produceConnection();
	void recycleConnection();
	void addConnection();
	string _ip;
	string _username;
	string _password;
	string _dbName;
	unsigned short _port;
	int _minSize;
	int _maxSize;
	int _timeout;
	int _maxIdleTime;
	bool _connState;
	queue<MysqlConn*> _connectionQ;
	mutex _mutexQ;
	condition_variable _cond;
};

#endif