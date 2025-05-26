#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>

/*基于muduo网络库开发服务器程序
1. 组合TcpServer对象
2. 创建EventLoop事件循环对象的指针
3. 明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4. 在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写的回调函数
5. 设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/
class ChatServer{
public:
    // 构造函数
    ChatServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, const std::string& nameArg)
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        // 设置服务器端的线程数量 1个I/O线程，3个worker线程
        _server.setThreadNum(4);
    }
    // 开启事件循环
    void start(){
        _server.start();
    }
private:
    // 专门用于处理用户的连接创建和断开
    void onConnection(const muduo::net::TcpConnectionPtr& cptr){
        if(cptr->connected()){
            std::cout << cptr->peerAddress().toIpPort() << " -> " << cptr->localAddress().toIpPort() << "state:online" << std::endl;
        }
        else{
            std::cout << cptr->peerAddress().toIpPort() << " -> " << cptr->localAddress().toIpPort() << "state:offline" << std::endl;
            cptr->shutdown();
        }
    }
    void onMessage(const muduo::net::TcpConnectionPtr& cptr, muduo::net::Buffer *buffer, muduo::Timestamp time){
        std::string buf = buffer->retrieveAllAsString();
        std::cout << "receive data:" << buf << " time:" << time.toString() << std::endl;
        cptr->send(buf);
    }
    muduo::net::TcpServer _server;
    muduo::net::EventLoop *_loop;
};

int main(){
    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");
    
    server.start();
    loop.loop();
    
    return 0;
}