cmake_minimum_required(VERSION 3.10)

project(test VERSION 1.0)

# 设置安装位置。
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})

# 设置c++编译标准。
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加库。
add_subdirectory(modules)
list(APPEND EXTRA_LIBS MyLibs) 

# 生成可执行文件。
file(GLOB driver "main.cpp" "CommHeaders.hpp")
add_executable(test ${driver} ${src_cpp} ${src_inc})

# 链接库。
target_link_libraries(test PUBLIC ${EXTRA_LIBS})

# 为项目添加附加包含目录。
target_include_directories(test 
PUBLIC  "${PROJECT_SOURCE_DIR}"
)



