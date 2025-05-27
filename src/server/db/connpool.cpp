#include "connpool.h"
#include <iostream>

using json = nlohmann::json;

// 静态成员函数通过类名获取实例
ConnectionPool* ConnectionPool::getConnectPool(){
	// 静态局部变量
	static ConnectionPool pool;
	return &pool;
}

// 解析json文件
bool ConnectionPool::parseJsonFile(){
	ifstream ifs;
	ifs.open("dbconfig.json", ios::in);
	if(ifs){
        json js = json::parse(ifs);
	    _ip = js["ip"];
        _port = js["port"].get<unsigned short>();
	    _username = js["username"];
	    _password = js["password"];
	    _dbName = js["dbName"];
	    _minSize = js["minSize"].get<int>();
	    _maxSize = js["maxSize"].get<int>();
	    _maxIdleTime = js["maxIdleTime"].get<int>();
	    _timeout = js["timeout"].get<int>();
        ifs.close();
        return true;
    }
    ifs.close();
    return false;
}

// 生产连接
void ConnectionPool::produceConnection(){
	while(true){
		unique_lock<mutex> locker(_mutexQ);
		while(_connectionQ.size() >= _minSize){
			_cond.wait(locker);
		}
		addConnection();
		_cond.notify_all();
	}
}

// 消费连接
void ConnectionPool::recycleConnection(){
	while(true){
		this_thread::sleep_for(chrono::milliseconds(500));
		lock_guard<mutex> locker(_mutexQ);
		while(_connectionQ.size() > _minSize){
			MysqlConn* conn = _connectionQ.front();
			if(conn->getAliveTime() >= _maxIdleTime){
				_connectionQ.pop();
				delete conn;
			}
			else{
				break;
			}
		}
	}
}

// 增加连接数
void ConnectionPool::addConnection() {
	MysqlConn* conn = new MysqlConn();
	conn->connect(_username, _password, _dbName, _ip, _port);
	conn->getAliveTime();
	_connectionQ.push(conn);
}

// 获取连接
shared_ptr<MysqlConn> ConnectionPool::getConnection(){
	unique_lock<mutex> locker(_mutexQ);
	while(_connectionQ.empty()){
		if(!_connState) break;
		if(cv_status::timeout == _cond.wait_for(locker, chrono::milliseconds(_timeout))){
			if(_connectionQ.empty()){
				continue;
			}
		}
	}
	shared_ptr<MysqlConn> connptr(_connectionQ.front(), [this](MysqlConn* conn){
		lock_guard<mutex> locker(_mutexQ);	// 创建位置生成锁，析构位置解锁
		conn->refreshAliveTime();
		_connectionQ.push(conn);
		});
	_connectionQ.pop();
	_cond.notify_all();
	return connptr;
}

// 析构函数
ConnectionPool::~ConnectionPool(){
	while(!_connectionQ.empty()){
		MysqlConn* conn = _connectionQ.front();
		_connectionQ.pop();
		delete conn;
	}
	_connState = false;
}

// 构造函数
ConnectionPool::ConnectionPool() {
	// 加载配置文件
	if(!parseJsonFile()){
		cout << "cannot find config file!" << endl;
		return;
	}
	_connState = true;
	for(int i = 0; i < _minSize; i++){
		addConnection();
	}
	thread producer(&ConnectionPool::produceConnection, this);
	thread recycler(&ConnectionPool::recycleConnection, this);
	producer.detach();
	recycler.detach();
}