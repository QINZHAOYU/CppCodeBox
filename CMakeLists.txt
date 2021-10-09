# 设置cmake版本要求。
cmake_minimum_required(VERSION 3.10)
set(ProjectName cppkits)

# 设置项目名称及c++版本要求。
add_compile_options(-std=c++11)
project(${ProjectName})

# 设置安装路径。
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})

# 添加外部库。
# find_package(PCL 1.8 REQUIRED)
# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})
# add_definitions(${PCL_DEFINITIONS})

# 添加内部库。
add_subdirectory(modules)
list(APPEND EXTRA_LIBS Mykits_lib) 

# 加载项目文件。
file(GLOB driver "main.cpp")
file(GLOB src_inc ${PROJECT_SOURCE_DIR}/common/*.hpp) 
file(GLOB src_cpp ${PROJECT_SOURCE_DIR}/common/*.cpp) 

# 选择是否生成动态库。
option(GEN_DLL "generate dll" ON)

# 生成动态库。
if (GEN_DLL)
    add_library(${ProjectName}_lib SHARED ${driver} ${src_inc} ${src_cpp})
    target_link_libraries(${ProjectName}_lib PUBLIC ${EXTRA_LIBS})

    # 设置动态库版本号及输出名。
    set_target_properties(${ProjectName}_lib PROPERTIES 
        VERSION 1.0.0 
        SOVERSION 1.0
    )
    set_target_properties(${ProjectName}_lib PROPERTIES 
        OUTPUT_NAME ${ProjectName}
    )
endif()

# 生成可执行文件。
add_executable(${ProjectName} ${driver} ${src_inc} ${src_cpp})
target_link_libraries(${ProjectName} PUBLIC ${EXTRA_LIBS})

# 为项目添加附加包含目录。
target_include_directories(${ProjectName} 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
)

# 安装动态库和头文件。
if (GEN_DLL) 
    install(TARGETS ${PROJECT_NAME}_lib
        RUNTIME DESTINATION dists/lib64
    )
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/modules 
        DESTINATION dists/inc
        FILES_MATCHING PATTERN  "*.hpp"
        PATTERN ".cpp" EXCLUDE
    )   
endif()

# 安装可执行程序。
install(TARGETS  ${PROJECT_NAME}  DESTINATION  dists)

