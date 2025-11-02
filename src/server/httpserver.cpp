#include "httpserver.hpp"
#include "picturehandler.hpp"
#include <thread>
#include <atomic>
#include <filesystem>

using json = nlohmann::json;

HttpServer::HttpServer(int port, const std::string& pictureBasePath)
    : port_(port)
    , pictureBasePath_(pictureBasePath)
    , server_(std::make_unique<httplib::Server>())
    , isRunning_(false)
{
    // 初始化头像处理器
    pictureHandler_ = std::make_unique<PictureHandler>(pictureBasePath_);
    
    // 设置路由
    setupRoutes();
    
    std::cout << "HTTP Server initialized on port " << port_ << std::endl;
    std::cout << "Picture storage path: " << pictureBasePath_ << std::endl;
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::setupRoutes() {
    // 设置请求日志（可选）
    server_->set_logger([](const httplib::Request& req, const httplib::Response& res) {
        std::cout << "HTTP " << req.method << " " << req.path << " -> " << res.status << std::endl;
    });
    
    // 头像上传接口 - POST /api/picture/upload
    server_->Post("/api/picture/upload", [this](const httplib::Request& req, httplib::Response& res) {
        handlePictureUpload(req, res);
    });
    
    // 头像下载接口 - GET /api/picture/:id
    server_->Get("/api/picture/(\\d+)", [this](const httplib::Request& req, httplib::Response& res) {
        handlePictureDownload(req, res);
    });
    
    // 直接文件访问接口 - GET /picture/:filename
    server_->Get("/picture/(.*)", [this](const httplib::Request& req, httplib::Response& res) {
        handleFileAccess(req, res);
    });
    
    // 健康检查接口 - GET /health
    server_->Get("/health", [this](const httplib::Request& req, httplib::Response& res) {
        handleHealthCheck(req, res);
    });
    
    // 默认的404处理
    server_->set_error_handler([this](const httplib::Request& req, httplib::Response& res) {
        handleNotFound(req, res);
    });
}

void HttpServer::handlePictureUpload(const httplib::Request& req, httplib::Response& res) {
    // 设置CORS头（如果需要）
    setCorsHeaders(res);
    
    try {
        // 检查Content-Type
        if (req.get_header_value("Content-Type") != "application/json") {
            sendErrorResponse(res, 400, "Content-Type must be application/json");
            return;
        }
        
        // 解析JSON请求体
        auto json_body = json::parse(req.body);
        
        // 验证必需字段
        if (!json_body.contains("id") || !json_body.contains("picture")) {
            sendErrorResponse(res, 400, "Missing required fields: id or picture");
            return;
        }
        
        // 获取用户ID和图片数据
        int id = json_body["id"];
        std::string picture = json_body["picture"];
        
        // 验证数据
        if (picture.empty()) {
            sendErrorResponse(res, 400, "picture cannot be empty");
            return;
        }
        
        // 处理头像上传
        json response;
        if (pictureHandler_->handleUpload(id, picture, response)) {
            res.set_content(response.dump(), "application/json");
        } else {
            res.status = 500;
            res.set_content(response.dump(), "application/json");
        }
        
    } catch (const json::parse_error& e) {
        sendErrorResponse(res, 400, "Invalid JSON format: " + std::string(e.what()));
    } catch (const std::exception& e) {
        std::cerr << "Picture upload error: " << e.what() << std::endl;
        sendErrorResponse(res, 500, "Internal server error");
    }
}

void HttpServer::handlePictureDownload(const httplib::Request& req, httplib::Response& res) {
    setCorsHeaders(res);
    
    try {
        std::string userIdStr = req.matches[1];
        
        // 验证用户ID格式
        if (!isValidUserId(userIdStr)) {
            sendErrorResponse(res, 400, "Invalid user ID format");
            return;
        }
        
        int id = std::stoi(userIdStr);
        
        // 处理头像下载
        json response;
        if (pictureHandler_->handleDownload(id, response)) {
            res.set_content(response.dump(), "application/json");
        } else {
            // 如果头像不存在，返回404
            if (response.contains("message") && response["message"] == "Picture not found") {
                sendErrorResponse(res, 404, "Picture not found for user " + std::to_string(id));
            } else {
                sendErrorResponse(res, 500, "Failed to download picture");
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Picture download error: " << e.what() << std::endl;
        sendErrorResponse(res, 500, "Internal server error");
    }
}

void HttpServer::handleFileAccess(const httplib::Request& req, httplib::Response& res) {
    setCorsHeaders(res);
    
    try {
        std::string filename = req.matches[1];
        
        // 安全检查：防止路径遍历攻击
        if (filename.find("..") != std::string::npos) {
            sendErrorResponse(res, 403, "Access denied");
            return;
        }
        
        std::string filepath = pictureBasePath_ + "/" + filename;
        
        // 检查文件是否存在
        if (!std::filesystem::exists(filepath)) {
            sendErrorResponse(res, 404, "File not found: " + filename);
            return;
        }
        
        // 设置正确的Content-Type
        std::string contentType = "image/png"; // 默认PNG
        if (filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos) {
            contentType = "image/jpeg";
        }
        
        // 直接发送文件内容
        res.set_content(filepath, contentType.c_str());
        
    } catch (const std::exception& e) {
        std::cerr << "File access error: " << e.what() << std::endl;
        sendErrorResponse(res, 500, "Internal server error");
    }
}

void HttpServer::handleHealthCheck(const httplib::Request& req, httplib::Response& res) {
    setCorsHeaders(res);
    
    json healthStatus = {
        {"status", "ok"},
        {"service", "chat_http_server"},
        {"port", port_},
        {"timestamp", std::time(nullptr)}
    };
    
    res.set_content(healthStatus.dump(), "application/json");
}

void HttpServer::handleNotFound(const httplib::Request& req, httplib::Response& res) {
    setCorsHeaders(res);
    
    json errorResponse = {
        {"error", true},
        {"message", "Endpoint not found: " + req.path},
        {"status", 404}
    };
    
    res.status = 404;
    res.set_content(errorResponse.dump(), "application/json");
}

void HttpServer::setCorsHeaders(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

bool HttpServer::isValidUserId(const std::string& userIdStr) const {
    try {
        int userId = std::stoi(userIdStr);
        return userId > 0; // 假设用户ID是正整数
    } catch (...) {
        return false;
    }
}

void HttpServer::sendErrorResponse(httplib::Response& res, int status, const std::string& message) {
    json errorResponse = {
        {"error", true},
        {"message", message},
        {"status", status}
    };
    
    res.status = status;
    res.set_content(errorResponse.dump(), "application/json");
}

void HttpServer::run() {
    if (isRunning_) {
        std::cout << "HTTP Server is already running on port " << port_ << std::endl;
        return;
    }
    
    std::cout << "Starting HTTP Server on port " << port_ << "..." << std::endl;
    
    isRunning_ = true;
    
    // 阻塞调用，直到服务器停止
    if (server_->listen("0.0.0.0", port_)) {
        std::cout << "HTTP Server started successfully on port " << port_ << std::endl;
    } else {
        std::cerr << "Failed to start HTTP Server on port " << port_ << std::endl;
    }
    
    isRunning_ = false;
    std::cout << "HTTP Server stopped" << std::endl;
}

void HttpServer::stop() {
    if (isRunning_ && server_) {
        std::cout << "Stopping HTTP Server..." << std::endl;
        server_->stop();
        isRunning_ = false;
    }
}

bool HttpServer::isRunning() const {
    return isRunning_;
}