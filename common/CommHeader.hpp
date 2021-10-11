#pragma once


// define WINDOWS.
#undef WINDOWS
#ifdef _WIN32
    #define WINDOWS
#elif __WIN32__
    #define WINDOWS
#endif


// define LINUX.
#undef LINUX
#ifdef __linux__
    #define LINUX
#endif


// define dll export macro.
#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC 
#endif 

#ifdef DLLEXPORT
    #define DLLAPI EXTERNC __declspec(dllexport) 
#else
    #define DLLAPI EXTERNC
#endif


// include common headers.
#ifdef WINDOWS
    #include <windows.h>
#elif LINUX
    #include <dlfcn.h>
#endif

#include <vector>
#include <array>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>

using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::array;
using std::tuple;
using std::unordered_map;
using std::unordered_set;
using std::stringstream;

using std::shared_ptr;
using std::unique_ptr;

using namespace std::chrono;
