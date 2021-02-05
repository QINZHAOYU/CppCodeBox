cmake_minimum_required(VERSION 3.1)
set (CMAKE_SYSTEM_VERSION 10.0 (latest installed version))

project(HelloX64)

# 查找当前目录下的所有源文件并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
set(CMAKE_INSTALL_PREFIX  ../../)


# 生成链接库
# add_library (HelloX64 SHARED ${DIR_LIB_SRCS})
ADD_EXECUTABLE(vecTest ${DIR_LIB_SRCS})  

# 指定库的安装路径
install (TARGETS vecTest DESTINATION src/build)
