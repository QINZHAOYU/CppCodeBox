file(GLOB_RECURSE lib_src_cpp ${PROJECT_SOURCE_DIR}/modules/*.cpp)
file(GLOB_RECURSE lib_src_inc ${PROJECT_SOURCE_DIR}/modules/*.hpp)

add_library(Mykits_lib  STATIC  ${lib_src_cpp}  ${lib_src_inc})
