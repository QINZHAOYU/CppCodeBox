#ifndef COMM_HEADER_H
#define COMM_HEADER_H


#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <memory>

using std::vector;
using std::array;
using std::map;
using std::string;
using std::shared_ptr;

using VecDbl     =  vector<double>;
using VecVecDbl  =  vector<vector<double>>;
using VecInt     =  vector<int>;
using VecVecInt  =  vector<vector<int>>;
using VecStr     =  vector<string>;
using VecVecStr  =  vector<vector<string>>;
using MapStrStr  =  map<string, string>;
using MapStrDbl  =  map<string, double>;
using MapStrVec  =  map<string, VecDbl>;


#endif