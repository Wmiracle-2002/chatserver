# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本
project(QChat VERSION 0.1 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置配置扩展
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 配置编译选项
set(CMake_CXX_FLAGS ${CMake_CXX_FLAGS} -g)

# 配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 条件编译控制
option(BUILD_SERVER "Build server component" ON)
option(BUILD_CLIENT "Build client component" ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/common)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

# 平台特定的构建
if(UNIX)
    # Linux平台：主要构建服务器，可选构建客户端
    if(BUILD_SERVER)
        include_directories(${PROJECT_SOURCE_DIR}/include/server)
        include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
        include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
        include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
    endif()
    
    if(BUILD_CLIENT)
        include_directories(${PROJECT_SOURCE_DIR}/include/client)
    endif()
    
elseif(WIN32)
    # Windows平台：只构建客户端
    if(BUILD_CLIENT)
        include_directories(${PROJECT_SOURCE_DIR}/include/client)
    endif()
endif()

# # 配置头文件的搜索路径
# include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${PROJECT_SOURCE_DIR}/include/server)
# include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
# include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
# include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
# include_directories(${PROJECT_SOURCE_DIR}/include/client)
# include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

# 加载子目录
add_subdirectory(src)