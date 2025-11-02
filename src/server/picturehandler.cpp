#include "picturehandler.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "public.hpp"

// Base64 编码表
static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// 检查字符是否为Base64有效字符
static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

PictureHandler::PictureHandler(const std::string& pictureBasePath) 
    : pictureBasePath_(pictureBasePath) {
    // 确保头像目录存在
    std::filesystem::create_directories(pictureBasePath_);
    std::cout << "PictureHandler initialized with path: " << pictureBasePath_ << std::endl;
}

bool PictureHandler::handleUpload(int userid, const std::string& base64Data, nlohmann::json& response) {
    try {
        if (base64Data.empty()) {
            response = {
                {"success", false},
                {"message", "Empty image data"}
            };
            return false;
        }
        
        // 解码Base64数据
        std::string imageData = base64_decode(base64Data);
        
        if (imageData.empty()) {
            response = {
                {"success", false},
                {"message", "Failed to decode base64 data"}
            };
            return false;
        }
        
        // 保存头像文件
        if (savePictureFile(userid, imageData)) {
            std::string picturePath = "resources/" + std::to_string(userid) + ".png";
            
            response = {
                {"success", true},
                {"picture_path", picturePath},
                {"message", "Picture uploaded successfully"},
                {"file_size", imageData.size()}
            };
            
            std::cout << "Picture uploaded for user " << userid 
                      << " (" << imageData.size() << " bytes)" << std::endl;
            return true;
        } else {
            response = {
                {"success", false},
                {"message", "Failed to save picture file"}
            };
            return false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Picture upload error for user " << userid << ": " << e.what() << std::endl;
        response = {
            {"success", false},
            {"message", "Internal server error: " + std::string(e.what())}
        };
        return false;
    }
}

bool PictureHandler::handleDownload(int userid, nlohmann::json& response) {
    try {
        std::string imageData = loadPictureFile(userid);
        
        if (imageData.empty()) {
            response = {
                {"success", false},
                {"message", "Picture not found"}
            };
            return false;
        }
        
        // 编码为Base64
        std::string base64Data = base64_encode(
            reinterpret_cast<const unsigned char*>(imageData.c_str()), 
            imageData.size()
        );
        
        response = {
            {"success", true},
            {"id", userid},
            {"picture", base64Data},
            {"picture_path", getPictureFilePath(userid)},
            {"file_size", imageData.size()}
        };
        
        std::cout << "Picture downloaded for user " << userid 
                  << " (" << imageData.size() << " bytes)" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Picture download error for user " << userid << ": " << e.what() << std::endl;
        response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return false;
    }
}

std::string PictureHandler::getPictureFilePath(int userid) const {
    return pictureBasePath_ + "/" + std::to_string(userid) + ".png";
}

std::string PictureHandler::base64_decode(const std::string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string PictureHandler::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

bool PictureHandler::savePictureFile(int userid, const std::string& imageData) {
    std::string filePath = getPictureFilePath(userid);
    
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filePath << std::endl;
        return false;
    }
    
    file.write(imageData.c_str(), imageData.size());
    file.close();
    
    return true;
}

std::string PictureHandler::loadPictureFile(int userid) {
    std::string filePath = getPictureFilePath(userid);
    
    if (!fileExists(filePath)) {
        return "";
    }
    
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return "";
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::string imageData(size, '\0');
    if (!file.read(&imageData[0], size)) {
        return "";
    }
    
    return imageData;
}

bool PictureHandler::fileExists(const std::string& path) const {
    return std::filesystem::exists(path);
}