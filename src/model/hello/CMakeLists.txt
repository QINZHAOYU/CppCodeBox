# 递归获取目录下所有的cpp.h文件
file(GLOB_RECURSE src_files ./*.cpp)
file(GLOB_RECURSE inc_files ./*.h)

# 生成动态库和静态库
add_library(hello_lib_shared  SHARED ${src_files})
add_library(hello_lib_static  STATIC ${src_files})

# 设置动态库和静态库的名字
set_target_properties(hello_lib_shared PROPERTIES OUTPUT_NAME "hellodll")
set_target_properties(hello_lib_static PROPERTIES OUTPUT_NAME "hellolib")

#设置动态库版本
set_target_properties(hello_lib_shared PROPERTIES VERSION 0.1)

#安装动态库和静态库
install(TARGETS hello_lib_shared hello_lib_static
    LIBRARY DESTINATION  /dist/lib
    ARCHIVE DESTINATION  /dist/lib)

#安装头文件
install(FILES HelloDemo.h  DESTINATION  /dist/inc)