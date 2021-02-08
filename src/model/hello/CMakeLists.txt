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
include_directories(${PROJECT_SOURCE_DIR}/src )
add_library(hello_lib_shared  SHARED ${src_files})
add_library(hello_lib_static  STATIC ${src_files})

# 设置动态库和静态库的名字
set_target_properties(hello_lib_shared PROPERTIES OUTPUT_NAME "hellodll")
set_target_properties(hello_lib_static PROPERTIES OUTPUT_NAME "hellolib")

# 使动态库和静态库同时存在
set_target_properties(hello_lib_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(hello_lib_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 设置动态库版本
set_target_properties(hello_lib_shared PROPERTIES VERSION 0.1 SOVERSION 1.0)

# 获取父级包含目录
target_include_directories(hello_lib_shared 
    PRIVATE .
    PUBLIC ${CPPCODEBOX_INCLUDE_DIRECTORIES})
target_include_directories(hello_lib_static 
    PRIVATE .
    PUBLIC ${CPPCODEBOX_INCLUDE_DIRECTORIES})    

# 获取当前包含目录
get_target_property(HELLO_INCLUDE_DIRECTORIES hello_lib_static INCLUDE_DIRECTORIES)

#安装动态库和静态库
install(TARGETS hello_lib_shared hello_lib_static
    LIBRARY DESTINATION  /dist/lib
    ARCHIVE DESTINATION  /dist/lib)

# 安装头文件
install(FILES HelloDemo.h  DESTINATION  /dist/inc)

