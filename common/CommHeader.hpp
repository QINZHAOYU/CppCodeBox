#pragma once


//===========================================================================
// determine compilation platform and export library.
//===========================================================================
// define WINDOWS.
#undef WINDOWS
#if defined(_WIN32)
#define WINDOWS
#elif defined(__WIN32__)
#define WINDOWS
#endif

// define LINUX.
#undef LINUX
#if defined(__linux__)
#define LINUX
#endif

// define dll export macro.
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif
#ifdef DLLEXPORT
#ifdef WINDOWS
#define DLLAPI EXTERNC __declspec(dllexport)
#elif defined(LINUX)
#define DLLAPI EXTERNC
#endif
#else
#define DLLAPI EXTERNC
#endif

// include common headers.
#if defined(WINDOWS)
#include <windows.h>
#elif defined(LINUX)
#include <dlfcn.h>
#include <stdexcept>
#endif


//===========================================================================
// define commonly used marcos.
//===========================================================================
#define _LOCA  string(", ") + string(__FILE__) + \
	string("(")  + std::to_string(__LINE__) + string(")\n")


//===========================================================================
// include built-in types headers and expose common objects.
//===========================================================================
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
