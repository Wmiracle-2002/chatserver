#ifndef PICTUREHANDLER_H
#define PICTUREHANDLER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <json.hpp>

// 如果没有使用 cpp-base64 库，我们可以自己实现简单的 Base64 编解码
// 或者使用系统提供的 Base64 函数

class PictureHandler {
public:
    explicit PictureHandler(const std::string& pictureBasePath = "../resources");
    
    // 处理头像上传
    bool handleUpload(int userid, const std::string& base64Data, nlohmann::json& response);
    // 处理头像下载
    bool handleDownload(int userid, nlohmann::json& response);
    // 获取头像文件路径
    std::string getPictureFilePath(int userid) const;

private:
    // Base64 解码
    std::string base64_decode(const std::string& encoded_string);
    // Base64 编码
    std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    // 保存头像文件
    bool savePictureFile(int userid, const std::string& imageData);
    // 加载头像文件
    std::string loadPictureFile(int userid);
    // 检查文件是否存在
    bool fileExists(const std::string& path) const;

private:
    std::string pictureBasePath_;  // 头像存储基础路径
};

#endif // !PICTUREHANDLER_H
