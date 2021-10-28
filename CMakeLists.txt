# 设置cmake版本要求。
cmake_minimum_required(VERSION 3.10)
set(ProjectName CppKits)

# 设置平台类型。
if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif (CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif (CMAKE_HOST_UNIX)
    set(LINUX 1)
endif ()

# 设置项目名称及c++版本要求。
project(${ProjectName} VERSION 21.10.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


message(STATUS "-----------------------------------------------------------------")
message(STATUS "------------------------ ${ProjectName} ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} ------------------------")
message(STATUS "-----------------------------------------------------------------")


# 设置安装路径。
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})

# 生成版本信息。
if(MSVC)
    set(VERSIONINFO_RC "${PROJECT_BINARY_DIR}/VersionInfo.rc")
    configure_file("${PROJECT_SOURCE_DIR}/modules/rc/VersionInfo.rc.in"
                   "${VERSIONINFO_RC}"
                   )
endif()

# 选择是否生成动态库。
option(GEN_DLL "If generate dynamic link library" ON)
message(STATUS "----------------------- If generate dynamic link library: ${GEN_DLL}")
if (GEN_DLL)
    add_definitions(-D DLLEXPORT)
endif()

# 添加内部库。
add_subdirectory(modules)
list(APPEND EXTRA_LIBS MyCppKits) 

# 选择是否执行测试。
option(RUN_TEST "If run catch test" ON)
message(STATUS "----------------------- If run catch test: ${RUN_TEST}")
if (RUN_TEST)
    add_subdirectory(tests)
    set(TESTNAME Catch2Test)
endif()

# 添加外部库。
set(MyLibPath $ENV{CPPLIBS}/mylibs/lib)
find_library(MY_LIBS NAMES MathSqrt  PATHS ${MyLibPath})
if (NOT MY_LIBS)
    message(FATAL "----------------------- MY_LIBS (MathSqrt) not found")
endif ()

# 加载项目文件。
file(GLOB driver "main.cpp")
file(GLOB src_inc ${PROJECT_SOURCE_DIR}/common/*.hpp ${PROJECT_SOURCE_DIR}/common/*.h) 
file(GLOB src_cpp ${PROJECT_SOURCE_DIR}/common/*.cpp ${PROJECT_SOURCE_DIR}/common/*.c) 

# 生成可执行文件。
add_executable(${ProjectName} ${driver} ${src_inc} ${src_cpp} ${VERSIONINFO_RC})
target_link_libraries(${ProjectName} 
    PUBLIC ${EXTRA_LIBS}
    PUBLIC ${MY_LIBS}
    )
if (LINUX)
    target_link_libraries(${ProjectName}
    PUBLIC dl
    )
endif ()

# 为项目添加附加包含目录。
target_include_directories(${ProjectName} 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
    )

# 安装可执行程序。
install(TARGETS  ${PROJECT_NAME}  
    DESTINATION  dists
    )
install(FILES "${PROJECT_SOURCE_DIR}/License.txt"
    DESTINATION  dists
    )

# 加载测试。
enable_testing()
if (WINDOWS)
    set(TESTPATH ${PROJECT_BINARY_DIR}/tests/Release)
elseif (LINUX)
    set(TESTPATH ${PROJECT_BINARY_DIR}/tests/)
endif ()
add_test(  
    NAME ${TESTNAME}
    COMMAND ${TESTPATH}/${TESTNAME} --reporter compact --success
)