#include "chatserver.hpp"
#include "chatservice.hpp"
#include "httpserver.hpp"
#include "connpool.h"
#include <iostream>
#include <signal.h>

using namespace std;

// 处理服务器结束后重置user状态信息
void resetHandler(int){
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char *argv[]){
    
    char *ip = "0.0.0.0";
    uint16_t port = 8000;
    signal(SIGINT, resetHandler);

    // 创建 HTTP 服务器对象
    auto httpServer = std::make_shared<HttpServer>(8080, "./picture");
    
    // 在单独线程中启动 HTTP 服务器（阻塞方式）
    std::thread httpThread([httpServer]() {
        httpServer->run();
    });

    // 启动TCP聊天服务器
    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");
    server.start();
    loop.loop();

    // 当 TCP 服务器停止时，停止 HTTP 服务器
    httpServer->stop();
    if (httpThread.joinable()) {
        httpThread.join();
    }
    return 0;
}