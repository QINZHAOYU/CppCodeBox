cmake_minimum_required(VERSION 3.1)
project(CppCodeBox VERSION 0.1)

message("--------------------------------------------------")
message("---*--- CppCodeBox ver: ${PROJECT_VERSION} ---*---")
message("--------------------------------------------------")

if (NOT (CMAKE_SIZEOF_VOID_P EQUAL 8))
    message("x86 are not supported, turn to x64.")
    set(BUILD_USE_64BITS on)
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(SINGLE_EXE  "compile to a single executables" ON)
option(ENABLE_HELLO "add HelloDemo to CppCodeBox"  ON)

set(src_dir  "src")
set(core_dir ${src_dir}/core)
set(common_dir ${src_dir}/common)
set(model_dir ${src_dir}/model)
set(hello_dir ${model_dir}/hello)
set(include_dir ${CMAKE_SOURCE_DIR}/tools)

file(GLOB driver "main.cpp")
file(GLOB_RECURSE src_files ${core_dir}/*.cpp ${common_dir}/*.cpp)
file(GLOB_RECURSE inc_files ${core_dir}/*.h   ${common_dir}/*.h)

set(target "${PROJECT_NAME}")
add_executable(${target} ${dirvier} ${src_files} ${inc_files})
set_target_properties(${target}
    PROPERTIES LINKER_LANGUAGE CXX
)

target_include_directories(${target} PUBLIC
    ${src_dir}
    ${include_dir}
    ${model_dir}
)

add_compile_definitions(
    CCB_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    CCB_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    CCB_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    ENABLE_HELLO=${ENABLE_HELLO}
)

if (ENABLE_HELLO)
    add_subdirectory(${hello_dir})
endif()

if (ENABLE_HELLO)
    target_link_directories(${target} PUBLIC hello)
endif()

target_link_libraries(${target} PUBLIC
    ${PLATFORM_LIBRARIES}
)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG   "-Og -D_DEBUG -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()