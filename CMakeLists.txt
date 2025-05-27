cmake_minimum_required(VERSION 3.20)
project(cnnx VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/shared)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library/runtime)


# 添加头文件目录（假设头文件在 include 文件夹）
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/include)

add_library(cnnx_static STATIC main.cpp)
target_link_libraries(cnnx_static PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party/library/openblas/libopenblas.a
)
set_target_properties(cnnx_static PROPERTIES OUTPUT_NAME "cnnx_static")

add_executable(cnnx main.cpp)
target_link_libraries(cnnx PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party/library/openblas/libopenblas.a
)