cmake_minimum_required(VERSION 3.1)
project(HelloDemo LANGUAGES CXX)


# 递归获取目录下所有的cpp.h文件
file(GLOB_RECURSE src_files ./*.cpp)
file(GLOB_RECURSE inc_files ./*.h)

# 设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# 生成动态库和静态库
set(target "${PROJECT_NAME}")
add_library(${target}  STATIC ${src_files})

# 获取父级包含目录
target_include_directories(${target} 
    PRIVATE .
    PUBLIC ${CPPCODEBOX_INCLUDE_DIRECTORIES})

# 获取当前包含目录
get_target_property(HELLODEMO_INCLUDE_DIRECTORIES ${target} INCLUDE_DIRECTORIES)

