cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(CppCodeBox VERSION 1.0)


message("--------------------------------------------------")
message("---*--- CppCodeBox ver: ${PROJECT_VERSION} ---*---")
message("--------------------------------------------------")


# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built to
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
# wether to build shared libraries
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only
configure_file(CppCodeBoxConfig.h.in CppCodeBoxConfig.h)

# wether add optional sublibrary
option(USE_HELLO "use hello module" ON)

# optional sublibrary
if(USE_HELLO)
  # add subdirectory to the project building
  add_subdirectory(src/model/hello) 
  # collect optional libraries that needed to the running program
  list(APPEND EXTRA_LIBS HelloLib) 
endif()

# add the executable
add_executable(CppCodeBox CppCodeBox.cpp)
target_link_libraries(CppCodeBox PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
target_include_directories(CppCodeBox PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src"
)

# add the install targets
set(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR})
install(TARGETS CppCodeBox DESTINATION bin/hello)
install(FILES "${PROJECT_BINARY_DIR}/CppCodeBoxConfig.h"
  DESTINATION bin/hello
)

# enable testing
include(CTest)

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME CASE${arg} COMMAND ${target} ${arg})
  set_tests_properties(CASE${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
  )
endfunction(do_test)

# do a bunch of result based tests
do_test(CppCodeBox 4 "4 is 2")
do_test(CppCodeBox 9 "9 is 3")

# provide both binary and source distributions, and license 
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_SOURCE_DIR}/dist)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/NOTICE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${CppCodeBox_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CppCodeBox_VERSION_MINOR}")
set(CPACK_SOURCE_IGNORE_FILES
  ${PROJECT_SOURCE_DIR}/build
  ${PROJECT_SOURCE_DIR}/dist 
  ${PROJECT_SOURCE_DIR}/.git
  ${PROJECT_SOURCE_DIR}/.gitignore 
)
include(CPack)