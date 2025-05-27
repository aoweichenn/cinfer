cmake_minimum_required(VERSION 3.20)
project(cnnx VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/shared)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/runtime)


# 添加头文件目录（假设头文件在 include 文件夹）
include_directories(include)

add_library(cnnx_static STATIC main.cpp)
set_target_properties(cnnx_static PROPERTIES OUTPUT_NAME "cnnx_static")


add_library(cnnx_shared SHARED main.cpp)
set_target_properties(cnnx_shared PROPERTIES
        OUTPUT_NAME "cnnx_shared"
        VERSION 1.0.0
        SOVERSION 1
)

add_executable(cnnx main.cpp)

## 链接库到可执行文件（这里以静态库为例）
#target_link_libraries(myapp PRIVATE
#        mylib_static          # 链接静态库
#        # 或链接动态库：mylib_shared
#)
#
## --------------------------------------------
## 安装规则（可选，执行 make install 时生效）
## --------------------------------------------
#install(TARGETS mylib_static mylib_shared myapp
#        ARCHIVE DESTINATION lib   # 静态库安装路径
#        LIBRARY DESTINATION lib   # 动态库安装路径
#        RUNTIME DESTINATION bin   # 可执行文件安装路径
#)
#
## 安装头文件（可选）
#install(DIRECTORY include/ DESTINATION include)