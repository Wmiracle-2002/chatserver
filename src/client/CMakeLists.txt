# 设置自动处理Qt的工具
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH "E:/Qt/6.8.3/msvc2022_64")
else()
    # 本地 Linux 编译
    set(CMAKE_PREFIX_PATH "/home/wmiracle/Qt/6.8.3/gcc_64")
endif()

# 查找Qt包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# 设置翻译源文件
set(TS_FILES QChat_zh_CN.ts)

# 设置项目头文件
set(H_FILES
    ../../include/client/clientcore.h
    ../../include/client/loginwidget.h
    ../../include/client/registerwidget.h
    ../../include/client/mainchatwindow.h
)

# 设置项目ui文件
set(UI_FILES
    loginwidget.ui
    registerwidget.ui
    mainchatwindow.ui
)

# 设置项目源文件
set(PROJECT_SOURCES
    main.cpp
    loginwidget.cpp
    clientcore.cpp
    registerwidget.cpp
    mainchatwindow.cpp
    ${UI_FILES}
    ${H_FILES}
    ${TS_FILES}
)

# 根据Qt版本创建可执行目标
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6) # >=Qt6
    qt_add_executable(QChat MANUAL_FINALIZATION ${PROJECT_SOURCES})
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()  # <Qt6
    if(ANDROID)
        add_library(QChat SHARED ${PROJECT_SOURCES})
    else()
        add_executable(QChat ${PROJECT_SOURCES})
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 跨平台特定配置
if(WIN32)
    # Windows特定设置
    set_target_properties(QChat PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    # 设置图标（如果有）
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/qchat_icon.ico")
        target_sources(QChat PRIVATE resources/qchat_icon.ico)
        set_target_properties(QChat PROPERTIES
            RC_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/resources/qchat_icon.ico"
        )
    endif()
elseif(APPLE)
    # macOS特定设置
    set_target_properties(QChat PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.mycompany.qchat"
        MACOSX_BUNDLE_BUNDLE_NAME "QChat"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    )
    # 设置图标（如果有）
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/qchat_icon.icns")
        set_source_files_properties(resources/qchat_icon.icns PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
        )
        target_sources(MyApp PRIVATE resources/qchat_icon.icns)
    endif()
endif()

# 链接Qt库
# target_link_libraries(QChat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets pthread)

# 链接Qt库 - 确保包含Network
target_link_libraries(QChat PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets pthread
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Network  # 关键：添加Network模块
    pthread
)

# 设置目标属性
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QChat)
endif()
set_target_properties(QChat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装配置
include(GNUInstallDirs)
install(TARGETS QChat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6的最终化处理
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QChat)
endif()

# # 定义了一个SRC_LIST变量，包含了该目录下所有源文件
# aux_source_directory(./qt SRC_LIST)

# # 指定生成可执行文件
# add_executable(ChatClient ${SRC_LIST})

# # 指定可执行文件链接时需要依赖的库文件
# target_link_libraries(ChatClient pthread)