# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (CmakeTest)

# 项目加载路径
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})

# 查找当前目录下的所有源文件并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 包含头文件库目录，用于调用其它目录下的头文件
include_directories(./src/model/hello)

# 显式添加子目录，这样子目录下的 CMakeLists.txt 文件和源代码也会被处理 
add_subdirectory(./src/model/hello)

# # 指定生成目标
# add_executable(CmakeApp main.cpp)

# # 添加链接库
# target_link_libraries(Hello HelloLib)

# # 指定安装路径
# install (TARGETS Hello DESTINATION bin)

# # 自定义自动测试
# # 定义一个宏，用来简化测试工作
# macro (do_test arg1 arg2 result)
# add_test (test_${arg1}_${arg2} Demo ${arg1} ${arg2})
# set_tests_properties (test_${arg1}_${arg2} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endmacro (do_test)

# # 使用该宏进行一系列的数据测试
# do_test (10 12 "Hello word!")
# do_test (itworked yes "This is HelloFun() lib api, input: ")