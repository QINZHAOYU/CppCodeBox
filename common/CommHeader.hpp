#pragma once


// define dll export macro.
#ifdef DLLEXPORT
#define DLLAPI __declspec(dllexport) 
#else
#define DLLAPI 
#endif


// include common headers.
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
