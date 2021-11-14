# 加载库文件。
file(GLOB_RECURSE test_src_cpp ${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.c)

# 生成测试。
add_executable(Catch2Test ${test_src_cpp})
target_link_libraries(Catch2Test 
    PUBLIC ${EXTRA_LIBS}
    )
target_include_directories(Catch2Test 
    PUBLIC  "${PROJECT_SOURCE_DIR}"
    )


