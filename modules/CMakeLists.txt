set(LibName MyCppkits)

# 加载库文件。
file(GLOB_RECURSE lib_src_cpp ${PROJECT_SOURCE_DIR}/modules/*.cpp ${PROJECT_SOURCE_DIR}/modules/*.c)
file(GLOB_RECURSE lib_src_inc ${PROJECT_SOURCE_DIR}/modules/*.hpp ${PROJECT_SOURCE_DIR}/modules/*.h)

# 生成静态库
add_library(${LibName}  STATIC  ${lib_src_cpp}  ${lib_src_inc})

# 生成动态库。
if (${GEN_DLL})
    message(STATUS "---------- dll generation option: ${GEN_DLL}")
    add_library(${LibName}_dll SHARED ${lib_src_cpp} ${lib_src_inc})
    target_include_directories(${LibName}_dll 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
    )

    # 设置动态库版本号及输出名。
    set_target_properties(${LibName}_dll PROPERTIES 
        VERSION 1.0.0 
        SOVERSION 1.0
    )
    set_target_properties(${LibName}_dll PROPERTIES 
        OUTPUT_NAME ${LibName}
    )    
endif()

# 添加搜索路径。
target_include_directories(${LibName} 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
)

# 安装动态库和头文件。
if (${GEN_DLL}) 
    install(TARGETS ${LibName}_dll
        RUNTIME DESTINATION dists/lib64
    )
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/modules 
        DESTINATION dists/inc
        FILES_MATCHING 
        PATTERN  "*.hpp"
        PATTERN  "*.h"
        PATTERN ".cpp" EXCLUDE
        PATTERN ".c"   EXCLUDE
    )   
endif()