# 设置cmake版本要求。
cmake_minimum_required(VERSION 3.10)
set(ProjectName cppkits)
message(STATUS "---------- project build started.")

# 设置项目名称及c++版本要求。
project(${ProjectName})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置安装路径。
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})

# 添加外部库。
# find_package(Poco 6.3 REQUIRED)
# include_directories(${Poco_INCLUDE_DIRS})
# link_directories(${Poco_LIBRARY_DIRS})
# add_definitions(${Poco_DEFINITIONS})

# 选择是否生成动态库。
option(GEN_DLL "generate dll" ON)
if (GEN_DLL)
    add_definitions(-D DLLEXPORT)
endif()

# 添加内部库。
add_subdirectory(modules)
list(APPEND EXTRA_LIBS MyCppkits) 

# 加载项目文件。
file(GLOB driver "main.cpp")
file(GLOB src_inc ${PROJECT_SOURCE_DIR}/common/*.hpp ${PROJECT_SOURCE_DIR}/common/*.h) 
file(GLOB src_cpp ${PROJECT_SOURCE_DIR}/common/*.cpp ${PROJECT_SOURCE_DIR}/common/*.c) 

# 生成可执行文件。
add_executable(${ProjectName} ${driver} ${src_inc} ${src_cpp})
target_link_libraries(${ProjectName} PUBLIC ${EXTRA_LIBS})

# 为项目添加附加包含目录。
target_include_directories(${ProjectName} 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
)

# 安装可执行程序。
install(TARGETS  ${PROJECT_NAME}  DESTINATION  dists)

