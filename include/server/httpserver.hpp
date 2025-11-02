#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <httplib.h>
#include <json.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <filesystem>

// 前向声明
class PictureHandler;

class HttpServer {
public:
    explicit HttpServer(int port = 8080, const std::string& pictureBasePath = "./picture");
    ~HttpServer();
    // 启动Http
    void run();
    // 停止Http
    void stop();
    // 判断Http是否在运行
    bool isRunning() const;

private:
    void setupRoutes();
    // 处理头像上传请求
    void handlePictureUpload(const httplib::Request& req, httplib::Response& res);
    // 处理头像下载请求
    void handlePictureDownload(const httplib::Request& req, httplib::Response& res);
    // 处理直接文件访问请求
    void handleFileAccess(const httplib::Request& req, httplib::Response& res);
    // 处理健康检查请求
    void handleHealthCheck(const httplib::Request& req, httplib::Response& res);
    // 处理未找到的路由
    void handleNotFound(const httplib::Request& req, httplib::Response& res);
    // 设置CORS头（如果需要支持Web前端）
    void setCorsHeaders(httplib::Response& res);
    // 验证用户ID格式
    bool isValidUserId(const std::string& userIdStr) const;
    // 发送错误响应
    void sendErrorResponse(httplib::Response& res, int status, const std::string& message);

private:
    int port_;                              // HTTP服务器端口
    std::string pictureBasePath_;           // 头像存储基础路径
    std::unique_ptr<httplib::Server> server_; // HTTP服务器实例
    std::unique_ptr<PictureHandler> pictureHandler_; // 头像处理器
    bool isRunning_;                       // 服务器运行状态
};

#endif // !HTTPSERVER_H